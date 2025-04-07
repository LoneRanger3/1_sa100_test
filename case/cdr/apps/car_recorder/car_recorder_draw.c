/*
 * car_recorder_draw.c - car recorder draw graphics.
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

#include <debug.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stddef.h>
#include <math.h>
#include <mqueue.h>
#include <clock_time.h>
#include "app_manage.h"
#include <div.h>

#include "car_recorder_draw.h"
#include "car_recorder_common.h"

#define BL_DRAW_MQ_SIZE		128
#define BL_DRAW_MQ_NUM_MAX	128
#define BL_DRAW_PRIO		27
#define BL_DRAW_STACK_SIZE	16384
#define MAX_CAR_NUM		8
#define BL_DRAW_LINE_COLOR	0x4cffff00
static int bl_draw_mq;

static int cdr_draw_thread_init(pthread_t *p_tid);
static int cdr_draw_create_msg(void);
static int cdr_draw_msg_recv(car_draw_info_t *msg_data, int timeout);
static void *cdr_draw_proc(void *param);
static int cdr_draw_destroy_msg(void);

int cur_line_buff_idx;
unsigned char *red_buff;
unsigned char *green_buff;
unsigned char *line_buff[2];
unsigned char *bsd_left_buff;
unsigned char *bsd_right_buff;
typedef struct line_info_tag {
	int cur_idx;
	unsigned char *buff[2];
} line_info_t;
static line_info_t line_info[MAX_CAR_NUM];

int cdr_draw_init(pthread_t *p_tid)
{
	int err = 0;

	lb_gal_bl_draw_show();
	lb_gal_bl_draw_init();

	err = cdr_draw_create_msg();
	if (err != 0) {
		LOG_E("lb_bl_draw_create_msg failed!\n");
		return err;
	}

	err = cdr_draw_thread_init(p_tid);
	if (err != 0) {
		LOG_E("lb_bl_draw_thread_init failed!\n");
		return err;
	}

	return 0;
}

int cdr_draw_exit(pthread_t tid)
{
	car_draw_info_t	tmp_draw_info;

	tmp_draw_info.b_draw = BKL_RELESE;
	cdr_draw_msg_send(tmp_draw_info);

	if (tid) {
		pthread_join(tid, NULL);
		tid = NULL;
	}

	cdr_draw_destroy_msg();
	/*lb_gal_bl_draw_hide();*/
	return 0;
}

static int cdr_draw_request(void)
{
	int i = 0;
	int red_color = 0x66ff0000;
	int green_color = 0x6600ff00;
	int en_adas = 0;
	int en_bsd = 0;

	en_adas = adas_get_enable();
	en_bsd = bsd_get_enable();

	if (en_adas == ENABLE_ON) {
		green_buff = (unsigned char *)rt_malloc(ADAS_BKL_RECT_SIZE_MAX);
		if (green_buff == NULL) {
			APP_LOG_E("malloc fail\n");
			return -1;
		}

		for (i = 0; i < ADAS_BKL_RECT_SIZE_MAX / 4; i++)
			((int *)green_buff)[i] = green_color;

		red_buff = (unsigned char *)rt_malloc(ADAS_BKL_RECT_SIZE_MAX);
		if (red_buff == NULL) {
			APP_LOG_E("malloc fail\n");
			return -1;
		}

		for (i = 0; i < ADAS_BKL_RECT_SIZE_MAX / 4; i++)
			((int *)red_buff)[i] = red_color;

		line_buff[0] = (unsigned char *)rt_malloc(ADAS_BKL_LINE_SIZE_MAX);
		if (line_buff[0] == NULL) {
			APP_LOG_E("malloc fail\n");
			return -1;
		}

		line_buff[1] = (unsigned char *)rt_malloc(ADAS_BKL_LINE_SIZE_MAX);
		if (line_buff[1] == NULL) {
			APP_LOG_E("malloc fail\n");
			return -1;
		}
	}

	if (en_bsd == ENABLE_ON) {
		int width, height;
		line_point point1, point2;
		int orange_color;
		int alpha = 0xff;

		width = BSD_BKL_LINE_WIDTH_MAX - 1;
		height = BSD_BKL_LINE_HEIGHT_MAX - BSD_BKL_LINE_START_POS;

		bsd_left_buff = (unsigned char *)rt_malloc(BSD_BKL_LINE_SIZE_MAX);
		if (bsd_left_buff == NULL) {
			APP_LOG_E("malloc fail\n");
			return -1;
		}
		rt_memset(bsd_left_buff, 0, BSD_BKL_LINE_SIZE_MAX);

		for (i = 0; i < BSD_BKL_LINE_WIDTH_MAX; i++) {
			point1.x = i;
			point1.y = 0;
			point2.x = width;
			point2.y = BSD_BKL_LINE_START_POS;
			orange_color = alpha << 24 | 0xfe571e;
			car_recorder_draw_line(bsd_left_buff, point1,
						point2, orange_color);
		}

		for (i = 0; i < height; i++) {
			point1.x = 0;
			point1.y = i;
			point2.x = width;
			point2.y = BSD_BKL_LINE_START_POS + i;
			orange_color = alpha << 24 | 0xfe571e;
			car_recorder_draw_line(bsd_left_buff, point1,
						point2, orange_color);
			if (alpha > 4)
				alpha -= 4;
		}

		bsd_right_buff = (unsigned char *)rt_malloc(BSD_BKL_LINE_SIZE_MAX);
		if (bsd_right_buff == NULL) {
			APP_LOG_E("malloc fail\n");
			return -1;
		}

		rt_memset(bsd_right_buff, 0, BSD_BKL_LINE_SIZE_MAX);

		alpha = 0xff;

		for (i = 0; i < BSD_BKL_LINE_WIDTH_MAX; i++) {
			point1.x = i;
			point1.y = BSD_BKL_LINE_HEIGHT_MAX - 1;
			point2.x = width;
			point2.y = height;
			orange_color = alpha << 24 | 0xfe571e;
			car_recorder_draw_line(bsd_right_buff, point1,
						point2, orange_color);
		}

		point1.y = BSD_BKL_LINE_HEIGHT_MAX;
		for (i = height - 1; i >= 0; i--) {
			point1.x = 0;
			point1.y--;
			point2.x = width;
			point2.y = i;
			orange_color = alpha << 24 | 0xfe571e;
			car_recorder_draw_line(bsd_right_buff,
						point1, point2, orange_color);

			if (alpha > 4)
				alpha -= 4;
		}

		for(i = 0; i < MAX_CAR_NUM; i++) {
			line_info[i].cur_idx = 0;
			line_info[i].buff[0] = (unsigned char *)rt_malloc(ADAS_BKL_LINE_SIZE_MAX);
			if (line_info[i].buff[0] == NULL) {
				APP_LOG_E("malloc fail\n");
				return -1;
			}
			
			line_info[i].buff[1] = (unsigned char *)rt_malloc(ADAS_BKL_LINE_SIZE_MAX);
			if (line_info[i].buff[1] == NULL) {
				APP_LOG_E("malloc fail\n");
				return -1;
			}
		}
	}

#if 0
	if (en_adas == ENABLE_ON && en_bsd != ENABLE_ON) {
		for (i = 0; i < ADAS_BKL_NUM_MAX; i++) {
			lb_gal_bl_draw_request(i);
			lb_gal_bl_draw_add(i);
		}
	}else if (en_adas != ENABLE_ON && en_bsd == ENABLE_ON) {
		for (i = ADAS_BKL_NUM_MAX; i < ADAS_BKL_NUM_MAX + BSD_BKL_NUM_MAX; i++) {
			lb_gal_bl_draw_request(i);
			lb_gal_bl_draw_add(i);
		}
	} else if (en_bsd == ENABLE_ON) {
		for (i = 0; i < ADAS_BKL_NUM_MAX + BSD_BKL_NUM_MAX; i++) {
			lb_gal_bl_draw_request(i);
			lb_gal_bl_draw_add(i);
		}
	}
#endif
	for (i = 0; i < ADAS_BKL_NUM_MAX + BSD_BKL_NUM_MAX; i++) {
		lb_gal_bl_draw_request(i);
		lb_gal_bl_draw_add(i);
	}

	return 0;
}

static int cdr_draw_release(void)
{
	int i = 0;
	int en_adas = 0;
	int en_bsd = 0;

	en_adas = adas_get_enable();
	en_bsd = bsd_get_enable();

	if (green_buff) {
		rt_free(green_buff);
		green_buff = NULL;
	}

	if (red_buff) {
		rt_free(red_buff);
		red_buff = NULL;
	}

	if (line_buff[0]) {
		rt_free(line_buff[0]);
		line_buff[0] = NULL;
	}

	if (line_buff[1]) {
		rt_free(line_buff[1]);
		line_buff[1] = NULL;
	}

	if (bsd_left_buff) {
		rt_free(bsd_left_buff);
		bsd_left_buff = NULL;
	}

	if (bsd_right_buff) {
		rt_free(bsd_right_buff);
		bsd_right_buff = NULL;
	}
	for (i = 0; i < MAX_CAR_NUM; i++) {
		if (line_info[i].buff[0])
			rt_free(line_info[i].buff[0]);
		if (line_info[i].buff[1])
			rt_free(line_info[i].buff[1]);
	}
	
#if 0
	if (en_adas == ENABLE_ON && en_bsd != ENABLE_ON) {
		for (i = 0; i < ADAS_BKL_NUM_MAX; i++)
			lb_gal_bl_draw_release(i);
	} else if (en_adas != ENABLE_ON && en_bsd == ENABLE_ON) {
		for (i = ADAS_BKL_NUM_MAX; i < ADAS_BKL_NUM_MAX + BSD_BKL_NUM_MAX; i++)
			lb_gal_bl_draw_release(i);
	} else if (en_adas == ENABLE_ON && en_bsd == ENABLE_ON) {
		for (i = 0; i < ADAS_BKL_NUM_MAX + BSD_BKL_NUM_MAX; i++)
			lb_gal_bl_draw_release(i);
	}
#endif
	for (i = 0; i < ADAS_BKL_NUM_MAX + BSD_BKL_NUM_MAX; i++)
		lb_gal_bl_draw_release(i);

	return 0;
}

static int cdr_bsd_draw_line(car_draw_info_t draw_info)
{
	unsigned char *p_draw_buff;
	lb_rect_t buff_rect;

	if (draw_info.idx == 9)
		p_draw_buff = bsd_left_buff;
	else if (draw_info.idx == 10)
		p_draw_buff = bsd_right_buff;
	else {
		APP_LOG_D("It is not bsd draw!\n");
		return 0;
	}

	buff_rect.x1 = draw_info.buff_rect.x1;
	buff_rect.x2 = draw_info.buff_rect.x2;
	buff_rect.y1 = draw_info.buff_rect.y1;
	buff_rect.y2 = draw_info.buff_rect.y2;

	if (abs(buff_rect.x1 - buff_rect.x2) == 1 ||
		abs(buff_rect.y1 - buff_rect.y2) == 1) {
		lb_gal_bl_draw_buff(draw_info.idx, buff_rect, p_draw_buff);
		return 0;
	}

	if (draw_info.idx == 9) {	/* left bsd image */
		buff_rect.x1 = 0;
		buff_rect.x2 = BSD_BKL_LINE_WIDTH_MAX;
		buff_rect.y1 = 0;
		buff_rect.y2 = BSD_BKL_LINE_HEIGHT_MAX;
	} else if (draw_info.idx == 10) {	/* right bsd image */
		buff_rect.x1 = 0;
		buff_rect.x2 = BSD_BKL_LINE_WIDTH_MAX;
		buff_rect.y1 = cdr_screen.width - BSD_BKL_LINE_HEIGHT_MAX;
		buff_rect.y2 = cdr_screen.width;

	}

#if 0
	if (draw_info.idx == 9) {	/* left bsd image */
		buff_rect.x1 = 0;
		buff_rect.x2 = BSD_BKL_LINE_WIDTH_MAX;
		buff_rect.y1 = 0;
		buff_rect.y2 = BSD_BKL_LINE_HEIGHT_MAX;
	} else if (draw_info.idx == 10) {	/* right bsd image */
		buff_rect.x1 = 0;
		buff_rect.x2 = BSD_BKL_LINE_WIDTH_MAX;
#ifdef SCREEN_ROT_90
		buff_rect.y1 = cdr_screen.width - BSD_BKL_LINE_HEIGHT_MAX;
		buff_rect.y2 = cdr_screen.width;
#else
		buff_rect.y1 = cdr_screen.height - BSD_BKL_LINE_HEIGHT_MAX;
		buff_rect.y2 = cdr_screen.height;
#endif
	}
#endif

	lb_gal_bl_draw_buff(draw_info.idx, buff_rect, p_draw_buff);

	return 0;
}
static int cdr_bsd_draw_polygon(car_draw_info_t draw_info)
{
	unsigned char *p_draw_buff;
	lb_rect_t buff_rect;
	line_point points[4];
	int i = 0;
	int idx = draw_info.idx;

	if (idx >= 8)
		return 0;

	if (line_info[idx].cur_idx == 0) {
		rt_memset(line_info[idx].buff[1], 0, ADAS_BKL_LINE_SIZE_MAX);
		p_draw_buff = line_info[idx].buff[1];
		line_info[idx].cur_idx = 1;
	} else {
		rt_memset(line_info[idx].buff[0], 0, ADAS_BKL_LINE_SIZE_MAX);
		p_draw_buff = line_info[idx].buff[0];
		line_info[idx].cur_idx = 0;
	}

	buff_rect.x1 = draw_info.buff_rect.x1;
	buff_rect.x2 = draw_info.buff_rect.x2;
	buff_rect.y1 = draw_info.buff_rect.y1;
	buff_rect.y2 = draw_info.buff_rect.y2;

	if (abs(buff_rect.x1 - buff_rect.x2) == 1 ||
		abs(buff_rect.y1 - buff_rect.y2) == 1) {
		lb_gal_bl_draw_buff(draw_info.idx, buff_rect, p_draw_buff);
		return 0;
	}

	points[0].x = draw_info.line_points[0].x;
	points[0].y = draw_info.line_points[0].y;
	points[1].x = draw_info.line_points[1].x;
	points[1].y = draw_info.line_points[1].y;
	points[2].x = draw_info.line_points[2].x;
	points[2].y = draw_info.line_points[2].y;
	points[3].x = draw_info.line_points[3].x;
	points[3].y = draw_info.line_points[3].y;

	car_recorder_draw_polygon(p_draw_buff, points, draw_info.border_width,
		draw_info.border_color, draw_info.en_fill, draw_info.fill_color);
	blk_draw_str(p_draw_buff, draw_info.buff_rect, SCREEN_ROTATE_270,
		draw_info.meters, draw_info.meters_color);
	lb_gal_bl_draw_buff(draw_info.idx, buff_rect, p_draw_buff);
	

	return 0;
}

static int cdr_adas_draw_line(car_draw_info_t draw_info)
{
	unsigned char *p_draw_buff;
	lb_rect_t buff_rect;
	line_point points[4];

	if (cur_line_buff_idx == 0) {
		rt_memset(line_buff[1], 0, ADAS_BKL_LINE_SIZE_MAX);
		p_draw_buff = line_buff[1];
		cur_line_buff_idx = 1;
	} else {
		rt_memset(line_buff[0], 0, ADAS_BKL_LINE_SIZE_MAX);
		p_draw_buff = line_buff[0];
		cur_line_buff_idx = 0;
	}

	buff_rect.x1 = draw_info.buff_rect.x1;
	buff_rect.x2 = draw_info.buff_rect.x2;
	buff_rect.y1 = draw_info.buff_rect.y1;
	buff_rect.y2 = draw_info.buff_rect.y2;

	if (abs(buff_rect.x1 - buff_rect.x2) == 1 ||
		abs(buff_rect.y1 - buff_rect.y2) == 1) {
		lb_gal_bl_draw_buff(draw_info.idx, buff_rect, p_draw_buff);
		return 0;
	}

	points[0].x = draw_info.line_points[0].x;
	points[0].y = draw_info.line_points[0].y;
	points[1].x = draw_info.line_points[1].x;
	points[1].y = draw_info.line_points[1].y;
	points[2].x = draw_info.line_points[2].x;
	points[2].y = draw_info.line_points[2].y;
	points[3].x = draw_info.line_points[3].x;
	points[3].y = draw_info.line_points[3].y;

	car_recorder_draw_polygon(p_draw_buff, points, draw_info.border_width,
		draw_info.border_color, draw_info.en_fill, draw_info.fill_color);
	lb_gal_bl_draw_buff(draw_info.idx, buff_rect, p_draw_buff);

	return 0;
}


static int cdr_adas_draw_rect(car_draw_info_t draw_info)
{
	unsigned char *p_draw_buff;
	unsigned int w;
	unsigned int h;
	lb_rect_t buff_rect;

	w = abs(draw_info.buff_rect.x2 - draw_info.buff_rect.x1);
	h = abs(draw_info.buff_rect.y2 - draw_info.buff_rect.y1);

	if (w > ADAS_BKL_RECT_WIDTH_MAX) {
		buff_rect.x1 = (w - ADAS_BKL_RECT_WIDTH_MAX)/2 +
			draw_info.buff_rect.x1;
		if (draw_info.buff_rect.x2 > draw_info.buff_rect.x1)
			buff_rect.x2 = buff_rect.x1 + ADAS_BKL_RECT_WIDTH_MAX;
		else
			buff_rect.x2 = buff_rect.x1 - ADAS_BKL_RECT_WIDTH_MAX;
		w = ADAS_BKL_RECT_WIDTH_MAX;
	} else {
		buff_rect.x1 = draw_info.buff_rect.x1;
		buff_rect.x2 = draw_info.buff_rect.x2;
	}

	if (h > ADAS_BKL_RECT_HEIGHT_MAX) {
		buff_rect.y1 = (h - ADAS_BKL_RECT_HEIGHT_MAX)/2 +
			draw_info.buff_rect.y1;
		if (draw_info.buff_rect.y2 > draw_info.buff_rect.y1)
			buff_rect.y2 = buff_rect.y1 + ADAS_BKL_RECT_HEIGHT_MAX;
		else
			buff_rect.y2 = buff_rect.y1 - ADAS_BKL_RECT_HEIGHT_MAX;
		h = ADAS_BKL_RECT_HEIGHT_MAX;
	} else {
		buff_rect.y1 = draw_info.buff_rect.y1;
		buff_rect.y2 = draw_info.buff_rect.y2;
	}

	if (draw_info.color == 0)
		p_draw_buff = red_buff;
	else
		p_draw_buff = green_buff;

	lb_gal_bl_draw_buff(draw_info.idx, buff_rect, p_draw_buff);

	return 0;
}

static int cdr_draw_thread_init(pthread_t *p_tid)
{
	pthread_attr_t tmp_attr;
	struct sched_param shed_param;
	int32_t ret = 0;

	if (p_tid == NULL) {
		ret = -1;
		LOG_E("failed ret:%d\n", ret);
		return -1;
	}

	ret = pthread_attr_init(&tmp_attr);
	if (ret != 0) {
		ret = -1;
		LOG_E("failed ret:%d\n", ret);
		return -1;
	}

	ret = pthread_attr_setscope(&tmp_attr, PTHREAD_SCOPE_SYSTEM);
	if (ret != 0) {
		ret = -1;
		LOG_E("failed ret:%d\n", ret);
		goto exit;
	}

	ret = pthread_attr_getschedparam(&tmp_attr, &shed_param);
	if (ret != 0) {
		ret = -1;
		LOG_E("failed ret:%d\n", ret);
		goto exit;
	}

	shed_param.sched_priority = BL_DRAW_PRIO;
	ret = pthread_attr_setschedparam(&tmp_attr, &shed_param);
	if (ret != 0) {
		ret = -1;
		LOG_E("failed ret:%d\n", ret);
		goto exit;
	}

	ret = pthread_attr_setstacksize(&tmp_attr, (size_t)BL_DRAW_STACK_SIZE);
	if (ret != 0) {
		ret = -1;
		LOG_E("failed ret:%d\n", ret);
		goto exit;
	}

	ret = pthread_create(p_tid, &tmp_attr, &cdr_draw_proc, NULL);
	if (ret != 0) {
		ret = -1;
		LOG_E("failed ret:%d\n", ret);
		goto exit;
	}

	return ret;

exit:
	pthread_attr_destroy(&tmp_attr);

	return ret;
}

static int cdr_draw_create_msg(void)
{
	struct mq_attr      mqstat;
	int                 flags = O_CREAT | O_RDWR | O_NONBLOCK;
	mqd_t               p_msg = NULL;

	memset(&mqstat, 0, sizeof(struct mq_attr));
	mqstat.mq_maxmsg    = BL_DRAW_MQ_SIZE;
	mqstat.mq_msgsize   = BL_DRAW_MQ_NUM_MAX;
	mqstat.mq_flags     = flags;

	p_msg = mq_open("bldraw_mq", flags, 0666, &mqstat);
	if (p_msg == NULL) {
		printf("[%s]Message queue open failed!\n", __func__);
		return -1;
	}

	bl_draw_mq = (int)p_msg;
	return 0;
}

static int cdr_draw_destroy_msg(void)
{
	mq_unlink("bldraw_mq");
	mq_close((mqd_t)bl_draw_mq);

	return 0;
}

int cdr_draw_msg_send(car_draw_info_t car_draw_info)
{
	int			err = 0;
	unsigned int		msg_act_len = 0;
	car_draw_info_t	*p_msg_data;

	msg_act_len = sizeof(car_draw_info_t);
	p_msg_data = (car_draw_info_t *)rt_malloc(msg_act_len);
	if (NULL == p_msg_data) {
		LOG_E("[%s]No enough memory!\n", __func__);
		return -1;
	}

	memcpy(p_msg_data, &car_draw_info, sizeof(car_draw_info_t));

	err = mq_send((mqd_t)bl_draw_mq, (char *)p_msg_data, msg_act_len, 0);
	if (err == -1) {
		LOG_E("Send message failed!\n");
		rt_free(p_msg_data);
		return -2;
	}

	rt_free(p_msg_data);
	return 0;
}

static int cdr_draw_msg_recv(car_draw_info_t *msg_data, int timeout)
{
	int             err = 0;
	char            recv_buf[BL_DRAW_MQ_SIZE] = {0};
	struct timespec tmo;

	if (timeout == 0) {
		err = mq_receive((mqd_t)bl_draw_mq, recv_buf,
					sizeof(car_draw_info_t), 0);
	} else {
		int nsec = 0, sec = timeout;
		clock_gettime(CLOCK_REALTIME, &tmo);
		nsec = do_div_rem(sec, 1000);
		nsec = nsec * 1000 * 1000;
		tmo.tv_nsec += nsec;
		tmo.tv_sec  += sec;
		err = mq_timedreceive((mqd_t)bl_draw_mq, recv_buf,
					sizeof(car_draw_info_t), 0, &tmo);
	}
	if (err == -1) {
		LOG_E("mq received failed!\n");
		return err;
	}

	memcpy(msg_data, recv_buf, sizeof(car_draw_info_t));

	return 0;
}

static void *cdr_draw_proc(void *param)
{
	int			err = 0;
	car_draw_info_t	bl_draw_pic_info;

	err = cdr_draw_request();
	if (err != 0) {
		pthread_exit(0);
		return NULL;
	}

	while (1) {
		err = cdr_draw_msg_recv(&bl_draw_pic_info, 0);
		if (0 == err) {
			switch (bl_draw_pic_info.b_draw) {
			case BKL_SHOW:
				lb_gal_bl_draw_show();
				break;
			case BKL_HIDE:
				lb_gal_bl_draw_hide();
				break;
			case BKL_DRAW_RECT:
				cdr_adas_draw_rect(bl_draw_pic_info);
				break;
			case BKL_DRAW_LINE:
				cdr_adas_draw_line(bl_draw_pic_info);
				break;
			case BKL_DRAW_BSD_LINE:
				/*cdr_bsd_draw_line(bl_draw_pic_info);*/
				cdr_bsd_draw_polygon(bl_draw_pic_info);
				break;
			case BKL_RELESE:
				cdr_draw_release();
				pthread_exit(0);
				break;
			default:
				break;
			}
		}
	}

	pthread_exit(0);
	return NULL;
}


