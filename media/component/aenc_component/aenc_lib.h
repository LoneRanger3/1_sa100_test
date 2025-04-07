/*
* copyright (c) 2018 All Rights Reserved
*
* This file is part of audio. this is a temp file,and maybe del for furtune
*
* File   : aenc_lib.h
* Version: V1.0
* Date   : 2018/8/21 17:16:50
* Other  : ffmpeg-3.2.7
*/
#ifndef AENC_LIB_H
#define AENC_LIB_H
#include "decode_com.h"

int pcmenc_init(struct tag_audio_dec *penc);
int pcmenc_frame(struct tag_audio_dec *penc, packet_t *pin, packet_t *pout);
int pcmenc_close(struct tag_audio_dec *penc);
int wavenc_init(struct tag_audio_dec *penc);
int wavenc_frame(struct tag_audio_dec *penc, packet_t *pin, packet_t *pout);
int wavenc_close(struct tag_audio_dec *penc);
int aacenc_init(struct tag_audio_dec *penc);
int aacenc_frame(struct tag_audio_dec *penc, packet_t *pin, packet_t *pout);
int aacenc_close(struct tag_audio_dec *penc);

#endif /* AENC_LIB_H */
