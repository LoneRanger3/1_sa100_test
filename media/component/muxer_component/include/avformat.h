#ifndef __AVFORMAT_H__
#define __AVFORMAT_H__

#include "audio_common.h"

#define AVIOContext FILE


#if 0
typedef enum AVMediaType {
	/* Usually treated as AVMEDIA_TYPE_DATA */
	AVMEDIA_TYPE_UNKNOWN = -1,
	AVMEDIA_TYPE_VIDEO,
	AVMEDIA_TYPE_AUDIO,
	/* Opaque data information usually continuous */
	AVMEDIA_TYPE_DATA,
	AVMEDIA_TYPE_SUBTITLE,
	/* Opaque data information usually sparse */
	AVMEDIA_TYPE_ATTACHMENT,
	AVMEDIA_TYPE_NB
} av_media_type_e;


typedef enum AVMuxerType {
	/* Usually treated as AVDEMUXER_TYPE_DATA */
	AV_MUXER_TYPE_UNKNOWN = 0,
	AV_MUXER_TYPE_G729,
	AV_MUXER_TYPE_MP3,
	AV_MUXER_TYPE_WAV,
	AV_MUXER_TYPE_MP4,
	AV_MUXER_TYPE_MOV,
	AV_MUXER_TYPE_RAW
} av_muxer_type_e;
#endif

typedef enum AVCodecID {
	AV_CODEC_ID_NONE,

	/* video codecs */
	AV_CODEC_ID_MPEG1VIDEO,
	AV_CODEC_ID_MPEG2VIDEO,
	AV_CODEC_ID_H263 = 5,
	AV_CODEC_ID_MJPEG = 8,
	AV_CODEC_ID_MPEG4 = 13,
	AV_CODEC_ID_H264 = 28,
	AV_CODEC_ID_VC1 = 71,
	AV_CODEC_ID_HEVC = 273,


	/* various PCM "codecs" */
	/* < A dummy id pointing at the start of audio codecs */
	AV_CODEC_ID_FIRST_AUDIO = 0x10000,
	AV_CODEC_ID_PCM_S16LE = 0x10000,
	AV_CODEC_ID_PCM_S16BE,
	AV_CODEC_ID_PCM_U16LE,
	AV_CODEC_ID_PCM_U16BE,
	AV_CODEC_ID_PCM_S8,
	AV_CODEC_ID_PCM_U8,
	AV_CODEC_ID_PCM_MULAW,
	AV_CODEC_ID_PCM_ALAW,
	AV_CODEC_ID_PCM_S32LE,
	AV_CODEC_ID_PCM_S32BE,
	AV_CODEC_ID_PCM_U32LE,
	AV_CODEC_ID_PCM_U32BE,
	AV_CODEC_ID_PCM_S24LE,
	AV_CODEC_ID_PCM_S24BE,
	AV_CODEC_ID_PCM_U24LE,
	AV_CODEC_ID_PCM_U24BE,
	AV_CODEC_ID_PCM_S24DAUD,
	AV_CODEC_ID_PCM_ZORK,
	AV_CODEC_ID_PCM_S16LE_PLANAR,
	AV_CODEC_ID_PCM_DVD,
	AV_CODEC_ID_PCM_F32BE,
	AV_CODEC_ID_PCM_F32LE,
	AV_CODEC_ID_PCM_F64BE,
	AV_CODEC_ID_PCM_F64LE,
	AV_CODEC_ID_PCM_BLURAY,
	AV_CODEC_ID_PCM_LXF,
	AV_CODEC_ID_S302M,
	AV_CODEC_ID_PCM_S8_PLANAR,
	AV_CODEC_ID_PCM_S24LE_PLANAR,
	AV_CODEC_ID_PCM_S32LE_PLANAR,
	AV_CODEC_ID_PCM_S16BE_PLANAR,

	AV_CODEC_ID_PCM_S64LE = 0x10800,
	AV_CODEC_ID_PCM_S64BE,

	/* various ADPCM codecs */
	AV_CODEC_ID_ADPCM_IMA_QT = 0x11000,
	AV_CODEC_ID_ADPCM_IMA_WAV,
	AV_CODEC_ID_ADPCM_IMA_DK3,
	AV_CODEC_ID_ADPCM_IMA_DK4,
	AV_CODEC_ID_ADPCM_IMA_WS,
	AV_CODEC_ID_ADPCM_IMA_SMJPEG,
	AV_CODEC_ID_ADPCM_MS,
	AV_CODEC_ID_ADPCM_G726 = 0x1100b,
	AV_CODEC_ID_ADPCM_YAMAHA = 0x1100e,
	/* AMR */
	AV_CODEC_ID_AMR_NB = 0x12000,
	AV_CODEC_ID_AMR_WB,
	/* audio codecs */
	AV_CODEC_ID_MP2 = 0x15000,
	AV_CODEC_ID_MP3, /* < preferred ID for decoding MPEG audio layer 1, 2 or 3 */
	AV_CODEC_ID_AAC,
	AV_CODEC_ID_AC3,
	AV_CODEC_ID_DTS,
	AV_CODEC_ID_VORBIS,
	AV_CODEC_ID_DVAUDIO,
	AV_CODEC_ID_WMAV1,
	AV_CODEC_ID_WMAV2,
	AV_CODEC_ID_MACE3,
	AV_CODEC_ID_MACE6,
	AV_CODEC_ID_VMDAUDIO,
	AV_CODEC_ID_FLAC,
	AV_CODEC_ID_MP3ADU,
	AV_CODEC_ID_MP3ON4,
	AV_CODEC_ID_SHORTEN,
	AV_CODEC_ID_ALAC,

	AV_CODEC_ID_WMAVOICE = 0x15025,
	AV_CODEC_ID_WMAPRO,
	AV_CODEC_ID_WMALOSSLESS,
	AV_CODEC_ID_ATRAC3P,
	AV_CODEC_ID_EAC3,
	AV_CODEC_ID_SIPR,
	AV_CODEC_ID_MP1,
	AV_CODEC_ID_AAC_LATM = 0x15032,
	AV_CODEC_ID_G723_1 = 0x15035,
	AV_CODEC_ID_G729,

	AV_CODEC_ID_ASS = 0x15040,
	AV_CODEC_ID_LRC,
	AV_CODEC_ID_TXT,
	AV_CODEC_ID_SRC
} av_codec_id_e;

/*
 * Rational number (pair of numerator and denominator).
 */
typedef struct AVRational {
	int num;/* < Numerator */
	int den;/* < Denominator */
} AVRational;

typedef struct AVCodecParameters {
	/**
	* General type of the encoded data.
	*/
	enum av_codec_type codec_type;
	/**
	* Specific type of the encoded data (the codec used).
	*/
	enum AVCodecID   codec_id;
	/**
	* Additional information about the codec (corresponds to the AVI FOURCC).
	*/
	uint32_t         codec_tag;

	/**
	* Extra binary data needed for initializing the decoder, codec-dependent.
	*
	* Must be allocated with av_malloc() and will be freed by
	* avcodec_parameters_free(). The allocated size of extradata must be at
	* least extradata_size + AV_INPUT_BUFFER_PADDING_SIZE, with the padding
	* bytes zeroed.
	*/
	uint8_t *extradata;
	/**
	* Size of the extradata content in bytes.
	*/
	int      extradata_size;

	/**
	* - video: the pixel format, the value corresponds to enum AVPixelFormat.
	* - audio: the sample format, the value corresponds to enum AVSampleFormat.
	*/
	/* int format; */

	/**
	* The average bitrate of the encoded data (in bits per second).
	*/
	int64_t bit_rate;

	/**
	* The number of bits per sample in the codedwords.
	*
	* This is basically the bitrate per sample. It is mandatory for a bunch of
	* formats to actually decode them. It's the number of bits for one sample in
	* the actual coded bitstream.
	*
	* This could be for example 4 for ADPCM
	* For PCM formats this matches bits_per_raw_sample
	* Can be 0
	*/
	int bits_per_coded_sample;

	/**
	* This is the number of valid bits in each output sample. If the
	* sample format has more bits, the least significant bits are additional
	* padding bits, which are always 0. Use right shifts to reduce the sample
	* to its actual size. For example, audio formats with 24 bit samples will
	* have bits_per_raw_sample set to 24, and format set to AV_SAMPLE_FMT_S32.
	* To get the original sample use "(int32_t)sample >> 8"."
	*
	* For ADPCM this might be 12 or 16 or similar
	* Can be 0
	*/
	/* int bits_per_raw_sample;*/

	/**
	* Codec-specific bitstream restrictions that the stream conforms to.
	*/
	/*int profile;*/
	/*int level;*/

	/**
	* Video only. The dimensions of the video frame in pixels.
	*/
	int width;
	int height;

	/**
	* Video only. The aspect ratio (width / height) which a single pixel
	* should have when displayed.
	*
	* When the aspect ratio is unknown / undefined, the numerator should be
	* set to 0 (the denominator may have any value).
	*/
	/* AVRational sample_aspect_ratio;*/

	/**
	* Video only. The order of the fields in interlaced video.
	*/
	/*enum AVFieldOrder                  field_order;*/

	/**
	* Video only. Additional colorspace characteristics.
	*/
	/* enum AVColorRange                  color_range;
	enum AVColorPrimaries              color_primaries;
	enum AVColorTransferCharacteristic color_trc;
	enum AVColorSpace                  color_space;
	enum AVChromaLocation              chroma_location;*/

	/**
	* Video only. Number of delayed frames.
	*/
	/*int video_delay;*/

	/**
	* Audio only. The channel layout bitmask. May be 0 if the channel layout is
	* unknown or unspecified, otherwise the number of bits set must be equal to
	* the channels field.
	*/
	uint64_t channel_layout;
	/**
	* Audio only. The number of audio channels.
	*/
	int      channels;
	/**
	* Audio only. The number of audio samples per second.
	*/
	int      sample_rate;
	/**
	* Audio only. The number of bytes per coded audio frame, required by some
	* formats.
	*
	* Corresponds to nBlockAlign in WAVEFORMATEX.
	*/
	int      block_align;
	/**
	* Audio only. Audio frame size, if known. Required by some formats to be static.
	*/
	int      frame_size;

	/**
	* Audio only. The amount of padding (in samples) inserted by the encoder at
	* the beginning of the audio. I.e. this number of leading decoded samples
	* must be discarded by the caller to get the original audio without leading
	* padding.
	*/
	/*int initial_padding;*/
	/**
	* Audio only. The amount of padding (in samples) appended by the encoder to
	* the end of the audio. I.e. this number of decoded samples must be
	* discarded by the caller from the end of the stream to get the original
	* audio without any trailing padding.
	*/
	/*int trailing_padding;*/
	/**
	* Audio only. Number of samples to skip after a discontinuity.
	*/
	/*int seek_preroll;*/
} AVCodecParameters;
/*struct AVFormatContext;

struct AVDeviceInfoList;
struct AVDeviceCapabilitiesQuery;
*/
/**
 * @defgroup metadata_api Public Metadata API
 * @{
 * @ingroup libavf
 * The metadata API allows libavformat to export metadata tags to a client
 * application when demuxing. Conversely it allows a client application to
 * set metadata when muxing.
 *
 * Metadata is exported or set as pairs of key/value strings in the 'metadata'
 * fields of the AVFormatContext, AVStream, AVChapter and AVProgram structs
 * using the @ref lavu_dict "AVDictionary" API. Like all strings in FFmpeg,
 * metadata is assumed to be UTF-8 encoded Unicode. Note that metadata
 * exported by demuxers isn't checked to be valid UTF-8 in most cases.
 *
 * Important concepts to keep in mind:
 * -  Keys are unique; there can never be 2 tags with the same key. This is
 *    also meant semantically, i.e., a demuxer should not knowingly produce
 *    several keys that are literally different but semantically identical.
 *    E.g., key=Author5, key=Author6. In this example, all authors must be
 *    placed in the same tag.
 * -  Metadata is flat, not hierarchical; there are no subtags. If you
 *    want to store, e.g., the email address of the child of producer Alice
 *    and actor Bob, that could have key=alice_and_bobs_childs_email_address.
 * -  Several modifiers can be applied to the tag name. This is done by
 *    appending a dash character ('-') and the modifier name in the order
 *    they appear in the list below -- e.g. foo-eng-sort, not foo-sort-eng.
 *    -  language -- a tag whose value is localized for a particular language
 *       is appended with the ISO 639-2/B 3-letter language code.
 *       For example: Author-ger=Michael, Author-eng=Mike
 *       The original/default language is in the unqualified "Author" tag.
 *       A demuxer should set a default if it sets any translated tag.
 *    -  sorting  -- a modified version of a tag that should be used for
 *       sorting will have '-sort' appended. E.g. artist="The Beatles",
 *       artist-sort="Beatles, The".
 * - Some protocols and demuxers support metadata updates. After a successful
 *   call to av_read_packet(), AVFormatContext.event_flags or AVStream.event_flags
 *   will be updated to indicate if metadata changed. In order to detect metadata
 *   changes on a stream, you need to loop through all streams in the AVFormatContext
 *   and check their individual event_flags.
 *
 * -  Demuxers attempt to export metadata in a generic format, however tags
 *    with no generic equivalents are left as they are stored in the container.
 *    Follows a list of generic tag names:
 *
 @verbatim
 album        -- name of the set this work belongs to
 album_artist -- main creator of the set/album, if different from artist.
				e.g. "Various Artists" for compilation albums.
 artist       -- main creator of the work
 comment      -- any additional description of the file.
 composer     -- who composed the work, if different from artist.
 copyright    -- name of copyright holder.
 creation_time-- date when the file was created, preferably in ISO 8601.
 date         -- date when the work was created, preferably in ISO 8601.
 disc         -- number of a subset, e.g. disc in a multi-disc collection.
 encoder      -- name/settings of the software/hardware that produced the file.
 encoded_by   -- person/group who created the file.
 filename     -- original name of the file.
 genre        -- <self-evident>.
 language     -- main language in which the work is performed, preferably
			in ISO 639-2 format. Multiple languages can be specified by
			separating them with commas.
 performer    -- artist who performed the work, if different from artist.
			E.g for "Also sprach Zarathustra", artist would be "Richard
			Strauss" and performer "London Philharmonic Orchestra".
 publisher    -- name of the label/publisher.
 service_name     -- name of the service in broadcasting (channel name).
 service_provider -- name of the service provider in broadcasting.
 title        -- name of the work.
 track        -- number of this work in the set, can be in form current/total.
 variant_bitrate -- the total bitrate of the bitrate variant that the current
								 stream is part of
 @endverbatim
 *
 * Look in the examples section for an application example how to use the Metadata API.
 *
 * @}
 */

/* packet functions */


/**
 * Allocate and read the payload of a packet and initialize its
 * fields with default values.
 *
 * @param s    associated IO context
 * @param pkt packet
 * @param size desired payload size
 * @return >0 (read size) if OK, AERROR_xxx otherwise
 */
/*int av_get_packet(AVIOContext *s, AVPacket *pkt, int size);*/


/**
 * Read data and append it to the current content of the AVPacket.
 * If pkt->size is 0 this is identical to av_get_packet.
 * Note that this uses av_grow_packet and thus involves a realloc
 * which is inefficient. Thus this function should only be used
 * when there is no reasonable way to know (an upper bound of)
 * the final size.
 *
 * @param s    associated IO context
 * @param pkt packet
 * @param size amount of data to read
 * @return >0 (read size) if OK, AERROR_xxx otherwise, previous data
 *         will not be lost even if an error occurs.
 */
/*int av_append_packet(AVIOContext *s, AVPacket *pkt, int size);*/

/*#if FF_API_LAVF_FRAC*/
/*************************************************/
/* fractional numbers for exact pts handling */

/**
 * The exact value of the fractional number is: 'val + num / den'.
 * num is assumed to be 0 <= num < den.
 */
/*typedef struct AVFrac {
    int64_t val, num, den;
} AVFrac;
#endif*/

/*************************************************/
/* input/output formats */

/*struct AVCodecTag;*/

/**
 * This structure contains the data a format has to probe a file.
 */

typedef struct AVProbeData {
	const char *filename;
	/*< Buffer must have AVPROBE_PADDING_SIZE of
	extra allocated bytes filled with zero. */
	unsigned char	*buf;
	int buf_size;       /* < Size of buf except extra allocated bytes */
	const char *mime_type; /* < mime_type, when known. */
} AVProbeData;

#define AVPROBE_SCORE_MAX       100 /* < maximum score */

typedef struct AVIndexEntry {
	int64_t pos;
	/**<
	 * Timestamp in AVStream.time_base units, preferably the time from which
	 * on correctly decoded frames are available
	 * when seeking to this entry. That means preferable
	 * PTS on keyframe based formats.
	 * But demuxers can choose to store a different timestamp, if it is more
	 * convenient for the implementation or nothing better
	 * is known
	 */
	int64_t timestamp;
#define AVINDEX_KEYFRAME 0x0001
	/**
	* Flag is used to indicate which frame should be discarded after decoding.
	*/
#define AVINDEX_DISCARD_FRAME  0x0002
	int flags:2;
	/* Yeah, trying to keep the size of this small to reduce memory
	requirements (it is 24 vs. 32 bytes due to possible 8-byte alignment).*/
	int size:30;
	/**< Minimum distance between this and the previous keyframe,
	used to avoid unneeded searching. */
	int min_distance;
} AVIndexEntry;

#define AV_DISPOSITION_DEFAULT   0x0001

/**
 * Stream structure.
 * New fields can be added to the end with minor version bumps.
 * Removal, reordering and changes to existing fields require a major
 * version bump.
 * sizeof(AVStream) must not be used outside libav*.
 */
typedef struct AVStream {
	int index;    /**< stream index in AVFormatContext */
	/**
	 * Format-specific stream ID.
	 * decoding: set by libavformat
	 * encoding: set by the user, replaced by libavformat if left unset
	 */
	int id;
	void *priv_data;
	/**
	 * This is the fundamental unit of time (in seconds) in terms
	 * of which frame timestamps are represented.
	 *
	 * decoding: set by libavformat
	 * encoding: May be set by the caller before avformat_write_header() to
	 *           provide a hint to the muxer about the desired timebase. In
	 *           avformat_write_header(), the muxer will overwrite this field
	 *           with the timebase that will actually be used for the timestamps
	 *           written into the file (which may or may not be related to the
	 *           user-provided one, depending on the format).
	 */
	AV_Rational time_base;

	/**
	 * Decoding: pts of the first frame of the stream in presentation order, in stream time base.
	 * Only set this if you are absolutely 100% sure that the value you set
	 * it to really is the pts of the first frame.
	 * This may be undefined (AV_NOPTS_VALUE).
	 * @note The ASF header does NOT contain a correct start_time the ASF
	 * demuxer must NOT set this.
	 */
	int64_t start_time;

	/**
	 * Decoding: duration of the stream, in stream time base.
	 * If a source file does not specify a duration, but does specify
	 * a bitrate, this value will be estimated from bitrate and file size.
	 */
	int64_t duration;

	int64_t nb_frames; /* < number of frames in this stream if known or 0 */

	int disposition; /**< AV_DISPOSITION_* bit field */
	int64_t cur_dts;
	AVIndexEntry *index_entries;
	int nb_index_entries;
	unsigned int index_entries_allocated_size;
	/**
	 * Number of samples to skip at the start of the frame decoded from the next packet.
	 */
	int skip_samples;
	/*
	 * Codec parameters associated with this stream. Allocated and freed by
	 * libavformat in avformat_new_stream() and avformat_free_context()
	 * respectively.
	 *
	 * - demuxing: filled by libavformat on stream creation or in
	 *             avformat_find_stream_info()
	 * - muxing: filled by the caller before avformat_write_header()
	 */
	AV_CodecParameters *codecpar;
} AVStream;

/**
 * Format I/O context.
 * New fields can be added to the end with minor version bumps.
 * Removal, reordering and changes to existing fields require a major
 * version bump.
 * sizeof(AVFormatContext) must not be used outside libav*, use
 * avformat_alloc_context() to create an AVFormatContext.
 *
 * Fields can be accessed through AVOptions (av_opt*),
 * the name string used matches the associated command line parameter name and
 * can be found in libavformat/options_table.h.
 * The AVOption/command line parameter names differ in some cases from the C
 * structure field names for historic reasons or brevity.
 */
typedef struct AVFormatContext {
	/**
	 * Format private data. This is an AVOptions-enabled struct
	 * if and only if iformat/oformat.priv_class is not NULL.
	 *
	 * - muxing: set by avformat_write_header()
	 * - demuxing: set by avformat_open_input()
	 */
	void *priv_data;

	/**
	 * I/O context.
	 *
	 * - demuxing: either set by the user before avformat_open_input() (then
	 *             the user must close it manually) or set by avformat_open_input().
	 * - muxing: set by the user before avformat_write_header(). The caller must
	 *           take care of closing / freeing the IO context.
	 *
	 * Do NOT set this field if AVFMT_NOFILE flag is set in
	 * iformat/oformat.flags. In such a case, the (de)muxer will handle
	 * I/O in some other way and this field will be NULL.
	 */
	AVIOContext *pb;


	/* stream info */
	/**
	 * Flags signalling stream properties. A combination of AVFMTCTX_*.
	 * Set by libavformat.
	 */
	/*    int ctx_flags;*/

	/**
	 * Number of elements in AVFormatContext.streams.
	 *
	 * Set by avformat_new_stream(), must not be modified by any other code.
	 */
	unsigned int nb_streams;
	/**
	 * A list of all streams in the file. New streams are created with
	 * avformat_new_stream().
	 *
	 * - demuxing: streams are created by libavformat in avformat_open_input().
	 *             If AVFMTCTX_NOHEADER is set in ctx_flags, then new streams may also
	 *             appear in av_read_frame().
	 * - muxing: streams are created by the user before avformat_write_header().
	 *
	 * Freed by libavformat in avformat_free_context().
	 */
	AVStream * streams[16];
	/**
	 * Duration of the stream, in AV_TIME_BASE fractional
	 * seconds. Only set this value if you know none of the individual stream
	 * durations and also do not set any of them. This is deduced from the
	 * AVStream values if not set.
	 *
	 * Demuxing only, set by libavformat.
	 */
	 int64_t duration;

	/**
	 * Flags modifying the (de)muxer behaviour. A combination of AVFMT_FLAG_*.
	 * Set by the user before avformat_open_input() / avformat_write_header().
	 */
	int flags;

	/**
	 * Number of chapters in AVChapter array.
	 * When muxing, chapters are normally written in the file header,
	 * so nb_chapters should normally be initialized before write_header
	 * is called. Some muxers (e.g. mov and mkv) can also write chapters
	 * in the trailer.  To write chapters in the trailer, nb_chapters
	 * must be zero when write_header is called and non-zero when
	 * write_trailer is called.
	 * - muxing: set by user
	 * - demuxing: set by libavformat
	 */
	unsigned int nb_chapters;
	int64_t data_offset; /**< offset of the first packet */
	/**
	 * The maximum number of streams.
	 * - encoding: unused
	 * - decoding: set by user through AVOptions (NO direct access)
	 */
	int max_streams;
} AVFormatContext;

#define AVSEEK_FLAG_BACKWARD 1 /* < seek backward */
#define AVSEEK_FLAG_BYTE     2 /* < seeking based on position in bytes */
#define AVSEEK_FLAG_ANY      4 /* < seek to any frame, even non-keyframes */
#define AVSEEK_FLAG_FRAME    8 /* < seeking based on frame number */

#endif /* __AVFORMAT_H__ */
