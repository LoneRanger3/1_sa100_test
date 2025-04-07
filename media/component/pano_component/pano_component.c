/*
 * pano_component.c - code for pano component.
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

#define DBG_LEVEL		DBG_INFO

#include <oscl.h>
#include <base_component.h>
#include "pano_component.h"
#include "lb_pano_intf.h"

typedef struct pano_buffer {
	OMX_BUFFERHEADERTYPE *inbuf;
	OMX_BUFFERHEADERTYPE *outbuf;
	OMX_S32 stop;
} pano_buffer_t;

typedef struct pano_private {
	OMX_S32 en_cali;       /* calibration switch*/
	OMX_S32 ext_cali_img;  /* 1: use extern calibration image,
				* 0: use internal camera image calibration */
	OMX_S32 ext_cutline;  /* 1: use extern cutline value,
				* 0: use internal cutline value */
	OMX_S32 cutline;

	OMX_S32 cali_complete; /* calibration complete flag */
	OMX_S32 pano_creat_flag;
	OMX_S32 pano_start;

	pano_img_t birdbiew_img;

	HDCalibOut out; /* calibration output data */
	HDCalibIn in;   /* calibration input data */
	HDIniSetPano ini_set_pano; /* pano init para */
	HDIniGetPano ini_get_pano;

	OMX_U32	inbuf_offset[3];
	OMX_U32	outbuf_offset[3];

	sem_t *enter_proc_sem;
	OMX_S32 stop_pano;

	oscl_queue_t pano_buf_queue;
	pano_buffer_t stop_buf;
} pano_private_t;

#define DEFAULT_VIDEO_WIDTH		(1280)
#define DEFAULT_VIDEO_HEIGHT		(720)
#define DEFAULT_BUFFER_NUM		(8)
#define DEFAULT_BUFFER_ALIGN_SIZE	(1024)
#define MAX_STATISTIC_NUM		(10)

#define DEBUG_ROOT_PATH "/mnt/sdcard"
#define DEBUG_PANO_CALI_OUT_DATA
#define DEBUG_PANO_CALI_BIRDBIEW_IMAG

static lb_omx_component_t *pano_component;
static pano_buffer_t cur_pano_buf;
static OMX_S32 g_wast[MAX_STATISTIC_NUM];
static OMX_S32 g_proc_start_time;

void pano_buf_return(lb_omx_component_t *component,
	OMX_BUFFERHEADERTYPE *inbuffer, OMX_BUFFERHEADERTYPE *outbuffer)
{
	base_port_t *inport = NULL;
	base_port_t *outport = NULL;

	inport = &component->port[PANO_INPUT_PORT_INDEX];
	outport = &component->port[PANO_OUTPUT_PORT_INDEX];
	if (inbuffer->hMarkTargetComponent != NULL) {
		if ((OMX_COMPONENTTYPE *)inbuffer->hMarkTargetComponent
				== &component->base_comp) {
				/*Clear the mark and generate an event*/
				((component->callbacks.EventHandler))
				(&component->base_comp,
					component->callback_data, OMX_EventMark,
					0, 0, inbuffer->pMarkData);
			} else {
				/* pass the mark*/
				outbuffer->hMarkTargetComponent
					= inbuffer->hMarkTargetComponent;
				outbuffer->pMarkData
					= inbuffer->pMarkData;
			}
			inbuffer->hMarkTargetComponent = NULL;
	}
	base_check_eos(component, outport, outbuffer);
	if ((outbuffer->nFilledLen != 0)
		|| (outbuffer->nFlags & OMX_BUFFERFLAG_EOS)) {
		outport->return_buffer(outport, outbuffer);
	}
	if (inbuffer->nFilledLen == 0)
		inport->return_buffer(inport, inbuffer);
}

OMX_ERRORTYPE pano_buf_add(pano_private_t *pano_private,
	OMX_BUFFERHEADERTYPE *inbuffer, OMX_BUFFERHEADERTYPE *outbuffer)
{
	pano_buffer_t *pano_buf = NULL;

	oscl_param_check((inbuffer != NULL), OMX_ErrorInsufficientResources, NULL);
	oscl_param_check((outbuffer != NULL), OMX_ErrorInsufficientResources, NULL);
	pano_buf = oscl_zalloc(sizeof(pano_buffer_t));
	oscl_param_check((pano_buf != NULL), OMX_ErrorInsufficientResources, NULL);
	inbuffer->nFilledLen = 0;
	outbuffer->nFilledLen = 0;
	pano_buf->inbuf = inbuffer;
	pano_buf->outbuf = outbuffer;
	oscl_queue_queue(&pano_private->pano_buf_queue, pano_buf);
	sem_post(pano_private->enter_proc_sem);

	return OMX_ErrorNone;
}

OMX_ERRORTYPE pano_buf_pop(pano_private_t *pano_private,
	OMX_BUFFERHEADERTYPE **inbuffer, OMX_BUFFERHEADERTYPE **outbuffer)
{
	pano_buffer_t *pano_buf = NULL;

	pano_buf = oscl_queue_dequeue(&pano_private->pano_buf_queue);
	if (pano_buf) {
		sem_trywait(pano_private->enter_proc_sem);
		*inbuffer = pano_buf->inbuf;
		*outbuffer = pano_buf->outbuf;
		oscl_free(pano_buf);
		return OMX_ErrorNone;
	}

	return OMX_ErrorInsufficientResources;
}

OMX_ERRORTYPE set_cali_checkerb(HDCheckerBoard *checkerb, pano_cali_para_t *para)
{
	if (checkerb->preViewWidth > 0 && checkerb->preViewHeight > 0) {
		if ((para->preview_width != checkerb->preViewWidth) ||
			(para->preview_height != checkerb->preViewHeight))
			return OMX_ErrorBadParameter;
	}
	/* Multimedia needs to provide these interface to the app */
	pano_set_cb_boxrows(checkerb, para->box_rows);
	pano_set_cb_boxcols(checkerb, para->box_cols);
	pano_set_cb_boxwidth(checkerb, para->box_width);
	pano_set_cb_boxheight(checkerb, para->box_height);
	pano_set_cb_dist2rear(checkerb, para->dist_2_rear);
	pano_set_cb_carwidth(checkerb, para->car_width);
	pano_set_cb_carlong(checkerb, para->car_length);
	pano_set_cb_previeww(checkerb, para->preview_width);
	pano_set_cb_previewh(checkerb, para->preview_height);
	pano_set_cb_frontdist(checkerb, para->front_dist);
	pano_set_cb_reardist(checkerb, para->rear_dist);
	pano_set_cb_align(checkerb, para->align);

	return OMX_ErrorNone;
}

OMX_ERRORTYPE set_pano_init_para(HDIniSetPano *inip, pano_init_para_t *para)
{
	/* Multimedia needs to provide these interface to the app */
	pano_set_inis_mod_gps(inip, para->in_gps);
	pano_set_inis_mod_obd(inip, para->in_obd);
	pano_set_inis_carp_en(inip, para->car_para_en);
	pano_set_inis_carp_w(inip, para->car_width);
	pano_set_inis_data_f(inip, para->data_format);
	pano_set_inis_warning_line(inip,
		(int *)para->warning_line, ARRARSIZE(para->warning_line));
	return OMX_ErrorNone;
}

OMX_ERRORTYPE set_ext_image_data(IMGYUVC *yuv, pano_img_t *img)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	OMX_S32 fd = 0;
	OMX_S32 size = 0;
	OMX_S32 rd_size = 0;
	OMX_PTR y = NULL;
	OMX_PTR uv = NULL;

	oscl_param_check((img->format ==
			OMX_COLOR_FormatYUV420SemiPlanar),
			OMX_ErrorBadParameter, NULL);
	if (yuv->yP) {
		oscl_free_align(yuv->yP);
		yuv->yP = NULL;
	}
	if (yuv->uvP) {
		oscl_free_align(yuv->uvP);
		yuv->uvP = NULL;
	}
	size = img->width * img->height;
	oscl_param_check((size > 0), OMX_ErrorBadParameter, NULL);
	fd = open((const char *)img->path, O_RDONLY);
	if (fd < 0) {
		OSCL_LOGE("fail to open file: %s\n", img->path);
		return OMX_ErrorInsufficientResources;
	}
	y = oscl_malloc_align(size + 16, 64);
	uv = oscl_malloc_align((size >> 1) + 16, 64);
	oscl_param_check_exit((y != NULL && uv != NULL),
			OMX_ErrorInsufficientResources, NULL);
	rd_size = read(fd, y, size);
	oscl_param_check_exit((rd_size == size), OMX_ErrorInsufficientResources, NULL);
	rd_size = read(fd, uv, size >> 1);
	oscl_param_check_exit((rd_size == (size >> 1)),
			OMX_ErrorInsufficientResources, NULL);
	yuv->yP = y;
	yuv->uvP = uv;
	yuv->size.width = img->width;
	yuv->size.height = img->height;
	close(fd);

	return OMX_ErrorNone;
EXIT:
	if (y)
		oscl_free(y);
	if (uv)
		oscl_free(uv);
	if (fd >= 0)
		close(fd);
	return ret;
}

OMX_ERRORTYPE set_inte_image_data(OMX_HANDLETYPE hComp,
		IMGYUVC *yuv, OMX_BUFFERHEADERTYPE *img)
{
	lb_omx_component_t *component = NULL;
	pano_private_t *pano_private = NULL;
	base_port_t *inport = NULL;
	OMX_VIDEO_PORTDEFINITIONTYPE *video = NULL;
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	OMX_S32 size = 0;
	OMX_PTR y = NULL;
	OMX_PTR uv = NULL;

	component = get_lb_component(hComp);
	pano_private = component->component_private;
	inport = &component->port[PANO_INPUT_PORT_INDEX];

	video = &inport->port_param.format.video;
	oscl_param_check((video->eColorFormat ==
			OMX_COLOR_FormatYUV420SemiPlanar),
			OMX_ErrorBadParameter, NULL);
	if (yuv->yP) {
		oscl_free_align(yuv->yP);
		yuv->yP = NULL;
	}
	if (yuv->uvP) {
		oscl_free_align(yuv->uvP);
		yuv->uvP = NULL;
	}
	size = pano_private->inbuf_offset[0] + pano_private->inbuf_offset[1];
	oscl_param_check((size > 0), OMX_ErrorBadParameter, NULL);
	y = oscl_malloc_align(size + 16, 64);
	uv = oscl_malloc_align((size >> 1) + 16, 64);
	oscl_param_check_exit((y != NULL && uv != NULL),
			OMX_ErrorInsufficientResources, NULL);
	memcpy(y, img->pBuffer, pano_private->inbuf_offset[0]);
	memcpy(uv, img->pBuffer + pano_private->inbuf_offset[0],
			pano_private->inbuf_offset[1]);
	yuv->yP = y;
	yuv->uvP = uv;
	yuv->size.width = video->nStride;
	yuv->size.height = video->nSliceHeight;

	OSCL_LOGI("calibration img>> w:%d h:%d", yuv->size.width, yuv->size.height);
	return OMX_ErrorNone;
EXIT:
	if (y)
		oscl_free(y);
	if (uv)
		oscl_free(uv);
	return ret;
}

static void print_cali_in_param(HDCalibIn *in)
{
	printf("-----------------calibrate in para-----------------\n");
	printf("align:%d\n", in->checkerBoard.align);
	printf("boxCols:%d\n", in->checkerBoard.boxCols);
	printf("boxheight:%d\n", in->checkerBoard.boxheight);
	printf("boxRows:%d\n", in->checkerBoard.boxRows);
	printf("boxWidth:%d\n", in->checkerBoard.boxWidth);
	printf("carLong:%d\n", in->checkerBoard.carLong);
	printf("carWidth:%d\n", in->checkerBoard.carWidth);
	printf("dist2Rear:%d\n", in->checkerBoard.dist2Rear);
	printf("frontDist:%d\n", in->checkerBoard.frontDist);
	printf("preViewHeight:%d\n", in->checkerBoard.preViewHeight);
	printf("preViewWidth:%d\n", in->checkerBoard.preViewWidth);
	printf("rearDist:%d\n", in->checkerBoard.rearDist);
	printf("----------------------------------------------------\n");
}

static void print_cali_out_param(HDCalibOut *out)
{
	OMX_S32 i = 0;

	/* These paras need to be saved so that there is no need to calibrate next time */
	printf("-----------------calibrate out data-----------------\n");
	printf("mode:%d\n", out->calibPara.calibInPara.modeName);
	printf("f:%08f\n", out->calibPara.calibInPara.f);
	printf("cx:%08f\n", out->calibPara.calibInPara.cp.x);
	printf("cy:%08f\n", out->calibPara.calibInPara.cp.y);
	for (i = 0; i < CALIB_OUT_NUM; i++) {
		printf("Homo[%ld]:%08f ", i, out->calibPara.calibOutPara.hmgp[i]);
		if (i == (CALIB_OUT_NUM - 1))
			printf("\n");
	}
	printf("preViewWidth:%d\n", out->calibPara.preView.preViewWidth);
	printf("preViewHeight:%d\n", out->calibPara.preView.preViewHeight);
	printf("carBorder.up:%d\n", out->calibPara.preView.carBorder.up);
	printf("carBorder.dn:%d\n", out->calibPara.preView.carBorder.dn);
	printf("carBorder.lt:%d\n", out->calibPara.preView.carBorder.lt);
	printf("carBorder.rt:%d\n", out->calibPara.preView.carBorder.rt);
	printf("rearDist:%d\n", out->calibPara.preView.rearDist);
	printf("rearAppear:%d\n", out->calibPara.preView.cutLinePara.rearAppear);
	printf("cutLineUpThr:%d\n", out->calibPara.preView.cutLinePara.cutLineUpThr);
	printf("cutLine:%d\n", out->calibPara.preView.cutLinePara.cutLine);
	printf("cutLineDnThr:%d\n", out->calibPara.preView.cutLinePara.cutLineDnThr);
	printf("out.birdBiewImg.size.width:%d\n", out->birdBiewImg.size.width);
	printf("out.birdBiewImg.size.height:%d\n", out->birdBiewImg.size.height);
	printf("----------------------------------------------------\n");
}

#ifdef DEBUG_PANO_CALI_OUT_DATA
static OMX_ERRORTYPE save_cali_out_param(HDCalibOut *out)
{
	char path[80] = {0};
	char tmp_buf[128] = {0};
	void *data = NULL;
	OMX_S32 data_size = 0;
	OMX_S32 fd = 0;

	snprintf(path, sizeof(path),
		DEBUG_ROOT_PATH"/pano_cali_out_%d.txt", oscl_get_msec());
	data_size = sizeof(HDCalibParaPano) + 1024;
	data = oscl_zalloc(data_size);
	if (NULL == data) {
		OSCL_LOGE("Request memory fail.\n");
		return OMX_ErrorInsufficientResources;
	}
	fd = open(path, O_WRONLY | O_CREAT);
	if (fd < 0) {
		OSCL_LOGE("fail to open file: %s\n", path);
		oscl_free(data);
		return OMX_ErrorInsufficientResources;
	}
	memset(tmp_buf, 0, sizeof(tmp_buf));

	/* out->calibPara.calibInPara */
	sprintf(tmp_buf, "mode\t\t\t%d\r\n", out->calibPara.calibInPara.modeName);
	strcat(data, tmp_buf);
	sprintf(tmp_buf, "f\t\t\t\t%08f\r\n", out->calibPara.calibInPara.f);
	strcat(data, tmp_buf);
	sprintf(tmp_buf, "cx\t\t\t\t%08f\r\n", out->calibPara.calibInPara.cp.x);
	strcat(data, tmp_buf);
	sprintf(tmp_buf, "cy\t\t\t\t%08f\r\n", out->calibPara.calibInPara.cp.y);
	strcat(data, tmp_buf);
	sprintf(tmp_buf, "Homo\t\t\t%f %f %f %f %f %f %f %f\r\n",
			out->calibPara.calibOutPara.hmgp[0],
			out->calibPara.calibOutPara.hmgp[1],
			out->calibPara.calibOutPara.hmgp[2],
			out->calibPara.calibOutPara.hmgp[3],
			out->calibPara.calibOutPara.hmgp[4],
			out->calibPara.calibOutPara.hmgp[5],
			out->calibPara.calibOutPara.hmgp[6],
			out->calibPara.calibOutPara.hmgp[7]);
	strcat(data, tmp_buf);

	/* out->calibPara.preView */
	sprintf(tmp_buf, "preViewWidth\t%d\r\n", out->calibPara.preView.preViewWidth);
	strcat(data, tmp_buf);
	sprintf(tmp_buf, "preViewHeight\t%d\r\n", out->calibPara.preView.preViewHeight);
	strcat(data, tmp_buf);
	sprintf(tmp_buf, "preViewCarUp\t%d\r\n", out->calibPara.preView.carBorder.up);
	strcat(data, tmp_buf);
	sprintf(tmp_buf, "preViewCarDn\t%d\r\n", out->calibPara.preView.carBorder.dn);
	strcat(data, tmp_buf);
	sprintf(tmp_buf, "preViewCarLt\t%d\r\n", out->calibPara.preView.carBorder.lt);
	strcat(data, tmp_buf);
	sprintf(tmp_buf, "preViewCarRt\t%d\r\n", out->calibPara.preView.carBorder.rt);
	strcat(data, tmp_buf);
	sprintf(tmp_buf, "rearDist\t\t%d\r\n", out->calibPara.preView.rearDist);
	strcat(data, tmp_buf);
	sprintf(tmp_buf, "rearAppear\t\t%d\r\n",
			out->calibPara.preView.cutLinePara.rearAppear);
	strcat(data, tmp_buf);
	sprintf(tmp_buf, "cutLineUpThr\t%d\r\n",
			out->calibPara.preView.cutLinePara.cutLineUpThr);
	strcat(data, tmp_buf);
	sprintf(tmp_buf, "cutLine\t\t\t%d\r\n",
			out->calibPara.preView.cutLinePara.cutLine);
	strcat(data, tmp_buf);
	sprintf(tmp_buf, "cutLineDnThr\t%d\r\n",
			out->calibPara.preView.cutLinePara.cutLineDnThr);
	strcat(data, tmp_buf);

	write(fd, data, strlen(data));
	close(fd);
	oscl_free(data);

	return OMX_ErrorNone;
}
#endif

#ifdef DEBUG_PANO_CALI_BIRDBIEW_IMAG
static OMX_ERRORTYPE save_bird_biew(pano_img_t *img, IMGYUVC *bird_biew)
{
	OMX_S32 fd = 0;
	OMX_S32 width = 0, height = 0;
	void *bird_yP = NULL;
	void *bird_uvP = NULL;

	oscl_param_check((img != NULL), OMX_ErrorBadParameter, NULL);
	oscl_param_check((bird_biew != NULL), OMX_ErrorBadParameter, NULL);

	bird_yP = (void *)bird_biew->yP;
	bird_uvP = (void *)bird_biew->uvP;
	width = bird_biew->size.width;
	height = bird_biew->size.height;
	oscl_param_check((bird_yP != NULL), OMX_ErrorBadParameter, NULL);
	oscl_param_check((bird_uvP != NULL), OMX_ErrorBadParameter, NULL);
	oscl_param_check((width > 0), OMX_ErrorBadParameter, NULL);
	oscl_param_check((height > 0), OMX_ErrorBadParameter, NULL);

	/* These paras need to be saved so that there is no need to calibrate next time */
	fd = open((const char *)img->path, O_WRONLY | O_CREAT);
	if (fd < 0) {
		OSCL_LOGE("fail to open file: %s\n", img->path);
		return OMX_ErrorInsufficientResources;
	}

	write(fd, bird_yP, width * height);
	write(fd, bird_uvP , (width * height) >> 1);
	close(fd);
	img->format = OMX_COLOR_FormatYUV420SemiPlanar;
	img->width = width;
	img->height = height;

	return OMX_ErrorNone;
}
#endif

#if 0
static void save_video_frame(char *prefix, char *y, char *uv, int size)
{
	OMX_S32 fd = 0;
	char path[64] = {0};
	static OMX_S32 frm_count;

	rt_snprintf(path, sizeof(path), "/mnt/sdcard/%s_%d.yuv", prefix, frm_count);
	fd = open(path, O_WRONLY | O_CREAT);
	if (fd < 0) {
		LOG_E("fail to open file: %s", path);
		return;
	} else {
		LOG_W("open file success: %s", path);
	}

	write(fd, y, size);
	write(fd, uv, size >> 1);
	close(fd);
	frm_count++;
}
#endif

static void set_data_pano(HDSetFramePano *set_frame, void *dv)
{
	pano_private_t *pano_private = NULL;
	base_port_t *inport = NULL;
	base_port_t *outport = NULL;
	OMX_ERRORTYPE ret = 0;
	OMX_BUFFERHEADERTYPE *inbuf = NULL;
	OMX_BUFFERHEADERTYPE *outbuf = NULL;

	pano_private = pano_component->component_private;
	inport = &pano_component->port[PANO_INPUT_PORT_INDEX];
	outport = &pano_component->port[PANO_OUTPUT_PORT_INDEX];

	memset(&cur_pano_buf, 0, sizeof(cur_pano_buf));
	while (1) {
		if (pano_private->stop_pano) {
			inbuf = pano_private->stop_buf.inbuf;
			outbuf = pano_private->stop_buf.outbuf;
			cur_pano_buf.stop = 1;
			OSCL_LOGI("Set pano quit, in:%p out:%p", inbuf, outbuf);
			break;
		}
		cur_pano_buf.stop = 0;
		if (oscl_sem_timedwait_ms(pano_private->enter_proc_sem, 80)) {
			OSCL_LOGE("set_data_pano wait timeout.");
			OSCL_LOGE("pano_start:%d, en_cali:%d, cali_complete:%d, %d %d",
					pano_private->pano_start,
					pano_private->en_cali,
					pano_private->cali_complete,
					oscl_queue_get_num(&inport->buffer_queue),
					oscl_queue_get_num(&outport->buffer_queue));
			continue;
		}
		if (oscl_queue_get_num(&pano_private->pano_buf_queue) > 0) {
			ret = pano_buf_pop(pano_private, &inbuf, &outbuf);
			if (ret != OMX_ErrorNone)
				continue;
			else
				break;
		}
	}
	cur_pano_buf.inbuf = inbuf;
	cur_pano_buf.outbuf = outbuf;
	g_proc_start_time = oscl_get_msec();
	/* OSCL_LOGD("set_data_pano:%p", pano_buf); */
	/* need to fill the  yP from camera */
	set_frame->imgYuv[0].yP = inbuf->pBuffer;
	set_frame->imgYuv[0].uvP = inbuf->pBuffer +
				pano_private->inbuf_offset[0];
	set_frame->imgYuv[0].size.width =
		inport->port_param.format.video.nStride;
	set_frame->imgYuv[0].size.height =
		inport->port_param.format.video.nSliceHeight;
#if 0
	save_video_frame("pano_b", set_frame->imgYuv[0].yP,
		set_frame->imgYuv[0].uvP,
		set_frame->imgYuv[0].size.width *
		set_frame->imgYuv[0].size.height);
#endif
}

static void get_data_pano(HDGetFramePano *get_frame, void *dv)
{
	pano_buffer_t *buf = &cur_pano_buf;
	pano_private_t *pano_private = NULL;
	base_port_t *outport = NULL;
	OMX_PTR y = NULL;
	OMX_PTR uv = NULL;
	OMX_S32 w, h;
	OMX_S32 total_wast = 0;
	OMX_S32 i = 0;
	static OMX_S32 start_time;
	static OMX_S32 end_time;
	static OMX_S32 freq, speed;

	pano_private = pano_component->component_private;
	outport = &pano_component->port[PANO_OUTPUT_PORT_INDEX];

	if (NULL == buf->inbuf || NULL == buf->outbuf) {
		OSCL_LOGD("get_data_pano buf is null.");
		return;
	}
	y = buf->outbuf->pBuffer;
	uv = buf->outbuf->pBuffer + pano_private->outbuf_offset[0];
	w = outport->port_param.format.video.nStride;
	h = outport->port_param.format.video.nSliceHeight;

	/* the result of panoramic algorithm to show */
	RT_ASSERT(get_frame->imgPreView.yP != NULL);
	memcpy(y, get_frame->imgPreView.yP, w * h);

	RT_ASSERT(get_frame->imgPreView.uvP != NULL);
	memcpy(uv, get_frame->imgPreView.uvP, (w * h) >> 1);
	OSCL_LOGD("get_data_pano:%p", buf);

	buf->inbuf->nFilledLen = 0;
	buf->outbuf->nFilledLen = pano_private->outbuf_offset[0] +
				pano_private->outbuf_offset[1] +
				pano_private->outbuf_offset[2];
	if (!buf->stop)
		pano_buf_return(pano_component, buf->inbuf, buf->outbuf);
	end_time = oscl_get_msec();
	g_wast[freq % MAX_STATISTIC_NUM] = end_time - g_proc_start_time;
	freq++;
	if ((end_time - start_time) >= 1000) {
		for (i = 0; i < MAX_STATISTIC_NUM; i++) {
			if (g_wast[i] <= 0)
				break;
			total_wast += g_wast[i];
		}
		speed = i * 1000 / total_wast;
		printf("pan process freq:%ld, proc speed:%ld(%ld)\n", freq, speed, i);
		memset(g_wast, -1, sizeof(g_wast));
		freq = 0;
		start_time = end_time;
	}
#if 0
	OSCL_LOGW("imgpreview w:%d h:%d", w, h);
	save_video_frame("pano_a", get_frame->imgPreView.yP,
		get_frame->imgPreView.uvP, w * h);
#endif
}

void pan_creat_ex(pano_private_t *pano_private)
{
	void *dev = NULL;

	if (!pano_private->pano_creat_flag) {
		OSCL_LOGI("Creat pano. ext cutline:%d value:%d\n",
			pano_private->ext_cutline, pano_private->cutline);
		if (pano_private->ext_cutline) {
			pano_private->out.calibPara.preView.cutLinePara.cutLine =
					pano_private->cutline;
		}
		pano_set_inis_calipara(&pano_private->ini_set_pano,
				&pano_private->out);

		/* key function which is to set frame to pano from camera */
		SetDataPano = set_data_pano;
		/* key function which is to get frame from pano to display */
		GetDataPano = get_data_pano;
		pano_creat(&pano_private->ini_set_pano,
				&pano_private->ini_get_pano, &dev);
		pano_private->pano_creat_flag = 1;
		OSCL_LOGI("pano_creat over...");
	}

}

OMX_ERRORTYPE pano_set_parameter(OMX_IN OMX_HANDLETYPE hComp,
	OMX_IN OMX_INDEXTYPE paramIndex,
	OMX_IN OMX_PTR paramData)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	lb_omx_component_t *component = NULL;
	pano_private_t *pano_private = NULL;
	OMX_PARAM_PORTDEFINITIONTYPE *port_def = NULL;
	OMX_VIDEO_PORTDEFINITIONTYPE *video = NULL;
	OMX_BUFFERHEADERTYPE *inbuf = NULL;
	OMX_BUFFERHEADERTYPE *outbuf = NULL;
	base_port_t *inport = NULL;
	base_port_t *outport = NULL;
	OMX_BUFFERHEADERTYPE *stop_inbuf = NULL;
	OMX_BUFFERHEADERTYPE *stop_outbuf = NULL;

	component = get_lb_component(hComp);
	oscl_param_check((component != NULL), OMX_ErrorBadParameter, NULL);
	pano_private = component->component_private;
	inport = &component->port[PANO_INPUT_PORT_INDEX];
	outport = &component->port[PANO_OUTPUT_PORT_INDEX];

	switch (paramIndex) {
	case omx_index_pano_cb_process:
		while (1) {
			if (oscl_sem_timedwait_ms(pano_private->enter_proc_sem, 1500)) {
				OSCL_LOGE("wait video input fail.");
				return OMX_ErrorInsufficientResources;
			}
			if (oscl_queue_get_num(&pano_private->pano_buf_queue) > 0) {
				ret = pano_buf_pop(pano_private, &inbuf, &outbuf);
				if (ret != OMX_ErrorNone)
					continue;
				else
					break;
			}
		}
		pano_private->cali_complete = 0;
		OSCL_LOGI("Calibration ......");
		if (!pano_private->ext_cali_img)
			set_inte_image_data(hComp, &pano_private->in.imgYuv, inbuf);
		/* Caclulate the out parameter for panoramic image */
		print_cali_in_param(&pano_private->in);
		ret = pano_creat_calibrate(&pano_private->out, &pano_private->in);
		OSCL_LOGI("Calibration complete.");
		pano_delete_calibrate();
		if (0 == ret) {
			OSCL_LOGE("Calibration fail.");
			outport->recive_buffer(outport, outbuf);
			inport->return_buffer(inport, inbuf);
			return OMX_ErrorInsufficientResources;

		}
		ret = OMX_ErrorNone;
		memcpy(outbuf->pBuffer,
				pano_private->out.birdBiewImg.yP,
				pano_private->outbuf_offset[0]);
		memcpy(outbuf->pBuffer + pano_private->outbuf_offset[0],
			pano_private->out.birdBiewImg.uvP,
			pano_private->outbuf_offset[1]);
		outbuf->nFilledLen = pano_private->outbuf_offset[0] +
					pano_private->outbuf_offset[1] +
					pano_private->outbuf_offset[2];
		pano_buf_return(component, inbuf, outbuf);
		if (paramData) {
			pano_cali_contex_t *ctx = (pano_cali_contex_t *)paramData;
			HDCalibPreViewPano *prev =
				&pano_private->out.calibPara.preView;
			ctx->cutline_dnthr = prev->cutLinePara.cutLineDnThr;
			ctx->cutline_upthr = prev->cutLinePara.cutLineUpThr;
			ctx->cutline = prev->cutLinePara.cutLine;
			ctx->car_rect.left = prev->carBorder.lt;
			ctx->car_rect.top = prev->carBorder.up;
			ctx->car_rect.width = prev->carBorder.rt - prev->carBorder.lt;
			ctx->car_rect.height = prev->carBorder.dn - prev->carBorder.up;
		}
		print_cali_out_param(&pano_private->out);
#ifdef DEBUG_PANO_CALI_OUT_DATA
		save_cali_out_param(&pano_private->out);
#endif
#ifdef DEBUG_PANO_CALI_BIRDBIEW_IMAG
		strncpy((char *)pano_private->birdbiew_img.path,
			DEBUG_ROOT_PATH"/birdbiew.bin",
			sizeof(pano_private->birdbiew_img.path) - 1);
		save_bird_biew(&pano_private->birdbiew_img,
					&pano_private->out.birdBiewImg);
#endif
		pano_private->cali_complete = 1;
		break;
	case omx_index_pano_cb_para: {
		pano_cali_para_t *cali_para = (pano_cali_para_t *)paramData;

		oscl_param_check((cali_para != NULL), OMX_ErrorBadParameter, NULL);
		ret = set_cali_checkerb(&pano_private->in.checkerBoard, cali_para);
		oscl_param_check((ret == OMX_ErrorNone), OMX_ErrorBadParameter, NULL);
		pano_private->ext_cali_img = cali_para->use_ext_cali_img;
		if (cali_para->use_ext_cali_img) {
			ret = set_ext_image_data(&pano_private->in.imgYuv,
				&cali_para->cali_img);
		}
		break;
	}
	case omx_index_pano_init_para: {
		pano_init_para_t *init_para = (pano_init_para_t *)paramData;
		oscl_param_check((init_para != NULL), OMX_ErrorBadParameter, NULL);
		set_pano_init_para(&pano_private->ini_set_pano, init_para);
		memset(&pano_private->ini_set_pano.carBodyImg, 0, sizeof(IMGYUVC));
		if (strlen((char *)init_para->carb_img.path) > 0) {
			ret = set_ext_image_data(&pano_private->ini_set_pano.carBodyImg,
							&init_para->carb_img);
		}
		pano_private->ext_cutline = init_para->use_ext_cutline;
		if (init_para->use_ext_cutline)
			pano_private->cutline = init_para->culine;
		break;
	}
	case omx_index_pano_cali_out_data: {
		pano_cali_out_data_t *cali_data = (pano_cali_out_data_t *)paramData;

		oscl_param_check((cali_data != NULL), OMX_ErrorBadParameter, NULL);
		oscl_param_check((cali_data->data != NULL), OMX_ErrorBadParameter, NULL);
		oscl_param_check((cali_data->data_size != 0),
					OMX_ErrorBadParameter, NULL);
		OSCL_LOGI("Set cali out data>> ");
		OSCL_LOGI("\t data:%p %d", cali_data->data, cali_data->data_size);
		if (cali_data->data_size != sizeof(HDCalibParaPano)) {
			OSCL_LOGE("Set cali data error. %d", cali_data->data_size);
			return OMX_ErrorInsufficientResources;
		}
		memcpy(&pano_private->out.calibPara, cali_data->data,
				sizeof(HDCalibParaPano));
		pano_private->pano_start = 1;
		break;
	}
	case OMX_IndexParamPortDefinition: {
		port_def = (OMX_PARAM_PORTDEFINITIONTYPE *)paramData;

		ret = base_set_parameter(hComp, paramIndex, paramData);
		oscl_param_check((OMX_ErrorNone == ret), ret, NULL);

		inport = &component->port[PANO_INPUT_PORT_INDEX];
		outport = &component->port[PANO_OUTPUT_PORT_INDEX];
		if (PANO_INPUT_PORT_INDEX == port_def->nPortIndex) {
			video = &inport->port_param.format.video;
			ret = calc_frame_size(video->eColorFormat,
				video->nStride,
				video->nSliceHeight,
				pano_private->inbuf_offset,
				inport->port_param.nBufferAlignment);
			oscl_param_check((OMX_ErrorNone == ret), ret, NULL);
			inport->port_param.nBufferSize = pano_private->inbuf_offset[0] +
						pano_private->inbuf_offset[1] +
						pano_private->inbuf_offset[2];
			oscl_param_check((inport->port_param.nBufferSize > 0),
					OMX_ErrorBadParameter, NULL);
			oscl_param_check((video->xFramerate > 0),
					OMX_ErrorBadParameter, NULL);
			OSCL_LOGD("input:%d %d %d %d, input format:%x, fps:%d",
				video->nFrameWidth, video->nFrameHeight,
				video->nStride, video->nSliceHeight,
				video->eColorFormat, video->xFramerate);
			pano_set_inis_camp_w(&pano_private->ini_set_pano, video->nStride);
			pano_set_inis_camp_h(&pano_private->ini_set_pano,
					video->nSliceHeight);
			pano_set_inis_camp_fps(&pano_private->ini_set_pano,
					video->xFramerate / 1000);

			stop_inbuf = pano_private->stop_buf.inbuf;
			if (stop_inbuf) {
				if (stop_inbuf->pBuffer) {
					oscl_free(stop_inbuf->pBuffer);
					stop_inbuf->pBuffer = NULL;
				}
				oscl_free(stop_inbuf);
				pano_private->stop_buf.inbuf = NULL;
			}
			stop_inbuf = oscl_zalloc(sizeof(OMX_BUFFERHEADERTYPE));
			oscl_param_check((NULL != stop_inbuf),
				OMX_ErrorInsufficientResources, NULL);
			stop_inbuf->pBuffer = oscl_zalloc(inport->port_param.nBufferSize);
			if (!stop_inbuf->pBuffer) {
				oscl_free(stop_inbuf);
				return OMX_ErrorInsufficientResources;
			}
			pano_private->stop_buf.inbuf = stop_inbuf;
		} else if (PANO_OUTPUT_PORT_INDEX == port_def->nPortIndex) {
			video = &outport->port_param.format.video;
			ret = calc_frame_size(video->eColorFormat,
				video->nStride,
				video->nSliceHeight,
				pano_private->outbuf_offset,
				outport->port_param.nBufferAlignment);
			oscl_param_check((OMX_ErrorNone == ret), ret, NULL);
			outport->port_param.nBufferSize = pano_private->outbuf_offset[0] +
						pano_private->outbuf_offset[1] +
						pano_private->outbuf_offset[2];
			oscl_param_check((outport->port_param.nBufferSize > 0),
					OMX_ErrorBadParameter, NULL);
			pano_set_cb_previeww(&pano_private->in.checkerBoard,
						video->nStride);
			pano_set_cb_previewh(&pano_private->in.checkerBoard,
						video->nSliceHeight);

			stop_outbuf = pano_private->stop_buf.outbuf;
			if (stop_outbuf) {
				if (stop_outbuf->pBuffer) {
					oscl_free(stop_outbuf->pBuffer);
					stop_outbuf->pBuffer = NULL;
				}
				oscl_free(stop_outbuf);
				pano_private->stop_buf.outbuf = NULL;
			}
			stop_outbuf = oscl_zalloc(sizeof(OMX_BUFFERHEADERTYPE));
			oscl_param_check((NULL != stop_outbuf),
				OMX_ErrorInsufficientResources, NULL);
			stop_outbuf->pBuffer = oscl_zalloc
				(outport->port_param.nBufferSize);
			if (!stop_outbuf->pBuffer) {
				oscl_free(stop_outbuf);
				return OMX_ErrorInsufficientResources;
			}
			pano_private->stop_buf.outbuf = stop_outbuf;
			OSCL_LOGD("output:%d %d %d %d, output format:%x",
				video->nFrameWidth, video->nFrameHeight,
				video->nStride, video->nSliceHeight,
				video->eColorFormat);
		}
		break;
	}
	default:
		ret = base_set_parameter(hComp, paramIndex, paramData);
		break;
	}

	return ret;
}

OMX_ERRORTYPE pano_get_parameter(OMX_IN OMX_HANDLETYPE hComp,
	OMX_IN OMX_INDEXTYPE paramIndex,
	OMX_IN OMX_PTR paramData)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	lb_omx_component_t *component = NULL;
	pano_private_t *pano_private = NULL;

	component = get_lb_component(hComp);
	oscl_param_check((component != NULL), OMX_ErrorBadParameter, NULL);
	pano_private = component->component_private;

	switch (paramIndex) {
	case omx_index_pano_cali_out_data: {
		pano_cali_out_data_t *cali_data = (pano_cali_out_data_t *)paramData;

		oscl_param_check((cali_data != NULL), OMX_ErrorBadParameter, NULL);
		cali_data->data_size = sizeof(HDCalibParaPano);
		cali_data->data = oscl_zalloc(cali_data->data_size);
		oscl_param_check((cali_data->data != NULL), OMX_ErrorBadParameter, NULL);
		memcpy(cali_data->data, &pano_private->out.calibPara,
					cali_data->data_size);
		OSCL_LOGI("Get cali out data>> ");
		OSCL_LOGI("\t data:%p %d", cali_data->data, cali_data->data_size);
		break;
	}
	default:
		ret = base_get_parameter(hComp, paramIndex, paramData);
		break;
	}

	return ret;
}

OMX_ERRORTYPE pano_set_state(OMX_HANDLETYPE hComp,
	OMX_U32 dest_state)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	lb_omx_component_t *component = NULL;
	pano_private_t *pano_private = NULL;
	OMX_STATETYPE pre_state;
	OMX_BUFFERHEADERTYPE *stop_inbuf = NULL;
	OMX_BUFFERHEADERTYPE *stop_outbuf = NULL;

	OSCL_TRACE(" %p, %x\n", hComp, dest_state);
	oscl_param_check(hComp != NULL, OMX_ErrorBadParameter, NULL);
	component = get_lb_component(hComp);
	pano_private = component->component_private;

	if (dest_state == OMX_StateExecuting && component->state == OMX_StateIdle) {
		pano_private->enter_proc_sem = (sem_t *)oscl_zalloc(sizeof(sem_t));
		oscl_param_check(pano_private->enter_proc_sem != NULL,
				OMX_ErrorInsufficientResources, NULL);
		sem_init(pano_private->enter_proc_sem, 0, 0);
		pano_private->stop_pano = 0;
		if (pano_private->pano_start)
			pan_creat_ex(pano_private);
	}
	pre_state = component->state;
	if (dest_state == OMX_StateIdle && component->state == OMX_StateExecuting) {
		pano_private->stop_pano = 1;
		if (pano_private->pano_creat_flag) {
			if (pano_private->enter_proc_sem)
				sem_post(pano_private->enter_proc_sem);
			pano_delete();
			pano_private->pano_creat_flag = 0;
		}
	}
	ret = base_component_set_state(hComp, dest_state);
	if (dest_state == OMX_StateIdle && pre_state == OMX_StateExecuting) {
		if (pano_private->ini_set_pano.carBodyImg.yP) {
			oscl_free_align(pano_private->ini_set_pano.carBodyImg.yP);
			pano_private->ini_set_pano.carBodyImg.yP = NULL;
		}
		if (pano_private->ini_set_pano.carBodyImg.uvP) {
			oscl_free_align(pano_private->ini_set_pano.carBodyImg.uvP);
			pano_private->ini_set_pano.carBodyImg.uvP = NULL;
		}
		if (pano_private->in.imgYuv.yP) {
			oscl_free_align(pano_private->in.imgYuv.yP);
			pano_private->in.imgYuv.yP = NULL;
		}
		if (pano_private->in.imgYuv.uvP) {
			oscl_free_align(pano_private->in.imgYuv.uvP);
			pano_private->in.imgYuv.uvP = NULL;
		}
		if (pano_private->enter_proc_sem) {
			sem_destroy(pano_private->enter_proc_sem);
			oscl_free(pano_private->enter_proc_sem);
			pano_private->enter_proc_sem = NULL;
		}
		stop_inbuf = pano_private->stop_buf.inbuf;
		if (stop_inbuf) {
			if (stop_inbuf->pBuffer) {
				oscl_free(stop_inbuf->pBuffer);
				stop_inbuf->pBuffer = NULL;
			}
			oscl_free(stop_inbuf);
			pano_private->stop_buf.inbuf = NULL;
		}
		stop_outbuf = pano_private->stop_buf.outbuf;
		if (stop_outbuf) {
			if (stop_outbuf->pBuffer) {
				oscl_free(stop_outbuf->pBuffer);
				stop_outbuf->pBuffer = NULL;
			}
			oscl_free(stop_outbuf);
			pano_private->stop_buf.outbuf = NULL;
		}
		pano_private->cali_complete = 0;
		pano_private->en_cali = 0;
	}
	return ret;
}

void pano_buf_handle(OMX_HANDLETYPE hComp,
	OMX_BUFFERHEADERTYPE *inbuf, OMX_BUFFERHEADERTYPE *outbuf)
{
	lb_omx_component_t *component = NULL;
	pano_private_t *pano_private = NULL;

	component = get_lb_component(hComp);
	pano_private = component->component_private;

	inbuf->nFilledLen = 0;
	outbuf->nFilledLen = 0;
	outbuf->nTimeStamp = inbuf->nTimeStamp;
	if (pano_private->en_cali) {
		pano_private->en_cali = 0;
		if (pano_private->pano_creat_flag) {
			pano_delete();
			pano_private->pano_creat_flag = 0;
			pano_private->pano_start = 0;
		}
	}
	pano_buf_add(pano_private, inbuf, outbuf);
}

void *pano_component_buffer_manager(void *param)
{
	lb_omx_component_t *component;
	base_port_t *inport;
	base_port_t *outport;
	OMX_BUFFERHEADERTYPE *outbuffer = NULL;
	OMX_BUFFERHEADERTYPE *inbuffer = NULL;
	OMX_BUFFERHEADERTYPE *tmp_outbuf = NULL;
	OMX_BUFFERHEADERTYPE *tmp_inbuf = NULL;
	OMX_ERRORTYPE ret;
	OMX_HANDLETYPE cmp_hdl = param;
	pano_private_t *pano_private = NULL;
	OMX_S32 num = 0;

	OSCL_TRACE("pano_component_buffer_manager %p\n", param);
	oscl_param_check((param != NULL), NULL, NULL);
	component = get_lb_component(cmp_hdl);
	inport = &component->port[PANO_INPUT_PORT_INDEX];
	outport = &component->port[PANO_OUTPUT_PORT_INDEX];
	component->dbg_flag = set_debug_state(component->dbg_flag,
			DEBUG_BUF_MGNT_SHT, DEBUG_THREAD_START);
	pano_private = component->component_private;

	/* checks if the component is in a state able to receive buffers */
	while (component->state == OMX_StateIdle
		|| component->state == OMX_StateExecuting
		|| component->state == OMX_StatePause) {
		/*Wait till the ports are being flushed*/
		component->dbg_wdog = 0;
		pthread_mutex_lock(&component->flush_mutex);

		while (inport->is_flushed || outport->is_flushed) {
			pthread_mutex_unlock(&component->flush_mutex);
			OSCL_LOGI("component:%x,flush buffers:%d %d\n",
				component, inport->is_flushed, outport->is_flushed);
			OSCL_LOGI("inport:%d, buffer:%p\n",
				inport->port_param.nPortIndex, inbuffer);
			OSCL_LOGI("outport:%d, buffer:%p\n",
				outport->port_param.nPortIndex, outbuffer);
			do {
				if (outbuffer && outport->is_flushed) {
					outport->recive_buffer(outport, outbuffer);
					outbuffer = NULL;
				}
				if (inbuffer && inport->is_flushed) {
					ret = inport->return_buffer(inport, inbuffer);
					if (OMX_ErrorNone != ret)
						OSCL_LOGE("component (%s) state error.",
							get_port_name(inport));
					inbuffer = NULL;
				}
				ret = pano_buf_pop(pano_private, &inbuffer, &outbuffer);
				if (ret != OMX_ErrorNone)
					break;
			} while (inbuffer || outbuffer);
			sem_post(component->mgmt_flush_sem);
			OSCL_LOGE("==%s %d\n", __FILE__, __LINE__);
			if (oscl_sem_timedwait_ms(component->flush_sem, 15000))
				OSCL_LOGE("wait sem timeout!");
			pthread_mutex_lock(&component->flush_mutex);
		}
		pthread_mutex_unlock(&component->flush_mutex);
		if (component->state != OMX_StateExecuting) {
			OSCL_LOGD("==========%d iqueue:%d oqueue:%d\n",
				component->buf_mgnt_sem->sem->value,
				oscl_queue_get_num(&inport->buffer_queue),
				oscl_queue_get_num(&outport->buffer_queue));
			OSCL_LOGD("component->buf_mgnt_sem");
			sem_wait(component->buf_mgnt_sem);
			continue;
		}
		inbuffer = NULL;
		outbuffer = NULL;
		while (1) {
			if (inport->is_flushed || outport->is_flushed)
				break;
			if (inbuffer == NULL)
				inbuffer = oscl_queue_dequeue(&inport->buffer_queue);
			num = oscl_queue_get_num(&pano_private->pano_buf_queue);
			if (inbuffer && num >= 1) {
				ret = pano_buf_pop(pano_private, &tmp_inbuf, &tmp_outbuf);
				if (ret == OMX_ErrorNone) {
					pano_buf_add(pano_private,
						inbuffer, tmp_outbuf);
					inport->return_buffer(inport, tmp_inbuf);
					inbuffer = NULL;
				}
			}
			if (outbuffer == NULL)
				outbuffer = oscl_queue_dequeue(&outport->buffer_queue);
			if (outbuffer == NULL || inbuffer == NULL) {
				OSCL_LOGD("waiting buffer:%x %x, sem value:%d\n",
					inbuffer, outbuffer,
					component->buf_mgnt_sem->sem->value);
				oscl_sem_timedwait_ms(component->buf_mgnt_sem, 50);
				continue;
			} else
				break;
		}
		if (inport->is_flushed || outport->is_flushed)
			continue;
		if (component->mark.hMarkTargetComponent) {
			outbuffer->hMarkTargetComponent
				= component->mark.hMarkTargetComponent;
			outbuffer->pMarkData
				= component->mark.pMarkData;
			component->mark.hMarkTargetComponent = NULL;
			component->mark.pMarkData = NULL;
		}
		if (component->state == OMX_StateExecuting) {
			if (component->buf_handle && inbuffer->nFilledLen > 0)
				((component->buf_handle))(component,
						inbuffer, outbuffer);
			else
				inbuffer->nFilledLen = 0;
		}
	}
	OSCL_LOGW("exit from buffer_manager:%s\n", rt_thread_self()->name);
	OSCL_TRACE(" %p\n", param);
	component->dbg_flag = set_debug_state(component->dbg_flag,
			DEBUG_BUF_MGNT_SHT, DEBUG_THREAD_EXIT);
	return NULL;
}

OMX_ERRORTYPE pano_component_deinit(OMX_IN OMX_HANDLETYPE hComp)
{
	lb_omx_component_t *component;
	pano_private_t *pano_private = NULL;

	oscl_param_check(hComp != NULL, OMX_ErrorBadParameter, NULL);

	OSCL_TRACE("base_cmp_handle:%p\n", hComp);
	oscl_param_check(hComp != NULL, OMX_ErrorBadParameter, NULL);
	component = get_lb_component(hComp);
	pano_private = component->component_private;

	base_port_deinit(&component->port[PANO_INPUT_PORT_INDEX]);
	base_port_deinit(&component->port[PANO_OUTPUT_PORT_INDEX]);
	oscl_queue_deinit(&pano_private->pano_buf_queue);
	oscl_free(pano_private);
	pano_component = NULL;
	base_component_deinit(hComp);

	return OMX_ErrorNone;
}

OMX_ERRORTYPE pano_component_init(lb_omx_component_t *cmp_handle)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	OMX_COMPONENTTYPE *base_cmp = &(cmp_handle->base_comp);
	pano_private_t *pano_private = NULL;
	OMX_PARAM_PORTDEFINITIONTYPE *port_param = NULL;
	OMX_U32 plane_size[3] = {0};

	pano_private = oscl_zalloc(sizeof(pano_private_t));
	oscl_param_check_exit((pano_private != NULL),
		OMX_ErrorInsufficientResources, NULL);
	oscl_queue_init(&pano_private->pano_buf_queue);
	ret = base_component_init(cmp_handle);
	if (ret != OMX_ErrorNone) {
		oscl_free(pano_private);
		return ret;
	}
	cmp_handle->name = "OMX.LB.VIDEO.PANO";
	cmp_handle->component_private = pano_private;
	cmp_handle->base_comp.ComponentDeInit = pano_component_deinit;
	cmp_handle->buf_handle = pano_buf_handle;
	cmp_handle->buf_manager = pano_component_buffer_manager;
	cmp_handle->do_state_set = pano_set_state;
	cmp_handle->num_ports = 2;
	pthread_attr_setstacksize(&cmp_handle->buf_thread_attr, 0x2000);
	/* input port */
	ret = base_port_init(cmp_handle, &cmp_handle->port[PANO_INPUT_PORT_INDEX],
			PANO_INPUT_PORT_INDEX,
			OMX_DirInput);
	if (OMX_ErrorNone != ret)
		goto EXIT;
	port_param = &cmp_handle->port[PANO_INPUT_PORT_INDEX].port_param;
	port_param->eDomain = OMX_PortDomainVideo;
	port_param->nBufferAlignment = DEFAULT_BUFFER_ALIGN_SIZE;
	port_param->nBufferCountActual = DEFAULT_BUFFER_NUM;
	port_param->bBuffersContiguous = 1;

	port_param->format.video.eColorFormat = OMX_COLOR_FormatYUV420SemiPlanar;
	port_param->format.video.nFrameWidth = DEFAULT_VIDEO_WIDTH;
	port_param->format.video.nFrameHeight = DEFAULT_VIDEO_HEIGHT;
	ret = calc_frame_size(port_param->format.video.eColorFormat,
			port_param->format.video.nFrameWidth,
			port_param->format.video.nFrameHeight,
			plane_size, port_param->nBufferAlignment);
	port_param->nBufferSize = plane_size[0] + plane_size[1] + plane_size[2];

	/* output port */
	ret = base_port_init(cmp_handle, &cmp_handle->port[PANO_OUTPUT_PORT_INDEX],
			PANO_OUTPUT_PORT_INDEX,
			OMX_DirOutput);
	if (ret != OMX_ErrorNone)
		goto EXIT1;
	port_param = &cmp_handle->port[PANO_OUTPUT_PORT_INDEX].port_param;
	port_param->eDomain = OMX_PortDomainVideo;
	port_param->nBufferAlignment = DEFAULT_BUFFER_ALIGN_SIZE;
	port_param->nBufferCountActual = DEFAULT_BUFFER_NUM;
	port_param->bBuffersContiguous = 1;

	port_param->format.video.eColorFormat = OMX_COLOR_FormatYUV420SemiPlanar;
	port_param->format.video.nFrameWidth = DEFAULT_VIDEO_HEIGHT;
	port_param->format.video.nFrameHeight = DEFAULT_VIDEO_WIDTH;
	ret = calc_frame_size(port_param->format.video.eColorFormat,
			port_param->format.video.nFrameWidth,
			port_param->format.video.nFrameHeight,
			plane_size, port_param->nBufferAlignment);
	port_param->nBufferSize = plane_size[0] + plane_size[1] + plane_size[2];

	base_cmp->SetParameter = pano_set_parameter;
	base_cmp->GetParameter = pano_get_parameter;

	pano_component = cmp_handle;
	OSCL_TRACE("pano_component_init:%d\n", ret);

	return ret;
EXIT1:
	base_port_deinit(&cmp_handle->port[PANO_INPUT_PORT_INDEX]);
EXIT:
	base_component_deinit(cmp_handle);
	oscl_free(pano_private);

	return OMX_ErrorInsufficientResources;
}

