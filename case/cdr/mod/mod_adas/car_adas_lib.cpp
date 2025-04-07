/*
 * car_adas_lib.cpp - the Auxiliary Driving System of car
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
#include "car_adas_lib.h"

rt_sem_t sem_adas_data;
rt_sem_t sem_adas_proc;
rt_sem_t sem_adas_mutex;
int get_data_proc_thread_exit;
int set_data_proc_thread_exit;

/* #define STATICS_ADAS	1 */

#ifdef STATICS_ADAS
static rt_tick_t set_prv_time = 0L;
static rt_tick_t set_cur_time = 0L;
static rt_tick_t get_cur_time = 0;
static rt_tick_t total_time = 0;
static int set_cnt_times = 0;
static int cnt_times = 0;
#endif

HDFrameSetData frame_set_data;
HDFrameGetData frame_get_data;
HDIniSetData init_set_data;

HDPed ped_info[32];/* the number of detecting ped is 32 */
HDCar car_info[32];/* the number of detecting car is 32 */

/**
 * adas_lib_set_data_cb -callback interface for adas lib
 * frameSetData adas lib get image data from this pointer
 * dv:no use
 *
 * This function use to callback interface for adas lib
 *
 * Returns NULL
 */
void adas_lib_set_data_cb(HDFrameSetData *frameSetData,void *dv)
{
	if(!get_data_proc_thread_exit) {
		RT_ASSERT(sem_adas_data);
		rt_sem_take(sem_adas_data, 20);
		/* need to fit */
		/* change the paremeters of the frameSetData through callback maybe */
		memcpy(frameSetData,&frame_set_data,sizeof(HDFrameSetData));
	}
	#ifdef STATICS_ADAS
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

void adas_lib_get_data_cb(HDFrameGetData *frameGetData,void *dv)
{
	#ifdef STATICS_ADAS
	get_cur_time = rt_time_get_msec();
	cnt_times++;
	total_time += get_cur_time - set_cur_time;
	if(cnt_times == 10){
		printf("al use times:%d\n", total_time/10);
		cnt_times = 0;
		total_time = 0;
	}
	#endif
	frame_get_data.reCalibrate = frameGetData->reCalibrate;
	frame_get_data.isDay = frameGetData->isDay;
	frame_get_data.timeStamp = frameGetData->timeStamp;

	/* some infomation about lanes */
	memcpy(&frame_get_data.lanes,&frameGetData->lanes,sizeof(HDLanes));

	/* some infomation about cars */
	memcpy(&frame_get_data.cars.warmCar,\
		&frameGetData->cars.warmCar,sizeof(HDWarmObject));
	frame_get_data.cars.num = frameGetData->cars.num;
	RT_ASSERT(frame_get_data.cars.p);
	if (frame_get_data.cars.num)
		memcpy((unsigned char*)frame_get_data.cars.p,\
		(unsigned char*)frameGetData->cars.p,\
		(sizeof(HDCar)*frame_get_data.cars.num));
	frame_get_data.cars.memSize = frameGetData->cars.memSize;

	/* some infomation about peds */
	memcpy(&frame_get_data.peds.warmPed,\
		&frameGetData->peds.warmPed,sizeof(HDWarmObject));
	frame_get_data.peds.num = frameGetData->peds.num;
	RT_ASSERT(frame_get_data.peds.p);
	if(frame_get_data.peds.num)
		memcpy((unsigned char*)frame_get_data.peds.p,\
		(unsigned char*)frameGetData->peds.p,\
		(sizeof(HDPed)*frame_get_data.peds.num));
	frame_get_data.peds.memSize = frameGetData->peds.memSize;

	frame_get_data.fiatigue = frameGetData->fiatigue;
	frame_get_data.lameOn.value = frameGetData->lameOn.value;
	frame_get_data.skyLine.isFullLearn = frameGetData->skyLine.isFullLearn;
	frame_get_data.skyLine.value = frameGetData->skyLine.value;

	RT_ASSERT(sem_adas_proc);
	rt_sem_release(sem_adas_proc);
}

int adas_lib_init(HDIniSetData *init_set_data)
{
	HDIniGetData init_get_data;
	void **dev=NULL;

	memset((unsigned char*)&ped_info,0x00,sizeof(ped_info));
	memset((unsigned char*)&car_info,0x00,sizeof(car_info));

	/* it should be dynamically allocated */
	frame_get_data.cars.p = car_info;
	/* it should be dynamically allocated */
	frame_get_data.peds.p = ped_info;

	SetAdasData = adas_lib_set_data_cb;
	GetAdasData = adas_lib_get_data_cb;
	CreatAdas(init_set_data,&init_get_data,dev);

	return 0;
}

int adas_lib_exit(void)
{
	DeleteAdas();

	return 0;
}

int adas_sem_init(void)
{
	int ret = 0;

	sem_adas_data = rt_sem_create("adas_data",0,RT_IPC_FLAG_FIFO);
	if (NULL == sem_adas_data)
		return 4;

	sem_adas_proc = rt_sem_create("adas_proc",0,RT_IPC_FLAG_FIFO);
	if (NULL == sem_adas_proc)
		return 4;

	sem_adas_mutex = rt_sem_create("adas_mutex",1,RT_IPC_FLAG_FIFO);
	if (NULL == sem_adas_mutex)
		return 4;

	return ret;
}

int adas_sem_exit(void)
{
	int ret = 0;

	if (sem_adas_data) {
		rt_sem_delete(sem_adas_data);
		sem_adas_data = NULL;
	}

	if (sem_adas_proc) {
		rt_sem_delete(sem_adas_proc);
		sem_adas_proc = NULL;
	}

	if (sem_adas_mutex) {
		rt_sem_delete(sem_adas_mutex);
		sem_adas_mutex = NULL;
	}

	ret = 0;

	return ret;
}
