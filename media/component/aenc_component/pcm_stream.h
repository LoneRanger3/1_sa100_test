/*
 * copyright (c) 2018 All Rights Reserved
 *
 * This file is part of audio.
 *
 * File   : encode_com.h
 * Version: V1.0
 * Date   : 2018/8/21 17:16:50
 */

#ifndef	ENCODE_COM_H
#define ENCODE_COM_H

#define PCM_BUFFER_SIZE (1024 * 1024)
#define PCM_FRAME_SIZE (4 * 1024)

/* not support buf encode with drv same one */
typedef struct pcm_buf_manager {
	unsigned char *bufStart;
	unsigned char *bufWrite;
	unsigned char *bufRead;
	int bufTotalLen;
	int bufDataLen;
	int nFlags;/*0x1:EOS */
} pcm_buf_manager;
pcm_buf_manager *pcm_buf_init(void);
int pcm_buf_deinit(pcm_buf_manager *pcm_manager);
int readPcmData(unsigned char *ptr, int len, pcm_buf_manager *pcm_manager);
int writePcmData(unsigned char *ptr, int len, pcm_buf_manager *pcm_manager);
int pcm_buf_reset(pcm_buf_manager *pcm_manager);


#endif/* ENCODE_COM_H */
