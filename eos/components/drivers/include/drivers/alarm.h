/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2012-10-27     heyuanjie87       first version.
 */

#ifndef __ALARM_H__
#define __ALARM_H__

#include <time.h>

#define RT_ALARM_TM_NOW        -1    /* set the alarm tm_day,tm_mon,tm_sec,etc.
                                        to now.we also call it "don't care" value */

/* alarm flags */
#define RT_ALARM_ONESHOT       0x000 /* only alarm onece */
#define RT_ALARM_DAILY         0x100 /* alarm everyday */
#define RT_ALARM_WEEKLY        0x200 /* alarm weekly at Monday or Friday etc. */
#define RT_ALARM_MONTHLY       0x400 /* alarm monthly at someday */
#define RT_ALARM_YAERLY        0x800 /* alarm yearly at a certain date */

/* alarm control cmd */
#define RT_ALARM_CTRL_MODIFY       1 /* modify alarm time or alarm flag */

typedef struct rt_alarm *rt_alarm_t;
typedef void (*rt_alarm_callback_t)(rt_alarm_t alarm, time_t timestamp);

/* used for low level RTC driver */
struct rt_rtc_wkalarm
{
    rt_bool_t  enable;               /* 0 = alarm disabled, 1 = alarm enabled */
    rt_int32_t tm_sec;               /* alarm at tm_sec */
    rt_int32_t tm_min;               /* alarm at tm_min */
    rt_int32_t tm_hour;              /* alarm at tm_hour */
};

struct rt_alarm
{
    rt_list_t list;
    rt_uint32_t flag;
    rt_alarm_callback_t callback;
    struct tm wktime;
};

struct rt_alarm_setup
{
    rt_uint32_t flag;                /* alarm flag */
    struct tm wktime;                /* when will the alarm wake up user */
};

struct rt_alarm_container
{
    rt_list_t head;
    struct rt_mutex mutex;
    struct rt_event event;
    struct rt_alarm *current;
};

rt_alarm_t rt_alarm_create(rt_alarm_callback_t    callback,
                           struct rt_alarm_setup *setup);
rt_err_t rt_alarm_control(rt_alarm_t alarm, int cmd, void *arg);
void rt_alarm_update(rt_device_t dev, rt_uint32_t event);
rt_err_t rt_alarm_delete(rt_alarm_t alarm);
rt_err_t rt_alarm_start(rt_alarm_t alarm);
rt_err_t rt_alarm_stop(rt_alarm_t alarm);
int rt_alarm_system_init(void);

#endif /* __ALARM_H__ */
