/*
 * File      : window.c
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
#include <rtgui/dc.h>
#include <rtgui/color.h>
#include <rtgui/image.h>
#include <rtgui/rtgui_system.h>

#include <rtgui/widgets/window.h>
#include <rtgui/widgets/button.h>
#include <rtgui/widgets/workbench.h>

static void _rtgui_win_constructor(rtgui_win_t *win)
{
	/* init window attribute */
	win->on_activate	= RT_NULL;
	win->on_deactivate	= RT_NULL;
	win->on_close		= RT_NULL;
	win->title			= RT_NULL;

	/* set window hide */
	RTGUI_WIDGET_HIDE(RTGUI_WIDGET(win));

	/* set window style */
	win->style = RTGUI_WIN_STYLE_DEFAULT;
	rtgui_widget_set_event_handler(RTGUI_WIDGET(win), rtgui_win_event_handler);

	/* init user data */
	win->user_data = 0;
}

static void _rtgui_win_destructor(rtgui_win_t* win)
{
	struct rtgui_event_win_destroy edestroy;

	if (RTGUI_TOPLEVEL(win)->server != RT_NULL)
	{
		/* destroy in server */
		RTGUI_EVENT_WIN_DESTROY_INIT(&edestroy);
		edestroy.wid = win;
		if (rtgui_thread_send_sync(RTGUI_TOPLEVEL(win)->server, RTGUI_EVENT(&edestroy),
			sizeof(struct rtgui_event_win_destroy)) != RT_EOK)
		{
			/* destroy in server failed */
			return;
		}
	}

	/* release field */
	rt_free(win->title);
}

static rt_bool_t _rtgui_win_create_in_server(rtgui_win_t* win)
{
	if (RTGUI_TOPLEVEL(win)->server == RT_NULL)
	{
		rt_thread_t server;
		struct rtgui_event_win_create ecreate;
		RTGUI_EVENT_WIN_CREATE_INIT(&ecreate);

		/* get server thread id */
		server = rtgui_thread_get_server();
		if (server == RT_NULL)
		{
			rt_kprintf("RTGUI server is not running...\n");
			return RT_FALSE;
		}

		/* send win create event to server */
		ecreate.wid 	= win;
		ecreate.extent 	= RTGUI_WIDGET(win)->extent;
		ecreate.flag 	= win->style;
		ecreate.mask 	= 0;
		rt_strncpy((char*)ecreate.title, (char*)win->title, RTGUI_NAME_MAX);

		if (rtgui_thread_send_sync(server, RTGUI_EVENT(&ecreate),
			sizeof(struct rtgui_event_win_create)) != RT_EOK)
		{
			rt_kprintf("create win: %s failed\n", win->title);
			return RT_FALSE;
		}

		/* set server */
		RTGUI_TOPLEVEL(win)->server = server;
	}

	return RT_TRUE;
}

rtgui_type_t *rtgui_win_type_get(void)
{
	static rtgui_type_t *win_type = RT_NULL;

	if (!win_type)
	{
		win_type = rtgui_type_create("win", RTGUI_TOPLEVEL_TYPE,
			sizeof(rtgui_win_t),
			RTGUI_CONSTRUCTOR(_rtgui_win_constructor),
			RTGUI_DESTRUCTOR(_rtgui_win_destructor));
	}

	return win_type;
}

rtgui_win_t* rtgui_win_create(rtgui_toplevel_t* parent_toplevel, const char* title, rtgui_rect_t *rect, rt_uint32_t style)
{
	struct rtgui_win* win;

	/* allocate win memory */
	win = (struct rtgui_win*) rtgui_widget_create (RTGUI_WIN_TYPE);
	if (win != RT_NULL)
	{
		/* set parent toplevel */
		win->parent_toplevel = parent_toplevel;

		/* set title, rect and style */
		if (title != RT_NULL)
			win->title = rt_strdup(title);
		else
			win->title = RT_NULL;

		rtgui_widget_set_rect(RTGUI_WIDGET(win), rect);
		win->style = style;

		if (_rtgui_win_create_in_server(win) == RT_FALSE)
		{
			rtgui_widget_destroy(RTGUI_WIDGET(win));
			return RT_NULL;
		}
	}

	return win;
}

void rtgui_win_destroy(struct rtgui_win* win)
{
	if (win->parent_toplevel != RT_NULL)
	{
		if (win->style & RTGUI_WIN_STYLE_MODAL)
		{
			/* exit modal mode */
			win->style &= ~RTGUI_WIN_STYLE_MODAL;
			/* set style to closed */
			win->style |= RTGUI_WIN_STYLE_CLOSED;
		}
		else rtgui_widget_destroy(RTGUI_WIDGET(win));

	}
	else if (win->style & RTGUI_WIN_STYLE_CLOSED)
	{
		rtgui_widget_destroy(RTGUI_WIDGET(win));
	}
	else
	{
		/* exit modal mode */
		win->style &= ~RTGUI_WIN_STYLE_MODAL;
		/* set style to closed */
		win->style |= RTGUI_WIN_STYLE_CLOSED;
	}
}

rtgui_modal_code_t rtgui_win_show(struct rtgui_win* win, rt_bool_t is_modal)
{
	rtgui_modal_code_t result;

	RT_ASSERT(win != RT_NULL);
	result = RTGUI_MODAL_CANCEL;

	/* if it does not register into server, create it in server */
	if (RTGUI_TOPLEVEL(win)->server == RT_NULL)
	{
		if (_rtgui_win_create_in_server(win) == RT_FALSE)
			return result;
	}

	if (RTGUI_WIDGET_IS_HIDE(RTGUI_WIDGET(win)))
	{
		/* send show message to server */
		struct rtgui_event_win_show eshow;
		RTGUI_EVENT_WIN_SHOW_INIT(&eshow);
		eshow.wid = win;

		if (rtgui_thread_send_sync(RTGUI_TOPLEVEL(win)->server, RTGUI_EVENT(&eshow),
			sizeof(struct rtgui_event_win_show)) != RT_EOK)
		{
			/* hide window failed */
			return result;
		}

		/* set window unhidden */
		RTGUI_WIDGET_UNHIDE(RTGUI_WIDGET(win));
	}
	else {
		rtgui_widget_update(RTGUI_WIDGET(win));
	}

	if (is_modal == RT_TRUE)
	{
		if (win->parent_toplevel != RT_NULL)
		{
			rtgui_workbench_t* workbench;

			/* get root toplevel */
			workbench = RTGUI_WORKBENCH(win->parent_toplevel);
			workbench->flag |= RTGUI_WORKBENCH_FLAG_MODAL_MODE;

			rtgui_workbench_event_loop(workbench);
			result = workbench->modal_code;
			workbench->flag &= ~RTGUI_WORKBENCH_FLAG_MODAL_MODE;
		}
		else
		{
			/* which is a root window */
			win->style |= RTGUI_WIN_STYLE_MODAL;
			rtgui_win_event_loop(win);

			result = win->modal_code;
			win->style &= ~RTGUI_WIN_STYLE_MODAL;
		}
	}

	return result;
}

void rtgui_win_end_modal(struct rtgui_win* win, rtgui_modal_code_t modal_code)
{
	if (win->parent_toplevel != RT_NULL)
	{
		rtgui_workbench_t* workbench;

		/* which is shown under workbench */
		workbench = RTGUI_WORKBENCH(win->parent_toplevel);
		workbench->modal_code = modal_code;
		workbench->flag &= ~RTGUI_WORKBENCH_FLAG_MODAL_MODE;
	}
	else
	{
		/* which is a stand alone window */
		win->modal_code = modal_code;
		win->style &= ~RTGUI_WIN_STYLE_MODAL;
	}
}

void rtgui_win_hiden(struct rtgui_win* win)
{
	RT_ASSERT(win != RT_NULL);

	if (!RTGUI_WIDGET_IS_HIDE(RTGUI_WIDGET(win)) &&
		RTGUI_TOPLEVEL(win)->server != RT_NULL)
	{
		/* send hidden message to server */
		struct rtgui_event_win_hide ehide;
		RTGUI_EVENT_WIN_HIDE_INIT(&ehide);
		ehide.wid = win;

		if (rtgui_thread_send_sync(RTGUI_TOPLEVEL(win)->server, RTGUI_EVENT(&ehide),
			sizeof(struct rtgui_event_win_hide)) != RT_EOK)
		{
			rt_kprintf("hide win: %s failed\n", win->title);
			return;
		}

		/* set window hide and deactivated */
		RTGUI_WIDGET_HIDE(RTGUI_WIDGET(win));
		win->style &= ~RTGUI_WIN_STYLE_ACTIVATE;
	}
}

rt_bool_t rtgui_win_is_activated(struct rtgui_win* win)
{
	RT_ASSERT(win != RT_NULL);

	return win->style & RTGUI_WIN_STYLE_ACTIVATE;
}

void rtgui_win_move(struct rtgui_win* win, int x, int y)
{
	struct rtgui_event_win_move emove;
	RTGUI_EVENT_WIN_MOVE_INIT(&emove);

	if (win == RT_NULL) return;

	if (RTGUI_TOPLEVEL(win)->server != RT_NULL)
	{
		/* set win hide firstly */
		RTGUI_WIDGET_HIDE(RTGUI_WIDGET(win));

		emove.wid 	= win;
		emove.x		= x;
		emove.y		= y;
		if (rtgui_thread_send_sync(RTGUI_TOPLEVEL(win)->server, RTGUI_EVENT(&emove),
			sizeof(struct rtgui_event_win_move)) != RT_EOK)
		{
			return;
		}
	}

	/* move window to logic position */
	rtgui_widget_move_to_logic(RTGUI_WIDGET(win),
		x - RTGUI_WIDGET(win)->extent.x1,
		y - RTGUI_WIDGET(win)->extent.y1);

	/* set window visible */
	RTGUI_WIDGET_UNHIDE(RTGUI_WIDGET(win));
	return;
}

static rt_bool_t rtgui_win_ondraw(struct rtgui_win* win)
{
	struct rtgui_dc* dc;
	struct rtgui_rect rect;
	struct rtgui_event_paint event;

	/* begin drawing */
	dc = rtgui_dc_begin_drawing(RTGUI_WIDGET(win));
	if (dc == RT_NULL) return RT_FALSE;

	/* get window rect */
	rtgui_widget_get_rect(RTGUI_WIDGET(win), &rect);
	/* fill area */
	rtgui_dc_fill_rect(dc, &rect);

	/* paint each widget */
	RTGUI_EVENT_PAINT_INIT(&event);
	event.wid = RT_NULL;
	rtgui_container_dispatch_event(RTGUI_CONTAINER(win), (rtgui_event_t*)&event);

	rtgui_dc_end_drawing(dc);
	return RT_FALSE;
}

rt_bool_t rtgui_win_event_handler(struct rtgui_widget* widget, struct rtgui_event* event)
{
	struct rtgui_win* win = (struct rtgui_win*)widget;

	RT_ASSERT((win != RT_NULL) && (event != RT_NULL));

	switch (event->type)
	{
	case RTGUI_EVENT_WIN_SHOW:
		rtgui_win_show(win, RT_FALSE);
		break;

	case RTGUI_EVENT_WIN_HIDE:
		rtgui_win_hiden(win);
		break;

	case RTGUI_EVENT_WIN_CLOSE:
		if (win->on_close != RT_NULL)
		{
			if (win->on_close(widget, event) == RT_FALSE) return RT_TRUE;
		}

		/* destroy window */
		rtgui_win_destroy(win);

		/* exit event loop */
		return RT_TRUE;

	case RTGUI_EVENT_WIN_MOVE:
	{
		struct rtgui_event_win_move* emove = (struct rtgui_event_win_move*)event;

		/* move window */
		rtgui_win_move(win, emove->x, emove->y);
	}
	break;

	case RTGUI_EVENT_WIN_ACTIVATE:
		if (RTGUI_WIDGET_IS_HIDE(RTGUI_WIDGET(win)))
		{
			rt_kprintf("activate window, but window is hide!\n");
		}

		win->style |= RTGUI_WIN_STYLE_ACTIVATE;
#ifndef RTGUI_USING_SMALL_SIZE
		if (widget->on_draw != RT_NULL) widget->on_draw(widget, event);
		else 
#endif
		rtgui_win_ondraw(win);

		if (win->on_activate != RT_NULL)
		{
			win->on_activate(widget, event);
		}
		break;

	case RTGUI_EVENT_WIN_DEACTIVATE:
		win->style &= ~RTGUI_WIN_STYLE_ACTIVATE;
#ifndef RTGUI_USING_SMALL_SIZE
		if (widget->on_draw != RT_NULL) widget->on_draw(widget, event);
		else 
#endif
			rtgui_win_ondraw(win);

		if (win->on_deactivate != RT_NULL)
		{
			win->on_deactivate(widget, event);
		}
		break;

	case RTGUI_EVENT_PAINT:
#ifndef RTGUI_USING_SMALL_SIZE
		if (widget->on_draw != RT_NULL) widget->on_draw(widget, event);
		else
#endif
			rtgui_win_ondraw(win);
		break;

	case RTGUI_EVENT_MOUSE_BUTTON:
		if (rtgui_container_dispatch_mouse_event(RTGUI_CONTAINER(win), (struct rtgui_event_mouse*)event) == RT_FALSE)
		{
#ifndef RTGUI_USING_SMALL_SIZE
			if (widget->on_mouseclick != RT_NULL)
			{
				return widget->on_mouseclick(widget, event);
			}
#endif
		}
		break;

	case RTGUI_EVENT_MOUSE_MOTION:
#if 0
		if (rtgui_widget_dispatch_mouse_event(widget,
			(struct rtgui_event_mouse*)event) == RT_FALSE)
		{
#ifndef RTGUI_USING_SMALL_SIZE
			/* handle event in current widget */
			if (widget->on_mousemotion != RT_NULL)
			{
				return widget->on_mousemotion(widget, event);
			}
#endif
		}
		else return RT_TRUE;
#endif
		break;

    case RTGUI_EVENT_KBD:
		if (RTGUI_CONTAINER(win)->focused != widget)
		{
			RTGUI_CONTAINER(win)->focused->event_handler(RTGUI_CONTAINER(win)->focused, event);
		}
        break;

	default:
		/* call parent event handler */
		return rtgui_toplevel_event_handler(widget, event);
	}

	return RT_FALSE;
}

/* windows event loop */
void rtgui_win_event_loop(rtgui_win_t* wnd)
{
	/* the buffer uses to receive event */
	char event_buf[256];

	struct rtgui_event* event = (struct rtgui_event*)&event_buf[0];

	if (wnd->style & RTGUI_WIN_STYLE_MODAL)
	{
		while (wnd->style & RTGUI_WIN_STYLE_MODAL)
		{
			if (rtgui_thread_recv(event, sizeof(event_buf)) == RT_EOK)
			{
				/* perform event handler */
				RTGUI_WIDGET(wnd)->event_handler(RTGUI_WIDGET(wnd), event);
			}
		}
	}
	else
	{
		while (!(wnd->style & RTGUI_WIN_STYLE_CLOSED))
		{
			if (rtgui_thread_recv(event, sizeof(event_buf)) == RT_EOK)
			{
				/* perform event handler */
				RTGUI_WIDGET(wnd)->event_handler(RTGUI_WIDGET(wnd), event);
			}
		}
	}

	/* destroy window */
	rtgui_widget_destroy(RTGUI_WIDGET(wnd));
}

void rtgui_win_set_rect(rtgui_win_t* win, rtgui_rect_t* rect)
{
	struct rtgui_event_win_resize event;

	if (win == RT_NULL || rect == RT_NULL) return;

	RTGUI_WIDGET(win)->extent = *rect;

	if (RTGUI_TOPLEVEL(win)->server != RT_NULL)
	{
		/* set window resize event to server */
		RTGUI_EVENT_WIN_RESIZE_INIT(&event);
		event.wid = win;
		event.rect = *rect;

		rtgui_thread_send(RTGUI_TOPLEVEL(win)->server, &(event.parent), sizeof(struct rtgui_event_win_resize));
	}
}

void rtgui_win_set_box(rtgui_win_t* win, rtgui_box_t* box)
{
	if (win == RT_NULL || box == RT_NULL) return;

	rtgui_container_add_child(RTGUI_CONTAINER(win), RTGUI_WIDGET(box));
	rtgui_widget_set_rect(RTGUI_WIDGET(box), &(RTGUI_WIDGET(win)->extent));
}

void rtgui_win_set_onactivate(rtgui_win_t* win, rtgui_event_handler_ptr handler)
{
	if (win != RT_NULL)
	{
		win->on_activate = handler;
	}
}

void rtgui_win_set_ondeactivate(rtgui_win_t* win, rtgui_event_handler_ptr handler)
{
	if (win != RT_NULL)
	{
		win->on_deactivate = handler;
	}
}

void rtgui_win_set_onclose(rtgui_win_t* win, rtgui_event_handler_ptr handler)
{
	if (win != RT_NULL)
	{
		win->on_close = handler;
	}
}

void rtgui_win_set_title(rtgui_win_t* win, const char *title)
{
	/* send title to server */
	if (RTGUI_TOPLEVEL(win)->server != RT_NULL)
	{
	}

	/* modify in local side */
	if (win->title != RT_NULL)
	{
		rtgui_free(win->title);
		win->title = RT_NULL;
	}

	if (title != RT_NULL)
	{
		win->title = rt_strdup(title);
	}
}

char* rtgui_win_get_title(rtgui_win_t* win)
{
	RT_ASSERT(win != RT_NULL);

	return win->title;
}
