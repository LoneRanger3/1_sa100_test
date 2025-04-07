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

#ifndef __RECORDER_PRIVATE_H_
#define __RECORDER_PRIVATE_H_
#include "watermark/watermark.h"

#define REC_SRC_FRONT_CAMERA   "vic"
#define REC_SRC_BACK_CAMERA   "mcsi"
#define REC_DEFAULT_VIDEO_SRC	REC_SRC_FRONT_CAMERA

#define REC_DISPLAY_DEVICE_NAME	"disp"
#define REC_DISPLAY_LAYER_NAME	"vrender"

#define REC_INPUT_CHANNEL_NUM	1

#define REC_DEF_VIDEO_COLOR_FMT OMX_COLOR_FormatYUV420SemiPlanar
#define REC_VIDEO_SIZE_VGA	(480)  /* 640*480 */
#define REC_VIDEO_SIZE_720P	(720)  /* 1280*720 */
#define REC_VIDEO_SIZE_1080P	(1080) /* 1920*1080 */
#define REC_VIDEO_SIZE_DEFAULT	REC_VIDEO_SIZE_720P

#define CAMERA_DEFAULT_BUF_NUM	(6)
#define CAMERA_CAP_BUF_NUM	(6)
#define CAMERA_PREV_BUF_NUM	(6)



#define PANO_DEFAULT_BUF_NUM 4
#define ENCODER_DEFAULT_BUF_NUM 16
#define ENCODER_BUF_SIZE (1*1024)

#define VIDEO_EXTRA_DATA_BUFSIZE (1024)

typedef enum rec_state {
	REC_STATE_ERR = 0,
	REC_STATE_INIT,
	REC_STATE_PREPARED,
	REC_STATE_PREVIEW,
	REC_STATE_RECORD
} rec_state_t;

typedef enum video_source_id {
	VSRC_ID_UNUSED = 0,
	VSRC_FRONT_CAMERA,
	VSRC_BACK_CAMERA,
} vsrc_id_t;

typedef struct vsink_info {
	al_comp_info_t al_comp;
	al_port_info_t *vin;
	char *dev_name;
	vdisp_para_t disp_para;
	int rotate;
} vsink_info_t;

typedef struct al_sink_info {
	al_comp_info_t al_comp;
	al_port_info_t *in;
	app_frame_cb_t cb;
	int enabled;
} al_sink_info_t;

typedef struct vsrc_info {
	al_comp_info_t al_comp;
	OMX_VIDEO_PORTDEFINITIONTYPE video;
	char *dev_name;
	int channel;
	al_port_info_t *vout;
} vsrc_info_t;

#define SPLT_OUT_VREND 1
#define SPLT_OUT_VENC 2
typedef struct splt_info {
	al_comp_info_t al_comp;
	al_port_info_t *in;
} splt_info_t;

typedef struct venc_info {
	al_comp_info_t al_comp;
	al_port_info_t *vin;
	al_port_info_t *vout;
	int slice_mode;
	int bitrate;
	int out_width;
	int out_height;
	win_rect_t venc_rect;

	/* fast record */
	int play_framerate;
	int interval;  /* encode interval, unit: ms*/
} venc_info_t;

typedef struct vrot_info {
	al_comp_info_t al_comp;
	al_port_info_t *vin;
	al_port_info_t *vout;
	OMX_CONFIG_ROTATIONTYPE mode;
} vrot_info_t;

typedef struct mux_info {
	al_comp_info_t al_comp;
	al_port_info_t *ain;
	al_port_info_t *vin;
	output_format_t fmt;
	char *filename;
} mux_info_t;

typedef struct asrc_info {
	al_comp_info_t al_comp;
	char *dev_name;
	int channel;
	al_port_info_t *aout;
} asrc_info_t;

typedef struct aenc_info {
	al_comp_info_t al_comp;
	al_port_info_t *ain;
	al_port_info_t *aout;
} aenc_info_t;

typedef struct pano_info {
	al_comp_info_t al_comp;
	al_port_info_t *vin;
	al_port_info_t *vout;
	int preview_w;
	int preview_h;
} pano_info_t;

#define AUDIO_STREAM_MAXHDL	4

typedef struct audio_stream {
	OMX_U32 channels;	/* Number of channels (e.g. 2 for stereo) */
	OMX_U32 bit_per_sample;	/* Bit per sample */
	OMX_U32 sample_rate;	/* Sampling rate of the source data.
				   Use 0 for variable or unknown sampling rate. */
	OMX_AUDIO_CODINGTYPE coding_type;

	rec_state_t cur_state;
	asrc_info_t asrc_info;
	aenc_info_t aenc_info;
	splt_info_t aenc_splt;
	rec_state_t state[AUDIO_STREAM_MAXHDL];
	al_port_info_t *aenc_splt_out[AUDIO_STREAM_MAXHDL];
	mux_info_t *mux_info[AUDIO_STREAM_MAXHDL];
	pthread_mutex_t lock;
	int ref;
	int hdl_map;
} audio_stream_t;
typedef int astream_handle;

typedef struct vsrc_camera {
	pthread_mutex_t *lock;
	vsrc_info_t vsrc_info;
	splt_info_t vsplt_info;
	vrot_info_t vrot_info;
	al_port_info_t *splt_rot;
	int vsplt_map;
	int rotate_map;
} vsrc_camera_t;

typedef struct video_recorder {
	video_rotate_e rot;
	rec_state_t state;
	recorder_type_e type;
	user_def_sys_cfg_t userdef_sys_para;
	int audio_en;
	int video_en;
	int prev_en;
	int is_isp_cap;
	rec_param_t rec_para;
	al_port_info_t *src_venc;
	al_port_info_t *src_vsink;
	al_port_info_t *src_vcb;
	al_port_info_t *src_pic;
	al_port_info_t *splt_pano;

	/* camera */
	vsrc_camera_t *camera;
	vsrc_camera_t *enc_cam;

	/* panoramic */
	pano_info_t pano_info;
	vrot_info_t prot_info;
	vsink_info_t psink_info;
	rec_state_t pstate;

	venc_info_t venc_info;
	vsink_info_t vsink_info;

	astream_handle audio;
	mux_info_t mux_info;
	s64 aref_time;
	s64 vref_time;

	void *pic_private;
	void *watermark_private;
} video_recorder_t;

typedef video_recorder_t *video_rec_handle;

typedef struct lb_recorder {
	pthread_mutex_t *lock;
	video_rec_handle video_rec;
} lb_recorder_t;

#define MAX_CAM_OPENED 4
typedef struct _cam_manager {
	pthread_mutex_t lock;
	vsrc_camera_t *cam_hdl[MAX_CAM_OPENED];
	int refcnt[MAX_CAM_OPENED];
} cam_manager_t;

vsrc_camera_t *get_enc_cam(video_recorder_t *video_rec);
void put_enc_cam(video_recorder_t *video_rec, vsrc_camera_t *enc_cam);
int _splt_prepare(splt_info_t *splt_info, OMX_PARAM_PORTDEFINITIONTYPE *port);
void *video_rec_creat(void);
void video_rec_release(void *hdl);
int video_rec_take_picture(video_recorder_t *video_rec, char *file);
int video_rec_set_mute(video_recorder_t *video_rec, int mute);
int video_rec_wm_set_bnp(video_recorder_t *video_rec, numeral_input_t *bnp);
int video_rec_wm_set_mark(video_recorder_t *video_rec, numeral_picture_index_t *mk);
int video_rec_start(video_rec_handle video_rec);
int video_rec_stop(video_rec_handle video_rec);
int video_rec_preview(video_rec_handle video_rec);
int video_rec_pause_preview(video_recorder_t *video_rec);
int video_rec_continue_preview(video_recorder_t *video_rec);
int video_rec_prepare(video_rec_handle video_rec);
int video_rec_set_video_source(video_rec_handle video_rec, char *src_name);
int video_rec_set_video_sink(video_recorder_t *video_rec, char *dev);
int video_rec_set_output_file(video_recorder_t *video_rec, char *filename);
int video_rec_set_output_format(video_recorder_t *video_rec, output_format_t fmt);
int video_rec_set_rotate(video_recorder_t *video_rec, int rotation);
int video_rec_set_user_def_sys_cfg(video_recorder_t *video_rec, void *p);
int video_set_layer_level(video_recorder_t *video_rec, video_layer_level_t level);
int video_rec_set_max_duration(video_recorder_t *video_rec, int *p_min);
int video_rec_set_frame_cb(video_recorder_t *video_rec, app_frame_cb_t *cb);
int video_rec_set_fix_duration_param(video_recorder_t *video_rec,
	void *hdl, fix_duration_param_t *param);
int app_empty_buffer_done(al_frame_t *frame);

int video_rec_set_timelag_para(video_recorder_t *video_rec,
	rec_time_lag_para_t *para);
int video_rec_set_mode(video_recorder_t *video_rec, int mode);
int video_rec_set_para(video_recorder_t *video_rec, rec_param_t *rec_para);
int video_rec_get_para(video_recorder_t *video_rec, rec_param_t *rec_para);
int video_set_disp_para(video_recorder_t *video_rec, win_para_t *disp_para);
int video_rec_stop_preview(video_recorder_t *video_rec);
int video_rec_get_time(video_recorder_t *video_rec, int *time);
int video_rec_get_status(video_recorder_t *video_rec, int *status);

astream_handle astream_creat(void);
int astream_prepare(astream_handle hdl);
void astream_release(astream_handle hdl);
int astream_start(astream_handle hdl);
int astream_stop(astream_handle hdl);
int astream_reset(astream_handle hdl);
int astream_set_muxer(astream_handle hdl, mux_info_t *muxer);
int astream_set_coding_type(astream_handle hdl, OMX_AUDIO_CODINGTYPE coding);
int astream_set_prio(astream_handle hdl, int prio);
int astream_set_channels(astream_handle hdl, int channels);
int astream_set_sample_width(astream_handle hdl, int bits);
int astream_set_sample_rate(astream_handle hdl, int rate);
s64 astream_get_time(astream_handle hdl);
int astream_get_sample_rate(astream_handle hdl);
int astream_get_sample_width(astream_handle hdl);
int astream_get_channels(astream_handle hdl);
int astream_get_coding_type(astream_handle hdl);
int astream_set_mute(astream_handle hdl, int mute_on);

int video_pano_cali_process(video_recorder_t *video_rec, cali_contex_t *ctx);
int video_pano_start(video_recorder_t *video_rec);
int video_pano_stop(video_recorder_t *video_rec);
int video_pano_get_cali_data(video_recorder_t *video_rec, cali_out_data_t *out);
int video_pano_reset(video_recorder_t *video_rec);
int video_pano_set_cali_data(video_recorder_t *video_rec, cali_out_data_t *out);
int video_pano_set_cali_para(video_recorder_t *video_rec, cali_param_t *para);
int video_pano_set_disp_mode(video_recorder_t *video_rec,	win_para_t *win);
int video_pano_set_init_para(video_recorder_t *video_rec,	pano_param_t *para);
int video_pano_set_preview_size(video_recorder_t *video_rec, vsize_t *size);
int video_pano_set_layer_level(video_recorder_t *video_rec,
			video_layer_level_t level);
int video_pano_creat(video_recorder_t *video_rec);
void video_pano_release(video_recorder_t *video_rec);

vsrc_camera_t *vsrc_camera_creat(void);
void vsrc_camera_release(vsrc_camera_t *camera);
int vsrc_camera_prepare(vsrc_camera_t *camera);
int vsrc_camera_set_para(vsrc_camera_t *camera, rec_param_t *rec_para);
int vsrc_camera_set_device(vsrc_camera_t *camera, char *src_name);
al_port_info_t *vsrc_camera_getport(vsrc_camera_t *camera, vdisp_rotate_mode_e mode);
int vsrc_camera_putport(vsrc_camera_t *camera, al_port_info_t *port);
int vsrc_camera_getpara_portinfo(vsrc_camera_t *camera,
	al_port_info_t *port, OMX_PARAM_PORTDEFINITIONTYPE *portinfo);
int vsrc_camera_enable_port(vsrc_camera_t *camera, al_port_info_t *port);
int vsrc_camera_disable_port(vsrc_camera_t *camera, al_port_info_t *port);
s64 vsrc_camera_get_time(vsrc_camera_t *camera);
int vsrc_camera_get_para(vsrc_camera_t *camera, rec_param_t *rec_para);
void vsrc_camera_set_buffer_num(vsrc_camera_t *camera, int num);
void *cam_manager_get_device(char *name);
void cam_manager_put_device(void *hdl);

watermark_priv_t *watermark_init(void);
void watermark_deinit(void *handle);
int watermark_set_bnp(watermark_priv_t *priv, numeral_input_t *bnp);
numeral_output_t *watermark_set_mark(watermark_priv_t *priv,
				numeral_picture_index_t *wm_config);
int watermark_get_markarray(watermark_priv_t *priv, numeral_output_t **mark_array);
int watermark_put_markarray(watermark_priv_t *priv, numeral_output_t *mark_array);

int take_pic_init(video_recorder_t *video_rec);
void take_pic_deinit(video_recorder_t *video_rec);

#endif

