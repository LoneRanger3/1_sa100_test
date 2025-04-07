/*
 * MOV, MP4 muxer
 */

#include "movenc.h"
#include "avformat.h"
#include "statname.h"
#define AV_PKT_FLAG_KEY     0x0001 /* < The packet contains a keyframe */
#ifdef __EOS__
#define MOUNT_ROOT_PATH "/mnt/sdcard/"
#else
#define MOUNT_ROOT_PATH "/data/"
#endif

char stco_name[] = "0mp4stco.bin";
char stsc_name[] = "0mp4stsc.bin";
char stsz_name[] = "0mp4stsz.bin";
char ctts_name[] = "0mp4ctts.bin";
char stss_name[] = "0mp4stss.bin";
char stts_name[] = "0mp4stts.bin";

MOVMuxContext *binfile_ctx[2];

#define MKTAG(a, b, c, d) ((a) | ((b) << 8) | ((c) << 16) | ((unsigned int)(d) << 24))
/*#define MKBETAG(a,b,c,d) ((d) | ((c) << 8) | ((b) << 16) | ((unsigned)(a) << 24))*/
#define FFMIN(a, b) ((a) > (b) ? (b) : (a))
#define FFMAX(a, b) ((a) > (b) ? (a) : (b))
#define AV_NOPTS_VALUE          -1
#ifndef INT_MAX
#define INT_MAX       2147483647    /* maximum (signed) int value */
#endif
/**
* Rounding methods.
*/
enum AVRounding {
	AV_ROUND_ZERO = 0, /*< Round toward zero.*/
	AV_ROUND_INF = 1, /*< Round away from zero.*/
	AV_ROUND_DOWN = 2, /*< Round toward -infinity.*/
	AV_ROUND_UP = 3, /*< Round toward +infinity.*/
	AV_ROUND_NEAR_INF = 5, /*< Round to nearest and halfway cases away from zero.*/
	AV_ROUND_PASS_MINMAX = 8192,
};

#ifndef AV_RB16
#   define AV_RB16(x)                           \
	((((const uint8_t *)(x))[0] << 8) | \
	((const uint8_t *)(x))[1])
#endif

#ifndef AV_WB32
#   define AV_WB32(p, darg) do {                \
	unsigned d = (darg);                    \
	((uint8_t *)(p))[3] = (d);               \
	((uint8_t *)(p))[2] = (d) >> 8;            \
	((uint8_t *)(p))[1] = (d) >> 16;           \
	((uint8_t *)(p))[0] = (d) >> 24;           \
} while (0)
#endif
#ifndef AV_RB32
#   define AV_RB32(x)                                \
	(((uint32_t)((const uint8_t *)(x))[0] << 24) | \
	(((const uint8_t *)(x))[1] << 16) | \
	(((const uint8_t *)(x))[2] << 8) | \
	((const uint8_t *)(x))[3])
#endif
#ifndef AV_RB24
#   define AV_RB24(x)                           \
	((((const uint8_t *)(x))[0] << 16) | \
	(((const uint8_t *)(x))[1] << 8) | \
	((const uint8_t *)(x))[2])
#endif

const AVCodecTag ff_mp4_obj_type[] = {
	{ AV_CODEC_TYPE_MPEG4, 0x20 },
	{ AV_CODEC_TYPE_H264, 0x21 },
	{ AV_CODEC_TYPE_AAC, 0x40 },
	{ AV_CODEC_TYPE_AAC, 0x66 }, /* MPEG-2 AAC Main */
	{ AV_CODEC_TYPE_AAC, 0x67 }, /* MPEG-2 AAC Low */
	{ AV_CODEC_TYPE_AAC, 0x68 }, /* MPEG-2 AAC SSR */
	{ AV_CODEC_TYPE_MP3, 0x69 }, /* 13818-3 */
	{ AV_CODEC_TYPE_MP3, 0x6B }, /* 11172-3 */
	{ AV_CODEC_TYPE_PCM_S16LE, 0x03 }, /* MOV */
	{ AV_CODEC_TYPE_ADPCM_IMA_WAV, 0x04 }, /* MOV */
	{ AV_CODEC_TYPE_INVALID, 0 },
};

#ifndef av_bswap16
static uint16_t av_bswap16(uint16_t x)
{
	x = (x >> 8) | (x << 8);
	return x;
}
#endif

void av_free(void *ptr)
{
	free(ptr);
}

void av_freep(void *arg)
{
	void *val;

	memcpy(&val, arg, sizeof(val));
	memcpy(arg, &(void *) {
		NULL
	}, sizeof(val));
	av_free(val);
}

void *av_malloc(size_t size)
{
	void *ptr = NULL;
	ptr = malloc(size);
	return ptr;
}

void *av_mallocz(size_t size)
{
	void *ptr = av_malloc(size);
	if (ptr)
		memset(ptr, 0, size);
	return ptr;
}

#if 0
void *av_realloc(void *ptr, size_t size)
{
	return realloc(ptr, size);
}
#endif

#ifndef __BS_FFMPEG
static void avio_w8(AVIOContext *s, int b)
{
	avio_fwrite(&b, 1, 1, (AVIOFILE)s);
}

void avio_wl16(AVIOContext *s, unsigned int val)
{
	/* avio_w8(s, (uint8_t)val);
	 avio_w8(s, (int)val >> 8); */
	avio_fwrite(&val, 1, 2, (AVIOFILE)s);
}
void avio_wb16(AVIOContext *s, unsigned int val)
{
	avio_w8(s, (int)val >> 8);
	avio_w8(s, (uint8_t)val);
}

void avio_wb24(AVIOContext *s, unsigned int val)
{
	avio_wb16(s, (int)val >> 8);
	avio_w8(s, (uint8_t)val);
}

void avio_wl32(AVIOContext *s, unsigned int val)
{
	/* avio_w8(s, (uint8_t) val       );
	avio_w8(s, (uint8_t)(val >> 8 ));
	avio_w8(s, (uint8_t)(val >> 16));
	avio_w8(s,           val >> 24 ); */
	avio_fwrite(&val, 1, 4, (AVIOFILE)s);
}

static void avio_wb32(AVIOContext *s, unsigned int val)
{
	/*avio_w8(s, val >> 24);
	avio_w8(s, (uint8_t)(val >> 16));
	avio_w8(s, (uint8_t)(val >> 8));
	avio_w8(s, (uint8_t)val);*/
	val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0x00FF00FF);
	val = (val >> 16) | (val << 16);
	avio_fwrite(&val, 1, 4, (AVIOFILE)s);
}

void avio_wb64(AVIOContext *s, uint64_t val)
{
	avio_wb32(s, (uint32_t)(val >> 32));
	avio_wb32(s, (uint32_t)(val & 0xffffffff));
}

void avio_write(AVIOContext *s, const unsigned char *buf, int size)
{
	avio_fwrite(buf, 1, size, (AVIOFILE)s);
}
#endif
static  void ffio_wfourcc(AVIOContext *pb, const char *s)
{
	avio_wl32(pb, MKTAG(s[0], s[1], s[2], s[3]));
}

enum MovChannelLayoutTag {
#define MOV_CH_LAYOUT_UNKNOWN             0xFFFF0000
	MOV_CH_LAYOUT_USE_DESCRIPTIONS = (0 << 16) | 0,
	MOV_CH_LAYOUT_USE_BITMAP = (1 << 16) | 0,
	MOV_CH_LAYOUT_DISCRETEINORDER = (147 << 16) | 0,
	MOV_CH_LAYOUT_MONO = (100 << 16) | 1,
	MOV_CH_LAYOUT_STEREO = (101 << 16) | 2,
};
/**
* Count number of bits set to one in x
* @param x value to count bits of
* @return the number of bits set to one in x
*/
static int av_popcount_c(uint32_t x)
{
	x -= (x >> 1) & 0x55555555;
	x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
	x = (x + (x >> 4)) & 0x0F0F0F0F;
	x += x >> 8;
	return (x + (x >> 16)) & 0x3F;
}

uint32_t ff_mov_get_channel_layout_tag(enum av_codec_type codec_id,
	uint64_t channel_layout,
	uint32_t *bitmap)
{
	uint32_t tag = 0;

	/* find the layout list for the specified codec */
	if ((codec_id == AV_CODEC_TYPE_AAC) || (codec_id == AV_CODEC_TYPE_PCM_S16LE)) {
		int channel = av_popcount_c(channel_layout);
		if (channel == 1)
			tag = MOV_CH_LAYOUT_MONO;
		else if (channel == 2)
			tag = MOV_CH_LAYOUT_STEREO;

	}

	/* if no tag was found, use channel bitmap as a backup if possible */
	if (tag == 0 && channel_layout > 0 && channel_layout < 0x40000) {
		tag = MOV_CH_LAYOUT_USE_BITMAP;
		*bitmap = (uint32_t)channel_layout;
	} else
		*bitmap = 0;

	/* TODO: set channel descriptions as a secondary backup */

	return tag;
}

int64_t av_rescale_rnd(int64_t a, int64_t b, int64_t c, enum AVRounding rnd)
{
	int64_t r = 0;
	/* av_assert2(c > 0);
	av_assert2(b >= 0);
	av_assert2((unsigned)(rnd & ~AV_ROUND_PASS_MINMAX) <= 5
		&& (rnd & ~AV_ROUND_PASS_MINMAX) != 4); */

	if (rnd == AV_ROUND_NEAR_INF)
		r = c / 2;
	else if (rnd & 1)
		r = c - 1;

	return (a * b + r) / c;
}

int av_get_bits_per_sample(enum av_codec_type codec_id)
{
	switch (codec_id) {
	case AV_CODEC_TYPE_ADPCM_IMA_WAV:
		return 4;
	case AV_CODEC_TYPE_PCM_S16LE:
		return 16;
	default:
		return 0;
	}
}

/* WAVEFORMATEX header */
/* returns the size or -1 on error */
int ff_put_wav_header(AVFormatContext *s, AVIOContext *pb,
	AV_CodecParameters *par, int flags)
{
	int bps, blkalign, bytespersec, frame_size;
	int hdrsize;
	int64_t hdrstart = avio_tell(pb);

	if (!par->codec_tag || par->codec_tag > 0xffff)
		return -1;

	/* We use the known constant frame size for the codec if known, otherwise
	 * fall back on using AVCodecContext.frame_size, which is not as reliable
	 * for indicating packet duration. */
	/* av_get_audio_frame_duration2(par, par->block_align); */
	frame_size = par->frame_size;

	avio_wl16(pb, par->codec_tag);

	avio_wl16(pb, par->channels);
	avio_wl32(pb, par->sample_rate);
	bps = par->bits_per_coded_sample;
	blkalign = par->block_align;

	if (par->codec_id == AV_CODEC_TYPE_PCM_S16LE)
		bytespersec = par->sample_rate * blkalign;
	else
		bytespersec = par->bit_rate / 8;

	avio_wl32(pb, bytespersec); /* bytes per second */
	avio_wl16(pb, blkalign);    /* block align */
	avio_wl16(pb, bps);         /* bits per sample */
	avio_wl16(pb, 2); /* cbSize */
	avio_wl16(pb, frame_size);
	hdrsize = avio_tell(pb) - hdrstart;
	if (hdrsize & 1) {
		hdrsize++;
		avio_w8(pb, 0);
	}

	return hdrsize;
}

static unsigned int ff_codec_get_tag(const AVCodecTag *tags, enum av_codec_type id)
{
	while (tags->id != AV_CODEC_TYPE_INVALID) {
		if (tags->id == id)
			return tags->tag;
		tags++;
	}
	return 0;
}

static const uint8_t *ff_avc_find_startcode_internal(const uint8_t *p, const uint8_t *end)
{
	const uint8_t *a = p + 4 - ((intptr_t)p & 3);

	for (end -= 3; p < a && p < end; p++) {
		if (p[0] == 0 && p[1] == 0 && p[2] == 1)
			return p;
	}

	for (end -= 3; p < end; p += 4) {
		uint32_t x = *(const uint32_t *)p;
		/*      if ((x - 0x01000100) & (~x) & 0x80008000) // little endian
		      if ((x - 0x00010001) & (~x) & 0x00800080) // big endian */
		if ((x - 0x01010101) & (~x) & 0x80808080) { /* generic */
			if (p[1] == 0) {
				if (p[0] == 0 && p[2] == 1)
					return p;
				if (p[2] == 0 && p[3] == 1)
					return p + 1;
			}
			if (p[3] == 0) {
				if (p[2] == 0 && p[4] == 1)
					return p + 2;
				if (p[4] == 0 && p[5] == 1)
					return p + 3;
			}
		}
	}

	for (end += 3; p < end; p++) {
		if (p[0] == 0 && p[1] == 0 && p[2] == 1)
			return p;
	}

	return end + 3;
}

static const uint8_t *ff_avc_find_startcode(const uint8_t *p, const uint8_t *end)
{
	const uint8_t *out = ff_avc_find_startcode_internal(p, end);
	if (p < out && out < end && !out[-1])
		out--;
	return out;
}

int ff_avc_parse_nal_units_buffer(uint8_t *buf, const uint8_t *buf_in, int size)
{
	const uint8_t *p = buf_in;
	const uint8_t *end = p + size;
	const uint8_t *nal_start, *nal_end;
	int nal_size = 0;
	uint8_t *buf_out = buf;

	size = 0;
	nal_start = ff_avc_find_startcode(p, end);
	for (;;) {
		while (nal_start < end && !*(nal_start++))
			;
		if (nal_start == end)
			break;

		nal_end = ff_avc_find_startcode(nal_start, end);
		/* avio_wb32(pb, nal_end - nal_start); */
		AV_WB32(&nal_size, nal_end - nal_start);
		memcpy(buf_out, &nal_size, 4);
		/* avio_write(pb, nal_start, nal_end - nal_start); */
		memcpy(buf_out + 4, nal_start, nal_end - nal_start);
		buf_out += 4 + nal_end - nal_start;
		size += 4 + nal_end - nal_start;
		nal_start = nal_end;
	}
	return size;
}

int ff_avc_parse_nal_units(AVIOContext *pb,  const uint8_t *buf_in, int size)
{
	const uint8_t *p = buf_in;
	const uint8_t *end = p + size;
	const uint8_t *nal_start, *nal_end;

	size = 0;
	nal_start = ff_avc_find_startcode(p, end);
	for (;;) {
		while (nal_start < end && !*(nal_start++))
			;
		if (nal_start == end)
			break;

		nal_end = ff_avc_find_startcode(nal_start, end);
		avio_wb32(pb, nal_end - nal_start);
		avio_write(pb, nal_start, nal_end - nal_start);
		size += 4 + nal_end - nal_start;
		nal_start = nal_end;
	}
	return size;
}


int ff_avc_parse_nal_units_buf(const uint8_t *buf_in, uint8_t **buf, int *size)
{
	int ret = 0;
	*buf = av_mallocz(*size + 1024);
	if (ret < 0)
		return ret;

	*size = ff_avc_parse_nal_units_buffer(*buf, buf_in, *size);

	return 0;
}

int ff_isom_write_avcc(AVIOContext *pb, const uint8_t *data, int len)
{
	if (len > 6) {
		/* check for H.264 start code */
		if (AV_RB32(data) == 0x00000001 ||
			AV_RB24(data) == 0x000001) {
			uint8_t *buf = NULL, *end, *start;
			uint32_t sps_size = 0, pps_size = 0;
			uint8_t *sps = 0, *pps = 0;

			int ret = ff_avc_parse_nal_units_buf(data, &buf, &len);
			if (ret < 0)
				return ret;
			start = buf;
			end = buf + len;

			/* look for sps and pps */
			while (end - buf > 4) {
				uint32_t size;
				uint8_t nal_type;
				size = FFMIN(AV_RB32(buf), end - buf - 4);
				buf += 4;
				nal_type = buf[0] & 0x1f;

				if (nal_type == 7) { /* SPS */
					sps = buf;
					sps_size = size;
				} else if (nal_type == 8) { /* PPS */
					pps = buf;
					pps_size = size;
				}

				buf += size;
			}

			if (!sps || !pps || sps_size < 4 || sps_size > UINT16_MAX
				|| pps_size > UINT16_MAX)
				return -1;/* AVERROR_INVALIDDATA;*/

			avio_w8(pb, 1); /* version */
			avio_w8(pb, sps[1]); /* profile */
			avio_w8(pb, sps[2]); /* profile compat */
			avio_w8(pb, sps[3]); /* level */
			/* 6 bits reserved (111111) + 2 bits nal size length - 1 (11) */
			avio_w8(pb, 0xff);
			/* 3 bits reserved (111) + 5 bits number of sps (00001) */
			avio_w8(pb, 0xe1);

			avio_wb16(pb, sps_size);
			avio_write(pb, sps, sps_size);
			avio_w8(pb, 1); /* number of pps */
			avio_wb16(pb, pps_size);
			avio_write(pb, pps, pps_size);
			av_free(start);
		} else {
			avio_write(pb, data, len);
		}
	}
	return 0;
}

/*static int get_moov_size(AVFormatContext *s);

static int utf8len(const uint8_t *b)
{
    int len = 0;
    int val;
	while (*b) {
		GET_UTF8(val, *b++, return -1;)
		len++;
    }
    return len;
} */

/* FIXME support 64 bit variant with wide placeholders */
static int64_t update_size(AVIOContext *pb, int64_t pos)
{
	int64_t curpos = avio_tell(pb);
	avio_seek(pb, pos, SEEK_SET);
	avio_wb32(pb, curpos - pos); /* rewrite size */
	avio_seek(pb, curpos, SEEK_SET);

	return curpos - pos;
}

/*
static int co64_required(const MOVTrack *track)
{
if (track->entry > 0 && track->cluster[track->entry - 1].pos
    + track->data_offset > UINT32_MAX)
return 1;
return 0;
}
*/
/* Chunk offset atom */
static int mov_write_stco_tag(AVIOContext *pb, MOVTrack *track)
{
	int i;
	/* int64_t pos = avio_tell(pb); */
	avio_wb32(pb, (track->chunkCount + 4) * 4); /* size */
	ffio_wfourcc(pb, "stco");
	avio_wb32(pb, 0); /* version & flags */
	avio_wb32(pb, track->chunkCount); /* entry count */
	if (track->clust.framechunk && track->clust.stcofile) {
		fseek(track->clust.stcofile, 0, 0);
		for (i = 0; i < track->clust.framechunk; i++) {
			int j = 0;
			int len;
			len = fread(track->clust.cache, 4, CHUNK_NUM,
					track->clust.stcofile);
			if (len != CHUNK_NUM)
				printf("read cache err, len = %d!", len);
			for (j = 0; j < CHUNK_NUM; j++)
				avio_wb32(pb, track->clust.cache[j]);

		}
	}

	for (i = 0; i < (track->chunkCount & (CHUNK_NUM - 1)); i++)
		avio_wb32(pb, track->clust.pos[i]);

	/*for (i = 0; i < track->entry; i++) {
	    if (!track->cluster[i].chunkNum)
			continue;
	    avio_wb32(pb, track->cluster[i].pos + track->data_offset);
	}*/
	return 0; /* update_size(pb, pos); */
}

/* Sample size atom */
static int mov_write_stsz_tag(AVIOContext *pb, MOVTrack *track)
{
	int equalChunks = 1;
	int i;

	/* int64_t pos = avio_tell(pb); */
	if ((track->par->codec_id == AV_CODEC_TYPE_ADPCM_IMA_WAV)
		|| (track->par->codec_id == AV_CODEC_TYPE_PCM_S16LE)) {
		equalChunks = 1;
	} else {
		equalChunks = 0;
	}

	avio_wb32(pb, equalChunks == 1 ? 20 : (track->entry + 5) * 4);  /* size */
	ffio_wfourcc(pb, "stsz");
	avio_wb32(pb, 0); /* version & flags */

	/* for (i = 0; i < track->entry; i++) {
		 tst = track->cluster[i].size / track->cluster[i].entries;
		 if (oldtst != -1 && tst != oldtst)
		 equalChunks = 0;
		 oldtst = tst;
		 entries += track->cluster[i].entries;
		 }*/

	if (equalChunks && track->entry) {
		int sSize = 0;
		if (!track->clust.entries) {
			track->clust.entries = (track->par->block_align -
					4 * track->par->channels) * 8 /
				(4 * track->par->channels) + 1;
		}
		sSize = track->entry ? track->clust.size[0] / track->clust.entries : 0;
		sSize = FFMAX(1, sSize); /* adpcm mono case could make sSize == 0 */
		avio_wb32(pb, sSize); /* sample size */
		avio_wb32(pb, track->entry * track->clust.entries); /* sample count */
	} else {
		avio_wb32(pb, 0); /* sample size */
		avio_wb32(pb, track->entry); /* sample count */
		if (track->clust.stszfile) {
			fseek(track->clust.stszfile, 0, 0);
			for (i = 0; i < track->clust.frameentry *
				(STSZ_NUM / CHUNK_NUM); i++) {
				int j = 0;
				int len;
				len = fread(track->clust.cache, 4, CHUNK_NUM,
						track->clust.stszfile);
				if (len != CHUNK_NUM)
					printf("read cache err, len=%d!", len);
				for (j = 0; j < CHUNK_NUM; j++)
					avio_wb32(pb, track->clust.cache[j]);

			}
		}

		for (i = 0; i < (track->entry & (STSZ_NUM - 1)); i++)
			avio_wb32(pb, track->clust.size[i]);

	}
	return 0; /* update_size(pb, pos); */
}

/* Sample to chunk atom */
static int mov_write_stsc_tag(AVIOContext *pb, MOVTrack *track)
{
	int index = 0, oldval = -1, i;
	int64_t entryPos, curpos;

	int64_t pos = avio_tell(pb);
	avio_wb32(pb, 0); /* size */
	ffio_wfourcc(pb, "stsc");
	avio_wb32(pb, 0); /* version & flags */
	entryPos = avio_tell(pb);
	avio_wb32(pb, track->chunkCount); /* entry count */
	/* for (i = 0; i < track->entry; i++) {
	    if (oldval != track->cluster[i].samples_in_chunk
	    && track->cluster[i].chunkNum) {
			avio_wb32(pb, track->cluster[i].chunkNum);
			avio_wb32(pb, track->cluster[i].samples_in_chunk);
			avio_wb32(pb, 0x1); // sample description index
			oldval = track->cluster[i].samples_in_chunk;
			index++;
	    }
	} */
	if (track->clust.framechunk && track->clust.stscfile) {
		fseek(track->clust.stscfile, 0, 0);
		for (i = 0; i < track->clust.framechunk; i++) {
			int j = 0;
			int ret = 0;
			ret = fread(track->clust.cache, 4, CHUNK_NUM,
					track->clust.stscfile);
			if (ret != CHUNK_NUM)
				printf("read cache err, len=%d!", ret);
			for (j = 0; j < ret; j++) {
				if (oldval != track->clust.cache[j]) {
					/* first chunk */
					avio_wb32(pb, i * CHUNK_NUM + j + 1);
					/* samples per chunk */
					avio_wb32(pb, track->clust.cache[j]);
					avio_wb32(pb, 0x1); /* sample description index */
					oldval = track->clust.cache[j];
					index++;
				}
			}
			if (ret != CHUNK_NUM) { /* chunk last no write */
				if (oldval !=
					track->clust.samples_in_chunk[CHUNK_NUM - 1]) {
					/* first chunk */
					avio_wb32(pb,
						track->clust.framechunk * CHUNK_NUM);
					/* samples per chunk */
					avio_wb32(pb,
						track->clust.samples_in_chunk[
							CHUNK_NUM - 1]);
					avio_wb32(pb, 0x1); /* sample description index */
					oldval = track->clust.samples_in_chunk[
							CHUNK_NUM - 1];
					index++;
				}

			}
		}
	}
	for (i = 0; i < (track->chunkCount & (CHUNK_NUM - 1)); i++) {
		if (oldval != track->clust.samples_in_chunk[i]) {
			/* first chunk */
			avio_wb32(pb, track->clust.framechunk * CHUNK_NUM + i + 1);
			/* samples per chunk */
			avio_wb32(pb, track->clust.samples_in_chunk[i]);
			avio_wb32(pb, 0x1); /* sample description index */
			oldval = track->clust.samples_in_chunk[i];
			index++;
		}
	}
	curpos = avio_tell(pb);
	avio_seek(pb, entryPos, SEEK_SET);
	avio_wb32(pb, index); /* rewrite size */
	avio_seek(pb, curpos, SEEK_SET);

	return update_size(pb, pos);
}

/* Sync sample atom */
static int mov_write_stss_tag(AVIOContext *pb, MOVTrack *track)
{
	/* int64_t curpos, entryPos; */
	int i;/* index = 0; */
	/* int64_t pos = avio_tell(pb); */
	avio_wb32(pb, (track->has_keyframes + 4) * 4); /* size */
	ffio_wfourcc(pb, "stss");
	avio_wb32(pb, 0); /* version & flags */
	/* entryPos = avio_tell(pb); */
	avio_wb32(pb, track->has_keyframes); /* entry count */
	if ((track->has_keyframes / STSS_NUM) && track->clust.stssfile) {
		int num = track->has_keyframes / STSS_NUM;
		num *= STSS_NUM / CHUNK_NUM;
		fseek(track->clust.stssfile, 0, 0);
		for (i = 0; i < num; i++) {
			int j = 0;
			int len = 0;
			len = fread(track->clust.cache, 4, CHUNK_NUM,
					track->clust.stssfile);
			if (len != CHUNK_NUM)
				printf("read cache err, len=%d!", len);
			for (j = 0; j < CHUNK_NUM; j++) {
				avio_wb32(pb, track->clust.cache[j]);
				/* index++; */
			}
		}
	}

	for (i = 0; i < (track->has_keyframes & (STSS_NUM - 1)); i++) {
		avio_wb32(pb, track->clust.keyflags[i]);
		/* index++; */
	}
	/*
	curpos = avio_tell(pb);
	avio_seek(pb, entryPos, SEEK_SET);
	avio_wb32(pb, index);
	avio_seek(pb, curpos, SEEK_SET);
	*/
	return 0;/* update_size(pb, pos); */
}

#if 0
static int mov_write_amr_tag(AVIOContext *pb, MOVTrack *track)
{
	avio_wb32(pb, 0x11); /* size */
	if (track->mode == MODE_MOV)
		ffio_wfourcc(pb, "samr");
	else
		ffio_wfourcc(pb, "damr");
	ffio_wfourcc(pb, "FFMP");
	avio_w8(pb, 0); /* decoder version */

	avio_wb16(pb, 0x81FF); /* Mode set (all modes for AMR_NB) */
	avio_w8(pb, 0x00); /* Mode change period (no restriction) */
	avio_w8(pb, 0x01); /* Frames per sample */
	return 0x11;
}

static int mov_write_ac3_tag(AVIOContext *pb, MOVTrack *track)
{
	GetBitContext gbc;
	PutBitContext pbc;
	uint8_t buf[3];
	int fscod, bsid, bsmod, acmod, lfeon, frmsizecod;

	if (track->vos_len < 7)
		return -1;

	avio_wb32(pb, 11);
	ffio_wfourcc(pb, "dac3");

	init_get_bits(&gbc, track->vos_data + 4, (track->vos_len - 4) * 8);
	fscod      = get_bits(&gbc, 2);
	frmsizecod = get_bits(&gbc, 6);
	bsid       = get_bits(&gbc, 5);
	bsmod      = get_bits(&gbc, 3);
	acmod      = get_bits(&gbc, 3);
	if (acmod == 2) {
		skip_bits(&gbc, 2); /* dsurmod */
	} else {
		if ((acmod & 1) && acmod != 1)
			skip_bits(&gbc, 2); /* cmixlev */
		if (acmod & 4)
			skip_bits(&gbc, 2); /* surmixlev */
	}
	lfeon = get_bits1(&gbc);

	init_put_bits(&pbc, buf, sizeof(buf));
	put_bits(&pbc, 2, fscod);
	put_bits(&pbc, 5, bsid);
	put_bits(&pbc, 3, bsmod);
	put_bits(&pbc, 3, acmod);
	put_bits(&pbc, 1, lfeon);
	put_bits(&pbc, 5, frmsizecod >> 1); /* bit_rate_code */
	put_bits(&pbc, 5, 0); /* reserved */

	flush_put_bits(&pbc);
	avio_write(pb, buf, sizeof(buf));

	return 11;
}

struct eac3_info {
	AVPacket pkt;
	uint8_t ec3_done;
	uint8_t num_blocks;

	/* Layout of the EC3SpecificBox */
	/* maximum bitrate */
	uint16_t data_rate;
	/* number of independent substreams */
	uint8_t num_ind_sub;
	struct {
		/* sample rate code (see ff_ac3_sample_rate_tab) 2 bits */
		uint8_t fscod;
		/* bit stream identification 5 bits */
		uint8_t bsid;
		/* one bit reserved */
		/* audio service mixing (not supported yet) 1 bit */
		/* bit stream mode 3 bits */
		uint8_t bsmod;
		/* audio coding mode 3 bits */
		uint8_t acmod;
		/* sub woofer on 1 bit */
		uint8_t lfeon;
		/* 3 bits reserved */
		/* number of dependent substreams associated with this substream 4 bits */
		uint8_t num_dep_sub;
		/* channel locations of the dependent substream(s), if any, 9 bits */
		uint16_t chan_loc;
		/* if there is no dependent substream, then one bit reserved instead */
	} substream[1]; /* TODO: support 8 independent substreams */
};

#if CONFIG_AC3_PARSER
static int handle_eac3(MOVMuxContext *mov, AVPacket *pkt, MOVTrack *track)
{
	GetBitContext gbc;
	AC3HeaderInfo tmp, *hdr = &tmp;
	struct eac3_info *info;
	int num_blocks;

	if (!track->eac3_priv) {
		track->eac3_priv = av_mallocz(sizeof(*info));
		if (!track->eac3_priv)
			return AVERROR(ENOMEM);
	}
	info = track->eac3_priv;

	init_get_bits(&gbc, pkt->data, pkt->size * 8);
	if (avpriv_ac3_parse_header(&gbc, &hdr) < 0) {
		/* drop the packets until we see a good one */
		if (!track->entry) {
			av_log(mov, AV_LOG_WARNING,
				"Dropping invalid packet from start of the stream\n");
			return 0;
		}
		return AVERROR_INVALIDDATA;
	}

	info->data_rate = FFMAX(info->data_rate, hdr->bit_rate / 1000);
	num_blocks = hdr->num_blocks;

	if (!info->ec3_done) {
		/* AC-3 substream must be the first one */
		if (hdr->bitstream_id <= 10 && hdr->substreamid != 0)
			return AVERROR(EINVAL);

		/* this should always be the case, given that our AC-3 parser
		 * concatenates dependent frames to their independent parent */
		if (hdr->frame_type == EAC3_FRAME_TYPE_INDEPENDENT) {
			/* substream ids must be incremental */
			if (hdr->substreamid > info->num_ind_sub + 1)
				return AVERROR(EINVAL);

			if (hdr->substreamid == info->num_ind_sub + 1) {
				/* info->num_ind_sub++; */
				avpriv_request_sample(track->par,
					"Multiple independent substreams");
				return AVERROR_PATCHWELCOME;
			} else if (hdr->substreamid < info->num_ind_sub ||
				hdr->substreamid == 0 && info->substream[0].bsid) {
				info->ec3_done = 1;
				goto concatenate;
			}
		}

		/* fill the info needed for the "dec3" atom */
		info->substream[hdr->substreamid].fscod = hdr->sr_code;
		info->substream[hdr->substreamid].bsid  = hdr->bitstream_id;
		info->substream[hdr->substreamid].bsmod = hdr->bitstream_mode;
		info->substream[hdr->substreamid].acmod = hdr->channel_mode;
		info->substream[hdr->substreamid].lfeon = hdr->lfe_on;

		/* Parse dependent substream(s), if any */
		if (pkt->size != hdr->frame_size) {
			int cumul_size = hdr->frame_size;
			int parent = hdr->substreamid;

			while (cumul_size != pkt->size) {
				int i;
				init_get_bits(&gbc, pkt->data + cumul_size,
					(pkt->size - cumul_size) * 8);
				if (avpriv_ac3_parse_header(&gbc, &hdr) < 0)
					return AVERROR_INVALIDDATA;
				if (hdr->frame_type != EAC3_FRAME_TYPE_DEPENDENT)
					return AVERROR(EINVAL);
				cumul_size += hdr->frame_size;
				info->substream[parent].num_dep_sub++;

				/* header is parsed up to lfeon, but custom channel
				map may be needed */
				/* skip bsid */
				skip_bits(&gbc, 5);
				/* skip volume control params */
				for (i = 0; i < (hdr->channel_mode ? 1 : 2); i++) {
					/* skip dialog normalization */
					skip_bits(&gbc, 5);
					if (get_bits1(&gbc))
						/* skip compression gain word */
						skip_bits(&gbc, 8);

				}
				/* get the dependent stream channel map, if exists */
				if (get_bits1(&gbc))
					info->substream[parent].chan_loc |=
						(get_bits(&gbc, 16) >> 5) & 0x1f;
				else
					info->substream[parent].chan_loc |=
						hdr->channel_mode;
			}
		}
	}

concatenate:
	if (!info->num_blocks && num_blocks == 6)
		return pkt->size;
	else if (info->num_blocks + num_blocks > 6)
		return AVERROR_INVALIDDATA;

	if (!info->num_blocks) {
		int ret;
		ret = av_copy_packet(&info->pkt, pkt);
		if (ret < 0)
			return ret;
		info->num_blocks = num_blocks;
		return 0;
	} else {
		int ret;
		ret = av_grow_packet(&info->pkt, pkt->size);
		if (ret < 0)
			return ret;
		memcpy(info->pkt.data + info->pkt.size - pkt->size, pkt->data, pkt->size);
		info->num_blocks += num_blocks;
		info->pkt.duration += pkt->duration;
		ret = av_copy_packet_side_data(&info->pkt, pkt);
		if (ret < 0)
			return ret;
		if (info->num_blocks != 6)
			return 0;
		av_packet_unref(pkt);
		ret = av_copy_packet(pkt, &info->pkt);
		if (ret < 0)
			return ret;
		av_packet_unref(&info->pkt);
		info->num_blocks = 0;
	}

	return pkt->size;
}
#endif

static int mov_write_eac3_tag(AVIOContext *pb, MOVTrack *track)
{
	PutBitContext pbc;
	uint8_t *buf;
	struct eac3_info *info;
	int size, i;

	if (!track->eac3_priv)
		return AVERROR(EINVAL);

	info = track->eac3_priv;
	size = 2 + 4 * (info->num_ind_sub + 1);
	buf = av_malloc(size);
	if (!buf) {
		size = AVERROR(ENOMEM);
		goto end;
	}

	init_put_bits(&pbc, buf, size);
	put_bits(&pbc, 13, info->data_rate);
	put_bits(&pbc,  3, info->num_ind_sub);
	for (i = 0; i <= info->num_ind_sub; i++) {
		put_bits(&pbc, 2, info->substream[i].fscod);
		put_bits(&pbc, 5, info->substream[i].bsid);
		put_bits(&pbc, 1, 0); /* reserved */
		put_bits(&pbc, 1, 0); /* asvc */
		put_bits(&pbc, 3, info->substream[i].bsmod);
		put_bits(&pbc, 3, info->substream[i].acmod);
		put_bits(&pbc, 1, info->substream[i].lfeon);
		put_bits(&pbc, 5, 0); /* reserved */
		put_bits(&pbc, 4, info->substream[i].num_dep_sub);
		if (!info->substream[i].num_dep_sub) {
			put_bits(&pbc, 1, 0); /* reserved */
			size--;
		} else {
			put_bits(&pbc, 9, info->substream[i].chan_loc);
		}
	}
	flush_put_bits(&pbc);

	avio_wb32(pb, size + 8);
	ffio_wfourcc(pb, "dec3");
	avio_write(pb, buf, size);

	av_free(buf);

end:
	av_packet_unref(&info->pkt);
	av_freep(&track->eac3_priv);

	return size;
}

/**
 * This function writes extradata "as is".
 * Extradata must be formatted like a valid atom (with size and tag).
 */
static int mov_write_extradata_tag(AVIOContext *pb, MOVTrack *track)
{
	avio_write(pb, track->par->extradata, track->par->extradata_size);
	return track->par->extradata_size;
}

static int mov_write_enda_tag(AVIOContext *pb)
{
	avio_wb32(pb, 10);
	ffio_wfourcc(pb, "enda");
	avio_wb16(pb, 1); /* little endian */
	return 10;
}

static int mov_write_enda_tag_be(AVIOContext *pb)
{
	avio_wb32(pb, 10);
	ffio_wfourcc(pb, "enda");
	avio_wb16(pb, 0); /* big endian */
	return 10;
}
#endif
static void put_descr(AVIOContext *pb, int tag, unsigned int size)
{
	int i = 3;
	avio_w8(pb, tag);
	for (; i > 0; i--)
		avio_w8(pb, (size >> (7 * i)) | 0x80);
	avio_w8(pb, size & 0x7F);
}

/*static unsigned compute_avg_bitrate(MOVTrack *track)
{
	uint64_t size = 0;
	int i;
	if (!track->track_duration)
		return 0;
	for (i = 0; i < track->entry; i++)
		size += track->cluster[i].size;
	return size * 8 * track->timescale / track->track_duration;
}*/

static int mov_write_esds_tag(AVIOContext *pb, MOVTrack *track) /* Basic */
{
	/* int64_t pos = avio_tell(pb); */
	int decoder_specific_info_len = track->vos_len ? 5 + track->vos_len : 0;
	unsigned avg_bitrate;

	/* size */
	avio_wb32(pb, 3 * 4 + 5 + 3 + 5 + 13 + decoder_specific_info_len + 5 + 1);
	ffio_wfourcc(pb, "esds");
	avio_wb32(pb, 0); /* Version */

	/* ES descriptor */
	put_descr(pb, 0x03, 3 + 5 + 13 + decoder_specific_info_len + 5 + 1);
	avio_wb16(pb, track->track_id);
	avio_w8(pb, 0x00); /* flags (= no flags) */

	/* DecoderConfig descriptor */
	put_descr(pb, 0x04, 13 + decoder_specific_info_len);

	/* Object type indication */
	if ((track->par->codec_id == AV_CODEC_TYPE_MP3) &&
		track->par->sample_rate > 24000)
		avio_w8(pb, 0x6B); /* 11172-3 */
	else
		avio_w8(pb, ff_codec_get_tag(ff_mp4_obj_type, track->par->codec_id));

	/* the following fields is made of 6 bits to identify the
	 streamtype (4 for video, 5 for audio)
	 plus 1 bit to indicate upstream and 1 bit set to 1 (reserved) */
	if (track->par->codec_type == AV_MEDIA_TYPE_AUDIO)
		avio_w8(pb, 0x15); /* flags (= Audiostream) */
	else
		avio_w8(pb, 0x11); /* flags (= Visualstream) */

	avio_wb24(pb, 0); /* Buffersize DB */

	/*compute_avg_bitrate(track);*/
	avg_bitrate = (int64_t)track->clust.totalsize * 8 * track->timescale /
		track->track_duration;
	/* maxbitrate (FIXME should be max rate in any 1 sec window) */
	avio_wb32(pb, FFMAX(track->par->bit_rate, avg_bitrate));
	avio_wb32(pb, avg_bitrate);

	if (track->vos_len) {
		/* DecoderSpecific info descriptor */
		put_descr(pb, 0x05, track->vos_len);
		avio_write(pb, track->vos_data, track->vos_len);
	}

	/* SL descriptor */
	put_descr(pb, 0x06, 1);
	avio_w8(pb, 0x02);
	return 0; /* update_size(pb, pos); */
}

/*
static int mov_pcm_le_gt16(enum av_codec_type codec_id)
{
    return codec_id == AV_CODEC_TYPE_PCM_S24LE ||
			codec_id == AV_CODEC_TYPE_PCM_S32LE ||
			codec_id == AV_CODEC_TYPE_PCM_F32LE ||
			codec_id == AV_CODEC_TYPE_PCM_F64LE;
}

static int mov_pcm_be_gt16(enum av_codec_type codec_id)
{
    return codec_id == AV_CODEC_TYPE_PCM_S24BE ||
			codec_id == AV_CODEC_TYPE_PCM_S32BE ||
			codec_id == AV_CODEC_TYPE_PCM_F32BE ||
			codec_id == AV_CODEC_TYPE_PCM_F64BE;
}
*/
static int mov_write_ms_tag(AVFormatContext *s, AVIOContext *pb, MOVTrack *track)
{
	int ret;
	/* int64_t pos = avio_tell(pb); */
	avio_wb32(pb, 2 * 4 + 20);
	avio_wl32(pb, track->tag); /* store it byteswapped */
	track->par->codec_tag = av_bswap16(track->tag >> 16);
	ret = ff_put_wav_header(s, pb, track->par, 0);
	if (ret < 0)
		return ret;
	return 0; /* update_size(pb, pos); */
}

/*
static int mov_write_wfex_tag(AVFormatContext *s, AVIOContext *pb, MOVTrack *track)
{
    int ret;
    int64_t pos = avio_tell(pb);
    avio_wb32(pb, 0);
    ffio_wfourcc(pb, "wfex");
    ret = ff_put_wav_header(s, pb, track->st->codecpar,
		FF_PUT_WAV_HEADER_FORCE_WAVEFORMATEX)
    if (ret < 0)
		return ret;
    return update_size(pb, pos);
}
*/
static int mov_write_chan_tag(AVFormatContext *s, AVIOContext *pb, MOVTrack *track)
{
	uint32_t layout_tag, bitmap;
	/* int64_t pos = avio_tell(pb); */

	layout_tag = ff_mov_get_channel_layout_tag(track->par->codec_id,
			track->par->channel_layout,
			&bitmap);
	if (!layout_tag) {
		printf("not writing 'chan' tag due to ");
		printf("lack of channel information\n");
		return 0;
	}

	avio_wb32(pb, 24);           /* Size */
	ffio_wfourcc(pb, "chan");   /* Type */
	avio_w8(pb, 0);             /* Version */
	avio_wb24(pb, 0);           /* Flags */
	avio_wb32(pb, layout_tag);  /* mChannelLayoutTag */
	avio_wb32(pb, bitmap);      /* mChannelBitmap */
	avio_wb32(pb, 0);           /* mNumberChannelDescriptions */

	return 0; /* update_size(pb, pos); */
}

static int mov_write_wave_tag(AVFormatContext *s, AVIOContext *pb, MOVTrack *track)
{
	int64_t pos = avio_tell(pb);

	avio_wb32(pb, 0);     /* size */
	ffio_wfourcc(pb, "wave");

	avio_wb32(pb, 12);    /* size */
	ffio_wfourcc(pb, "frma");
	avio_wl32(pb, track->tag);

	if (track->par->codec_id == AV_CODEC_TYPE_AAC) {
		/* useless atom needed by mplayer, ipod, not needed by quicktime */
		avio_wb32(pb, 12); /* size */
		ffio_wfourcc(pb, "mp4a");
		avio_wb32(pb, 0);
		mov_write_esds_tag(pb, track);
	} else if (track->par->codec_id == AV_CODEC_TYPE_ADPCM_IMA_WAV) {
		mov_write_ms_tag(s, pb, track);
	}

	avio_wb32(pb, 8);     /* size */
	avio_wb32(pb, 0);     /* null tag */

	return update_size(pb, pos);
}

#if 0
static int mov_write_dvc1_structs(MOVTrack *track, uint8_t *buf)
{
	uint8_t *unescaped;
	const uint8_t *start, *next, *end = track->vos_data + track->vos_len;
	int unescaped_size, seq_found = 0;
	int level = 0, interlace = 0;
	int packet_seq   = track->vc1_info.packet_seq;
	int packet_entry = track->vc1_info.packet_entry;
	int slices       = track->vc1_info.slices;
	PutBitContext pbc;

	if (track->start_dts == AV_NOPTS_VALUE) {
		/* No packets written yet, vc1_info isn't authoritative yet. */
		/* Assume inline sequence and entry headers. */
		packet_seq = packet_entry = 1;
		av_log(NULL, AV_LOG_WARNING,
			"moov atom written before any packets, unable to write correct "
			"dvc1 atom. Set the delay_moov flag to fix this.\n");
	}

	unescaped = av_mallocz(track->vos_len + AV_INPUT_BUFFER_PADDING_SIZE);
	if (!unescaped)
		return AVERROR(ENOMEM);
	start = find_next_marker(track->vos_data, end);
	for (next = start; next < end; start = next) {
		GetBitContext gb;
		int size;
		next = find_next_marker(start + 4, end);
		size = next - start - 4;
		if (size <= 0)
			continue;
		unescaped_size = vc1_unescape_buffer(start + 4, size, unescaped);
		init_get_bits(&gb, unescaped, 8 * unescaped_size);
		if (AV_RB32(start) == VC1_CODE_SEQHDR) {
			int profile = get_bits(&gb, 2);
			if (profile != PROFILE_ADVANCED) {
				av_free(unescaped);
				return AVERROR(ENOSYS);
			}
			seq_found = 1;
			level = get_bits(&gb, 3);
			/* chromaformat, frmrtq_postproc, bitrtq_postproc, postprocflag,
			 * width, height */
			skip_bits_long(&gb, 2 + 3 + 5 + 1 + 2 * 12);
			skip_bits(&gb, 1); /* broadcast */
			interlace = get_bits1(&gb);
			skip_bits(&gb, 4); /* tfcntrflag, finterpflag, reserved, psf */
		}
	}
	if (!seq_found) {
		av_free(unescaped);
		return AVERROR(ENOSYS);
	}

	init_put_bits(&pbc, buf, 7);
	/* VC1DecSpecStruc */
	put_bits(&pbc, 4, 12); /* profile - advanced */
	put_bits(&pbc, 3, level);
	put_bits(&pbc, 1, 0); /* reserved */
	/* VC1AdvDecSpecStruc */
	put_bits(&pbc, 3, level);
	put_bits(&pbc, 1, 0); /* cbr */
	put_bits(&pbc, 6, 0); /* reserved */
	put_bits(&pbc, 1, !interlace); /* no interlace */
	put_bits(&pbc, 1, !packet_seq); /* no multiple seq */
	put_bits(&pbc, 1, !packet_entry); /* no multiple entry */
	put_bits(&pbc, 1, !slices); /* no slice code */
	put_bits(&pbc, 1, 0); /* no bframe */
	put_bits(&pbc, 1, 0); /* reserved */

	/* framerate */
	if (track->st->avg_frame_rate.num > 0 && track->st->avg_frame_rate.den > 0)
		put_bits32(&pbc, track->st->avg_frame_rate.num /
			track->st->avg_frame_rate.den);
	else
		put_bits32(&pbc, 0xffffffff);

	flush_put_bits(&pbc);

	av_free(unescaped);

	return 0;
}

static int mov_write_dvc1_tag(AVIOContext *pb, MOVTrack *track)
{
	uint8_t buf[7] = { 0 };
	int ret;

	ret = mov_write_dvc1_structs(track, buf);
	if (ret < 0)
		return ret;

	avio_wb32(pb, track->vos_len + 8 + sizeof(buf));
	ffio_wfourcc(pb, "dvc1");
	avio_write(pb, buf, sizeof(buf));
	avio_write(pb, track->vos_data, track->vos_len);

	return 0;
}
#endif
static int mov_write_glbl_tag(AVIOContext *pb, MOVTrack *track)
{
	avio_wb32(pb, track->vos_len + 8);
	ffio_wfourcc(pb, "glbl");
	avio_write(pb, track->vos_data, track->vos_len);
	return 8 + track->vos_len;
}

#if 0
/**
 * Compute flags for 'lpcm' tag.
 * See CoreAudioTypes and AudioStreamBasicDescription at Apple.
 */
static int mov_get_lpcm_flags(enum av_codec_type codec_id)
{
	switch (codec_id) {
	case AV_CODEC_TYPE_PCM_F32BE:
	case AV_CODEC_TYPE_PCM_F64BE:
		return 11;
	case AV_CODEC_TYPE_PCM_F32LE:
	case AV_CODEC_TYPE_PCM_F64LE:
		return 9;
	case AV_CODEC_TYPE_PCM_U8:
		return 10;
	case AV_CODEC_TYPE_PCM_S16BE:
	case AV_CODEC_TYPE_PCM_S24BE:
	case AV_CODEC_TYPE_PCM_S32BE:
		return 14;
	case AV_CODEC_TYPE_PCM_S8:
	case AV_CODEC_TYPE_PCM_S16LE:
	case AV_CODEC_TYPE_PCM_S24LE:
	case AV_CODEC_TYPE_PCM_S32LE:
		return 12;
	default:
		return 0;
	}
}

static int get_cluster_duration(MOVTrack *track, int cluster_idx)
{
	int64_t next_dts;

	if (cluster_idx >= track->entry)
		return 0;

	if (cluster_idx + 1 == track->entry)
		next_dts = track->track_duration + track->start_dts;
	else
		next_dts = track->cluster[cluster_idx + 1].dts;

	next_dts -= track->cluster[cluster_idx].dts;

	av_assert0(next_dts >= 0);
	av_assert0(next_dts <= INT_MAX);

	return next_dts;
}

static int get_samples_per_packet(MOVTrack *track)
{
	int i, first_duration;

	/* return track->par->frame_size; */

	/* use 1 for raw PCM */
	if (!track->audio_vbr)
		return 1;

	/* check to see if duration is constant for all clusters */
	if (!track->entry)
		return 0;
	first_duration = get_cluster_duration(track, 0);
	for (i = 1; i < track->entry; i++) {
		if (get_cluster_duration(track, i) != first_duration)
			return 0;
	}
	return first_duration;
}
#endif
static int mov_write_audio_tag(AVFormatContext *s, AVIOContext *pb,
	MOVMuxContext *mov,	MOVTrack *track)
{
	int64_t pos = avio_tell(pb);
	int version = 0;
	uint32_t tag = track->tag;

	if (track->mode == MODE_MOV) {
		if (track->audio_vbr  ||
			track->par->codec_id == AV_CODEC_TYPE_ADPCM_IMA_WAV)
			version = 1;

	}

	avio_wb32(pb, 0); /* size */
	avio_wl32(pb, tag); /* store it byteswapped */
	avio_wb32(pb, 0); /* Reserved */
	avio_wb16(pb, 0); /* Reserved */
	avio_wb16(pb, 1); /* Data-reference index, XXX  == 1 */

	/* SoundDescription */
	avio_wb16(pb, version); /* Version */
	avio_wb16(pb, 0); /* Revision level */
	avio_wb32(pb, 0); /* Reserved */

	avio_wb16(pb, track->par->channels);/* avio_wb16(pb, 2); */
	avio_wb16(pb, 16);
	avio_wb16(pb, 0);
	avio_wb16(pb, 0); /* packet size (= 0) */
	avio_wb16(pb, track->par->sample_rate <= UINT16_MAX ?
		track->par->sample_rate : 0);
	avio_wb16(pb, 0); /* Reserved */

	if (version == 1) { /* SoundDescription V1 extended info */
		if ((track->par->codec_id == AV_CODEC_TYPE_ADPCM_IMA_WAV) &&
			(!track->par->frame_size))
			track->par->frame_size = (track->par->block_align - 4 *
					track->par->channels) * 8 /
				(4 * track->par->channels) + 1;

		avio_wb32(pb, track->par->frame_size); /* Samples per packet */
		/* Bytes per packet */
		avio_wb32(pb, track->sample_size / track->par->channels);
		avio_wb32(pb, track->sample_size); /* Bytes per frame */
		avio_wb32(pb, 2); /* Bytes per sample */
	}

	if (track->mode == MODE_MOV &&
		(track->par->codec_id == AV_CODEC_TYPE_AAC ||
			track->par->codec_id == AV_CODEC_TYPE_ADPCM_IMA_WAV))
		mov_write_wave_tag(s, pb, track);
	else if (track->tag == MKTAG('m', 'p', '4', 'a'))
		mov_write_esds_tag(pb, track);
	else if (track->vos_len > 0)
		mov_write_glbl_tag(pb, track);

	if (track->mode == MODE_MOV && track->par->codec_type == AV_MEDIA_TYPE_AUDIO)
		mov_write_chan_tag(s, pb, track);

	return update_size(pb, pos);
}

#if 0
static int mov_write_d263_tag(AVIOContext *pb)
{
	avio_wb32(pb, 0xf); /* size */
	ffio_wfourcc(pb, "d263");
	ffio_wfourcc(pb, "FFMP");
	avio_w8(pb, 0); /* decoder version */
	/* FIXME use AVCodecContext level/profile, when encoder will set values */
	avio_w8(pb, 0xa); /* level */
	avio_w8(pb, 0); /* profile */
	return 0xf;
}
#endif
static int mov_write_avcc_tag(AVIOContext *pb, MOVTrack *track)
{
	int64_t pos = avio_tell(pb);

	avio_wb32(pb, 0);
	ffio_wfourcc(pb, "avcC");
	ff_isom_write_avcc(pb, track->vos_data, track->vos_len);
	return update_size(pb, pos);
}

#if 0
static int mov_write_vpcc_tag(AVFormatContext *s, AVIOContext *pb, MOVTrack *track)
{
	int64_t pos = avio_tell(pb);

	avio_wb32(pb, 0);
	ffio_wfourcc(pb, "vpcC");
	avio_wb32(pb, 0); /* version & flags */
	ff_isom_write_vpcc(s, pb, track->par);
	return update_size(pb, pos);
}

static int mov_write_hvcc_tag(AVIOContext *pb, MOVTrack *track)
{
	int64_t pos = avio_tell(pb);

	avio_wb32(pb, 0);
	ffio_wfourcc(pb, "hvcC");
	ff_isom_write_hvcc(pb, track->vos_data, track->vos_len, 0);
	return update_size(pb, pos);
}

/* also used by all avid codecs (dv, imx, meridien) and their variants */
static int mov_write_avid_tag(AVIOContext *pb, MOVTrack *track)
{
	int i;
	int interlaced;
	int cid;
	int display_width = track->par->width;

	if (track->vos_data && track->vos_len > 0x29) {
		if (ff_dnxhd_parse_header_prefix(track->vos_data) != 0) {
			/* looks like a DNxHD bit stream */
			interlaced = (track->vos_data[5] & 2);
			cid = AV_RB32(track->vos_data + 0x28);
		} else {
			av_log(NULL, AV_LOG_WARNING,
				"Could not locate DNxHD bit stream in vos_data\n");
			return 0;
		}
	} else {
		av_log(NULL, AV_LOG_WARNING,
			"Could not locate DNxHD bit stream, vos_data too small\n");
		return 0;
	}

	avio_wb32(pb, 24); /* size */
	ffio_wfourcc(pb, "ACLR");
	ffio_wfourcc(pb, "ACLR");
	ffio_wfourcc(pb, "0001");
	if (track->par->color_range == AVCOL_RANGE_MPEG || /* Legal range (16-235) */
		track->par->color_range == AVCOL_RANGE_UNSPECIFIED) {
		avio_wb32(pb, 1); /* Corresponds to 709 in official encoder */
	} else { /* Full range (0-255) */
		avio_wb32(pb, 2); /* Corresponds to RGB in official encoder */
	}
	avio_wb32(pb, 0); /* unknown */

	if (track->tag == MKTAG('A', 'V', 'd', 'h')) {
		avio_wb32(pb, 32);
		ffio_wfourcc(pb, "ADHR");
		ffio_wfourcc(pb, "0001");
		avio_wb32(pb, cid);
		avio_wb32(pb, 0); /* unknown */
		avio_wb32(pb, 1); /* unknown */
		avio_wb32(pb, 0); /* unknown */
		avio_wb32(pb, 0); /* unknown */
		return 0;
	}

	avio_wb32(pb, 24); /* size */
	ffio_wfourcc(pb, "APRG");
	ffio_wfourcc(pb, "APRG");
	ffio_wfourcc(pb, "0001");
	avio_wb32(pb, 1); /* unknown */
	avio_wb32(pb, 0); /* unknown */

	avio_wb32(pb, 120); /* size */
	ffio_wfourcc(pb, "ARES");
	ffio_wfourcc(pb, "ARES");
	ffio_wfourcc(pb, "0001");
	avio_wb32(pb, cid); /* dnxhd cid, some id ? */
	if (track->par->sample_aspect_ratio.num > 0
		&& track->par->sample_aspect_ratio.den > 0)
		display_width = display_width * track->par->sample_aspect_ratio.num /
			track->par->sample_aspect_ratio.den;
	avio_wb32(pb, display_width);
	/* values below are based on samples created with quicktime and avid codecs */
	if (interlaced) {
		avio_wb32(pb, track->par->height / 2);
		avio_wb32(pb, 2); /* unknown */
		avio_wb32(pb, 0); /* unknown */
		avio_wb32(pb, 4); /* unknown */
	} else {
		avio_wb32(pb, track->par->height);
		avio_wb32(pb, 1); /* unknown */
		avio_wb32(pb, 0); /* unknown */
		if (track->par->height == 1080)
			avio_wb32(pb, 5); /* unknown */
		else
			avio_wb32(pb, 6); /* unknown */
	}
	/* padding */
	for (i = 0; i < 10; i++)
		avio_wb64(pb, 0);

	return 0;
}

static int mov_write_dpxe_tag(AVIOContext *pb, MOVTrack *track)
{
	avio_wb32(pb, 12);
	ffio_wfourcc(pb, "DpxE");
	if (track->par->extradata_size >= 12 &&
		!memcmp(&track->par->extradata[4], "DpxE", 4)) {
		avio_wb32(pb, track->par->extradata[11]);
	} else {
		avio_wb32(pb, 1);
	}
	return 0;
}
#endif
static int mp4_get_codec_tag(AVFormatContext *s, MOVTrack *track)
{
	int tag = track->par->codec_tag;

	if (!ff_codec_get_tag(ff_mp4_obj_type, track->par->codec_id))
		return 0;

	if (track->par->codec_id == AV_CODEC_TYPE_H264)
		tag = MKTAG('a', 'v', 'c', '1');
	else if (track->par->codec_id == AV_CODEC_TYPE_ADPCM_IMA_WAV)
		tag = MKTAG('m', 's', 0x00, 0x11);
	else if (track->par->codec_id == AV_CODEC_TYPE_PCM_S16LE)
		tag = MKTAG('s', 'o', 'w', 't');
	else if (track->par->codec_type == AV_MEDIA_TYPE_VIDEO)
		tag = MKTAG('m', 'p', '4', 'v');
	else if (track->par->codec_type == AV_MEDIA_TYPE_AUDIO)
		tag = MKTAG('m', 'p', '4', 'a');

	return tag;
}

#if 0
static const AVCodecTag codec_ipod_tags[] = {
	{ AV_CODEC_TYPE_H264,     MKTAG('a', 'v', 'c', '1') },
	{ AV_CODEC_TYPE_MPEG4,    MKTAG('m', 'p', '4', 'v') },
	{ AV_CODEC_TYPE_AAC,      MKTAG('m', 'p', '4', 'a') },
	{ AV_CODEC_TYPE_ALAC,     MKTAG('a', 'l', 'a', 'c') },
	{ AV_CODEC_TYPE_AC3,      MKTAG('a', 'c', '-', '3') },
	{ AV_CODEC_TYPE_MOV_TEXT, MKTAG('t', 'x', '3', 'g') },
	{ AV_CODEC_TYPE_MOV_TEXT, MKTAG('t', 'e', 'x', 't') },
	{ AV_CODEC_TYPE_INVALID, 0 },
};

static int ipod_get_codec_tag(AVFormatContext *s, MOVTrack *track)
{
	int tag = track->par->codec_tag;

	/* keep original tag for subs, ipod supports both formats */
	if (!(track->par->codec_type == AV_MEDIA_TYPE_SUBTITLE &&
			(tag == MKTAG('t', 'x', '3', 'g') ||
				tag == MKTAG('t', 'e', 'x', 't'))))
		tag = ff_codec_get_tag(codec_ipod_tags, track->par->codec_id);

	if (!av_match_ext(s->filename, "m4a") &&
		!av_match_ext(s->filename, "m4b") &&
		!av_match_ext(s->filename, "m4v"))
		av_log(s, AV_LOG_WARNING,
			"Warning, extension is not .m4a, .m4v nor  .m4b "
			"Quicktime/Ipod might not play the file\n");

	return tag;
}

static int mov_get_dv_codec_tag(AVFormatContext *s, MOVTrack *track)
{
	int tag;

	if (track->par->width == 720) { /* SD */
		if (track->par->height == 480) { /* NTSC */
			if (track->par->format == AV_PIX_FMT_YUV422P)
				tag = MKTAG('d', 'v', '5', 'n');
			else
				tag = MKTAG('d', 'v', 'c', ' ');
		} else if (track->par->format == AV_PIX_FMT_YUV422P)
			tag = MKTAG('d', 'v', '5', 'p');
		else if (track->par->format == AV_PIX_FMT_YUV420P)
			tag = MKTAG('d', 'v', 'c', 'p');
		else
			tag = MKTAG('d', 'v', 'p', 'p');
	} else if (track->par->height == 720) { /* HD 720 line */
		if (track->st->time_base.den == 50)
			tag = MKTAG('d', 'v', 'h', 'q');
		else
			tag = MKTAG('d', 'v', 'h', 'p');
	} else if (track->par->height == 1080) { /* HD 1080 line */
		if (track->st->time_base.den == 25)
			tag = MKTAG('d', 'v', 'h', '5');
		else
			tag = MKTAG('d', 'v', 'h', '6');
	} else {
		av_log(s, AV_LOG_ERROR, "unsupported height for dv codec\n");
		return 0;
	}

	return tag;
}

static AV_Rational find_fps(AVFormatContext *s, AVStream *st)
{
	AV_Rational rate = st->avg_frame_rate;

#if FF_API_LAVF_AVCTX
	FF_DISABLE_DEPRECATION_WARNINGS
	rate = av_inv_q(st->codec->time_base);
	if (av_timecode_check_frame_rate(rate) < 0) {
		av_log(s, AV_LOG_DEBUG,
			"timecode: tbc=%d/%d invalid, fallback on %d/%d\n",
			rate.num, rate.den, st->avg_frame_rate.num,
			st->avg_frame_rate.den);
		rate = st->avg_frame_rate;
	}
	FF_ENABLE_DEPRECATION_WARNINGS
#endif

	return rate;
}

static int mov_get_mpeg2_xdcam_codec_tag(AVFormatContext *s, MOVTrack *track)
{
	int tag = track->par->codec_tag;
	int interlaced = track->par->field_order > AV_FIELD_PROGRESSIVE;
	AVStream *st = track->st;
	int rate = av_q2d(find_fps(s, st));

	if (!tag)
		tag = MKTAG('m', '2', 'v', '1'); /* fallback tag */

	if (track->par->format == AV_PIX_FMT_YUV420P) {
		if (track->par->width == 1280 && track->par->height == 720) {
			if (!interlaced) {
				if (rate == 24)
					tag = MKTAG('x', 'd', 'v', '4');
				else if (rate == 25)
					tag = MKTAG('x', 'd', 'v', '5');
				else if (rate == 30)
					tag = MKTAG('x', 'd', 'v', '1');
				else if (rate == 50)
					tag = MKTAG('x', 'd', 'v', 'a');
				else if (rate == 60)
					tag = MKTAG('x', 'd', 'v', '9');
			}
		} else if (track->par->width == 1440 && track->par->height == 1080) {
			if (!interlaced) {
				if (rate == 24)
					tag = MKTAG('x', 'd', 'v', '6');
				else if (rate == 25)
					tag = MKTAG('x', 'd', 'v', '7');
				else if (rate == 30)
					tag = MKTAG('x', 'd', 'v', '8');
			} else {
				if (rate == 25)
					tag = MKTAG('x', 'd', 'v', '3');
				else if (rate == 30)
					tag = MKTAG('x', 'd', 'v', '2');
			}
		} else if (track->par->width == 1920 && track->par->height == 1080) {
			if (!interlaced) {
				if (rate == 24)
					tag = MKTAG('x', 'd', 'v', 'd');
				else if (rate == 25)
					tag = MKTAG('x', 'd', 'v', 'e');
				else if (rate == 30)
					tag = MKTAG('x', 'd', 'v', 'f');
			} else {
				if (rate == 25)
					tag = MKTAG('x', 'd', 'v', 'c');
				else if (rate == 30)
					tag = MKTAG('x', 'd', 'v', 'b');
			}
		}
	} else if (track->par->format == AV_PIX_FMT_YUV422P) {
		if (track->par->width == 1280 && track->par->height == 720) {
			if (!interlaced) {
				if (rate == 24)
					tag = MKTAG('x', 'd', '5', '4');
				else if (rate == 25)
					tag = MKTAG('x', 'd', '5', '5');
				else if (rate == 30)
					tag = MKTAG('x', 'd', '5', '1');
				else if (rate == 50)
					tag = MKTAG('x', 'd', '5', 'a');
				else if (rate == 60)
					tag = MKTAG('x', 'd', '5', '9');
			}
		} else if (track->par->width == 1920 && track->par->height == 1080) {
			if (!interlaced) {
				if (rate == 24)
					tag = MKTAG('x', 'd', '5', 'd');
				else if (rate == 25)
					tag = MKTAG('x', 'd', '5', 'e');
				else if (rate == 30)
					tag = MKTAG('x', 'd', '5', 'f');
			} else {
				if (rate == 25)
					tag = MKTAG('x', 'd', '5', 'c');
				else if (rate == 30)
					tag = MKTAG('x', 'd', '5', 'b');
			}
		}
	}

	return tag;
}

static int mov_get_h264_codec_tag(AVFormatContext *s, MOVTrack *track)
{
	int tag = track->par->codec_tag;
	int interlaced = track->par->field_order > AV_FIELD_PROGRESSIVE;
	AVStream *st = track->st;
	int rate = av_q2d(find_fps(s, st));

	if (!tag)
		tag = MKTAG('a', 'v', 'c', 'i'); /* fallback tag */

	if (track->par->format == AV_PIX_FMT_YUV420P10) {
		if (track->par->width == 960 && track->par->height == 720) {
			if (!interlaced) {
				if (rate == 24)
					tag = MKTAG('a', 'i', '5', 'p');
				else if (rate == 25)
					tag = MKTAG('a', 'i', '5', 'q');
				else if (rate == 30)
					tag = MKTAG('a', 'i', '5', 'p');
				else if (rate == 50)
					tag = MKTAG('a', 'i', '5', 'q');
				else if (rate == 60)
					tag = MKTAG('a', 'i', '5', 'p');
			}
		} else if (track->par->width == 1440 && track->par->height == 1080) {
			if (!interlaced) {
				if (rate == 24)
					tag = MKTAG('a', 'i', '5', '3');
				else if (rate == 25)
					tag = MKTAG('a', 'i', '5', '2');
				else if (rate == 30)
					tag = MKTAG('a', 'i', '5', '3');
			} else {
				if (rate == 50)
					tag = MKTAG('a', 'i', '5', '5');
				else if (rate == 60)
					tag = MKTAG('a', 'i', '5', '6');
			}
		}
	} else if (track->par->format == AV_PIX_FMT_YUV422P10) {
		if (track->par->width == 1280 && track->par->height == 720) {
			if (!interlaced) {
				if (rate == 24)
					tag = MKTAG('a', 'i', '1', 'p');
				else if (rate == 25)
					tag = MKTAG('a', 'i', '1', 'q');
				else if (rate == 30)
					tag = MKTAG('a', 'i', '1', 'p');
				else if (rate == 50)
					tag = MKTAG('a', 'i', '1', 'q');
				else if (rate == 60)
					tag = MKTAG('a', 'i', '1', 'p');
			}
		} else if (track->par->width == 1920 && track->par->height == 1080) {
			if (!interlaced) {
				if (rate == 24)
					tag = MKTAG('a', 'i', '1', '3');
				else if (rate == 25)
					tag = MKTAG('a', 'i', '1', '2');
				else if (rate == 30)
					tag = MKTAG('a', 'i', '1', '3');
			} else {
				if (rate == 25)
					tag = MKTAG('a', 'i', '1', '5');
				else if (rate == 50)
					tag = MKTAG('a', 'i', '1', '5');
				else if (rate == 60)
					tag = MKTAG('a', 'i', '1', '6');
			}
		} else if (track->par->width == 4096 && track->par->height == 2160
			|| track->par->width == 3840 && track->par->height == 2160
			|| track->par->width == 2048 && track->par->height == 1080) {
			tag = MKTAG('a', 'i', 'v', 'x');
		}
	}

	return tag;
}

static const struct {
	enum AVPixelFormat pix_fmt;
	uint32_t tag;
	unsigned bps;
} mov_pix_fmt_tags[] = {
	{ AV_PIX_FMT_YUYV422, MKTAG('y', 'u', 'v', '2'),  0 },
	{ AV_PIX_FMT_YUYV422, MKTAG('y', 'u', 'v', 's'),  0 },
	{ AV_PIX_FMT_UYVY422, MKTAG('2', 'v', 'u', 'y'),  0 },
	{ AV_PIX_FMT_RGB555BE, MKTAG('r', 'a', 'w', ' '), 16 },
	{ AV_PIX_FMT_RGB555LE, MKTAG('L', '5', '5', '5'), 16 },
	{ AV_PIX_FMT_RGB565LE, MKTAG('L', '5', '6', '5'), 16 },
	{ AV_PIX_FMT_RGB565BE, MKTAG('B', '5', '6', '5'), 16 },
	{ AV_PIX_FMT_GRAY16BE, MKTAG('b', '1', '6', 'g'), 16 },
	{ AV_PIX_FMT_RGB24,   MKTAG('r', 'a', 'w', ' '), 24 },
	{ AV_PIX_FMT_BGR24,   MKTAG('2', '4', 'B', 'G'), 24 },
	{ AV_PIX_FMT_ARGB,    MKTAG('r', 'a', 'w', ' '), 32 },
	{ AV_PIX_FMT_BGRA,    MKTAG('B', 'G', 'R', 'A'), 32 },
	{ AV_PIX_FMT_RGBA,    MKTAG('R', 'G', 'B', 'A'), 32 },
	{ AV_PIX_FMT_ABGR,    MKTAG('A', 'B', 'G', 'R'), 32 },
	{ AV_PIX_FMT_RGB48BE, MKTAG('b', '4', '8', 'r'), 48 },
};

static int mov_get_dnxhd_codec_tag(AVFormatContext *s, MOVTrack *track)
{
	int tag = MKTAG('A', 'V', 'd', 'n');
	if (track->par->profile != FF_PROFILE_UNKNOWN &&
		track->par->profile != FF_PROFILE_DNXHD)
		tag = MKTAG('A', 'V', 'd', 'h');
	return tag;
}

static int mov_get_rawvideo_codec_tag(AVFormatContext *s, MOVTrack *track)
{
	int tag = track->par->codec_tag;
	int i;
	enum AVPixelFormat pix_fmt;

	for (i = 0; i < FF_ARRAY_ELEMS(mov_pix_fmt_tags); i++) {
		if (track->par->format == mov_pix_fmt_tags[i].pix_fmt) {
			tag = mov_pix_fmt_tags[i].tag;
			track->par->bits_per_coded_sample = mov_pix_fmt_tags[i].bps;
			if (track->par->codec_tag == mov_pix_fmt_tags[i].tag)
				break;
		}
	}

	pix_fmt = avpriv_find_pix_fmt(avpriv_pix_fmt_bps_mov,
			track->par->bits_per_coded_sample);
	if (tag == MKTAG('r', 'a', 'w', ' ') &&
		track->par->format != pix_fmt &&
		track->par->format != AV_PIX_FMT_NONE)
		av_log(s, AV_LOG_ERROR,
			"%s rawvideo cannot be written to mov, output file will be unreadable\n",
			av_get_pix_fmt_name(track->par->format));
	return tag;
}

static int mov_get_codec_tag(AVFormatContext *s, MOVTrack *track)
{
	int tag = track->par->codec_tag;

	if (!tag || (s->strict_std_compliance >= FF_COMPLIANCE_NORMAL &&
			(track->par->codec_id == AV_CODEC_TYPE_DVVIDEO ||
				track->par->codec_id == AV_CODEC_TYPE_RAWVIDEO ||
				track->par->codec_id == AV_CODEC_TYPE_H263 ||
				track->par->codec_id == AV_CODEC_TYPE_H264 ||
				track->par->codec_id == AV_CODEC_TYPE_DNXHD ||
				track->par->codec_id == AV_CODEC_TYPE_MPEG2VIDEO ||
				av_get_bits_per_sample(track->par->codec_id)))) {
		/* pcm audio */
		if (track->par->codec_id == AV_CODEC_TYPE_DVVIDEO)
			tag = mov_get_dv_codec_tag(s, track);
		else if (track->par->codec_id == AV_CODEC_TYPE_RAWVIDEO)
			tag = mov_get_rawvideo_codec_tag(s, track);
		else if (track->par->codec_id == AV_CODEC_TYPE_MPEG2VIDEO)
			tag = mov_get_mpeg2_xdcam_codec_tag(s, track);
		else if (track->par->codec_id == AV_CODEC_TYPE_H264)
			tag = mov_get_h264_codec_tag(s, track);
		else if (track->par->codec_id == AV_CODEC_TYPE_DNXHD)
			tag = mov_get_dnxhd_codec_tag(s, track);
		else if (track->par->codec_type == AV_MEDIA_TYPE_VIDEO) {
			tag = ff_codec_get_tag(ff_codec_movvideo_tags,
					track->par->codec_id);
			if (!tag) { /* if no mac fcc found, try with Microsoft tags */
				tag = ff_codec_get_tag(ff_codec_bmp_tags,
						track->par->codec_id);
				if (tag)
					av_log(s, AV_LOG_WARNING,
						"Using MS style video codec tag, "
						"the file may be unplayable!\n");
			}
		} else if (track->par->codec_type == AV_MEDIA_TYPE_AUDIO) {
			tag = ff_codec_get_tag(ff_codec_movaudio_tags,
					track->par->codec_id);
			if (!tag) { /* if no mac fcc found, try with Microsoft tags */
				int ms_tag = ff_codec_get_tag(ff_codec_wav_tags,
						track->par->codec_id);
				if (ms_tag) {
					tag = MKTAG('m', 's', ((ms_tag >> 8) & 0xff),
							(ms_tag & 0xff));
					av_log(s, AV_LOG_WARNING,
						"Using MS style audio codec tag, "
						"the file may be unplayable!\n");
				}
			}
		} else if (track->par->codec_type == AV_MEDIA_TYPE_SUBTITLE)
			tag = ff_codec_get_tag(ff_codec_movsubtitle_tags,
					track->par->codec_id);
	}

	return tag;
}

static const AVCodecTag codec_3gp_tags[] = {
	{ AV_CODEC_TYPE_H263,     MKTAG('s', '2', '6', '3') },
	{ AV_CODEC_TYPE_H264,     MKTAG('a', 'v', 'c', '1') },
	{ AV_CODEC_TYPE_MPEG4,    MKTAG('m', 'p', '4', 'v') },
	{ AV_CODEC_TYPE_AAC,      MKTAG('m', 'p', '4', 'a') },
	{ AV_CODEC_TYPE_AMR_NB,   MKTAG('s', 'a', 'm', 'r') },
	{ AV_CODEC_TYPE_AMR_WB,   MKTAG('s', 'a', 'w', 'b') },
	{ AV_CODEC_TYPE_MOV_TEXT, MKTAG('t', 'x', '3', 'g') },
	{ AV_CODEC_TYPE_INVALID, 0 },
};

static const AVCodecTag codec_f4v_tags[] = { /* XXX: add GIF/PNG/JPEG? */
	{ AV_CODEC_TYPE_MP3,    MKTAG('.', 'm', 'p', '3') },
	{ AV_CODEC_TYPE_AAC,    MKTAG('m', 'p', '4', 'a') },
	{ AV_CODEC_TYPE_H264,   MKTAG('a', 'v', 'c', '1') },
	{ AV_CODEC_TYPE_VP6A,   MKTAG('V', 'P', '6', 'A') },
	{ AV_CODEC_TYPE_VP6F,   MKTAG('V', 'P', '6', 'F') },
	{ AV_CODEC_TYPE_INVALID, 0 },
};

static int mov_find_codec_tag(AVFormatContext *s, MOVTrack *track)
{
	int tag;

	if (track->mode == MODE_MP4 || track->mode == MODE_PSP)
		tag = mp4_get_codec_tag(s, track);
	else if (track->mode == MODE_ISM) {
		tag = mp4_get_codec_tag(s, track);
		if (!tag && track->par->codec_id == AV_CODEC_TYPE_WMAPRO)
			tag = MKTAG('w', 'm', 'a', ' ');
	} else if (track->mode == MODE_IPOD)
		tag = ipod_get_codec_tag(s, track);
	else if (track->mode & MODE_3GP)
		tag = ff_codec_get_tag(codec_3gp_tags, track->par->codec_id);
	else if (track->mode == MODE_F4V)
		tag = ff_codec_get_tag(codec_f4v_tags, track->par->codec_id);
	else
		tag = mov_get_codec_tag(s, track);

	return tag;
}

/** Write uuid atom.
 * Needed to make file play in iPods running newest firmware
 * goes after avcC atom in moov.trak.mdia.minf.stbl.stsd.avc1
 */
static int mov_write_uuid_tag_ipod(AVIOContext *pb)
{
	avio_wb32(pb, 28);
	ffio_wfourcc(pb, "uuid");
	avio_wb32(pb, 0x6b6840f2);
	avio_wb32(pb, 0x5f244fc5);
	avio_wb32(pb, 0xba39a51b);
	avio_wb32(pb, 0xcf0323f3);
	avio_wb32(pb, 0x0);
	return 28;
}

static const uint16_t fiel_data[] = {
	0x0000, 0x0100, 0x0201, 0x0206, 0x0209, 0x020e
};

static int mov_write_fiel_tag(AVIOContext *pb, MOVTrack *track, int field_order)
{
	unsigned mov_field_order = 0;
	if (field_order < FF_ARRAY_ELEMS(fiel_data))
		mov_field_order = fiel_data[field_order];
	else
		return 0;
	avio_wb32(pb, 10);
	ffio_wfourcc(pb, "fiel");
	avio_wb16(pb, mov_field_order);
	return 10;
}

static int mov_write_subtitle_tag(AVIOContext *pb, MOVTrack *track)
{
	int64_t pos = avio_tell(pb);
	avio_wb32(pb, 0);    /* size */
	avio_wl32(pb, track->tag); /* store it byteswapped */
	avio_wb32(pb, 0);    /* Reserved */
	avio_wb16(pb, 0);    /* Reserved */
	avio_wb16(pb, 1);    /* Data-reference index */

	if (track->par->codec_id == AV_CODEC_TYPE_DVD_SUBTITLE)
		mov_write_esds_tag(pb, track);
	else if (track->par->extradata_size)
		avio_write(pb, track->par->extradata, track->par->extradata_size);

	return update_size(pb, pos);
}

static int mov_write_pasp_tag(AVIOContext *pb, MOVTrack *track)
{
	AV_Rational sar;
	av_reduce(&sar.num, &sar.den, track->par->sample_aspect_ratio.num,
		track->par->sample_aspect_ratio.den, INT_MAX);

	avio_wb32(pb, 16);
	ffio_wfourcc(pb, "pasp");
	avio_wb32(pb, sar.num);
	avio_wb32(pb, sar.den);
	return 16;
}

static int mov_write_gama_tag(AVIOContext *pb, MOVTrack *track, double gamma)
{
	uint32_t gama = 0;
	if (gamma <= 0.0)
		gamma = avpriv_get_gamma_from_trc(track->par->color_trc);

	av_log(pb, AV_LOG_DEBUG, "gamma value %g\n", gamma);

	if (gamma > 1e-6) {
		gama = (uint32_t)lrint((double)(1 << 16) * gamma);
		av_log(pb, AV_LOG_DEBUG, "writing gama value %d\n", gama);

		av_assert0(track->mode == MODE_MOV);
		avio_wb32(pb, 12);
		ffio_wfourcc(pb, "gama");
		avio_wb32(pb, gama);
		return 12;
	} else {
		av_log(pb, AV_LOG_WARNING,
			"gamma value unknown, unable to write gama atom\n");
	}
	return 0;
}

static int mov_write_colr_tag(AVIOContext *pb, MOVTrack *track)
{
	/* Ref (MOV): https://developer.apple.com/library/mac/technotes/
		tn2162/_index.html#//apple_ref/doc/uid/DTS40013070-CH1-TNTAG9
	 Ref (MP4): ISO/IEC 14496-12:2012 */

	if (track->par->color_primaries == AVCOL_PRI_UNSPECIFIED &&
		track->par->color_trc == AVCOL_TRC_UNSPECIFIED &&
		track->par->color_space == AVCOL_SPC_UNSPECIFIED) {
		if ((track->par->width >= 1920 && track->par->height >= 1080)
			|| (track->par->width == 1280 && track->par->height == 720)) {
			av_log(NULL, AV_LOG_WARNING,
				"color primaries unspecified, assuming bt709\n");
			track->par->color_primaries = AVCOL_PRI_BT709;
		} else if (track->par->width == 720 && track->height == 576) {
			av_log(NULL, AV_LOG_WARNING,
				"color primaries unspecified, assuming bt470bg\n");
			track->par->color_primaries = AVCOL_PRI_BT470BG;
		} else if (track->par->width == 720 &&
			(track->height == 486 || track->height == 480)) {
			av_log(NULL, AV_LOG_WARNING,
				"color primaries unspecified, assuming smpte170\n");
			track->par->color_primaries = AVCOL_PRI_SMPTE170M;
		} else {
			av_log(NULL, AV_LOG_WARNING,
				"color primaries unspecified, unable to assume anything\n");
		}
		switch (track->par->color_primaries) {
		case AVCOL_PRI_BT709:
			track->par->color_trc = AVCOL_TRC_BT709;
			track->par->color_space = AVCOL_SPC_BT709;
			break;
		case AVCOL_PRI_SMPTE170M:
		case AVCOL_PRI_BT470BG:
			track->par->color_trc = AVCOL_TRC_BT709;
			track->par->color_space = AVCOL_SPC_SMPTE170M;
			break;
		}
	}

	/* We should only ever be called by MOV or MP4. */
	av_assert0(track->mode == MODE_MOV || track->mode == MODE_MP4);

	avio_wb32(pb, 18 + (track->mode == MODE_MP4));
	ffio_wfourcc(pb, "colr");
	if (track->mode == MODE_MP4)
		ffio_wfourcc(pb, "nclx");
	else
		ffio_wfourcc(pb, "nclc");
	switch (track->par->color_primaries) {
	case AVCOL_PRI_BT709:
		avio_wb16(pb, 1);
		break;
	case AVCOL_PRI_SMPTE170M:
	case AVCOL_PRI_SMPTE240M:
		avio_wb16(pb, 6);
		break;
	case AVCOL_PRI_BT470BG:
		avio_wb16(pb, 5);
		break;
	default:
		avio_wb16(pb, 2);
	}
	switch (track->par->color_trc) {
	case AVCOL_TRC_BT709:
		avio_wb16(pb, 1);
		break;
	case AVCOL_TRC_SMPTE170M:
		avio_wb16(pb, 1);
		break; /* remapped */
	case AVCOL_TRC_SMPTE240M:
		avio_wb16(pb, 7);
		break;
	default:
		avio_wb16(pb, 2);
	}
	switch (track->par->color_space) {
	case AVCOL_SPC_BT709:
		avio_wb16(pb, 1);
		break;
	case AVCOL_SPC_BT470BG:
	case AVCOL_SPC_SMPTE170M:
		avio_wb16(pb, 6);
		break;
	case AVCOL_SPC_SMPTE240M:
		avio_wb16(pb, 7);
		break;
	default:
		avio_wb16(pb, 2);
	}

	if (track->mode == MODE_MP4) {
		int full_range = track->par->color_range == AVCOL_RANGE_JPEG;
		avio_w8(pb, full_range << 7);
		return 19;
	} else {
		return 18;
	}
}

static void find_compressor(char *compressor_name, int len, MOVTrack *track)
{
	AVDictionaryEntry *encoder;
	int xdcam_res = (track->par->width == 1280 && track->par->height == 720)
		|| (track->par->width == 1440 && track->par->height == 1080)
		|| (track->par->width == 1920 && track->par->height == 1080);

	/*	if (track->mode == MODE_MOV &&
			(encoder = av_dict_get(track->st->metadata, "encoder", NULL, 0)))
			av_strlcpy(compressor_name, encoder->value, 32);*/
	if (track->mode == MODE_MOV) {
		encoder = av_dict_get(track->st->metadata, "encoder", NULL, 0);
		if (encoder)
			av_strlcpy(compressor_name, encoder->value, 32);
	} else if (track->par->codec_id == AV_CODEC_TYPE_MPEG2VIDEO && xdcam_res) {
		int interlaced = track->par->field_order > AV_FIELD_PROGRESSIVE;
		AVStream *st = track->st;
		int rate = av_q2d(find_fps(NULL, st));
		av_strlcatf(compressor_name, len, "XDCAM");
		if (track->par->format == AV_PIX_FMT_YUV422P)
			av_strlcatf(compressor_name, len, " HD422");
		else if (track->par->width == 1440)
			av_strlcatf(compressor_name, len, " HD");
		else
			av_strlcatf(compressor_name, len, " EX");

		av_strlcatf(compressor_name, len, " %d%c",
			track->par->height, interlaced ? 'i' : 'p');

		av_strlcatf(compressor_name, len, "%d", rate * (interlaced + 1));
	}
}
#endif

static int mov_write_video_tag(AVIOContext *pb, MOVMuxContext *mov, MOVTrack *track)
{
	int64_t pos = avio_tell(pb);
	char compressor_name[32] = { 0 };

	avio_wb32(pb, 0); /* size */
	avio_wl32(pb, track->tag); /* store it byteswapped */
	avio_wb32(pb, 0); /* Reserved */
	avio_wb16(pb, 0); /* Reserved */
	avio_wb16(pb, 1); /* Data-reference index */

	avio_wb16(pb, 0); /* Codec stream version */
	avio_wb16(pb, 0); /* Codec stream revision (=0) */
	if (track->mode == MODE_MOV) {
		ffio_wfourcc(pb, "FFMP"); /* Vendor */
		avio_wb32(pb, 0x200); /* Temporal Quality = normal */
		avio_wb32(pb, 0x200); /* Spatial Quality = normal */
	} else {
		avio_wb32(pb, 0); /* Reserved */
		avio_wb32(pb, 0); /* Reserved */
		avio_wb32(pb, 0); /* Reserved */
	}
	avio_wb16(pb, track->par->width); /* Video width */
	avio_wb16(pb, track->height); /* Video height */
	avio_wb32(pb, 0x00480000); /* Horizontal resolution 72dpi */
	avio_wb32(pb, 0x00480000); /* Vertical resolution 72dpi */
	avio_wb32(pb, 0); /* Data size (= 0) */
	avio_wb16(pb, 1); /* Frame count (= 1) */

	/* FIXME not sure, ISO 14496-1 draft where it shall be set to 0 */
	/*find_compressor(compressor_name, 32, track); */
	avio_w8(pb, strlen(compressor_name));
	avio_write(pb, (unsigned char *)compressor_name, 31);

	if (track->mode == MODE_MOV && track->par->bits_per_coded_sample)
		avio_wb16(pb, track->par->bits_per_coded_sample);
	else
		avio_wb16(pb, 0x18); /* Reserved */

	avio_wb16(pb, 0xffff); /* Reserved */

	if (track->tag == MKTAG('m', 'p', '4', 'v'))
		mov_write_esds_tag(pb, track);
	else if (track->par->codec_id == AV_CODEC_TYPE_H264)
		mov_write_avcc_tag(pb, track);

	return update_size(pb, pos);
}

#if 0
static int mov_write_rtp_tag(AVIOContext *pb, MOVTrack *track)
{
	int64_t pos = avio_tell(pb);
	avio_wb32(pb, 0); /* size */
	ffio_wfourcc(pb, "rtp ");
	avio_wb32(pb, 0); /* Reserved */
	avio_wb16(pb, 0); /* Reserved */
	avio_wb16(pb, 1); /* Data-reference index */

	avio_wb16(pb, 1); /* Hint track version */
	avio_wb16(pb, 1); /* Highest compatible version */
	avio_wb32(pb, track->max_packet_size); /* Max packet size */

	avio_wb32(pb, 12); /* size */
	ffio_wfourcc(pb, "tims");
	avio_wb32(pb, track->timescale);

	return update_size(pb, pos);
}

static int mov_write_source_reference_tag(AVIOContext *pb, MOVTrack *track,
	const char *reel_name)
{
	uint64_t str_size = strlen(reel_name);
	int64_t pos = avio_tell(pb);

	if (str_size >= UINT16_MAX) {
		av_log(NULL, AV_LOG_ERROR,
			"reel_name length %"PRIu64" is too large\n", str_size);
		avio_wb16(pb, 0);
		return AVERROR(EINVAL);
	}

	avio_wb32(pb, 0);                              /* size */
	ffio_wfourcc(pb, "name");                      /* Data format */
	avio_wb16(pb, str_size);                       /* string size */
	avio_wb16(pb, track->language);                /* langcode */
	avio_write(pb, reel_name, str_size);           /* reel name */
	return update_size(pb, pos);
}

static int mov_write_tmcd_tag(AVIOContext *pb, MOVTrack *track)
{
	int64_t pos = avio_tell(pb);
#if 1
	int frame_duration;
	int nb_frames;
	AVDictionaryEntry *t = NULL;

	if (!track->st->avg_frame_rate.num || !track->st->avg_frame_rate.den) {
#if FF_API_LAVF_AVCTX
		FF_DISABLE_DEPRECATION_WARNINGS
		frame_duration = av_rescale(track->timescale,
				track->st->codec->time_base.num,
				track->st->codec->time_base.den);
		nb_frames      = ROUNDED_DIV(track->st->codec->time_base.den,
				track->st->codec->time_base.num);
		FF_ENABLE_DEPRECATION_WARNINGS
#else
		av_log(NULL, AV_LOG_ERROR, "avg_frame_rate not set for tmcd track.\n");
		return AVERROR(EINVAL);
#endif
	} else {
		frame_duration = av_rescale(track->timescale,
				track->st->avg_frame_rate.num,
				track->st->avg_frame_rate.den);
		nb_frames      = ROUNDED_DIV(track->st->avg_frame_rate.den,
				track->st->avg_frame_rate.num);
	}

	if (nb_frames > 255) {
		av_log(NULL, AV_LOG_ERROR, "fps %d is too large\n", nb_frames);
		return AVERROR(EINVAL);
	}

	avio_wb32(pb, 0); /* size */
	ffio_wfourcc(pb, "tmcd");               /* Data format */
	avio_wb32(pb, 0);                       /* Reserved */
	avio_wb32(pb, 1);                       /* Data reference index */
	avio_wb32(pb, 0);                       /* Flags */
	avio_wb32(pb, track->timecode_flags);   /* Flags (timecode) */
	avio_wb32(pb, track->timescale);        /* Timescale */
	avio_wb32(pb, frame_duration);          /* Frame duration */
	avio_w8(pb, nb_frames);                 /* Number of frames */
	avio_w8(pb, 0);                         /* Reserved */

	t = av_dict_get(track->st->metadata, "reel_name", NULL, 0);
	if (t && utf8len(t->value) && track->mode != MODE_MP4)
		mov_write_source_reference_tag(pb, track, t->value);
	else
		avio_wb16(pb, 0); /* zero size */
#else

	avio_wb32(pb, 0); /* size */
	ffio_wfourcc(pb, "tmcd");               /* Data format */
	avio_wb32(pb, 0);                       /* Reserved */
	avio_wb32(pb, 1);                       /* Data reference index */
	if (track->par->extradata_size)
		avio_write(pb, track->par->extradata, track->par->extradata_size);
#endif
	return update_size(pb, pos);
}
#endif

static int mov_write_stsd_tag(AVFormatContext *s, AVIOContext *pb, MOVMuxContext *mov,
	MOVTrack *track)
{
	int64_t pos = avio_tell(pb);
	avio_wb32(pb, 0); /* size */
	ffio_wfourcc(pb, "stsd");
	avio_wb32(pb, 0); /* version & flags */
	avio_wb32(pb, 1); /* entry count */
	if (track->par->codec_type == AV_MEDIA_TYPE_VIDEO)
		mov_write_video_tag(pb, mov, track);
	else if (track->par->codec_type == AV_MEDIA_TYPE_AUDIO)
		mov_write_audio_tag(s, pb, mov, track);

	return update_size(pb, pos);
}

static int mov_write_ctts_tag(AVIOContext *pb, MOVTrack *track)
{
	uint32_t entries = 0;
	uint32_t atom_size;
	int i;
	int num = 0;
	entries = track->clust.framectts;
	atom_size = 16 + (entries * 8);
	avio_wb32(pb, atom_size); /* size */
	ffio_wfourcc(pb, "ctts");
	avio_wb32(pb, 0); /* version & flags */
	avio_wb32(pb, entries); /* entry count */
	num = track->clust.framectts / CTTS_NUM;
	num *= CTTS_NUM / CHUNK_NUM;
	num *= 2;
	if (track->clust.cttsfile) {
		int ret = CHUNK_NUM;
		fseek(track->clust.cttsfile, 0, 0);
		for (i = 0; i < num; i++) {
			int j = 0;
			ret = fread(track->clust.cache, 4, CHUNK_NUM,
					track->clust.cttsfile);
			if (ret != CHUNK_NUM)
				printf("read cache err, ret=%d!", ret);
			for (j = 0; j < ret; j++)
				avio_wb32(pb, track->clust.cache[j]);

		}
		if (ret != CHUNK_NUM) {
			avio_wb32(pb, track->clust.ctts[CTTS_NUM - 1].count);
			avio_wb32(pb, track->clust.ctts[CTTS_NUM - 1].duration);
		}
	}

	entries &= CTTS_NUM - 1;
	for (i = 0; i < entries; i++) {
		avio_wb32(pb, track->clust.ctts[i].count);
		avio_wb32(pb, track->clust.ctts[i].duration);
	}
	return atom_size;
}

/* Time to sample atom */
static int mov_write_stts_tag(AVIOContext *pb, MOVTrack *track)
{
	uint32_t entries = -1;
	uint32_t atom_size;
	int i;
	entries = track->clust.framestts;

	if (track->par->codec_type == AV_MEDIA_TYPE_AUDIO && !track->audio_vbr) {
		track->clust.stts[0].count = track->sample_count;
		track->clust.stts[0].duration = 1;
		entries = 1;
	}
	atom_size = 16 + (entries * 8);
	avio_wb32(pb, atom_size); /* size */
	ffio_wfourcc(pb, "stts");
	avio_wb32(pb, 0); /* version & flags */
	avio_wb32(pb, entries); /* entry count */
	if (track->clust.sttsfile) {
		int ret = CHUNK_NUM;
		int num = entries / CHUNK_NUM;
		num *= 2;
		fseek(track->clust.sttsfile, 0, 0);
		for (i = 0; i < num; i++) {
			int j = 0;
			ret = fread(track->clust.cache, 4, CHUNK_NUM,
					track->clust.sttsfile);
			for (j = 0; j < ret; j++)
				avio_wb32(pb, track->clust.cache[j]);

		}
		if (ret != CHUNK_NUM) {
			avio_wb32(pb, track->clust.stts[CHUNK_NUM - 1].count);
			avio_wb32(pb, track->clust.stts[CHUNK_NUM - 1].duration);
		}
	}
	entries &= CHUNK_NUM - 1;
	for (i = 0; i < entries; i++) {
		avio_wb32(pb, track->clust.stts[i].count);
		avio_wb32(pb, track->clust.stts[i].duration);
	}
	return atom_size;
}

static int mov_write_dref_tag(AVIOContext *pb)
{
	avio_wb32(pb, 28); /* size */
	ffio_wfourcc(pb, "dref");
	avio_wb32(pb, 0); /* version & flags */
	avio_wb32(pb, 1); /* entry count */

	avio_wb32(pb, 0xc); /* size */
	/* FIXME add the alis and rsrc atom */
	ffio_wfourcc(pb, "url ");
	avio_wb32(pb, 1); /* version & flags */

	return 28;
}

static int mov_write_stbl_tag(AVFormatContext *s, AVIOContext *pb, MOVMuxContext *mov,
	MOVTrack *track)
{
	int64_t pos = avio_tell(pb);
	int ret;

	avio_wb32(pb, 0); /* size */
	ffio_wfourcc(pb, "stbl");
	mov_write_stsd_tag(s, pb, mov, track);
	mov_write_stts_tag(pb, track);
	if ((track->par->codec_type == AV_MEDIA_TYPE_VIDEO) &&
		track->has_keyframes && track->has_keyframes < track->entry)
		mov_write_stss_tag(pb, track);
	if (track->par->codec_type == AV_MEDIA_TYPE_VIDEO &&
		track->flags & MOV_TRACK_CTTS && track->entry) {

		ret = mov_write_ctts_tag(pb, track);
		if (ret < 0)
			return ret;
	}
	mov_write_stsc_tag(pb, track);
	mov_write_stsz_tag(pb, track);
	mov_write_stco_tag(pb, track);

	return update_size(pb, pos);
}

static int mov_write_dinf_tag(AVIOContext *pb)
{
	/* int64_t pos = avio_tell(pb); */
	avio_wb32(pb, 8 + 28); /* size */
	ffio_wfourcc(pb, "dinf");
	mov_write_dref_tag(pb);
	return 0; /* update_size(pb, pos); */
}

#if 0
static int mov_write_nmhd_tag(AVIOContext *pb)
{
	avio_wb32(pb, 12);
	ffio_wfourcc(pb, "nmhd");
	avio_wb32(pb, 0);
	return 12;
}

static int mov_write_tcmi_tag(AVIOContext *pb, MOVTrack *track)
{
	int64_t pos = avio_tell(pb);
	const char *font = "Lucida Grande";
	avio_wb32(pb, 0);                   /* size */
	ffio_wfourcc(pb, "tcmi");           /* timecode media information atom */
	avio_wb32(pb, 0);                   /* version & flags */
	avio_wb16(pb, 0);                   /* text font */
	avio_wb16(pb, 0);                   /* text face */
	avio_wb16(pb, 12);                  /* text size */
	avio_wb16(pb, 0);                   /* (unknown, not in the QT specs...) */
	avio_wb16(pb, 0x0000);              /* text color (red) */
	avio_wb16(pb, 0x0000);              /* text color (green) */
	avio_wb16(pb, 0x0000);              /* text color (blue) */
	avio_wb16(pb, 0xffff);              /* background color (red) */
	avio_wb16(pb, 0xffff);              /* background color (green) */
	avio_wb16(pb, 0xffff);              /* background color (blue) */
	avio_w8(pb, strlen(font));          /* font len (part of the pascal string) */
	avio_write(pb, font, strlen(font)); /* font name */
	return update_size(pb, pos);
}

static int mov_write_gmhd_tag(AVIOContext *pb, MOVTrack *track)
{
	int64_t pos = avio_tell(pb);
	avio_wb32(pb, 0);      /* size */
	ffio_wfourcc(pb, "gmhd");
	avio_wb32(pb, 0x18);   /* gmin size */
	ffio_wfourcc(pb, "gmin");/* generic media info */
	avio_wb32(pb, 0);      /* version & flags */
	avio_wb16(pb, 0x40);   /* graphics mode = */
	avio_wb16(pb, 0x8000); /* opColor (r?) */
	avio_wb16(pb, 0x8000); /* opColor (g?) */
	avio_wb16(pb, 0x8000); /* opColor (b?) */
	avio_wb16(pb, 0);      /* balance */
	avio_wb16(pb, 0);      /* reserved */

	/*
	 * This special text atom is required for
	 * Apple Quicktime chapters. The contents
	 * don't appear to be documented, so the
	 * bytes are copied verbatim.
	 */
	if (track->tag != MKTAG('c', '6', '0', '8')) {
		avio_wb32(pb, 0x2C);   /* size */
		ffio_wfourcc(pb, "text");
		avio_wb16(pb, 0x01);
		avio_wb32(pb, 0x00);
		avio_wb32(pb, 0x00);
		avio_wb32(pb, 0x00);
		avio_wb32(pb, 0x01);
		avio_wb32(pb, 0x00);
		avio_wb32(pb, 0x00);
		avio_wb32(pb, 0x00);
		avio_wb32(pb, 0x00004000);
		avio_wb16(pb, 0x0000);
	}

	if (track->par->codec_tag == MKTAG('t', 'm', 'c', 'd')) {
		int64_t tmcd_pos = avio_tell(pb);
		avio_wb32(pb, 0); /* size */
		ffio_wfourcc(pb, "tmcd");
		mov_write_tcmi_tag(pb, track);
		update_size(pb, tmcd_pos);
	}
	return update_size(pb, pos);
}
#endif
static int mov_write_smhd_tag(AVIOContext *pb)
{
	avio_wb32(pb, 16); /* size */
	ffio_wfourcc(pb, "smhd");
	avio_wb32(pb, 0); /* version & flags */
	avio_wb16(pb, 0); /* reserved (balance, normally = 0) */
	avio_wb16(pb, 0); /* reserved */
	return 16;
}

static int mov_write_vmhd_tag(AVIOContext *pb)
{
	avio_wb32(pb, 0x14); /* size (always 0x14) */
	ffio_wfourcc(pb, "vmhd");
	avio_wb32(pb, 0x01); /* version & flags */
	avio_wb64(pb, 0); /* reserved (graphics mode = copy) */
	return 0x14;
}

/*static int is_clcp_track(MOVTrack *track)
{
    return track->tag == MKTAG('c','7','0','8') ||
			track->tag == MKTAG('c','6','0','8');
}*/

static int mov_write_hdlr_tag(AVFormatContext *s, AVIOContext *pb, MOVTrack *track)
{
	const char *hdlr, *descr = NULL, *hdlr_type = NULL;
	/* int64_t pos = avio_tell(pb); */

	hdlr = "dhlr";
	hdlr_type = "url ";
	descr = "DataHandler";

	if (track) {
		hdlr = (track->mode == MODE_MOV) ? "mhlr" : "\0\0\0\0";
		if (track->par->codec_type == AV_MEDIA_TYPE_VIDEO) {
			hdlr_type = "vide";
			descr = "VideoHandler";
		} else if (track->par->codec_type == AV_MEDIA_TYPE_AUDIO) {
			hdlr_type = "soun";
			descr = "SoundHandler";
		}
	}

	avio_wb32(pb, 32 + 1 + strlen(descr)); /* size */
	ffio_wfourcc(pb, "hdlr");
	avio_wb32(pb, 0); /* Version & flags */
	avio_write(pb, (unsigned char *)hdlr, 4); /* handler */
	ffio_wfourcc(pb, hdlr_type); /* handler type */
	avio_wb32(pb, 0); /* reserved */
	avio_wb32(pb, 0); /* reserved */
	avio_wb32(pb, 0); /* reserved */
	if (!track || track->mode == MODE_MOV)
		avio_w8(pb, strlen(descr)); /* pascal string */
	avio_write(pb, (unsigned char *)descr, strlen(descr)); /* handler description */
	if (track && track->mode != MODE_MOV)
		avio_w8(pb, 0); /* c string */
	return 0; /*  update_size(pb, pos); */
}

#if 0
static int mov_write_hmhd_tag(AVIOContext *pb)
{
	/* This atom must be present, but leaving the values at zero
	 * seems harmless. */
	avio_wb32(pb, 28); /* size */
	ffio_wfourcc(pb, "hmhd");
	avio_wb32(pb, 0); /* version, flags */
	avio_wb16(pb, 0); /* maxPDUsize */
	avio_wb16(pb, 0); /* avgPDUsize */
	avio_wb32(pb, 0); /* maxbitrate */
	avio_wb32(pb, 0); /* avgbitrate */
	avio_wb32(pb, 0); /* reserved */
	return 28;
}
#endif
static int mov_write_minf_tag(AVFormatContext *s, AVIOContext *pb, MOVMuxContext *mov,
	MOVTrack *track)
{
	int64_t pos = avio_tell(pb);
	int ret;

	avio_wb32(pb, 0); /* size */
	ffio_wfourcc(pb, "minf");
	if (track->par->codec_type == AV_MEDIA_TYPE_VIDEO)
		mov_write_vmhd_tag(pb);
	else if (track->par->codec_type == AV_MEDIA_TYPE_AUDIO)
		mov_write_smhd_tag(pb);

	if (track->mode == MODE_MOV) /* FIXME: Why do it for MODE_MOV only ? */
		mov_write_hdlr_tag(s, pb, NULL);
	mov_write_dinf_tag(pb);
	ret = mov_write_stbl_tag(s, pb, mov, track);
	if (ret < 0)
		return ret;
	return update_size(pb, pos);
}

static int mov_write_mdhd_tag(AVIOContext *pb, MOVMuxContext *mov,
	MOVTrack *track)
{
	int version = 0;

	avio_wb32(pb, 32); /* size */
	ffio_wfourcc(pb, "mdhd");
	avio_w8(pb, version);
	avio_wb24(pb, 0); /* flags */
	avio_wb32(pb, track->time); /* creation time */
	avio_wb32(pb, track->time); /* modification time */
	avio_wb32(pb, track->timescale); /* time scale (sample rate for audio) */
	if (!track->entry)
		avio_wb32(pb, 0);
	else
		avio_wb32(pb, track->track_duration); /* duration */
	avio_wb16(pb, track->language); /* language */
	avio_wb16(pb, 0); /* reserved (quality) */

	return 32;
}

static int mov_write_mdia_tag(AVFormatContext *s, AVIOContext *pb,
	MOVMuxContext *mov, MOVTrack *track)
{
	int64_t pos = avio_tell(pb);
	int ret;

	avio_wb32(pb, 0); /* size */
	ffio_wfourcc(pb, "mdia");
	mov_write_mdhd_tag(pb, mov, track);
	mov_write_hdlr_tag(s, pb, track);
	ret = mov_write_minf_tag(s, pb, mov, track);
	if (ret < 0)
		return ret;
	return update_size(pb, pos);
}

/* transformation matrix
	 |a  b  u|
	 |c  d  v|
	 |tx ty w| */
static void write_matrix(AVIOContext *pb, int16_t a, int16_t b, int16_t c,
	int16_t d, int16_t tx, int16_t ty)
{
	avio_wb32(pb, a << 16);  /* 16.16 format */
	avio_wb32(pb, b << 16);  /* 16.16 format */
	avio_wb32(pb, 0);        /* u in 2.30 format */
	avio_wb32(pb, c << 16);  /* 16.16 format */
	avio_wb32(pb, d << 16);  /* 16.16 format */
	avio_wb32(pb, 0);        /* v in 2.30 format */
	avio_wb32(pb, tx << 16); /* 16.16 format */
	avio_wb32(pb, ty << 16); /* 16.16 format */
	avio_wb32(pb, 1 << 30);  /* w in 2.30 format */
}

static int mov_write_tkhd_tag(AVIOContext *pb, MOVMuxContext *mov,
	MOVTrack *track, AVStream *st)
{
	int64_t duration = av_rescale_rnd(track->track_duration, MOV_TIMESCALE,
			track->timescale, AV_ROUND_UP);
	int version = duration < INT32_MAX ? 0 : 1;
	int flags = MOV_TKHD_FLAG_IN_MOVIE;
	int group = 0;

	if (st) {
		if (mov->per_stream_grouping)
			group = st->index;
		else
			group = st->codecpar->codec_type;
	}

	if (track->flags & MOV_TRACK_ENABLED)
		flags |= MOV_TKHD_FLAG_ENABLED;

	(version == 1) ? avio_wb32(pb, 104) : avio_wb32(pb, 92); /* size */
	ffio_wfourcc(pb, "tkhd");
	avio_w8(pb, version);
	avio_wb24(pb, flags);
	if (version == 1) {
		avio_wb64(pb, track->time);
		avio_wb64(pb, track->time);
	} else {
		avio_wb32(pb, track->time); /* creation time */
		avio_wb32(pb, track->time); /* modification time */
	}
	avio_wb32(pb, track->track_id); /* track-id */
	avio_wb32(pb, 0); /* reserved */
	if (!track->entry)
		(version == 1) ? avio_wb64(pb, 0) : avio_wb32(pb, 0);
	else
		(version == 1) ? avio_wb64(pb, duration) : avio_wb32(pb, duration);

	avio_wb32(pb, 0); /* reserved */
	avio_wb32(pb, 0); /* reserved */
	avio_wb16(pb, 0); /* layer */
	avio_wb16(pb, group); /* alternate group) */
	/* Volume, only for audio */
	if (track->par->codec_type == AV_MEDIA_TYPE_AUDIO)
		avio_wb16(pb, 0x0100);
	else
		avio_wb16(pb, 0);
	avio_wb16(pb, 0); /* reserved */

	/* Matrix structure */
	write_matrix(pb, 1, 0, 0, 1, 0, 0);

	/* Track width and height, for visual only */
	if (st && (track->par->codec_type == AV_MEDIA_TYPE_VIDEO ||
			track->par->codec_type == AV_MEDIA_TYPE_SUBTITLE)) {
		int64_t track_width_1616;

		track_width_1616 = track->par->width * 0x10000ULL;
		if (track_width_1616 > UINT32_MAX) {
			printf("track width is too large\n");
			track_width_1616 = 0;
		}
		avio_wb32(pb, track_width_1616);
		if (track->height > 0xFFFF) {
			printf("track height is too large\n");
			avio_wb32(pb, 0);
		} else
			avio_wb32(pb, track->height * 0x10000U);
	} else {
		avio_wb32(pb, 0);
		avio_wb32(pb, 0);
	}
	return 0x5c;
}

#if 0
static int mov_write_tapt_tag(AVIOContext *pb, MOVTrack *track)
{
	int32_t width = av_rescale(track->par->sample_aspect_ratio.num,
			track->par->width, track->par->sample_aspect_ratio.den);

	int64_t pos = avio_tell(pb);

	avio_wb32(pb, 0); /* size */
	ffio_wfourcc(pb, "tapt");

	avio_wb32(pb, 20);
	ffio_wfourcc(pb, "clef");
	avio_wb32(pb, 0);
	avio_wb32(pb, width << 16);
	avio_wb32(pb, track->par->height << 16);

	avio_wb32(pb, 20);
	ffio_wfourcc(pb, "prof");
	avio_wb32(pb, 0);
	avio_wb32(pb, width << 16);
	avio_wb32(pb, track->par->height << 16);

	avio_wb32(pb, 20);
	ffio_wfourcc(pb, "enof");
	avio_wb32(pb, 0);
	avio_wb32(pb, track->par->width << 16);
	avio_wb32(pb, track->par->height << 16);

	return update_size(pb, pos);
}

/* This box seems important for the psp playback ...
without it the movie seems to hang */
static int mov_write_edts_tag(AVIOContext *pb, MOVMuxContext *mov,
	MOVTrack *track)
{
	int64_t duration = av_rescale_rnd(track->track_duration, MOV_TIMESCALE,
			track->timescale, AV_ROUND_UP);
	int version = duration < INT32_MAX ? 0 : 1;
	int entry_size, entry_count, size;
	int64_t delay, start_ct = track->start_cts;
	int64_t start_dts = track->start_dts;

	if (track->entry) {
		if (start_dts != track->cluster[0].dts ||
			start_ct != track->cluster[0].cts) {

			PRINTF("EDTS using dts:%"PRId64" cts:%d",
				track->cluster[0].dts, track->cluster[0].cts);
			PRINTF("instead of dts:%"PRId64" cts:%"PRId64" tid:%d\n",
				start_dts, start_ct, track->track_id);
			start_dts = track->cluster[0].dts;
			start_ct  = track->cluster[0].cts;
		}
	}

	delay = av_rescale_rnd(start_dts + start_ct, MOV_TIMESCALE,
			track->timescale, AV_ROUND_DOWN);
	version |= delay < INT32_MAX ? 0 : 1;

	entry_size = (version == 1) ? 20 : 12;
	entry_count = 1 + (delay > 0);
	size = 24 + entry_count * entry_size;

	/* write the atom data */
	avio_wb32(pb, size);
	ffio_wfourcc(pb, "edts");
	avio_wb32(pb, size - 8);
	ffio_wfourcc(pb, "elst");
	avio_w8(pb, version);
	avio_wb24(pb, 0); /* flags */

	avio_wb32(pb, entry_count);
	if (delay > 0) { /* add an empty edit to delay presentation */
		/* In the positive delay case, the delay includes the cts
		 * offset, and the second edit list entry below trims out
		 * the same amount from the actual content. This makes sure
		 * that the offset last sample is included in the edit
		 * list duration as well. */
		if (version == 1) {
			avio_wb64(pb, delay);
			avio_wb64(pb, -1);
		} else {
			avio_wb32(pb, delay);
			avio_wb32(pb, -1);
		}
		avio_wb32(pb, 0x00010000);
	} else {
		/* Avoid accidentally ending up with start_ct = -1 which has got a
		 * special meaning. Normally start_ct should end up positive or zero
		 * here, but use FFMIN in case dts is a small positive integer
		 * rounded to 0 when represented in MOV_TIMESCALE units. */
		av_assert0(av_rescale_rnd(start_dts, MOV_TIMESCALE, track->timescale,
				AV_ROUND_DOWN) <= 0);
		start_ct  = -FFMIN(start_dts, 0);
		/* Note, this delay is calculated from the pts of the first sample,
		 * ensuring that we don't reduce the duration for cases with
		 * dts<0 pts=0. */
		duration += delay;
	}

	/* For fragmented files, we don't know the full length yet. Setting
	 * duration to 0 allows us to only specify the offset, including
	 * the rest of the content (from all future fragments) without specifying
	 * an explicit duration. */
	if (mov->flags & FF_MOV_FLAG_FRAGMENT)
		duration = 0;

	/* duration */
	if (version == 1) {
		avio_wb64(pb, duration);
		avio_wb64(pb, start_ct);
	} else {
		avio_wb32(pb, duration);
		avio_wb32(pb, start_ct);
	}
	avio_wb32(pb, 0x00010000);
	return size;
}

static int mov_write_tref_tag(AVIOContext *pb, MOVTrack *track)
{
	avio_wb32(pb, 20);   /* size */
	ffio_wfourcc(pb, "tref");
	avio_wb32(pb, 12);   /* size (subatom) */
	avio_wl32(pb, track->tref_tag);
	avio_wb32(pb, track->tref_id);
	return 20;
}

/* goes at the end of each track!
... Critical for PSP playback ("Incompatible data" without it) */
static int mov_write_uuid_tag_psp(AVIOContext *pb, MOVTrack *mov)
{
	avio_wb32(pb, 0x34); /* size ... reports as 28 in mp4box! */
	ffio_wfourcc(pb, "uuid");
	ffio_wfourcc(pb, "USMT");
	avio_wb32(pb, 0x21d24fce);
	avio_wb32(pb, 0xbb88695c);
	avio_wb32(pb, 0xfac9c740);
	avio_wb32(pb, 0x1c);     /* another size here! */
	ffio_wfourcc(pb, "MTDT");
	avio_wb32(pb, 0x00010012);
	avio_wb32(pb, 0x0a);
	avio_wb32(pb, 0x55c40000);
	avio_wb32(pb, 0x1);
	avio_wb32(pb, 0x0);
	return 0x34;
}

static int mov_write_udta_sdp(AVIOContext *pb, MOVTrack *track)
{
	AVFormatContext *ctx = track->rtp_ctx;
	char buf[1000] = "";
	int len;

	ff_sdp_write_media(buf, sizeof(buf), ctx->streams[0], track->src_track,
		NULL, NULL, 0, 0, ctx);
	av_strlcatf(buf, sizeof(buf), "a=control:streamid=%d\r\n", track->track_id);
	len = strlen(buf);

	avio_wb32(pb, len + 24);
	ffio_wfourcc(pb, "udta");
	avio_wb32(pb, len + 16);
	ffio_wfourcc(pb, "hnti");
	avio_wb32(pb, len + 8);
	ffio_wfourcc(pb, "sdp ");
	avio_write(pb, buf, len);
	return len + 24;
}

static int mov_write_track_metadata(AVIOContext *pb, AVStream *st,
	const char *tag, const char *str)
{
	int64_t pos = avio_tell(pb);
	AVDictionaryEntry *t = av_dict_get(st->metadata, str, NULL, 0);
	if (!t || !utf8len(t->value))
		return 0;

	avio_wb32(pb, 0);   /* size */
	ffio_wfourcc(pb, tag); /* type */
	avio_write(pb, t->value, strlen(t->value)); /* UTF8 string value */
	return update_size(pb, pos);
}

static int mov_write_track_udta_tag(AVIOContext *pb, MOVMuxContext *mov,
	AVStream *st)
{
	AVIOContext *pb_buf;
	int ret, size;
	uint8_t *buf;

	if (!st)
		return 0;

	ret = avio_open_dyn_buf(&pb_buf);
	if (ret < 0)
		return ret;

	if (mov->mode & MODE_MP4)
		mov_write_track_metadata(pb_buf, st, "name", "title");

	size = avio_close_dyn_buf(pb_buf, &buf);
	if (size > 0) {
		avio_wb32(pb, size + 8);
		ffio_wfourcc(pb, "udta");
		avio_write(pb, buf, size);
	}
	av_free(buf);

	return 0;
}
#endif

static int mov_write_trak_tag(AVFormatContext *s, AVIOContext *pb, MOVMuxContext *mov,
	MOVTrack *track, AVStream *st)
{
	int64_t pos = avio_tell(pb);
	int entry_backup = track->entry;
	int chunk_backup = track->chunkCount;
	int ret;

	avio_wb32(pb, 0); /* size */
	ffio_wfourcc(pb, "trak");
	mov_write_tkhd_tag(pb, mov, track, st);

	ret = mov_write_mdia_tag(s, pb, mov, track);
	if (ret < 0)
		return ret;

	track->entry = entry_backup;
	track->chunkCount = chunk_backup;
	return update_size(pb, pos);
}

#if 0
static int mov_write_iods_tag(AVIOContext *pb, MOVMuxContext *mov)
{
	int i, has_audio = 0, has_video = 0;
	int64_t pos = avio_tell(pb);
	int audio_profile = mov->iods_audio_profile;
	int video_profile = mov->iods_video_profile;
	for (i = 0; i < mov->nb_streams; i++) {
		if (mov->tracks[i].entry > 0 || mov->flags & FF_MOV_FLAG_EMPTY_MOOV) {
			has_audio |=
				mov->tracks[i].par->codec_type == AV_MEDIA_TYPE_AUDIO;
			has_video |=
				mov->tracks[i].par->codec_type == AV_MEDIA_TYPE_VIDEO;
		}
	}
	if (audio_profile < 0)
		audio_profile = 0xFF - has_audio;
	if (video_profile < 0)
		video_profile = 0xFF - has_video;
	avio_wb32(pb, 0x0); /* size */
	ffio_wfourcc(pb, "iods");
	avio_wb32(pb, 0);    /* version & flags */
	put_descr(pb, 0x10, 7);
	avio_wb16(pb, 0x004f);
	avio_w8(pb, 0xff);
	avio_w8(pb, 0xff);
	avio_w8(pb, audio_profile);
	avio_w8(pb, video_profile);
	avio_w8(pb, 0xff);
	return update_size(pb, pos);
}
#endif

static int mov_write_trex_tag(AVIOContext *pb, MOVTrack *track)
{
	avio_wb32(pb, 0x20); /* size */
	ffio_wfourcc(pb, "trex");
	avio_wb32(pb, 0);   /* version & flags */
	avio_wb32(pb, track->track_id); /* track ID */
	avio_wb32(pb, 1);   /* default sample description index */
	avio_wb32(pb, 0);   /* default sample duration */
	avio_wb32(pb, 0);   /* default sample size */
	avio_wb32(pb, 0);   /* default sample flags */
	return 0;
}

static int mov_write_mvex_tag(AVIOContext *pb, MOVMuxContext *mov)
{
	int64_t pos = avio_tell(pb);
	int i;
	avio_wb32(pb, 0x0); /* size */
	ffio_wfourcc(pb, "mvex");
	for (i = 0; i < mov->nb_streams; i++)
		mov_write_trex_tag(pb, &mov->tracks[i]);
	return update_size(pb, pos);
}

static int mov_write_mvhd_tag(AVIOContext *pb, MOVMuxContext *mov)
{
	int max_track_id = 1, i;
	int64_t max_track_len = 0;
	int version;

	for (i = 0; i < mov->nb_streams; i++) {
		if (mov->tracks[i].entry > 0 && mov->tracks[i].timescale) {
			int64_t max_track_len_temp =
				av_rescale_rnd(mov->tracks[i].track_duration,
					MOV_TIMESCALE,
					mov->tracks[i].timescale,
					AV_ROUND_UP);
			if (max_track_len < max_track_len_temp)
				max_track_len = max_track_len_temp;
			if (max_track_id < mov->tracks[i].track_id)
				max_track_id = mov->tracks[i].track_id;
		}
	}
	/* If using delay_moov, make sure the output is the same as if no
	 * samples had been written yet. */
	if (mov->flags & FF_MOV_FLAG_EMPTY_MOOV) {
		max_track_len = 0;
		max_track_id = 1;
	}

	version = max_track_len < UINT32_MAX ? 0 : 1;
	avio_wb32(pb, version == 1 ? 120 : 108); /* size */

	ffio_wfourcc(pb, "mvhd");
	avio_w8(pb, version);
	avio_wb24(pb, 0); /* flags */
	if (version == 1) {
		avio_wb64(pb, mov->time);
		avio_wb64(pb, mov->time);
	} else {
		avio_wb32(pb, mov->time); /* creation time */
		avio_wb32(pb, mov->time); /* modification time */
	}
	avio_wb32(pb, MOV_TIMESCALE);
	(version == 1) ? avio_wb64(pb, max_track_len) : avio_wb32(pb,
		max_track_len); /* duration of longest track */

	avio_wb32(pb, 0x00010000); /* reserved (preferred rate) 1.0 = normal */
	avio_wb16(pb, 0x0100); /* reserved (preferred volume) 1.0 = normal */
	avio_wb16(pb, 0); /* reserved */
	avio_wb32(pb, 0); /* reserved */
	avio_wb32(pb, 0); /* reserved */

	/* Matrix structure */
	write_matrix(pb, 1, 0, 0, 1, 0, 0);

	avio_wb32(pb, 0); /* reserved (preview time) */
	avio_wb32(pb, 0); /* reserved (preview duration) */
	avio_wb32(pb, 0); /* reserved (poster time) */
	avio_wb32(pb, 0); /* reserved (selection time) */
	avio_wb32(pb, 0); /* reserved (selection duration) */
	avio_wb32(pb, 0); /* reserved (current time) */
	avio_wb32(pb, max_track_id + 1); /* Next track id */
	return 0x6c;
}

#if 0
static int mov_write_itunes_hdlr_tag(AVIOContext *pb, MOVMuxContext *mov,
	AVFormatContext *s)
{
	avio_wb32(pb, 33); /* size */
	ffio_wfourcc(pb, "hdlr");
	avio_wb32(pb, 0);
	avio_wb32(pb, 0);
	ffio_wfourcc(pb, "mdir");
	ffio_wfourcc(pb, "appl");
	avio_wb32(pb, 0);
	avio_wb32(pb, 0);
	avio_w8(pb, 0);
	return 33;
}

/* helper function to write a data tag with the specified string as data */
static int mov_write_string_data_tag(AVIOContext *pb, const char *data, int lang,
	int long_style)
{
	if (long_style) {
		int size = 16 + strlen(data);
		avio_wb32(pb, size); /* size */
		ffio_wfourcc(pb, "data");
		avio_wb32(pb, 1);
		avio_wb32(pb, 0);
		avio_write(pb, data, strlen(data));
		return size;
	} else {
		if (!lang)
			lang = ff_mov_iso639_to_lang("und", 1);
		avio_wb16(pb, strlen(data)); /* string length */
		avio_wb16(pb, lang);
		avio_write(pb, data, strlen(data));
		return strlen(data) + 4;
	}
}

static int mov_write_string_tag(AVIOContext *pb, const char *name,
	const char *value, int lang, int long_style)
{
	int size = 0;
	if (value && value[0]) {
		int64_t pos = avio_tell(pb);
		avio_wb32(pb, 0); /* size */
		ffio_wfourcc(pb, name);
		mov_write_string_data_tag(pb, value, lang, long_style);
		size = update_size(pb, pos);
	}
	return size;
}

static AVDictionaryEntry *get_metadata_lang(AVFormatContext *s,
	const char *tag, int *lang)
{
	int l, len, len2;
	AVDictionaryEntry *t, *t2 = NULL;
	char tag2[16];

	*lang = 0;

	t = av_dict_get(s->metadata, tag, NULL, 0);
	if (!t)
		return NULL;

	len = strlen(t->key);
	snprintf(tag2, sizeof(tag2), "%s-", tag);
	while ((t2 = av_dict_get(s->metadata, tag2, t2, AV_DICT_IGNORE_SUFFIX))) {
		len2 = strlen(t2->key);
		l = ff_mov_iso639_to_lang(&t2->key[len2 - 3], 1);
		if (len2 == len + 4 && !strcmp(t->value, t2->value)
			&& l >= 0) {
			*lang = l;
			return t;
		}
	}
	return t;
}

static int mov_write_string_metadata(AVFormatContext *s, AVIOContext *pb,
	const char *name, const char *tag,
	int long_style)
{
	int lang;
	AVDictionaryEntry *t = get_metadata_lang(s, tag, &lang);
	if (!t)
		return 0;
	return mov_write_string_tag(pb, name, t->value, lang, long_style);
}

/* iTunes bpm number */
static int mov_write_tmpo_tag(AVIOContext *pb, AVFormatContext *s)
{
	AVDictionaryEntry *t = av_dict_get(s->metadata, "tmpo", NULL, 0);
	int size = 0, tmpo = t ? atoi(t->value) : 0;
	if (tmpo) {
		size = 26;
		avio_wb32(pb, size);
		ffio_wfourcc(pb, "tmpo");
		avio_wb32(pb, size - 8); /* size */
		ffio_wfourcc(pb, "data");
		avio_wb32(pb, 0x15);  /* type specifier */
		avio_wb32(pb, 0);
		avio_wb16(pb, tmpo);        /* data */
	}
	return size;
}

/* 3GPP TS 26.244 */
static int mov_write_loci_tag(AVFormatContext *s, AVIOContext *pb)
{
	int lang;
	int64_t pos = avio_tell(pb);
	double latitude, longitude, altitude;
	int32_t latitude_fix, longitude_fix, altitude_fix;
	AVDictionaryEntry *t = get_metadata_lang(s, "location", &lang);
	const char *ptr, *place = "";
	char *end;
	static const char *astronomical_body = "earth";
	if (!t)
		return 0;

	ptr = t->value;
	longitude = strtod(ptr, &end);
	if (end == ptr) {
		av_log(s, AV_LOG_WARNING, "malformed location metadata\n");
		return 0;
	}
	ptr = end;
	latitude = strtod(ptr, &end);
	if (end == ptr) {
		av_log(s, AV_LOG_WARNING, "malformed location metadata\n");
		return 0;
	}
	ptr = end;
	altitude = strtod(ptr, &end);
	/* If no altitude was present, the default 0 should be fine */
	if (*end == '/')
		place = end + 1;

	latitude_fix  = (int32_t)((1 << 16) * latitude);
	longitude_fix = (int32_t)((1 << 16) * longitude);
	altitude_fix  = (int32_t)((1 << 16) * altitude);

	avio_wb32(pb, 0);         /* size */
	ffio_wfourcc(pb, "loci"); /* type */
	avio_wb32(pb, 0);         /* version + flags */
	avio_wb16(pb, lang);
	avio_write(pb, place, strlen(place) + 1);
	/* role of place (0 == shooting location, 1 == real location,
	2 == fictional location) */
	avio_w8(pb,	0);
	avio_wb32(pb, latitude_fix);
	avio_wb32(pb, longitude_fix);
	avio_wb32(pb, altitude_fix);
	avio_write(pb, astronomical_body, strlen(astronomical_body) + 1);
	avio_w8(pb, 0);           /* additional notes, null terminated string */

	return update_size(pb, pos);
}

/* iTunes track or disc number */
static int mov_write_trkn_tag(AVIOContext *pb, MOVMuxContext *mov,
	AVFormatContext *s, int disc)
{
	AVDictionaryEntry *t = av_dict_get(s->metadata,
			disc ? "disc" : "track",
			NULL, 0);
	int size = 0, track = t ? atoi(t->value) : 0;
	if (track) {
		int tracks = 0;
		char *slash = strchr(t->value, '/');
		if (slash)
			tracks = atoi(slash + 1);
		avio_wb32(pb, 32); /* size */
		ffio_wfourcc(pb, disc ? "disk" : "trkn");
		avio_wb32(pb, 24); /* size */
		ffio_wfourcc(pb, "data");
		avio_wb32(pb, 0);        /* 8 bytes empty */
		avio_wb32(pb, 0);
		avio_wb16(pb, 0);        /* empty */
		avio_wb16(pb, track);    /* track / disc number */
		avio_wb16(pb, tracks);   /* total track / disc number */
		avio_wb16(pb, 0);        /* empty */
		size = 32;
	}
	return size;
}

static int mov_write_int8_metadata(AVFormatContext *s, AVIOContext *pb,
	const char *name, const char *tag,
	int len)
{
	AVDictionaryEntry *t = NULL;
	uint8_t num;
	int size = 24 + len;

	if (len != 1 && len != 4)
		return -1;

	t = av_dict_get(s->metadata, tag, NULL, 0);
	if (!t)
		return 0;
	num = atoi(t->value);

	avio_wb32(pb, size);
	ffio_wfourcc(pb, name);
	avio_wb32(pb, size - 8);
	ffio_wfourcc(pb, "data");
	avio_wb32(pb, 0x15);
	avio_wb32(pb, 0);
	if (len == 4)
		avio_wb32(pb, num);
	else
		avio_w8(pb, num);

	return size;
}

/* iTunes meta data list */
static int mov_write_ilst_tag(AVIOContext *pb, MOVMuxContext *mov,
	AVFormatContext *s)
{
	int64_t pos = avio_tell(pb);
	avio_wb32(pb, 0); /* size */
	ffio_wfourcc(pb, "ilst");
	mov_write_string_metadata(s, pb, "\251nam", "title", 1);
	mov_write_string_metadata(s, pb, "\251ART", "artist", 1);
	mov_write_string_metadata(s, pb, "aART", "album_artist", 1);
	mov_write_string_metadata(s, pb, "\251wrt", "composer", 1);
	mov_write_string_metadata(s, pb, "\251alb", "album", 1);
	mov_write_string_metadata(s, pb, "\251day", "date", 1);
	if (!mov_write_string_metadata(s, pb, "\251too", "encoding_tool", 1)) {
		if (!(s->flags & AVFMT_FLAG_BITEXACT))
			mov_write_string_tag(pb, "\251too", LIBAVFORMAT_IDENT, 0, 1);
	}
	mov_write_string_metadata(s, pb, "\251cmt", "comment", 1);
	mov_write_string_metadata(s, pb, "\251gen", "genre", 1);
	mov_write_string_metadata(s, pb, "\251cpy", "copyright", 1);
	mov_write_string_metadata(s, pb, "\251grp", "grouping", 1);
	mov_write_string_metadata(s, pb, "\251lyr", "lyrics", 1);
	mov_write_string_metadata(s, pb, "desc",    "description", 1);
	mov_write_string_metadata(s, pb, "ldes",    "synopsis", 1);
	mov_write_string_metadata(s, pb, "tvsh",    "show", 1);
	mov_write_string_metadata(s, pb, "tven",    "episode_id", 1);
	mov_write_string_metadata(s, pb, "tvnn",    "network", 1);
	mov_write_int8_metadata(s, pb, "tves",    "episode_sort", 4);
	mov_write_int8_metadata(s, pb, "tvsn",    "season_number", 4);
	mov_write_int8_metadata(s, pb, "stik",    "media_type", 1);
	mov_write_int8_metadata(s, pb, "hdvd",    "hd_video",  1);
	mov_write_int8_metadata(s, pb, "pgap",    "gapless_playback", 1);
	mov_write_int8_metadata(s, pb, "cpil",    "compilation", 1);
	mov_write_trkn_tag(pb, mov, s, 0); /* track number */
	mov_write_trkn_tag(pb, mov, s, 1); /* disc number */
	mov_write_tmpo_tag(pb, s);
	return update_size(pb, pos);
}

static int mov_write_mdta_hdlr_tag(AVIOContext *pb, MOVMuxContext *mov,
	AVFormatContext *s)
{
	avio_wb32(pb, 33); /* size */
	ffio_wfourcc(pb, "hdlr");
	avio_wb32(pb, 0);
	avio_wb32(pb, 0);
	ffio_wfourcc(pb, "mdta");
	avio_wb32(pb, 0);
	avio_wb32(pb, 0);
	avio_wb32(pb, 0);
	avio_w8(pb, 0);
	return 33;
}

static int mov_write_mdta_keys_tag(AVIOContext *pb, MOVMuxContext *mov,
	AVFormatContext *s)
{
	AVDictionaryEntry *t = NULL;
	int64_t pos = avio_tell(pb);
	int64_t curpos, entry_pos;
	int count = 0;

	avio_wb32(pb, 0); /* size */
	ffio_wfourcc(pb, "keys");
	avio_wb32(pb, 0);
	entry_pos = avio_tell(pb);
	avio_wb32(pb, 0); /* entry count */

	while (t = av_dict_get(s->metadata, "", t, AV_DICT_IGNORE_SUFFIX)) {
		avio_wb32(pb, strlen(t->key) + 8);
		ffio_wfourcc(pb, "mdta");
		avio_write(pb, t->key, strlen(t->key));
		count += 1;
	}
	curpos = avio_tell(pb);
	avio_seek(pb, entry_pos, SEEK_SET);
	avio_wb32(pb, count); /* rewrite entry count */
	avio_seek(pb, curpos, SEEK_SET);

	return update_size(pb, pos);
}

static int mov_write_mdta_ilst_tag(AVIOContext *pb, MOVMuxContext *mov,
	AVFormatContext *s)
{
	AVDictionaryEntry *t = NULL;
	int64_t pos = avio_tell(pb);
	int count = 1; /* keys are 1-index based */

	avio_wb32(pb, 0); /* size */
	ffio_wfourcc(pb, "ilst");

	while (t = av_dict_get(s->metadata, "", t, AV_DICT_IGNORE_SUFFIX)) {
		int64_t entry_pos = avio_tell(pb);
		avio_wb32(pb, 0); /* size */
		avio_wb32(pb, count); /* key */
		mov_write_string_data_tag(pb, t->value, 0, 1);
		update_size(pb, entry_pos);
		count += 1;
	}
	return update_size(pb, pos);
}

/* meta data tags */
static int mov_write_meta_tag(AVIOContext *pb, MOVMuxContext *mov,
	AVFormatContext *s)
{
	int size = 0;
	int64_t pos = avio_tell(pb);
	avio_wb32(pb, 0); /* size */
	ffio_wfourcc(pb, "meta");
	avio_wb32(pb, 0);
	if (mov->flags & FF_MOV_FLAG_USE_MDTA) {
		mov_write_mdta_hdlr_tag(pb, mov, s);
		mov_write_mdta_keys_tag(pb, mov, s);
		mov_write_mdta_ilst_tag(pb, mov, s);
	} else {
		/* iTunes metadata tag */
		mov_write_itunes_hdlr_tag(pb, mov, s);
		mov_write_ilst_tag(pb, mov, s);
	}
	size = update_size(pb, pos);
	return size;
}

static int mov_write_raw_metadata_tag(AVFormatContext *s, AVIOContext *pb,
	const char *name, const char *key)
{
	int len;
	AVDictionaryEntry *t;

	t = av_dict_get(s->metadata, key, NULL, 0);
	if (!t)
		return 0;

	len = strlen(t->value);
	if (len > 0) {
		int size = len + 8;
		avio_wb32(pb, size);
		ffio_wfourcc(pb, name);
		avio_write(pb, t->value, len);
		return size;
	}
	return 0;
}

static int ascii_to_wc(AVIOContext *pb, const uint8_t *b)
{
	int val;
	while (*b) {
		GET_UTF8(val, *b++, return -1;)
		avio_wb16(pb, val);
	}
	avio_wb16(pb, 0x00);
	return 0;
}

static uint16_t language_code(const char *str)
{
	return (((str[0] - 0x60) & 0x1F) << 10) +
		(((str[1] - 0x60) & 0x1F) <<  5) +
		((str[2] - 0x60) & 0x1F);
}

static int mov_write_3gp_udta_tag(AVIOContext *pb, AVFormatContext *s,
	const char *tag, const char *str)
{
	int64_t pos = avio_tell(pb);
	AVDictionaryEntry *t = av_dict_get(s->metadata, str, NULL, 0);
	if (!t || !utf8len(t->value))
		return 0;
	avio_wb32(pb, 0);   /* size */
	ffio_wfourcc(pb, tag); /* type */
	avio_wb32(pb, 0);   /* version + flags */
	if (!strcmp(tag, "yrrc"))
		avio_wb16(pb, atoi(t->value));
	else {
		avio_wb16(pb, language_code("eng")); /* language */
		avio_write(pb, t->value, strlen(t->value) + 1); /* UTF8 string value */
		t = av_dict_get(s->metadata, "track", NULL, 0);
		if (!strcmp(tag, "albm") &&
			t)
			avio_w8(pb, atoi(t->value));
	}
	return update_size(pb, pos);
}

static int mov_write_chpl_tag(AVIOContext *pb, AVFormatContext *s)
{
	int64_t pos = avio_tell(pb);
	int i, nb_chapters = FFMIN(s->nb_chapters, 255);

	avio_wb32(pb, 0);            /* size */
	ffio_wfourcc(pb, "chpl");
	avio_wb32(pb, 0x01000000);   /* version + flags */
	avio_wb32(pb, 0);            /* unknown */
	avio_w8(pb, nb_chapters);

	for (i = 0; i < nb_chapters; i++) {
		AVChapter *c = s->chapters[i];
		AVDictionaryEntry *t;
		avio_wb64(pb, av_rescale_q(c->start, c->time_base, (AV_Rational) {
			1, 10000000
		}));

		t = av_dict_get(c->metadata, "title", NULL, 0);
		if (t) {
			int len = FFMIN(strlen(t->value), 255);
			avio_w8(pb, len);
			avio_write(pb, t->value, len);
		} else
			avio_w8(pb, 0);
	}
	return update_size(pb, pos);
}

static int mov_write_udta_tag(AVIOContext *pb, MOVMuxContext *mov,
	AVFormatContext *s)
{
	AVIOContext *pb_buf;
	int ret, size;
	uint8_t *buf;

	ret = avio_open_dyn_buf(&pb_buf);
	if (ret < 0)
		return ret;

	if (mov->mode & MODE_3GP) {
		mov_write_3gp_udta_tag(pb_buf, s, "perf", "artist");
		mov_write_3gp_udta_tag(pb_buf, s, "titl", "title");
		mov_write_3gp_udta_tag(pb_buf, s, "auth", "author");
		mov_write_3gp_udta_tag(pb_buf, s, "gnre", "genre");
		mov_write_3gp_udta_tag(pb_buf, s, "dscp", "comment");
		mov_write_3gp_udta_tag(pb_buf, s, "albm", "album");
		mov_write_3gp_udta_tag(pb_buf, s, "cprt", "copyright");
		mov_write_3gp_udta_tag(pb_buf, s, "yrrc", "date");
		mov_write_loci_tag(s, pb_buf);
	} else if (mov->mode == MODE_MOV
		&& !(mov->flags & FF_MOV_FLAG_USE_MDTA)) {
		/* the title field breaks gtkpod with mp4 and my
		suspicion is that stuff is not valid in mp4 */
		mov_write_string_metadata(s, pb_buf, "\251ART", "artist",      0);
		mov_write_string_metadata(s, pb_buf, "\251nam", "title",       0);
		mov_write_string_metadata(s, pb_buf, "\251aut", "author",      0);
		mov_write_string_metadata(s, pb_buf, "\251alb", "album",       0);
		mov_write_string_metadata(s, pb_buf, "\251day", "date",        0);
		mov_write_string_metadata(s, pb_buf, "\251swr", "encoder",     0);
		/* currently ignored by mov.c */
		mov_write_string_metadata(s, pb_buf, "\251des", "comment",     0);
		/* add support for libquicktime, this atom
		is also actually read by mov.c */
		mov_write_string_metadata(s, pb_buf, "\251cmt", "comment",     0);
		mov_write_string_metadata(s, pb_buf, "\251gen", "genre",       0);
		mov_write_string_metadata(s, pb_buf, "\251cpy", "copyright",   0);
		mov_write_string_metadata(s, pb_buf, "\251mak", "make",        0);
		mov_write_string_metadata(s, pb_buf, "\251mod", "model",       0);
		mov_write_string_metadata(s, pb_buf, "\251xyz", "location",    0);
		mov_write_raw_metadata_tag(s, pb_buf, "XMP_", "xmp");
	} else {
		/* iTunes meta data */
		mov_write_meta_tag(pb_buf, mov, s);
		mov_write_loci_tag(s, pb_buf);
	}

	if (s->nb_chapters && !(mov->flags & FF_MOV_FLAG_DISABLE_CHPL))
		mov_write_chpl_tag(pb_buf, s);

	size = avio_close_dyn_buf(pb_buf, &buf);
	if (size > 0) {
		avio_wb32(pb, size + 8);
		ffio_wfourcc(pb, "udta");
		avio_write(pb, buf, size);
	}
	av_free(buf);

	return 0;
}

static void mov_write_psp_udta_tag(AVIOContext *pb,
	const char *str, const char *lang, int type)
{
	int len = utf8len(str) + 1;
	if (len <= 0)
		return;
	avio_wb16(pb, len * 2 + 10);        /* size */
	avio_wb32(pb, type);                /* type */
	avio_wb16(pb, language_code(lang)); /* language */
	avio_wb16(pb, 0x01);                /* ? */
	ascii_to_wc(pb, str);
}

static int mov_write_uuidusmt_tag(AVIOContext *pb, AVFormatContext *s)
{
	AVDictionaryEntry *title = av_dict_get(s->metadata, "title", NULL, 0);
	int64_t pos, pos2;

	if (title) {
		pos = avio_tell(pb);
		avio_wb32(pb, 0); /* size placeholder*/
		ffio_wfourcc(pb, "uuid");
		ffio_wfourcc(pb, "USMT");
		avio_wb32(pb, 0x21d24fce); /* 96 bit UUID */
		avio_wb32(pb, 0xbb88695c);
		avio_wb32(pb, 0xfac9c740);

		pos2 = avio_tell(pb);
		avio_wb32(pb, 0); /* size placeholder*/
		ffio_wfourcc(pb, "MTDT");
		avio_wb16(pb, 4);

		avio_wb16(pb, 0x0C);                 /* size */
		avio_wb32(pb, 0x0B);                 /* type */
		avio_wb16(pb, language_code("und")); /* language */
		avio_wb16(pb, 0x0);                  /* ? */
		avio_wb16(pb, 0x021C);               /* data */

		if (!(s->flags & AVFMT_FLAG_BITEXACT))
			mov_write_psp_udta_tag(pb, LIBAVCODEC_IDENT,      "eng", 0x04);
		mov_write_psp_udta_tag(pb, title->value,          "eng", 0x01);
		mov_write_psp_udta_tag(pb, "2006/04/01 11:11:11", "und", 0x03);

		update_size(pb, pos2);
		return update_size(pb, pos);
	}

	return 0;
}

static void build_chunks(MOVTrack *trk)
{
	int i;
	return 0;
	MOVIentry *chunk = &trk->cluster[0];
	uint64_t chunkSize = chunk->size;
	chunk->chunkNum = 1;
	if (trk->chunkCount)
		return;
	trk->chunkCount = 1;
	for (i = 1; i < trk->entry; i++) {
		if (chunk->pos + chunkSize == trk->cluster[i].pos &&
			chunkSize + trk->cluster[i].size < (1 << 20)) {
			chunkSize             += trk->cluster[i].size;
			chunk->samples_in_chunk += trk->cluster[i].entries;
		} else {
			trk->cluster[i].chunkNum = chunk->chunkNum + 1;
			chunk = &trk->cluster[i];
			chunkSize = chunk->size;
			trk->chunkCount++;
		}
	}
}
#endif

static void build_chunk(MOVTrack *trk)
{
	int len;
	if (trk->chunkCount == 0) { /* init */
		/*trk->clust.chunk = &trk->cluster[0];
		trk->clust.chunk->chunkNum = 1;*/
		trk->chunkCount = 1;
		trk->clust.chunkSize = trk->clust.size[0];
	} else {
		if (trk->clust.pos[(trk->chunkCount - 1) & (CHUNK_NUM - 1)] +
			trk->clust.chunkSize ==
			trk->clust.pos[trk->chunkCount & (CHUNK_NUM - 1)] &&
			trk->clust.chunkSize +
			trk->clust.size[trk->entry & (STSZ_NUM - 1)] < (1 << 20)) {
			trk->clust.chunkSize += trk->clust.size[trk->entry &
						   (STSZ_NUM - 1)];
			trk->clust.samples_in_chunk[(trk->chunkCount - 1) &
						      (CHUNK_NUM - 1)] +=
					trk->clust.entries;
		} else {
			/* trk->cluster[trk->entry].chunkNum =
			trk->clust.chunk->chunkNum + 1;
			trk->clust.chunk = &trk->cluster[trk->entry]; */
			/* trk->clust.chunk->size; */
			trk->clust.chunkSize = trk->clust.size[trk->entry & (STSZ_NUM -
							   1)];
			trk->chunkCount++;
			if ((trk->chunkCount == (trk->clust.framechunk) * CHUNK_NUM + 1)
				&& (trk->clust.stscfile)) {
				len = fwrite(&trk->clust.samples_in_chunk[CHUNK_NUM - 1],
						4, 1, trk->clust.stscfile);
				if (len != 1)
					printf("==write samples_in_chunk error");
			}
		}

	}
}

/**
* Assign track ids. If option "use_stream_ids_as_track_ids" is set,
* the stream ids are used as track ids.
*
* This assumes mov->tracks and s->streams are in the same order and
* there are no gaps in either of them (so mov->tracks[n] refers to
* s->streams[n]).
*
* As an exception, there can be more entries in
* s->streams than in mov->tracks, in which case new track ids are
* generated (starting after the largest found stream id).
*/
static int mov_setup_track_ids(MOVMuxContext *mov, AVFormatContext *s)
{
	int i;

	if (mov->track_ids_ok)
		return 0;

	if (mov->use_stream_ids_as_track_ids) {
		int next_generated_track_id = 0;
		for (i = 0; i < s->nb_streams; i++) {
			if (s->streams[i]->id > next_generated_track_id)
				next_generated_track_id = s->streams[i]->id;
		}

		for (i = 0; i < mov->nb_streams; i++) {
			if (mov->tracks[i].entry <= 0 &&
				!(mov->flags & FF_MOV_FLAG_FRAGMENT))
				continue;

			mov->tracks[i].track_id = i >= s->nb_streams ?
				++next_generated_track_id :	s->streams[i]->id;
		}
	} else {
		for (i = 0; i < mov->nb_streams; i++) {
			if (mov->tracks[i].entry <= 0 &&
				!(mov->flags & FF_MOV_FLAG_FRAGMENT))
				continue;

			mov->tracks[i].track_id = i + 1;
		}
	}

	mov->track_ids_ok = 1;

	return 0;
}

static int mov_write_moov_tag(AVIOContext *pb, MOVMuxContext *mov,
	AVFormatContext *s)
{
	int i;
	int64_t pos = avio_tell(pb);
	avio_wb32(pb, 0); /* size placeholder*/
	ffio_wfourcc(pb, "moov");

	mov_setup_track_ids(mov, s);

	for (i = 0; i < mov->nb_streams; i++) {
		if (mov->tracks[i].entry <= 0 && !(mov->flags & FF_MOV_FLAG_FRAGMENT))
			continue;

		mov->tracks[i].time = mov->time;

		/* if (mov->tracks[i].entry)
		    build_chunks(&mov->tracks[i]); */
	}

	mov_write_mvhd_tag(pb, mov);

	for (i = 0; i < mov->nb_streams; i++) {
		if (mov->tracks[i].entry > 0 || mov->flags & FF_MOV_FLAG_FRAGMENT) {
			int ret = mov_write_trak_tag(s, pb, mov, &(mov->tracks[i]),
					i < s->nb_streams ? s->streams[i] : NULL);
			if (ret < 0)
				return ret;
		}
	}
	if (mov->flags & FF_MOV_FLAG_FRAGMENT)
		mov_write_mvex_tag(pb, mov); /* QuickTime requires trak to precede this */

	return update_size(pb, pos);
}

#if 0
static void param_write_int(AVIOContext *pb, const char *name, int value)
{
	avio_printf(pb,
		"<param name=\"%s\" value=\"%d\" valuetype=\"data\"/>\n", name, value);
}

static void param_write_string(AVIOContext *pb, const char *name, const char *value)
{
	avio_printf(pb,
		"<param name=\"%s\" value=\"%s\" valuetype=\"data\"/>\n", name, value);
}

static void param_write_hex(AVIOContext *pb, const char *name, const uint8_t *value,
	int len)
{
	char buf[150];
	len = FFMIN(sizeof(buf) / 2 - 1, len);
	ff_data_to_hex(buf, value, len, 0);
	buf[2 * len] = '\0';
	avio_printf(pb,
		"<param name=\"%s\" value=\"%s\" valuetype=\"data\"/>\n", name, buf);
}

static int mov_write_isml_manifest(AVIOContext *pb, MOVMuxContext *mov,
	AVFormatContext *s)
{
	int64_t pos = avio_tell(pb);
	int i;
	int64_t manifest_bit_rate = 0;
	AVCPBProperties *props = NULL;

	static const uint8_t uuid[] = {
		0xa5, 0xd4, 0x0b, 0x30, 0xe8, 0x14, 0x11, 0xdd,
		0xba, 0x2f, 0x08, 0x00, 0x20, 0x0c, 0x9a, 0x66
	};

	avio_wb32(pb, 0);
	ffio_wfourcc(pb, "uuid");
	avio_write(pb, uuid, sizeof(uuid));
	avio_wb32(pb, 0);

	avio_printf(pb, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
	avio_printf(pb, "<smil xmlns=\"http://www.w3.org/2001/SMIL20/Language\">\n");
	avio_printf(pb, "<head>\n");
	if (!(mov->fc->flags & AVFMT_FLAG_BITEXACT))
		avio_printf(pb, "<meta name=\"creator\" content=\"%s\" />\n",
			LIBAVFORMAT_IDENT);
	avio_printf(pb, "</head>\n");
	avio_printf(pb, "<body>\n");
	avio_printf(pb, "<switch>\n");

	mov_setup_track_ids(mov, s);

	for (i = 0; i < mov->nb_streams; i++) {
		MOVTrack *track = &mov->tracks[i];
		const char *type;
		int track_id = track->track_id;

		AVStream *st = track->st;
		AVDictionaryEntry *lang = av_dict_get(st->metadata, "language", NULL, 0);

		if (track->par->codec_type == AV_MEDIA_TYPE_VIDEO)
			type = "video";
		else if (track->par->codec_type == AV_MEDIA_TYPE_AUDIO)
			type = "audio";
		else
			continue;


		props = (AVCPBProperties *)av_stream_get_side_data(track->st,
				AV_PKT_DATA_CPB_PROPERTIES,	NULL);

		if (track->par->bit_rate)
			manifest_bit_rate = track->par->bit_rate;
		else if (props)
			manifest_bit_rate = props->max_bitrate;


		avio_printf(pb, "<%s systemBitrate=\"%"PRId64"\">\n", type,
			manifest_bit_rate);
		param_write_int(pb, "systemBitrate", manifest_bit_rate);
		param_write_int(pb, "trackID", track_id);
		param_write_string(pb, "systemLanguage", lang ? lang->value : "und");
		if (track->par->codec_type == AV_MEDIA_TYPE_VIDEO) {
			if (track->par->codec_id == AV_CODEC_TYPE_H264) {
				uint8_t *ptr;
				int size = track->par->extradata_size;
				if (!ff_avc_write_annexb_extradata(track->par->extradata,
						&ptr, &size)) {
					param_write_hex(pb, "CodecPrivateData",
						ptr ? ptr : track->par->extradata,
						size);
					av_free(ptr);
				}
				param_write_string(pb, "FourCC", "H264");
			} else if (track->par->codec_id == AV_CODEC_TYPE_VC1) {
				param_write_string(pb, "FourCC", "WVC1");
				param_write_hex(pb, "CodecPrivateData",
					track->par->extradata,
					track->par->extradata_size);
			}
			param_write_int(pb, "MaxWidth", track->par->width);
			param_write_int(pb, "MaxHeight", track->par->height);
			param_write_int(pb, "DisplayWidth", track->par->width);
			param_write_int(pb, "DisplayHeight", track->par->height);
		} else {
			if (track->par->codec_id == AV_CODEC_TYPE_AAC) {
				switch (track->par->profile) {
				case FF_PROFILE_AAC_HE_V2:
					param_write_string(pb, "FourCC", "AACP");
					break;
				case FF_PROFILE_AAC_HE:
					param_write_string(pb, "FourCC", "AACH");
					break;
				default:
					param_write_string(pb, "FourCC", "AACL");
				}
			} else if (track->par->codec_id == AV_CODEC_TYPE_WMAPRO) {
				param_write_string(pb, "FourCC", "WMAP");
			}
			param_write_hex(pb, "CodecPrivateData", track->par->extradata,
				track->par->extradata_size);
			param_write_int(pb, "AudioTag",
				ff_codec_get_tag(ff_codec_wav_tags,
					track->par->codec_id));
			param_write_int(pb, "Channels", track->par->channels);
			param_write_int(pb, "SamplingRate", track->par->sample_rate);
			param_write_int(pb, "BitsPerSample", 16);
			param_write_int(pb, "PacketSize", track->par->block_align ?
				track->par->block_align : 4);
		}
		avio_printf(pb, "</%s>\n", type);
	}
	avio_printf(pb, "</switch>\n");
	avio_printf(pb, "</body>\n");
	avio_printf(pb, "</smil>\n");

	return update_size(pb, pos);
}

static int mov_write_mfhd_tag(AVIOContext *pb, MOVMuxContext *mov)
{
	avio_wb32(pb, 16);
	ffio_wfourcc(pb, "mfhd");
	avio_wb32(pb, 0);
	avio_wb32(pb, mov->fragments);
	return 0;
}

static uint32_t get_sample_flags(MOVTrack *track, MOVIentry *entry)
{
	return entry->flags & MOV_SYNC_SAMPLE ? MOV_FRAG_SAMPLE_FLAG_DEPENDS_NO :
		(MOV_FRAG_SAMPLE_FLAG_DEPENDS_YES | MOV_FRAG_SAMPLE_FLAG_IS_NON_SYNC);
}

static int mov_write_tfhd_tag(AVIOContext *pb, MOVMuxContext *mov,
	MOVTrack *track, int64_t moof_offset)
{
	int64_t pos = avio_tell(pb);
	uint32_t flags = MOV_TFHD_DEFAULT_SIZE | MOV_TFHD_DEFAULT_DURATION |
		MOV_TFHD_BASE_DATA_OFFSET;
	if (!track->entry)
		flags |= MOV_TFHD_DURATION_IS_EMPTY;
	else
		flags |= MOV_TFHD_DEFAULT_FLAGS;

	if (mov->flags & FF_MOV_FLAG_OMIT_TFHD_OFFSET)
		flags &= ~MOV_TFHD_BASE_DATA_OFFSET;
	if (mov->flags & FF_MOV_FLAG_DEFAULT_BASE_MOOF) {
		flags &= ~MOV_TFHD_BASE_DATA_OFFSET;
		flags |= MOV_TFHD_DEFAULT_BASE_IS_MOOF;
	}

	/* Don't set a default sample size, the silverlight player refuses
	 * to play files with that set. Don't set a default sample duration,
	 * WMP freaks out if it is set. Don't set a base data offset, PIFF
	 * file format says it MUST NOT be set. */
	if (track->mode == MODE_ISM)
		flags &= ~(MOV_TFHD_DEFAULT_SIZE | MOV_TFHD_DEFAULT_DURATION |
				MOV_TFHD_BASE_DATA_OFFSET);

	avio_wb32(pb, 0); /* size placeholder */
	ffio_wfourcc(pb, "tfhd");
	avio_w8(pb, 0); /* version */
	avio_wb24(pb, flags);

	avio_wb32(pb, track->track_id); /* track-id */
	if (flags & MOV_TFHD_BASE_DATA_OFFSET)
		avio_wb64(pb, moof_offset);
	if (flags & MOV_TFHD_DEFAULT_DURATION) {
		track->default_duration = get_cluster_duration(track, 0);
		avio_wb32(pb, track->default_duration);
	}
	if (flags & MOV_TFHD_DEFAULT_SIZE) {
		track->default_size = track->entry ? track->cluster[0].size : 1;
		avio_wb32(pb, track->default_size);
	} else
		track->default_size = -1;

	if (flags & MOV_TFHD_DEFAULT_FLAGS) {
		/* Set the default flags based on the second sample, if available.
		 * If the first sample is different,
		 * that can be signaled via a separate field. */
		if (track->entry > 1)
			track->default_sample_flags =
				get_sample_flags(track, &track->cluster[1]);
		else
			track->default_sample_flags =
				track->par->codec_type == AV_MEDIA_TYPE_VIDEO ?
				(MOV_FRAG_SAMPLE_FLAG_DEPENDS_YES |
					MOV_FRAG_SAMPLE_FLAG_IS_NON_SYNC) :
				MOV_FRAG_SAMPLE_FLAG_DEPENDS_NO;
		avio_wb32(pb, track->default_sample_flags);
	}

	return update_size(pb, pos);
}

static int mov_write_trun_tag(AVIOContext *pb, MOVMuxContext *mov,
	MOVTrack *track, int moof_size,
	int first, int end)
{
	int64_t pos = avio_tell(pb);
	uint32_t flags = MOV_TRUN_DATA_OFFSET;
	int i;

	for (i = first; i < end; i++) {
		if (get_cluster_duration(track, i) != track->default_duration)
			flags |= MOV_TRUN_SAMPLE_DURATION;
		if (track->cluster[i].size != track->default_size)
			flags |= MOV_TRUN_SAMPLE_SIZE;
		if (i > first
			&& get_sample_flags(track, &track->cluster[i]) !=
			track->default_sample_flags)
			flags |= MOV_TRUN_SAMPLE_FLAGS;
	}
	if (!(flags & MOV_TRUN_SAMPLE_FLAGS) && track->entry > 0 &&
		get_sample_flags(track, &track->cluster[0]) !=
		track->default_sample_flags)
		flags |= MOV_TRUN_FIRST_SAMPLE_FLAGS;
	if (track->flags & MOV_TRACK_CTTS)
		flags |= MOV_TRUN_SAMPLE_CTS;

	avio_wb32(pb, 0); /* size placeholder */
	ffio_wfourcc(pb, "trun");
	avio_w8(pb, 0); /* version */
	avio_wb24(pb, flags);

	avio_wb32(pb, end - first); /* sample count */
	if (mov->flags & FF_MOV_FLAG_OMIT_TFHD_OFFSET &&
		!(mov->flags & FF_MOV_FLAG_DEFAULT_BASE_MOOF) &&
		!mov->first_trun)
		/* Later tracks follow immediately after the previous one */
		avio_wb32(pb, 0);
	else
		avio_wb32(pb, moof_size + 8 + track->data_offset +
			track->cluster[first].pos); /* data offset */
	if (flags & MOV_TRUN_FIRST_SAMPLE_FLAGS)
		avio_wb32(pb, get_sample_flags(track, &track->cluster[first]));

	for (i = first; i < end; i++) {
		if (flags & MOV_TRUN_SAMPLE_DURATION)
			avio_wb32(pb, get_cluster_duration(track, i));
		if (flags & MOV_TRUN_SAMPLE_SIZE)
			avio_wb32(pb, track->cluster[i].size);
		if (flags & MOV_TRUN_SAMPLE_FLAGS)
			avio_wb32(pb, get_sample_flags(track, &track->cluster[i]));
		if (flags & MOV_TRUN_SAMPLE_CTS)
			avio_wb32(pb, track->cluster[i].cts);
	}

	mov->first_trun = 0;
	return update_size(pb, pos);
}

static int mov_write_tfxd_tag(AVIOContext *pb, MOVTrack *track)
{
	int64_t pos = avio_tell(pb);
	static const uint8_t uuid[] = {
		0x6d, 0x1d, 0x9b, 0x05, 0x42, 0xd5, 0x44, 0xe6,
		0x80, 0xe2, 0x14, 0x1d, 0xaf, 0xf7, 0x57, 0xb2
	};

	avio_wb32(pb, 0); /* size placeholder */
	ffio_wfourcc(pb, "uuid");
	avio_write(pb, uuid, sizeof(uuid));
	avio_w8(pb, 1);
	avio_wb24(pb, 0);
	avio_wb64(pb, track->start_dts + track->frag_start +
		track->cluster[0].cts);
	avio_wb64(pb, track->end_pts -
		(track->cluster[0].dts + track->cluster[0].cts));

	return update_size(pb, pos);
}

static int mov_write_tfrf_tag(AVIOContext *pb, MOVMuxContext *mov,
	MOVTrack *track, int entry)
{
	int n = track->nb_frag_info - 1 - entry, i;
	int size = 8 + 16 + 4 + 1 + 16 * n;
	static const uint8_t uuid[] = {
		0xd4, 0x80, 0x7e, 0xf2, 0xca, 0x39, 0x46, 0x95,
		0x8e, 0x54, 0x26, 0xcb, 0x9e, 0x46, 0xa7, 0x9f
	};

	if (entry < 0)
		return 0;

	avio_seek(pb, track->frag_info[entry].tfrf_offset, SEEK_SET);
	avio_wb32(pb, size);
	ffio_wfourcc(pb, "uuid");
	avio_write(pb, uuid, sizeof(uuid));
	avio_w8(pb, 1);
	avio_wb24(pb, 0);
	avio_w8(pb, n);
	for (i = 0; i < n; i++) {
		int index = entry + 1 + i;
		avio_wb64(pb, track->frag_info[index].time);
		avio_wb64(pb, track->frag_info[index].duration);
	}
	if (n < mov->ism_lookahead) {
		int free_size = 16 * (mov->ism_lookahead - n);
		avio_wb32(pb, free_size);
		ffio_wfourcc(pb, "free");
		ffio_fill(pb, 0, free_size - 8);
	}

	return 0;
}

static int mov_write_tfrf_tags(AVIOContext *pb, MOVMuxContext *mov,
	MOVTrack *track)
{
	int64_t pos = avio_tell(pb);
	int i;
	for (i = 0; i < mov->ism_lookahead; i++) {
		/* Update the tfrf tag for the last ism_lookahead fragments,
		 * nb_frag_info - 1 is the next fragment to be written. */
		mov_write_tfrf_tag(pb, mov, track, track->nb_frag_info - 2 - i);
	}
	avio_seek(pb, pos, SEEK_SET);
	return 0;
}

static int mov_add_tfra_entries(AVIOContext *pb, MOVMuxContext *mov, int tracks,
	int size)
{
	int i;
	for (i = 0; i < mov->nb_streams; i++) {
		MOVTrack *track = &mov->tracks[i];
		MOVFragmentInfo *info;
		if ((tracks >= 0 && i != tracks) || !track->entry)
			continue;
		track->nb_frag_info++;
		if (track->nb_frag_info >= track->frag_info_capacity) {
			unsigned new_capacity = track->nb_frag_info
				+ MOV_FRAG_INFO_ALLOC_INCREMENT;
			if (av_reallocp_array(&track->frag_info,
					new_capacity,
					sizeof(*track->frag_info)))
				return AVERROR(ENOMEM);
			track->frag_info_capacity = new_capacity;
		}
		info = &track->frag_info[track->nb_frag_info - 1];
		info->offset   = avio_tell(pb);
		info->size     = size;
		/* Try to recreate the original pts for the first packet
		 from the fields we have stored */
		info->time     = track->start_dts + track->frag_start +
			track->cluster[0].cts;
		info->duration = track->end_pts -
			(track->cluster[0].dts + track->cluster[0].cts);
		/* If the pts is less than zero, we will have trimmed
		 away parts of the media track using an edit list,
		 and the corresponding start presentation time is zero.*/
		if (info->time < 0) {
			info->duration += info->time;
			info->time = 0;
		}
		info->tfrf_offset = 0;
		mov_write_tfrf_tags(pb, mov, track);
	}
	return 0;
}

static int mov_write_tfdt_tag(AVIOContext *pb, MOVTrack *track)
{
	int64_t pos = avio_tell(pb);

	avio_wb32(pb, 0); /* size */
	ffio_wfourcc(pb, "tfdt");
	avio_w8(pb, 1); /* version */
	avio_wb24(pb, 0);
	avio_wb64(pb, track->frag_start);
	return update_size(pb, pos);
}

static int mov_write_traf_tag(AVIOContext *pb, MOVMuxContext *mov,
	MOVTrack *track, int64_t moof_offset,
	int moof_size)
{
	int64_t pos = avio_tell(pb);
	int i, start = 0;
	avio_wb32(pb, 0); /* size placeholder */
	ffio_wfourcc(pb, "traf");

	mov_write_tfhd_tag(pb, mov, track, moof_offset);
	if (mov->mode != MODE_ISM)
		mov_write_tfdt_tag(pb, track);
	for (i = 1; i < track->entry; i++) {
		if (track->cluster[i].pos != track->cluster[i - 1].pos
			+ track->cluster[i - 1].size) {
			mov_write_trun_tag(pb, mov, track, moof_size, start, i);
			start = i;
		}
	}
	mov_write_trun_tag(pb, mov, track, moof_size, start, track->entry);
	if (mov->mode == MODE_ISM) {
		mov_write_tfxd_tag(pb, track);

		if (mov->ism_lookahead) {
			int i, size = 16 + 4 + 1 + 16 * mov->ism_lookahead;

			if (track->nb_frag_info > 0) {
				MOVFragmentInfo *info =
					&track->frag_info[track->nb_frag_info - 1];
				if (!info->tfrf_offset)
					info->tfrf_offset = avio_tell(pb);
			}
			avio_wb32(pb, 8 + size);
			ffio_wfourcc(pb, "free");
			for (i = 0; i < size; i++)
				avio_w8(pb, 0);
		}
	}

	return update_size(pb, pos);
}

static int mov_write_moof_tag_internal(AVIOContext *pb, MOVMuxContext *mov,
	int tracks, int moof_size)
{
	int64_t pos = avio_tell(pb);
	int i;

	avio_wb32(pb, 0); /* size placeholder */
	ffio_wfourcc(pb, "moof");
	mov->first_trun = 1;

	mov_write_mfhd_tag(pb, mov);
	for (i = 0; i < mov->nb_streams; i++) {
		MOVTrack *track = &mov->tracks[i];
		if (tracks >= 0 && i != tracks)
			continue;
		if (!track->entry)
			continue;
		mov_write_traf_tag(pb, mov, track, pos, moof_size);
	}

	return update_size(pb, pos);
}

static int mov_write_sidx_tag(AVIOContext *pb,
	MOVTrack *track, int ref_size, int total_sidx_size)
{
	int64_t pos = avio_tell(pb), offset_pos, end_pos;
	int64_t presentation_time, duration, offset;
	int starts_with_SAP, i, entries;

	if (track->entry) {
		entries = 1;
		presentation_time = track->start_dts + track->frag_start +
			track->cluster[0].cts;
		duration = track->end_pts -
			(track->cluster[0].dts + track->cluster[0].cts);
		starts_with_SAP = track->cluster[0].flags & MOV_SYNC_SAMPLE;

		/* pts<0 should be cut away using edts */
		if (presentation_time < 0) {
			duration += presentation_time;
			presentation_time = 0;
		}
	} else {
		entries = track->nb_frag_info;
		if (entries <= 0)
			return 0;
		presentation_time = track->frag_info[0].time;
	}

	avio_wb32(pb, 0); /* size */
	ffio_wfourcc(pb, "sidx");
	avio_w8(pb, 1); /* version */
	avio_wb24(pb, 0);
	avio_wb32(pb, track->track_id); /* reference_ID */
	avio_wb32(pb, track->timescale); /* timescale */
	avio_wb64(pb, presentation_time); /* earliest_presentation_time */
	offset_pos = avio_tell(pb);
	avio_wb64(pb, 0); /* first_offset (offset to referenced moof) */
	avio_wb16(pb, 0); /* reserved */

	avio_wb16(pb, entries); /* reference_count */
	for (i = 0; i < entries; i++) {
		if (!track->entry) {
			if (i > 1 && track->frag_info[i].offset !=
				track->frag_info[i - 1].offset +
				track->frag_info[i - 1].size) {
				av_log(NULL, AV_LOG_ERROR,
					"Non-consecutive fragments, writing incorrect sidx\n");
			}
			duration = track->frag_info[i].duration;
			ref_size = track->frag_info[i].size;
			starts_with_SAP = 1;
		}
		/* reference_type (0 = media) | referenced_size */
		avio_wb32(pb, (0 << 31) | (ref_size & 0x7fffffff));
		avio_wb32(pb, duration); /* subsegment_duration */
		avio_wb32(pb, (starts_with_SAP << 31) | (0 << 28) |
			0); /* starts_with_SAP | SAP_type | SAP_delta_time */
	}

	end_pos = avio_tell(pb);
	offset = pos + total_sidx_size - end_pos;
	avio_seek(pb, offset_pos, SEEK_SET);
	avio_wb64(pb, offset);
	avio_seek(pb, end_pos, SEEK_SET);
	return update_size(pb, pos);
}

static int mov_write_sidx_tags(AVIOContext *pb, MOVMuxContext *mov,
	int tracks, int ref_size)
{
	int i, round, ret;
	AVIOContext *avio_buf;
	int total_size = 0;
	for (round = 0; round < 2; round++) {
		/* First run one round to calculate the total size of all
		 sidx atoms.
		 This would be much simpler if we'd only write one sidx
		 atom, for the first track in the moof.*/
		if (round == 0) {
			ret = ffio_open_null_buf(&avio_buf);
			if (ret < 0)
				return ret;
		} else {
			avio_buf = pb;
		}
		for (i = 0; i < mov->nb_streams; i++) {
			MOVTrack *track = &mov->tracks[i];
			if (tracks >= 0 && i != tracks)
				continue;
			/* When writing a sidx for the full file, entry is 0, but
			 we want to include all tracks. ref_size is 0 in this case,
			 since we read it from frag_info instead.*/
			if (!track->entry && ref_size > 0)
				continue;
			total_size -= mov_write_sidx_tag(avio_buf, track, ref_size,
					total_size);
		}
		if (round == 0)
			total_size = ffio_close_null_buf(avio_buf);
	}
	return 0;
}

static int mov_write_moof_tag(AVIOContext *pb, MOVMuxContext *mov, int tracks,
	int64_t mdat_size)
{
	AVIOContext *avio_buf;
	int ret, moof_size;

	ret = ffio_open_null_buf(&avio_buf);
	if (ret < 0)
		return ret;
	mov_write_moof_tag_internal(avio_buf, mov, tracks, 0);
	moof_size = ffio_close_null_buf(avio_buf);

	if (mov->flags & FF_MOV_FLAG_DASH && !(mov->flags & FF_MOV_FLAG_GLOBAL_SIDX))
		mov_write_sidx_tags(pb, mov, tracks, moof_size + 8 + mdat_size);

	ret = mov_add_tfra_entries(pb, mov, tracks, moof_size + 8 + mdat_size);
	if (ret < 0)
		return ret;

	return mov_write_moof_tag_internal(pb, mov, tracks, moof_size);
}

static int mov_write_tfra_tag(AVIOContext *pb, MOVTrack *track)
{
	int64_t pos = avio_tell(pb);
	int i;

	avio_wb32(pb, 0); /* size placeholder */
	ffio_wfourcc(pb, "tfra");
	avio_w8(pb, 1); /* version */
	avio_wb24(pb, 0);

	avio_wb32(pb, track->track_id);
	avio_wb32(pb, 0); /* length of traf/trun/sample num */
	avio_wb32(pb, track->nb_frag_info);
	for (i = 0; i < track->nb_frag_info; i++) {
		avio_wb64(pb, track->frag_info[i].time);
		avio_wb64(pb, track->frag_info[i].offset + track->data_offset);
		avio_w8(pb, 1); /* traf number */
		avio_w8(pb, 1); /* trun number */
		avio_w8(pb, 1); /* sample number */
	}

	return update_size(pb, pos);
}

static int mov_write_mfra_tag(AVIOContext *pb, MOVMuxContext *mov)
{
	int64_t pos = avio_tell(pb);
	int i;

	avio_wb32(pb, 0); /* size placeholder */
	ffio_wfourcc(pb, "mfra");
	/* An empty mfra atom is enough to indicate to the publishing point that
	 * the stream has ended. */
	if (mov->flags & FF_MOV_FLAG_ISML)
		return update_size(pb, pos);

	for (i = 0; i < mov->nb_streams; i++) {
		MOVTrack *track = &mov->tracks[i];
		if (track->nb_frag_info)
			mov_write_tfra_tag(pb, track);
	}

	avio_wb32(pb, 16);
	ffio_wfourcc(pb, "mfro");
	avio_wb32(pb, 0); /* version + flags */
	avio_wb32(pb, avio_tell(pb) + 4 - pos);

	return update_size(pb, pos);
}
#endif

static int mov_write_mdat_tag(AVIOContext *pb, MOVMuxContext *mov)
{
#if 0
	avio_wb32(pb, 8);    /* placeholder for extended size field (64 bit) */
	ffio_wfourcc(pb, mov->mode == MODE_MOV ? "wide" : "free");
#endif

	/* mov->mdat_pos = avio_tell(pb); */
	avio_wb32(pb, 0); /* size placeholder*/
	ffio_wfourcc(pb, "mdat");
	return 0;
}

/* TODO: This needs to be more general */
static int mov_write_ftyp_tag(AVIOContext *pb, AVFormatContext *s)
{
	MOVMuxContext *mov = s->priv_data;
	/* int64_t pos = avio_tell(pb); */
	int has_h264 = 0;
	int minor = 0x200;
	int i;
	int size = 0;
	for (i = 0; i < s->nb_streams; i++) {
		AVStream *st = s->streams[i];
		if (st->codecpar->codec_id == AV_CODEC_TYPE_H264)
			has_h264 = 1;
	}

	if (mov->mode == MODE_MOV)
		size = 16 + 4;
	else if (mov->mode == MODE_MP4)
		size = 16 + 12 + has_h264 * 4;
	else
		size = 16 + 8 + has_h264 * 4;

	avio_wb32(pb, size); /* size */
	ffio_wfourcc(pb, "ftyp");

	if (mov->mode == MODE_MP4)
		ffio_wfourcc(pb, "isom");
	else
		ffio_wfourcc(pb, "qt  ");

	avio_wb32(pb, minor);

	if (mov->mode == MODE_MOV)
		ffio_wfourcc(pb, "qt  ");
	else {
		ffio_wfourcc(pb, "isom");
		ffio_wfourcc(pb, "iso2");
		if (has_h264)
			ffio_wfourcc(pb, "avc1");
	}

	if (mov->mode == MODE_MP4)
		ffio_wfourcc(pb, "mp41");

	mov->mdat_pos = size/* + 8*/;/* finally write free data, so don't add 8*/
	return 0; /* update_size(pb, pos); */
}

#if 0
static int mov_write_uuidprof_tag(AVIOContext *pb, AVFormatContext *s)
{
	AVStream       *video_st    = s->streams[0];
	AV_CodecParameters *video_par = s->streams[0]->codecpar;
	AV_CodecParameters *audio_par = s->streams[1]->codecpar;
	int audio_rate = audio_par->sample_rate;
	int64_t frame_rate = (video_st->avg_frame_rate.num * 0x10000LL) /
		video_st->avg_frame_rate.den;
	int audio_kbitrate = audio_par->bit_rate / 1000;
	int video_kbitrate = FFMIN(video_par->bit_rate / 1000, 800 - audio_kbitrate);

	if (frame_rate < 0 || frame_rate > INT32_MAX) {
		av_log(s, AV_LOG_ERROR, "Frame rate %f outside supported range\n",
			frame_rate / (double)0x10000);
		return AVERROR(EINVAL);
	}

	avio_wb32(pb, 0x94); /* size */
	ffio_wfourcc(pb, "uuid");
	ffio_wfourcc(pb, "PROF");

	avio_wb32(pb, 0x21d24fce); /* 96 bit UUID */
	avio_wb32(pb, 0xbb88695c);
	avio_wb32(pb, 0xfac9c740);

	avio_wb32(pb, 0x0);  /* ? */
	avio_wb32(pb, 0x3);  /* 3 sections ? */

	avio_wb32(pb, 0x14); /* size */
	ffio_wfourcc(pb, "FPRF");
	avio_wb32(pb, 0x0);  /* ? */
	avio_wb32(pb, 0x0);  /* ? */
	avio_wb32(pb, 0x0);  /* ? */

	avio_wb32(pb, 0x2c);  /* size */
	ffio_wfourcc(pb, "APRF"); /* audio */
	avio_wb32(pb, 0x0);
	avio_wb32(pb, 0x2);   /* TrackID */
	ffio_wfourcc(pb, "mp4a");
	avio_wb32(pb, 0x20f);
	avio_wb32(pb, 0x0);
	avio_wb32(pb, audio_kbitrate);
	avio_wb32(pb, audio_kbitrate);
	avio_wb32(pb, audio_rate);
	avio_wb32(pb, audio_par->channels);

	avio_wb32(pb, 0x34);  /* size */
	ffio_wfourcc(pb, "VPRF");   /* video */
	avio_wb32(pb, 0x0);
	avio_wb32(pb, 0x1);    /* TrackID */
	if (video_par->codec_id == AV_CODEC_TYPE_H264) {
		ffio_wfourcc(pb, "avc1");
		avio_wb16(pb, 0x014D);
		avio_wb16(pb, 0x0015);
	} else {
		ffio_wfourcc(pb, "mp4v");
		avio_wb16(pb, 0x0000);
		avio_wb16(pb, 0x0103);
	}
	avio_wb32(pb, 0x0);
	avio_wb32(pb, video_kbitrate);
	avio_wb32(pb, video_kbitrate);
	avio_wb32(pb, frame_rate);
	avio_wb32(pb, frame_rate);
	avio_wb16(pb, video_par->width);
	avio_wb16(pb, video_par->height);
	avio_wb32(pb, 0x010001); /* ? */

	return 0;
}

static int mov_write_identification(AVIOContext *pb, AVFormatContext *s)
{
	MOVMuxContext *mov = s->priv_data;
	int i;

	mov_write_ftyp_tag(pb, s);
	if (mov->mode == MODE_PSP) {
		int video_streams_nb = 0, audio_streams_nb = 0, other_streams_nb = 0;
		for (i = 0; i < s->nb_streams; i++) {
			AVStream *st = s->streams[i];
			if (st->codecpar->codec_type == AV_MEDIA_TYPE_VIDEO)
				video_streams_nb++;
			else if (st->codecpar->codec_type == AV_MEDIA_TYPE_AUDIO)
				audio_streams_nb++;
			else
				other_streams_nb++;
		}

		if (video_streams_nb != 1 || audio_streams_nb != 1 || other_streams_nb) {
			av_log(s, AV_LOG_ERROR,
				"PSP mode need one video and one audio stream\n");
			return AVERROR(EINVAL);
		}
		return mov_write_uuidprof_tag(pb, s);
	}
	return 0;
}

static int mov_parse_mpeg2_frame(AVPacket *pkt, uint32_t *flags)
{
	uint32_t c = -1;
	int i, closed_gop = 0;

	for (i = 0; i < pkt->size - 4; i++) {
		c = (c << 8) + pkt->data[i];
		if (c == 0x1b8) { /* gop */
			closed_gop = pkt->data[i + 4] >> 6 & 0x01;
		} else if (c == 0x100) { /* pic */
			int temp_ref = (pkt->data[i + 1] << 2) | (pkt->data[i + 2] >> 6);
			if (!temp_ref || closed_gop) /* I picture is not reordered */
				*flags = MOV_SYNC_SAMPLE;
			else
				*flags = MOV_PARTIAL_SYNC_SAMPLE;
			break;
		}
	}
	return 0;
}

static void mov_parse_vc1_frame(AVPacket *pkt, MOVTrack *trk)
{
	const uint8_t *start, *next, *end = pkt->data + pkt->size;
	int seq = 0, entry = 0;
	int key = pkt->flags & AV_PKT_FLAG_KEY;
	start = find_next_marker(pkt->data, end);
	for (next = start; next < end; start = next) {
		next = find_next_marker(start + 4, end);
		switch (AV_RB32(start)) {
		case VC1_CODE_SEQHDR:
			seq = 1;
			break;
		case VC1_CODE_ENTRYPOINT:
			entry = 1;
			break;
		case VC1_CODE_SLICE:
			trk->vc1_info.slices = 1;
			break;
		}
	}
	if (!trk->entry && trk->vc1_info.first_packet_seen)
		trk->vc1_info.first_frag_written = 1;
	if (!trk->entry && !trk->vc1_info.first_frag_written) {
		/* First packet in first fragment */
		trk->vc1_info.first_packet_seq   = seq;
		trk->vc1_info.first_packet_entry = entry;
		trk->vc1_info.first_packet_seen  = 1;
	} else if ((seq && !trk->vc1_info.packet_seq) ||
		(entry && !trk->vc1_info.packet_entry)) {
		int i;
		for (i = 0; i < trk->entry; i++)
			trk->cluster[i].flags &= ~MOV_SYNC_SAMPLE;
		trk->has_keyframes = 0;
		if (seq)
			trk->vc1_info.packet_seq = 1;
		if (entry)
			trk->vc1_info.packet_entry = 1;
		if (!trk->vc1_info.first_frag_written) {
			/* First fragment */
			if ((!seq   || trk->vc1_info.first_packet_seq) &&
				(!entry || trk->vc1_info.first_packet_entry)) {
				/* First packet had the same headers as this one,
				 * readd thesync sample flag. */
				trk->cluster[0].flags |= MOV_SYNC_SAMPLE;
				trk->has_keyframes = 1;
			}
		}
	}
	if (trk->vc1_info.packet_seq && trk->vc1_info.packet_entry)
		key = seq && entry;
	else if (trk->vc1_info.packet_seq)
		key = seq;
	else if (trk->vc1_info.packet_entry)
		key = entry;
	if (key) {
		trk->cluster[trk->entry].flags |= MOV_SYNC_SAMPLE;
		trk->has_keyframes++;
	}
}

static int mov_flush_fragment_interleaving(AVFormatContext *s, MOVTrack *track)
{
	MOVMuxContext *mov = s->priv_data;
	int ret, buf_size;
	uint8_t *buf;
	int i, offset;

	if (!track->mdat_buf)
		return 0;
	if (mov->mdat_buf == NULL) {
		ret = avio_open_dyn_buf(&mov->mdat_buf);
		if (ret < 0)
			return ret;
	}
	buf_size = avio_close_dyn_buf(track->mdat_buf, &buf);
	track->mdat_buf = NULL;

	offset = avio_tell(mov->mdat_buf);
	avio_write(mov->mdat_buf, buf, buf_size);
	av_free(buf);

	for (i = track->entries_flushed; i < track->entry; i++)
		track->cluster[i].pos += offset;
	track->entries_flushed = track->entry;
	return 0;
}

static int mov_flush_fragment(AVFormatContext *s, int force)
{
	MOVMuxContext *mov = s->priv_data;
	int i, first_track = -1;
	int64_t mdat_size = 0;
	int ret;
	int has_video = 0, starts_with_key = 0, first_video_track = 1;

	if (!(mov->flags & FF_MOV_FLAG_FRAGMENT))
		return 0;

	/* Try to fill in the duration of the last packet in each stream
	 from queued packets in the interleave queues. If the flushing
	 of fragments was triggered automatically by an AVPacket, we
	 already have reliable info for the end of that track, but other
	 tracks may need to be filled in.*/
	for (i = 0; i < s->nb_streams; i++) {
		MOVTrack *track = &mov->tracks[i];
		if (!track->end_reliable) {
			int64_t ts_offset;
			const AVPacket *next = ff_interleaved_peek(s, i, &ts_offset);
			if (next) {
				track->track_duration = next->dts -
					track->start_dts + ts_offset;
				if (next->pts != AV_NOPTS_VALUE)
					track->end_pts = next->pts;
				else
					track->end_pts = next->dts;
				track->end_pts += ts_offset;
			}
		}
	}

	for (i = 0; i < mov->nb_streams; i++) {
		MOVTrack *track = &mov->tracks[i];
		if (track->entry <= 1)
			continue;
		/* Sample durations are calculated as the diff of dts values,
		 but for the last sample in a fragment, we don't know the dts
		 of the first sample in the next fragment, so we have to rely
		 on what was set as duration in the AVPacket. Not all callers
		 set this though, so we might want to replace it with an
		 estimate if it currently is zero.*/
		if (get_cluster_duration(track, track->entry - 1) != 0)
			continue;
		/* Use the duration (i.e. dts diff) of the second last sample for
		 the last one. This is a wild guess (and fatal if it turns out
		 to be too long), but probably the best we can do - having a zero
		 duration is bad as well.*/
		track->track_duration += get_cluster_duration(track, track->entry - 2);
		track->end_pts        += get_cluster_duration(track, track->entry - 2);
		if (!mov->missing_duration_warned) {
			av_log(s, AV_LOG_WARNING,
				"Estimating the duration of the last packet in a "
				"fragment, consider setting the duration field in "
				"AVPacket instead.\n");
			mov->missing_duration_warned = 1;
		}
	}

	if (!mov->moov_written) {
		int64_t pos = avio_tell(s->pb);
		uint8_t *buf;
		int buf_size, moov_size;

		for (i = 0; i < mov->nb_streams; i++)
			if (!mov->tracks[i].entry)
				break;
		/* Don't write the initial moov unless all tracks have data */
		if (i < mov->nb_streams && !force)
			return 0;

		moov_size = get_moov_size(s);
		for (i = 0; i < mov->nb_streams; i++)
			mov->tracks[i].data_offset = pos + moov_size + 8;

		avio_write_marker(s->pb, AV_NOPTS_VALUE, AVIO_DATA_MARKER_HEADER);
		if (mov->flags & FF_MOV_FLAG_DELAY_MOOV)
			mov_write_identification(s->pb, s);
		ret = mov_write_moov_tag(s->pb, mov, s);
		if (ret < 0)
			return ret;

		if (mov->flags & FF_MOV_FLAG_DELAY_MOOV) {
			if (mov->flags & FF_MOV_FLAG_GLOBAL_SIDX)
				mov->reserved_header_pos = avio_tell(s->pb);
			avio_flush(s->pb);
			mov->moov_written = 1;
			return 0;
		}

		buf_size = avio_close_dyn_buf(mov->mdat_buf, &buf);
		mov->mdat_buf = NULL;
		avio_wb32(s->pb, buf_size + 8);
		ffio_wfourcc(s->pb, "mdat");
		avio_write(s->pb, buf, buf_size);
		av_free(buf);

		if (mov->flags & FF_MOV_FLAG_GLOBAL_SIDX)
			mov->reserved_header_pos = avio_tell(s->pb);

		mov->moov_written = 1;
		mov->mdat_size = 0;
		for (i = 0; i < mov->nb_streams; i++) {
			if (mov->tracks[i].entry)
				mov->tracks[i].frag_start += mov->tracks[i].start_dts +
					mov->tracks[i].track_duration -
					mov->tracks[i].cluster[0].dts;
			mov->tracks[i].entry = 0;
			mov->tracks[i].end_reliable = 0;
		}
		avio_flush(s->pb);
		return 0;
	}

	if (mov->frag_interleave) {
		for (i = 0; i < mov->nb_streams; i++) {
			MOVTrack *track = &mov->tracks[i];
			int ret = mov_flush_fragment_interleaving(s, track);
			if (ret < 0)
				return ret;
		}

		if (!mov->mdat_buf)
			return 0;
		mdat_size = avio_tell(mov->mdat_buf);
	}

	for (i = 0; i < mov->nb_streams; i++) {
		MOVTrack *track = &mov->tracks[i];
		if (mov->flags & FF_MOV_FLAG_SEPARATE_MOOF || mov->frag_interleave)
			track->data_offset = 0;
		else
			track->data_offset = mdat_size;
		if (track->par->codec_type == AV_MEDIA_TYPE_VIDEO) {
			has_video = 1;
			if (first_video_track) {
				if (track->entry)
					starts_with_key =
						track->cluster[0].flags & MOV_SYNC_SAMPLE;
				first_video_track = 0;
			}
		}
		if (!track->entry)
			continue;
		if (track->mdat_buf)
			mdat_size += avio_tell(track->mdat_buf);
		if (first_track < 0)
			first_track = i;
	}

	if (!mdat_size)
		return 0;

	avio_write_marker(s->pb,
		av_rescale(mov->tracks[first_track].cluster[0].dts, AV_TIME_BASE,
			mov->tracks[first_track].timescale),
		(has_video ? starts_with_key : mov->tracks[first_track].cluster[0].flags
			& MOV_SYNC_SAMPLE) ?
		AVIO_DATA_MARKER_SYNC_POINT : AVIO_DATA_MARKER_BOUNDARY_POINT);

	for (i = 0; i < mov->nb_streams; i++) {
		MOVTrack *track = &mov->tracks[i];
		int buf_size, write_moof = 1, moof_tracks = -1;
		uint8_t *buf;
		int64_t duration = 0;

		if (track->entry)
			duration = track->start_dts + track->track_duration -
				track->cluster[0].dts;
		if (mov->flags & FF_MOV_FLAG_SEPARATE_MOOF) {
			if (!track->mdat_buf)
				continue;
			mdat_size = avio_tell(track->mdat_buf);
			moof_tracks = i;
		} else {
			write_moof = i == first_track;
		}

		if (write_moof) {
			avio_flush(s->pb);

			mov_write_moof_tag(s->pb, mov, moof_tracks, mdat_size);
			mov->fragments++;

			avio_wb32(s->pb, mdat_size + 8);
			ffio_wfourcc(s->pb, "mdat");
		}

		if (track->entry)
			track->frag_start += duration;
		track->entry = 0;
		track->entries_flushed = 0;
		track->end_reliable = 0;
		if (!mov->frag_interleave) {
			if (!track->mdat_buf)
				continue;
			buf_size = avio_close_dyn_buf(track->mdat_buf, &buf);
			track->mdat_buf = NULL;
		} else {
			if (!mov->mdat_buf)
				continue;
			buf_size = avio_close_dyn_buf(mov->mdat_buf, &buf);
			mov->mdat_buf = NULL;
		}

		avio_write(s->pb, buf, buf_size);
		av_free(buf);
	}

	mov->mdat_size = 0;

	avio_flush(s->pb);
	return 0;
}

static int mov_auto_flush_fragment(AVFormatContext *s, int force)
{
	MOVMuxContext *mov = s->priv_data;
	int had_moov = mov->moov_written;
	int ret = mov_flush_fragment(s, force);
	if (ret < 0)
		return ret;
	/* If using delay_moov, the first flush only wrote the moov,
	 not the actual moof+mdat pair, thus flush once again.*/
	if (!had_moov && mov->flags & FF_MOV_FLAG_DELAY_MOOV)
		ret = mov_flush_fragment(s, force);
	return ret;
}
#endif

static int check_pkt(AVFormatContext *s, AVPacket *pkt)
{
	MOVMuxContext *mov = s->priv_data;
	MOVTrack *trk = &mov->tracks[pkt->stream_index];
	int64_t ref;
	uint64_t duration;

	if (trk->entry) {
		ref = trk->clust.dts;
	} else if (trk->start_dts != AV_NOPTS_VALUE
		&& !trk->frag_discont) {
		ref = trk->start_dts + trk->track_duration;
	} else
		ref = pkt->dts; /* Skip tests for the first packet */

	duration = pkt->dts - ref;
	if (pkt->dts < ref || duration >= INT_MAX) {
		printf("Application provided duration: ");
		printf("%d / timestamp: %d ", (int)duration, (int)pkt->dts);
		printf("is out of range for mov/mp4 format\n");

		pkt->dts = ref + 1;
		pkt->pts = AV_NOPTS_VALUE;
	}

	if (pkt->duration < 0 || pkt->duration > INT_MAX) {
		printf("Application provided duration: %d is invalid\n",
			(int)pkt->duration);
		return -1;
	}
	return 0;
}

static inline int get_binfile_index(MOVMuxContext *mov)
{
	int index = 0;
	if (mov == binfile_ctx[0])
		index = 0;
	else if (mov == binfile_ctx[1])
		index = 1;
	else if (0 == binfile_ctx[0]) {
		binfile_ctx[0] = mov;
		index = 0;
	} else if (0 == binfile_ctx[1]) {
		binfile_ctx[1] = mov;
		index = 1;
	} else
		printf("cannot find free binfile index\n");
	return index;
}

int ff_mov_write_packet(AVFormatContext *s, AVPacket *pkt)
{
	MOVMuxContext *mov = s->priv_data;
	AVIOContext *pb = s->pb;
	MOVTrack *trk = &mov->tracks[pkt->stream_index];
	AVStream *st = s->streams[pkt->stream_index];
	AV_CodecParameters *par = trk->par;
	char path[100] = { 0 };
	unsigned int samples_in_chunk = 0;
	int size = pkt->size, ret = 0;
	int dts = 0;
	int len;

	if (trk->vos_len == 0) {
		/* copy extradata if it exists */
		if (st->codecpar->extradata_size) {
			trk->vos_len = st->codecpar->extradata_size;
			trk->vos_data = av_malloc(trk->vos_len);
			if (!trk->vos_data)
				return -1;

			memcpy(trk->vos_data, st->codecpar->extradata, trk->vos_len);
		}
		/* printf("index:%d,codeid:%d,len:%d\n",
			pkt->stream_index, par->codec_id, trk->vos_len); */
	}
	ret = check_pkt(s, pkt);
	if (ret < 0)
		return ret;

	if (par->codec_id == AV_CODEC_TYPE_ADPCM_IMA_WAV) {
		samples_in_chunk = trk->par->frame_size;
		if (!samples_in_chunk) {
			samples_in_chunk = (trk->par->block_align - 4 *
					trk->par->channels) * 8 /
				(4 * trk->par->channels) + 1;
		}
	} else if (trk->sample_size)
		samples_in_chunk = size / trk->sample_size;
	else
		samples_in_chunk = 1;

	if (par->codec_id == AV_CODEC_TYPE_AAC && pkt->size > 2 &&
		(AV_RB16(pkt->data) & 0xfff0) == 0xfff0) {
		if (!s->streams[pkt->stream_index]->nb_frames) {
			printf("Malformed AAC bitstream detected: ");
			printf("use the audio bitstream filter");
			printf(" 'aac_adtstoasc' to fix it ");
			printf("('-bsf:a aac_adtstoasc' option with ffmpeg)\n");
			return -1;
		}
		printf("aac bitstream error\n");
	}

	if (par->codec_id == AV_CODEC_TYPE_H264 && trk->vos_len > 0
		&& *(uint8_t *)trk->vos_data != 1) {
		/* from x264 or from bytestream H.264 */
		/* NAL reformatting needed */
		size = ff_avc_parse_nal_units(pb, pkt->data, pkt->size);
	} else {
		avio_write(pb, pkt->data, size);
	}

	/*if (trk->entry >= trk->cluster_capacity) {
		unsigned new_capacity = 20 * (trk->entry + MOV_INDEX_CLUSTER_SIZE);
		if (!trk->cluster){
			trk->cluster = av_malloc(new_capacity *
				sizeof(*trk->cluster));
			if (!trk->cluster){
				ret = -1;
				goto err;
			}

		}
		else if (!(trk->cluster = av_realloc(trk->cluster, new_capacity *
			sizeof(*trk->cluster)))) {
			ret = -1;
			goto err;
		}
		trk->cluster_capacity = new_capacity;
	}*/

	/* trk->cluster[trk->entry].pos              = avio_tell(pb) - size; */
	trk->clust.pos[trk->chunkCount & (CHUNK_NUM - 1)] = avio_tell(pb) - size +
		trk->data_offset;
	/* trk->cluster[trk->entry].samples_in_chunk = samples_in_chunk; */
	trk->clust.samples_in_chunk[trk->chunkCount & (CHUNK_NUM - 1)] = samples_in_chunk;
	/* trk->cluster[trk->entry].chunkNum         = 0;
	trk->cluster[trk->entry].size = size;*/
	trk->clust.size[trk->entry & (STSZ_NUM - 1)] = size;
	trk->clust.entries = samples_in_chunk;
	/* trk->cluster[trk->entry].dts = pkt->dts; */
	dts = pkt->dts;
	if (!trk->entry && trk->start_dts != AV_NOPTS_VALUE) {
		if (!trk->frag_discont) {
			/* First packet of a new fragment. We already wrote the
			 * duration of the last packet of the previous fragment
			 * based on track_duration,which might not exactly match
			 * our dts. Therefore adjust the dts of this packet to be
			 * what the previous packets duration implies.*/
			dts = trk->start_dts + trk->track_duration;
			/* We also may have written the pts and the corresponding duration
			 * in sidx/tfrf/tfxd tags; make sure the sidx pts
			 * and duration match up with
			 * the next fragment. This means the cts of the first sample must
			 * be the same in all fragments, unless end_pts was updated by
			 * the packet causing the fragment to be written. */
		} else {
			/* New fragment, but discontinuous from previous fragments.
			 * Pretend the duration sum of the earlier fragments is
			 * pkt->dts - trk->start_dts. */
			trk->frag_start = pkt->dts - trk->start_dts;
			trk->end_pts = AV_NOPTS_VALUE;
			trk->frag_discont = 0;
		}
	}

	if (trk->start_dts == AV_NOPTS_VALUE) {
		trk->start_dts = pkt->dts;
		if (trk->frag_discont) {
			if (mov->use_editlist) {
				/* Pretend the whole stream started at pts=0,
				 * with earlier fragments already written.
				 * If the stream started at pts=0, the duration sum
				 * of earlier fragments would have been pkt->pts. */
				trk->frag_start = pkt->pts;
				trk->start_dts = pkt->dts - pkt->pts;
			} else {
				/* Pretend the whole stream started at dts=0,
				 * with earlier fragments already written,
				 * with a duration summing up to pkt->dts. */
				trk->frag_start = pkt->dts;
				trk->start_dts = 0;
			}
			trk->frag_discont = 0;
		}
	}
	trk->track_duration = pkt->dts - trk->start_dts + pkt->duration;
	/*  trk->last_sample_is_subtitle_end = 0; */

	if (pkt->pts == AV_NOPTS_VALUE) {
		printf("pts has no value\n");
		pkt->pts = pkt->dts;
	}
	if (pkt->dts != pkt->pts)
		trk->flags |= MOV_TRACK_CTTS;
	trk->clust.ctts[trk->clust.framectts & (CTTS_NUM - 1)].duration =
		pkt->pts - pkt->dts;
	trk->clust.keyflags[trk->has_keyframes & (STSS_NUM - 1)] = 0;
	if (trk->start_cts == AV_NOPTS_VALUE)
		trk->start_cts = pkt->pts - pkt->dts;
	if (trk->end_pts == AV_NOPTS_VALUE)
		trk->end_pts = dts +
			trk->clust.ctts[trk->clust.framectts & (CTTS_NUM - 1)].duration
			+ pkt->duration;
	else
		trk->end_pts = FFMAX(trk->end_pts, dts +
				trk->clust.ctts[trk->clust.framectts
					& (CTTS_NUM - 1)].duration	+ pkt->duration);

	if (pkt->flags & AV_PKT_FLAG_KEY) {
		trk->clust.keyflags[trk->has_keyframes & (STSS_NUM - 1)] = trk->entry + 1;
		trk->has_keyframes++;
	}
	build_chunk(trk);
	/* stts */
	if (trk->clust.framestts == 0) {
		trk->clust.stts[0].duration = pkt->duration;
		trk->clust.stts[0].count = 1;
		trk->clust.framestts++;
		trk->clust.dts = dts;
	} else {
		int duration = pkt->dts - trk->clust.dts;
		if (duration == pkt->duration) {/* compress */
			trk->clust.stts[(trk->clust.framestts - 1)
				& (CHUNK_NUM - 1)].count++;
		} else {
			trk->clust.stts[trk->clust.framestts & (CHUNK_NUM - 1)].duration
				= duration;
			trk->clust.stts[trk->clust.framestts & (CHUNK_NUM - 1)].count = 1;
			trk->clust.framestts++;
			if (((trk->clust.framestts  & (CHUNK_NUM - 1)) == 1)
				&& trk->clust.sttsfile) {
				len = fwrite(&trk->clust.stts[CHUNK_NUM - 1],
						8, 1, trk->clust.sttsfile);
				if (len != 1)
					printf("==write stts error:%d\n", len);
			}

		}
		trk->clust.dts = dts;
	}
	/* ctts */
	if (trk->clust.framectts == 0) {
		trk->clust.ctts[0].count = 1;
		trk->clust.framectts++;
	} else {
		if (trk->clust.ctts[(trk->clust.framectts - 1) & (CTTS_NUM - 1)].duration
			== trk->clust.ctts[trk->clust.framectts
				& (CTTS_NUM - 1)].duration) {
			/* compress */
			trk->clust.ctts[(trk->clust.framectts - 1)
				& (CTTS_NUM - 1)].count++;
		} else {
			trk->clust.ctts[trk->clust.framectts & (CTTS_NUM - 1)].count = 1;
			trk->clust.framectts++;
			if (((trk->clust.framectts  & (CTTS_NUM - 1)) == 1)
				&& trk->clust.cttsfile) {
				len = fwrite(&trk->clust.ctts[CTTS_NUM - 1], 8, 1,
						trk->clust.cttsfile);
				if (len != 1)
					printf("==write ctts error");
			}

		}
	}
	trk->clust.totalsize += pkt->size;
	trk->entry++;
	trk->sample_count += samples_in_chunk;
	mov->mdat_size += size;

	if (trk->chunkCount == (trk->clust.framechunk + 1) * CHUNK_NUM) {
		if (!trk->clust.stcofile) {
			stco_name[0] = pkt->stream_index + '0';
			stco_name[1] = get_binfile_index(mov) + '0';
			snprintf(path, sizeof(path), MOUNT_ROOT_PATH"%s", stco_name);
			trk->clust.stcofile = avio_open(path, FWRMODE);
			if (!trk->clust.stcofile)
				printf("open file err:%s ", path);

		}
		len = fwrite(trk->clust.pos, 4, CHUNK_NUM, trk->clust.stcofile);
		if (len != CHUNK_NUM)
			printf("write pos error");
		if (!trk->clust.stscfile) {
			stsc_name[0] = pkt->stream_index + '0';
			stsc_name[1] = get_binfile_index(mov) + '0';
			snprintf(path, sizeof(path), MOUNT_ROOT_PATH"%s", stsc_name);
			trk->clust.stscfile = avio_open(path, FWRMODE);
			if (!trk->clust.stscfile)
				printf("open file err:%s ", path);

		}
		len = fwrite(trk->clust.samples_in_chunk, 4, CHUNK_NUM - 1,
				trk->clust.stscfile);
		if (len != CHUNK_NUM - 1)
			printf("write samples_in_chunk error");
		trk->clust.framechunk++;
	}

	if (trk->entry == (trk->clust.frameentry + 1) * STSZ_NUM) {
		if (!trk->clust.stszfile) {
			stsz_name[0] = pkt->stream_index + '0';
			stsz_name[1] = get_binfile_index(mov) + '0';
			snprintf(path, sizeof(path), MOUNT_ROOT_PATH"%s", stsz_name);
			trk->clust.stszfile = avio_open(path, FWRMODE);
			if (!trk->clust.stszfile)
				printf("open file err:%s ", path);

		}
		len = fwrite(trk->clust.size, 4, STSZ_NUM, trk->clust.stszfile);
		if (len != STSZ_NUM)
			printf("write clust.siz error");
		trk->clust.frameentry++;
	}
	if (((trk->clust.framectts  & (CTTS_NUM - 1)) == 0) &&
		(trk->clust.framectts != 0)) {
		if (!trk->clust.cttsfile) {
			ctts_name[0] = pkt->stream_index + '0';
			ctts_name[1] = get_binfile_index(mov) + '0';
			snprintf(path, sizeof(path), MOUNT_ROOT_PATH"%s", ctts_name);
			trk->clust.cttsfile = avio_open(path, FWRMODE);
			if (!trk->clust.cttsfile)
				printf("open file err:%s ", path);

		}
		len = fwrite(trk->clust.ctts, 8, CTTS_NUM - 1, trk->clust.cttsfile);
		if (len != CTTS_NUM - 1)
			printf("write clust.ctts error");
	}
	if (((trk->clust.framestts  & (CHUNK_NUM - 1)) == 0) &&
		(trk->clust.framestts != 0)) {
		if (!trk->clust.sttsfile) {
			stts_name[0] = pkt->stream_index + '0';
			stts_name[1] = get_binfile_index(mov) + '0';
			snprintf(path, sizeof(path), MOUNT_ROOT_PATH"%s", stts_name);
			trk->clust.sttsfile = avio_open(path, FWRMODE);
			if (!trk->clust.sttsfile)
				printf("open file err:%s ", path);

		}
		len = fwrite(trk->clust.stts, 8, CHUNK_NUM - 1, trk->clust.sttsfile);
		if (len != CHUNK_NUM - 1)
			printf("write clust.stts error:%d", len);
	}

	if ((pkt->flags & AV_PKT_FLAG_KEY) && (trk->has_keyframes != 0) &&
		((trk->has_keyframes  & (STSS_NUM - 1)) == 0)) {
		if (!trk->clust.stssfile) {
			stss_name[0] = pkt->stream_index + '0';
			stss_name[1] = get_binfile_index(mov) + '0';
			snprintf(path, sizeof(path), MOUNT_ROOT_PATH"%s", stss_name);
			trk->clust.stssfile = avio_open(path, FWRMODE);
			if (!trk->clust.stssfile)
				printf("open file err:%s ", path);

		}
		len = fwrite(trk->clust.keyflags, 4, STSS_NUM, trk->clust.stssfile);
		if (len != STSS_NUM)
			printf("write clust.keyflags error");
	}

	return ret;
}

#if 0
static int mov_write_single_packet(AVFormatContext *s, AVPacket *pkt)
{
	MOVMuxContext *mov = s->priv_data;
	MOVTrack *trk = &mov->tracks[pkt->stream_index];
	AV_CodecParameters *par = trk->par;
	int64_t frag_duration = 0;
	int size = pkt->size;

	int ret = check_pkt(s, pkt);
	if (ret < 0)
		return ret;

	if (mov->flags & FF_MOV_FLAG_FRAG_DISCONT) {
		int i;
		for (i = 0; i < s->nb_streams; i++)
			mov->tracks[i].frag_discont = 1;
		mov->flags &= ~FF_MOV_FLAG_FRAG_DISCONT;
	}

	if (!pkt->size) {
		if (trk->start_dts == AV_NOPTS_VALUE && trk->frag_discont) {
			trk->start_dts = pkt->dts;
			if (pkt->pts != AV_NOPTS_VALUE)
				trk->start_cts = pkt->pts - pkt->dts;
			else
				trk->start_cts = 0;
		}

		if (trk->par->codec_id == AV_CODEC_TYPE_MP4ALS) {
			int side_size = 0;
			uint8_t *side = av_packet_get_side_data(pkt,
					AV_PKT_DATA_NEW_EXTRADATA, &side_size);
			if (side && side_size > 0 && (side_size != par->extradata_size
					|| memcmp(side, par->extradata, side_size))) {
				void *newextra = av_mallocz(side_size
						+ AV_INPUT_BUFFER_PADDING_SIZE);
				if (!newextra)
					return AVERROR(ENOMEM);
				av_free(par->extradata);
				par->extradata = newextra;
				memcpy(par->extradata, side, side_size);
				par->extradata_size = side_size;
				mov->need_rewrite_extradata = 1;
			}
		}

		return 0;             /* Discard 0 sized packets */
	}

	if (trk->entry && pkt->stream_index < s->nb_streams)
		frag_duration = av_rescale_q(pkt->dts - trk->cluster[0].dts,
				s->streams[pkt->stream_index]->time_base,
				AV_TIME_BASE_Q);
	if ((mov->max_fragment_duration &&
			frag_duration >= mov->max_fragment_duration) ||
		(mov->max_fragment_size &&
			mov->mdat_size + size >= mov->max_fragment_size) ||
		(mov->flags & FF_MOV_FLAG_FRAG_KEYFRAME &&
			par->codec_type == AV_MEDIA_TYPE_VIDEO &&
			trk->entry && pkt->flags & AV_PKT_FLAG_KEY)) {
		if (frag_duration >= mov->min_fragment_duration) {
			/* Set the duration of this track to line up with the next
			 sample in this track. This avoids relying on AVPacket
			 duration, but only helps for this particular track, not
			 for the other ones that are flushed at the same time.*/
			trk->track_duration = pkt->dts - trk->start_dts;
			if (pkt->pts != AV_NOPTS_VALUE)
				trk->end_pts = pkt->pts;
			else
				trk->end_pts = pkt->dts;
			trk->end_reliable = 1;
			mov_auto_flush_fragment(s, 0);
		}
	}

	return ff_mov_write_packet(s, pkt);
}

static int mov_write_subtitle_end_packet(AVFormatContext *s,
	int stream_index,
	int64_t dts)
{
	AVPacket end;
	uint8_t data[2] = {0};
	int ret;

	av_init_packet(&end);
	end.size = sizeof(data);
	end.data = data;
	end.pts = dts;
	end.dts = dts;
	end.duration = 0;
	end.stream_index = stream_index;

	ret = mov_write_single_packet(s, &end);
	av_packet_unref(&end);

	return ret;
}
#endif

int mov_write_packet(AVFormatContext *s, AVPacket *pkt)
{
#if 0
	int i;
	MOVMuxContext *mov = s->priv_data;
	MOVTrack *trk = &mov->tracks[pkt->stream_index];

	if (!pkt->size)
		return mov_write_single_packet(s, pkt); /* Passthrough. */

	/*
	* Subtitles require special handling.
	*
	* 1) For full complaince, every track must have a sample at
	* dts == 0, which is rarely true for subtitles. So, as soon
	* as we see any packet with dts > 0, write an empty subtitle
	* at dts == 0 for any subtitle track with no samples in it.
	*
	* 2) For each subtitle track, check if the current packet's
	* dts is past the duration of the last subtitle sample. If
	* so, we now need to write an end sample for that subtitle.
	*
	* This must be done conditionally to allow for subtitles that
	* immediately replace each other, in which case an end sample
	* is not needed, and is, in fact, actively harmful.
	*
	* 3) See mov_write_trailer for how the final end sample is
	* handled.
	*/
	for (i = 0; i < mov->nb_streams; i++) {
		MOVTrack *trk = &mov->tracks[i];
		int ret;

		if (trk->par->codec_id == AV_CODEC_TYPE_MOV_TEXT &&
			trk->track_duration < pkt->dts &&
			(trk->entry == 0 || !trk->last_sample_is_subtitle_end)) {
			ret = mov_write_subtitle_end_packet(s, i, trk->track_duration);
			if (ret < 0)
				return ret;
			trk->last_sample_is_subtitle_end = 1;
		}
	}

	if (trk->mode == MODE_MOV && trk->par->codec_type == AV_MEDIA_TYPE_VIDEO) {
		AVPacket *opkt = pkt;
		int reshuffle_ret, ret;
		if (trk->is_unaligned_qt_rgb) {
			int64_t bpc = trk->par->bits_per_coded_sample != 15 ?
				trk->par->bits_per_coded_sample : 16;
			int expected_stride = ((trk->par->width * bpc + 15) >> 4) * 2;
			reshuffle_ret = ff_reshuffle_raw_rgb(s, &pkt, trk->par,
					expected_stride);
			if (reshuffle_ret < 0)
				return reshuffle_ret;
		} else
			reshuffle_ret = 0;
		if (trk->par->format == AV_PIX_FMT_PAL8 && !trk->pal_done) {
			ret = ff_get_packet_palette(s, opkt, reshuffle_ret, trk->palette);
			if (ret < 0)
				goto fail;
			if (ret)
				trk->pal_done++;
		} else if (trk->par->codec_id == AV_CODEC_TYPE_RAWVIDEO &&
			(trk->par->format == AV_PIX_FMT_GRAY8 ||
				trk->par->format == AV_PIX_FMT_MONOBLACK)) {
			for (i = 0; i < pkt->size; i++)
				pkt->data[i] = ~pkt->data[i];
		}
		if (reshuffle_ret) {
			ret = mov_write_single_packet(s, pkt);
fail:
			if (reshuffle_ret)
				av_packet_free(&pkt);
			return ret;
		}
	}
#endif
	return ff_mov_write_packet(s, pkt);/* mov_write_single_packet(s, pkt); */
}

#if 0
/* QuickTime chapters involve an additional text track with the chapter names
 as samples, and a tref pointing from the other tracks to the chapter one. */
static int mov_create_chapter_track(AVFormatContext *s, int tracknum)
{
	AVIOContext *pb;

	MOVMuxContext *mov = s->priv_data;
	MOVTrack *track = &mov->tracks[tracknum];
	AVPacket pkt = { .stream_index = tracknum, .flags = AV_PKT_FLAG_KEY };
	int i, len;

	track->mode = mov->mode;
	track->tag = MKTAG('t', 'e', 'x', 't');
	track->timescale = MOV_TIMESCALE;
	track->par = avcodec_parameters_alloc();
	if (!track->par)
		return AVERROR(ENOMEM);
	track->par->codec_type = AV_MEDIA_TYPE_SUBTITLE;
#if 0
	/* These properties are required to make QT recognize the chapter track */
	uint8_t chapter_properties[43] = { 0, 0, 0, 0, 0, 0, 0, 1, };
	if (ff_alloc_extradata(track->par, sizeof(chapter_properties)))
		return AVERROR(ENOMEM);
	memcpy(track->par->extradata, chapter_properties, sizeof(chapter_properties));
#else
	if (avio_open_dyn_buf(&pb) >= 0) {
		int size;
		uint8_t *buf;

		/* Stub header (usually for Quicktime chapter track) */
		/* TextSampleEntry */
		avio_wb32(pb, 0x01); /* displayFlags */
		avio_w8(pb, 0x00);   /* horizontal justification */
		avio_w8(pb, 0x00);   /* vertical justification */
		avio_w8(pb, 0x00);   /* bgColourRed */
		avio_w8(pb, 0x00);   /* bgColourGreen */
		avio_w8(pb, 0x00);   /* bgColourBlue */
		avio_w8(pb, 0x00);   /* bgColourAlpha */
		/* BoxRecord */
		avio_wb16(pb, 0x00); /* defTextBoxTop */
		avio_wb16(pb, 0x00); /* defTextBoxLeft */
		avio_wb16(pb, 0x00); /* defTextBoxBottom */
		avio_wb16(pb, 0x00); /* defTextBoxRight */
		/* StyleRecord */
		avio_wb16(pb, 0x00); /* startChar */
		avio_wb16(pb, 0x00); /* endChar */
		avio_wb16(pb, 0x01); /* fontID */
		avio_w8(pb, 0x00);   /* fontStyleFlags */
		avio_w8(pb, 0x00);   /* fontSize */
		avio_w8(pb, 0x00);   /* fgColourRed */
		avio_w8(pb, 0x00);   /* fgColourGreen */
		avio_w8(pb, 0x00);   /* fgColourBlue */
		avio_w8(pb, 0x00);   /* fgColourAlpha */
		/* FontTableBox */
		avio_wb32(pb, 0x0D); /* box size */
		ffio_wfourcc(pb, "ftab"); /* box atom name */
		avio_wb16(pb, 0x01); /* entry count */
		/* FontRecord */
		avio_wb16(pb, 0x01); /* font ID */
		avio_w8(pb, 0x00);   /* font name length */

		size = avio_close_dyn_buf(pb, &buf);
		if (size > 0) {
			track->par->extradata = buf;
			track->par->extradata_size = size;
		} else {
			av_freep(&buf);
		}
	}
#endif

	for (i = 0; i < s->nb_chapters; i++) {
		AVChapter *c = s->chapters[i];
		AVDictionaryEntry *t;

		int64_t end = av_rescale_q(c->end, c->time_base, (AV_Rational) {
			1, MOV_TIMESCALE
		});
		pkt.pts = pkt.dts = av_rescale_q(c->start, c->time_base, (AV_Rational) {
			1, MOV_TIMESCALE
		});
		pkt.duration = end - pkt.dts;

		t = av_dict_get(c->metadata, "title", NULL, 0);
		if (t) {
			static const char encd[12] = {
				0x00, 0x00, 0x00, 0x0C,
				'e',  'n',  'c',  'd',
				0x00, 0x00, 0x01, 0x00
			};
			len      = strlen(t->value);
			pkt.size = len + 2 + 12;
			pkt.data = av_malloc(pkt.size);
			if (!pkt.data)
				return AVERROR(ENOMEM);
			AV_WB16(pkt.data, len);
			memcpy(pkt.data + 2, t->value, len);
			memcpy(pkt.data + len + 2, encd, sizeof(encd));
			ff_mov_write_packet(s, &pkt);
			av_freep(&pkt.data);
		}
	}

	return 0;
}

static int mov_check_timecode_track(AVFormatContext *s, AVTimecode *tc,
	int src_index, const char *tcstr)
{
	int ret;

	/* compute the frame number */
	ret = av_timecode_init_from_string(tc, find_fps(s, s->streams[src_index]),
			tcstr, s);
	return ret;
}

static int mov_create_timecode_track(AVFormatContext *s, int index, int src_index,
	AVTimecode tc)
{
	int ret;
	MOVMuxContext *mov  = s->priv_data;
	MOVTrack *track     = &mov->tracks[index];
	AVStream *src_st    = s->streams[src_index];
	AVPacket pkt    = {.stream_index = index, .flags = AV_PKT_FLAG_KEY, .size = 4};
	AV_Rational rate = find_fps(s, src_st);

	/* tmcd track based on video stream */
	track->mode      = mov->mode;
	track->tag       = MKTAG('t', 'm', 'c', 'd');
	track->src_track = src_index;
	track->timescale = mov->tracks[src_index].timescale;
	if (tc.flags & AV_TIMECODE_FLAG_DROPFRAME)
		track->timecode_flags |= MOV_TIMECODE_FLAG_DROPFRAME;

	/* set st to src_st for metadata access*/
	track->st = src_st;

	/* encode context: tmcd data stream */
	track->par = avcodec_parameters_alloc();
	if (!track->par)
		return AVERROR(ENOMEM);
	track->par->codec_type = AV_MEDIA_TYPE_DATA;
	track->par->codec_tag  = track->tag;
	track->st->avg_frame_rate = av_inv_q(rate);

	/* the tmcd track just contains one packet with the frame number */
	pkt.data = av_malloc(pkt.size);
	if (!pkt.data)
		return AVERROR(ENOMEM);
	AV_WB32(pkt.data, tc.start);
	ret = ff_mov_write_packet(s, &pkt);
	av_free(pkt.data);
	return ret;
}
#endif
/*
* st->disposition controls the "enabled" flag in the tkhd tag.
* QuickTime will not play a track if it is not enabled.  So make sure
* that one track of each type (audio, video, subtitle) is enabled.
*
* Subtitles are special.  For audio and video, setting "enabled" also
* makes the track "default" (i.e. it is rendered when played). For
* subtitles, an "enabled" subtitle is not rendered by default, but
* if no subtitle is enabled, the subtitle menu in QuickTime will be
* empty!
*/
static void enable_tracks(AVFormatContext *s)
{
	MOVMuxContext *mov = s->priv_data;
	int i;
	int enabled[AV_MEDIA_TYPE_DATA];
	int first[AV_MEDIA_TYPE_DATA];

	for (i = 0; i < AV_MEDIA_TYPE_DATA; i++) {
		enabled[i] = 0;
		first[i] = -1;
	}

	for (i = 0; i < s->nb_streams; i++) {
		AVStream *st = s->streams[i];

		if (st->codecpar->codec_type <= AV_MEDIA_TYPE_INVALID ||
			st->codecpar->codec_type >= AV_MEDIA_TYPE_DATA)
			continue;

		if (first[st->codecpar->codec_type] < 0)
			first[st->codecpar->codec_type] = i;
		if (st->disposition & AV_DISPOSITION_DEFAULT) {
			mov->tracks[i].flags |= MOV_TRACK_ENABLED;
			enabled[st->codecpar->codec_type]++;
		}
	}

	for (i = 0; i < AV_MEDIA_TYPE_DATA; i++) {
		switch (i) {
		case AV_MEDIA_TYPE_VIDEO:
		case AV_MEDIA_TYPE_AUDIO:
			if (enabled[i] > 1)
				mov->per_stream_grouping = 1;
			if (!enabled[i] && first[i] >= 0)
				mov->tracks[first[i]].flags |= MOV_TRACK_ENABLED;
			break;
		}
	}
}

void mov_free(AVFormatContext *s)
{
	MOVMuxContext *mov = s->priv_data;
	int i;

	if (mov) {
		for (i = 0; i < mov->nb_streams; i++) {
			/* av_freep(&mov->tracks[i].cluster); */
			if (mov->tracks) {
				if (mov->tracks[i].vos_len &&
					mov->tracks[i].vos_data)
					av_freep(&mov->tracks[i].vos_data);
				if (mov->tracks[i].clust.pos)
					av_free(mov->tracks[i].clust.pos);
			}
		}
		if (mov->tracks) {
			av_freep(&mov->tracks);
			mov->tracks = NULL;
		}
	}
}
/*
static uint32_t rgb_to_yuv(uint32_t rgb)
{
    uint8_t r, g, b;
    int y, cb, cr;

    r = (rgb >> 16) & 0xFF;
    g = (rgb >>  8) & 0xFF;
    b = (rgb      ) & 0xFF;

    y  = av_clip_uint8(( 16000 +  257 * r + 504 * g +  98 * b)/1000);
    cb = av_clip_uint8((128000 -  148 * r - 291 * g + 439 * b)/1000);
    cr = av_clip_uint8((128000 +  439 * r - 368 * g -  71 * b)/1000);

    return (y << 16) | (cr << 8) | cb;
}

static int mov_create_dvd_sub_decoder_specific_info(MOVTrack *track,
							AVStream *st)
{
    int i, width = 720, height = 480;
    int have_palette = 0, have_size = 0;
    uint32_t palette[16];
    char *cur = st->codecpar->extradata;

    while (cur && *cur) {
		if (strncmp("palette:", cur, 8) == 0) {
			int i, count;
			count = sscanf(cur + 8,
				"%06"PRIx32", %06"PRIx32", %06"PRIx32", %06"PRIx32", "
				"%06"PRIx32", %06"PRIx32", %06"PRIx32", %06"PRIx32", "
				"%06"PRIx32", %06"PRIx32", %06"PRIx32", %06"PRIx32", "
				"%06"PRIx32", %06"PRIx32", %06"PRIx32", %06"PRIx32"",
				&palette[ 0], &palette[ 1], &palette[ 2], &palette[ 3],
				&palette[ 4], &palette[ 5], &palette[ 6], &palette[ 7],
				&palette[ 8], &palette[ 9], &palette[10], &palette[11],
				&palette[12], &palette[13], &palette[14], &palette[15]);

			for (i = 0; i < count; i++) {
				palette[i] = rgb_to_yuv(palette[i]);
			}
			have_palette = 1;
		} else if (!strncmp("size:", cur, 5)) {
			sscanf(cur + 5, "%dx%d", &width, &height);
			have_size = 1;
		}
		if (have_palette && have_size)
			break;
		cur += strcspn(cur, "\n\r");
		cur += strspn(cur, "\n\r");
    }
	if (have_palette) {
		track->vos_data = av_malloc(16*4);
		if (!track->vos_data)
			return AVERROR(ENOMEM);
		for (i = 0; i < 16; i++) {
			AV_WB32(track->vos_data + i * 4, palette[i]);
		}
		track->vos_len = 16 * 4;
    }
    st->codecpar->width = width;
    st->codecpar->height = track->height = height;

    return 0;
}
unsigned char vos[10] = { 0x12, 0x10, 0x56, 0xe5, 0x00 };*/
int mov_init(AVFormatContext *s)
{
	MOVMuxContext *mov = s->priv_data;
	int i;

	/* Default mode == MP4 */

	/*
	   if (s->oformat) {
	   if (!strcmp("mov", s->oformat->name)) mov->mode = MODE_MOV;
	   }

	   if (mov->flags & FF_MOV_FLAG_FASTSTART) {
	   mov->reserved_moov_size = -1;
	   }
	   */

	mov->nb_streams = s->nb_streams;

	/* Reserve an extra stream for chapters for the case where chapters
	 are written in the trailer */
	mov->tracks = av_mallocz(mov->nb_streams * sizeof(*mov->tracks));
	if (!mov->tracks) {
		PRINTF("malloc error!\n");
		return -1;
	}
	for (i = 0; i < s->nb_streams; i++) {
		AVStream *st = s->streams[i];
		MOVTrack *track = &mov->tracks[i];

		track->clust.pos  = av_mallocz((3 * CHUNK_NUM  + STSS_NUM + STSZ_NUM) *
				sizeof(int) + (CHUNK_NUM + CTTS_NUM) * sizeof(MOVStts));
		if (!track->clust.pos) {
			printf("malloc pos err,stream: %d\n", i);
			goto EXIT;
		}
		track->clust.samples_in_chunk =
			(unsigned int *)&track->clust.pos[CHUNK_NUM];
		track->clust.keyflags =
			(uint32_t *)&track->clust.samples_in_chunk[CHUNK_NUM];
		track->clust.size = (unsigned int *)&track->clust.keyflags[STSS_NUM];
		track->clust.stts = (MOVStts *)&track->clust.size[STSZ_NUM];
		track->clust.ctts = &track->clust.stts[CHUNK_NUM];
		track->clust.cache = (int *)&track->clust.ctts[CTTS_NUM];

		track->st = st;
		track->par = st->codecpar;
		track->mode = mov->mode;
		track->tag = mp4_get_codec_tag(s, track);

		/* If hinting of this track is enabled by a later hint track,
		 * this is updated. */
		track->start_dts = AV_NOPTS_VALUE;
		track->start_cts = AV_NOPTS_VALUE;
		track->end_pts = AV_NOPTS_VALUE;
		if (st->codecpar->codec_type == AV_MEDIA_TYPE_VIDEO) {
			track->timescale = st->time_base.den;
			while (track->timescale < 10000)
				track->timescale *= 2;
		} else if (st->codecpar->codec_type == AV_MEDIA_TYPE_AUDIO) {
			track->timescale = st->codecpar->sample_rate;
			if (!st->codecpar->frame_size &&
				!av_get_bits_per_sample(st->codecpar->codec_id)) {
				/* printf("track %d:codec frame size is not set\n", i); */
				track->audio_vbr = 1;
			} else if (st->codecpar->codec_id ==
				AV_CODEC_TYPE_ADPCM_IMA_WAV) {
				if (!st->codecpar->block_align) {
					printf("track %d:", i);
					printf(" block align is not set for adpcm\n");
					return -1;
				}
				track->sample_size = st->codecpar->block_align;
			} else if (st->codecpar->frame_size > 1) {
				/* assume compressed audio */
				track->audio_vbr = 1;
			} else {
				track->sample_size =
					(av_get_bits_per_sample(st->codecpar->codec_id)
						>> 3) *	st->codecpar->channels;
			}
		} else {
			track->timescale = MOV_TIMESCALE;
		}
		if (!track->height)
			track->height = st->codecpar->height;


		/*avpriv_set_pts_info(st, 64, 1, track->timescale); */
	}

	enable_tracks(s);
	return 0;
EXIT:
	for (i = 0; i < s->nb_streams; i++) {
		MOVTrack *track = &mov->tracks[i];
		if (track->clust.pos)
			av_free(track->clust.pos);
	}
	av_free(mov->tracks);
	mov->tracks = NULL;

	return -1;
}

static int mov_reset(AVFormatContext *s)
{
	MOVMuxContext *mov = s->priv_data;
	int i;

	mov->mdat_size = 0;

	for (i = 0; i < mov->nb_streams; i++) {
		/* AVStream *st = s->streams[i]; */
		MOVTrack *track = &mov->tracks[i];

		track->start_dts = AV_NOPTS_VALUE;
		track->start_cts = AV_NOPTS_VALUE;
		track->end_pts = AV_NOPTS_VALUE;
		track->entry = 0;
		track->sample_count = 0;
		track->chunkCount = 0;
		track->has_keyframes = 0;
	}

	return 0;
}

int mov_write_header(AVFormatContext *s)
{
	AVIOContext *pb = s->pb;
	MOVMuxContext *mov = s->priv_data;
	int i = 0;

	mov_reset(s);

	for (i = 0; i < s->nb_streams; i++) {
		AVStream *st = s->streams[i];
		MOVTrack *track = &mov->tracks[i];
		/* copy extradata if it exists */
		if (st->codecpar->extradata_size && (track->vos_len == 0)) {
			track->vos_len = st->codecpar->extradata_size;
			track->vos_data = av_malloc(track->vos_len);
			if (!track->vos_data)
				return -1;

			memcpy(track->vos_data, st->codecpar->extradata, track->vos_len);
		}
		/* printf("*****i:%d,codeid:%d,extradata_size:%d,%d,streams:%d\n",
			i, track->par->codec_id,
			st->codecpar->extradata_size, track->vos_len, s->nb_streams); */
	}
	mov_write_ftyp_tag(pb, s);
	mov_write_mdat_tag(pb, mov);
	/*
	   ff_parse_creation_time_metadata(s, &mov->time, 1);
	   if (mov->time)
	   mov->time += 0x7C25B080; // 1970 based -> 1904 based
	   */

	return 0;
}

#if 0
static int get_moov_size(AVFormatContext *s)
{
	int ret;
	AVIOContext *moov_buf;
	MOVMuxContext *mov = s->priv_data;

	ret = ffio_open_null_buf(&moov_buf);
	if (ret < 0)
		return ret;
	ret = mov_write_moov_tag(moov_buf, mov, s);
	if (ret < 0)
		return ret;
	return ffio_close_null_buf(moov_buf);
}

static int get_sidx_size(AVFormatContext *s)
{
	int ret;
	AVIOContext *buf;
	MOVMuxContext *mov = s->priv_data;

	ret = ffio_open_null_buf(&buf);
	if (ret < 0)
		return ret;
	mov_write_sidx_tags(buf, mov, -1, 0);
	return ffio_close_null_buf(buf);
}

/*
 * This function gets the moov size if moved to the top of the file: the chunk
 * offset table can switch between stco (32-bit entries) to co64 (64-bit
 * entries) when the moov is moved to the beginning, so the size of the moov
 * would change. It also updates the chunk offset tables.
 */
static int compute_moov_size(AVFormatContext *s)
{
	int i, moov_size, moov_size2;
	MOVMuxContext *mov = s->priv_data;

	moov_size = get_moov_size(s);
	if (moov_size < 0)
		return moov_size;

	for (i = 0; i < mov->nb_streams; i++)
		mov->tracks[i].data_offset += moov_size;

	moov_size2 = get_moov_size(s);
	if (moov_size2 < 0)
		return moov_size2;

	/* if the size changed, we just switched from stco to co64 and need to
	 * update the offsets */
	if (moov_size2 != moov_size)
		for (i = 0; i < mov->nb_streams; i++)
			mov->tracks[i].data_offset += moov_size2 - moov_size;

	return moov_size2;
}

static int compute_sidx_size(AVFormatContext *s)
{
	int i, sidx_size;
	MOVMuxContext *mov = s->priv_data;

	sidx_size = get_sidx_size(s);
	if (sidx_size < 0)
		return sidx_size;

	for (i = 0; i < mov->nb_streams; i++)
		mov->tracks[i].data_offset += sidx_size;

	return sidx_size;
}

static int shift_data(AVFormatContext *s)
{
	int ret = 0, moov_size;
	MOVMuxContext *mov = s->priv_data;
	int64_t pos, pos_end = avio_tell(s->pb);
	uint8_t *buf, *read_buf[2];
	int read_buf_id = 0;
	int read_size[2];
	AVIOContext *read_pb;

	if (mov->flags & FF_MOV_FLAG_FRAGMENT)
		moov_size = compute_sidx_size(s);
	else
		moov_size = compute_moov_size(s);
	if (moov_size < 0)
		return moov_size;

	buf = av_malloc(moov_size * 2);
	if (!buf)
		return AVERROR(ENOMEM);
	read_buf[0] = buf;
	read_buf[1] = buf + moov_size;

	/* Shift the data: the AVIO context of the output can only be used for
	 * writing, so we re-open the same output, but for reading. It also avoids
	 * a read/seek/write/seek back and forth. */
	avio_flush(s->pb);
	ret = s->io_open(s, &read_pb, s->filename, AVIO_FLAG_READ, NULL);
	if (ret < 0) {
		PRINTF("Unable to re-open %s", s->filename);
		PRINTF("output file for the second pass (faststart)\n");
		goto end;
	}

	/* mark the end of the shift to up to the last data we wrote, and get ready
	 * for writing */
	pos_end = avio_tell(s->pb);
	avio_seek(s->pb, mov->reserved_header_pos + moov_size, SEEK_SET);

	/* start reading at where the new moov will be placed */
	avio_seek(read_pb, mov->reserved_header_pos, SEEK_SET);
	pos = avio_tell(read_pb);

#define READ_BLOCK do { \
		read_size[read_buf_id] = \
		avio_read(read_pb, read_buf[read_buf_id], moov_size);  \
		read_buf_id ^= 1;  \
	} while (0)

	/* shift data by chunk of at most moov_size */
	READ_BLOCK;
	do {
		int n;
		READ_BLOCK;
		n = read_size[read_buf_id];
		if (n <= 0)
			break;
		avio_write(s->pb, read_buf[read_buf_id], n);
		pos += n;
	} while (pos < pos_end);
	ff_format_io_close(s, &read_pb);

end:
	av_free(buf);
	return ret;
}
#endif
int mov_write_free_tag(AVIOContext *pb, MOVMuxContext *mov)
{
	avio_tell(pb);
	avio_wb32(pb, 8 + mov->free_data_size);
	ffio_wfourcc(pb, mov->mode == MODE_MOV ? "wide" : "free");
	if (mov->free_buff && mov->free_data_size) {
		avio_fwrite(mov->free_buff, 1,
			mov->free_data_size, (AVIOFILE)pb);
	}
	mov->free_buff = NULL;
	mov->free_data_size = 0;

	return 0;
}

int mov_write_trailer(AVFormatContext *s)
{
	MOVMuxContext *mov = s->priv_data;
	AVIOContext *pb = s->pb;
	int res = 0;
	int64_t moov_pos = 0;
	int i = 0;

	if (!(mov->flags & FF_MOV_FLAG_FRAGMENT)) {
		moov_pos = avio_tell(pb);

		/* Write size of mdat tag */
		avio_seek(pb, mov->mdat_pos, SEEK_SET);
		avio_wb32(pb, mov->mdat_size + 8);

		avio_seek(pb, moov_pos, SEEK_SET);

		res = mov_write_moov_tag(pb, mov, s);
		if (res < 0)
			return res;

		res = 0;
	}
	mov_write_free_tag(pb, mov);
	for (i = 0; i < mov->nb_streams; i++) {
		if (mov->tracks[i].clust.stcofile) {
			fclose(mov->tracks[i].clust.stcofile);
			mov->tracks[i].clust.stcofile = FNULL;
		}
		if (mov->tracks[i].clust.stscfile) {
			fclose(mov->tracks[i].clust.stscfile);
			mov->tracks[i].clust.stscfile = FNULL;
		}
		if (mov->tracks[i].clust.stszfile) {
			fclose(mov->tracks[i].clust.stszfile);
			mov->tracks[i].clust.stszfile = FNULL;
		}
		if (mov->tracks[i].clust.cttsfile) {
			fclose(mov->tracks[i].clust.cttsfile);
			mov->tracks[i].clust.cttsfile = FNULL;
		}
		if (mov->tracks[i].clust.sttsfile) {
			fclose(mov->tracks[i].clust.sttsfile);
			mov->tracks[i].clust.sttsfile = FNULL;
		}
		if (mov->tracks[i].clust.stssfile) {
			fclose(mov->tracks[i].clust.stssfile);
			mov->tracks[i].clust.stssfile = FNULL;
		}
		if (mov == binfile_ctx[0])
			binfile_ctx[0] = 0;
		if (mov == binfile_ctx[1])
			binfile_ctx[1] = 0;
	}
	return res;
}

