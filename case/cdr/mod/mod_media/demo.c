#include <app_manage.h>
#include <mod_manage.h>
#include <stdio.h>
#include <stdlib.h>
#include <rtthread.h>
#include "mod_media.h"

typedef char *thumb_img_buf_l;

static mod_t g_mod_media;

/* load media module */
int demo_mod_media_load(void)
{
	s32 ret = 0;

	memset(&g_mod_media, 0, sizeof(mod_t));
	ret = lb_mod_open(&g_mod_media, "/sdcard/mod_media.mod", 0);
	if (ret < 0) {
		printf("ret--:%d\n", ret);
		return ret;
	}

	return ret;
}

/* show photo first, once*/
int demo_mod_media_photo_show_start(void)
{
	s32 ret = 0;
	struct media_photo_show_start_param param;

	param.way = MEDIA_PHOTO_ROTATION_90;
	param.mode = MEDIA_PHOTO_SHOW_STRETCH;
	param.win_w = 300;
	param.win_h = 200;
	param.win_offset_x = 200;
	param.win_offset_y = 100;
	ret = lb_mod_ctrl(&g_mod_media, MOD_MEDIA_PHOTO_SHOW_START, 0, &param);
	printf("lb_mod_ctrl: %d\n", ret);

	return ret;
}

/* you can show your photos */
int demo_mod_media_photo_show(char *path)
{
	s32 ret = 0;

	ret = lb_mod_ctrl(&g_mod_media, MOD_MEDIA_PHOTO_SHOW_DIRECT, 0, path);
	printf("lb_mod_ctrl: %d\n", ret);

	return ret;
}

/* show photo last, once*/
int demo_mod_media_photo_show_end(void)
{
	s32 ret = 0;

	ret = lb_mod_ctrl(&g_mod_media, MOD_MEDIA_PHOTO_SHOW_END, 0, 0);
	printf("lb_mod_ctrl: %d\n", ret);

	return ret;
}

/* get thumb of photo at any time  */
int demo_mod_media_photo_get_thumb(void)
{
	s32 ret = 0;
	struct media_photo_thumb_param temp;

	strcpy(temp.file_path, "/mnt/sdcard/demo.jpg");
	temp.thumb_w = 320;
	temp.thumb_h = 240;
	/* ARGB8888 buffer */
	temp.thumb_buf = rt_malloc_align(temp.thumb_w * temp.thumb_h * 4 + 4, 32);
	temp.way = MEDIA_PHOTO_ROTATION_90;

	*((u32 *) temp.thumb_buf) = ((5 << 0) | (0 << 5) | (0 << 8) | (temp.thumb_w << 10)
			| (temp.thumb_h << 21));

	temp.thumb_buf = temp.thumb_buf + 4;
	ret = lb_mod_ctrl(&g_mod_media, MOD_MEDIA_PHOTO_GET_THUMB, 0, &temp);
	temp.thumb_buf = temp.thumb_buf - 4;
	printf("lb_mod_ctrl: %d\n", ret);

	/* todo */

	rt_free_align(temp.thumb_buf);

	return ret;
}

/* get thumb of photo at any time for lvgl
 * @file_path: image path
 * @thumb_w: user need thumb width
 * @thumb_h: user need thumb height
 *
 * success return lvgl image buffer, fail return NULL
 * */
thumb_img_buf_l app_mod_media_photo_get_thumb(char *file_path, int thumb_w, int thumb_h)
{
	s32 ret = 0;
	struct media_photo_thumb_param temp;

	strcpy(temp.file_path, file_path);
	temp.thumb_w = thumb_w;
	temp.thumb_h = thumb_h;
	/* ARGB8888 buffer */
	temp.thumb_buf = rt_malloc_align(temp.thumb_w * temp.thumb_h * 4 + 32, 32);
	temp.way = MEDIA_PHOTO_ROTATION_NONE;

	strcpy((char *)temp.thumb_buf, "thumb buffer");
	((u32 *) temp.thumb_buf)[7] = ((5 << 0) | (0 << 5) | (0 << 8)
			| (temp.thumb_w << 10) | (temp.thumb_h << 21));

	temp.thumb_buf = temp.thumb_buf + 32;
	ret = lb_mod_ctrl(&g_mod_media, MOD_MEDIA_PHOTO_GET_THUMB, 0, &temp);
	temp.thumb_buf = temp.thumb_buf - 4;
	printf("lb_mod_ctrl: %d\n", ret);

	return (thumb_img_buf_l)temp.thumb_buf;
}

/* free thumb of photo buffer
 * @thumb_buf: thumb buffer
 *
 * success return 0, fail return -1
 **/
int app_mod_media_photo_free_thumb(thumb_img_buf_l thumb_buf)
{
	if (thumb_buf) {
		if (strcmp(thumb_buf - 28, "thumb buffer") == 0) {
			rt_free_align(thumb_buf - 28);
			return 0;
		} else {
			printf("not thumb buffer\n");
			return -1;
		}
	} else
		return -1;
}

/* play video first, once*/
int demo_mod_media_video_start(void)
{
	s32 ret = 0;
	struct media_video_start_param param;

	param.way = MEDIA_VIDEO_ROTATION_90;
	param.mode = MEDIA_VIDEO_SHOW_STRETCH;
	param.win_w = 300;
	param.win_h = 200;
	param.win_offset_x = 200;
	param.win_offset_y = 100;
	ret = lb_mod_ctrl(&g_mod_media, MOD_MEDIA_VIDEO_START, 0, &param);
	printf("lb_mod_ctrl: %d\n", ret);

	return ret;
}

/* play video step 1 */
int demo_mod_media_video_set_path(char *video_path)
{
	s32 ret = 0;

	ret = lb_mod_ctrl(&g_mod_media, MOD_MEDIA_VIDEO_SET_FILE, 0, video_path);
	printf("lb_mod_ctrl: %d\n", ret);

	return ret;

}

/* play video step 2 */
int demo_mod_media_video_play(void)
{
	s32 ret = 0;

	ret = lb_mod_ctrl(&g_mod_media, MOD_MEDIA_VIDEO_PLAY, 0, 0);
	printf("lb_mod_ctrl: %d\n", ret);

	return ret;

}

/* play video last, once*/
int demo_mod_media_video_end(void)
{
	s32 ret = 0;

	ret = lb_mod_ctrl(&g_mod_media, MOD_MEDIA_VIDEO_END, 0, 0);
	printf("lb_mod_ctrl: %d\n", ret);

	return ret;
}

/* get thumb of video at any time for lvgl
 * @file_path: image path
 * @thumb_w: user need thumb width
 * @thumb_h: user need thumb height
 *
 * success return lvgl image buffer, fail return NULL
 * */
thumb_img_buf_l app_mod_media_video_get_thumb(char *file_path, int thumb_w, int thumb_h)
{
	s32 ret = 0;
	struct media_video_thumb_param temp;

	strcpy(temp.file_path, file_path);
	temp.thumb_w = thumb_w;
	temp.thumb_h = thumb_h;
	/* ARGB8888 buffer */
	temp.thumb_buf = rt_malloc_align(temp.thumb_w * temp.thumb_h * 4 + 32, 32);
	temp.way = MEDIA_VIDEO_ROTATION_NONE;

	strcpy((char *)temp.thumb_buf, "thumb buffer");
	((u32 *) temp.thumb_buf)[7] = ((5 << 0) | (0 << 5) | (0 << 8)
			| (temp.thumb_w << 10) | (temp.thumb_h << 21));

	temp.thumb_buf = temp.thumb_buf + 32;
	ret = lb_mod_ctrl(&g_mod_media, MOD_MEDIA_VIDEO_GET_THUMB, 0, &temp);
	temp.thumb_buf = temp.thumb_buf - 4;
	printf("lb_mod_ctrl: %d\n", ret);

	return (thumb_img_buf_l)temp.thumb_buf;
}

/* free thumb of video buffer
 * @thumb_buf: thumb buffer
 *
 * success return 0, fail return -1
 **/
int app_mod_media_video_free_thumb(thumb_img_buf_l thumb_buf)
{
	if (thumb_buf) {
		if (strcmp(thumb_buf - 28, "thumb buffer") == 0) {
			rt_free_align(thumb_buf - 28);
			return 0;
		} else {
			printf("not thumb buffer\n");
			return -1;
		}
	} else
		return -1;
}

/* unload media module */
int demo_mod_media_unload(void)
{
	s32 ret = 0;

	ret = lb_mod_close(&g_mod_media);
	printf("lb_mod_close: %d\n", ret);

	return ret;
}
