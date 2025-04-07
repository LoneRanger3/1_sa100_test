/*
 * h264_decoder_test.c - test h264 decoder omx component.
 *
 * Copyright (C) 2016-2018, LomboTech Co.Ltd.
 * Author: lomboswer <lomboswer@lombotech.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
#define DBG_LEVEL		DBG_WARNING /* DBG_INFO */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include <oscl.h>
#include <base_component.h>
#include <omx_vendor_lb.h>
#include <lb_omx_core.h>
#include <omx_test.h>
#include "vrender_component.h"
#include "h264_decoder_component.h"

#define H264_INPUT_BUFFER_CNT	2
#define H264_OUTPUT_BUFFER_CNT	5

#define H264_DEFAULT_STREAM_SIZE	(128 * 1024)

/* H264 parser states */
enum h264_parser_state {
	H264_PARSER_NO_CODE,
	H264_PARSER_CODE_0x1,
	H264_PARSER_CODE_0x2,
	H264_PARSER_CODE_0x3,
	H264_PARSER_CODE_1x1,
	H264_PARSER_CODE_SLICE,
};

/* H264 recent tag type */
enum h264_tag_type {
	H264_TAG_HEAD,
	H264_TAG_SLICE,
};


/* Parser context */
struct h264_parser_context {
	int state; /* cur parse state */
	int last_tag;
	char bytes[6];
	int main_count; /* parse slice count */
	int headers_count; /* parse header count */
	int tmp_code_start;
	int code_start;
	int code_end;
	char got_start; /* have got a start code */
	char got_end;
	char seek_end;
	int short_header;
};


typedef struct h264_decoder_test {
	OMX_COMPONENTTYPE *decoder; /* h264dec component handle */
	OMX_COMPONENTTYPE *render; /* video render component */

	vdisp_para_t disp_para; /* display params */

	sem_t sem_event;
	sem_t sem_eos;

	OMX_S32 vout_buf_num; /* video output buf num */
	OMX_S32 vin_buf_num; /* video input buf num */
	OMX_BUFFERHEADERTYPE **vin_buffer; /* video input buffer array */
	OMX_BUFFERHEADERTYPE **vout_buffer; /* video outpout buffer array */
	OMX_PARAM_PORTDEFINITIONTYPE outport_def;
	int stat;

	FILE *infile; /* input file */
	int width, height;
	int file_len;
	int cur_offset;
	char *file_buffer;
} h264_decoder_test_t;

static h264_decoder_test_t decoder_priv;
static OMX_U32 error_value = 0u;

static OMX_ERRORTYPE get_screen_size(disp_size_t *size)
{
	void *hdl = NULL;

	oscl_param_check((size != NULL), OMX_ErrorBadParameter, NULL);
	hdl = oscl_open_disp_engine();
	if (hdl) {
		oscl_disp_get_screen_size(hdl, size);
		oscl_close_disp_engine(hdl);
	} else {
		return OMX_ErrorResourcesLost;
	}

	return OMX_ErrorNone;
}

/*
in: input buffer
in_size: input buffer size
out: output buffer
out_size: output buffer size
consumed: buffer size consumed in this func
frame_size: size of the stream of one frame for this parse
get_head: if we only get the header of the stream
*/
static int parse_h264_stream(
	struct h264_parser_context *ctx,
	char *in, int in_size, unsigned char *out, int out_size,
	int *consumed, int *frame_size, char get_head)
{
	char *in_orig;
	char tmp;
	char frame_finished;
	int frame_length;

	in_orig = in;
	*consumed = 0;
	frame_finished = 0;

	while (in_size-- > 0) {
		switch (ctx->state) {
		case H264_PARSER_NO_CODE:
			if (*in == 0x0) {
				ctx->state = H264_PARSER_CODE_0x1;
				ctx->tmp_code_start = *consumed;
			}
			break;
		case H264_PARSER_CODE_0x1:
			if (*in == 0x0)
				ctx->state = H264_PARSER_CODE_0x2;
			else
				ctx->state = H264_PARSER_NO_CODE;
			break;
		case H264_PARSER_CODE_0x2:
			if (*in == 0x1) {
				/*出现000001，表明是一个NALU开始*/
				ctx->state = H264_PARSER_CODE_1x1;
			} else if (*in == 0x0) {
				ctx->state = H264_PARSER_CODE_0x3;
			} else {
				ctx->state = H264_PARSER_NO_CODE;
			}
			break;
		case H264_PARSER_CODE_0x3:
			if (*in == 0x1)
				/* get start of a NALU */
				ctx->state = H264_PARSER_CODE_1x1;
			else if (*in == 0x0)
				ctx->tmp_code_start++;
			else
				ctx->state = H264_PARSER_NO_CODE;
			break;
		case H264_PARSER_CODE_1x1:
			/* get the NALU type */
			tmp = *in & 0x1F;

			if (tmp == 1 || tmp == 5) {
				/* type = 1 or 5, it's a slice */
				ctx->state = H264_PARSER_CODE_SLICE;
			} else if (tmp == 6 || tmp == 7 || tmp == 8) {
				/* type = 6,7,8，NALU are params */
				ctx->state = H264_PARSER_NO_CODE;
				ctx->last_tag = H264_TAG_HEAD;
				ctx->headers_count++;
			} else
				ctx->state = H264_PARSER_NO_CODE;
			break;
		case H264_PARSER_CODE_SLICE:
			if ((*in & 0x80) == 0x80) {
				/* NALU R bit is not 0，so it can't be drop */
				ctx->main_count++;
				ctx->last_tag = H264_TAG_SLICE;
			}
			ctx->state = H264_PARSER_NO_CODE;
			break;
		}
		/* get a head and a slice, then we can end the search */
		if (get_head == 1 && ctx->headers_count >= 1 &&
				ctx->main_count == 1) {
			ctx->code_end = ctx->tmp_code_start;
			ctx->got_end = 1;
			OSCL_LOGI("got head end code_end %d\n", ctx->code_end);
			break;
		}

		if (ctx->got_start == 0 && ctx->headers_count == 1 &&
				ctx->main_count == 0) {
			ctx->code_start = ctx->tmp_code_start;
			ctx->got_start = 1;
			OSCL_LOGI("got head code_start %d\n", ctx->code_start);
		}

		if (ctx->got_start == 0 && ctx->headers_count == 0 &&
				ctx->main_count == 1) {
			ctx->code_start = ctx->tmp_code_start;
			ctx->got_start = 1;
			ctx->seek_end = 1;
			ctx->headers_count = 0;
			ctx->main_count = 0;
			OSCL_LOGI("got slice code_start %d\n", ctx->code_start);
		}

		if (ctx->seek_end == 0 && ctx->headers_count > 0 &&
				ctx->main_count == 1) {
			ctx->seek_end = 1;
			ctx->headers_count = 0;
			ctx->main_count = 0;
			OSCL_LOGI("seek_end code_end %d\n", ctx->code_end);
		}

		if (ctx->seek_end == 1 && (ctx->headers_count > 0 ||
				ctx->main_count > 0)) {
			ctx->code_end = ctx->tmp_code_start;
			ctx->got_end = 1;
			if (ctx->headers_count == 0)
				ctx->seek_end = 1;
			else
				ctx->seek_end = 0;
			break;
			OSCL_LOGI("headers count %d, mani cnt %d\n",
				ctx->headers_count, ctx->main_count);
		}

		in++;
		(*consumed)++;
	}

	OSCL_LOGI("ctx->got_end %d, consumed %d\n", ctx->got_end, *consumed);
	*frame_size = 0;

	if (ctx->got_end == 1)
		frame_length = ctx->code_end;
	else
		frame_length = *consumed;


	if (ctx->code_start >= 0) {
		frame_length -= ctx->code_start;
		in = in_orig + ctx->code_start;
	} else {
		OSCL_LOGI("ctx->code_start %d\n", ctx->code_start);
		memcpy(out, ctx->bytes, -ctx->code_start);
		*frame_size += -ctx->code_start;
		out += -ctx->code_start;
		in_size -= -ctx->code_start;
		in = in_orig;
	}

	if (ctx->got_start) {
		if (out_size < frame_length) {
			OSCL_LOGE("Output buffer too small for current frame");
			return 0;
		}
		OSCL_LOGI("copy frame length %d\n", frame_length);
		memcpy(out, in, frame_length);
		*frame_size += frame_length;

		if (ctx->got_end) {
			ctx->code_start = ctx->code_end - *consumed;
			ctx->got_start = 1;
			ctx->got_end = 0;
			frame_finished = 1;
			if (ctx->last_tag == H264_TAG_SLICE) {
				ctx->seek_end = 1;
				ctx->main_count = 0;
				ctx->headers_count = 0;
			} else {
				ctx->seek_end = 0;
				ctx->main_count = 0;
				ctx->headers_count = 1;
			}
			OSCL_LOGI("got end copy bytes %d\n", *consumed - ctx->code_end);
			memcpy(ctx->bytes, in_orig + ctx->code_end,
				*consumed - ctx->code_end);
		} else {
			OSCL_LOGI("not end set code_start 0\n");
			ctx->code_start = 0;
			frame_finished = 0;
		}
	}

	ctx->tmp_code_start -= *consumed;

	return frame_finished;
}


static int parse_one_frame(h264_decoder_test_t *priv,
		unsigned char *buffer, unsigned int *size, char get_head)
{
	int ret;
	int consumed, frame_size, rlen;
	struct h264_parser_context parse_ctx;

	fseek(priv->infile, priv->cur_offset, SEEK_SET);
	rlen = fread(priv->file_buffer, 1, H264_DEFAULT_STREAM_SIZE, priv->infile);
	if (rlen <= 0) {
		OSCL_LOGE("read file error!\n");
		return -1;
	}
	/* parse one frame */
	memset(&parse_ctx, 0, sizeof(parse_ctx));
	ret = parse_h264_stream(&parse_ctx, priv->file_buffer, rlen,
		buffer, H264_DEFAULT_STREAM_SIZE, &consumed, &frame_size, get_head);
	if (ret == 0) {
		OSCL_LOGE("Parser has extracted all frames\n");
		return -1;
	}
	priv->cur_offset += frame_size;
	*size = frame_size;

	return 0;
}

static int alloc_output_buffer(h264_decoder_test_t *priv, int size)
{
	int i, j;
	int ret = OMX_ErrorNone;

	/* alloc output buffer header pointer */
	priv->vout_buf_num = H264_OUTPUT_BUFFER_CNT;
	priv->vout_buffer = oscl_zalloc(priv->vout_buf_num *
				sizeof(OMX_BUFFERHEADERTYPE *));
	if (!priv->vout_buffer) {
		OSCL_LOGE("alloc vout_buffer error!\n");
		return -1;
	}

	for (i = 0; i < priv->vout_buf_num; i++) {
		/* alloc the actual buffer */
		ret = OMX_AllocateBuffer(priv->decoder, &priv->vout_buffer[i],
			BASE_OUTPUT_PORT, priv, size);
		if (ret != OMX_ErrorNone) {
			OSCL_LOGE("omx use buffer %d error\n", i);
			goto error;
		}
		/* queue the buffer in the output port */
		ret = OMX_FillThisBuffer(priv->decoder, priv->vout_buffer[i]);
		if (ret != OMX_ErrorNone) {
			OSCL_LOGE("omx fill buffer %d error\n", i);
			goto error;
		}
	}

	return 0;
error:
	for (j = 0; j < i; j++) {
		if (priv->vout_buffer[j]) {
			OMX_FreeBuffer(priv->decoder,
				BASE_OUTPUT_PORT, priv->vout_buffer[j]);
		}
	}

	return -1;
}

static void free_output_buffer(h264_decoder_test_t *priv)
{
	int i;

	for (i = 0; i < priv->vout_buf_num; i++) {
		if (priv->vout_buffer[i]) {
			OMX_FreeBuffer(priv->decoder,
				BASE_OUTPUT_PORT, priv->vout_buffer[i]);
		}
	}

	oscl_free(priv->vout_buffer);
}

/* Callbacks implementation */
static OMX_ERRORTYPE event_handler(
	OMX_HANDLETYPE hComponent,
	OMX_PTR pAppData,
	OMX_EVENTTYPE eEvent,
	OMX_U32 Data1,
	OMX_U32 Data2,
	OMX_PTR pEventData)
{
	h264_decoder_test_t *private = (h264_decoder_test_t *)pAppData;
	char *name;
	/* OMX_PARAM_PORTDEFINITIONTYPE *port_params; */
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	OSCL_TRACE("hComponent:%p", hComponent);
	if (hComponent == private->decoder)
		name = "decoder";
	if (hComponent == private->render)
		name = "render";
	if (eEvent == OMX_EventCmdComplete) {
		if (Data1 == OMX_CommandStateSet) {
			switch ((int)Data2) {
			case OMX_StateInvalid:
				OSCL_LOGI("%s StateSet OMX_StateInvalid", name);
				break;
			case OMX_StateLoaded:
				OSCL_LOGI("%s StateSet OMX_StateLoaded", name);
				break;
			case OMX_StateIdle:
				OSCL_LOGI("%s StateSet OMX_StateIdle", name);
				break;
			case OMX_StateExecuting:
				OSCL_LOGI("%s StateSet OMX_StateExecuting", name);
				break;
			case OMX_StatePause:
				OSCL_LOGI("%s StateSet OMX_StatePause", name);
				break;
			case OMX_StateWaitForResources:
				OSCL_LOGI("%s StateSet WaitForResources", name);
				break;
			default:
				OSCL_LOGE("%s StateSet unkown state", name);
				break;
			}
			sem_post(&private->sem_event);
		} else if (Data1 == OMX_CommandPortEnable) {
			OSCL_LOGI("%s CmdComplete OMX_CommandPortEnable", name);
			sem_post(&private->sem_event);
		} else if (Data1 == OMX_CommandPortDisable) {
			OSCL_LOGI("%s CmdComplete OMX_CommandPortDisable", name);
			sem_post(&private->sem_event);
		}
	} else if (eEvent == OMX_EventBufferFlag) {
		if ((int)Data2 == OMX_BUFFERFLAG_EOS) {
			OSCL_LOGW("%s BufferFlag OMX_BUFFERFLAG_EOS", name);
			if (hComponent == private->decoder) {
				OSCL_LOGW("end of input stream\n");
				sem_post(&private->sem_eos);
			}
		} else
			OSCL_LOGW("%s OMX_EventBufferFlag %x", name, Data2);
	}  else if (eEvent == OMX_EventError) {
		error_value = Data1;
		OSCL_LOGE("Receive error event. value:%x", error_value);
		sem_post(&private->sem_event);
	} else if (eEvent == OMX_EventPortSettingsChanged) {
#if 0
		/* todo: port setting changed, alloc buffer according to new settings */
		if (hComponent == private->decoder && Data1 == OMX_DirOutput) {
			port_params = (OMX_PARAM_PORTDEFINITIONTYPE *)pEventData;
			OSCL_LOGI("output port setting change, width %d, height %d\n",
				port_params->format.video.nFrameWidth,
				port_params->format.video.nFrameHeight);
		}
#endif
	} else
		OSCL_LOGI("%s parm:%x %x", name, Data1, Data2);

	return ret;
}

static OMX_ERRORTYPE untunnel_empty_buffer_done(
	OMX_HANDLETYPE hComponent,
	OMX_PTR pAppData,
	OMX_BUFFERHEADERTYPE *pBuffer)
{
	int ret;
	OMX_ERRORTYPE err = OMX_ErrorNone;
	lb_omx_component_t *component = NULL;
	h264_decoder_test_t *priv = pAppData;

	if (pBuffer == NULL || pAppData == NULL) {
		OSCL_LOGE("err: buffer header is null");
		return OMX_ErrorBadParameter;
	}
	component = get_lb_component(hComponent);
	OSCL_LOGD("component %s, empty buffer done>> %p, %d, input:%d output:%d",
		component->name, pBuffer->pBuffer,
		pBuffer->nFlags, pBuffer->nInputPortIndex, pBuffer->nOutputPortIndex);
	if (component->eos_flag) {
		OSCL_LOGW("component eos\n");
		return err;
	}

	if (hComponent == priv->decoder) {
		/* read one packet and call empty this buffer */
		ret = parse_one_frame(priv, pBuffer->pBuffer,
			(unsigned int *)(&pBuffer->nFilledLen), 0);
		if (ret < 0) {
			OSCL_LOGE("parse frame error, end the stream!\n");
			pBuffer->nFilledLen = 0;
			pBuffer->nFlags |= OMX_BUFFERFLAG_EOS;
		}
		err = OMX_EmptyThisBuffer(priv->decoder, pBuffer);
	} else if (hComponent == priv->render) {
		pBuffer->nOutputPortIndex = BASE_OUTPUT_PORT;
		err = OMX_FillThisBuffer(priv->decoder, pBuffer);
	} else
		OSCL_LOGE("error component!\n");
	if (err != OMX_ErrorNone)
		OSCL_LOGE("OMX_FillThisBuffer err: %x", err);

	return err;
}

static OMX_ERRORTYPE untunnel_fill_buffer_done(
	OMX_HANDLETYPE hComponent,
	OMX_PTR pAppData,
	OMX_BUFFERHEADERTYPE *pBuffer)
{
	OMX_ERRORTYPE err = OMX_ErrorNone;
	/* lb_omx_component_t *component = NULL; */
	h264_decoder_test_t *priv = pAppData;

	if (pBuffer == NULL || pAppData == NULL) {
		OSCL_LOGE("err: buffer header is null");
		return OMX_ErrorBadParameter;
	}
	/*component = get_lb_component(hComponent);
	OSCL_LOGD("component %s, fill buffer done>> %p, %d, input:%d output:%d",
		component->name, pBuffer->pBuffer,
		pBuffer->nFlags, pBuffer->nInputPortIndex, pBuffer->nOutputPortIndex);
	*/

	if (hComponent == priv->decoder) {
		/* call render to empty this buffer */
		pBuffer->nInputPortIndex = 0;
		err = OMX_EmptyThisBuffer(priv->render, pBuffer);
	} else
		OSCL_LOGE("error component!\n");

	if (err != OMX_ErrorNone)
		OSCL_LOGE("OMX_EmptyThisBuffer err: %x", err);

	return err;
}

static OMX_CALLBACKTYPE untunnel_callbacks = {
	.EventHandler = event_handler,
	.EmptyBufferDone = untunnel_empty_buffer_done,
	.FillBufferDone = untunnel_fill_buffer_done,
};

static int untunnel_config_decoder_component(h264_decoder_test_t *priv)
{
	int i, j;
	int frame_size;
	int ret = OMX_ErrorNone;
	OMX_PARAM_PORTDEFINITIONTYPE port_def;

	/* set decoder input port params */
	memset(&port_def, 0, sizeof(port_def));
	port_def.nPortIndex = BASE_INPUT_PORT;
	port_def.nBufferCountActual = H264_INPUT_BUFFER_CNT;
	port_def.bBuffersContiguous = 1;
	port_def.eDir = OMX_DirInput;
	port_def.eDomain = OMX_PortDomainVideo;
	port_def.nBufferSize = H264_DEFAULT_STREAM_SIZE;
	ret = OMX_SetParameter(priv->decoder, OMX_IndexParamPortDefinition,
			&port_def);
	if (ret) {
		OSCL_LOGE("set input port params error!");
		return ret;
	}

	/* alloc input port buffer */
	priv->vin_buf_num = port_def.nBufferCountActual;
	priv->vin_buffer = oscl_zalloc(priv->vin_buf_num *
			sizeof(OMX_BUFFERHEADERTYPE *));
	if (NULL == priv->vin_buffer)
		return OMX_ErrorBadParameter;
	for (i = 0; i < priv->vin_buf_num; i++) {
		priv->vin_buffer[i] = NULL;
		ret = OMX_AllocateBuffer(priv->decoder, &priv->vin_buffer[i],
			BASE_INPUT_PORT, priv, port_def.nBufferSize);
		if (ret != OMX_ErrorNone) {
			OSCL_LOGE("alloc input port buffer %d error!\n", i);
			goto error1;
		}
		OSCL_LOGI("vin buffer %d, addr %p\n", i, priv->vin_buffer[i]->pBuffer);
	}

	/* set decoder output port params */
	memset(&port_def, 0, sizeof(port_def));
	port_def.nPortIndex = BASE_OUTPUT_PORT;
	port_def.nBufferCountActual = H264_OUTPUT_BUFFER_CNT;
	port_def.bBuffersContiguous = 1;
	port_def.eDir = OMX_DirOutput;
	port_def.eDomain = OMX_PortDomainVideo;
	port_def.format.video.eColorFormat = OMX_COLOR_FormatYUV420SemiPlanar;
	port_def.format.video.nFrameWidth  = priv->width;
	port_def.format.video.nFrameHeight = priv->height;
	port_def.format.video.nStride      = (priv->width + 15) & 0xFFFFFFF0;
	port_def.format.video.nSliceHeight = (priv->height + 15) & 0xFFFFFFF0;
	frame_size = port_def.format.video.nStride *
			port_def.format.video.nSliceHeight;
	port_def.nBufferSize = ((frame_size + 1024 - 1) & 0xFFFFFC00)
					+ frame_size / 2; /* plane alinged 1024 */
	ret = OMX_SetParameter(priv->decoder, OMX_IndexParamPortDefinition,
			&port_def);
	if (ret) {
		OSCL_LOGE("set output port params error!");
		goto error1;
	}

	/* alloc outport buffer */
	ret = alloc_output_buffer(priv, port_def.nBufferSize);
	if (ret != OMX_ErrorNone) {
		OSCL_LOGE("alloc out buffer error!\n");
		goto error1;
	}

	return ret;

error1:
	oscl_free(priv->vin_buffer);
	for (j = 0; j < i; j++)
		OMX_FreeBuffer(priv->decoder, BASE_INPUT_PORT, priv->vin_buffer[j]);
	return ret;
}

static int untunnel_config_render_component(h264_decoder_test_t *priv)
{
	int ret = OMX_ErrorNone;
	int frame_size;
	OMX_PARAM_PORTDEFINITIONTYPE port_def;
	OMX_CONFIG_ROTATIONTYPE rot_mode;
	disp_size_t scn_size;

	memset(&priv->disp_para, 0, sizeof(priv->disp_para));
	memset(&scn_size, 0, sizeof(scn_size));
	ret = get_screen_size(&scn_size);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);

	priv->disp_para.mode = VDISP_WINDOW_FULL_SCREEN_VIDEO_RATIO;
	priv->disp_para.win_rect.width = scn_size.width;
	priv->disp_para.win_rect.height = scn_size.height;
	priv->disp_para.win_rect.left =
		scn_size.width - priv->disp_para.win_rect.width;
	priv->disp_para.win_rect.top = 0;

	ret = OMX_SetParameter(priv->render, omx_index_vrender_mode,
			&priv->disp_para);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);
	rot_mode.nRotation = VDISP_ROTATE_NONE;
	ret = OMX_SetParameter(priv->render, OMX_IndexConfigCommonRotate, &rot_mode);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);

	/* set render input port params*/
	memset(&port_def, 0, sizeof(port_def));
	port_def.nPortIndex = BASE_INPUT_PORT;
	port_def.nBufferCountActual = H264_OUTPUT_BUFFER_CNT;
	port_def.bBuffersContiguous = 1;
	port_def.nBufferAlignment = 1024;
	port_def.eDir = OMX_DirInput;
	port_def.eDomain = OMX_PortDomainVideo;
	port_def.format.video.eColorFormat = OMX_COLOR_FormatYUV420SemiPlanar;
	port_def.format.video.nFrameWidth  = priv->width;
	port_def.format.video.nFrameHeight = priv->height;
	port_def.format.video.nStride      = (priv->width + 15) & 0xFFFFFFF0;
	port_def.format.video.nSliceHeight = (priv->height + 15) & 0xFFFFFFF0;
	frame_size = port_def.format.video.nStride *
			port_def.format.video.nSliceHeight;
	port_def.nBufferSize = ((frame_size + 1024 - 1) & 0xFFFFFC00)
					+ frame_size / 2; /* plane alinged 1024 */
	ret = OMX_SetParameter(priv->render, OMX_IndexParamPortDefinition,
			&port_def);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);
#if 0
	stride_info.width = (priv->width + 15) & 0xFFFFFFF0; /* alinged 16 */
	stride_info.height = (priv->height + 15) & 0xFFFFFFF0; /* alinged 16 */
	ret = OMX_SetParameter(priv->render, omx_index_vrender_stride_info,
				&stride_info);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);
#endif

EXIT:
	return ret;
}

int omx_test_h264dec_untunnel(char *infile, int width, int height)
{
	int i = 0;
	int ret = OMX_ErrorNone;
	h264_decoder_test_t *priv = &decoder_priv;
	struct stat file_info;

	OSCL_LOGI("file %s, width %d, height %d\n", infile, width, height);
	memset(priv, 0, sizeof(h264_decoder_test_t));
	priv->width = width;
	priv->height = height;

	priv->file_buffer = oscl_zalloc(H264_DEFAULT_STREAM_SIZE);
	if (priv->file_buffer == NULL) {
		OSCL_LOGE("malloc file buffer error!\n");
		return -1;
	}

	priv->infile = fopen(infile, "rb");
	if (priv->infile == NULL) {
		OSCL_LOGE("open input file %s error\n", infile);
		oscl_free(priv->file_buffer);
		return -1;
	}
	ret = stat(infile, &file_info);
	if (ret != 0) {
		OSCL_LOGE("get file info error!\n");
		ret = -1;
		goto error1;
	}
	priv->file_len = file_info.st_size;
	OSCL_LOGI("file size %d\n", priv->file_len);

	/* 1. get component handle */
	OMX_GetHandle((void **)&priv->decoder,
		"OMX.LB.VIDEO.H264DEC", priv, &untunnel_callbacks);
	OMX_GetHandle((void **)&priv->render,
		"OMX.LB.SINK.VRENDER", priv, &untunnel_callbacks);
	OSCL_LOGD("here");
	if (priv->decoder == NULL || priv->render == NULL) {
		OSCL_LOGE("get handle failed! %x, %x, %x", priv->decoder,
			priv->render);
		ret = -1;
		goto error2;
	}
	OSCL_LOGD("here");

	sem_init(&priv->sem_event, 0, 0);
	sem_init(&priv->sem_eos, 0, 0);

	/* 2. config decoder & render component */
	ret = untunnel_config_decoder_component(priv);
	if (ret != OMX_ErrorNone) {
		OSCL_LOGE("config decoder error!\n");
		goto error3;
	}
	ret = untunnel_config_render_component(priv);
	if (ret != OMX_ErrorNone) {
		OSCL_LOGE("config render error!\n");
		goto error3;
	}

	/* 3. set component stat to idle */
	ret = OMX_SendCommand(priv->decoder, OMX_CommandStateSet, OMX_StateIdle, NULL);
	if (ret != OMX_ErrorNone) {
		OSCL_LOGE("set decoder to idle error!\n");
		goto error3;
	}
	sem_wait(&priv->sem_event);

	ret = OMX_SendCommand(priv->render, OMX_CommandStateSet, OMX_StateIdle, NULL);
	if (ret != OMX_ErrorNone) {
		OSCL_LOGE("set render to idle error!\n");
		goto error3;
	}
	sem_wait(&priv->sem_event);

	/* 4. get header and queue to decoder component input port */
	ret = parse_one_frame(priv, priv->vin_buffer[0]->pBuffer,
			(unsigned int *)(&priv->vin_buffer[0]->nFilledLen), 1);
	if (ret < 0) {
		OSCL_LOGE("get h264 header error!\n");
		goto error3;
	}
	ret = OMX_EmptyThisBuffer(priv->decoder, priv->vin_buffer[0]);
	if (ret != OMX_ErrorNone) {
		OSCL_LOGE("omx empty buffer error!\n");
		goto error3;
	}

	/* 5. set component stat to executing */
	ret = OMX_SendCommand(priv->decoder, OMX_CommandStateSet,
			OMX_StateExecuting, NULL);
	if (ret != OMX_ErrorNone) {
		OSCL_LOGE("set decoder to executing error!\n");
		goto error3;
	}
	sem_wait(&priv->sem_event);

	ret = OMX_SendCommand(priv->render, OMX_CommandStateSet,
			OMX_StateExecuting, NULL);
	if (ret != OMX_ErrorNone) {
		OSCL_LOGE("set render to executing error!\n");
		goto error3;
	}
	sem_wait(&priv->sem_event);

#if 1
	sem_wait(&priv->sem_eos);
#else
	rt_thread_delay(3500); /* decode 15s */
	OSCL_LOGI("quit test...");
#endif

	/* 6. set component stat to idle */
	ret = OMX_SendCommand(priv->decoder, OMX_CommandStateSet, OMX_StateIdle, NULL);
	if (ret != OMX_ErrorNone) {
		OSCL_LOGE("set decoder to idle error!\n");
		goto error3;
	}
	sem_wait(&priv->sem_event);
	ret = OMX_SendCommand(priv->render, OMX_CommandStateSet, OMX_StateIdle, NULL);
	if (ret != OMX_ErrorNone) {
		OSCL_LOGE("set render to idle error!\n");
		goto error3;
	}
	sem_wait(&priv->sem_event);

	/* free input buffer */
	for (i = 0; i < priv->vin_buf_num; i++)
		OMX_FreeBuffer(priv->decoder, BASE_INPUT_PORT, priv->vin_buffer[i]);

	/* free output buffer */
	free_output_buffer(priv);

	/* 7. set component stat to loaded */
	ret = OMX_SendCommand(priv->decoder, OMX_CommandStateSet, OMX_StateLoaded, NULL);
	if (ret != OMX_ErrorNone) {
		OSCL_LOGE("set decoder to loaded error!\n");
		goto error3;
	}
	sem_wait(&priv->sem_event);
	ret = OMX_SendCommand(priv->render, OMX_CommandStateSet, OMX_StateLoaded, NULL);
	if (ret != OMX_ErrorNone) {
		OSCL_LOGE("set render to loaded error!\n");
		goto error3;
	}
	sem_wait(&priv->sem_event);

error3:
	sem_destroy(&priv->sem_event);
	sem_destroy(&priv->sem_eos);
error2:
	OMX_FreeHandle(priv->decoder);
	priv->decoder = NULL;
	OMX_FreeHandle(priv->render);
	priv->render = NULL;
error1:
	oscl_free(priv->file_buffer);
	fclose(priv->infile);
	return ret;
}


static omx_component_tbl_t omx_decoder_component = {
	"OMX.LB.VIDEO.H264DEC",
	1,
	h264dec_component_init, NULL, NULL,
	{
		"video_decoder.h264",
		NULL
	}
};

int h264dec_component_test(int argc, char **argv)
{
	if (argc < 3) {
		OSCL_LOGW("usage: h264dec_component_test filename width height");
		return -1;
	}
	/* init omx and regster components */
	OMX_Init();
	omx_component_register(&omx_decoder_component);

	/* test untunnel audio receiver component */
	omx_test_h264dec_untunnel(argv[1], atoi(argv[2]), atoi(argv[3]));

	OSCL_LOGI("test complete....");
	return 0;
}

/* usage: h264dec_component_test filename width height */
MSH_CMD_EXPORT(h264dec_component_test, "h264dec_component_test");

