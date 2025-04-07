#ifndef __MEDIA_VIDEO_H__
#define __MEDIA_VIDEO_H__

#include "mod_media_i.h"

#define MV_PARAM_CHECK \
	do {\
		if (!(g_mv_param)) {\
			MD_ERR("g_mv_param == NULL");\
			return -1;\
		} \
	} while (0);

extern int media_video_start(struct media_video_start_param *param);
extern int media_video_set_file(char *path);
extern int media_video_play(void);
extern int media_video_pause(void);
extern int media_video_stop(void);
extern int media_video_set_cur_pos(int second);
extern int media_video_get_end_pos(void);
extern int media_video_get_cur_pos(void);
extern int media_video_get_state(void);
extern int media_video_set_win_level(int level);
extern int media_video_get_thumb(struct media_video_thumb_param *param);
extern int media_video_end(void);
extern int media_video_set_win_layer(int aux);

#endif /* __MEDIA_VIDEO_H__ */
