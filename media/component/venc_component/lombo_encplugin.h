#ifndef LOMBO_ENCPLUGIN_H
#define LOMBO_ENCPLUGIN_H

#include "lombo_enc_common.h"

#define VENC_MAX_BLENDING_PICTURES 21
#define VENC_MIN_STREAM_BUF_SIZE (2*1024)

typedef enum {
	INVALID,
	VENC_INIT_PACKET,
	VENC_I_FRAME,
	VENC_P_FRAME
} venc_frame_type_e;

typedef enum {
	VENC_FRAME_END_STATUS_INIT,
	VENC_FRAME_END,
	VENC_FRAME_NOT_END
} venc_frame_end_status_e;

typedef enum {
	VENC_SET_BLENDING_PICTURE,
	VENC_DISABLE_BLENDING,
	VENC_FORCE_INTRA,
	VENC_SET_BLOCKING
} venc_cmd_e;

typedef struct {
	int in_width; /* the input video width */
	int in_height;
	int l_stride; /* the input video width aligned*/
	int out_width;
	int out_height;
	int bitrate; /* the output raw data bitrate */
	int slice_mode; /* 0:sigle 1:muti */
	int input_mode; /* ENC_INPUT_MODE type */
	int idr_period;
	int frame_rate;
	int quality; /* support for mjpeg */
} venc_parm_t;

typedef struct {
	unsigned char *vir_addr[3];
	unsigned long phy_addr[3];
	long long time_stamp;
} venc_capbuf_t;


typedef struct {
	unsigned char *vir_addr;
	unsigned long phy_addr;
	int size;
	venc_frame_type_e pic_type;
	long long time_stamp;
	venc_frame_end_status_e frame_end_flag;
} venc_packet_t;

#ifdef __LINUX__

#elif defined __EOS__
	void *h264_enc_open(venc_parm_t *enc_parm, int buf_size);
	int h264_queue_buf(void *handle, venc_packet_t *p_frame);
	int h264_enc_frame(void *handle, venc_capbuf_t *capbuf_s);
	int h264_dequeue_buf(void *handle, venc_packet_t *p_frame, int wait_time);
	int h264_enc_close(void *handle);
	int h264_enc_ops(void *handle, venc_cmd_e cmd, void *arg);

	void *mjpeg_enc_open(venc_parm_t *enc_parm, int buf_size);
	int mjpeg_queue_buf(void *handle, venc_packet_t *p_frame);
	int mjpeg_enc_frame(void *handle, venc_capbuf_t *capbuf_s);
	int mjpeg_dequeue_buf(void *handle, venc_packet_t *p_frame, int wait_time);
	int mjpeg_enc_close(void *handle);
	int mjpeg_enc_ops(void *handle, venc_cmd_e cmd, void *arg);
#else

#endif

typedef struct {
	void *(*open)(venc_parm_t *enc_parm, int buf_size);
	int (*queue_buf)(void *handle, venc_packet_t *p_frame);
	int (*encode_frame)(void *handle, venc_capbuf_t *capbuf_s);
	int (*dequeue_buf)(void *handle, venc_packet_t *p_frame, int wait_time);
	int (*close)(void *handle);
	int (*ex_ops)(void *handle, venc_cmd_e cmd, void *arg);
} venc_plugin_t;

#endif
