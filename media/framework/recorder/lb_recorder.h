/*
 * lb_recorder.h - Standard functionality for lombo recorder.
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef __LB_RECORDER_H_
#define __LB_RECORDER_H_
#include "OMX_IVCommon.h"

typedef enum recorder_type {
	RECORDER_TYPE_NORMAL,  /* normal speed recorder */
	RECORDER_TYPE_TIME_LAG,/* time-lapse video */
} recorder_type_e;

typedef enum muxer_chace_tag {
	RECORDER_CACHE_NOT_USE,
	RECORDER_CACHE_USE,
} muxer_chace_e;

typedef enum recorder_state {
	RECORDER_STATE_ERR = 0,
	RECORDER_STATE_INIT,
	RECORDER_STATE_PREPARED,
	RECORDER_STATE_RECORD,
} recorder_state_t;

typedef enum al_frame_type {
	AL_INVAILD_FRAME,
	AL_VIDEO_RAW_FRAME,
	AL_VIDEO_ROT_FRAME,
	AL_AUDIO_FRAME,
	AL_IMAGE_FRAME,
	AL_OTHER_FRAME,
} al_frame_type_t;

typedef struct al_video_frame_info {
	OMX_COLOR_FORMATTYPE color_fmt;
	int width;
	int height;
	int planar;
	u8 *addr[3];
	int size[3];
	s64 time_stamp;
} al_video_frame_info_t;

typedef struct al_audio_frame_info {
	int sample_rate;
	char *addr;
	char *size;
	s64 time_stamp;
} al_audio_frame_info_t;

typedef struct al_frame {
	al_frame_type_t type;
	union {
		al_video_frame_info_t video;
		al_audio_frame_info_t audio;
	} info;
	void *al_data;
	void *header;
} al_frame_t;

typedef struct app_sink_callback_info {
	al_frame_type_t type;
	void *app_data;
	int(*buf_handle)(void *, al_frame_t *);
} app_frame_cb_t;

typedef struct user_def_sys_cfg {
	char camera_buf_num;	/*camera buffer number*/
} user_def_sys_cfg_t;

typedef enum OUTPUT_FORMAT {
	REC_OUTPUT_FORMAT_NULL = 0,
	REC_OUTPUT_FORMAT_MP4 = 1,
	REC_OUTPUT_FORMAT_MOV = 2,
	REC_OUTPUT_FORMAT_RAW = 3,
	REC_OUTPUT_FORMAT_TS = 4,
	REC_OUTPUT_FORMAT_AUDIO_AAC = 0x100,
	REC_OUTPUT_FORMAT_NONE = 0x7fff
} output_format_t;

typedef enum VIDEO_ENCODER {
	VIDEO_ENCODER_NULL = 0,
	VIDEO_ENCODER_H264,
	VIDEO_ENCODER_NONE = 0x7fff
} video_encoder_t;

typedef enum AUDIO_ENCODER {
	AUDIO_ENCODER_NULL = 0,
	AUDIO_ENCODER_AAC,
	AUDIO_ENCODER_ADPCM,
	AUDIO_ENCODER_PCM,
	AUDIO_ENCODER_NONE = 0x7fff
} audio_encoder_t;

typedef enum VIDEO_LAYER_LEVEL {
	VIDEO_LAYER_TOP = 0,
	VIDEO_LAYER_BOTTOM,
	VIDEO_LAYER_NONE = 0x7fff
} video_layer_level_t;

typedef enum video_rotate {
	VIDEO_ROTATE_NONE	= 0,
	VIDEO_ROTATE_90		= 1,
	VIDEO_ROTATE_180	= 2,
	VIDEO_ROTATE_270	= 3,
	VIDEO_ROTATE_FLIP_H		= 4,
	VIDEO_ROTATE_FLIP_H_ROT_90	= 5,
	VIDEO_ROTATE_FLIP_V		= 6,
	VIDEO_ROTATE_FLIP_V_ROT_90	= 7,
} video_rotate_e;

typedef enum VIDEO_DISP_MODE {
	/* Display in the window at the original size of the video,
	 * can't overflow the window */
	VIDEO_WINDOW_ORIGINAL,
	/* Scale to full screen by video ratio, the video show normal */
	VIDEO_WINDOW_FULL_SCREEN_VIDEO_RATIO,
	/* Scale to full screen by screen ratio, the video may be distorted */
	VIDEO_WINDOW_FULL_SCREEN_SCREEN_RATIO,
	/* Forced to display at 4:3 ratio, the video may be distorted */
	VIDEO_WINDOW_4R3MODE,
	/* Forced to display at 16:9 ratio, the video may be distorted */
	VIDEO_WINDOW_16R9MODE,
	/* Used to cut off the black side of the video */
	VIDEO_WINDOW_CUTEDGE,
	/* User defined mode */
	VIDEO_WINDOW_USERDEF,
} video_disp_mode_t;

typedef struct win_rect {
	int   x;
	int   y;
	int   width;
	int   height;
} win_rect_t;

typedef struct vsize {
	int   width;
	int   height;
} vsize_t;

typedef struct win_para {
	video_disp_mode_t mode;
	win_rect_t rect;  /* Window postion and size in the csreen */
	win_rect_t crop;  /* Need display image pos and size in the framebuffer,
			   * default full image */
} win_para_t;

typedef struct rec_param {
	/* audio para */
	int audio_channels;		/* Number of channels (e.g. 2 for stereo) */
	int audio_sample_width;	/* Bit per sample */
	int audio_sample_rate;	/* Sampling rate of the source data.
				   Use 0 for variable or unknown sampling rate. */

	/* video para */
	int source_height;
	int source_width;
	int frame_rate;
	video_rotate_e rotate;

	/* encoder para */
	win_rect_t enc_rect;
	audio_encoder_t aenc_format;
	video_encoder_t venc_format;
	int height;
	int width;
	int bitrate;
	output_format_t file_fmt;
	int muxer_cache_flag;

#if 0
	/* fast record para */
	int interval;	/* unit: ms */
	int play_framerate;
#endif
} rec_param_t;
typedef struct rec_time_lag_para {
	int interval;	/* coded fream sampling interval */
	int play_framerate;/* play fream rate */
} rec_time_lag_para_t;
typedef struct img {
	char path[64];
	char format[8]; /* img format, support nv12,nv21 */
	int width;
	int height;
} img_t;

/* panoramic calibration para */
typedef struct cali_param {
	int  box_rows;	/* the number of rows in the back and white grid */
	int  box_cols;	/* the number of columns in the back and white grid */
	int  box_width;	/* back and white grid width (unit: cm) */
	int  box_height;	/* back and white grid height (unit: cm) */
	int  dist_2_rear;	/* the bottom row of the grid to the rear of the car */
	int  car_width;	/* car width */
	int  car_length;	/* car length */

	int  front_dist;    /* car front distance (unit: cm) */
	int  rear_dist;	/* car rear distance (unit: cm)  */
	int  align;	    /* rear direction. -1 or 1*/

	int use_ext_cali_img;  /* use extern calibration image, only test */
	img_t ext_cali_img; /* extern calibration image */
} cali_param_t;

typedef struct cali_contex {
	int cutline_dnthr;
	int cutline_upthr;
	int cutline;
	win_rect_t car_rect;
} cali_contex_t;

/* panoramic calibration out data */
typedef struct cali_out_data {
	int data_size;
	void *data; /* openmax malloc, app need to free memory */
} cali_out_data_t;

/* panoramic init para */
typedef struct pano_param {
	int in_gps;	  /* use gps speed */
	int in_obd;	  /* use obd para */

	/* car para */
	int car_para_en;	/* whether to enabel use paras */
	int car_width;      /* car width (unit: cm) */

	img_t carboard_img;
	char *data_format;

	int warning_line[3];

	int use_ext_cutline;
	int culine;
} pano_param_t;

typedef struct fix_duration_param {
	int file_duration;
	int (*cb_get_next_file)(void *hdl, char *next_file);
	int (*cb_file_closed)(void *hdl, char *file_name);
	int (*cb_get_user_data)(void *hdl, char **user_data, unsigned int *data_size);
	/*user data buff is maintained by application,
	multimedia middleware only for use*/
} fix_duration_param_t;

/* watermark definitions */
#define MAX_BLENDING_PICTURES 21

typedef struct {
	unsigned char *data;
	unsigned int phy_addr;
	unsigned int width;
	unsigned int height;
	unsigned int picture_size;
	unsigned int stride;
} numeral_picture_t;

typedef struct {
	numeral_picture_t numeral_picture[MAX_BLENDING_PICTURES];
	unsigned int input_picture_num;
	unsigned int colorspace;
} numeral_input_t;

typedef struct {
	char index_array[64];
	unsigned int total_index_num;
	unsigned int start_x_pos;
	unsigned int start_y_pos;
	unsigned int blending_area_index;
} numeral_picture_index_t;


typedef enum LB_REC_TYPE {
	LB_REC_INVALID =  0,

	LB_REC_PREPARE,
	LB_REC_PREVIEW,
	LB_REC_PAUSE_PREVIEW,
	LB_REC_CONTINUE_PREVIEW,
	LB_REC_STOP_PREVIEW,
	LB_REC_START,
	LB_REC_STOP,
	LB_REC_PAUSE,
	LB_REC_GET_TIME,		/* int * */
	LB_REC_GET_STATUS,		/* recorder_state_t */

	LB_REC_SET_PARA,		/* rec_param_t */
	LB_REC_GET_PARA,		/* rec_param_t */

	LB_REC_SET_MODE,		/* recording mode */
	LB_REC_SET_TIME_LAG_PARA,	/* parameter of time lag recording */

	/* source */
	LB_REC_SET_VIDEO_SOURCE,	/* ptr: source dev name */
	LB_REC_SET_CAP_RATE,		/* capture frame rate */
	LB_REC_SET_WATERMARK_SOURCE,	/* numeral_input_t */
	LB_REC_SET_WATERMARK_INDEX,	/* numeral_picture_index_t */

	/* encode */
	LB_REC_SET_OUTPUT_FILE,		/* ptr: output filename */
	LB_REC_SET_OUTPUT_FORMAT,	/* output_format_t */
	LB_REC_SET_FIX_DURATION_PARA,	/* fix_duration_param_t */

	LB_REC_TAKE_PICTURE,	/* filename */

	/* preview */
	LB_REC_SET_SINK,	/* ptr: display dev name */
	LB_REC_SET_ROTATE,	/* video_rotate_e */
	LB_REC_SET_USER_DEF_SYS_PRAR,/*user_def_sys_cfg_t*/
	LB_REC_SET_CB_FRAME,	/* app_frame_cb_t */
	LB_REC_FREE_FRAME,	/* al_frame_t */
	LB_REC_SET_LAYER_LEVEL, /* video_layer_level_t */
	LB_REC_SET_DISP_PARA,	/* win_para_t */

	/* audio */
	LB_REC_SET_AUDIO_SOURCE,	/* ptr: source dev name */
	LB_REC_SET_AUDIO_MUTE,		/* 0:disable mute; 1:mute */

	/* panoramic */
	LB_REC_PANO_CREAT,
	LB_REC_PANO_RELEASE,
	LB_REC_PANO_START,
	LB_REC_PANO_STOP,
	LB_REC_PANO_SET_LAYER_LEVEL, /* video_layer_level_t */
	LB_REC_PANO_CALI_PROCESS,
	LB_REC_PANO_SET_PREVIEW_SIZE,
	LB_REC_PANO_SET_DISP_MODE,
	LB_REC_PANO_SET_CALI_PARA,
	LB_REC_PANO_SET_INIT_PARA,
	LB_REC_PANO_SET_CALI_DATA,
	LB_REC_PANO_GET_CALI_DATA,
} lombo_rec_cmd_t;

static inline const char *media_cmd_as_string(lombo_rec_cmd_t i)
{
	switch (i) {
	case LB_REC_PREPARE:
		return "cmd_prepare";
	case LB_REC_PREVIEW:
		return "cmd_preview";
	case LB_REC_STOP_PREVIEW:
		return "cmd_stop_preview";
	case LB_REC_START:
		return "cmd_start";
	case LB_REC_STOP:
		return "cmd_stop";
	case LB_REC_PAUSE:
		return "cmd_pause";
	case LB_REC_GET_TIME:
		return "cmd_get_time";
	case LB_REC_GET_STATUS:
		return "cmd_get_stat";
	case LB_REC_SET_PARA:
		return "cmd_set_para";
	case LB_REC_GET_PARA:
		return "cmd_get_para";
	case LB_REC_SET_VIDEO_SOURCE:
		return "cmd_set_video_source";
	case LB_REC_SET_CAP_RATE:
		return "cmd_set_cap_rate(frame rate)";
	case LB_REC_SET_OUTPUT_FILE:
		return "cmd_set_outpu_file";
	case LB_REC_SET_CB_FRAME:
		return "cmd_set_cb_frame";
	case LB_REC_SET_LAYER_LEVEL:
		return "cmd_set_layer_level";
	case LB_REC_SET_SINK:
		return "cmd_set_sink";

	default:
		break;
	}
	return "unknown media cmd??";
}

void lb_recorder_release(void *hdl);
void *lb_recorder_creat();
int lb_recorder_ctrl(void *hdl, int cmd, void *para);


#endif

