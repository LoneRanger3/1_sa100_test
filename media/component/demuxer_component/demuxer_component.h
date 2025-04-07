#ifndef __DEMUXER_COMPOMEMT_H__
#define __DEMUXER_COMPOMEMT_H__

#include "base_component.h"
#include "demuxer/demuxer.h"

#define AUDIO_PORT 0
#define VIDEO_PORT 1
#define OTHER_PORT 2
#define SUBTITLE_PORT 2
#define MAX_FILE_SIZE 512

typedef struct demuxer_component_private {
	int role_type;
	char src_file_name[MAX_FILE_SIZE];
	void *handle;
	void *lib_handle;
	media_info_t medie_info;
	int lib_init_flag;
	int is_demuxering;
	pthread_mutex_t demuxer_mutex;
	OMX_TICKS video_time;
	OMX_TICKS audio_time;
	int video_extra;
	int audio_extra;
	int flame_flag;
	int seekflag;
	OMX_TICKS nTimestamp;
} demuxer_component_private_t;

OMX_ERRORTYPE demuxer_component_init(lb_omx_component_t *cmp_handle);

#endif
