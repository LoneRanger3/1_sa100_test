/*
 * asoundlib.h - common lib api used by lombo audio.
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

#ifndef __ASOUNDLIB__
#define __ASOUNDLIB__

#define PCM_OUT        0x00000000
#define PCM_IN         0x10000000

/* Configuration for a stream */
typedef struct pcm_config {
	unsigned int channels;
	unsigned int rate;
	unsigned int bitwidth;
	unsigned int period_size;
	unsigned int period_count;
} pcm_config_t;

/* Open and close a stream */
void *pcm_open(unsigned int flags, pcm_config_t *config);
int pcm_close(void *pcm, unsigned int flags);
/* Write data to the fifo.
 * Will start playback on the first write or on a write that
 * occurs after a fifo underrun.
 */
int pcm_write(void *pcm, void *data, unsigned int count);
int pcm_read(void *pcm, void *data, unsigned int count);
int pcm_flush(void *pcm, unsigned int flags);

int pcm_set_volume(int volume, unsigned int flags);

int pcm_set_mute(int mute, unsigned int flags);


/* Returns the pcm latency in ms */
unsigned int pcm_get_latency(void *pcm, unsigned int flags);

#endif /* __ASOUNDLIB__ */

