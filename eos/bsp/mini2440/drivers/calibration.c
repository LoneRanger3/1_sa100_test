#include <rtgui/rtgui.h>
#include <rtgui/dc.h>
#include <rtgui/rtgui_system.h>
#include <rtgui/widgets/window.h>
//#include <rtgui/rtgui_application.h>

#include "touch.h"

/*
 * temporary, fix the bug that the upper left calibration pointer
 * can not be out, lomboswer
 */
#define TEMP_FOR_SHOW

#define CALIBRATION_STEP_LEFTTOP		0
#define CALIBRATION_STEP_RIGHTTOP		1
#define CALIBRATION_STEP_RIGHTBOTTOM		2
#define CALIBRATION_STEP_LEFTBOTTOM		3
#define CALIBRATION_STEP_CENTER			4

#define TOUCH_WIN_UPDATE			1
#define TOUCH_WIN_CLOSE				2

#define CALIBRATION_WIDTH			15
#define CALIBRATION_HEIGHT			15

struct calibration_session
{
	rt_uint8_t step;

	struct calibration_data data;

	rt_uint16_t width;
	rt_uint16_t height;

	rt_device_t device;
	rt_thread_t tid;
	struct rtgui_win *wid;
};
static struct calibration_session *calibration_ptr = RT_NULL;

rt_mq_t calibration_mq; /* lomboswer */

static void calibration_data_post(rt_uint16_t x, rt_uint16_t y)
{
	if (calibration_ptr != RT_NULL)
	{
		switch (calibration_ptr->step)
		{
		case CALIBRATION_STEP_LEFTTOP:
			calibration_ptr->data.min_x = x;
			calibration_ptr->data.min_y = y;
			break;

		case CALIBRATION_STEP_RIGHTTOP:
			calibration_ptr->data.max_x = x;
			calibration_ptr->data.min_y = (calibration_ptr->data.min_y + y)/2;
			break;

		case CALIBRATION_STEP_LEFTBOTTOM:
			calibration_ptr->data.min_x = (calibration_ptr->data.min_x + x)/2;
			calibration_ptr->data.max_y = y;
			break;

		case CALIBRATION_STEP_RIGHTBOTTOM:
			calibration_ptr->data.max_x = (calibration_ptr->data.max_x + x)/2;
			calibration_ptr->data.max_y = (calibration_ptr->data.max_y + y)/2;
			break;

		case CALIBRATION_STEP_CENTER:
			/* calibration done */
			{
				rt_uint16_t w, h;

				struct rtgui_event_command ecmd;
				RTGUI_EVENT_COMMAND_INIT(&ecmd);
				ecmd.command_id = TOUCH_WIN_CLOSE;

				/* calculate calibrated data */
				if (calibration_ptr->data.max_x > calibration_ptr->data.min_x)
					w = calibration_ptr->data.max_x - calibration_ptr->data.min_x;
				else
					w = calibration_ptr->data.min_x - calibration_ptr->data.max_x;
				w = (w/(calibration_ptr->width - 2 * CALIBRATION_WIDTH)) * CALIBRATION_WIDTH;

				if (calibration_ptr->data.max_y > calibration_ptr->data.min_y)
					h = calibration_ptr->data.max_y - calibration_ptr->data.min_y;
				else
					h = calibration_ptr->data.min_y - calibration_ptr->data.max_y;
				h = (h/(calibration_ptr->height - 2 * CALIBRATION_HEIGHT)) * CALIBRATION_HEIGHT;

				if (calibration_ptr->data.max_x > calibration_ptr->data.min_x)
				{
					calibration_ptr->data.min_x -= w;
					calibration_ptr->data.max_x += w;
				}
				else
				{
					calibration_ptr->data.min_x += w;
					calibration_ptr->data.max_x -= w;
				}

				if (calibration_ptr->data.max_y > calibration_ptr->data.min_y)
				{
					calibration_ptr->data.min_y -= h;
					calibration_ptr->data.max_y += h;
				}
				else
				{
					calibration_ptr->data.min_y += h;
					calibration_ptr->data.max_y -= h;
				}

				rt_kprintf("calibration data: (%d, %d), (%d, %d)\n",
					calibration_ptr->data.min_x,
					calibration_ptr->data.max_x,
					calibration_ptr->data.min_y,
					calibration_ptr->data.max_y);
				/* rtgui_application_send(calibration_ptr->tid, &ecmd.parent, sizeof(struct rtgui_event_command)); */
				rtgui_thread_send(calibration_ptr->tid, &ecmd.parent, sizeof(struct rtgui_event_command));
			}
			return;
		}

		calibration_ptr->step ++;

		/* post command event */
		{
			struct rtgui_event_command ecmd;
			RTGUI_EVENT_COMMAND_INIT(&ecmd);
			ecmd.command_id = TOUCH_WIN_UPDATE;
			/* ecmd.wid = calibration_ptr->wid; */

			/* rtgui_application_send(calibration_ptr->tid, &ecmd.parent, sizeof(struct rtgui_event_command)); */
			rtgui_thread_send(calibration_ptr->tid, &ecmd.parent, sizeof(struct rtgui_event_command));
		}
	}
}

#if 1 /* lomboswer, let calibaration window's rtgui_win_event_loop exit */
rt_bool_t my_rtgui_win_close(struct rtgui_win* win)
{
	rtgui_win_hiden(win);
	rtgui_win_end_modal(win, RTGUI_MODAL_CANCEL);
}
#endif

rt_bool_t calibration_event_handler(struct rtgui_widget* pwidget, struct rtgui_event* event)
{
	struct rtgui_widget *widget;
	widget = pwidget;

	switch (event->type)
	{
	case RTGUI_EVENT_PAINT:
	{
		struct rtgui_dc *dc;
		struct rtgui_rect rect;

		dc = rtgui_dc_begin_drawing(widget);
		if (dc == RT_NULL) {
			break;
		}

		/* get rect information */
		rtgui_widget_get_rect(widget, &rect);

		/* clear whole window */
		RTGUI_WIDGET_BACKGROUND(widget) = white;
		rtgui_dc_fill_rect(dc, &rect);

		/* reset color */
		RTGUI_WIDGET_BACKGROUND(widget) = green;
		RTGUI_WIDGET_FOREGROUND(widget) = black;

		switch (calibration_ptr->step)
		{
			case CALIBRATION_STEP_LEFTTOP:
				rtgui_dc_draw_hline(dc, 0, 2 * CALIBRATION_WIDTH, CALIBRATION_HEIGHT);
				rtgui_dc_draw_vline(dc, CALIBRATION_WIDTH, 0, 2 * CALIBRATION_HEIGHT);
				RTGUI_WIDGET_FOREGROUND(widget) = red;
				rtgui_dc_fill_circle(dc, CALIBRATION_WIDTH, CALIBRATION_HEIGHT, 4);
				break;

			case CALIBRATION_STEP_RIGHTTOP:
				rtgui_dc_draw_hline(dc, calibration_ptr->width - 2 * CALIBRATION_WIDTH,
					calibration_ptr->width, CALIBRATION_HEIGHT);
				rtgui_dc_draw_vline(dc, calibration_ptr->width - CALIBRATION_WIDTH, 0, 2 * CALIBRATION_HEIGHT);
				RTGUI_WIDGET_FOREGROUND(widget) = red;
				rtgui_dc_fill_circle(dc, calibration_ptr->width - CALIBRATION_WIDTH, CALIBRATION_HEIGHT, 4);
				break;

			case CALIBRATION_STEP_LEFTBOTTOM:
				rtgui_dc_draw_hline(dc, 0, 2 * CALIBRATION_WIDTH, calibration_ptr->height - CALIBRATION_HEIGHT);
				rtgui_dc_draw_vline(dc, CALIBRATION_WIDTH, calibration_ptr->height - 2 * CALIBRATION_HEIGHT, calibration_ptr->height);
				RTGUI_WIDGET_FOREGROUND(widget) = red;
				rtgui_dc_fill_circle(dc, CALIBRATION_WIDTH, calibration_ptr->height - CALIBRATION_HEIGHT, 4);
				break;

			case CALIBRATION_STEP_RIGHTBOTTOM:
				rtgui_dc_draw_hline(dc, calibration_ptr->width - 2 * CALIBRATION_WIDTH,
					calibration_ptr->width, calibration_ptr->height - CALIBRATION_HEIGHT);
				rtgui_dc_draw_vline(dc, calibration_ptr->width - CALIBRATION_WIDTH, calibration_ptr->height - 2 * CALIBRATION_HEIGHT, calibration_ptr->height);
				RTGUI_WIDGET_FOREGROUND(widget) = red;
				rtgui_dc_fill_circle(dc, calibration_ptr->width - CALIBRATION_WIDTH, calibration_ptr->height - CALIBRATION_HEIGHT, 4);
				break;

			case CALIBRATION_STEP_CENTER:
				rtgui_dc_draw_hline(dc, calibration_ptr->width/2 - CALIBRATION_WIDTH, calibration_ptr->width/2 + CALIBRATION_WIDTH, calibration_ptr->height/2);
				rtgui_dc_draw_vline(dc, calibration_ptr->width/2, calibration_ptr->height/2 - CALIBRATION_HEIGHT, calibration_ptr->height/2 + CALIBRATION_HEIGHT);
				RTGUI_WIDGET_FOREGROUND(widget) = red;
				rtgui_dc_fill_circle(dc, calibration_ptr->width/2, calibration_ptr->height/2, 4);
				break;
		}
		rtgui_dc_end_drawing(dc);
	}
	break;

	case RTGUI_EVENT_COMMAND:
	{
			struct rtgui_event_command *ecmd = (struct rtgui_event_command *)event;

			switch (ecmd->command_id)
			{
			case TOUCH_WIN_UPDATE:
				rtgui_widget_update(widget);
				break;
			case TOUCH_WIN_CLOSE:
				/* lomboswer, let calibaration window's _event_loop exit */
				my_rtgui_win_close(RTGUI_WIN(widget));
				break;
			}
	}
		return RT_TRUE;

	default:
		/*
		 * remove the RTGUI_EVENT_WIN_ACTIVATE, RTGUI_EVENT_CLIP_INFO msg,
		 * or the lcd is initial green, lomboswer
		 */
#ifndef TEMP_FOR_SHOW
		/* rtgui_win_event_handler(RTGUI_OBJECT(widget), event); */
		rtgui_win_event_handler((struct rtgui_widget*)RTGUI_OBJECT(widget), event);
#endif
		break;
	}

	return RT_FALSE;
}

void calibration_entry(void *parameter)
{
	struct rtgui_win *win;
	struct rtgui_rect rect;

	rtgui_graphic_driver_get_rect(rtgui_graphic_driver_get_default(), &rect);

	/* set screen rect */
	calibration_ptr->width = rect.x2;
	calibration_ptr->height = rect.y2;

	/* create calibration window */
	win = rtgui_win_create(RT_NULL,
		"calibration", &rect, RTGUI_WIN_STYLE_NO_TITLE | RTGUI_WIN_STYLE_NO_BORDER);
	if (win == RT_NULL)
	{
		return;
	}

	calibration_ptr->wid = win;

	/* rtgui_object_set_event_handler(RTGUI_OBJECT(win), calibration_event_handler); */
	rtgui_widget_set_event_handler((rtgui_widget_t*)RTGUI_OBJECT(win), calibration_event_handler);

	/* to make rtgui_win_show->rtgui_widget_update be called, lomboswer */
#ifdef TEMP_FOR_SHOW
	rtgui_win_show(win, RT_FALSE);
#endif
	rtgui_win_show(win, RT_TRUE);
	rtgui_win_destroy(win);

	/* rtgui_application_destroy(app); */

	/* set calibration data */
	rt_device_control(calibration_ptr->device, RT_TOUCH_CALIBRATION_DATA, &calibration_ptr->data);

	/* recover to normal */
	rt_device_control(calibration_ptr->device, RT_TOUCH_NORMAL, RT_NULL);

#if 1 /* lomboswer */
	rtgui_thread_deregister(calibration_ptr->tid);
	if (calibration_mq != RT_NULL) {
		rt_mq_delete(calibration_mq);
		calibration_mq = RT_NULL;
	}
#endif

	/* release memory */
	rt_free(calibration_ptr);
	calibration_ptr = RT_NULL;

#if 0 /* lomboswer */
	{
	rt_thread_t gui_thread;
	extern void rt_gui_thread_entry(void *parameter);
	gui_thread = rt_thread_create("gui", rt_gui_thread_entry, RT_NULL, 2048, 100, 10);
	if (gui_thread != RT_NULL)
		rt_thread_startup(gui_thread);
	}
#endif
}

void calibration_init(void)
{
	rt_device_t device;
	rt_mq_t mq; /* lomboswer */

	device = rt_device_find("touch");
	if (device == RT_NULL) {
		return;
	}

	calibration_ptr = (struct calibration_session *)rt_malloc(sizeof(struct calibration_session));
	if (calibration_ptr == RT_NULL) {
		rt_kprintf("%s %d! ERROR\n", __func__, __LINE__);
		return;
	}
	rt_memset(calibration_ptr, 0, sizeof(struct calibration_data));
	calibration_ptr->device = device;

	rt_device_control(calibration_ptr->device, RT_TOUCH_CALIBRATION, (void *)calibration_data_post);

	calibration_ptr->tid = rt_thread_create("cali", calibration_entry, RT_NULL, 2048, 20, 5);

#if 1 /* lomboswer */
	calibration_mq = rt_mq_create("cali", 256, 4, RT_IPC_FLAG_FIFO);
	if (calibration_mq == RT_NULL) {
		return;
	}
	rtgui_thread_register(calibration_ptr->tid, calibration_mq);
#endif

	if (calibration_ptr->tid != RT_NULL)
		rt_thread_startup(calibration_ptr->tid);
}

/* lomboswer
#ifdef RT_USING_FINSH
#include <finsh.h>
void calibration(void)
{
	calibration_init();
}
FINSH_FUNCTION_EXPORT(calibration, perform touch calibration);
#endif
*/
