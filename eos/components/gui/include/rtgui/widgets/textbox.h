/*
 * File      : textbox.h
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
#ifndef __RTGUI_TEXTBOX_H__
#define __RTGUI_TEXTBOX_H__

#include <rtgui/rtgui.h>
#include <rtgui/widgets/widget.h>

/** Gets the type of a textbox */
#define RTGUI_TEXTBOX_TYPE       (rtgui_textbox_type_get())
/** Casts the object to a rtgui_textbox */
#define RTGUI_TEXTBOX(obj)       (RTGUI_OBJECT_CAST((obj), RTGUI_TEXTBOX_TYPE, rtgui_textbox_t))
/** Checks if the object is a rtgui_textbox */
#define RTGUI_IS_TEXTBOX(obj)    (RTGUI_OBJECT_CHECK_TYPE((obj), RTGUI_TEXTBOX_TYPE))

#define RTGUI_TEXTBOX_DEFAULT_WIDTH		80
#define RTGUI_TEXTBOX_DEFAULT_HEIGHT	20

#define RTGUI_TEXTBOX_SINGLE		0x00
#define RTGUI_TEXTBOX_MULTI		0x00

struct rtgui_textbox_line
{
	rt_uint8_t* line_text;

	struct rtgui_textbox_line *prev, *next;
};

struct rtgui_textbox
{
	/* inherit from widget */
	struct rtgui_widget parent;

	/* text box type */
	rt_ubase_t type;

	/* current line and position */
	rt_size_t line, line_begin, position, line_length;

	rt_uint8_t* text;
	rt_size_t font_width;

	struct rtgui_timer* caret_timer;

	/* widget private data */
	rt_bool_t (*on_enter) 	(struct rtgui_widget* widget, struct rtgui_event* event);
};
typedef struct rtgui_textbox rtgui_textbox_t;

struct rtgui_textbox* rtgui_textbox_create(const char* text);
void rtgui_textbox_destroy(struct rtgui_textbox* box);

rt_bool_t rtgui_textbox_event_handler(struct rtgui_widget* widget, struct rtgui_event* event);

void rtgui_textbox_set_value(struct rtgui_textbox* box, const char* text);
const char* rtgui_textbox_get_value(struct rtgui_textbox* box);

void rtgui_widget_set_line_length(struct rtgui_textbox* box, rt_size_t length);

#endif
