
#ifndef __JPEG_DEC_INTERFACE_H__
#define __JPEG_DEC_INTERFACE_H__

#include "mod_media_i.h"

typedef struct {
	unsigned char *output_data;
	int offset_luma;
	int offset_chroma;
	int size_luma;
	int size_chroma;

	int output_w;
	int output_h;
	int real_h;
	int real_w;

	int jpeg_file;
} jpeg_param_t;

extern int jpeg_decode_start(char *jpeg_path, jpeg_param_t *jpg_param);

extern int jpeg_decode_end(jpeg_param_t *jpg_param);

#endif /* __JPEG_DEC_INTERFACE_H__ */
