#ifndef LOMBO_ENC_COMMON_H
#define LOMBO_ENC_COMMON_H

#define MAX_ENC_BLENDING_NUM 4

typedef enum {
	ENC_YUV420P = 0, /* for input mode */
	ENC_YVU420P,
	ENC_YUV420SP,
	ENC_YVU420SP,
	ENC_YUYV422,
	ENC_UYVY422,
	ENC_YVYU422,
	ENC_VYUY422,
	ENC_YUV422P,
	ENC_YVU422P,
	ENC_YUV422SP,
	ENC_YVU422SP,
	ENC_ARGB8888 = 100, /* for blending */
	ENC_RGBA8888,
	ENC_ABGR8888,
	ENC_BGRA8888,
} enc_input_mode_e;

typedef struct {
	unsigned int blending_enable;
	unsigned char *vir_addr;
	unsigned long phy_addr;
	unsigned int colorspace;
	unsigned int blending_x_pos;
	unsigned int blending_y_pos;
	unsigned int blending_width;
	unsigned int blending_height;
	unsigned int blending_stride;
} watermark_picture_t;


typedef struct {
	watermark_picture_t watermark_pictures[MAX_ENC_BLENDING_NUM];
	unsigned int watermark_picture_num;
} watermark_t;

#endif
