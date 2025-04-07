#include <stdio.h>
#include <string.h>
#include "media_photo.h"
#include "jpeg_dec_interface.h"
#include "media_display.h"

struct mp_param {
	jpeg_param_t jpg_param[2];
	int jpg_buf_index;
	unsigned int rotation;
};

typedef int (*pmedia_disp_show_photo)(struct media_disp_param *in_win_param,
		struct media_disp_param *out_win_param);

static struct mp_param *g_mp_param;
static const pmedia_disp_show_photo show_photo_func[MEDIA_PHOTO_SHOW_MAX] = {
							media_disp_win_left2right,
							media_disp_win_right2left,
							media_disp_win_show };

/*! \def SVPNG_PUT
 *brief Write a byte
 */
#ifndef SVPNG_PUT
#define SVPNG_PUT(u) fputc((int)(u), fp)
#endif

/*!
 *brief Save a RGB/RGBA image in PNG format.
 *param FILE *fp Output stream (by default using file descriptor).
 *param w Width of the image. (<16383)
 *param h Height of the image.
 *param img Image pixel data in 24-bit RGB or 32-bit RGBA format.
 *param alpha Whether the image contains alpha channel.
 */
static void svpng(FILE *fp, unsigned w, unsigned h, const unsigned char *img, int alpha)
{
	static const unsigned t[] = { 0, 0x1db71064, 0x3b6e20c8, 0x26d930ac, 0x76dc4190,
			0x6b6b51f4, 0x4db26158, 0x5005713c,
			/* CRC32 Table */0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
			0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c };
	/* ADLER-a, ADLER-b, CRC, pitch */
	const char *temp = "\x89PNG\r\n\32\n";
	unsigned a = 1, b = 0, c, p = w * (alpha ? 4 : 3) + 1, x, y, i;
#define SVPNG_U8A(ua, l)\
	for (i = 0; i < l; i++) {\
		SVPNG_PUT((ua)[i]);\
	}
#define SVPNG_U32(u)\
		do {\
			SVPNG_PUT((u) >> 24);\
			SVPNG_PUT(((u) >> 16) & 255);\
			SVPNG_PUT(((u) >> 8) & 255);\
			SVPNG_PUT((u) & 255);\
		} while (0)
#define SVPNG_U8C(u)\
	do {\
		SVPNG_PUT(u);\
		c ^= ((int)(u)); c = (c >> 4) ^ t[c & 15];\
		c = (c >> 4) ^ t[c & 15];\
	} while (0)
#define SVPNG_U8AC(ua, l)\
	for (i = 0; i < l; i++)\
		SVPNG_U8C((ua)[i])
#define SVPNG_U16LC(u)\
	do {\
		SVPNG_U8C((u) & 255);\
		SVPNG_U8C(((u) >> 8) & 255);\
	} while (0)
#define SVPNG_U32C(u)\
	do {\
		SVPNG_U8C((u) >> 24);\
		SVPNG_U8C(((u) >> 16) & 255);\
		SVPNG_U8C(((u) >> 8) & 255);\
		SVPNG_U8C((u) & 255);\
	} while (0)
#define SVPNG_U8ADLER(u)\
	do {\
		SVPNG_U8C(u); a = (a + (u)) % 65521; b = (b + a) % 65521;\
	} while (0)
#define SVPNG_BEGIN(s, l)\
	do {\
		SVPNG_U32(l);\
		c = ~0U; SVPNG_U8AC(s, 4);\
	} while (0)
#define SVPNG_END() SVPNG_U32(~c)
	SVPNG_U8A(temp, 8); /* Magic */
	SVPNG_BEGIN("IHDR", 13); /* IHDR chunk { */
	SVPNG_U32C(w);
	SVPNG_U32C(h); /*   Width & Height (8 bytes) */
	SVPNG_U8C(8);
	/* Depth=8, Color=True color with/without alpha (2 bytes) */
	SVPNG_U8C(alpha ? 6 : 2);
	/* Compression=Deflate, Filter=No, Interlace=No (3 bytes) */
	SVPNG_U8AC("\0\0\0", 3);
	SVPNG_END()
	; /* } */
	SVPNG_BEGIN("IDAT", 2 + h * (5 + p) + 4); /* IDAT chunk { */
	SVPNG_U8AC("\x78\1", 2); /* Deflate block begin (2 bytes) */
	for (y = 0; y < h; y++) { /* Each horizontal line makes a block for simplicity */
		SVPNG_U8C(y == h - 1); /* 1 for the last block, 0 for others (1 byte) */
		SVPNG_U16LC(p);
		/* Size of block in little endian and its 1's complement (4 bytes) */
		SVPNG_U16LC(~p);
		SVPNG_U8ADLER(0); /* No filter prefix (1 byte) */
		for (x = 0; x < p - 1; x++, img++) {
			if (x % 4 == 0)
				SVPNG_U8ADLER(*(img + 2));
			else if (x % 4 == 2)
				SVPNG_U8ADLER(*(img - 2));
			else
				SVPNG_U8ADLER(*img); /*   Image pixel data */
		}
	}
	SVPNG_U32C((b << 16) | a); /*   Deflate block end with adler (4 bytes) */
	SVPNG_END()
	; /* } */
	SVPNG_BEGIN("IEND", 0);
	SVPNG_END()
	; /* IEND chunk {} */
}

/**
 * media_photo_save_thumb - save to png for debug
 *
 * Returns 0 if there is no error; -1, return the error code.
 */
int media_photo_save_thumb(void)
{
	FILE *f1;
	int width, height, real_w, real_h, ret;
	u8 *out_addr;
	u8 *in_addr;

	MP_PARAM_CHECK

	f1 = fopen("/mnt/sdcard/thumb.png", "w+b");
	if (f1 == NULL) {
		MD_ERR("open fbp.argb fail");
		return -1;
	}

	if (g_mp_param->jpg_param[0].output_data) {
		in_addr = (u8 *) g_mp_param->jpg_param[0].output_data;
		width = g_mp_param->jpg_param[0].output_w;
		height = g_mp_param->jpg_param[0].output_h;
		real_w = g_mp_param->jpg_param[0].real_w;
		real_h = g_mp_param->jpg_param[0].real_h;
	} else if (g_mp_param->jpg_param[1].output_data) {
		in_addr = (u8 *) g_mp_param->jpg_param[1].output_data;
		width = g_mp_param->jpg_param[1].output_w;
		height = g_mp_param->jpg_param[1].output_h;
		real_w = g_mp_param->jpg_param[1].real_w;
		real_h = g_mp_param->jpg_param[1].real_h;
	} else {
		in_addr = NULL;
		width = 0;
		height = 0;
		real_w = 0;
		real_h = 0;
	}

	MD_LOG("start write png file,(%d,%d)", 320, 240);
	out_addr = (u8 *)rt_malloc_align(320*240*4, 32);
	ret = media_disp_scale_nv12_to_argb(in_addr, width, height, real_w, real_h,
						out_addr, 320, 240);

	if (ret == 0) {
		svpng(f1, 320, 240, (const unsigned char *)out_addr, 1);

		MD_LOG("save ok!");
	} else {
		MD_LOG("save fail !");
	}

	fclose(f1);

	rt_free_align(out_addr);

	return 0;
}

/**
 * media_photo_save_buffer - save to bin for debug
 *
 * Returns 0 if there is no error; -1, return the error code.
 */
int media_photo_save_buffer(void)
{
	FILE *fp_out;

	MP_PARAM_CHECK

	if (g_mp_param->jpg_param[0].output_data) {

		MD_LOG("start save buffer");

		fp_out = fopen("/mnt/sdcard/jpeg_out0.bin", "wb");
		if (fp_out == NULL) {
			MD_ERR("fopen fp_out error");
			return -1;
		}

		fwrite(g_mp_param->jpg_param[0].output_data, 1,
				g_mp_param->jpg_param[0].size_luma, fp_out);
		fwrite(
				g_mp_param->jpg_param[0].output_data
						+ g_mp_param->jpg_param[0].offset_chroma,
				1, g_mp_param->jpg_param[0].size_chroma, fp_out);
		fclose(fp_out);
		MD_LOG("save jpeg_out0.bin ok!(%d,%d)",
				g_mp_param->jpg_param[0].output_w,
				g_mp_param->jpg_param[0].output_h);
	}

	if (g_mp_param->jpg_param[1].output_data) {

		MD_LOG("start save buffer");

		fp_out = fopen("/mnt/sdcard/jpeg_out1.bin", "wb");
		if (fp_out == NULL) {
			MD_ERR("fopen fp_out error");
			return -1;
		}

		fwrite(g_mp_param->jpg_param[1].output_data, 1,
				g_mp_param->jpg_param[1].size_luma, fp_out);
		fwrite(
				g_mp_param->jpg_param[1].output_data
						+ g_mp_param->jpg_param[1].offset_chroma,
				1, g_mp_param->jpg_param[1].size_chroma, fp_out);
		fclose(fp_out);
		MD_LOG("save jpeg_out1.bin ok!(%d,%d)",
				g_mp_param->jpg_param[0].output_w,
				g_mp_param->jpg_param[0].output_h);
	}
	MD_LOG("end save buffer!");

	return 0;
}

/**
 * _media_photo_decode_start - start decode photo
 * @photo_path: file path.
 * @jpg_param: pointer to jpeg data pointer.
 *
 * Returns 0 if there is no error; -1, return the error code.
 */
static int _media_photo_decode_start(char *photo_path, jpeg_param_t *jpg_param)
{
	int ret;

	ret = jpeg_decode_start(photo_path, jpg_param);
	if (ret == 0 && g_mp_param->rotation != 0)
		media_disp_rotation_jpg(jpg_param, g_mp_param->rotation);

	return ret;
}

/**
 * _media_photo_decode_end - end decode photo
 * @jpg_param: pointer to jpeg data pointer.
 *
 * Returns 0 if there is no error; -1, return the error code.
 */
static int _media_photo_decode_end(jpeg_param_t *jpg_param)
{
	if (g_mp_param->rotation == 0)
		jpeg_decode_end(jpg_param);
	else
		media_disp_rotation_jpg_free(jpg_param);
	return 0;
}

/**
 * media_photo_show_start - start show photo
 * @param: pointer to show data pointer.
 *
 *
 * Returns 0 if there is no error; -1, return the error code.
 */
int media_photo_show_start(struct media_photo_show_start_param *param)
{
	int input_w, input_h, input_x, input_y;
	struct media_disp_win_info inf;
	int screen_w, screen_h;

	if (g_mp_param) {
		MD_ERR("g_mp_param != NULL");
		return -1;
	}

	if (media_disp_photo_init())
		return -1;

	media_disp_get_wh(&screen_w, &screen_h);

	/* media_disp_rot_test(); */

	g_mp_param = (struct mp_param *) malloc(sizeof(struct mp_param));
	if (g_mp_param) {
		memset(g_mp_param, 0, sizeof(struct mp_param));

		if (param) {
			g_mp_param->rotation = param->way;
			if ((MEDIA_PHOTO_ROTATION_90 == param->way) ||
				(MEDIA_PHOTO_ROTATION_270 == param->way)){
				input_w = param->win_w;
				input_h = param->win_h;
				input_x = param->win_offset_x;
				input_y = param->win_offset_y;

				inf.win_w = input_h;
				inf.win_h = input_w;
				inf.screen_offset_x = screen_w - input_y - input_h;
				inf.screen_offset_y = input_x;
				inf.mode = param->mode;
			} else {
				inf.win_w = param->win_w;
				inf.win_h = param->win_h;
				inf.screen_offset_x = param->win_offset_x;
				inf.screen_offset_y = param->win_offset_y;
				inf.mode = param->mode;
			}
		} else {
			g_mp_param->rotation = MEDIA_PHOTO_ROTATION_NONE;
			inf.win_w = 160;
			inf.win_h = 120;
			inf.screen_offset_x = 0;
			inf.screen_offset_y = 0;
			inf.mode = MEDIA_PHOTO_SHOW_CENTER;
		}

		media_disp_set_win_info(&inf);

		return 0;
	}
	return -1;
}

/**
 * media_photo_show_end - end show photo
 *
 * Returns 0 if there is no error; -1, return the error code.
 */
int media_photo_show_end(void)
{
	struct media_disp_param out_win_param = { 0 };
	int out_index = 0;

	MP_PARAM_CHECK

	for (out_index = 0; out_index < 2; out_index++)
		if (g_mp_param->jpg_param[out_index].output_data) {
			out_win_param.win_index = out_index;
			out_win_param.pixel_format = DISP_FORMAT_NV12;
			out_win_param.addr[0] =
					(u32)g_mp_param->jpg_param[out_index].output_data;
			out_win_param.addr[1] =
					(u32)g_mp_param->jpg_param[out_index].output_data
					+ g_mp_param->jpg_param[out_index].offset_chroma;
			out_win_param.addr[2] = 0;
			out_win_param.img_w = g_mp_param->jpg_param[out_index].output_w;
			out_win_param.img_h = g_mp_param->jpg_param[out_index].output_h;
			out_win_param.show_w = g_mp_param->jpg_param[out_index].real_w;
			out_win_param.show_h = g_mp_param->jpg_param[out_index].real_h;
			out_win_param.x_offset = 0;
			out_win_param.y_offset = 0;

			media_disp_win_hide(&out_win_param);

			_media_photo_decode_end(&g_mp_param->jpg_param[out_index]);
		}

	media_disp_photo_exit();

	free(g_mp_param);
	g_mp_param = NULL;

	return 0;
}

/**
 * media_photo_show - showing photo
 * @photo_path: pointer file path.
 * @mode: show mode.
 *
 * Returns 0 if there is no error; -1, return the error code.
 */
int media_photo_show(char *photo_path, media_photo_show_mode mode)
{
	int ret = -1, out_index;
	struct media_disp_param in_win_param = { 0 };
	struct media_disp_param out_win_param = { 0 };

	MP_PARAM_CHECK

	ret = _media_photo_decode_start(photo_path,
			&g_mp_param->jpg_param[g_mp_param->jpg_buf_index]);
	if (ret == 0) {
		in_win_param.win_index = g_mp_param->jpg_buf_index;
		in_win_param.pixel_format = DISP_FORMAT_NV12;
		in_win_param.addr[0] =
			(u32)g_mp_param->jpg_param[g_mp_param->jpg_buf_index].output_data;
		in_win_param.addr[1] =
			(u32)g_mp_param->jpg_param[g_mp_param->jpg_buf_index].output_data
			+ g_mp_param->jpg_param[g_mp_param->jpg_buf_index].offset_chroma;
		in_win_param.addr[2] = 0;
		in_win_param.img_w =
				g_mp_param->jpg_param[g_mp_param->jpg_buf_index].output_w;
		in_win_param.img_h =
				g_mp_param->jpg_param[g_mp_param->jpg_buf_index].output_h;
		in_win_param.show_w =
				g_mp_param->jpg_param[g_mp_param->jpg_buf_index].real_w;
		in_win_param.show_h =
				g_mp_param->jpg_param[g_mp_param->jpg_buf_index].real_h;
		in_win_param.x_offset = 0;
		in_win_param.y_offset = 0;

		out_index = (g_mp_param->jpg_buf_index == 0);
		if (g_mp_param->jpg_param[out_index].output_data) {
			out_win_param.win_index = out_index;
			out_win_param.pixel_format = DISP_FORMAT_NV12;
			out_win_param.addr[0] =
					(u32)g_mp_param->jpg_param[out_index].output_data;
			out_win_param.addr[1] =
					(u32) g_mp_param->jpg_param[out_index].output_data
					+ g_mp_param->jpg_param[out_index].offset_chroma;
			out_win_param.addr[2] = 0;
			out_win_param.img_w = g_mp_param->jpg_param[out_index].output_w;
			out_win_param.img_h = g_mp_param->jpg_param[out_index].output_h;
			out_win_param.show_w = g_mp_param->jpg_param[out_index].real_w;
			out_win_param.show_h = g_mp_param->jpg_param[out_index].real_h;
			out_win_param.x_offset = 0;
			out_win_param.y_offset = 0;

			show_photo_func[mode](&in_win_param, &out_win_param);

			_media_photo_decode_end(&g_mp_param->jpg_param[out_index]);
		} else {
			out_win_param.win_index = -1;
			show_photo_func[mode](&in_win_param, &out_win_param);
		}

		g_mp_param->jpg_buf_index = out_index;
	}

	return ret;
}

/**
 * media_photo_set_win_level - showing level
 * @level: number.
 *
 * Returns 0 if there is no error; -1, return the error code.
 */
int media_photo_set_win_level(int level)
{
	MP_PARAM_CHECK

	return media_disp_set_win_level(level);
}

/**
 * media_photo_get_thumb - get photo thumb
 * @param: pointer to thumb data pointer.
 *
 * Returns 0 if there is no error; -1, return the error code.
 */
int media_photo_get_thumb(struct media_photo_thumb_param *param)
{
	int ret;
	jpeg_param_t jpg_param;
#ifdef MEDIA_PRINT_INF_ON
	rt_tick_t rt_start;
	rt_start = rt_tick_get();
#endif
	if (param == NULL) {
		MD_ERR("param == NULL");
		return -1;
	}

	ret = jpeg_decode_start(param->file_path, &jpg_param);
#ifdef MEDIA_PRINT_INF_ON
	MD_LOG("jpeg_decode_start time = %d", rt_tick_get()-rt_start);
	rt_start = rt_tick_get();
#endif
	if (ret == 0) {
		if (param->way != 0)
			media_disp_rotation_jpg(&jpg_param, param->way);
#ifdef MEDIA_PRINT_INF_ON
		MD_LOG("media_disp_rotation_jpg time = %d", rt_tick_get()-rt_start);
		rt_start = rt_tick_get();
#endif
		media_disp_scale_nv12_to_argb(jpg_param.output_data, jpg_param.output_w,
						jpg_param.output_h, jpg_param.real_w,
						jpg_param.real_h, param->thumb_buf,
						param->thumb_w, param->thumb_h);
#ifdef MEDIA_PRINT_INF_ON
		MD_LOG("media_disp_scale_nv12_to_argb time = %d", rt_tick_get()-rt_start);
		rt_start = rt_tick_get();
#endif
		if (param->way == 0)
			jpeg_decode_end(&jpg_param);
		else
			media_disp_rotation_jpg_free(&jpg_param);
#ifdef MEDIA_PRINT_INF_ON
		MD_LOG("jpeg_decode_end time = %d", rt_tick_get()-rt_start);
		rt_start = rt_tick_get();
#endif
#ifdef MY_MEDIA_DEBUG_ON
		if (0) {
			FILE *f1;
			char new_p[256];

			strcpy(new_p, param->file_path);
			strcat(new_p, ".png");
			f1 = fopen(new_p, "w+b");

			if (f1 == NULL) {
				MD_ERR("open png fail");
				return -1;
			}

			svpng(f1, param->thumb_w, param->thumb_h,
				(const unsigned char *)param->thumb_buf, 1);

			MD_LOG("save ok!");

			fclose(f1);
		} else {
			struct media_disp_win_info inf;
			struct media_disp_param win_param = {0};
			static int d_flag = 1;

			win_param.win_index = 0;
			win_param.pixel_format = DISP_FORMAT_ARGB8888;
			win_param.img_w = param->thumb_w;
			win_param.img_h = param->thumb_h;
			win_param.show_w = param->thumb_w;
			win_param.show_h = param->thumb_h;
			win_param.x_offset = 0;
			win_param.y_offset = 0;
			win_param.addr[0] = (u32)param->thumb_buf;
			win_param.addr[1] = 0;
			win_param.addr[2] = 0;

			if (d_flag == 1) {
				d_flag = 0;
				inf.win_w = 160;
				inf.win_h = 120;
				inf.screen_offset_x = 50;
				inf.screen_offset_y = 200;
				inf.mode = MEDIA_PHOTO_SHOW_CENTER;
				media_disp_set_win_info(&inf);

				media_disp_photo_init();
			}

			media_disp_win(&win_param);

			/* media_disp_photo_exit(); */
		}
#endif
	}

	return ret;
}
