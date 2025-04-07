#ifndef __IOSTREAM_PLUGIN_H__
#define __IOSTREAM_PLUGIN_H__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "av_media_type.h"

typedef enum iostream_type {
	STREAM_TYPE_FILE,
	STREAM_TYPE_EXTERNAL,
	STREAM_TYPE_EXTERNAL_RAW,
	STREAM_TYPE_MAX
} iostream_type_e;

typedef enum iostream_cmd {
	IOSTREAM_CMD_UNKNOWN,

	/* fill stream data, used for STREAM_TYPE_EXTERNAL. */
	IOSTREAM_CMD_FILL_DATA,

	/* used for STREAM_TYPE_EXTERNAL_RAW */
	IOSTREAM_CMD_FILL_PKT,
	IOSTREAM_CMD_GET_PKT,
	IOSTREAM_CMD_RETURN_PKT,
} iostream_cmd_e;

typedef struct stream_packet {
	uint8_t *data;
	size_t data_size; /* data size in buffer */
	int64_t timestamp; /* timestamp in us */
	av_media_type_e media_type;
} stream_packet_t;

typedef struct buffer_info {
	int size;
	int align;
} buffer_info_t;

typedef struct iostream_plugin {
	void *(*create)(void *param, iostream_type_e type);
	void (*destroy)(void *s);
	long (*ctrl)(void *s, int cmd, void *arg);
} iostream_plugin_t;

#endif /* __IOSTREAM_PLUGIN_H__ */

