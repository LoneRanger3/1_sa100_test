/*
 * File      : widget.c
 * This file is part of RTGUI in RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2009, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-10-04     Bernard      first version
 */
#include <rtgui/widgets/widget.h>
#include <rtgui/widgets/window.h>
#include <rtgui/widgets/view.h>
#include <rtgui/widgets/list_view.h>

static void _rtgui_widget_constructor(rtgui_widget_t *widget)
{
   if (!widget) return;

	/* set default flag */
	widget->flag = RTGUI_WIDGET_FLAG_DEFAULT;

	/* init list */
	rtgui_list_init(&(widget->sibling));

	/* init gc */
	widget->gc.foreground = default_foreground;
	widget->gc.background = default_background;
	widget->gc.font = rtgui_font_default();
	widget->gc.textalign = RTGUI_ALIGN_LEFT | RTGUI_ALIGN_TOP;
	widget->align = RTGUI_ALIGN_LEFT | RTGUI_ALIGN_TOP;

	/* set parent and toplevel root */
	widget->parent			= RT_NULL;
	widget->toplevel		= RT_NULL;

	/* some common event handler */
#ifndef RTGUI_USING_SMALL_SIZE
	widget->on_draw 		= RT_NULL;
	widget->on_focus_in		= RT_NULL;
	widget->on_focus_out	= RT_NULL;
	widget->on_mouseclick 	= RT_NULL;
	widget->on_key 			= RT_NULL;
	widget->on_size 		= RT_NULL;
	widget->on_command 		= RT_NULL;
#endif

	/* set default event handler */
	widget->event_handler = rtgui_widget_event_handler;

	/* does not set widget extent and only set clip_sync to zero */
	rtgui_region_init(&(widget->clip));
#ifndef RTGUI_USING_SMALL_SIZE
	widget->clip_sync = 0;
#endif
}

/* Destroys the widget */
static void _rtgui_widget_destructor(rtgui_widget_t *widget)
{
	if (widget == RT_NULL) return;

	if (widget->parent != RT_NULL)
	{
		/* remove widget from parent's children list */
		rtgui_list_remove(&(RTGUI_CONTAINER(widget->parent)->children), &(widget->sibling));

		widget->parent = RT_NULL;
	}

	/* fini clip region */
	rtgui_region_fini(&(widget->clip));
#ifndef RTGUI_USING_SMALL_SIZE
	widget->clip_sync = 0;
#endif
}

rtgui_type_t *rtgui_widget_type_get(void)
{
	static rtgui_type_t *widget_type = RT_NULL;

	if (!widget_type)
	{
		widget_type = rtgui_type_create("rtgui_widget", RTGUI_OBJECT_TYPE,
			sizeof(rtgui_widget_t), RTGUI_CONSTRUCTOR(_rtgui_widget_constructor),
			RTGUI_DESTRUCTOR(_rtgui_widget_destructor));
	}

	return widget_type;
}

rtgui_widget_t *rtgui_widget_create(rtgui_type_t *widget_type)
{
	struct rtgui_widget* widget;

	widget = RTGUI_WIDGET(rtgui_object_create(widget_type));
	return widget;
}

void rtgui_widget_destroy(rtgui_widget_t* widget)
{
	rtgui_object_destroy(RTGUI_OBJECT(widget));
}

void rtgui_widget_set_rect(rtgui_widget_t* widget, rtgui_rect_t* rect)
{
	if (widget == RT_NULL || rect == RT_NULL) return;

	widget->extent = *rect;

#ifndef RTGUI_USING_SMALL_SIZE
	/* reset mini width and height */
	widget->mini_width  = rtgui_rect_width(widget->extent);
	widget->mini_height = rtgui_rect_height(widget->extent);
#endif

	/* it's not empty, fini it */
	if (rtgui_region_not_empty(&(widget->clip)))
	{
		rtgui_region_fini(&(widget->clip));
	}

	/* reset clip info */
	rtgui_region_init_with_extents(&(widget->clip), rect);
}

#ifndef RTGUI_USING_SMALL_SIZE
void rtgui_widget_set_miniwidth(rtgui_widget_t* widget, int width)
{
	RT_ASSERT(widget != RT_NULL);

	widget->mini_width = width;
}

void rtgui_widget_set_miniheight(rtgui_widget_t* widget, int height)
{
	RT_ASSERT(widget != RT_NULL);

	widget->mini_height = height;
}
#endif

/*
 * This function moves widget and its children to a logic point
 */
void rtgui_widget_move_to_logic(rtgui_widget_t* widget, int dx, int dy)
{
	struct rtgui_list_node* node;
	rtgui_widget_t* child;

	if (widget == RT_NULL) return;

	rtgui_rect_moveto(&(widget->extent), dx, dy);

	/* move each child */
	if (RTGUI_IS_CONTAINER(widget))
	{
		rtgui_list_foreach(node, &(RTGUI_CONTAINER(widget)->children))
		{
			child = rtgui_list_entry(node, rtgui_widget_t, sibling);

			rtgui_widget_move_to_logic(child, dx, dy);
		}
	}
}

void rtgui_widget_set_event_handler(rtgui_widget_t* widget, rtgui_event_handler_ptr handler)
{
	RT_ASSERT(widget != RT_NULL);

	widget->event_handler = handler;
}
RTM_EXPORT(rtgui_widget_set_event_handler);

void rtgui_widget_get_rect(rtgui_widget_t* widget, rtgui_rect_t *rect)
{
	RT_ASSERT(widget != RT_NULL);

	if (rect != RT_NULL)
	{
		rect->x1 = rect->y1 = 0;
		rect->x2 = widget->extent.x2 - widget->extent.x1;
		rect->y2 = widget->extent.y2 - widget->extent.y1;
	}
}
RTM_EXPORT(rtgui_widget_get_rect);

void rtgui_widget_set_onfocus(rtgui_widget_t* widget, rtgui_event_handler_ptr handler)
{
	RT_ASSERT(widget != RT_NULL);

	widget->on_focus_in = handler;
}

void rtgui_widget_set_onunfocus(rtgui_widget_t* widget, rtgui_event_handler_ptr handler)
{
	RT_ASSERT(widget != RT_NULL);

	widget->on_focus_out = handler;
}

#ifndef RTGUI_USING_SMALL_SIZE
void rtgui_widget_set_ondraw(rtgui_widget_t* widget, rtgui_event_handler_ptr handler)
{
	RT_ASSERT(widget != RT_NULL);

	widget->on_draw = handler;
}

void rtgui_widget_set_onmouseclick(rtgui_widget_t* widget, rtgui_event_handler_ptr handler)
{
	RT_ASSERT(widget != RT_NULL);

	widget->on_mouseclick = handler;
}

void rtgui_widget_set_onkey(rtgui_widget_t* widget, rtgui_event_handler_ptr handler)
{
	RT_ASSERT(widget != RT_NULL);

	widget->on_key = handler;
}

void rtgui_widget_set_onsize(rtgui_widget_t* widget, rtgui_event_handler_ptr handler)
{
	RT_ASSERT(widget != RT_NULL);

	widget->on_size = handler;
}

void rtgui_widget_set_oncommand(rtgui_widget_t* widget, rtgui_event_handler_ptr handler)
{
	RT_ASSERT(widget != RT_NULL);

	widget->on_command = handler;
}
#endif

/**
 * @brief Focuses the widget. The focused widget is the widget which can receive the keyboard events
 * @param widget a widget
 * @note The widget has to be attached to a toplevel widget, otherwise it will have no effect
 */
void rtgui_widget_focus(rtgui_widget_t *widget)
{
	rtgui_widget_t *focused;
	rtgui_container_t *parent;

	RT_ASSERT(widget != RT_NULL);

	if (!widget->parent || !RTGUI_WIDGET_IS_FOCUSABLE(widget) || !RTGUI_WIDGET_IS_ENABLE(widget))
		return;

	/* set widget as focused */
	widget->flag |= RTGUI_WIDGET_FLAG_FOCUS;

	/* get parent container */
	parent = RTGUI_CONTAINER(widget->parent);

	/* get old focused widget */
	focused = parent->focused;
	if (focused == widget) return ; /* it's the same focused widget */

	if (focused != RT_NULL)
		rtgui_widget_unfocus(focused);

	/* set widget as focused widget in parent link */
	parent->focused = widget;
	while (RTGUI_WIDGET(parent)->parent != RT_NULL)
	{
		parent = RTGUI_CONTAINER(RTGUI_WIDGET(parent)->parent);
		parent->focused = widget;

		/* if the parent is hide, break it */
		if (RTGUI_WIDGET_IS_HIDE(RTGUI_WIDGET(parent))) break;
	}

#ifndef RTGUI_USING_SMALL_SIZE
	/* invoke on focus in call back */
	if (widget->on_focus_in != RT_NULL)
   		widget->on_focus_in(widget, RT_NULL);
#endif
}
RTM_EXPORT(rtgui_widget_focus);

/**
 * @brief Unfocused the widget
 * @param widget a widget
 */
void rtgui_widget_unfocus(rtgui_widget_t *widget)
{
	RT_ASSERT(widget != RT_NULL);

	widget->flag &= ~RTGUI_WIDGET_FLAG_FOCUS;

	if (!widget->toplevel || !RTGUI_WIDGET_IS_FOCUS(widget))
		return;

#ifndef RTGUI_USING_SMALL_SIZE
	if (widget->on_focus_out != RT_NULL)
   		widget->on_focus_out(widget, RT_NULL);
#endif
}

void rtgui_widget_point_to_device(rtgui_widget_t* widget, rtgui_point_t* point)
{
	RT_ASSERT(widget != RT_NULL);

	if (point != RT_NULL)
	{
		point->x += widget->extent.x1;
		point->y += widget->extent.y1;
	}
}

void rtgui_widget_rect_to_device(rtgui_widget_t* widget, rtgui_rect_t* rect)
{
	RT_ASSERT(widget != RT_NULL);

	if (rect != RT_NULL)
	{
		rect->x1 += widget->extent.x1;
		rect->x2 += widget->extent.x1;

		rect->y1 += widget->extent.y1;
		rect->y2 += widget->extent.y1;
	}
}

void rtgui_widget_point_to_logic(rtgui_widget_t* widget, rtgui_point_t* point)
{
	RT_ASSERT(widget != RT_NULL);

	if (point != RT_NULL)
	{
		point->x -= widget->extent.x1;
		point->y -= widget->extent.y1;
	}
}

void rtgui_widget_rect_to_logic(rtgui_widget_t* widget, rtgui_rect_t* rect)
{
	RT_ASSERT(widget != RT_NULL);

	if (rect != RT_NULL)
	{
		rect->x1 -= widget->extent.x1;
		rect->x2 -= widget->extent.x1;

		rect->y1 -= widget->extent.y1;
		rect->y2 -= widget->extent.y1;
	}
}

rtgui_widget_t* rtgui_widget_get_toplevel(rtgui_widget_t* widget)
{
	rtgui_widget_t* r;

	RT_ASSERT(widget != RT_NULL);

	if (widget->toplevel) return widget->toplevel;

	r = widget;
	/* get the toplevel widget */
	while (r->parent != RT_NULL) r = r->parent;

	/* set toplevel */
	widget->toplevel = r;

	return r;
}

rt_bool_t rtgui_widget_event_handler(rtgui_widget_t* widget, rtgui_event_t* event)
{
#ifndef RTGUI_USING_SMALL_SIZE
	switch (event->type)
	{
	case RTGUI_EVENT_PAINT:
		if (widget->on_draw != RT_NULL) return widget->on_draw(widget, event);
		break;

	case RTGUI_EVENT_KBD:
		if (widget->on_key != RT_NULL) return widget->on_key(widget, event);
		break;

	case RTGUI_EVENT_MOUSE_BUTTON:
		if (widget->on_mouseclick != RT_NULL) return widget->on_mouseclick(widget, event);
		break;

	case RTGUI_EVENT_COMMAND:
		if (widget->on_command != RT_NULL) return widget->on_command(widget, event);
		break;

	case RTGUI_EVENT_RESIZE:
		if (widget->on_size != RT_NULL) return widget->on_size(widget, event);
		break;
	}
#endif

	return RT_FALSE;
}

/*
 * This function updates the clip info of widget
 */
void rtgui_widget_update_clip(rtgui_widget_t* widget)
{
	struct rtgui_list_node* node;
	rtgui_widget_t *parent;

	/* no widget or widget is hide, no update clip */
	if (widget == RT_NULL || RTGUI_WIDGET_IS_HIDE(widget)) return;

	parent = widget->parent;
	/* if there is no parent, do not update clip (please use toplevel widget API) */
	if (parent == RT_NULL) return;

#ifndef RTGUI_USING_SMALL_SIZE
	/* increase clip sync */
	widget->clip_sync ++;
#endif

	/* reset clip to extent */
	rtgui_region_reset(&(widget->clip), &(widget->extent));

	/* limit widget extent in parent extent */
	rtgui_region_intersect(&(widget->clip), &(widget->clip), &(parent->clip));

	/* get the no transparent parent */
	while (parent != RT_NULL && parent->flag & RTGUI_WIDGET_FLAG_TRANSPARENT)
	{
		parent = parent->parent;
	}

	if (parent != RT_NULL)
	{
		/* subtract widget clip in parent clip */
		if (!(widget->flag & RTGUI_WIDGET_FLAG_TRANSPARENT))
		{
			rtgui_region_subtract_rect(&(parent->clip), &(parent->clip),
				&(widget->extent));
		}
	}

	/*
	 * note: since the layout widget introduction, the sibling widget will not
	 * intersect.
	 */

	/* if it's a container object, update the clip info of children */
	if (RTGUI_IS_CONTAINER(widget))
	{
		rtgui_widget_t* child;
		rtgui_list_foreach(node, &(RTGUI_CONTAINER(widget)->children))
		{
			child = rtgui_list_entry(node, rtgui_widget_t, sibling);

			rtgui_widget_update_clip(child);
		}
	}
}

void rtgui_widget_show(rtgui_widget_t* widget)
{
	/* there is no parent or the parent is hide, no show at all */
	if (widget->parent == RT_NULL ||
		RTGUI_WIDGET_IS_HIDE(widget->parent)) return;

	/* update the clip info of widget */
	RTGUI_WIDGET_UNHIDE(widget);
	rtgui_widget_update_clip(widget);
}

void rtgui_widget_hide(rtgui_widget_t* widget)
{
	/* hide this widget */
	RTGUI_WIDGET_HIDE(widget);

	/* update the clip info of widget parent */
	rtgui_widget_update_clip(widget->parent);
}

void rtgui_widget_update(rtgui_widget_t* widget)
{
	struct rtgui_event_paint paint;
	RTGUI_EVENT_PAINT_INIT(&paint);
	paint.wid = RT_NULL;

	RT_ASSERT(widget != RT_NULL);

	if (widget->event_handler != RT_NULL)
	{
		widget->event_handler(widget, &paint.parent);
	}
}

rtgui_widget_t* rtgui_widget_get_next_sibling(rtgui_widget_t* widget)
{
	rtgui_widget_t* sibling = RT_NULL;

	if (widget->sibling.next != RT_NULL)
	{
		sibling = rtgui_list_entry(widget->sibling.next, rtgui_widget_t, sibling);
	}

	return sibling;
}

rtgui_widget_t* rtgui_widget_get_prev_sibling(rtgui_widget_t* widget)
{
	struct rtgui_list_node* node;
	rtgui_widget_t *sibling, *parent;

	node = RT_NULL; sibling = RT_NULL;
	parent = widget->parent;
	if (parent != RT_NULL)
	{
		rtgui_list_foreach(node, &(RTGUI_CONTAINER(parent)->children))
		{
			if (node->next == &(widget->sibling))
				break;
		}
	}

	if (node != RT_NULL)
		sibling = rtgui_list_entry(node, rtgui_widget_t, sibling);

	return sibling;
}
