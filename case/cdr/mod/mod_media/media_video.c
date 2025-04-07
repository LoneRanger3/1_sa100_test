#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "media_display.h"
#include "media_video.h"

#include "omx_mediaplayer.h"
#include "media_extractor.h"

struct mv_param {
	mp_callback_ops_t cb_ops;
	omxmp_win_t win;
	void *omxplayer;

	enum media_video_rotation rotation;
	enum media_video_show_mode mode;
	enum media_video_state state;
};

static struct mv_param *g_mv_param;

static void _media_video_completion(void *handle)
{
	MD_LOG("------on_completion--------");
}
static void _media_video_error(void *handle, int omx_err)
{
	MD_ERR("------on_error %d--------", omx_err);
}
static void _media_video_prepared(void *handle)
{
	MD_LOG("------on_prepared--------");
}
static void _media_video_seek_complete(void *handle)
{
	MD_LOG("------on_seek_complete--------");
}
static void _media_video_video_size_changed(void *handle, int width, int height)
{
	MD_LOG("on_video_size_changed width=%d, height=%d", width, height);
}
static void _media_video_timed_text(void *handle, char *text)
{
	MD_LOG("------on_timed_text %s--------", text);
}

/**
 * media_video_start - start video work
 * @param: pointer to start data pointer.
 *
 * Returns 0 if there is no error; -1, return the error code.
 */
int media_video_start(struct media_video_start_param *param)
{
	int ret;
	int input_w, input_h, input_x, input_y;
	int screen_w, screen_h;

	if (g_mv_param) {
		MD_ERR("g_mv_param != NULL");
		return -1;
	}

	media_disp_get_wh(&screen_w, &screen_h);

	g_mv_param = (struct mv_param *) malloc(sizeof(struct mv_param));
	if (g_mv_param) {
		memset(g_mv_param, 0, sizeof(struct mv_param));

		if (param) {
			g_mv_param->rotation = param->way;
			if (MEDIA_VIDEO_ROTATION_90 == param->way) {
				input_w = param->win_w;
				input_h = param->win_h;
				input_x = param->win_offset_x;
				input_y = param->win_offset_y;

				g_mv_param->win.width = input_h;
				g_mv_param->win.height = input_w;
				g_mv_param->win.left = screen_w - input_y - input_h;
				g_mv_param->win.top = input_x;
				g_mv_param->mode = param->mode;
			} else if (MEDIA_VIDEO_ROTATION_270 == param->way) {
				input_w = param->win_w;
				input_h = param->win_h;
				input_x = param->win_offset_x;
				input_y = param->win_offset_y;

				g_mv_param->win.width = input_h;
				g_mv_param->win.height = input_w;
				g_mv_param->win.left = screen_w - input_y - input_h;
				g_mv_param->win.top = input_x;
				g_mv_param->mode = param->mode;

			} else {
				g_mv_param->win.width = param->win_w;
				g_mv_param->win.height = param->win_h;
				g_mv_param->win.left = param->win_offset_x;
				g_mv_param->win.top = param->win_offset_y;
				g_mv_param->mode = param->mode;
			}
		} else {
			g_mv_param->rotation = MEDIA_VIDEO_ROTATION_NONE;
			g_mv_param->win.width = 0;
			g_mv_param->win.height = 0;
			g_mv_param->win.left = 0;
			g_mv_param->win.top = 0;
			g_mv_param->mode = MEDIA_VIDEO_SHOW_CENTER;
		}

		g_mv_param->cb_ops.on_completion = _media_video_completion;
		g_mv_param->cb_ops.on_error = _media_video_error;
		g_mv_param->cb_ops.on_prepared = _media_video_prepared;
		g_mv_param->cb_ops.on_seek_complete = _media_video_seek_complete;
		g_mv_param->cb_ops.on_video_size_changed =
				_media_video_video_size_changed;
		g_mv_param->cb_ops.on_timed_text = _media_video_timed_text;

		g_mv_param->omxplayer = omxmp_create(&g_mv_param->cb_ops);
		if (g_mv_param->omxplayer == NULL) {
			MD_ERR("media video create omxplayer error!");
			free(g_mv_param);
			g_mv_param = NULL;
			return -1;
		}
		MD_LOG("win(%d,%d,%d,%d)", g_mv_param->win.left, g_mv_param->win.top,
				g_mv_param->win.width, g_mv_param->win.height);
		/* set win */
		ret = omxmp_set_window(g_mv_param->omxplayer, &g_mv_param->win);
		if (ret) {
			MD_ERR("media video set win error %d", ret);
			free(g_mv_param);
			g_mv_param = NULL;
			return -1;
		}
		/* set scaling mode */
		ret = omxmp_set_scaling_mode(g_mv_param->omxplayer, g_mv_param->mode);
		if (ret) {
			MD_ERR("media video set scale mode error %d", ret);
			free(g_mv_param);
			g_mv_param = NULL;
			return -1;
		}
		/* set rotate mode */
		ret = omxmp_set_rotation(g_mv_param->omxplayer, g_mv_param->rotation);
		if (ret) {
			MD_ERR("media video set rotate mode error %d", ret);
			free(g_mv_param);
			g_mv_param = NULL;
			return -1;
		}
	}

	return 0;
}

/**
 * media_video_end - end video work
 *
 * Returns 0 if there is no error; -1, return the error code.
 */
int media_video_end(void)
{
	MV_PARAM_CHECK

	media_video_stop();

	omxmp_release(g_mv_param->omxplayer);

	free(g_mv_param);
	g_mv_param = NULL;

	return 0;
}

/**
 * media_video_set_file - set video file
 * @path: file path.
 *
 * Returns 0 if there is no error; -1, return the error code.
 */
int media_video_set_file(char *path)
{
	int ret;

	MV_PARAM_CHECK

	media_video_stop();

	ret = omxmp_set_data_source(g_mv_param->omxplayer, path);
	if (ret) {
		MD_ERR("omxmp_set_data_source error");
		return -1;
	}

	ret = omxmp_prepare(g_mv_param->omxplayer);
	if (ret) {
		MD_ERR("omxmp_prepare fail");
		return -1;
	}

	return ret;
}

/**
 * media_video_play - play video
 *
 * Returns 0 if there is no error; -1, return the error code.
 */
int media_video_play(void)
{
	int ret;

	MV_PARAM_CHECK

	ret = omxmp_start(g_mv_param->omxplayer);
	if (ret == 0) {
		g_mv_param->state = MEDIA_VIDEO_STATE_STARTED;
		return 0;
	} else {
		MD_ERR("omxmp_start fail");
		return -1;
	}
}

/**
 * media_video_pause - pause video
 *
 * Returns 0 if there is no error; -1, return the error code.
 */
int media_video_pause(void)
{
	int ret;

	MV_PARAM_CHECK

	ret = omxmp_pause(g_mv_param->omxplayer);
	if (ret == 0) {
		g_mv_param->state = MEDIA_VIDEO_STATE_PAUSED;
		return 0;
	} else {
		MD_ERR("omxmp_pause fail");
		return -1;
	}
}

/**
 * media_video_stop - stop video
 *
 * Returns 0 if there is no error; -1, return the error code.
 */
int media_video_stop(void)
{
	MV_PARAM_CHECK

	omxmp_stop(g_mv_param->omxplayer);
	omxmp_reset(g_mv_param->omxplayer);

	return 0;
}

/**
 * media_video_set_cur_pos - set video play time
 * @second: set video play time
 *
 * Returns 0 if there is no error; -1, return the error code.
 */
int media_video_set_cur_pos(int second)
{
	int ret;

	MV_PARAM_CHECK

	ret = omxmp_seek_to(g_mv_param->omxplayer, (long)(second*1000));
	if (ret == -1) {
		MD_ERR("omxmp_seek_to fail, %d second", second);
		return -1;
	}

	return ret;
}

/**
 * media_video_get_end_pos - get video play end time
 *
 * Returns >0 if there is no error; -1, return the error code.
 */
int media_video_get_end_pos(void)
{
	long ret;

	MV_PARAM_CHECK

	ret = omxmp_get_duration(g_mv_param->omxplayer);
	if (ret == -1) {
		MD_ERR("media_video_get_end_pos fail");
		return -1;
	}

	return (ret+500)/1000;
}

/**
 * media_video_get_end_pos - get video play time
 * @param: pointer to thumb data pointer.
 *
 * Returns >0 if there is no error; -1, return the error code.
 */
int media_video_get_cur_pos(void)
{
	long ret;

	MV_PARAM_CHECK

	ret = omxmp_get_current_position(g_mv_param->omxplayer);
	if (ret == -1) {
		MD_ERR("omxmp_get_current_position fail");
		return -1;
	}

	return (ret + 500) / 1000;
}

/**
 * media_video_get_state - get video state
 *
 * Returns 0 if there is no error; -1, return the error code.
 */
int media_video_get_state(void)
{
	int ret;

	MV_PARAM_CHECK

	ret = omxmp_get_state(g_mv_param->omxplayer);

	return ret;
}

/**
 * media_photo_get_thumb - set video window level
 * @level: number.
 *
 * Returns 0 if there is no error; -1, return the error code.
 */
int media_video_set_win_level(int level)
{
	int ret = -1;

	MV_PARAM_CHECK

	ret = omxmp_set_win_layer(g_mv_param->omxplayer, level);
	if (ret == -1) {
		MD_ERR("omxmp_set_win_layer fail, %d", level);
		return -1;
	}

	return ret;

}

/**
 * media_video_get_thumb - get vide thumb
 * @param: pointer to thumb data pointer.
 *
 * Returns 0 if there is no error; -1, return the error code.
 */
int media_video_get_thumb(struct media_video_thumb_param *param)
{
	int ret;
	void *extractor = NULL;
	media_format_t *mformat = NULL;
	unsigned char *buffer;
	int buffer_size;
	jpeg_param_t jpg_param;
#ifdef MEDIA_PRINT_INF_ON
	rt_tick_t rt_start;
	rt_start = rt_tick_get();
#endif

	if (param == NULL) {
		MD_ERR("param == NULL");
		return -1;
	}

	extractor = mextractor_open(param->file_path);
	if (extractor == NULL) {
		MD_ERR("open mextractor error");
		return -1;
	}
#ifdef MEDIA_PRINT_INF_ON
	MD_LOG("mextractor_open time = %d", rt_tick_get()-rt_start);
	rt_start = rt_tick_get();
#endif

	mformat = mextractor_get_format(extractor);
	if (mformat == NULL) {
		MD_ERR("open mformat error");
		mextractor_close(extractor);
		return -1;
	}
#ifdef MEDIA_PRINT_INF_ON
	MD_LOG("mextractor_get_format time = %d", rt_tick_get()-rt_start);
	rt_start = rt_tick_get();
#endif

	buffer_size = mformat->width * mformat->height * 3 / 2;
	buffer = rt_malloc_align(buffer_size, 32);
	if (!buffer) {
		MD_ERR("malloc buffer error\n");
		mextractor_close(extractor);
		return -1;
	}

	ret = mextractor_get_thumbnail(extractor, buffer);
	if (ret == -1) {
		MD_ERR("mextractor_get_thumbnail error");
		rt_free_align(buffer);
		mextractor_close(extractor);
		return -1;
	}
#ifdef MEDIA_PRINT_INF_ON
	MD_LOG("mextractor_get_thumbnail time = %d", rt_tick_get()-rt_start);
	rt_start = rt_tick_get();
#endif

	jpg_param.output_data = buffer;
	jpg_param.offset_luma = 0;
	jpg_param.offset_chroma = mformat->width * mformat->height;
	jpg_param.size_luma = mformat->width * mformat->height;
	jpg_param.size_chroma = mformat->width * mformat->height / 2;
	jpg_param.output_w = mformat->width;
	jpg_param.output_h = mformat->height;
	jpg_param.real_h = mformat->width;
	jpg_param.real_w = mformat->height;
	jpg_param.jpeg_file = 0;

	if (param->way != 0)
		media_disp_rotation_jpg(&jpg_param, param->way);

	media_disp_scale_nv12_to_argb(jpg_param.output_data, jpg_param.output_w,
					jpg_param.output_h, jpg_param.real_w,
					jpg_param.real_h, param->thumb_buf,
					param->thumb_w, param->thumb_h);

	rt_free_align((void *)jpg_param.output_data);
	mextractor_close(extractor);

#ifdef MY_MEDIA_DEBUG_ON
		if (1) {
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
	return 0;
}

