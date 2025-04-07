
/**************************************************************************************
 *
 * statname.h - name mangling macros for static linking
 **************************************************************************************/

#ifndef STATNAME_H
#define STATNAME_H

/* define STAT_PREFIX to a unique name for static linking
 * all the C functions and global variables will be mangled by the preprocessor
 *   e.g. void FFT(int *fftbuf) becomes void cook_FFT(int *fftbuf)
 */
#define STAT_PREFIX		LIBMP4MUXER

#define STATCC2(x, y)	x##y
#define STATCC1(x, y)	STATCC2(x, y)


#ifdef STAT_PREFIX
#define STATNAME(func)	STATCC1(STAT_PREFIX, func)
#else
#define STATNAME(func)	func
#endif

/* global functions */

/* for ffmpeg */
#define ff_mp4_obj_type                        STATNAME(ff_mp4_obj_type)
#define av_free                                STATNAME(av_free)
#define av_freep                               STATNAME(av_freep)
#define av_malloc                              STATNAME(av_malloc)
#define av_mallocz                             STATNAME(av_mallocz)
#define av_realloc                             STATNAME(av_realloc)
#ifndef __BS_FFMPEG
#define avio_wl16                              STATNAME(avio_wl16)
#define avio_wb16                              STATNAME(avio_wb16)
#define avio_wb24                              STATNAME(avio_wb24)
#define avio_wl32                              STATNAME(avio_wl32)
#define avio_wb64                              STATNAME(avio_wb64)
#define avio_write                             STATNAME(avio_write)
#endif
#define av_rescale_rnd                         STATNAME(av_rescale_rnd)
#define av_get_bits_per_sample                 STATNAME(av_get_bits_per_sample)
#define ff_put_wav_header                      STATNAME(ff_put_wav_header)
#define ff_avc_parse_nal_units                 STATNAME(ff_avc_parse_nal_units)
#define ff_avc_parse_nal_units_buf             STATNAME(ff_avc_parse_nal_units_buf)
#define ff_isom_write_avcc                     STATNAME(ff_isom_write_avcc)
#define ff_mov_write_packet                    STATNAME(ff_mov_write_packet)
#define ff_mov_get_channel_layout_tag  \
			STATNAME(ff_mov_wff_mov_get_channel_layout_tagrite_packet)

/* global (const) data */
/* #define pkkey				STATNAME(pkkey) */

/* assembly functions - either inline or in separate asm file */
/* #define MULSHIFT32			STATNAME(MULSHIFT32) */

#endif/*STATNAME_H*/
