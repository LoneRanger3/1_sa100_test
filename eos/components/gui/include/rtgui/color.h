/*
 * File      : color.h
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
#ifndef __RTGUI_COLOR_H__
#define __RTGUI_COLOR_H__

#include <rtgui/rtgui.h>

typedef unsigned long rtgui_color_t;
#define RTGUI_ARGB(a, r, g, b)	\
		((rtgui_color_t)(((rt_uint8_t)(r)|\
		(((unsigned)(rt_uint8_t)(g))<<8))|\
		(((unsigned long)(rt_uint8_t)(b))<<16)|\
		(((unsigned long)(rt_uint8_t)(a))<<24)))
#define RTGUI_RGB(r, g, b)	RTGUI_ARGB(255, (r), (g), (b))

#define RTGUI_RGB_R(c)	((c) & 0xff)
#define RTGUI_RGB_G(c)	(((c) >> 8)  & 0xff)
#define RTGUI_RGB_B(c)	(((c) >> 16) & 0xff)
#define RTGUI_RGB_A(c)	(((c) >> 24) & 0xff)

extern const rtgui_color_t default_foreground;
extern const rtgui_color_t default_background;

extern const rtgui_color_t red;
extern const rtgui_color_t green;
extern const rtgui_color_t blue;
extern const rtgui_color_t black;
extern const rtgui_color_t white;
extern const rtgui_color_t high_light;
extern const rtgui_color_t dark_grey;
extern const rtgui_color_t light_grey;

/*
 * RTGUI default color format
 * BBBB BBBB GGGG GGGG RRRR RRRR
 */

/* convert rtgui color to BBBBBGGGGGGRRRRR */
rt_inline rt_uint16_t rtgui_color_to_565(rtgui_color_t c)
{
	rt_uint16_t pixel;

	pixel = (rt_uint16_t)(((RTGUI_RGB_B(c)>> 3) << 11) | ((RTGUI_RGB_G(c) >> 2) << 5) | (RTGUI_RGB_R(c) >> 3));

	return pixel;
}

rt_inline rtgui_color_t rtgui_color_from_565(rt_uint16_t pixel)
{
	rt_uint16_t r, g, b;
	rtgui_color_t color;

	r = pixel & 0x1f;
	g = (pixel >> 5) & 0x3f;
	b = (pixel >> 11) & 0x1f;

	color = r * 8225 / 1024 + ((g * 4047 / 1024) << 8) + ((b * 8225 / 1024) << 16);

	return color;
}

/* convert rtgui color to RRRRRGGGGGGBBBBB */
rt_inline rt_uint16_t rtgui_color_to_565p(rtgui_color_t c)
{
	rt_uint16_t pixel;

	pixel = (rt_uint16_t)(((RTGUI_RGB_R(c) >> 3) << 11) | ((RTGUI_RGB_G(c) >> 2) << 5) | (RTGUI_RGB_B(c)>> 3));
	return pixel;
}

rt_inline rtgui_color_t rtgui_color_from_565p(rt_uint16_t pixel)
{
	rt_uint8_t r, g, b;
	rtgui_color_t color;

	r = (pixel >> 11) & 0x1f;
	g = (pixel >> 5)  & 0x3f;
	b = pixel & 0x1f;

	color = r * 8225 / 1024 + ((g * 4047 / 1024) << 8) + ((b * 8225 / 1024) << 16);

	return color;
}

#endif

