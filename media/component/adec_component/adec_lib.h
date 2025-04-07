/*
* copyright (c) 2018 All Rights Reserved
*
* This file is part of audio. this is a temp file,and maybe del for furtune
*
* File   : adec_lib.h
* Version: V1.0
* Date   : 2018/8/21 17:16:50
* Other  : ffmpeg-3.2.7
*/
#ifndef ADEC_LIB_H
#define ADEC_LIB_H
#include "decode_com.h"

int  AAC_DecOpen(struct  tag_audio_dec *pdec);
int  WAV_DecOpen(struct  tag_audio_dec *pdec);

#endif /* ADEC_LIB_H */
