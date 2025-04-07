#ifndef __H264_DECODER_API_H__
#define __H264_DECODER_API_H__

enum OUTPUT_MODE {
	VDC_YUV420P = 0,
	VDC_YVU420P,
	VDC_YUV420SP,
	VDC_YVU420SP,
	VDC_YUYV422,
	VDC_UYVY422,
	VDC_YVYU422,
	VDC_VYUY422,
	VDC_YUV422P,
	VDC_YVU422P,
	VDC_YUV422SP,
	VDC_YVU422SP,
};

enum H264DEC_BUF_STATUS {
	H264DEC_NO_FRAME = 0,
	H264DEC_REF,
	H264DEC_DISPLAY,
	H264DEC_ERROR,
};

enum H264DEC_ERR {
	H264DEC_NO_FRAME_BUF = 1000,
	H264DEC_PARSE_NAL_ERR,
	H264DEC_PARSE_SLICE_ERR,
	H264DEC_STREAM_EMPTY,
	H264DEC_STREAM_ERR,
	H264DEC_WAIT_TIMEOUT,
};

typedef struct dec_open_args {
	int ouput_type;
	int frame_buf_cnt;
} dec_open_args_t;

typedef struct fb_buffer {
	int index;
	int plane_cnt;
	int buf_flag;
	unsigned int   buffer_size[3];
	unsigned int   buf_fd[3];
	unsigned int   phy_addr[3];
	unsigned char *vir_addr[3];
	void *priv;
	struct fb_buffer *next;
} fb_buffer_t;

typedef struct frame_info {
	int width;
	int height;
	int buffer_width;
	int buffer_height;
} frame_info_t;

void *h264_dec_open(dec_open_args_t *args);

void h264_dec_close(void *handle);

int h264_dec_header(void *handle, unsigned char *header,
	int size, frame_info_t *info);

int h264_dec_packet(void *handle, unsigned char *packet, int size);

int h264_dec_get_info(void *handle, frame_info_t *info);

void h264_enqueue_empty_frame(void *handle, fb_buffer_t *frame);

void h264_dequeue_full_frame(void *handle, fb_buffer_t **frame);

void h264_dec_flush(void *handle);

int h264_dec_keyframe(void *handle, unsigned char *packet,
	int size, unsigned char *out_frame);

#endif /* __H264_DECODER_API_H__ */
