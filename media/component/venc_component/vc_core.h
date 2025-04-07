#ifndef _VC_CORE_H_
#define _VC_CORE_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	IDLE,
	DEC_H264,
	DEC_JPEG,
	ENC_H264,
	ENC_JPEG,
	DEC_MJPEG,
	ENC_MJPEG,
} vc_type_t;


typedef enum {
	TOP_REG,
	PRE_REG,
	DEC_REG,
	ENC_REG,
	JPEG_REG,
	SCALE_PARM,
	CABAC_TABLE,
} vc_reg_type_t;


#define MAX_VC_TOP_REG_NUM    36
#define MAX_VC_PRE_REG_NUM    55
#define MAX_VC_DEC_REG_NUM    61
#define MAX_VC_ENC_REG_NUM    61
#define MAX_VC_JPEG_REG_NUM   61
#define MAX_SCALE_PARM 32
#define MAX_VC_H264_DEC_CABAC_TABLE 96
#define MAX_VC_H264_ENC_CABAC_TABLE 96
#define MAX_VC_JPEG_DEC_CABAC_TABLE 215
#define MAX_VC_JPEG_ENC_CABAC_TABLE 128
#define MAX_VC_CABAC_TABLE 256

typedef enum VC_STATUS {
	VC_STATUS_IDLE,
	VC_STATUS_READY_TO_RUN,
	VC_STATUS_RUNING,
	VC_ENC_STATUS_TIMEOUT,
	VC_ENC_STATUS_STREAM_FULL,
	VC_ENC_STATUS_PREVIEW_FINISH,
	VC_ENC_STATUS_FINISH_JPEG,
	VC_ENC_STATUS_FINISH_H264,
	VC_ENC_STATUS_ERROR,
	VC_DEC_STATUS_TIMEOUT,
	VC_DEC_STATUS_EMPTY,
	VC_DEC_STATUS_ERROR,
	VC_DEC_STATUS_MBLINE_FININSH_JPEG,
	VC_DEC_STATUS_FININSH_JPEG,
	VC_DEC_STATUS_FININSH_H264,
	VC_DEC_STATUS_SLICE_FININSH_H264,
	VC_STATUS_DEAD,
	VC_STATUS_UNKOWNERROR,
} vc_status_t;


typedef struct vc_handle {
	unsigned int (*readreg)(struct vc_handle *, int, int);

	int (*writereg)(struct vc_handle *, int, int, const unsigned int);

	int (*run)(struct vc_handle *);

	int (*query)(struct vc_handle *, vc_status_t *);

	int (*query_timeout)(struct vc_handle *, vc_status_t *);

	int (*reset)(struct vc_handle *);

} vc_handle_t;

vc_handle_t *vc_gethandle(int vc_type);

void vc_freehandle(struct vc_handle *);

void al_vc_core_init();
#ifdef __cplusplus
}
#endif

#endif

