#ifndef __VIDEO_ENCODE_COMPOMEMT_H__
#define __VIDEO_ENCODE_COMPOMEMT_H__

#include "base_component.h"
#include "lombo_encplugin.h"

#define INPUT_PORT 0
#define OUTPUT_PORT 1
#define H264_INFO_SIZE 1024

/* #define FILL_BUFFER_INTERVAL_DEBUG */
#define INTERVAL_TIME 100

typedef enum OMX_VIDEO_ENCODER_ROLE {
	UNSUPPORT_ROLE_TYPE = 0,
	H264_ROLE_TYPE,
	JPEG_ROLE_TYPE,
} OMX_VIDEO_ENCODER_ROLE;

typedef struct wm_data {
	int index; /* blending index */
	watermark_picture_t wm_pic;
} wm_data_t;
typedef struct win_rect_info {
	int   x;
	int   y;
	int   width;
	int   height;
} win_rect_info_t;

typedef struct venc_component_private {
	int role_type;
	int init_codec_flag;
	void *handle;
	venc_parm_t *encode_parm;
	void *lib_handle;
	win_rect_info_t enc_rect;
	unsigned int cap_offsets[3];
	unsigned int cap_plan_num;
	venc_plugin_t *video_encode_info;
	unsigned int is_encoding_flag; /* marked omx is encoding a frame */
	watermark_t wartermark;
	pthread_mutex_t encode_mutex;
	OMX_S64 ref_time;
	int proc_interval;  /* unit: ms */
	OMX_TICKS start_pts;
	OMX_TICKS new_pts;
	OMX_TICKS last_pts;
	sem_t *enc_flush_sem;
	OMX_BUFFERHEADERTYPE *outbuffer_addr;
	OMX_S32 outbuffer_thread_id;
	pthread_t outbuf_hand;
#ifdef FILL_BUFFER_INTERVAL_DEBUG
	int pre_time;
	int encode_num;
	int pre_num;
#endif
} venc_component_private_t;

void al_vc_core_init();
OMX_ERRORTYPE video_encode_port_init(lb_omx_component_t *component,
	base_port_t *base_port, OMX_U32 index, OMX_U32 dir_type);
#endif
