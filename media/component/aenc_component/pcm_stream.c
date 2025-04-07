/*
 * pcm_steam.c - demo code for aenc component.
 *
 * Copyright (C) 2016-2018, LomboTech Co.Ltd.
 * Author: lomboswer <lomboswer@lombotech.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
#include <oscl.h>
#include <base_component.h>
#include "pcm_stream.h"

pcm_buf_manager *pcm_buf_init(void)
{
	pcm_buf_manager *pcm_manager = NULL;

	pcm_manager = oscl_zalloc(sizeof(pcm_buf_manager));
	if (!pcm_manager) {
		OSCL_LOGE("malloc pcm_manager error!\n");
		return NULL;
	}
	pcm_manager->bufStart = oscl_zalloc(PCM_BUFFER_SIZE);
	if (!pcm_manager->bufStart) {
		OSCL_LOGE("malloc pcm_manager bufStart error!\n");
		oscl_free(pcm_manager);
		pcm_manager = NULL;
		return NULL;
	}
	pcm_manager->bufWrite = pcm_manager->bufStart;
	pcm_manager->bufRead = pcm_manager->bufStart;
	pcm_manager->bufTotalLen = PCM_BUFFER_SIZE;
	pcm_manager->bufDataLen = 0;
	pcm_manager->nFlags = 0;

	return pcm_manager;
}

int pcm_buf_reset(pcm_buf_manager *pcm_manager)
{
	pcm_manager->bufWrite = pcm_manager->bufStart;
	pcm_manager->bufRead = pcm_manager->bufStart;
	pcm_manager->bufDataLen = 0;
	pcm_manager->nFlags = 0;

	return 0;
}

int pcm_buf_deinit(pcm_buf_manager *pcm_manager)
{
	int ret = 0;
	if (!pcm_manager)
		return -1;
	if (pcm_manager->bufStart) {
		oscl_free(pcm_manager->bufStart);
		pcm_manager->bufStart = NULL;
	}

	if (pcm_manager)
		oscl_free(pcm_manager);

	return ret;
}

/*
********************************************************************************
*Description: read pcm data for audio encode lib.
*
*Arguments  : ptr       buffer for store pcm data;
*             len       length of pcm data that audio encode lib needed;
*             pcm_manager   pcm buffer manager;
*
*Return     : length of pcm data got actually;
********************************************************************************
*/
int readPcmData(unsigned char *ptr, int len, pcm_buf_manager *pcm_manager)
{
	if (!ptr || !pcm_manager || (len < 0))
		return -1;

	/* end EOS */
	if (pcm_manager->nFlags) {
		if (len > pcm_manager->bufDataLen)
			len = pcm_manager->bufDataLen;
	}
	if (pcm_manager->bufDataLen < len) {
		OSCL_LOGD("read len:%d,datalen:%d", len, pcm_manager->bufDataLen);
		return 0;
	}
	if ((pcm_manager->bufRead + len) >
		(pcm_manager->bufStart + pcm_manager->bufTotalLen)) {
		int size = pcm_manager->bufStart +
			pcm_manager->bufTotalLen - pcm_manager->bufRead;
		memcpy(ptr, pcm_manager->bufRead, size);
		memcpy(ptr + size, pcm_manager->bufStart, len - size);
	} else {
		memcpy(ptr, pcm_manager->bufRead, len);
	}

	pcm_manager->bufRead += len;
	pcm_manager->bufDataLen -= len;
	if (pcm_manager->bufRead >=
		(pcm_manager->bufStart + pcm_manager->bufTotalLen))
		pcm_manager->bufRead -= pcm_manager->bufTotalLen;

	return len;
}

/*
********************************************************************************
*Description: write pcm data to pcm buffer manager.
*
*Arguments  : ptr       buffer for store pcm data;
*             len       length of pcm data that audio drv send;
*             pcm_manager   pcm buffer manager;
*
*Return     : length of pcm data got actually;
********************************************************************************
*/
int writePcmData(unsigned char *ptr, int len, pcm_buf_manager *pcm_manager)
{
	if (!ptr || !pcm_manager)
		return -1;

	if ((pcm_manager->bufTotalLen - pcm_manager->bufDataLen) < len) {
		oscl_mdelay(10);
		OSCL_LOGI("write len:%d,datalen:%d", len, pcm_manager->bufDataLen);
		return 0;
	}

	if ((pcm_manager->bufWrite + len) >
		(pcm_manager->bufStart + pcm_manager->bufTotalLen)) {
		int size = pcm_manager->bufStart +
			pcm_manager->bufTotalLen - pcm_manager->bufWrite;
		memcpy(pcm_manager->bufWrite, ptr, size);
		memcpy(pcm_manager->bufStart, ptr + size, len - size);
	} else
		memcpy(pcm_manager->bufWrite, ptr, len);

	pcm_manager->bufWrite += len;
	pcm_manager->bufDataLen += len;
	if (pcm_manager->bufWrite >=
		(pcm_manager->bufStart + pcm_manager->bufTotalLen))
		pcm_manager->bufWrite -= pcm_manager->bufTotalLen;

	return len;
}
