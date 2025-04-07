/*
* copyright (c) 2018 All Rights Reserved
*
* This file is part of media.
*
* File   : media_probe.c
* Version: V1.0
* Date   : 2019/8/7 10:21:02
* Other  : ffmpeg-3.2.7
*/

#define DBG_LEVEL         DBG_WARNING
#define LOG_TAG           "media_probe"

#include <string.h>
#include <strings.h>
#include <oscl.h>
#include <base_component.h>
#include <av_media_type.h>
#include "media_probe.h"

#define MIN_PROBE_SIZE (2048)
#define MAX_PROBE_SIZE (1 << 20)
/* < maximum score */
#define AVPROBE_SCORE_MAX       100
#define AVPROBE_SCORE_EXTENSION  50 /* < score for file extension */
#define AVPROBE_SCORE_RETRY (AVPROBE_SCORE_MAX/4)

#ifndef AV_RB32
#   define AV_RB32(x)                                \
	(((uint32_t)((const uint8_t *)(x))[0] << 24) | \
	(((const uint8_t *)(x))[1] << 16) | \
	(((const uint8_t *)(x))[2] << 8) | \
	((const uint8_t *)(x))[3])
#endif

#ifndef AV_RL32
#   define AV_RL32(x)                                \
	(((uint32_t)((const uint8_t *)(x))[3] << 24) | \
	(((const uint8_t *)(x))[2] << 16) | \
	(((const uint8_t *)(x))[1] << 8) | \
	((const uint8_t *)(x))[0])
#endif

#ifndef AV_RB64
#   define AV_RB64(x)                                   \
	(((uint64_t)((const uint8_t *)(x))[0] << 56) | \
	((uint64_t)((const uint8_t *)(x))[1] << 48) | \
	((uint64_t)((const uint8_t *)(x))[2] << 40) | \
	((uint64_t)((const uint8_t *)(x))[3] << 32) | \
	((uint64_t)((const uint8_t *)(x))[4] << 24) | \
	((uint64_t)((const uint8_t *)(x))[5] << 16) | \
	((uint64_t)((const uint8_t *)(x))[6] << 8) | \
	(uint64_t)((const uint8_t *)(x))[7])
#endif

#define MKTAG(a, b, c, d) ((a) | ((b) << 8) | ((c) << 16) | ((unsigned int)(d) << 24))
#define FFMIN(a, b) ((a) > (b) ? (b) : (a))
#define FFMAX(a, b) ((a) > (b) ? (a) : (b))
#define FFMAX3(a, b, c) FFMAX(FFMAX(a, b), c)

#ifndef AV_RB16
#define AV_RB16(x) ((((const uint8_t *)(x))[0] << 8) | ((const uint8_t *)(x))[1])
#endif

typedef struct av_probe {
	char *suffix;
	char *name;
	av_muxer_type_e id;
	int (*probe)(uint8_t *data, int size);
} av_probe_t;

/*
********************************************************************
* audio probe
********************************************************************
*/
static int wav_probe(uint8_t *data, int size)
{
	/* check file header */
	if (size <= 32)
		return 0;
	if (!memcmp(data + 8, "WAVE", 4)) {
		if (!memcmp(data, "RIFF", 4) || !memcmp(data, "RIFX", 4))
			/* Since the ACT demuxer has a standard WAV header at the top of
			* its own, the returned score is decreased to avoid a probe
			* conflict between ACT and WAV. */
			return 100 - 1;
		else if (!memcmp(data, "RF64", 4) &&
			!memcmp(data + 12, "ds64", 4))
			return 100;
	}
	return 0;
}

/*
********************************************************************
* video probe
********************************************************************
*/
static int mov_probe(uint8_t *data, int size)
{
	int64_t offset;
	uint32_t tag;
	int score = 0;
	int moov_offset = -1;

	/* check file header */
	offset = 0;
	for (;;) {
		/* ignore invalid offset */
		if ((offset + 8) > size)
			break;
		tag = AV_RL32(data + offset + 4);
		switch (tag) {
		/* check for obvious tags */
		case MKTAG('m', 'o', 'o', 'v'):
			moov_offset = offset + 4;
		case MKTAG('m', 'd', 'a', 't'):
		/* detect movs with preview pics like ew.mov and april.mov */
		case MKTAG('p', 'n', 'o', 't'):
		/* Packet Video PVAuthor adds this and a lot of more junk */
		case MKTAG('u', 'd', 't', 'a'):
		case MKTAG('f', 't', 'y', 'p'):
			if (AV_RB32(data + offset) < 8 &&
				(AV_RB32(data + offset) != 1 ||
					offset + 12 > size ||
					AV_RB64(data + offset + 8) == 0)) {
				score = FFMAX(score, AVPROBE_SCORE_EXTENSION);
			} else if (tag == MKTAG('f', 't', 'y', 'p') &&
				(AV_RL32(data + offset + 8) ==
					MKTAG('j', 'p', '2', ' ')
					|| AV_RL32(data + offset + 8) ==
						MKTAG('j', 'p', 'x', ' ')
				)) {
				score = FFMAX(score, 5);
			} else {
				score = AVPROBE_SCORE_MAX;
			}
			offset = FFMAX(4, AV_RB32(data + offset)) + offset;
			break;
		/* those are more common words, so rate then a bit less */
		case MKTAG('e', 'd', 'i', 'w'): /* xdcam files have reverted first tags */
		case MKTAG('w', 'i', 'd', 'e'):
		case MKTAG('f', 'r', 'e', 'e'):
		case MKTAG('j', 'u', 'n', 'k'):
		case MKTAG('p', 'i', 'c', 't'):
			score = FFMAX(score, AVPROBE_SCORE_MAX - 5);
			offset = FFMAX(4, AV_RB32(data + offset)) + offset;
			break;
		case MKTAG(0x82, 0x82, 0x7f, 0x7d):
		case MKTAG('s', 'k', 'i', 'p'):
		case MKTAG('u', 'u', 'i', 'd'):
		case MKTAG('p', 'r', 'f', 'l'):
			/* if we only find those cause probedata
			is too small at least rate them */
			score = FFMAX(score, AVPROBE_SCORE_EXTENSION);
			offset = FFMAX(4, AV_RB32(data + offset)) + offset;
			break;
		default:
			offset = FFMAX(4, AV_RB32(data + offset)) + offset;
		}
	}
	if (score > AVPROBE_SCORE_MAX - 50 && moov_offset != -1) {
		/* moov atom in the header - we should make sure that this is not a
		 * MOV-packed MPEG-PS */
		offset = moov_offset;

		while (offset < (size - 16)) { /* Sufficient space */
			/* We found an actual hdlr atom */
			if (AV_RL32(data + offset) == MKTAG('h', 'd', 'l', 'r') &&
				AV_RL32(data + offset + 8) ==
					MKTAG('m', 'h', 'l', 'r') &&
				AV_RL32(data + offset + 12) ==
					MKTAG('M', 'P', 'E', 'G')) {
				/* printf("Found media data tag MPEG indicating
				this is a MOV-packed MPEG-PS.\n"); */
				/* We found a media handler reference atom describing an
				 * MPEG-PS-in-MOV, return a
				 * low score to force expanding the probe window until
				 * mpegps_probe finds what it needs */
				return 5;
			} else
				/* Keep looking */
				offset += 2;
		}
	}

	return score;
}


#define TS_FEC_PACKET_SIZE 204
#define TS_DVHS_PACKET_SIZE 192
#define TS_PACKET_SIZE 188
#define TS_MAX_PACKET_SIZE 204
static int analyze(const uint8_t *buf, int size, int packet_size, int probe)
{
	int stat[TS_MAX_PACKET_SIZE];
	int stat_all = 0;
	int i;
	int best_score = 0;

	memset(stat, 0, packet_size * sizeof(*stat));

	for (i = 0; i < size - 3; i++) {
		if (buf[i] == 0x47) {
			int pid = AV_RB16(buf+1) & 0x1FFF;
			int asc = buf[i + 3] & 0x30;
			if (!probe || pid == 0x1FFF || asc) {
				int x = i % packet_size;
				stat[x]++;
				stat_all++;
				if (stat[x] > best_score)
					best_score = stat[x];
			}
		}
	}

	return best_score - FFMAX(stat_all - 10*best_score, 0)/10;
}

static int mpegts_probe(uint8_t *data, int datasize)
{
	const int size = datasize;
	int maxscore = 0;
	int sumscore = 0;
	int i;
	int check_count = size / TS_FEC_PACKET_SIZE;
#define CHECK_COUNT 10
#define CHECK_BLOCK 100

	if (!check_count)
		return 0;

	for (i = 0; i < check_count; i += CHECK_BLOCK) {
		int left = FFMIN(check_count - i, CHECK_BLOCK);
		int score      = analyze(data + TS_PACKET_SIZE * i,
			TS_PACKET_SIZE * left, TS_PACKET_SIZE, 1);
		int dvhs_score = analyze(data + TS_DVHS_PACKET_SIZE * i,
			TS_DVHS_PACKET_SIZE * left, TS_DVHS_PACKET_SIZE, 1);
		int fec_score  = analyze(data + TS_FEC_PACKET_SIZE * i,
			TS_FEC_PACKET_SIZE * left, TS_FEC_PACKET_SIZE , 1);
		score = FFMAX3(score, dvhs_score, fec_score);
		sumscore += score;
		maxscore = FFMAX(maxscore, score);
	}

	sumscore = sumscore * CHECK_COUNT / check_count;
	maxscore = maxscore * CHECK_COUNT / CHECK_BLOCK;

	if (check_count > CHECK_COUNT && sumscore > 6)
		return AVPROBE_SCORE_MAX   + sumscore - CHECK_COUNT;
	else if (check_count >= CHECK_COUNT && sumscore > 6)
		return AVPROBE_SCORE_MAX/2 + sumscore - CHECK_COUNT;
	else if (check_count >= CHECK_COUNT && maxscore > 6)
		return AVPROBE_SCORE_MAX/2 + sumscore - CHECK_COUNT;
	else if (sumscore > 6)
		return 2;
	else
		return 0;
}

static const  av_probe_t demuxer_uri[] = {
	/* video probe */
	{"mov,mp4,m4a,3gp", "mov", AV_MUXER_TYPE_MOV, mov_probe},

	{"ts,m2ts", "ts", AV_MUXER_TYPE_TS, mpegts_probe},

	/* audio probe */
	{"wav", "wav", AV_MUXER_TYPE_WAV, wav_probe},

	{NULL, NULL, AV_MUXER_TYPE_INVALID, NULL}
};

char *media_probe(char *file_name)
{
	char *suffix = NULL;
	uint8_t *buf = NULL;
	char *name  = NULL;
	int data_size;
	int id3len = 0;
	int fd = -1;
	int i = 0;
	int found = 0;
	int probe_size = MIN_PROBE_SIZE;

	fd = open(file_name, O_RDONLY);
	if (fd == -1) {
		OSCL_LOGE("open file error:%s!", file_name);
		return NULL;
	}

	buf = oscl_malloc(probe_size);
	if (buf == NULL)
		goto EXIT;

	data_size = read(fd, buf, probe_size);
	if (data_size  == -1)
		goto EXIT;

	while ((buf[0] == 0x49
		&& buf[1] == 0x44
		&& buf[2] == 0x33)
		&& data_size == probe_size) {
		id3len += (((int)buf[6] & 0x7f) << 21) |
			(((int)buf[7] & 0x7f) << 14) |
			(((int)buf[8] & 0x7f) << 7) |
			((int)buf[9] & 0x7f);
		id3len += 10;
		lseek(fd, id3len, SEEK_SET);
		OSCL_LOGI("id3len=%d", id3len);
		data_size = read(fd, buf, probe_size);
	}

	suffix = strrchr(file_name, '.');
	if (suffix != NULL) {
		int len;
		suffix++;
		len = strlen(suffix);
		i = 0;
		while (demuxer_uri[i].suffix != NULL) {
			char *suf = demuxer_uri[i].suffix;
			while (suf != NULL) {
				if (!strncasecmp(suf, suffix, len) &&
					(suf[len] == ',' || suf[len] == '\0')) {
					found = 1;
					break;
				}
				suf = strchr(suf, ',');
				if (suf != NULL)
					suf++;
			}
			if (found)
				break;
			i++;
		}
	}

	if (found) {
		if (demuxer_uri[i].probe(buf, data_size) > AVPROBE_SCORE_RETRY) {
			name = demuxer_uri[i].name;
			OSCL_LOGI("file:%s, type:%s, index:%d", file_name, name, i);
			goto EXIT;
		} else
			OSCL_LOGI("file:%s suffix %s probe failed!", file_name, suffix);
	} else
		OSCL_LOGI("suffix %s not support", suffix);

	int buf_ofs = data_size;
	int max_score = 0, idx = -1;
	while (1) {
		int score = 0;
		max_score = 0;
		idx = -1;
		for (i = 0; demuxer_uri[i].probe != NULL; i++) {
			score = demuxer_uri[i].probe(buf, data_size);
			if (score > max_score) {
				max_score = score;
				idx = i;
			}
		}
		OSCL_LOGD("probe_size=%d, max_score=%d, index=%d",
			probe_size, max_score, idx);
		if (max_score > AVPROBE_SCORE_RETRY)
			break;

		probe_size = probe_size << 1;
		if (probe_size > MAX_PROBE_SIZE)
			break;
		buf = oscl_realloc(buf, probe_size);
		if (buf == NULL) {
			OSCL_LOGE("realloc buf error!");
			break;
		}
		data_size = read(fd, buf + buf_ofs, probe_size - buf_ofs);
		if (data_size <= 0) {
			OSCL_LOGE("read error or reach eof(ret=%d)!", data_size);
			break;
		}
		buf_ofs += data_size;
	}

	if (max_score > 0 && idx >= 0) {
		name = demuxer_uri[idx].name;
		OSCL_LOGI("file:%s, max_score:%d, index:%d", file_name, max_score, idx);
		if (max_score <= AVPROBE_SCORE_RETRY) {
			OSCL_LOGW("file %s format %s detected only with low score of %d",
				file_name, name, max_score);
			OSCL_LOGW("misdetection possible!");
		}
	}

	if (name == NULL)
		OSCL_LOGW("file:%s not supported yet!", file_name);

EXIT:
	if (buf != NULL)
		oscl_free(buf);
	if (fd >= 0)
		close(fd);
	return name;
}

