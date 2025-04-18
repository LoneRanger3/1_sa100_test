/*
 * File      : title.h
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2009, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-10-16     Bernard      first version
 */
#ifndef __RTGUI_TITLE__
#define __RTGUI_TITLE__

#include <rtgui/widgets/toplevel.h>

/** Gets the type of a top win */
#define RTGUI_WINTITLE_TYPE       (rtgui_wintitle_type_get())
/** Casts the object to an rtgui_wintitle */
#define RTGUI_WINTITLE(obj)       (RTGUI_OBJECT_CAST((obj), RTGUI_WINTITLE_TYPE, rtgui_wintitle_t))
/** Checks if the object is an rtgui_wintitle */
#define RTGUI_IS_WINTITLE(obj)    (RTGUI_OBJECT_CHECK_TYPE((obj), RTGUI_WINTITLE_TYPE))

struct rtgui_wintitle
{
	struct rtgui_toplevel parent;

	/* title */
	rt_uint8_t* title;
};
typedef struct rtgui_wintitle rtgui_wintitle_t;

rtgui_type_t* rtgui_wintitle_type_get(void);

rtgui_wintitle_t* rtgui_wintitle_create(const rt_uint8_t* title);
void rtgui_wintitle_destroy(rtgui_wintitle_t* wintitle);

rt_bool_t rtgui_wintile_event_handler(rtgui_widget_t* widget, rtgui_event_t* event);

void rtgui_wintitle_set_title(rtgui_wintitle_t* wintitle, const rt_uint8_t* title);
rt_uint8_t *rtgui_wintitle_get_title(rtgui_wintitle_t* wintitle);

#endif
