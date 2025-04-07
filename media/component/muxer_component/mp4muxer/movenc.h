/*
 * MOV, MP4 muxer
 */

#ifndef MOVENC_H
#define MOVENC_H

#include "encode_plat.h"
#include "avformat.h"
#ifndef __BS_FFMPEG
#define avio_fwrite fwrite
#define avio_tell ftell
#define avio_seek fseek
#define avio_open fopen
#define avio_close fclose
#else
#define  avio_tell(s)	avio_seek(s, 0, SEEK_CUR)
#endif

#define av_assert2
#define av_assert0

#ifndef UINT16_MAX
#define UINT16_MAX       (0xffffui16)
#endif
#ifndef INT32_MAX
#define INT32_MAX        (2147483647i32)
#endif

#ifndef UINT32_MAX
#define UINT32_MAX       (0xffffffffui32)
#endif

#define MOV_TKHD_FLAG_ENABLED       0x0001
#define MOV_TKHD_FLAG_IN_MOVIE      0x0002

/* #define MOV_FRAG_INFO_ALLOC_INCREMENT 64 */
#define MOV_INDEX_CLUSTER_SIZE 1024
#define CHUNK_NUM (8 * MOV_INDEX_CLUSTER_SIZE)
/* #define ENTRAY_NUM (4 * 8 * MOV_INDEX_CLUSTER_SIZE) */
#define STSZ_NUM (8*CHUNK_NUM)
#define CTTS_NUM (8*CHUNK_NUM)
#define STSS_NUM (8*CHUNK_NUM)

#define MOV_TIMESCALE 1000

/* #define RTP_MAX_PACKET_SIZE 1450 */

#define MODE_MP4  0x01
#define MODE_MOV  0x02

typedef struct MOVStts {
	int count;
	int duration;
} MOVStts;

typedef struct AVCodecTag {
	enum av_codec_type id;
	unsigned int tag;
} AVCodecTag;
/**
 * This structure stores compressed data. It is typically exported by demuxers
 * and then passed as input to decoders, or received as output from encoders and
 * then passed to muxers.
 *
 * For video, it should typically contain one compressed frame. For audio it may
 * contain several compressed frames. Encoders are allowed to output empty
 * packets, with no compressed data, containing only side data
 * (e.g. to update some stream parameters at the end of encoding).
 *
 * AVPacket is one of the few structs in FFmpeg, whose size is a part of public
 * ABI. Thus it may be allocated on stack and no new fields can be added to it
 * without libavcodec and libavformat major bump.
 *
 * The semantics of data ownership depends on the buf field.
 * If it is set, the packet data is dynamically allocated and is
 * valid indefinitely until a call to av_packet_unref() reduces the
 * reference count to 0.
 *
 * If the buf field is not set av_packet_ref() would make a copy instead
 * of increasing the reference count.
 *
 * The side data is always allocated with av_malloc(), copied by
 * av_packet_ref() and freed by av_packet_unref().
 *
 * @see av_packet_ref
 * @see av_packet_unref
 */
typedef struct AVPacket {
	/**
	* A reference to the reference-counted buffer where the packet data is
	* stored.
	* May be NULL, then the packet data is not reference-counted.
	*/
	/* AVBufferRef *buf; */
	/**
	 * Presentation timestamp in AVStream->time_base units; the time at which
	 * the decompressed packet will be presented to the user.
	 * Can be AV_NOPTS_VALUE if it is not stored in the file.
	 * pts MUST be larger or equal to dts as presentation cannot happen before
	 * decompression, unless one wants to view hex dumps. Some formats misuse
	 * the terms dts and pts/cts to mean something different. Such timestamps
	 * must be converted to true pts/dts before they are stored in AVPacket.
	 */
	int64_t pts;
	/**
	 * Decompression timestamp in AVStream->time_base units; the time at which
	 * the packet is decompressed.
	 * Can be AV_NOPTS_VALUE if it is not stored in the file.
	 */
	int64_t dts;
	uint8_t *data;
	int   size;
	int   stream_index;
	/**
	 * A combination of AV_PKT_FLAG values
	 */
	int   flags;
	/**
	* Additional packet data that can be provided by the container.
	* Packet can contain several types of side information.
	*/
	/* AVPacketSideData *side_data;
	 int side_data_elems; */

	/**
	 * Duration of this packet in AVStream->time_base units, 0 if unknown.
	 * Equals next_pts - this_pts in presentation order.
	 */
	int64_t duration;

	/* int64_t pos; ///< byte position in stream, -1 if unknown */

#if 0 /* FF_API_CONVERGENCE_DURATION */
	/**
	* @deprecated Same as the duration field, but as int64_t. This was required
	* for Matroska subtitles, whose duration values could overflow when the
	* duration field was still an int.
	*/
	attribute_deprecated
	int64_t convergence_duration;
#endif
} AVPacket;
typedef struct MOVIentry {
	/* uint64_t     pos;
	int64_t      dts;
	unsigned int size;
	unsigned int samples_in_chunk;
	///< Chunk number if the current entry is a chunk start otherwise 0
	unsigned int chunkNum;
	unsigned int entries; */
	int          cts;
#define MOV_SYNC_SAMPLE         0x0001
#define MOV_PARTIAL_SYNC_SAMPLE 0x0002
	uint32_t     flags;
} MOVIentry;

/* typedef struct HintSample {
    uint8_t *data;
    int size;
    int sample_number;
    int offset;
    int own_data;
} HintSample;

typedef struct HintSampleQueue {
    int size;
    int len;
    HintSample *samples;
} HintSampleQueue;

typedef struct MOVFragmentInfo {
    int64_t offset;
    int64_t time;
    int64_t duration;
    int64_t tfrf_offset;
    int size;
} MOVFragmentInfo;*/
typedef struct MovCluster {
	uint64_t chunkSize;
	/*MOVIentry *chunk; */
#if 0
	int pos[CHUNK_NUM];/* stco */
	unsigned int samples_in_chunk[CHUNK_NUM];/* stsc -1 */
	uint32_t  keyflags[STSS_NUM];/* stss */
	unsigned int size[STSZ_NUM];/* stsz */
	MOVStts stts[CHUNK_NUM];/* maybe only one -1 */
	MOVStts ctts[CTTS_NUM];/* -1 */
	int cache[CHUNK_NUM];
#else
	int *pos;/* stco */
	unsigned int *samples_in_chunk;/* stsc -1 */
	uint32_t  *keyflags;/* stss */
	unsigned int *size;/* stsz */
	MOVStts *stts;/* maybe only one -1 */
	MOVStts *ctts;/* -1 */
	int *cache;
#endif
	int framechunk;
	int frameentry;
	unsigned int entries;
	int dts;
	int framestts;
	int framectts;
	int totalsize;
	AVIOFILE stcofile;
	AVIOFILE stscfile;
	AVIOFILE stszfile;
	AVIOFILE cttsfile;
	AVIOFILE stssfile;
	AVIOFILE sttsfile;
} MovCluster;
typedef struct MOVTrack {
	int         mode;
	int         entry;
	unsigned    timescale;
	uint64_t    time;
	int64_t     track_duration;
	/*    int         last_sample_is_subtitle_end; */
	long        sample_count;
	long        sample_size;
	long        chunkCount;
	int         has_keyframes;
#define MOV_TRACK_CTTS         0x0001
#define MOV_TRACK_STPS         0x0002
#define MOV_TRACK_ENABLED      0x0004
	uint32_t    flags;
	/* #define MOV_TIMECODE_FLAG_DROPFRAME     0x0001
	#define MOV_TIMECODE_FLAG_24HOURSMAX    0x0002
	#define MOV_TIMECODE_FLAG_ALLOWNEGATIVE 0x0004
	    uint32_t    timecode_flags;*/
	int         language;
	int         track_id;
	int         tag; /* < stsd fourcc */
	AVStream        *st;
	AV_CodecParameters *par;
	/*    int multichannel_as_mono; */

	int         vos_len;
	uint8_t     *vos_data;
	MOVIentry   *cluster;
	unsigned    cluster_capacity;
	int         audio_vbr;
	int         height; /* < active picture (w/o VBI) height for D-10/IMX */
	/*    uint32_t    tref_tag;
	    int         tref_id; ///< trackID of the referenced track */
	int64_t     start_dts;
	int64_t     start_cts;
	int64_t     end_pts;
	/*    int         end_reliable;

			///< the track that hints this track, -1 if no hint track is set
	    int         hint_track;
	    ///< the track that this hint (or tmcd) track describes
	    int         src_track;
	    AVFormatContext *rtp_ctx; ///< the format context for the hinting rtp muxer
	    uint32_t    prev_rtp_ts;
	    int64_t     cur_rtp_ts_unwrapped;
	    uint32_t    max_packet_size;

	    int64_t     default_duration;
	    uint32_t    default_sample_flags;
	    uint32_t    default_size;

	    HintSampleQueue sample_queue;

	    AVIOContext *mdat_buf;*/
	int64_t     data_offset;
	int64_t     frag_start;
	int         frag_discont;
	/*    int         entries_flushed;

	    int         nb_frag_info;
	    MOVFragmentInfo *frag_info;
	    unsigned    frag_info_capacity;

	    struct {
					int     first_packet_seq;
					int     first_packet_entry;
					int     first_packet_seen;
					int     first_frag_written;
					int     packet_seq;
					int     packet_entry;
					int     slices;
	    } vc1_info;

	    void       *eac3_priv;

	    MOVMuxCencContext cenc;

	    uint32_t palette[AVPALETTE_COUNT];
	    int pal_done;

	    int is_unaligned_qt_rgb;*/
	MovCluster clust;
} MOVTrack;

/*typedef enum {
    MOV_ENC_NONE = 0,
    MOV_ENC_CENC_AES_CTR,
} MOVEncryptionScheme;*/

typedef struct MOVMuxContext {
	/*    const AVClass *av_class; */
	int     mode;
	int64_t time;
	int     nb_streams;
	/* < number of new created tmcd track based on metadata (aka not data copy)*/
	/* int  nb_meta_tmcd;
	int     chapter_track; ///< qt chapter track number */
	int64_t mdat_pos;
	uint64_t mdat_size;
	MOVTrack *tracks;

	int flags;
	/*    int rtp_flags;

	    int iods_skip;
	    int iods_video_profile;
	    int iods_audio_profile;

	    int moov_written;
	    int fragments;
	    int max_fragment_duration;
	    int min_fragment_duration;
	    int max_fragment_size;
	    int ism_lookahead;
	    AVIOContext *mdat_buf;
	    int first_trun;

	    int video_track_timescale;

	    int reserved_moov_size; ///< 0 for disabled, -1 for automatic, size otherwise
	    int64_t reserved_header_pos;

	    char *major_brand;*/

	int per_stream_grouping;
	/*    AVFormatContext *fc; */

	int use_editlist;
	/*    float gamma;

	    int frag_interleave;
	    int missing_duration_warned;

	    char *encryption_scheme_str;
	    MOVEncryptionScheme encryption_scheme;
	    uint8_t *encryption_key;
	    int encryption_key_len;
	    uint8_t *encryption_kid;
	    int encryption_kid_len;

	    int need_rewrite_extradata;
	*/
	unsigned char *free_buff;
	int free_data_size;
	int use_stream_ids_as_track_ids;
	int track_ids_ok;
	/*    int write_tmcd;*/
} MOVMuxContext;

/* #define FF_MOV_FLAG_RTP_HINT              (1 <<  0) */
#define FF_MOV_FLAG_FRAGMENT              (1 <<  1)
#define FF_MOV_FLAG_EMPTY_MOOV            (1 <<  2)
/* #define FF_MOV_FLAG_FRAG_KEYFRAME         (1 <<  3)
#define FF_MOV_FLAG_SEPARATE_MOOF         (1 <<  4)
#define FF_MOV_FLAG_FRAG_CUSTOM           (1 <<  5)
#define FF_MOV_FLAG_ISML                  (1 <<  6)*/
#define FF_MOV_FLAG_FASTSTART             (1 <<  7)
/* #define FF_MOV_FLAG_OMIT_TFHD_OFFSET      (1 <<  8)
#define FF_MOV_FLAG_DISABLE_CHPL          (1 <<  9)
#define FF_MOV_FLAG_DEFAULT_BASE_MOOF     (1 << 10)
#define FF_MOV_FLAG_DASH                  (1 << 11)
#define FF_MOV_FLAG_FRAG_DISCONT          (1 << 12)
#define FF_MOV_FLAG_DELAY_MOOV            (1 << 13)
#define FF_MOV_FLAG_GLOBAL_SIDX           (1 << 14)
#define FF_MOV_FLAG_WRITE_COLR            (1 << 15)
#define FF_MOV_FLAG_WRITE_GAMA            (1 << 16)
#define FF_MOV_FLAG_USE_MDTA              (1 << 17)

int ff_mov_write_packet(AVFormatContext *s, AVPacket *pkt);

int ff_mov_init_hinting(AVFormatContext *s, int index, int src_index);
int ff_mov_add_hinted_packet(AVFormatContext *s, AVPacket *pkt,
					int track_index, int sample,
					uint8_t *sample_data, int sample_size);
void ff_mov_close_hinting(MOVTrack *track);
*/
int mov_init(AVFormatContext *s);
int mov_write_header(AVFormatContext *s);
int mov_write_packet(AVFormatContext *s, AVPacket *pkt);
int mov_write_trailer(AVFormatContext *s);
void mov_free(AVFormatContext *s);
#endif /* MOVENC_H */

