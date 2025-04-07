/*
 * car_bsd_lib.cpp - the Auxiliary Driving System of car
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <finsh.h>
#include <rtthread.h>
#include <string.h>
#include <stdio.h>
#include "libbsd.h"
#include "car_bsd_lib.h"

rt_sem_t sem_bsd_data;
rt_sem_t sem_bsd_proc;
rt_sem_t sem_bsd_mutex;
int get_data_proc_thread_exit;
int set_data_proc_thread_exit;

/* #define STATICS_BSD	1  */

#ifdef STATICS_BSD
static rt_tick_t set_prv_time = 0L;
static rt_tick_t set_cur_time = 0L;
static rt_tick_t get_cur_time = 0;
static rt_tick_t total_time = 0;
static int set_cnt_times = 0;
static int cnt_times = 0;
#endif

HDFrameSetBsd frame_set_data;
HDFrameGetBsd frame_get_data;
HDIniSetBsd init_set_data;

HDPedBsd ped_info[32];/* the number of detecting ped is 32 */
HDCarBsd car_info[32];/* the number of detecting car is 32 */

/**
 * bsd_lib_set_data_cb -callback interface for bsd lib
 * frameSetData bsd lib get image data from this pointer
 * dv:no use
 *
 * This function use to callback interface for bsd lib
 *
 * Returns NULL
 */
void bsd_lib_set_data_cb(HDFrameSetBsd *frameSetData,void *dv)
{
	if(!get_data_proc_thread_exit) {
		RT_ASSERT(sem_bsd_data);
		rt_sem_take(sem_bsd_data, 20);
		/* need to fit */
		/* change the paremeters of the frameSetData through callback maybe */
		memcpy(frameSetData,&frame_set_data,sizeof(HDFrameSetBsd));
	}
	#ifdef STATICS_BSD
	if(set_cnt_times == 0)
		 set_prv_time = rt_time_get_msec();
	set_cnt_times++;
	set_cur_time = rt_time_get_msec();
	if(set_cur_time - set_prv_time > 1000) {
		printf("cnt_times:%d\n", set_cnt_times);
		set_cnt_times = 0;
	}
	#endif

}

void bsd_lib_get_data_cb(HDFrameGetBsd *frameGetData,void *dv)
{
	#ifdef STATICS_BSD
	get_cur_time = rt_time_get_msec();
	cnt_times++;
	total_time += get_cur_time - set_cur_time;
	if(cnt_times == 10){
		printf("al use times:%d\n", total_time/10);
		cnt_times = 0;
		total_time = 0;
	}
	#endif
	memcpy(&frame_get_data, frameGetData, sizeof(HDFrameGetBsd));
#if 0
	frame_get_data.reCalibrate = frameGetData->reCalibrate;
	frame_get_data.isDay = frameGetData->isDay;
	frame_get_data.timeStamp = frameGetData->timeStamp;

	/* some infomation about lanes */
	memcpy(&frame_get_data.lanes,&frameGetData->lanes,sizeof(HDLanesBsd));

	/* some infomation about cars */
	memcpy(&frame_get_data.cars.warmCar,\
		&frameGetData->cars.warmCar,sizeof(HDWarmObjectBsd));
	frame_get_data.cars.num = frameGetData->cars.num;
	RT_ASSERT(frame_get_data.cars.p);
	if (frame_get_data.cars.num)
		memcpy((unsigned char*)frame_get_data.cars.p,\
		(unsigned char*)frameGetData->cars.p,\
		(sizeof(HDPedBsd)*frame_get_data.cars.num));
	frame_get_data.cars.memSize = frameGetData->cars.memSize;

	/* some infomation about peds */
	memcpy(&frame_get_data.peds.warmPed,\
		&frameGetData->peds.warmPed,sizeof(HDWarmObjectBsd));
	frame_get_data.peds.num = frameGetData->peds.num;
	RT_ASSERT(frame_get_data.peds.p);
	if(frame_get_data.peds.num)
		memcpy((unsigned char*)frame_get_data.peds.p,\
		(unsigned char*)frameGetData->peds.p,\
		(sizeof(HDPedBsd)*frame_get_data.peds.num));
	frame_get_data.peds.memSize = frameGetData->peds.memSize;

	frame_get_data.fiatigue = frameGetData->fiatigue;
	frame_get_data.lameOn.value = frameGetData->lameOn.value;
	frame_get_data.skyLine.isFullLearn = frameGetData->skyLine.isFullLearn;
	frame_get_data.skyLine.value = frameGetData->skyLine.value;
#endif
	RT_ASSERT(sem_bsd_proc);
	rt_sem_release(sem_bsd_proc);
}

int bsd_lib_init(HDIniSetBsd *init_set_data)
{
	HDIniGetBsd init_get_data;
	void **dev=NULL;

	memset((unsigned char*)&ped_info,0x00,sizeof(ped_info));
	memset((unsigned char*)&car_info,0x00,sizeof(car_info));

	/* it should be dynamically allocated */
	frame_get_data.cars.p = car_info;
	/* it should be dynamically allocated */
	frame_get_data.peds.p = ped_info;

	SetBsdData = bsd_lib_set_data_cb;
	GetBsdData = bsd_lib_get_data_cb;
	CreatBsd(init_set_data,&init_get_data,dev);

	return 0;
}

int bsd_lib_exit(void)
{
	DeleteBsd();

	return 0;
}

int bsd_sem_init(void)
{
	int ret = 0;

	sem_bsd_data = rt_sem_create("bsd_data",0,RT_IPC_FLAG_FIFO);
	if (NULL == sem_bsd_data)
		return 4;

	sem_bsd_proc = rt_sem_create("bsd_proc",0,RT_IPC_FLAG_FIFO);
	if (NULL == sem_bsd_proc)
		return 4;

	sem_bsd_mutex = rt_sem_create("bsd_mutex",1,RT_IPC_FLAG_FIFO);
	if (NULL == sem_bsd_mutex)
		return 4;

	return ret;
}

int bsd_sem_exit(void)
{
	int ret = 0;

	if (sem_bsd_data) {
		rt_sem_delete(sem_bsd_data);
		sem_bsd_data = NULL;
	}

	if (sem_bsd_proc) {
		rt_sem_delete(sem_bsd_proc);
		sem_bsd_proc = NULL;
	}

	if (sem_bsd_mutex) {
		rt_sem_delete(sem_bsd_mutex);
		sem_bsd_mutex = NULL;
	}

	ret = 0;

	return ret;
}
