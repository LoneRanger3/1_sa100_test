#define DBG_LEVEL		DBG_WARNING
#define LOG_TAG			"venc_component"

#include "oscl.h"
#include "venc_component.h"
#include "lombo_al_lib.h"
#include "lombo_enc_common.h"
#include "lombo_encplugin.h"

#ifdef __linux__
	#define DYNAMIC_LOADING
#endif

#define BLENDING_BUF_ALIGN 4096

#define VENCODER_BUFFER_SIZE (2*1024+384)
#define MJPEG_BUFFER_SIZE (5*1024)

/* #define SAVE_RAW_STREAM */
#ifdef SAVE_RAW_STREAM
FILE *test_flip;
static int file_num;
static int raw_cnt_num;
#endif

/* #define SAVE_YUV_PICTURE */
#ifdef SAVE_YUV_PICTURE
static int yuv_frame_cnt;
static int yuv_file_cnt;
FILE *yuv_flip;
#endif


#ifdef DYNAMIC_LOADING
static int __load_symb(venc_component_private_t *vec_private)
{
	int ret = 0;
	venc_plugin_t *(*func_get_plugin_info)(void);
	char *shared_lib;

	if (vec_private->lib_handle != NULL) {
		OSCL_LOGE("symble not unload while load symbol");
		unload_library(vec_private->lib_handle);
		vec_private->lib_handle = NULL;
	}
	if (vec_private->role_type == H264_ROLE_TYPE)
		shared_lib = "vce_h264.so";
	else if (vec_private->role_type == JPEG_ROLE_TYPE)
		shared_lib = "vce_mjpeg.so";
	else {
		OSCL_LOGE("unsupport role, set to default(h264)");
		vec_private->role_type = H264_ROLE_TYPE;
		shared_lib = "vce_h264.so";
	}

	if (vec_private->lib_handle == NULL) {
		vec_private->lib_handle = load_library(shared_lib);
		oscl_param_check_exit(vec_private->lib_handle != NULL, -1, NULL);

		func_get_plugin_info = get_library_entry(vec_private->lib_handle,
			"get_plugin_info");
		oscl_param_check_exit(func_get_plugin_info != NULL, -1, NULL);

		vec_private->video_encode_info =
				(venc_plugin_t *)func_get_plugin_info();
		oscl_param_check_exit(vec_private->video_encode_info != NULL, -1, NULL);
	}
EXIT:
	return ret;
}

static void __unload_symb(venc_component_private_t *vec_private)
{
	if (vec_private->lib_handle) {
		unload_library(vec_private->lib_handle);
		vec_private->lib_handle = NULL;
	}
	vec_private->video_encode_info = NULL;
}

#else
venc_plugin_t venc_h264_plugin = {
	.open = h264_enc_open,
	.queue_buf = h264_queue_buf,
	.encode_frame = h264_enc_frame,
	.dequeue_buf = h264_dequeue_buf,
	.close = h264_enc_close,
	.ex_ops = h264_enc_ops,
};
venc_plugin_t venc_mjpeg_plugin = {
	.open = mjpeg_enc_open,
	.queue_buf = mjpeg_queue_buf,
	.encode_frame = mjpeg_enc_frame,
	.dequeue_buf = mjpeg_dequeue_buf,
	.close = mjpeg_enc_close,
	.ex_ops = mjpeg_enc_ops,
};
static int __load_symb(venc_component_private_t *vec_private)
{
	vec_private->video_encode_info = &venc_h264_plugin;
	if (vec_private->role_type == JPEG_ROLE_TYPE)
		vec_private->video_encode_info = &venc_mjpeg_plugin;
	return 0;
}

static void __unload_symb(venc_component_private_t *vec_private)
{
	vec_private->video_encode_info = NULL;
}
#endif

static int OmxFormat_to_EncodeFormat(int OmxFormat)
{
	int EncodeFormat = -1;

	if (OmxFormat == OMX_COLOR_FormatYUV420Planar)
		EncodeFormat = ENC_YUV420P;
	else if (OmxFormat == OMX_COLOR_FormatYUV420SemiPlanar)
		EncodeFormat = ENC_YUV420SP;

	if (EncodeFormat == -1)
		OSCL_LOGE("not support the OmxFormat\n");

	return EncodeFormat;
}

static int OmxFormat_to_BlendingFormat(int OmxFormat)
{
	int EncodeFormat = -1;

	if (OmxFormat == OMX_COLOR_Format32bitARGB8888)
		EncodeFormat = ENC_ARGB8888;
	else if (OmxFormat == OMX_COLOR_Format32bitBGRA8888)
		EncodeFormat = ENC_BGRA8888;
	else if (OmxFormat == OMX_COLOR_Format32BitRGBA8888)
		EncodeFormat = ENC_RGBA8888;

	if (EncodeFormat == -1)
		OSCL_LOGE("not support the OmxFormat blending\n");
	return EncodeFormat;
}

static int write_watermark(venc_component_private_t *vec_private,
					wm_data_t *wm_data)
{
	watermark_picture_t *wm_pic;
	int index, align_width, align_height;
	int pic_size = 0, bpp = 0;
	int ret = 0;
	int new_wm = 0;
	unsigned char *blending_buf = NULL;

	oscl_param_check_exit(vec_private != NULL, OMX_ErrorBadParameter, NULL);
	oscl_param_check_exit(vec_private->handle != NULL, OMX_ErrorBadParameter, NULL);
	oscl_param_check_exit(wm_data != NULL, OMX_ErrorBadParameter, NULL);
	index = wm_data->index;
	oscl_param_check_exit((index >= 0), OMX_ErrorBadParameter, NULL);
	oscl_param_check_exit((index < MAX_ENC_BLENDING_NUM),
				OMX_ErrorBadParameter, NULL);

	wm_pic = &vec_private->wartermark.watermark_pictures[index];
	new_wm = vec_private->wartermark.watermark_picture_num;
	blending_buf = wm_pic->vir_addr;

	if ((wm_data->wm_pic.colorspace == OMX_COLOR_Format32bitARGB8888) ||
		(wm_data->wm_pic.colorspace == OMX_COLOR_Format32bitBGRA8888) ||
		(wm_data->wm_pic.colorspace == OMX_COLOR_Format32BitRGBA8888))
		bpp = 4;
	else {
		OSCL_LOGE("Not support watermark pic format.");
		return OMX_ErrorBadParameter;
	}
	align_width = (wm_data->wm_pic.blending_width + 15) & 0xFFFFFFF0;
	align_height = (wm_data->wm_pic.blending_height + 15) & 0xFFFFFFF0;
	pic_size = align_width * align_height * bpp;
	if (!blending_buf)
		new_wm++;
	if (blending_buf &&
		((wm_pic->blending_width != wm_data->wm_pic.blending_width) ||
		(wm_pic->blending_height != wm_data->wm_pic.blending_height))) {
			oscl_free_align(blending_buf);
			wm_pic->vir_addr = NULL;
			blending_buf = NULL;
	}
	if (!blending_buf) {
		if (pic_size)
			blending_buf = oscl_malloc_align(pic_size,
							BLENDING_BUF_ALIGN);
		if (!blending_buf) {
			OSCL_LOGE("Malloc fail. %d", pic_size);
			return OMX_ErrorBadParameter;
		}
	}

	memcpy(blending_buf, wm_data->wm_pic.vir_addr, pic_size);
	memcpy(wm_pic, &wm_data->wm_pic, sizeof(watermark_picture_t));
	wm_pic->blending_enable = 1;
	wm_pic->colorspace = OmxFormat_to_BlendingFormat(wm_data->wm_pic.colorspace);
	wm_pic->vir_addr = blending_buf;
	wm_pic->phy_addr = oscl_virt_to_phys(blending_buf);
	vec_private->wartermark.watermark_picture_num = new_wm;

	OSCL_LOGD("watermark_picture_num:%d, index:%d, pic_size:%d color:%d(w:%d h:%d)\n",
		new_wm, index, pic_size, wm_pic->colorspace,
		wm_data->wm_pic.blending_width, wm_data->wm_pic.blending_height);
	oscl_cache_flush_vir(wm_pic->vir_addr, pic_size);
	ret = vec_private->video_encode_info->ex_ops(vec_private->handle,
		VENC_SET_BLENDING_PICTURE, &vec_private->wartermark);
EXIT:
	if (ret != 0)
		OSCL_LOGE("set blending picture error\n");
	return ret;

}

static int disable_watermark(venc_component_private_t *vec_private, int index)
{
	int ret = 0;
	watermark_picture_t *wm_pic = NULL;

	oscl_param_check_exit(vec_private != NULL, OMX_ErrorBadParameter, NULL);
	oscl_param_check_exit(vec_private->handle != NULL, OMX_ErrorBadParameter, NULL);
	oscl_param_check_exit((index >= 0), OMX_ErrorBadParameter, NULL);
	oscl_param_check_exit((index < MAX_ENC_BLENDING_NUM),
					OMX_ErrorBadParameter, NULL);

	wm_pic = &vec_private->wartermark.watermark_pictures[index];
	wm_pic->blending_enable = 0;
	ret = vec_private->video_encode_info->ex_ops(vec_private->handle,
		VENC_SET_BLENDING_PICTURE, &vec_private->wartermark);

EXIT:
	if (ret != 0)
		OSCL_LOGE("set blending picture error\n");
	return ret;

}

static int encodec_init(OMX_IN OMX_HANDLETYPE cmp_handle)
{
	lb_omx_component_t *component = NULL;
	venc_component_private_t *vec_private = NULL;
	component = (lb_omx_component_t *)cmp_handle;
	int ret = 0;
	int src_height;
	int src_width;
	int offset[3];
	int buffer_size = VENCODER_BUFFER_SIZE;

	OSCL_LOGI("venc init start..");
	al_vc_core_init();
	OSCL_LOGI("venc init end..");
	component = (lb_omx_component_t *)cmp_handle;
	vec_private = (venc_component_private_t *)(component->component_private);

	src_height = vec_private->encode_parm->in_height;
	src_width = vec_private->encode_parm->in_width;
	if (vec_private->enc_rect.width > src_width)
		vec_private->enc_rect.width = src_width;
	if (vec_private->enc_rect.height > src_height)
		vec_private->enc_rect.height = src_height;

	offset[0] = 0;
	offset[1] = 0;
	offset[2] = 0;
	if (vec_private->enc_rect.width != 0 && vec_private->enc_rect.height != 0) {
		offset[0] = src_width * vec_private->enc_rect.y;
		offset[0] += vec_private->enc_rect.x;
		if (vec_private->encode_parm->input_mode == ENC_YUV420SP) {
			offset[1] = src_width * vec_private->enc_rect.y / 2;
			offset[1] += vec_private->enc_rect.x;
		}
		if (vec_private->encode_parm->input_mode == ENC_YUV420P) {
			offset[1] = src_width * vec_private->enc_rect.y / 4;
			offset[1] += vec_private->enc_rect.x / 2;
			offset[2] = offset[1];
		}
		vec_private->encode_parm->in_width = vec_private->enc_rect.width;
		vec_private->encode_parm->in_height = vec_private->enc_rect.height;
		vec_private->cap_offsets[0] += offset[0];
		vec_private->cap_offsets[1] += offset[1];
		vec_private->cap_offsets[2] += offset[2];
	}

	OSCL_LOGI("enc_parm: rect[%d,%d,%d,%d], offset:%d-%d-%d",
		vec_private->enc_rect.x,
		vec_private->enc_rect.y,
		vec_private->enc_rect.width,
		vec_private->enc_rect.height,
		offset[0], offset[1], offset[2]);
	OSCL_LOGI("enc_parm: insize[%dx%d], outsize[%dx%d], bitrate[%d], framerate[%d]",
		vec_private->encode_parm->in_width,
		vec_private->encode_parm->in_height,
		vec_private->encode_parm->out_width,
		vec_private->encode_parm->out_height,
		vec_private->encode_parm->bitrate,
		vec_private->encode_parm->frame_rate);
	OSCL_LOGI("plan_num %d, offsets : %d - %d - %d\n",
		vec_private->cap_plan_num,
		vec_private->cap_offsets[0],
		vec_private->cap_offsets[1],
		vec_private->cap_offsets[2]);

	ret = __load_symb(vec_private);
	oscl_param_check_exit(ret == 0, -1, NULL);

	OSCL_LOGI("venc open start..");
	if (vec_private->role_type == JPEG_ROLE_TYPE)
		buffer_size = MJPEG_BUFFER_SIZE;
	if (vec_private->handle == NULL)
		vec_private->handle = vec_private->video_encode_info->open
			(vec_private->encode_parm, buffer_size);
	oscl_param_check_exit(vec_private->handle != NULL, -1, NULL);
	OSCL_LOGI("venc open end..");
EXIT:
	if (ret != 0)
		__unload_symb(vec_private);
	return ret;
}

static int encodec_deinit(OMX_IN OMX_HANDLETYPE cmp_handle)
{
	lb_omx_component_t *component = NULL;
	venc_component_private_t *vec_private = NULL;

	component = (lb_omx_component_t *)cmp_handle;
	vec_private = (venc_component_private_t *)(component->component_private);
	if (vec_private->handle != NULL) {
		OSCL_LOGI("venc close start..");
		vec_private->video_encode_info->close(vec_private->handle);
		vec_private->handle = NULL;
		OSCL_LOGI("venc close end..");
	}
	__unload_symb(vec_private);

	return 0;
}

static int check_encode_parm(venc_parm_t *parm)
{
	if ((parm->in_width <= 0) || (parm->in_height <= 0) || (parm->l_stride <= 0)) {
		OSCL_LOGE("no input width/height\n");
		return -1;
	}

	if ((parm->out_width <= 0) || (parm->out_height <= 0)) {
		OSCL_LOGE("no ouput width/height\n");
		return -1;
	}

	if (parm->bitrate <= 0) {
		OSCL_LOGW("no bitrate, use defalut 2M bitrate\n");
		parm->bitrate = 2*1024*1024;
	}

	parm->slice_mode = 0;

	if ((parm->input_mode != 0) && (parm->input_mode != 2))
		OSCL_LOGW("other input mode, need attention\n");

/*
	if (parm->idr_period == 0)
		parm->idr_period = 30;
	else {
		if (parm->idr_period <= 10)
			parm->idr_period = 10;
		if (parm->idr_period >= 30)
			parm->idr_period = 30;
	}
*/
	if (parm->frame_rate == 0)
		parm->frame_rate = 30;
	parm->idr_period = parm->frame_rate;
/*
	if (parm->frame_rate == 0)
		parm->frame_rate = 30;
	else {
		if (parm->frame_rate <= 5)
			parm->frame_rate = 5;
		if (parm->frame_rate >= 60)
			parm->frame_rate = 60;
	}
*/
	return 0;
}


static int OmxFormat_to_plan_num(int OmxFormat)
{
	int plan_num = -1;

	if (OmxFormat == OMX_COLOR_FormatYUV420Planar)
		plan_num = 3;
	else if (OmxFormat == OMX_COLOR_FormatYUV420SemiPlanar)
		plan_num = 2;
	else
		plan_num = -1;

	if (plan_num == -1)
		OSCL_LOGE("not support the OmxFormat\n");

	return plan_num;
}

OMX_ERRORTYPE video_encoder_empty_this_buffer(OMX_IN OMX_HANDLETYPE hComp,
	OMX_IN OMX_BUFFERHEADERTYPE *header)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
#ifdef SAVE_YUV_PICTURE
	lb_omx_component_t *component;
	venc_component_private_t *vec_pri;
	unsigned char *vir_addr[3] = {0, 0, 0};
	component = get_lb_component(hComp);
	vec_pri = (venc_component_private_t *)(component->component_private);
	int offset0 = vec_pri->cap_offsets[0];
	int offset1 = vec_pri->cap_offsets[1];
	unsigned char *omx_vir_addr = header->pBuffer;
	vir_addr[0] = omx_vir_addr + offset0;
	vir_addr[1] = omx_vir_addr + offset1;
	yuv_frame_cnt++;
	if (yuv_frame_cnt % 500 == 0) {
		char file_name[100] = "\0";
		int w = component->port[0].port_param.format.video.nFrameWidth;
		int h = component->port[0].port_param.format.video.nFrameHeight;
		sprintf(file_name, "/mnt/sdcard/h264_%d.yuv", yuv_file_cnt%100);
		yuv_file_cnt++;
		yuv_flip = fopen(file_name, "wb+");
		fwrite(vir_addr[0], 1, w * h, yuv_flip);
		fwrite(vir_addr[1], 1, w * h / 2, yuv_flip);
		OSCL_LOGE("omx_vir_addr : %p -- %d -- %d -- %d\n", omx_vir_addr,
			vec_pri->cap_offsets[0],
			vec_pri->cap_offsets[1],
			vec_pri->cap_offsets[2]);
		fclose(yuv_flip);
	}
#endif
	ret = base_empty_this_buffer(hComp, header);
	return ret;
}

OMX_ERRORTYPE video_encoder_set_config(OMX_IN OMX_HANDLETYPE hcomp,
		OMX_IN OMX_INDEXTYPE cfg_index,
		OMX_IN OMX_PTR cfg_data)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	lb_omx_component_t *component;
	venc_component_private_t *vec_private;
	OMX_TIME_CONFIG_TIMESTAMPTYPE *time;

	oscl_param_check(hcomp != NULL, -1, NULL);
	oscl_param_check(cfg_data != NULL, -1, NULL);

	component = get_lb_component(hcomp);
	oscl_param_check(component != NULL, -1, NULL);
	vec_private = (venc_component_private_t *)(component->component_private);
	oscl_param_check(vec_private != NULL, -1, NULL);

	switch (cfg_index) {
	case omx_index_lombo_config_cur_time:
		time = cfg_data;
		vec_private->ref_time = time->nTimestamp;
		break;
	default:
		ret = base_set_config(hcomp, cfg_index, cfg_data);
		break;
	}
	OSCL_TRACE(" %d\n", ret);

	return ret;
}

OMX_ERRORTYPE video_encoder_get_parameter(OMX_IN OMX_HANDLETYPE     hComp,
	OMX_IN OMX_INDEXTYPE    paramIndex,
	OMX_INOUT OMX_PTR       paramData)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	lb_omx_component_t *component;
	venc_component_private_t *vec_private;
	venc_parm_t *encode_parm = NULL;
	unsigned int *parm;

	component = get_lb_component(hComp);
	vec_private = (venc_component_private_t *)(component->component_private);

	switch (paramIndex) {
	case omx_index_lombo_venc_framerate: /* get the framerate of the video */
	case omx_index_lombo_venc_voprefresh: /* get the idr period of the video */
		if (vec_private == NULL) {
			OSCL_LOGE("vec_private is NULL\n");
			ret = OMX_ErrorBadParameter;
			return ret;
		}
		encode_parm = (venc_parm_t *)(vec_private->encode_parm);
		if (encode_parm == NULL) {
			OSCL_LOGE("encode_parm1 is NULL\n");
			ret = OMX_ErrorNotReady;
			return ret;
		}

		parm = (unsigned int *)paramData;
		switch (paramIndex) {
		case omx_index_lombo_venc_framerate:
			*parm = encode_parm->frame_rate;
			break;
		case omx_index_lombo_venc_voprefresh:
			*parm = encode_parm->idr_period;
			break;
		default:
			OSCL_LOGE("should not be here\n");
			break;
		}
		break;
	default:
		ret = base_get_parameter(hComp, paramIndex, paramData);
		break;
	}
	return ret;
}

OMX_ERRORTYPE video_encoder_set_parameter(OMX_IN OMX_HANDLETYPE  hComp,
		OMX_IN OMX_INDEXTYPE  paramIndex,
		OMX_IN OMX_PTR        paramData)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	lb_omx_component_t *component;
	venc_component_private_t *vec_private;
	OMX_PARAM_PORTDEFINITIONTYPE *port_def;
	OMX_PARAM_COMPONENTROLETYPE *pRole = NULL;
	base_port_t *outport = NULL;
	venc_parm_t *encode_parm;

	component = get_lb_component(hComp);
	vec_private = (venc_component_private_t *)(component->component_private);
	if (vec_private == NULL) {
		OSCL_LOGE("vec_private is NULL\n");
		ret = OMX_ErrorBadParameter;
		return ret;
	}
	encode_parm = (venc_parm_t *)(vec_private->encode_parm);
	if (encode_parm == NULL) {
		OSCL_LOGE("encode_parm5 is NULL\n");
		ret = OMX_ErrorBadParameter;
		return ret;
	}

	switch (paramIndex) {
	case OMX_IndexParamPortDefinition:
		ret = base_set_parameter(hComp, paramIndex, paramData);
		if (ret != OMX_ErrorNone) {
			OSCL_LOGE("base set port definition error\n");
			return ret;
		}
		port_def = (OMX_PARAM_PORTDEFINITIONTYPE *)paramData;
		oscl_param_check(port_def->nPortIndex < component->num_ports,
			OMX_ErrorBadParameter, NULL);
		outport = &component->port[OUTPUT_PORT];
		if (port_def->nPortIndex == INPUT_PORT) {
			encode_parm->in_width = port_def->format.video.nFrameWidth;
			encode_parm->in_height = port_def->format.video.nFrameHeight;
			encode_parm->l_stride = port_def->format.video.nStride;
			encode_parm->input_mode = OmxFormat_to_EncodeFormat
						(port_def->format.video.eColorFormat);
			vec_private->cap_plan_num = OmxFormat_to_plan_num
						(port_def->format.video.eColorFormat);
			encode_parm->frame_rate = port_def->format.video.xFramerate;
			if (encode_parm->frame_rate > 1000)
				encode_parm->frame_rate /= 1000;
			outport->port_param.format.video.xFramerate =
					port_def->format.video.xFramerate;
		} else if (port_def->nPortIndex == OUTPUT_PORT) {
			encode_parm->out_width = port_def->format.video.nFrameWidth;
			encode_parm->out_height = port_def->format.video.nFrameHeight;
			encode_parm->bitrate = port_def->format.video.nBitrate;
			OSCL_LOGI("bitrate:%x", port_def->format.video.nBitrate);
		}
		break;

	case omx_index_lombo_venc_bitrate:
		encode_parm->bitrate = *((int *)paramData);
		break;
	case omx_index_lombo_venc_mjpeg_quality:
		encode_parm->quality = *((int *)paramData);
		break;
	case omx_index_lombo_venc_rect:
		memcpy(&vec_private->enc_rect, paramData, sizeof(win_rect_info_t));
		break;
	case omx_index_lombo_input_width:
		encode_parm->in_width = *((int *)paramData);
		break;
	case omx_index_lombo_input_height:
		encode_parm->in_height = *((int *)paramData);
		break;
	case omx_index_lombo_output_width:
		encode_parm->out_width = *((int *)paramData);
		break;
	case omx_index_lombo_output_height:
		encode_parm->out_height = *((int *)paramData);
		break;
	case omx_index_lombo_input_mode:
		encode_parm->input_mode = OmxFormat_to_EncodeFormat(*((int *)paramData));
		vec_private->cap_plan_num = OmxFormat_to_plan_num(*((int *)paramData));
		break;
	case omx_index_lombo_venc_slicemode:
		encode_parm->slice_mode = *((int *)paramData);
		break;
	case omx_index_lombo_venc_framerate:
		encode_parm->frame_rate = *((int *)paramData);
		break;
	case omx_index_lombo_venc_voprefresh:
		encode_parm->idr_period = *((int *)paramData);
		break;

	case omx_index_lombo_process_interval:
		vec_private->proc_interval = *((int *)paramData);
		break;
	case OMX_IndexParamStandardComponentRole:
		pRole = (OMX_PARAM_COMPONENTROLETYPE *)paramData;
		if (strcmp((char *)(pRole->cRole), "video_encode.h264") == 0) {
			vec_private->role_type = H264_ROLE_TYPE;
		} else if (strcmp((char *)(pRole->cRole), "video_encode.jpeg") == 0) {
			vec_private->role_type = JPEG_ROLE_TYPE;
		} else {
			OSCL_LOGE("no support role\n");
			ret = OMX_ErrorBadParameter;
		}
		break;
	case omx_index_lombo_capture_plan_offsets:
		if (paramData == NULL) {
			OSCL_LOGE("paramData is NULL\n");
			ret = OMX_ErrorBadParameter;
			return ret;
		}
		memcpy(vec_private->cap_offsets, paramData, sizeof(int)*3);
		OSCL_LOGI("offsets : %d - %d - %d\n",
			vec_private->cap_offsets[0], vec_private->cap_offsets[1],
				vec_private->cap_offsets[2]);
		break;
	case omx_index_lombo_blending_picture_indexs:
		pthread_mutex_lock(&vec_private->encode_mutex);
		OSCL_LOGI("omx_index_lombo_blending_picture_indexs");
		ret = write_watermark(vec_private, paramData);
		pthread_mutex_unlock(&vec_private->encode_mutex);
		break;
	case omx_index_lombo_disable_blending_picture:
		pthread_mutex_lock(&vec_private->encode_mutex);
		ret = disable_watermark(vec_private, (int)paramData);
		pthread_mutex_unlock(&vec_private->encode_mutex);
		break;
	default:
		ret = base_set_parameter(hComp, paramIndex, paramData);
		break;
	}
	return ret;
}
void *video_encoder_outbuffer_manager(void *param)
{
	lb_omx_component_t *component = NULL;
	venc_component_private_t *vec_private = NULL;
	venc_packet_t *packet = NULL;
	int ret = 0, cnt = 0;
	base_port_t *outport = NULL;
	OMX_BUFFERHEADERTYPE *outbuffer = NULL;
	int flag = 0;
	component = get_lb_component(param);
	outport = &component->port[BASE_OUTPUT_PORT];

	vec_private = (venc_component_private_t *)(component->component_private);
	/* checks if the component is in a state able to receive buffers */
	while (component->state == OMX_StateIdle
		|| component->state == OMX_StateExecuting
		|| component->state == OMX_StatePause) {

		while (outport->is_flushed) {
			if (flag == 0) {
				if (outbuffer)
					vec_private->outbuffer_addr = outbuffer;
				sem_post(vec_private->enc_flush_sem);
				flag = 1;
			}
			oscl_mdelay(20);
			continue;
		}

		if (component->state != OMX_StateExecuting
			|| (vec_private->handle == NULL)
			|| (vec_private->video_encode_info == NULL)
			|| (vec_private->video_encode_info->dequeue_buf == NULL)) {
			oscl_mdelay(20);
			continue;
		}
		flag = 0;



		if (outbuffer == NULL) {
			outbuffer = oscl_queue_dequeue(&outport->buffer_queue);
			if (outbuffer)
				oscl_sem_trywait(component->buf_mgnt_sem);
		}
		if (outbuffer == NULL) {
			cnt++;
			if (cnt >= 20) {
				/*reduce printing frequency,thus reducing
					CPU utilization*/
				cnt = 0;
				OSCL_LOGE("not get outbuffer");
			}
			oscl_sem_wait(component->buf_mgnt_sem);
			continue;
		}
		cnt = 0;
		packet = (venc_packet_t *)outbuffer->pBuffer;
		if (outbuffer->nAllocLen != sizeof(venc_packet_t)) {
			OSCL_LOGE("the alloclen of out buffer is error!");
			return NULL;
		}
		if (packet == NULL) {
			OSCL_LOGE("packet para is NULL");
			return NULL;
		}

		outbuffer->nFlags = 0;
		outbuffer->nFilledLen = 0;
		memset(packet, 0, sizeof(venc_packet_t));
		ret = vec_private->video_encode_info->dequeue_buf(vec_private->handle,
									packet, 0);
		if (packet->vir_addr == NULL) {
			/*OSCL_LOGE("don't have valid data");*/
			oscl_mdelay(10);
			continue;
		}
		/*OSCL_PRINT(">>>> %p\n",packet->vir_addr);*/
		/* the picture type of the frame */
		if (packet->pic_type == VENC_I_FRAME)
			outbuffer->nFlags |= OMX_BUFFERFLAG_SYNCFRAME;
		else if (packet->pic_type == VENC_INIT_PACKET)
			outbuffer->nFlags = OMX_BUFFERFLAG_CODECCONFIG;
		outbuffer->nTimeStamp = packet->time_stamp;
		outbuffer->nFilledLen = sizeof(venc_packet_t);

		if (outbuffer && outbuffer->nFilledLen != 0) {
			outport->return_buffer(outport, outbuffer);
			outbuffer = NULL;
		}
	}
	return NULL;
}

OMX_ERRORTYPE video_encoder_set_state(OMX_HANDLETYPE hComp,
	OMX_U32 dest_state)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	lb_omx_component_t *component;
	venc_component_private_t *vec_private;
	component = get_lb_component(hComp);
	vec_private = (venc_component_private_t *)component->component_private;
	OMX_STATETYPE old_state = component->state;

	OSCL_LOGI("video_encoder_set_state state:%d->%d", old_state, dest_state);
	ret = base_component_set_state(hComp, dest_state);
	if (ret != OMX_ErrorNone) {
		OSCL_LOGE("base component set state error\n");
		return ret;
	}
	if ((component->state == OMX_StateIdle) && (old_state == OMX_StateLoaded)) {
		if (vec_private->init_codec_flag == 0) {
			ret = check_encode_parm(vec_private->encode_parm);
			if (ret != 0) {
				OSCL_LOGE("encode parm error\n");
				ret = OMX_ErrorBadParameter;
				return ret;
			}
			ret = encodec_init(component);
			if (ret != 0) {
				OSCL_LOGE("encodec_init error\n");
				ret = OMX_ErrorDynamicResourcesUnavailable;
				return ret;
			}
			vec_private->init_codec_flag = 1;
		} else {
			OSCL_LOGE("encode lib has been opened, need debug\n");
		}
	} else if ((component->state == OMX_StateLoaded)
			&& (old_state == OMX_StateIdle)) {
		if (vec_private->init_codec_flag == 1) {
			while (vec_private->is_encoding_flag == 1) {
				OSCL_LOGE("wait for encoding end\n");
				oscl_mdelay(200);
			}
			ret = encodec_deinit(component);
			if (ret != 0) {
				OSCL_LOGE("encode deinit error\n");
				return ret;
			}
			vec_private->init_codec_flag = 0;
		} else
			OSCL_LOGE("The lib has not be inited\n");
		if (vec_private->outbuffer_thread_id == 0) {
			pthread_join(vec_private->outbuf_hand, NULL);
			vec_private->outbuffer_thread_id = -1;
			vec_private->outbuf_hand = NULL;
		}
	} else if ((component->state == OMX_StateIdle)
			&& (old_state == OMX_StateExecuting)) {
		while (vec_private->is_encoding_flag == 1) {
			OSCL_LOGE("wait encode frame end\n");
			oscl_mdelay(100);
		}

		vec_private->ref_time = INT_MAX;
	} else if ((component->state == OMX_StateExecuting)
			&& (old_state == OMX_StateIdle)) {
		pthread_attr_t msg_thread_attr;
		struct sched_param shed_param = {0};
		pthread_attr_init(&msg_thread_attr);
		pthread_attr_setstacksize(&msg_thread_attr, 2048);
		shed_param.sched_priority = 8;
		pthread_attr_setschedparam(&msg_thread_attr, &shed_param);
		vec_private->outbuffer_thread_id = pthread_create(
				&vec_private->outbuf_hand, &msg_thread_attr,
				video_encoder_outbuffer_manager, hComp);
	}
	return ret;
}

OMX_ERRORTYPE video_encoder_fill_this_buffer(OMX_IN OMX_HANDLETYPE hComp,
	OMX_IN OMX_BUFFERHEADERTYPE *header)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	lb_omx_component_t *component;
	venc_component_private_t *vec_private;
	venc_packet_t *packet;

	oscl_param_check(hComp != NULL, OMX_ErrorBadParameter, NULL);
	oscl_param_check(header != NULL, OMX_ErrorBadParameter, NULL);
	component = get_lb_component(hComp);
	vec_private = (venc_component_private_t *)(component->component_private);

#ifdef FILL_BUFFER_INTERVAL_DEBUG
	vec_private->encode_num++;
	int time = oscl_get_msec();
	if ((time - vec_private->pre_time) > INTERVAL_TIME) {
		OSCL_LOGE("can not get a empty buffer : %d  %d\n",
			time, vec_private->pre_time);
		/* OSCL_LOGE("can not get a empty buffer3 : %d  %d\n",
			vec_private->pre_num, vec_private->encode_num); */
	}
	vec_private->pre_time = time;
	vec_private->pre_num = vec_private->encode_num;
#endif

#ifdef __EOS__
	rt_hw_cpu_dcache_ops(RT_HW_CACHE_INVALIDATE, header->pBuffer, header->nAllocLen);
#endif

	packet = (venc_packet_t *)header->pBuffer;
	if (packet->vir_addr != NULL) {
		OSCL_LOGI("%p, %p, %p, %d", vec_private->handle, packet,
				packet->vir_addr, packet->pic_type);
		/*OSCL_PRINT("<<< %p\n", packet->vir_addr);*/
		ret = vec_private->video_encode_info->queue_buf(vec_private->handle,
			packet);
		if (ret)
			OSCL_LOGE("return packet to venc codec failed!");
	}

	header->nFilledLen = 0;
	header->nTimeStamp = 0;
	ret = base_fill_this_buffer(hComp, header);

	return ret;
}




void video_encoder_buffer_handle(OMX_HANDLETYPE cmp_handle,
		OMX_BUFFERHEADERTYPE *inbuffer,
		OMX_BUFFERHEADERTYPE *outbuffer)
{
	lb_omx_component_t *component = NULL;
	venc_component_private_t *vec_private = NULL;
	venc_capbuf_t cap_buf;
	/*venc_packet_t *packet;*/
	int ret = 0;
	int rate = 0;
	OMX_TICKS duration;

	component = (lb_omx_component_t *)cmp_handle;
	vec_private = (venc_component_private_t *)(component->component_private);
	if (vec_private->proc_interval) {
		rate = vec_private->encode_parm->frame_rate;
		duration = inbuffer->nTimeStamp - vec_private->last_pts;
#if 0
		OSCL_LOGE("duration: %d;TimeStamp: %lld\n",
			duration,
			inbuffer->nTimeStamp);
#endif

		if (((inbuffer->nTimeStamp > vec_private->proc_interval) ||
			(vec_private->new_pts != 0)) &&
			duration < vec_private->proc_interval) {
			inbuffer->nFilledLen = 0;
			return;
		}
		if (inbuffer->nTimeStamp > vec_private->proc_interval)
			vec_private->last_pts = inbuffer->nTimeStamp;


		inbuffer->nTimeStamp = vec_private->new_pts;
		vec_private->new_pts += (1000 / rate);
#if 1
		OSCL_LOGI("inbuffer->nTimeStamp: %lld\n", inbuffer->nTimeStamp);
		OSCL_LOGI("vec_private->new_pts: %lld\n", vec_private->new_pts);
#endif
	}

	if (inbuffer && inbuffer->nFilledLen) {
		memset(&cap_buf, 0, sizeof(venc_capbuf_t));
		cap_buf.vir_addr[0] = inbuffer->pBuffer + vec_private->cap_offsets[0];
		cap_buf.vir_addr[1] = inbuffer->pBuffer + vec_private->cap_offsets[1];
		cap_buf.vir_addr[2] = inbuffer->pBuffer + vec_private->cap_offsets[2];
		cap_buf.phy_addr[0] = oscl_virt_to_phys((void *)inbuffer->pBuffer);
		cap_buf.phy_addr[1] = cap_buf.phy_addr[0] + vec_private->cap_offsets[1];
		cap_buf.phy_addr[2] = cap_buf.phy_addr[0] + vec_private->cap_offsets[2];
		cap_buf.phy_addr[0] += vec_private->cap_offsets[0];
		cap_buf.time_stamp = inbuffer->nTimeStamp;
		pthread_mutex_lock(&vec_private->encode_mutex);
		ret = vec_private->video_encode_info->encode_frame(vec_private->handle,
									&cap_buf);
		pthread_mutex_unlock(&vec_private->encode_mutex);
		if (ret != 0)
			OSCL_LOGE("h264_enc_frame error\n");
		inbuffer->nFilledLen = 0;
	}
#if 0
	if (outbuffer == NULL)
		goto EXIT;
	packet = (venc_packet_t *)outbuffer->pBuffer;
	oscl_param_check_exit(outbuffer->nAllocLen == sizeof(venc_packet_t), -1, NULL);
	oscl_param_check_exit(packet != NULL, -1, NULL);
	outbuffer->nFlags = 0;
	outbuffer->nFilledLen = 0;
	memset(packet, 0, sizeof(venc_packet_t));

	ret = vec_private->video_encode_info->dequeue_buf(vec_private->handle,
								packet, 10);
	if (ret != 0)
		goto EXIT;
	/* the picture type of the frame */
	if (packet->pic_type == VENC_I_FRAME)
		outbuffer->nFlags |= OMX_BUFFERFLAG_SYNCFRAME;
	else if (packet->pic_type == VENC_INIT_PACKET)
		outbuffer->nFlags = OMX_BUFFERFLAG_CODECCONFIG;
	outbuffer->nTimeStamp = packet->time_stamp;
	outbuffer->nFilledLen = sizeof(venc_packet_t);

EXIT:
#endif
	return;
}

void *video_encoder_buffer_manager(void *param)
{
	lb_omx_component_t *component;
	venc_component_private_t *vec_private;
	base_port_t *inport;
	base_port_t *outport;
	OMX_BUFFERHEADERTYPE *outbuffer = NULL;
	OMX_BUFFERHEADERTYPE *inbuffer = NULL;
	OSCL_TRACE(" %p\n", param);
	oscl_param_check((param != NULL), NULL, NULL);
	component = get_lb_component(param);
	inport = &component->port[BASE_INPUT_PORT];
	outport = &component->port[BASE_OUTPUT_PORT];

	vec_private = (venc_component_private_t *)
					(component->component_private);
	/* checks if the component is in a state able to receive buffers */
	while (component->state == OMX_StateIdle
		|| component->state == OMX_StateExecuting
		|| component->state == OMX_StatePause) {

		pthread_mutex_lock(&component->flush_mutex);
		while (inport->is_flushed || outport->is_flushed) {
			pthread_mutex_unlock(&component->flush_mutex);
			if (inbuffer && inport->is_flushed) {
				inport->return_buffer(inport, inbuffer);
				inbuffer = NULL;
			}
			if (outport->is_flushed) {
				sem_wait(vec_private->enc_flush_sem);
				outbuffer = vec_private->outbuffer_addr;
			}
			if (outbuffer && outport->is_flushed) {
				outport->return_buffer(outport, outbuffer);
				outbuffer = NULL;
			}


			sem_post(component->mgmt_flush_sem);
			sem_wait(component->flush_sem);
			pthread_mutex_lock(&component->flush_mutex);
		}
		pthread_mutex_unlock(&component->flush_mutex);
		if (component->state != OMX_StateExecuting) {
			sem_wait(component->buf_mgnt_sem);
			continue;
		}

		if (inbuffer == NULL) {
			inbuffer = oscl_queue_dequeue(&inport->buffer_queue);
			if (inbuffer)
				oscl_sem_trywait(component->buf_mgnt_sem);
		}

		if (inbuffer == NULL) {
			oscl_sem_wait(component->buf_mgnt_sem);
			continue;
		}

		vec_private->is_encoding_flag = 1;
		if (component->state == OMX_StateExecuting) {
			if (component->buf_handle)
				((component->buf_handle))(component, inbuffer, NULL);
			else
				inbuffer->nFilledLen = 0;
		}

		if (inbuffer && inbuffer->nFilledLen == 0) {
			inport->return_buffer(inport, inbuffer);
			inbuffer = NULL;
		}
		vec_private->is_encoding_flag = 0;
	}
	OSCL_LOGD("exit from buffer_manager\n");
	OSCL_TRACE(" %p\n", param);
	pthread_exit(NULL);
	return NULL;
}

OMX_ERRORTYPE video_encoder_component_deinit(OMX_IN OMX_HANDLETYPE hComp)
{
	lb_omx_component_t *component;
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	venc_component_private_t *vec_private;
	int i = 0;

	component = get_lb_component(hComp);
	vec_private = (venc_component_private_t *)(component->component_private);

	pthread_mutex_destroy(&vec_private->encode_mutex);
	for (i = 0; i < vec_private->wartermark.watermark_picture_num; i++) {
		if (vec_private->wartermark.watermark_pictures[i].vir_addr != NULL) {
			oscl_free_align(
				vec_private->wartermark.watermark_pictures[i].vir_addr);
			vec_private->wartermark.watermark_pictures[i].vir_addr = NULL;
		}
	}
	if (vec_private->encode_parm != NULL) {
		oscl_free(vec_private->encode_parm);
		vec_private->encode_parm = NULL;
	}

	if (vec_private->enc_flush_sem != NULL) {
		sem_destroy(vec_private->enc_flush_sem);
		oscl_free(vec_private->enc_flush_sem);
		vec_private->enc_flush_sem = NULL;
	}

	if (vec_private->outbuffer_thread_id == 0) {
		pthread_join(vec_private->outbuf_hand, NULL);
		vec_private->outbuffer_thread_id = -1;
		vec_private->outbuf_hand = NULL;
	}

	if (component->component_private != NULL) {
		oscl_free(component->component_private);
		component->component_private = NULL;
	}
	base_port_deinit(&component->port[INPUT_PORT]);
	base_port_deinit(&component->port[OUTPUT_PORT]);
	ret = base_component_deinit(hComp);
	return ret;
}


OMX_ERRORTYPE video_encoder_component_init(lb_omx_component_t *cmp_handle)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	venc_component_private_t *vec_private = NULL;
	venc_parm_t *encode_parm = NULL;
	OMX_COMPONENTTYPE *base_cmp = &(cmp_handle->base_comp);

	ret = base_component_init(cmp_handle);
	if (ret != OMX_ErrorNone) {
		OSCL_LOGE(" base init component error\n");
		return ret;
	}

	vec_private = oscl_malloc(sizeof(venc_component_private_t));
	if (vec_private == NULL) {
		OSCL_LOGE("malloc vec_private error\n");
		ret = OMX_ErrorInsufficientResources;
		goto err_init_exit;
	}
	memset(vec_private, 0, sizeof(venc_component_private_t));
	vec_private->outbuffer_thread_id = -1;
	vec_private->enc_flush_sem = (sem_t *)oscl_zalloc(sizeof(sem_t));
	if (vec_private->enc_flush_sem == NULL) {
		OSCL_LOGE("malloc vec_private enc over sem error\n");
		ret = OMX_ErrorInsufficientResources;
		goto err_init_exit;
	}
	sem_init(vec_private->enc_flush_sem, 0, 0);


	encode_parm = (venc_parm_t *)oscl_malloc(sizeof(venc_parm_t));
	if (encode_parm == NULL) {
		OSCL_LOGE("encode_parm malloc error\n");
		ret = OMX_ErrorDynamicResourcesUnavailable;
		goto err_init_exit;
	}
	memset(encode_parm, 0, sizeof(venc_parm_t));

	vec_private->encode_parm = encode_parm;
	vec_private->ref_time = INT_MAX;
	pthread_mutex_init(&vec_private->encode_mutex, NULL);

	cmp_handle->name = "OMX.LB.VIDEO.ENCODECOMPONENT";
	cmp_handle->component_private = (OMX_PTR)vec_private;
	cmp_handle->buf_manager = video_encoder_buffer_manager;
	cmp_handle->buf_handle = video_encoder_buffer_handle;
	cmp_handle->do_state_set = video_encoder_set_state;
	cmp_handle->num_ports = 2;

	video_encode_port_init(cmp_handle, &cmp_handle->port[INPUT_PORT], INPUT_PORT,
		OMX_DirInput);
	video_encode_port_init(cmp_handle, &cmp_handle->port[OUTPUT_PORT], OUTPUT_PORT,
		OMX_DirOutput);

	base_cmp->SetConfig	      = video_encoder_set_config;
	base_cmp->SetParameter        = video_encoder_set_parameter;
	base_cmp->GetParameter        = video_encoder_get_parameter;
	base_cmp->FillThisBuffer      = video_encoder_fill_this_buffer;
	base_cmp->ComponentDeInit     = video_encoder_component_deinit;
	base_cmp->EmptyThisBuffer     = video_encoder_empty_this_buffer;

	pthread_attr_setstacksize(&cmp_handle->buf_thread_attr, 0x2000);

	return ret;

err_init_exit:

	if (encode_parm != NULL) {
		oscl_free(encode_parm);
		vec_private->encode_parm = NULL;
	}

	if (vec_private != NULL) {
		oscl_free(vec_private);
		vec_private = NULL;
	}
	return ret;
}

#ifdef OMX_DYNAMIC_LOADING
void *omx_component_init = video_encoder_component_init;
#endif

