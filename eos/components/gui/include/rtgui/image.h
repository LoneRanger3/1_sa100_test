/*
 * File      : image.h
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
#ifndef __RTGUI_IMAGE_H__
#define __RTGUI_IMAGE_H__

#include <rtgui/dc.h>
#include <rtgui/filerw.h>
#include <rtgui/region.h>

struct rtgui_image;
struct rtgui_image_engine
{
	const char* name;
	struct rtgui_list_node list;

	/* image engine function */
	rt_bool_t (*image_check)(struct rtgui_filerw* file);

	rt_bool_t (*image_load)(struct rtgui_image* image, struct rtgui_filerw* file, rt_bool_t load);
	void (*image_unload)(struct rtgui_image* image);

	void (*image_blit)(struct rtgui_image* image, struct rtgui_dc* dc, struct rtgui_rect* rect);
};

struct rtgui_image
{
	/* image metrics */
	rt_uint16_t w, h;

	/* image engine */
	struct rtgui_image_engine* engine;

	/* image private data */
	void* data;
};
typedef struct rtgui_image rtgui_image_t;

/* init rtgui image system */
void rtgui_system_image_init(void);

struct rtgui_image* rtgui_image_create_from_file(const char* type, const char* filename, rt_bool_t load);
struct rtgui_image* rtgui_image_create_from_mem(const char* type, const rt_uint8_t* data, rt_size_t length, rt_bool_t load);
void rtgui_image_destroy(struct rtgui_image* image);

/* register an image engine */
void rtgui_image_register_engine(struct rtgui_image_engine* engine);

/* blit an image on DC */
void rtgui_image_blit(struct rtgui_image* image, struct rtgui_dc* dc, struct rtgui_rect* rect);


#endif
