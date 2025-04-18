/*
 * File      : box.h
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
#ifndef __RTGUI_BOX_H__
#define __RTGUI_BOX_H__

#include <rtgui/rtgui.h>
#include <rtgui/widgets/widget.h>
#include <rtgui/widgets/container.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Gets the type of a box */
#define RTGUI_BOX_TYPE       (rtgui_box_type_get())
/** Casts the object to an rtgui_box */
#define RTGUI_BOX(obj)       (RTGUI_OBJECT_CAST((obj), RTGUI_BOX_TYPE, rtgui_box_t))
/** Checks if the object is an rtgui_box */
#define RTGUI_IS_BOX(obj)    (RTGUI_OBJECT_CHECK_TYPE((obj), RTGUI_BOX_TYPE))

struct rtgui_box
{
	struct rtgui_container parent;

	rt_uint16_t orientation;
	rt_uint16_t border_size;
};
typedef struct rtgui_box rtgui_box_t;

rtgui_type_t *rtgui_box_type_get(void);

struct rtgui_box* rtgui_box_create(int orientation, rtgui_rect_t* rect);
void rtgui_box_destroy(struct rtgui_box* box);

rt_bool_t rtgui_box_event_handler(rtgui_widget_t* widget, rtgui_event_t* event);

void rtgui_box_append(rtgui_box_t* box, rtgui_widget_t* widget);
void rtgui_box_layout(rtgui_box_t* box);

rt_uint32_t rtgui_box_get_width(rtgui_box_t* box);
rt_uint32_t rtgui_box_get_height(rtgui_box_t* box);

#ifdef __cplusplus
}
#endif

#endif
