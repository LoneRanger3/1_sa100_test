/*
 * muxer_component.c - Standard functionality for the media encodec component.
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
#define DBG_LEVEL		DBG_ERR

#include <strings.h>
#include <oscl.h>
#include <base_component.h>
#include "muxer_com.h"
#include "muxer_common.h"
#include "avformat.h"
#include "oscl_cache_file.h"
#include "lombo_encplugin.h"

#define FILE_NAME_MAX_LENTH 512
#define MAX_FRAME_NUM 250

#define SINK_INPUT_DEFAULT_SIZE (16 * 1024)
#define BLKSIZE 1024
#define OMX_VIDEO_BUF_SIZE (400 * 1024)
#define MUXER_NUM 2

#define FHD_FILE_CACHE_SIZE     (512 * 1024)
#define FHD_FILE_CACHE_NUM      (2)
#define OTHER_FILE_CACHE_SIZE   (256 * 1024)
#define OTHER_FILE_CACHE_NUM    (3)

/* ms */
#define AV_PACKET_CNT_DIFF 3

/* #define MUXERING_INTERVAL_DEBUG */
#define INTERVAL_TIME 100

typedef enum Muxer_Handle_Staus {
	MUXER_INIT = 0,
	MUXER_MUXERING,
	MUXER_CLOSEING,
} Muxer_Handle_Staus;
enum {
	MUXER_CACHE_NOT_USE,
	MUXER_CACHE_USE,
};
typedef enum stream_output_type {
	STREAM_OUTPUT_TYPE_INVALID,
	STREAM_OUTPUT_TYPE_FILE,
	STREAM_OUTPUT_TYPE_EXTERNAL,
} stream_output_type_e;
typedef struct muxer_cache_tag {
	format_muxer *muxer_handle;
	OMX_S32 write_cache_thread_id;
	pthread_t write_cache_thread;
	sem_t file_close_sem;
	OMX_S32 exit_thread_flag;
	OMX_S32 close_flag;
	OMX_S32 start_mux_flag;
	oscl_queue_t pkt_queue;
} muxer_cache_t;
typedef struct MuxerContext {
	int id;
	int64_t cur_time;
	AVIOFILE fp;
	char *filename;
	int file_total_time;
	int new_muxer_flag;
	pthread_mutex_t context_mutex;
	format_muxer muxer_handle_sz[MUXER_NUM];
	format_muxer *muxer_handle;
	muxer_cache_t muxer_cache_sz[MUXER_NUM];
	muxer_cache_t *muxer_cache;
	OMX_S32 use_cache_flag;
	int num_stream;
	int stream_exist[2];
	AV_CodecParameters parameters[2]; /**< audio common info used during init */
	int64_t aref_time;
	int rebuilding_flag;
	venc_packet_t combine_packet;
	int bitrate;
	int err_flag;
	stream_output_type_e stream_out_type;
	external_stream_writer_t ext_writer;
} MuxerContext;

static void wait_file_close_finish(MuxerContext *context)
{
	OMX_S32 i = 0;
	OMX_S32 wait = 0;
	format_muxer *muxer = NULL;

	do {
		wait = 0;
		for (i = 0; i < MUXER_NUM; i++) {
			muxer = &(context->muxer_handle_sz[i]);
			if (!(muxer->muxer_status == MUXER_INIT)) {
				wait = 1;
				oscl_mdelay(10);
				OSCL_LOGI("wait muxer handle to be idle, stat[%d]:%d\n",
						i, muxer->muxer_status);
			}
		}
	} while (wait);
}

static format_muxer *__get_muxer_handle(MuxerContext *context)
{
	int i = 0;
	int ret = OMX_ErrorNone;
	format_muxer *muxer = NULL;

	for (i = 0; i < MUXER_NUM; i++) {
		muxer = &(context->muxer_handle_sz[i]);
		if (muxer->muxer_status == MUXER_INIT)
			break;
	}
	if (i >= MUXER_NUM) {
		OSCL_LOGE("can not find free muxer handle\n");
		goto EXIT;
	}
	if (context->use_cache_flag == MUXER_CACHE_USE) {
		for (i = 0; i < MUXER_NUM; i++) {
			if (muxer ==  context->muxer_cache_sz[i].muxer_handle) {
				context->muxer_cache = &(context->muxer_cache_sz[i]);
				break;
			}
		}
		if (i >= MUXER_NUM) {
			OSCL_LOGE("can not find free muxer cache\n");
			muxer = NULL;
			goto EXIT;
		}
	}
	/* set muxer para */
	for (i = 0; i < context->num_stream; i++) {
		int size_ex = context->parameters[i].extradata_size +
			AV_INPUT_BUFFER_PADDING_SIZE;
		memcpy(&(muxer->para[i]), &(context->parameters[i]),
			sizeof(AV_CodecParameters));
		if (context->parameters[i].extradata != NULL) {
			muxer->para[i].extradata = oscl_zalloc(size_ex);
			memcpy(muxer->para[i].extradata,
				context->parameters[i].extradata,
				muxer->para[i].extradata_size);
		}
	}
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);
	muxer->nb_streams = context->num_stream;
	/* muxer init */
	ret = muxer_init(muxer, context->id);
	muxer->muxer_status = MUXER_MUXERING;
	muxer->start_pts[AUDIO_INDEX] = -1;
	muxer->start_pts[VIDEO_INDEX] = -1;
	context->cur_time = 0;

EXIT:
	if (ret != OMX_ErrorNone && muxer) {
		muxer_deinit(muxer);
		muxer->muxer_status = MUXER_INIT;
		muxer = NULL;
	}

	return muxer;
}

static void __put_muxer_handle(MuxerContext *context, format_muxer *muxer_handle)
{
	if (context == NULL || muxer_handle == NULL) {
		OSCL_LOGE("err param");
		return;
	}
	if (muxer_handle->muxer_status == MUXER_MUXERING) {
		if (muxer_handle->fp != FNULL)
			oscl_cfile_set_prio(muxer_handle->fp, LB_FILE_CLOSE_PRIO);
		muxer_handle->muxer_status = MUXER_CLOSEING;
		sem_post(&(muxer_handle->file_close_sem));
	} else
		OSCL_LOGE("muxer is not MUXER_MUXERING while close\n");
	return;
}

static int __start_new_muxer(MuxerContext *context)
{
	int ret = OMX_ErrorNone;
	int32_t flag = 0;
	/* close old file */
	if (context->muxer_handle) {
		__put_muxer_handle(context, context->muxer_handle);
		context->muxer_handle = NULL;
	} else
		flag = 1;

	/* get new handle */
	if (context->muxer_handle == NULL)
		context->muxer_handle = __get_muxer_handle(context);
	oscl_param_check_exit(context->muxer_handle, -1, NULL);

	/* output raw data mode, set writer as cb */
	if (context->ext_writer.write != NULL) {
		OSCL_LOGI("muxer_ctrl set stream cb");
		muxer_ctrl(context->muxer_handle, MUXER_CMD_SET_STREAM_CB,
			&context->ext_writer);
	}

	/* STREAM_OUTPUT_TYPE_FILE, open file and write header */
	if (context->stream_out_type == STREAM_OUTPUT_TYPE_FILE && context->filename) {
		context->fp = oscl_cfile_open(context->filename, FWRMODE);
		oscl_param_check_exit(context->fp != FNULL, -1, NULL);
		context->muxer_handle->filename =
			oscl_strdup(context->filename);
		context->muxer_handle->fp = context->fp;
		if (flag && (context->use_cache_flag == MUXER_CACHE_USE)
			&& context->muxer_cache)
			context->muxer_cache->start_mux_flag = 1;
		if (context->parameters[VIDEO_INDEX].height >= 1920 ||
			context->parameters[VIDEO_INDEX].width >= 1920)
			oscl_cfile_realloc_cache(context->fp,
				FHD_FILE_CACHE_NUM, FHD_FILE_CACHE_SIZE);
		else
			oscl_cfile_realloc_cache(context->fp,
				OTHER_FILE_CACHE_NUM, OTHER_FILE_CACHE_SIZE);
		ret = muxer_write_header(context->muxer_handle);
		if (ret != OMX_ErrorNone) {
			OSCL_LOGE("write_header error:%d", ret);
			goto EXIT;
		}
	}
	context->new_muxer_flag = 0;

EXIT:
	if (ret != OMX_ErrorNone) {
		context->err_flag = 1;
		__put_muxer_handle(context, context->muxer_handle);
		context->muxer_handle = NULL;
	}
	return ret;
}

static int __frame_rebuild(MuxerContext *context, venc_packet_t *venc_packet)
{
	int ret = 0;
	unsigned char *tmp_buffer;
	venc_packet_t *combine_packet = &context->combine_packet;

	/* err state in previous rebuilding process */
	if (context->rebuilding_flag && (combine_packet->vir_addr == NULL)) {
		ret = -1;
		goto EXIT;
	}

	/* copy buffer header info when we recevice first part of a frame */
	if (context->rebuilding_flag == 0) {
		memcpy(combine_packet, venc_packet, sizeof(venc_packet_t));
		combine_packet->vir_addr = NULL;
		combine_packet->phy_addr = 0;
		combine_packet->size = 0;
		context->rebuilding_flag = 1;
	}

	/* realloc buffers when compined packet is full */
	tmp_buffer = oscl_zalloc(combine_packet->size + venc_packet->size);
	if (tmp_buffer && combine_packet->vir_addr)
		memcpy(tmp_buffer, combine_packet->vir_addr, combine_packet->size);
	if (combine_packet->vir_addr)
		free(combine_packet->vir_addr);
	combine_packet->vir_addr = tmp_buffer;
	if (combine_packet->vir_addr == NULL) {
		OSCL_LOGE("malloc combine_packet pbuffer error");
		combine_packet->size = 0;
		ret = -1;
		goto EXIT;
	}
	memcpy(combine_packet->vir_addr + combine_packet->size,
		venc_packet->vir_addr, venc_packet->size);
	combine_packet->size += venc_packet->size;
EXIT:
	if (ret != 0) {
		OSCL_LOGE("rebuild video packet failed , check it!");
		if (combine_packet->vir_addr != NULL)
			free(combine_packet->vir_addr);
		memset(combine_packet, 0, sizeof(venc_packet_t));
	}
	return 0;
}
static int32_t muxer_pkt_put_queue(muxer_cache_t *muxer_cache, packet_t *pin)
{
	packet_t *p_pkt = NULL;

	oscl_param_check(muxer_cache != NULL, -1, NULL);
	oscl_param_check(pin != NULL, -1, NULL);

	if (oscl_message_count(&muxer_cache->pkt_queue) > MAX_FRAME_NUM) {
		OSCL_LOGE("the queue count is more than MAX_FRAME_NUM, drop it");
		return -1;
	}

	p_pkt = (packet_t *)oscl_zalloc(sizeof(packet_t));
	if (p_pkt == NULL) {
		OSCL_LOGE("packet_t zalloc failed");
		return -1;
	}

	p_pkt->data = (uint8_t *)oscl_zalloc(pin->size);
	if (NULL == p_pkt->data) {
		OSCL_LOGE("p_pkt->data zalloc failed");
		oscl_free(p_pkt);
		return -1;
	}

	memcpy(p_pkt->data, pin->data, pin->size);
	p_pkt->size = pin->size;
	p_pkt->pts = pin->pts;
	p_pkt->dts = pin->dts;
	p_pkt->duration = pin->duration;
	p_pkt->stream_index = pin->stream_index;
	p_pkt->flags = pin->flags;
	p_pkt->buf_size = pin->buf_size;
	p_pkt->timestamp = pin->timestamp;
	return oscl_queue_queue(&muxer_cache->pkt_queue, p_pkt);
}

static int audio_buffer_handle(MuxerContext *context,
	OMX_BUFFERHEADERTYPE *inbuffer)
{
	int ret = 0;
	int index;
	packet_t pin = { 0 };
	int64_t timestamp;

	index = inbuffer->nInputPortIndex;

	pin.data = inbuffer->pBuffer + inbuffer->nOffset;
	pin.stream_index = index;
	pin.size = inbuffer->nFilledLen;
	if (context->muxer_handle->start_pts[index] < 0) {
		OSCL_LOGI("p[%d] start pts %lld", index, inbuffer->nTimeStamp);
		context->muxer_handle->start_pts[index] = inbuffer->nTimeStamp;
	}
	timestamp = inbuffer->nTimeStamp - context->muxer_handle->start_pts[index];
	pin.pts = timestamp;
	timestamp = pin.pts * 1000 /
		context->muxer_handle->para[index].sample_rate;
	pin.dts = pin.pts;
	if (pin.dts < 0) {
		OSCL_LOGE("pin.dts %lld - %lld - %lld\n", (OMX_S64)pin.dts,
			inbuffer->nTimeStamp, context->muxer_handle->start_pts[index]);
	}
	pin.timestamp = timestamp;

	if (context->use_cache_flag == MUXER_CACHE_NOT_USE) {
		ret = muxer_write_packet(context->muxer_handle, &pin);
		if (ret < 0)
			OSCL_LOGE("err ret :%d\n", ret);
	} else {
		ret = muxer_pkt_put_queue(context->muxer_cache, &pin);
		if (ret < 0)
			OSCL_LOGE("audio: cache queue is full");
	}

	return ret;
}

static void muxer_buffer_handle(OMX_HANDLETYPE stand_com,
	OMX_BUFFERHEADERTYPE *inbuffer,
	OMX_BUFFERHEADERTYPE *outbuffer)
{
	MuxerContext *context = NULL;
	lb_omx_component_t *component;
	int ret = 0;
	int index;
	packet_t pin = { 0 };
	int64_t timestamp;
	venc_packet_t *venc_packet;
	int end_of_frame;

	component = get_lb_component(stand_com);
	context = component->component_private;

	oscl_param_check_exit(context != NULL, -1, NULL);
	oscl_param_check_exit(inbuffer != NULL, -1, NULL);
	oscl_param_check_exit(inbuffer->pBuffer != NULL, -1, NULL);

	index = inbuffer->nInputPortIndex;
	if (index == AUDIO_INDEX) {
		ret = audio_buffer_handle(context, inbuffer);
		goto EXIT;
	}
	oscl_param_check_exit(index == VIDEO_INDEX, -1, NULL);

	venc_packet = (venc_packet_t *)inbuffer->pBuffer;
	if (venc_packet->pic_type == VENC_INIT_PACKET) {
		pthread_mutex_lock(&context->context_mutex);
		if (context->parameters[index].extradata != NULL) {
			OSCL_LOGI("recevied new extradata\n");
			oscl_free(context->parameters[index].extradata);
		}
		context->parameters[index].extradata =
			oscl_zalloc(venc_packet->size + AV_INPUT_BUFFER_PADDING_SIZE);
		memcpy(context->parameters[index].extradata,
			venc_packet->vir_addr, venc_packet->size);
		context->parameters[index].extradata_size = venc_packet->size;
		OSCL_LOGI("eCompressionFormat:%d, width:%d, height:%d, framerate:%d\n",
			context->parameters[index].codec_id,
			context->parameters[index].width,
			context->parameters[index].height,
			context->parameters[index].sample_rate);
		OSCL_LOGI("video extradata(%d)\n",
			context->parameters[index].extradata_size);
		pthread_mutex_unlock(&context->context_mutex);
		goto EXIT;
	}

	/* rebuild uncomplete frames */
	end_of_frame = (venc_packet->frame_end_flag == VENC_FRAME_END);
	if ((index == VIDEO_INDEX) && (context->rebuilding_flag || end_of_frame == 0)) {
		pthread_mutex_lock(&context->context_mutex);
		__frame_rebuild(context, venc_packet);
		pthread_mutex_unlock(&context->context_mutex);
		venc_packet = &context->combine_packet;
		if (end_of_frame == 0) {
			venc_packet = NULL;
			goto EXIT;
		}
	}

	pin.data = venc_packet->vir_addr;
	pin.stream_index = index;
	pin.size = venc_packet->size;
	if (context->muxer_handle->start_pts[index] < 0) {
		OSCL_LOGI("p[%d] start pts %lld", index, venc_packet->time_stamp);
		context->muxer_handle->start_pts[index] = venc_packet->time_stamp;
	}
	timestamp = venc_packet->time_stamp - context->muxer_handle->start_pts[index];
	pin.pts = timestamp * context->muxer_handle->para[index].sample_rate / 1000;
	context->cur_time = timestamp;
	pin.dts = pin.pts;
	if (pin.dts < 0) {
		OSCL_LOGE("pin.dts %lld - %lld - %lld\n", (OMX_S64)pin.dts,
			inbuffer->nTimeStamp, context->muxer_handle->start_pts[index]);
	}
	pin.timestamp = timestamp;

	if (venc_packet->pic_type == VENC_I_FRAME)
		pin.flags |= AV_PKT_FLAG_KEY;
	if (context->use_cache_flag == MUXER_CACHE_NOT_USE) {
		ret = muxer_write_packet(context->muxer_handle, &pin);
		if (ret < 0)
			OSCL_LOGE("err ret :%d\n", ret);
	} else {
		ret = muxer_pkt_put_queue(context->muxer_cache, &pin);
		if (ret < 0)
			OSCL_LOGE("video: cache queue is full");
	}


#if 1
	/* free tmp buffer used for rebuild video packet */
	if (context->rebuilding_flag) {
		if (context->combine_packet.vir_addr)
			oscl_free(context->combine_packet.vir_addr);
		memset(&(context->combine_packet), 0, sizeof(venc_packet_t));
		context->rebuilding_flag = 0;
	}
#endif

EXIT:
	inbuffer->nOffset = 0;
	inbuffer->nFilledLen = 0;
	inbuffer->nFlags = 0;
	if (ret < 0)
		OSCL_LOGE("muxer buffer handle failed!");
	return;
}

static void *muxer_buffer_manager(void *param)
{
	lb_omx_component_t *component;
	MuxerContext *context = NULL;
	OMX_COMPONENTTYPE *stand_comp = (OMX_COMPONENTTYPE *)param;
	base_port_t *base_port;
	OMX_BUFFERHEADERTYPE *inbuffer = NULL;
	int process_cnt = 0;
	int index = AUDIO_INDEX;
	int skip_packet_cnt[2];
	venc_packet_t *combine_packet;
	int64_t start_time = -1;
	int cnt = 0;

	OSCL_TRACE(" %p\n", param);
	oscl_param_check((param != NULL), NULL, NULL);
	component = get_lb_component(stand_comp);
	context = component->component_private;
	OSCL_LOGI("======comp:%s", get_component_name(component));
	component->dbg_flag = set_debug_state(component->dbg_flag,
			DEBUG_MSG_SHT, DEBUG_THREAD_START);
	combine_packet = &context->combine_packet;
	base_port = &component->port[index];

	while (component->state == OMX_StateIdle
		|| component->state == OMX_StateExecuting
		|| component->state == OMX_StatePause
		|| component->target_state == OMX_StateIdle) {

		/*Wait till the ports are being flushed*/
		pthread_mutex_lock(&component->flush_mutex);
		while (base_port->is_flushed) {
			pthread_mutex_unlock(&component->flush_mutex);
			if (inbuffer != NULL) {
				base_port->return_buffer(base_port, inbuffer);
				inbuffer = NULL;
				OSCL_LOGI("retrun buffer while flushing port");
			}
			sem_post(component->mgmt_flush_sem);
			OSCL_LOGI("wait flush_sem");
			sem_wait(component->flush_sem);
			OSCL_LOGI("after wait flush_sem, queue len:%d, inbuffer %p",
				oscl_queue_get_num(&base_port->buffer_queue),
				inbuffer);
			skip_packet_cnt[0] = 1;
			skip_packet_cnt[1] = 1;
			if (combine_packet->vir_addr)
				oscl_free(combine_packet->vir_addr);
			memset(combine_packet, 0, sizeof(venc_packet_t));
			context->rebuilding_flag = 0;
			pthread_mutex_lock(&component->flush_mutex);
		}
		pthread_mutex_unlock(&component->flush_mutex);

		if (component->state != OMX_StateExecuting) {
			start_time = -1;
			sem_wait(component->buf_mgnt_sem);
			continue;
		}
		/* ======= get a media packet from input port ===== */
		/* get packet step 1. get each stream one by one */
		index = (process_cnt++ % context->num_stream);

		/* get packet step 2. when frame is in rebuilding process, get the
		 * remain buffers until rebuilding finished */
		if (context->rebuilding_flag == 1)
			index = VIDEO_INDEX;

		/* get packet step 3. we get video stream first becase we need video pps
		 * to init muxer, and we also need a keyframe of video stream to start */
		if (context->muxer_handle == NULL && context->err_flag == 0 &&
			context->stream_exist[VIDEO_INDEX])
			index = VIDEO_INDEX;

		/* muxer donot have cur stream, skip it. */
		if (context->stream_exist[index] == 0)
			continue;
		base_port = &component->port[index];
		inbuffer = oscl_queue_dequeue(&base_port->buffer_queue);
		if (inbuffer == NULL) {
			skip_packet_cnt[index]++;
			/* can not get buffer for 3 times , retry to get it */
			if (skip_packet_cnt[index] >= AV_PACKET_CNT_DIFF) {
				if (skip_packet_cnt[index] >= 8 &&
					context->muxer_handle) {
					cnt++;
					if (cnt > 10) {
						/*reduce printing frequency,thus reducing
						CPU utilization*/
						cnt = 0;
						OSCL_LOGE("skip cnts:%d %d, %d",
							skip_packet_cnt[0],
							skip_packet_cnt[1], index);
					}
				}
				oscl_mdelay(10);
				inbuffer = oscl_queue_dequeue(&base_port->buffer_queue);
			}
		}
		if (inbuffer) {
			cnt = 0;
			skip_packet_cnt[index] = 0;
			oscl_sem_trywait(component->buf_mgnt_sem);
		} else {
			/* both stream is null, wait */
			if (skip_packet_cnt[0] && skip_packet_cnt[1])
				oscl_sem_wait(component->buf_mgnt_sem);
			/* only exist one stream, wait */
			else if (!context->stream_exist[0] || !context->stream_exist[1])
				oscl_sem_wait(component->buf_mgnt_sem);
			continue;
		}
		pthread_mutex_lock(&context->context_mutex);
		if ((context->file_total_time > 0) && index == VIDEO_INDEX) {
			if (start_time < 0)
				start_time = inbuffer->nTimeStamp;
			else if (context->new_muxer_flag == 0) {
				int64_t tmp_time = inbuffer->nTimeStamp - start_time;
				if (tmp_time  >= (context->file_total_time - 100)) {
					((component->callbacks.EventHandler))(
						&component->base_comp,
						component->callback_data,
						OMX_Eventneednewoutputfilename,
						0, 0, context->filename);
					start_time += context->file_total_time;
					context->new_muxer_flag = 1;
				}
			}
		}

		if (context->new_muxer_flag) {
			/* do not exist video stream, start a new file */
			if (context->stream_exist[VIDEO_INDEX] == 0) {
				__start_new_muxer(context);
				OSCL_LOGE("start newfile (%s):%lld", context->filename,
					inbuffer->nTimeStamp);
			}
			/* start a new file untile recevie a keyframe in video stream */
			if (inbuffer->nInputPortIndex == VIDEO_INDEX &&
				(inbuffer->nFlags & OMX_BUFFERFLAG_SYNCFRAME)) {
				__start_new_muxer(context);
				OSCL_LOGI("start newfile (%s):%lld", context->filename,
					inbuffer->nTimeStamp);
			}
		}
		pthread_mutex_unlock(&context->context_mutex);

		if (component->state == OMX_StateExecuting && context->err_flag == 0) {
			if (component->buf_handle && inbuffer->nFilledLen != 0)
				(*(component->buf_handle))(stand_comp, inbuffer, NULL);
		}

		base_check_mark(component, inbuffer);
		base_check_eos(component, base_port, inbuffer);

		if ((inbuffer && inbuffer->nFilledLen == 0) || context->err_flag ||
			component->target_state == OMX_StateLoaded) {
			base_port->return_buffer(base_port, inbuffer);
			inbuffer = NULL;
		}
	}

	if ((inbuffer != NULL) && (!PORT_IS_SUPPLIER(base_port))) {
		OSCL_LOGI("inport return_buffer before exit");
		base_port->return_buffer(base_port, inbuffer);
	}
	OSCL_LOGW("exit from writer_buffer_manager:\n");
	OSCL_TRACE(" %p\n", param);
	component->dbg_flag = set_debug_state(component->dbg_flag,
			DEBUG_MSG_SHT, DEBUG_THREAD_EXIT);
	pthread_exit(NULL);
	return NULL;
}

OMX_ERRORTYPE muxer_set_config(OMX_IN OMX_HANDLETYPE hcomp,
	OMX_IN OMX_INDEXTYPE cfg_index,
	OMX_IN OMX_PTR cfg_data)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	lb_omx_component_t *component;
	MuxerContext *context = NULL;
	OMX_TIME_CONFIG_TIMESTAMPTYPE *time;

	OSCL_TRACE(" %p, %d , %p\n", hcomp, cfg_index, cfg_data);
	oscl_param_check((hcomp != NULL) && (cfg_data != NULL),
		OMX_ErrorBadParameter, NULL);

	component = get_lb_component(hcomp);
	oscl_param_check((component != NULL), OMX_ErrorBadParameter, NULL);
	context = component->component_private;

	switch (cfg_index) {
	case omx_index_lombo_config_cur_time:
		time = cfg_data;
		if (time->nPortIndex == AUDIO_INDEX)
			context->aref_time = time->nTimestamp;
		break;
	default:
		ret = base_get_config(hcomp, cfg_index, cfg_data);
		break;
	}
	OSCL_TRACE(" %d\n", ret);

	return ret;
}

OMX_ERRORTYPE muxer_get_config(OMX_IN OMX_HANDLETYPE hcomp,
	OMX_IN OMX_INDEXTYPE cfg_index,
	OMX_IN OMX_PTR cfg_data)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	lb_omx_component_t *component;
	MuxerContext *context = NULL;
	OMX_TIME_CONFIG_TIMESTAMPTYPE *time;

	OSCL_TRACE(" %p, %d , %p\n", hcomp, cfg_index, cfg_data);
	oscl_param_check((hcomp != NULL) && (cfg_data != NULL),
		OMX_ErrorBadParameter, NULL);

	component = get_lb_component(hcomp);
	oscl_param_check((component != NULL), OMX_ErrorBadParameter, NULL);
	context = component->component_private;

	switch (cfg_index) {
	case omx_index_lombo_config_cur_time:
		time = cfg_data;
		time->nTimestamp = context->cur_time;
		break;
	default:
		ret = base_get_config(hcomp, cfg_index, cfg_data);
		break;
	}
	OSCL_TRACE(" %d\n", ret);

	return ret;
}

OMX_ERRORTYPE muxer_port_recive_buffer(base_port_t *port,
	OMX_BUFFERHEADERTYPE *buffer)
{
	OMX_U32 port_idx;
	lb_omx_component_t *component = (lb_omx_component_t *)port->component;
	MuxerContext *context = NULL;
	int64_t ref_time = 0;
	static int count = 1;

	OSCL_TRACE("%p:%p", port, buffer);
	context = component->component_private;

	port_idx = buffer->nInputPortIndex;
	oscl_param_check((port_idx == port->port_param.nPortIndex),
		OMX_ErrorBadPortIndex, NULL);
	if (port_idx == AUDIO_INDEX)
		ref_time = context->aref_time;
	if (buffer->nTimeStamp < ref_time) {
		if ((buffer->nTimeStamp >> 12) > count) {
			OSCL_LOGI("%d drop frame time:%lld ref:%lld", port_idx,
				(long long)buffer->nTimeStamp, (long long)ref_time);
			count++;
		}
		return -1;
	}

	return base_port_recive_buffer(port, buffer);
}

OMX_ERRORTYPE muxer_set_parameter(OMX_IN OMX_HANDLETYPE hComp,
	OMX_IN OMX_INDEXTYPE paramIndex,
	OMX_IN OMX_PTR paramData)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	lb_omx_component_t *component;
	MuxerContext *context = NULL;
	AV_CodecParameters *codec_para;
	OSCL_TRACE(" %p, %p , %x\n", hComp, paramData, paramIndex);

	oscl_param_check((hComp != NULL) && (paramData != NULL),
		OMX_ErrorBadParameter, NULL);
	component = get_lb_component(hComp);
	oscl_param_check((component != NULL), OMX_ErrorBadParameter, NULL);
	context = component->component_private;

	OSCL_TRACE(" %x\n", paramIndex);
	switch (paramIndex) {
	case omx_index_vendor_set_max_duration: {
		pthread_mutex_lock(&context->context_mutex);
		context->file_total_time = (*((int *)paramData)) * 1000;
		pthread_mutex_unlock(&context->context_mutex);
		break;
	}
	case omx_index_vendor_output_filename:
		pthread_mutex_lock(&context->context_mutex);
		OSCL_TRACE(" %x\n", paramIndex);
		if (context->filename == NULL)
			context->filename = oscl_malloc(FILE_NAME_MAX_LENTH);
		if (paramData != NULL && context->filename != NULL)
			strcpy(context->filename, (char *)paramData);
		OSCL_TRACE(" %x\n", paramIndex);
		context->new_muxer_flag = 1;
		context->stream_out_type = STREAM_OUTPUT_TYPE_FILE;
		pthread_mutex_unlock(&context->context_mutex);
		break;
	case OMX_IndexParamAudioPcm: {
		pthread_mutex_lock(&context->context_mutex);
		OSCL_TRACE(" %x\n", paramIndex);
		OMX_AUDIO_PARAM_PCMMODETYPE *audio_params =
			(OMX_AUDIO_PARAM_PCMMODETYPE *)paramData;
		context->parameters[AUDIO_INDEX].codec_type = AV_MEDIA_TYPE_AUDIO;
		context->parameters[AUDIO_INDEX].codec_id = AV_CODEC_TYPE_PCM_S16LE;
		context->parameters[AUDIO_INDEX].sample_rate =
			audio_params->nSamplingRate;
		context->parameters[AUDIO_INDEX].channels = audio_params->nChannels;
		context->parameters[AUDIO_INDEX].channel_layout =
			audio_params->nChannels == 2 ? 3 : 4;
		context->parameters[AUDIO_INDEX].frame_size =  BLKSIZE;
		context->parameters[AUDIO_INDEX].block_align = BLKSIZE *
			audio_params->nChannels * 2;
		context->stream_exist[AUDIO_INDEX] = 1;
		pthread_mutex_unlock(&context->context_mutex);
	}
	break;
	case OMX_IndexParamAudioAdpcm: {
		pthread_mutex_lock(&context->context_mutex);
		OSCL_TRACE(" %x\n", paramIndex);
		OMX_AUDIO_PARAM_ADPCMTYPE *audio_params =
			(OMX_AUDIO_PARAM_ADPCMTYPE *)paramData;
		context->parameters[AUDIO_INDEX].codec_type = AV_MEDIA_TYPE_AUDIO;
		context->parameters[AUDIO_INDEX].codec_id = AV_CODEC_TYPE_ADPCM_IMA_WAV;
		context->parameters[AUDIO_INDEX].sample_rate = audio_params->nSampleRate;
		context->parameters[AUDIO_INDEX].channels = audio_params->nChannels;
		context->parameters[AUDIO_INDEX].channel_layout =
			audio_params->nChannels == 2 ? 3 : 4;
		context->parameters[AUDIO_INDEX].frame_size = (BLKSIZE  - 4) * 2 + 1;
		context->parameters[AUDIO_INDEX].block_align = BLKSIZE *
			audio_params->nChannels;
		OSCL_TRACE(" %x\n", paramIndex);
		context->stream_exist[AUDIO_INDEX] = 1;
		pthread_mutex_unlock(&context->context_mutex);
	}

	break;
	case OMX_IndexParamAudioAac: {
		pthread_mutex_lock(&context->context_mutex);
		OSCL_TRACE(" %x\n", paramIndex);
		OMX_AUDIO_PARAM_AACPROFILETYPE *audio_params =
			(OMX_AUDIO_PARAM_AACPROFILETYPE *)paramData;
		context->parameters[AUDIO_INDEX].codec_type = AV_MEDIA_TYPE_AUDIO;
		context->parameters[AUDIO_INDEX].codec_id = AV_CODEC_TYPE_AAC;
		context->parameters[AUDIO_INDEX].channels = audio_params->nChannels;
		context->parameters[AUDIO_INDEX].sample_rate = audio_params->nSampleRate;
		OSCL_TRACE(" %x\n", paramIndex);
		context->stream_exist[AUDIO_INDEX] = 1;
		pthread_mutex_unlock(&context->context_mutex);
	}
	break;
	case OMX_IndexParamAudioG729: {
		pthread_mutex_lock(&context->context_mutex);
		OMX_AUDIO_PARAM_G729TYPE *audio_params;
		audio_params = (OMX_AUDIO_PARAM_G729TYPE *)paramData;
		context->parameters[AUDIO_INDEX].codec_type = AV_MEDIA_TYPE_AUDIO;
		context->parameters[AUDIO_INDEX].codec_id = AV_CODEC_TYPE_G729;
		context->parameters[AUDIO_INDEX].channels = audio_params->nChannels;
		context->parameters[AUDIO_INDEX].sample_rate = 8000;
		OSCL_TRACE(" %x\n", paramIndex);
		context->stream_exist[AUDIO_INDEX] = 1;
		pthread_mutex_unlock(&context->context_mutex);
	}
	break;
	/* case OMX_IndexParamVideoAvc: {
		OMX_VIDEO_PARAM_AVCTYPE *video_params =
			(OMX_VIDEO_PARAM_AVCTYPE *)paramData;
		muxer->para[VIDEO_INDEX].codec_type = AV_MEDIA_TYPE_VIDEO;
		muxer->para[VIDEO_INDEX].codec_id = AV_CODEC_TYPE_H264;
	}
	break; */
	case OMX_IndexParamPortDefinition: {
		OMX_PARAM_PORTDEFINITIONTYPE *port_def =
			(OMX_PARAM_PORTDEFINITIONTYPE *)paramData;
		OMX_VIDEO_PORTDEFINITIONTYPE *video;

		OSCL_TRACE(" %x\n", paramIndex);
		switch (port_def->eDomain) {
		case OMX_PortDomainVideo:
			pthread_mutex_lock(&context->context_mutex);
			video = &port_def->format.video;
			OSCL_LOGI("eCompressionFormat:%d, w:%d, h:%d, framerate:%d",
				video->eCompressionFormat,
				video->nFrameWidth,
				video->nFrameHeight,
				video->xFramerate);
			codec_para = &context->parameters[VIDEO_INDEX];
			codec_para->codec_type = AV_MEDIA_TYPE_VIDEO;
			if (video->eCompressionFormat == OMX_VIDEO_CodingAVC)
				codec_para->codec_id = AV_CODEC_TYPE_H264;
			else
				codec_para->codec_id = AV_CODEC_TYPE_INVALID;

			codec_para->width = video->nFrameWidth;
			codec_para->height = video->nFrameHeight;
			codec_para->sample_rate =
				(video->xFramerate / 1000) * VIDEOTIMEFRAME;
			if (codec_para->sample_rate == 0)
				codec_para->sample_rate = 25 * VIDEOTIMEFRAME;
			context->stream_exist[VIDEO_INDEX] = 1;
			pthread_mutex_unlock(&context->context_mutex);
			break;
		default:
			ret = OMX_ErrorBadParameter;
			break;
		}
		ret = base_set_parameter(hComp, paramIndex, paramData);
	}
	OSCL_TRACE(" %x\n", paramIndex);
	break;
	case omx_index_lombo_set_stream_callback:
		context->ext_writer = *((external_stream_writer_t *)paramData);
		context->stream_out_type = STREAM_OUTPUT_TYPE_EXTERNAL;
		context->new_muxer_flag = 1;
		OSCL_LOGI("set_stream_callback %p", context->ext_writer.write);
		break;
	case omx_index_lombo_set_muxer_format:
		context->id = *((int *)paramData);
		OSCL_LOGI("set_muxer_format %d", context->id);
		break;
	case omx_index_lombo_set_muxer_nb_streams:
		context->num_stream = *((int *)paramData);
		OSCL_LOGI("set_muxer_nb_streams %d", context->num_stream);
		break;
	case omx_index_lombo_set_muxer_cache_sw:
		context->use_cache_flag = *((int *)paramData);
		if (context->use_cache_flag > MUXER_CACHE_USE)
			context->use_cache_flag = MUXER_CACHE_NOT_USE;
		OSCL_LOGI("muxer use cache switch: %d, %d", context->use_cache_flag,
			*((int *)paramData));
		break;
	default:
		OSCL_TRACE(" %x\n", paramIndex);
		ret = base_set_parameter(hComp, paramIndex, paramData);
		break;
	}
	OSCL_TRACE(" %x\n", paramIndex);

	OSCL_TRACE(" %d\n", ret);
	return ret;
}

static void *muxer_pkt_write_thread(void *parm)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	muxer_cache_t *muxer_cache = (muxer_cache_t *)parm;
	lb_omx_component_t *component = NULL;
	packet_t *p_pkt = NULL;
	component = (lb_omx_component_t *)(muxer_cache->muxer_handle->component);


	while (1) {
		if (muxer_cache->exit_thread_flag == 1)
			break;
		if (muxer_cache->start_mux_flag == 0) {
			oscl_mdelay(10);
			continue;
		}
		p_pkt = oscl_queue_dequeue(&muxer_cache->pkt_queue);
		if (p_pkt == NULL) {
			if (muxer_cache->close_flag == MUXER_CLOSEING) {
				OSCL_LOGI("send sem: %p", muxer_cache);
				sem_post(&(muxer_cache->file_close_sem));
			}
			oscl_mdelay(10);
			continue;
		}
		ret = muxer_write_packet(muxer_cache->muxer_handle, p_pkt);
		if (ret < 0)
			OSCL_LOGE("err ret :%d\n", ret);
		oscl_free(p_pkt->data);
		oscl_free(p_pkt);
	}
	pthread_exit(NULL);
	return NULL;
}

static int32_t muxer_cache_init(MuxerContext *context)
{
	pthread_attr_t thread_attr;
	struct sched_param shed_param = {0};
	muxer_cache_t *muxer_cache = NULL;
	int i = 0;

	pthread_attr_init(&thread_attr);
	shed_param.sched_priority = LB_FILE_CLOSE_PRIO;
	pthread_attr_setstacksize(&thread_attr, 0x1000);
	pthread_attr_setschedparam(&thread_attr, &shed_param);

	for (i = 0; i < MUXER_NUM; i++) {
		muxer_cache = &(context->muxer_cache_sz[i]);
		OSCL_LOGI("muxer_cache = %p", muxer_cache);
		memset(muxer_cache, 0, sizeof(muxer_cache_t));
		muxer_cache->write_cache_thread_id = -1;
		muxer_cache->muxer_handle = &(context->muxer_handle_sz[i]);
		oscl_queue_init(&muxer_cache->pkt_queue);
		sem_init(&(muxer_cache->file_close_sem), 0, 0);
		muxer_cache->write_cache_thread_id = pthread_create(
				&(muxer_cache->write_cache_thread), &thread_attr,
				muxer_pkt_write_thread, muxer_cache);
		if (muxer_cache->write_cache_thread_id < 0) {
			oscl_queue_deinit(&muxer_cache->pkt_queue);
			OSCL_LOGE("creat write cache thread error : %d\n", i);
			return -1;
		}
	}

	return 0;
}
static int32_t muxer_cache_deinit(MuxerContext *context)
{
	muxer_cache_t *muxer_cache = NULL;
	int i = 0;

	if (context->use_cache_flag != MUXER_CACHE_USE)
		return 0;

	for (i = 0; i < MUXER_NUM; i++) {
		muxer_cache = &(context->muxer_cache_sz[i]);
		if (muxer_cache->write_cache_thread_id == 0) {
			muxer_cache->exit_thread_flag = 1;
			pthread_join(muxer_cache->write_cache_thread, NULL);
			muxer_cache->write_cache_thread_id = -1;
			oscl_queue_deinit(&muxer_cache->pkt_queue);
			sem_destroy(&muxer_cache->file_close_sem);
		}
	}

	return 0;
}

OMX_ERRORTYPE muxer_set_state(OMX_HANDLETYPE hComp,
	OMX_U32 dest_state)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	lb_omx_component_t *component = NULL;
	MuxerContext *context = NULL;

	OMX_STATETYPE old_state;

	OSCL_TRACE("%p, %x\n", hComp, dest_state);
	oscl_param_check(hComp != NULL, OMX_ErrorBadParameter, NULL);
	component = get_lb_component(hComp);
	old_state = component->state;
	context = component->component_private;

	if (context->use_cache_flag &&
			(old_state == OMX_StateIdle) &&
			(dest_state == OMX_StateExecuting)) {
			ret = muxer_cache_init(context);
	}

	ret = base_component_set_state(hComp, dest_state);
	if ((old_state == OMX_StateExecuting) && (component->state == OMX_StateIdle)) {
		if (context->muxer_handle != NULL) {
			if (context->muxer_handle->muxer_status == MUXER_MUXERING) {
#ifdef __EOS__
				int prio = LB_FILE_CLOSE_PRIO;
#endif
				context->muxer_handle->muxer_status = MUXER_CLOSEING;
#ifdef __EOS__
				oscl_cfile_set_prio(context->fp, prio);
#endif
				sem_post(&(context->muxer_handle->file_close_sem));
				wait_file_close_finish(context);
				context->muxer_handle = NULL;
			} else
				OSCL_LOGE("muxer not need close, need debug\n");
			context->aref_time = INT_MAX;
			context->stream_exist[AUDIO_INDEX] = 0;
			context->stream_exist[VIDEO_INDEX] = 0;
		}
		muxer_cache_deinit(context);
		context->err_flag = 0;
	}
	return ret;
}


OMX_ERRORTYPE muxer_component_deinit(OMX_IN OMX_HANDLETYPE hComponent)
{
	OMX_COMPONENTTYPE *base_cmp = (OMX_COMPONENTTYPE *)hComponent;
	lb_omx_component_t *component = NULL;
	MuxerContext *context = NULL;
	format_muxer *muxer_handle = NULL;
	int i = 0, j = 0;

	OSCL_TRACE("base_cmp_handle:%p\n", hComponent);
	oscl_param_check(hComponent != NULL, OMX_ErrorBadParameter, NULL);
	component = (lb_omx_component_t *)base_cmp->pComponentPrivate;
	context = component->component_private;

	for (i = 0; i < MUXER_NUM; i++) {
		muxer_handle = &(context->muxer_handle_sz[i]);

		if (muxer_handle->file_close_thread_id == 0) {
			muxer_handle->exit_thread_flag = 1;
			sem_post(&muxer_handle->file_close_sem);
			pthread_join(muxer_handle->file_close_thread, NULL);
			muxer_handle->file_close_thread_id = -1;
		}
		sem_destroy(&muxer_handle->file_close_sem);
	}
	muxer_cache_deinit(context);

	for (i = 0; i < MUXER_NUM; i++) {
		muxer_handle = &(context->muxer_handle_sz[i]);
		if (muxer_handle->muxer_status != MUXER_INIT)
			OSCL_LOGE("muxer is busy, need debug\n");
	}

	pthread_mutex_destroy(&context->context_mutex);
	/* free extradata buffer */
	for (i = 0; i < MUXER_NUM; i++) {
		muxer_handle = &context->muxer_handle_sz[i];
		for (j = 0; j < context->num_stream; j++) {
			if (muxer_handle->para[j].extradata) {
				oscl_free(muxer_handle->para[j].extradata);
				muxer_handle->para[j].extradata = NULL;
				muxer_handle->para[j].extradata_size = 0;
			}
		}
		if (muxer_handle->filename != NULL) {
			free(muxer_handle->filename);
			muxer_handle->filename = NULL;
		}
	}
	if (context->filename != NULL) {
		free(context->filename);
		context->filename = NULL;
	}

	for (i = 0; i < context->num_stream; i++) {
		if (context->parameters[i].extradata) {
			oscl_free(context->parameters[i].extradata);
			context->parameters[i].extradata = NULL;
			context->parameters[i].extradata_size = 0;
		}
	}

	/* deinit port */
	component->port[AUDIO_INDEX].deinit(
		&component->port[AUDIO_INDEX]);
	component->port[VIDEO_INDEX].deinit(
		&component->port[VIDEO_INDEX]);

	if (context->combine_packet.vir_addr != NULL) {
		oscl_free(context->combine_packet.vir_addr);
		context->combine_packet.vir_addr = NULL;
		context->combine_packet.size = 0;
		context->rebuilding_flag = 0;
	}
	/* free context */
	if (NULL != context) {
		oscl_free(context);
		component->component_private = NULL;
	}
	base_component_deinit(hComponent);

	return OMX_ErrorNone;
}


static void *muxer_file_close_thread(void *parm)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	format_muxer *muxer_handle = (format_muxer *)parm;
	lb_omx_component_t *component = (lb_omx_component_t *)(muxer_handle->component);
	MuxerContext *context = component->component_private;
	muxer_cache_t *p_cahe = NULL;
	int32_t i;

	while (1) {
		if (muxer_handle->exit_thread_flag == 1)
			break;
		sem_wait(&muxer_handle->file_close_sem);
		if (muxer_handle->fp != FNULL) {
			/*flush queue*/
			if (context->use_cache_flag == MUXER_CACHE_USE) {
				for (i = 0; i < MUXER_NUM; i++) {
					if (muxer_handle ==
						context->muxer_cache_sz[i].muxer_handle){
						p_cahe = &(context->muxer_cache_sz[i]);
						break;
					}
				}
				if (p_cahe) {
					/*waitting for writting data finished*/
					p_cahe->close_flag = MUXER_CLOSEING;
					OSCL_LOGI("wait sem: %p", p_cahe);
					sem_wait(&(p_cahe->file_close_sem));
					p_cahe->close_flag = MUXER_INIT;
					p_cahe->start_mux_flag = 0;
				}
			}
			/*get user data e g gps data*/
			muxer_handle->user_data.is_valid = 0;
			((component->callbacks.EventHandler)) (&component->base_comp,
					component->callback_data,  OMX_Eventgetuserdata,
					0, (OMX_U32)&muxer_handle->user_data.size,
					&muxer_handle->user_data.pdata);

			OSCL_LOGI("user data: %s, size(%d)",
				muxer_handle->user_data.pdata,
				muxer_handle->user_data.size);
			if (muxer_handle->user_data.size &&
				muxer_handle->user_data.pdata) {
				muxer_handle->user_data.is_valid = 1;
			}
			/*OSCL_LOGE("muxer_write_trailer: %d\n", muxer_handle->fp);*/
			ret = muxer_write_trailer(muxer_handle);
			if (ret != OMX_ErrorNone)
				OSCL_LOGE("muxer_write_trailer error\n");
			oscl_cfile_close(muxer_handle->fp);
			muxer_handle->fp = FNULL;
			((component->callbacks.EventHandler))(&component->base_comp,
				component->callback_data,  OMX_Eventfileclosed,
				0, 0, muxer_handle->filename);
			/*start new muxer*/
			if (context->use_cache_flag == MUXER_CACHE_USE) {
				OSCL_LOGI("new pkt queue number: %d, %s",
					oscl_message_count(
					&context->muxer_cache->pkt_queue),
					muxer_handle->filename);
				context->muxer_cache->start_mux_flag = 1;
			}
		}
		if (muxer_handle->muxer_status == MUXER_MUXERING) {
			OSCL_LOGE("error muxer status\n");
			muxer_handle->muxer_status = MUXER_CLOSEING;
		}
		if (muxer_handle->muxer_status == MUXER_CLOSEING) {
			ret = muxer_deinit(muxer_handle);
			if (ret != OMX_ErrorNone)
				OSCL_LOGE("muxer_deinit error\n");
			muxer_handle->muxer_status = MUXER_INIT;
		}
	}
	pthread_exit(NULL);
	return NULL;
}
OMX_ERRORTYPE muxer_component_init(lb_omx_component_t *cmp_handle)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	int i = 0;
	MuxerContext *context = NULL;
	format_muxer *muxer_handle = NULL;
	struct sched_param shed_param = {0};

	context = oscl_zalloc(sizeof(MuxerContext));
	oscl_param_check_exit((context != NULL),
		OMX_ErrorInsufficientResources, NULL);
	ret = base_component_init(cmp_handle);

	pthread_attr_setstacksize(&cmp_handle->buf_thread_attr, 0x2000);
	pthread_attr_getschedparam(&cmp_handle->buf_thread_attr, &shed_param);
	OSCL_LOGI("priority:%d", shed_param.sched_priority);
	pthread_attr_setschedparam(&cmp_handle->buf_thread_attr, &shed_param);
	oscl_param_check_exit((ret == OMX_ErrorNone), ret, NULL);
	cmp_handle->name = "OMX.LB.SINK.MUXER";
	cmp_handle->component_private = context;
	cmp_handle->buf_manager = muxer_buffer_manager;
	cmp_handle->buf_handle = muxer_buffer_handle;
	cmp_handle->base_comp.ComponentDeInit = muxer_component_deinit;
	cmp_handle->base_comp.SetParameter = muxer_set_parameter;
	cmp_handle->base_comp.SetConfig = muxer_set_config;
	cmp_handle->base_comp.GetConfig = muxer_get_config;
	cmp_handle->do_state_set = muxer_set_state;

	cmp_handle->num_ports = 2;
	ret = base_port_init(cmp_handle, &cmp_handle->port[AUDIO_INDEX],
			AUDIO_INDEX,
			OMX_DirInput);
	if (ret != OMX_ErrorNone) {
		base_component_deinit(cmp_handle);
		return ret;
	}
	ret = base_port_init(cmp_handle, &cmp_handle->port[VIDEO_INDEX],
			VIDEO_INDEX,
			OMX_DirInput);
	if (ret != OMX_ErrorNone) {
		base_component_deinit(cmp_handle);
		cmp_handle->port[AUDIO_INDEX].deinit(
			&cmp_handle->port[AUDIO_INDEX]);
		return ret;
	}

	cmp_handle->port[AUDIO_INDEX].port_param.nBufferSize = SINK_INPUT_DEFAULT_SIZE;
	cmp_handle->port[AUDIO_INDEX].port_param.eDomain = OMX_PortDomainAudio;
	cmp_handle->port[AUDIO_INDEX].recive_buffer = muxer_port_recive_buffer;
	cmp_handle->port[VIDEO_INDEX].port_param.nBufferSize = OMX_VIDEO_BUF_SIZE;
	cmp_handle->port[VIDEO_INDEX].port_param.eDomain = OMX_PortDomainVideo;

	pthread_mutex_init(&context->context_mutex, NULL);
	context->aref_time = INT_MAX;
	context->fp = FNULL;
	context->filename = NULL;
	context->parameters[0].codec_type = AV_MEDIA_TYPE_INVALID;
	context->parameters[1].codec_type = AV_MEDIA_TYPE_INVALID;
	context->num_stream = 2;
	context->id = AV_MUXER_TYPE_MOV;
	context->stream_out_type = STREAM_OUTPUT_TYPE_FILE;
	for (i = 0; i < MUXER_NUM; i++) {
		pthread_attr_t thread_attr;
		struct sched_param shed_param = {0};
		muxer_handle = &(context->muxer_handle_sz[i]);
		memset(muxer_handle, 0, sizeof(format_muxer));
		muxer_handle->para[0].codec_type = AV_MEDIA_TYPE_INVALID;
		muxer_handle->para[1].codec_type = AV_MEDIA_TYPE_INVALID;
		muxer_handle->filename = NULL;
		muxer_handle->fp = FNULL;
		sem_init(&(muxer_handle->file_close_sem), 0, 0);
		muxer_handle->component = cmp_handle;
		pthread_attr_init(&thread_attr);
#ifdef __EOS__
		shed_param.sched_priority = LB_FILE_CLOSE_PRIO;
		pthread_attr_setstacksize(&thread_attr, 0x1000);
#endif
		pthread_attr_setschedparam(&thread_attr, &shed_param);
		muxer_handle->file_close_thread_id = pthread_create(
				&(muxer_handle->file_close_thread), &thread_attr,
				muxer_file_close_thread, muxer_handle);

		if (muxer_handle->file_close_thread_id < 0) {
			OSCL_LOGE("creat file close thread error : %d\n", i);
			ret = OMX_ErrorInsufficientResources;
			goto EXIT;
		}
	}

EXIT:
	if (ret != OMX_ErrorNone) {
		if (context != NULL)
			oscl_free(context);
	}
	OSCL_TRACE("%d\n", ret);
	return ret;
}

#ifdef OMX_DYNAMIC_LOADING
void *omx_component_init = muxer_component_init;
#endif

