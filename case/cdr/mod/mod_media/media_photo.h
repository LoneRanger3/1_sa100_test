#ifndef __MEDIA_PHOTO_H__
#define __MEDIA_PHOTO_H__

#include "mod_media_i.h"

#define MP_PARAM_CHECK \
	do {\
		if (!(g_mp_param)) {\
			MD_ERR("g_mp_param == NULL");\
			return -1;\
		} \
	} while (0);

typedef enum {
	MEDIA_PHOTO_SHOW_RIGHT_LEFT = 0,
	MEDIA_PHOTO_SHOW_LEFT_RIGHT,
	MEDIA_PHOTO_SHOW_DIRECT,
	MEDIA_PHOTO_SHOW_MAX
} media_photo_show_mode;

extern int media_photo_show_start(struct media_photo_show_start_param *param);
extern int media_photo_save_thumb(void);
extern int media_photo_save_buffer(void);
extern int media_photo_show(char *photo_path, media_photo_show_mode mode);
extern int media_photo_set_win_level(int level);
extern int media_photo_show_end(void);
extern int media_photo_get_thumb(struct media_photo_thumb_param *param);

#endif /* __MEDIA_PHOTO_H__ */
