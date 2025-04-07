#ifndef LOMBO_JPEGENC_WATERMARK_H
#define LOMBO_JPEGENC_WATERMARK_H

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
	numeral_picture_t numeral_picture;
	unsigned int colorspace;
	unsigned int start_x_pos;
	unsigned int start_y_pos;
	unsigned int end_x_pos;
	unsigned int end_y_pos;
	unsigned int blending_area_index;
} numeral_output_t;

#endif
