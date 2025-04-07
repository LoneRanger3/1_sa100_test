/*
 * copyright (c) 2018 All Rights Reserved
 *
 * This file is part of audio.
 *
 * File   : decode_com.h
 * Version: V1.0
 * Date   : 2018/8/21 17:16:50
 */

#ifndef	DECODE_COM_H
#define DECODE_COM_H

#include "audio_common.h"

typedef enum __AUDIO_DEC_RESULT {
	DEC_FLUSH = -4,   /* flush decode lib */
	DEC_ERRINIT = -3, /* audio init error */
	DEC_BSEND = -2,   /* audio bitstream decode end */
	DEC_ERROR = -1,   /* frame failed */

	DEC_SUCCESS = 0,   /* decode success. */

} audio_dec_result_t;

typedef struct tag_audio_dec {
	int init_flag;
	AV_CodecParameters *para;
	void *priv;
	int (*init)(struct tag_audio_dec *pdec);
	int (*decode)(struct tag_audio_dec *pdec, packet_t *pin, packet_t *pout);
	int (*close)(struct tag_audio_dec *pdec);
	int(*flush)(struct tag_audio_dec *pdec);
} audio_dec_t;

#endif /* DECODE_COM_H */
