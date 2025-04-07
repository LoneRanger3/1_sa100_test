/*
 * vic_det_dev.c - sensor input/output detection
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

#include "system/system_mq.h"

#if defined(ARCH_LOMBO_N7V1_TDR) && defined(ARCH_LOMBO_N7V1_CDR)
#define DBG_LEVEL		DBG_ERROR

#include <debug.h>
#include "viss.h"
#include "vic_det_dev.h"
#include "vic_det_drv.h"
#include "media_dev.h"
#include "spinlock.h"
#include "gpio/pinctrl.h"
#include <div.h>

#define VIC_DET_GPIO_MODULE	"vic_det"
#define VIC_CAR_BACK_DET_1        "back_det_1"
#define VIC_CAR_BACK_DET_2        "back_det_2"
#define VIC_CAR_BACK_DET_3        "back_det_3"
#define VIC_CAR_BACK_DET_4        "back_det_4"

#define DET_MS		130
#define DET_TIMEOUT_TICK		(DET_MS*RT_TICK_PER_SECOND)

struct vic_det_status present_status_1;
static int shake_cnt_1;
static int last_back_val_1;
struct vic_det_status present_status_2;
static int shake_cnt_2;
static int last_back_val_2;
struct vic_det_status present_status_3;
static int shake_cnt_3;
static int last_back_val_3;
struct vic_det_status present_status_4;
static int shake_cnt_4;
static int last_back_val_4;

static rt_err_t __vic_det_change_1(struct vic_det_dev *vic_det, rt_int32_t type)
{
	rt_uint32_t ret = 0, temp = 0;
	/* LOG_E("__vic_det_change %d", type); */
	if (1 == type) {
		if (EINT_TRIG_LOW_LEVEL == vic_det->back_in_det_gpio_irq_1.trig_type)
			vic_det->back_in_det_gpio_irq_1.trig_type = EINT_TRIG_HIGH_LEVEL;
		else if (EINT_TRIG_HIGH_LEVEL ==
						vic_det->back_in_det_gpio_irq_1.trig_type)
			vic_det->back_in_det_gpio_irq_1.trig_type = EINT_TRIG_LOW_LEVEL;
		temp = pinctrl_gpio_set_irq_trig_type(vic_det->pctrl,
			vic_det->back_in_det_gpio_1, &vic_det->back_in_det_gpio_irq_1);
		if (temp != RT_EOK) {
			LOG_E("Failed to pinctrl_gpio_free_irq");
			return -1;
		}
	} else if (2 == type) {
		temp = pinctrl_gpio_set_irq_trig_type(vic_det->pctrl,
			vic_det->back_in_det_gpio_1, &vic_det->back_in_det_gpio_irq_1);
		if (temp != RT_EOK) {
			LOG_E("Failed to pinctrl_gpio_free_irq");
			return -1;
		}
	}
	if (temp != RT_EOK)
		LOG_E("Failed to __vic_det_change");
	return ret;
}

static rt_err_t __vic_det_change_2(struct vic_det_dev *vic_det, rt_int32_t type)
{
	rt_uint32_t ret = 0, temp = 0;
	/* LOG_E("__vic_det_change %d", type); */
	if (1 == type) {
		if (EINT_TRIG_LOW_LEVEL == vic_det->back_in_det_gpio_irq_2.trig_type)
			vic_det->back_in_det_gpio_irq_2.trig_type = EINT_TRIG_HIGH_LEVEL;
		else if (EINT_TRIG_HIGH_LEVEL ==
						vic_det->back_in_det_gpio_irq_2.trig_type)
			vic_det->back_in_det_gpio_irq_2.trig_type = EINT_TRIG_LOW_LEVEL;
		temp = pinctrl_gpio_set_irq_trig_type(vic_det->pctrl,
			vic_det->back_in_det_gpio_2, &vic_det->back_in_det_gpio_irq_2);
		if (temp != RT_EOK) {
			LOG_E("Failed to pinctrl_gpio_free_irq");
			return -1;
		}
	} else if (2 == type) {
		temp = pinctrl_gpio_set_irq_trig_type(vic_det->pctrl,
			vic_det->back_in_det_gpio_2, &vic_det->back_in_det_gpio_irq_2);
		if (temp != RT_EOK) {
			LOG_E("Failed to pinctrl_gpio_free_irq");
			return -1;
		}
	}
	if (temp != RT_EOK)
		LOG_E("Failed to __vic_det_change");
	return ret;
}

static rt_err_t __vic_det_change_3(struct vic_det_dev *vic_det, rt_int32_t type)
{
	rt_uint32_t ret = 0, temp = 0;
	/* LOG_E("__vic_det_change %d", type); */
	if (1 == type) {
		if (EINT_TRIG_LOW_LEVEL == vic_det->back_in_det_gpio_irq_3.trig_type)
			vic_det->back_in_det_gpio_irq_3.trig_type = EINT_TRIG_HIGH_LEVEL;
		else if (EINT_TRIG_HIGH_LEVEL ==
						vic_det->back_in_det_gpio_irq_3.trig_type)
			vic_det->back_in_det_gpio_irq_3.trig_type = EINT_TRIG_LOW_LEVEL;
		temp = pinctrl_gpio_set_irq_trig_type(vic_det->pctrl,
			vic_det->back_in_det_gpio_3, &vic_det->back_in_det_gpio_irq_3);
		if (temp != RT_EOK) {
			LOG_E("Failed to pinctrl_gpio_free_irq");
			return -1;
		}
	} else if (2 == type) {
		temp = pinctrl_gpio_set_irq_trig_type(vic_det->pctrl,
			vic_det->back_in_det_gpio_3, &vic_det->back_in_det_gpio_irq_3);
		if (temp != RT_EOK) {
			LOG_E("Failed to pinctrl_gpio_free_irq");
			return -1;
		}
	}
	if (temp != RT_EOK)
		LOG_E("Failed to __vic_det_change");
	return ret;
}

static rt_err_t __vic_det_change_4(struct vic_det_dev *vic_det, rt_int32_t type)
{
	rt_uint32_t ret = 0, temp = 0;
	/* LOG_E("__vic_det_change %d", type); */
	if (1 == type) {
		if (EINT_TRIG_LOW_LEVEL == vic_det->back_in_det_gpio_irq_4.trig_type)
			vic_det->back_in_det_gpio_irq_4.trig_type = EINT_TRIG_HIGH_LEVEL;
		else if (EINT_TRIG_HIGH_LEVEL ==
						vic_det->back_in_det_gpio_irq_4.trig_type)
			vic_det->back_in_det_gpio_irq_4.trig_type = EINT_TRIG_LOW_LEVEL;
		temp = pinctrl_gpio_set_irq_trig_type(vic_det->pctrl,
			vic_det->back_in_det_gpio_4, &vic_det->back_in_det_gpio_irq_4);
		if (temp != RT_EOK) {
			LOG_E("Failed to pinctrl_gpio_free_irq");
			return -1;
		}
	} else if (2 == type) {
		temp = pinctrl_gpio_set_irq_trig_type(vic_det->pctrl,
			vic_det->back_in_det_gpio_4, &vic_det->back_in_det_gpio_irq_4);
		if (temp != RT_EOK) {
			LOG_E("Failed to pinctrl_gpio_free_irq");
			return -1;
		}
	}
	if (temp != RT_EOK)
		LOG_E("Failed to __vic_det_change");
	return ret;
}

static void __back_det_handler_fun_1(void *param)
{
	struct vic_det_dev *vic_det = (struct vic_det_dev *)param;
	rt_uint32_t ret = 0, last_status;

	rt_timer_stop(&vic_det->back_timer_1);
	last_status = vic_det->back_status_1;
	ret = pinctrl_gpio_get_value(vic_det->pctrl, vic_det->back_in_det_gpio_1);
	if (ret != last_back_val_1)
		shake_cnt_1 = 0;
	/* LOG_E("__back_det_handler_fun %d %d\n", shake_cnt,ret); */
	if (ret) {
		if (LB_SYSMSG_BACK_OFF_1 != last_status) {
			shake_cnt_1++;
			if (shake_cnt_1 >= 8) {
				vic_det->back_status_1 = LB_SYSMSG_BACK_OFF_1;
				lb_system_mq_send(LB_SYSMSG_BACK_OFF_1, NULL, 0, 0);
				LOG_E("LB_SYSMSG_BACK_OFF_1");
				shake_cnt_1 = 0;
				__vic_det_change_1(vic_det, 1);
				goto VIC_EIXT;
			}
		}
	} else {
		if (LB_SYSMSG_BACK_ON_1 != last_status) {
			if (LB_SYSMSG_AV_PLUGIN == vic_det->av_status) {
				shake_cnt_1++;
				if (shake_cnt_1 >= 1) {
					vic_det->back_status_1 = LB_SYSMSG_BACK_ON_1;
					lb_system_mq_send(LB_SYSMSG_BACK_ON_1,
								NULL, 0, 0);
					LOG_E("LB_SYSMSG_BACK_ON_1");
					shake_cnt_1 = 0;
					__vic_det_change_1(vic_det, 1);
					goto VIC_EIXT;
				}
			}
		}
	}
	__vic_det_change_1(vic_det, 2);
VIC_EIXT:
	present_status_1.back_status = vic_det->back_status_1;
	last_back_val_1 = ret;
}

static void __back_det_handler_fun_2(void *param)
{
	struct vic_det_dev *vic_det = (struct vic_det_dev *)param;
	rt_uint32_t ret = 0, last_status;

	rt_timer_stop(&vic_det->back_timer_2);
	last_status = vic_det->back_status_2;
	ret = pinctrl_gpio_get_value(vic_det->pctrl, vic_det->back_in_det_gpio_2);
	if (ret != last_back_val_2)
		shake_cnt_2 = 0;
	/* LOG_E("__back_det_handler_fun %d %d\n", shake_cnt,ret); */
	if (ret) {
		if (LB_SYSMSG_BACK_OFF_2 != last_status) {
			shake_cnt_2++;
			if (shake_cnt_2 >= 8) {
				vic_det->back_status_2 = LB_SYSMSG_BACK_OFF_2;
				lb_system_mq_send(LB_SYSMSG_BACK_OFF_2, NULL, 0, 0);
				LOG_E("LB_SYSMSG_BACK_OFF_2");
				shake_cnt_2 = 0;
				__vic_det_change_2(vic_det, 1);
				goto VIC_EIXT;
			}
		}
	} else {
		if (LB_SYSMSG_BACK_ON_2 != last_status) {
			if (LB_SYSMSG_AV_PLUGIN == vic_det->av_status) {
				shake_cnt_2++;
				if (shake_cnt_2 >= 1) {
					vic_det->back_status_2 = LB_SYSMSG_BACK_ON_2;
					lb_system_mq_send(LB_SYSMSG_BACK_ON_2,
								NULL, 0, 0);
					LOG_E("LB_SYSMSG_BACK_ON_2");
					shake_cnt_2 = 0;
					__vic_det_change_2(vic_det, 1);
					goto VIC_EIXT;
				}
			}
		}
	}
	__vic_det_change_2(vic_det, 2);
VIC_EIXT:
	present_status_2.back_status = vic_det->back_status_2;
	last_back_val_2 = ret;
}

static void __back_det_handler_fun_3(void *param)
{
	struct vic_det_dev *vic_det = (struct vic_det_dev *)param;
	rt_uint32_t ret = 0, last_status;

	rt_timer_stop(&vic_det->back_timer_3);
	last_status = vic_det->back_status_3;
	ret = pinctrl_gpio_get_value(vic_det->pctrl, vic_det->back_in_det_gpio_3);
	if (ret != last_back_val_3)
		shake_cnt_3 = 0;
	/* LOG_E("__back_det_handler_fun %d %d\n", shake_cnt,ret); */
	if (ret) {
		if (LB_SYSMSG_BACK_OFF_3 != last_status) {
			shake_cnt_3++;
			if (shake_cnt_3 >= 16) {
				vic_det->back_status_3 = LB_SYSMSG_BACK_OFF_3;
				lb_system_mq_send(LB_SYSMSG_BACK_OFF_3, NULL, 0, 0);
				LOG_E("LB_SYSMSG_BACK_OFF_3");
				shake_cnt_3 = 0;
				__vic_det_change_3(vic_det, 1);
				goto VIC_EIXT;
			}
		}
	} else {
		if (LB_SYSMSG_BACK_ON_3 != last_status) {
			if (LB_SYSMSG_AV_PLUGIN == vic_det->av_status) {
				shake_cnt_3++;
				if (shake_cnt_3 >= 1) {
					vic_det->back_status_3 = LB_SYSMSG_BACK_ON_3;
					lb_system_mq_send(LB_SYSMSG_BACK_ON_3,
								NULL, 0, 0);
					LOG_E("LB_SYSMSG_BACK_ON_3");
					shake_cnt_3 = 0;
					__vic_det_change_3(vic_det, 1);
					goto VIC_EIXT;
				}
			}
		}
	}
	__vic_det_change_3(vic_det, 2);
VIC_EIXT:
	present_status_3.back_status = vic_det->back_status_3;
	last_back_val_3 = ret;
}

static void __back_det_handler_fun_4(void *param)
{
	struct vic_det_dev *vic_det = (struct vic_det_dev *)param;
	rt_uint32_t ret = 0, last_status;

	rt_timer_stop(&vic_det->back_timer_4);
	last_status = vic_det->back_status_4;
	ret = pinctrl_gpio_get_value(vic_det->pctrl, vic_det->back_in_det_gpio_4);
	if (ret != last_back_val_4)
		shake_cnt_4 = 0;
	/* LOG_E("__back_det_handler_fun %d %d\n", shake_cnt,ret); */
	if (ret) {
		if (LB_SYSMSG_BACK_OFF_4 != last_status) {
			shake_cnt_4++;
			if (shake_cnt_4 >= 16) {
				vic_det->back_status_4 = LB_SYSMSG_BACK_OFF_4;
				lb_system_mq_send(LB_SYSMSG_BACK_OFF_4, NULL, 0, 0);
				LOG_E("LB_SYSMSG_BACK_OFF_4");
				shake_cnt_4 = 0;
				__vic_det_change_4(vic_det, 1);
				goto VIC_EIXT;
			}
		}
	} else {
		if (LB_SYSMSG_BACK_ON_4 != last_status) {
			if (LB_SYSMSG_AV_PLUGIN == vic_det->av_status) {
				shake_cnt_4++;
				if (shake_cnt_4 >= 1) {
					vic_det->back_status_4 = LB_SYSMSG_BACK_ON_4;
					lb_system_mq_send(LB_SYSMSG_BACK_ON_4,
								NULL, 0, 0);
					LOG_E("LB_SYSMSG_BACK_ON_4");
					shake_cnt_4 = 0;
					__vic_det_change_4(vic_det, 1);
					goto VIC_EIXT;
				}
			}
		}
	}
	__vic_det_change_4(vic_det, 2);
VIC_EIXT:
	present_status_4.back_status = vic_det->back_status_4;
	last_back_val_4 = ret;
}

static void __back_in_det_irq_handler_1(void *data)
{
	struct vic_det_dev *vic_det = (struct vic_det_dev *)data;
	/* LOG_E("__back_in_det_irq_handler_1 "); */
	pinctrl_gpio_irq_disable(vic_det->pctrl, vic_det->back_in_det_gpio_1);
	rt_timer_start(&vic_det->back_timer_1);
}

static void __back_in_det_irq_handler_2(void *data)
{
	struct vic_det_dev *vic_det = (struct vic_det_dev *)data;
	/* LOG_E("__back_in_det_irq_handler_2 "); */
	pinctrl_gpio_irq_disable(vic_det->pctrl, vic_det->back_in_det_gpio_2);
	rt_timer_start(&vic_det->back_timer_2);
}

static void __back_in_det_irq_handler_3(void *data)
{
	struct vic_det_dev *vic_det = (struct vic_det_dev *)data;
	/* LOG_E("__back_in_det_irq_handler_3 "); */
	pinctrl_gpio_irq_disable(vic_det->pctrl, vic_det->back_in_det_gpio_3);
	rt_timer_start(&vic_det->back_timer_3);
}

static void __back_in_det_irq_handler_4(void *data)
{
	struct vic_det_dev *vic_det = (struct vic_det_dev *)data;
	/* LOG_E("__back_in_det_irq_handler_4 "); */
	pinctrl_gpio_irq_disable(vic_det->pctrl, vic_det->back_in_det_gpio_4);
	rt_timer_start(&vic_det->back_timer_4);
}

rt_err_t __vic_det_dev_config(struct vic_det_dev *vic_det)
{
	rt_err_t ret = 0;

	vic_det->pctrl = pinctrl_get(VIC_DET_GPIO_MODULE);
	if (RT_NULL == vic_det->pctrl) {
		LOG_E("pinctrl_get Failed");
		return -RT_ERROR;
	}

	vic_det->back_in_det_gpio_1 = -RT_ERROR;
	ret = config_get_u32_array(VIC_DET_GPIO_MODULE, VIC_CAR_BACK_DET_1,
				vic_det->back_in_det_pctrl_1,
				ARRAY_SIZE(vic_det->back_in_det_pctrl_1));
	if (ret != ARRAY_SIZE(vic_det->back_in_det_pctrl_1)) {
		LOG_E("vic_det: get back_in_det_gpio_1 error. ret:%d", ret);
		return -RT_ERROR;
	} else {
		vic_det->back_in_det_gpio_1 = pinctrl_gpio_request(vic_det->pctrl,
			vic_det->back_in_det_pctrl_1[0], vic_det->back_in_det_pctrl_1[1]);
		if (vic_det->back_in_det_gpio_1 < 0) {
			LOG_E("Failed to request back_in_det_gpio_1");
			return -RT_ERROR;
		} else {
			ret = pinctrl_gpio_direction_input(vic_det->pctrl,
					vic_det->back_in_det_gpio_1);
			if (ret != RT_EOK) {
				LOG_E("Can't set back_in_det_gpio_1 direction");
				return -RT_ERROR;
			}
		}

	}

	vic_det->back_in_det_gpio_2 = -RT_ERROR;
	ret = config_get_u32_array(VIC_DET_GPIO_MODULE, VIC_CAR_BACK_DET_2,
				vic_det->back_in_det_pctrl_2,
				ARRAY_SIZE(vic_det->back_in_det_pctrl_2));
	if (ret != ARRAY_SIZE(vic_det->back_in_det_pctrl_2)) {
		LOG_E("vic_det: get back_in_det_gpio_2 error. ret:%d", ret);
		return -RT_ERROR;
	} else {
		vic_det->back_in_det_gpio_2 = pinctrl_gpio_request(vic_det->pctrl,
			vic_det->back_in_det_pctrl_2[0], vic_det->back_in_det_pctrl_2[1]);
		if (vic_det->back_in_det_gpio_2 < 0) {
			LOG_E("Failed to request back_in_det_gpio_2");
			return -RT_ERROR;
		} else {
			ret = pinctrl_gpio_direction_input(vic_det->pctrl,
					vic_det->back_in_det_gpio_2);
			if (ret != RT_EOK) {
				LOG_E("Can't set back_in_det_gpio_2 direction");
				return -RT_ERROR;
			}
		}

	}

	vic_det->back_in_det_gpio_3 = -RT_ERROR;
	ret = config_get_u32_array(VIC_DET_GPIO_MODULE, VIC_CAR_BACK_DET_3,
				vic_det->back_in_det_pctrl_3,
				ARRAY_SIZE(vic_det->back_in_det_pctrl_3));
	if (ret != ARRAY_SIZE(vic_det->back_in_det_pctrl_3)) {
		LOG_E("vic_det: get back_in_det_gpio_3 error. ret:%d", ret);
		return -RT_ERROR;
	} else {
		vic_det->back_in_det_gpio_3 = pinctrl_gpio_request(vic_det->pctrl,
			vic_det->back_in_det_pctrl_3[0], vic_det->back_in_det_pctrl_3[1]);
		if (vic_det->back_in_det_gpio_3 < 0) {
			LOG_E("Failed to request back_in_det_gpio_3");
			return -RT_ERROR;
		} else {
			ret = pinctrl_gpio_direction_input(vic_det->pctrl,
					vic_det->back_in_det_gpio_3);
			if (ret != RT_EOK) {
				LOG_E("Can't set back_in_det_gpio_3 direction");
				return -RT_ERROR;
			}
		}

	}

	vic_det->back_in_det_gpio_4 = -RT_ERROR;
	ret = config_get_u32_array(VIC_DET_GPIO_MODULE, VIC_CAR_BACK_DET_4,
				vic_det->back_in_det_pctrl_4,
				ARRAY_SIZE(vic_det->back_in_det_pctrl_4));
	if (ret != ARRAY_SIZE(vic_det->back_in_det_pctrl_4)) {
		LOG_E("vic_det: get back_in_det_gpio_4 error. ret:%d", ret);
		return -RT_ERROR;
	} else {
		vic_det->back_in_det_gpio_4 = pinctrl_gpio_request(vic_det->pctrl,
			vic_det->back_in_det_pctrl_4[0], vic_det->back_in_det_pctrl_4[1]);
		if (vic_det->back_in_det_gpio_4 < 0) {
			LOG_E("Failed to request back_in_det_gpio_4");
			return -RT_ERROR;
		} else {
			ret = pinctrl_gpio_direction_input(vic_det->pctrl,
					vic_det->back_in_det_gpio_4);
			if (ret != RT_EOK) {
				LOG_E("Can't set back_in_det_gpio_4 direction");
				return -RT_ERROR;
			}
		}

	}
	return RT_EOK;
}

static void __vic_det_dev_unconfig(struct vic_det_dev *vic_det)
{
	rt_err_t ret;
	if (RT_NULL != vic_det->pctrl) {
		if (vic_det->back_in_det_gpio_1 != (-RT_ERROR)) {
			ret = pinctrl_gpio_free_irq(vic_det->pctrl,
					vic_det->back_in_det_gpio_1);
			if (ret != RT_EOK)
				LOG_E("free back_in_det_gpio_1 irq Failed");
		}

		if (vic_det->back_in_det_gpio_2 != (-RT_ERROR)) {
			ret = pinctrl_gpio_free_irq(vic_det->pctrl,
					vic_det->back_in_det_gpio_2);
			if (ret != RT_EOK)
				LOG_E("free back_in_det_gpio_2 irq Failed");
		}

		if (vic_det->back_in_det_gpio_3 != (-RT_ERROR)) {
			ret = pinctrl_gpio_free_irq(vic_det->pctrl,
					vic_det->back_in_det_gpio_3);
			if (ret != RT_EOK)
				LOG_E("free back_in_det_gpio_3 irq Failed");
		}

		if (vic_det->back_in_det_gpio_4 != (-RT_ERROR)) {
			ret = pinctrl_gpio_free_irq(vic_det->pctrl,
					vic_det->back_in_det_gpio_4);
			if (ret != RT_EOK)
				LOG_E("free back_in_det_gpio_4 irq Failed");
		}

		pinctrl_put(vic_det->pctrl);
		vic_det->pctrl = RT_NULL;
	}
}

static rt_err_t __vic_det_init(struct vic_det_dev *vic_det)
{
	rt_err_t ret = 0, leave = 0;
	rt_tick_t shake_time = 0;

	vic_det->av_status = LB_SYSMSG_AV_PLUGIN;

	vic_det->back_in_det_gpio_irq_1.handler = __back_in_det_irq_handler_1;
	vic_det->back_in_det_gpio_irq_1.irq_arg = vic_det;
	vic_det->back_in_det_gpio_irq_1.clock_src = GPIO_IRQ_HOSC_24MHZ;
	vic_det->back_in_det_gpio_irq_1.clock_src_div = 1;
	leave = 0;
	leave = pinctrl_gpio_get_value(vic_det->pctrl, vic_det->back_in_det_gpio_1);
	if (leave)
		vic_det->back_in_det_gpio_irq_1.trig_type = EINT_TRIG_LOW_LEVEL;
	else
		vic_det->back_in_det_gpio_irq_1.trig_type = EINT_TRIG_HIGH_LEVEL;
	shake_time = DET_TIMEOUT_TICK;
	do_div(shake_time, 1000);
	if (vic_det->back_in_det_gpio_1 != (-RT_ERROR)) {
		rt_timer_init(&vic_det->back_timer_1, "back_det1",
			__back_det_handler_fun_1, vic_det,
			shake_time,
			RT_TIMER_FLAG_DEACTIVATED |
			RT_TIMER_FLAG_ONE_SHOT |
			RT_TIMER_FLAG_HARD_TIMER);
		ret = pinctrl_gpio_request_irq(vic_det->pctrl,
			vic_det->back_in_det_gpio_1, &vic_det->back_in_det_gpio_irq_1);
		if (ret != RT_EOK) {
			LOG_E("Failed to request back det irq_1");
			goto off_1;
		}
		/* leave = pinctrl_gpio_get_value(vic_det->pctrl,
						vic_det->back_in_det_gpio); */
		if (leave) {
			vic_det->back_status_1 = LB_SYSMSG_BACK_OFF_1;
			lb_system_mq_send(LB_SYSMSG_BACK_OFF_1, NULL, 0, 0);
			LOG_D("LB_SYSMSG_BACK_OFF_1");
		} else {
			vic_det->back_status_1 = LB_SYSMSG_BACK_ON_1;
			lb_system_mq_send(LB_SYSMSG_BACK_ON_1, NULL, 0, 0);
			LOG_D("LB_SYSMSG_BACK_ON_1");
		}
	}
off_1:

	vic_det->back_in_det_gpio_irq_2.handler = __back_in_det_irq_handler_2;
	vic_det->back_in_det_gpio_irq_2.irq_arg = vic_det;
	vic_det->back_in_det_gpio_irq_2.clock_src = GPIO_IRQ_HOSC_24MHZ;
	vic_det->back_in_det_gpio_irq_2.clock_src_div = 1;
	leave = 0;
	leave = pinctrl_gpio_get_value(vic_det->pctrl, vic_det->back_in_det_gpio_2);
	if (leave)
		vic_det->back_in_det_gpio_irq_2.trig_type = EINT_TRIG_LOW_LEVEL;
	else
		vic_det->back_in_det_gpio_irq_2.trig_type = EINT_TRIG_HIGH_LEVEL;
	shake_time = DET_TIMEOUT_TICK;
	do_div(shake_time, 1000);
	if (vic_det->back_in_det_gpio_2 != (-RT_ERROR)) {
		rt_timer_init(&vic_det->back_timer_2, "back_det2",
			__back_det_handler_fun_2, vic_det,
			shake_time,
			RT_TIMER_FLAG_DEACTIVATED |
			RT_TIMER_FLAG_ONE_SHOT |
			RT_TIMER_FLAG_HARD_TIMER);
		ret = pinctrl_gpio_request_irq(vic_det->pctrl,
			vic_det->back_in_det_gpio_2, &vic_det->back_in_det_gpio_irq_2);
		if (ret != RT_EOK) {
			LOG_E("Failed to request back det irq_2");
			goto off_2;
		}
		/* leave = pinctrl_gpio_get_value(vic_det->pctrl,
						vic_det->back_in_det_gpio); */
		if (leave) {
			vic_det->back_status_2 = LB_SYSMSG_BACK_OFF_2;
			lb_system_mq_send(LB_SYSMSG_BACK_OFF_2, NULL, 0, 0);
			LOG_D("LB_SYSMSG_BACK_OFF_2");
		} else {
			vic_det->back_status_2 = LB_SYSMSG_BACK_ON_2;
			lb_system_mq_send(LB_SYSMSG_BACK_ON_2, NULL, 0, 0);
			LOG_D("LB_SYSMSG_BACK_ON_2");
		}
	}
off_2:

	vic_det->back_in_det_gpio_irq_3.handler = __back_in_det_irq_handler_3;
	vic_det->back_in_det_gpio_irq_3.irq_arg = vic_det;
	vic_det->back_in_det_gpio_irq_3.clock_src = GPIO_IRQ_HOSC_24MHZ;
	vic_det->back_in_det_gpio_irq_3.clock_src_div = 1;
	leave = 0;
	leave = pinctrl_gpio_get_value(vic_det->pctrl, vic_det->back_in_det_gpio_3);
	if (leave)
		vic_det->back_in_det_gpio_irq_3.trig_type = EINT_TRIG_LOW_LEVEL;
	else
		vic_det->back_in_det_gpio_irq_3.trig_type = EINT_TRIG_HIGH_LEVEL;
	shake_time = DET_TIMEOUT_TICK;
	do_div(shake_time, 1000);
	if (vic_det->back_in_det_gpio_3 != (-RT_ERROR)) {
		rt_timer_init(&vic_det->back_timer_3, "back_det3",
			__back_det_handler_fun_3, vic_det,
			shake_time,
			RT_TIMER_FLAG_DEACTIVATED |
			RT_TIMER_FLAG_ONE_SHOT |
			RT_TIMER_FLAG_HARD_TIMER);
		ret = pinctrl_gpio_request_irq(vic_det->pctrl,
			vic_det->back_in_det_gpio_3, &vic_det->back_in_det_gpio_irq_3);
		if (ret != RT_EOK) {
			LOG_E("Failed to request back det irq_3");
			goto off_3;
		}
		/* leave = pinctrl_gpio_get_value(vic_det->pctrl,
						vic_det->back_in_det_gpio); */
		if (leave) {
			vic_det->back_status_3 = LB_SYSMSG_BACK_OFF_3;
			lb_system_mq_send(LB_SYSMSG_BACK_OFF_3, NULL, 0, 0);
			LOG_D("LB_SYSMSG_BACK_OFF_3");
		} else {
			vic_det->back_status_3 = LB_SYSMSG_BACK_ON_3;
			lb_system_mq_send(LB_SYSMSG_BACK_ON_3, NULL, 0, 0);
			LOG_D("LB_SYSMSG_BACK_ON_3");
		}
	}
off_3:

	vic_det->back_in_det_gpio_irq_4.handler = __back_in_det_irq_handler_4;
	vic_det->back_in_det_gpio_irq_4.irq_arg = vic_det;
	vic_det->back_in_det_gpio_irq_4.clock_src = GPIO_IRQ_HOSC_24MHZ;
	vic_det->back_in_det_gpio_irq_4.clock_src_div = 1;
	leave = 0;
	leave = pinctrl_gpio_get_value(vic_det->pctrl, vic_det->back_in_det_gpio_4);
	if (leave)
		vic_det->back_in_det_gpio_irq_4.trig_type = EINT_TRIG_LOW_LEVEL;
	else
		vic_det->back_in_det_gpio_irq_4.trig_type = EINT_TRIG_HIGH_LEVEL;
	shake_time = DET_TIMEOUT_TICK;
	do_div(shake_time, 1000);
	if (vic_det->back_in_det_gpio_4 != (-RT_ERROR)) {
		rt_timer_init(&vic_det->back_timer_4, "back_det4",
			__back_det_handler_fun_4, vic_det,
			shake_time,
			RT_TIMER_FLAG_DEACTIVATED |
			RT_TIMER_FLAG_ONE_SHOT |
			RT_TIMER_FLAG_HARD_TIMER);
		ret = pinctrl_gpio_request_irq(vic_det->pctrl,
			vic_det->back_in_det_gpio_4, &vic_det->back_in_det_gpio_irq_4);
		if (ret != RT_EOK) {
			LOG_E("Failed to request back det irq_4");
			goto off_4;
		}
		/* leave = pinctrl_gpio_get_value(vic_det->pctrl,
						vic_det->back_in_det_gpio); */
		if (leave) {
			vic_det->back_status_4 = LB_SYSMSG_BACK_OFF_4;
			lb_system_mq_send(LB_SYSMSG_BACK_OFF_4, NULL, 0, 0);
			LOG_D("LB_SYSMSG_BACK_OFF_4");
		} else {
			vic_det->back_status_4 = LB_SYSMSG_BACK_ON_4;
			lb_system_mq_send(LB_SYSMSG_BACK_ON_4, NULL, 0, 0);
			LOG_D("LB_SYSMSG_BACK_ON_4");
		}
	}
off_4:

	return ret;
}

struct vic_det_dev *vic_det_dev_create(void)
{
	struct vic_det_dev *vic_det = RT_NULL;
	rt_err_t ret = 0, leave = 0;

	vic_det = rt_zalloc(sizeof(struct vic_det_dev));
	if (RT_NULL == vic_det)
		return RT_NULL;
	ret = __vic_det_dev_config(vic_det);
	if (RT_EOK != ret) {
		rt_free(vic_det);
		return RT_NULL;
	}
	leave = __vic_det_init(vic_det);
	if (leave != RT_EOK) {
		LOG_E("__vic_det_init error");
		rt_free(vic_det);
		return RT_NULL;
	}
	present_status_1.back_status = vic_det->back_status_1;
	present_status_1.av_status = vic_det->av_status;
	present_status_2.back_status = vic_det->back_status_2;
	present_status_2.av_status = vic_det->av_status;
	present_status_3.back_status = vic_det->back_status_3;
	present_status_3.av_status = vic_det->av_status;
	present_status_4.back_status = vic_det->back_status_4;
	present_status_4.av_status = vic_det->av_status;
	return vic_det;
}

void vic_det_dev_destory(struct vic_det_dev *vic_det)
{
	if (RT_NULL != vic_det) {
		__vic_det_dev_unconfig(vic_det);
		rt_free(vic_det);
	}
	rt_timer_detach(&vic_det->back_timer_1);
	present_status_1.back_status = LB_SYSMSG_BACK_OFF;
	present_status_1.av_status = LB_SYSMSG_AV_PLUGOUT;
	rt_timer_detach(&vic_det->back_timer_2);
	present_status_2.back_status = LB_SYSMSG_BACK_OFF;
	present_status_2.av_status = LB_SYSMSG_AV_PLUGOUT;
	rt_timer_detach(&vic_det->back_timer_3);
	present_status_3.back_status = LB_SYSMSG_BACK_OFF;
	present_status_3.av_status = LB_SYSMSG_AV_PLUGOUT;
	rt_timer_detach(&vic_det->back_timer_4);
	present_status_4.back_status = LB_SYSMSG_BACK_OFF;
	present_status_4.av_status = LB_SYSMSG_AV_PLUGOUT;

}

struct vic_det_status vic_det_dev_get_sta(void)
{
	if ((0 == present_status_1.back_status) || (0 == present_status_1.av_status))
		LOG_E("vic det not be create");
	/* LOG_E("present_status.av_status:%x",present_status.av_status); */
	return present_status_1;
}
rt_err_t vic_det_dev_control(struct vic_det_dev *vic_det, rt_int32_t cmd,
							void *para)
{
	rt_err_t ret = RT_EOK;
	return ret;
}

void vic_det_dev_suspend(struct vic_det_dev *vic_det)
{
	LOG_D("vic_det dev suspend");
}

void vic_det_dev_resume(struct vic_det_dev *vic_det)
{
	LOG_D("vic_det dev resume");
}

#else
#ifdef ARCH_LOMBO_N7V1_CDR

#define DBG_LEVEL		DBG_ERROR

#include "vic_det_dev.h"
#include "vic_det_drv.h"
#include "media_dev.h"
#include "spinlock.h"
#include "gpio/pinctrl.h"
#include <div.h>

#define VIC_DET_GPIO_MODULE	"vic_det"
#define VIC_AV_IN_DET           "av_in_det"
#define VIC_AV_DMS_DET          "av_dms_det"
#define VIC_CAR_BACK_DET        "back_det"

#define DET_MS		130
#define DET_TIMEOUT_TICK		(DET_MS*RT_TICK_PER_SECOND)

struct vic_det_status present_status;
static int shake_cnt;
static int last_back_val;

static rt_err_t __vic_det_change(struct vic_det_dev *vic_det, rt_int32_t src,
								rt_int32_t type)
{
	rt_uint32_t ret = 0, temp = 0;
	/* LOG_E("__vic_det_change %d", type); */
	if (AV_IN_DET == src) {
#ifndef RT_USING_VIC_DET_SIGNAL
		if (EINT_TRIG_LOW_LEVEL == vic_det->av_in_det_gpio_irq.trig_type)
			vic_det->av_in_det_gpio_irq.trig_type = EINT_TRIG_HIGH_LEVEL;
		else if (EINT_TRIG_HIGH_LEVEL == vic_det->av_in_det_gpio_irq.trig_type)
			vic_det->av_in_det_gpio_irq.trig_type = EINT_TRIG_LOW_LEVEL;
		temp = pinctrl_gpio_set_irq_trig_type(vic_det->pctrl,
				vic_det->av_in_det_gpio, &vic_det->av_in_det_gpio_irq);
		if (temp != RT_EOK) {
			LOG_E("Failed to pinctrl_gpio_free_irq");
			return -1;
		}
#endif
	} else if (AV_DMS_DET == src) {
#ifndef RT_USING_VIC_DET_SIGNAL
#ifdef ARCH_LOMBO_N7V1_SAR
		if (EINT_TRIG_LOW_LEVEL == vic_det->av_dms_det_gpio_irq.trig_type)
			vic_det->av_dms_det_gpio_irq.trig_type = EINT_TRIG_HIGH_LEVEL;
		else if (EINT_TRIG_HIGH_LEVEL == vic_det->av_dms_det_gpio_irq.trig_type)
			vic_det->av_dms_det_gpio_irq.trig_type = EINT_TRIG_LOW_LEVEL;
		temp = pinctrl_gpio_set_irq_trig_type(vic_det->pctrl,
				vic_det->av_dms_det_gpio, &vic_det->av_dms_det_gpio_irq);
		if (temp != RT_EOK) {
			LOG_E("Failed to pinctrl_gpio_free_irq");
			return -1;
		}
#endif
#endif
	} else {
		if (1 == type) {
			if (EINT_TRIG_LOW_LEVEL ==
					vic_det->back_in_det_gpio_irq.trig_type)
				vic_det->back_in_det_gpio_irq.trig_type =
								EINT_TRIG_HIGH_LEVEL;
			else if (EINT_TRIG_HIGH_LEVEL ==
					vic_det->back_in_det_gpio_irq.trig_type)
				vic_det->back_in_det_gpio_irq.trig_type =
								EINT_TRIG_LOW_LEVEL;
			temp = pinctrl_gpio_set_irq_trig_type(vic_det->pctrl,
			vic_det->back_in_det_gpio, &vic_det->back_in_det_gpio_irq);
			if (temp != RT_EOK) {
				LOG_E("Failed to pinctrl_gpio_free_irq");
				return -1;
			}
		} else if (2 == type) {
			temp = pinctrl_gpio_set_irq_trig_type(vic_det->pctrl,
			vic_det->back_in_det_gpio, &vic_det->back_in_det_gpio_irq);
			if (temp != RT_EOK) {
				LOG_E("Failed to pinctrl_gpio_free_irq");
				return -1;
			}
		}
	}
	if (temp != RT_EOK)
		LOG_E("Failed to __vic_det_change");
	return ret;
}

#ifndef RT_USING_VIC_DET_SIGNAL
static void __av_in_det_handler_fun(void *param)
{
	struct vic_det_dev *vic_det = (struct vic_det_dev *)param;
	rt_uint32_t ret = 0, level;
	spin_lock_irqsave(&vic_det->av_det_spinlock, level);
	rt_timer_stop(&vic_det->av_timer);
	ret = pinctrl_gpio_get_value(vic_det->pctrl, vic_det->av_in_det_gpio);
	if (ret) {
		vic_det->av_status = LB_SYSMSG_AV_PLUGOUT;
		lb_system_mq_send(LB_SYSMSG_AV_PLUGOUT, NULL, 0, 0);
		LOG_E("LB_SYSMSG_AV_PLUGOUT");
	} else {
		vic_det->av_status = LB_SYSMSG_AV_PLUGIN;
		lb_system_mq_send(LB_SYSMSG_AV_PLUGIN, NULL, 0, 0);
		LOG_E("LB_SYSMSG_AV_PLUGIN");
	}
	present_status.av_status = vic_det->av_status;
	__vic_det_change(vic_det, AV_IN_DET, 0);
	spin_unlock_irqrestore(&vic_det->av_det_spinlock, level);
	/* LOG_E("__av_det_handler_fun "); */
}
#ifdef ARCH_LOMBO_N7V1_SAR
static void __av_dms_det_handler_fun(void *param)
{
	struct vic_det_dev *vic_det = (struct vic_det_dev *)param;
	rt_uint32_t ret = 0, level, last_status;
	spin_lock_irqsave(&vic_det->dms_det_spinlock, level);
	rt_timer_stop(&vic_det->dms_timer);
	last_status = vic_det->dms_status;
	ret = pinctrl_gpio_get_value(vic_det->pctrl, vic_det->av_dms_det_gpio);
	if (ret) {
		if (last_status != LB_SYSMSG_DMS_PLUGOUT) {
			vic_det->dms_status = LB_SYSMSG_DMS_PLUGOUT;
			lb_system_mq_send(LB_SYSMSG_DMS_PLUGOUT, NULL, 0, 0);
			LOG_E("LB_SYSMSG_DMS_PLUGOUT");
		}
	} else {
		if (last_status != LB_SYSMSG_DMS_PLUGIN) {
			vic_det->dms_status = LB_SYSMSG_DMS_PLUGIN;
			lb_system_mq_send(LB_SYSMSG_DMS_PLUGIN, NULL, 0, 0);
			LOG_E("LB_SYSMSG_DMS_PLUGIN");
		}
	}

	present_status.dms_status = vic_det->dms_status;
	__vic_det_change(vic_det, AV_DMS_DET, 0);
	spin_unlock_irqrestore(&vic_det->dms_det_spinlock, level);
	/* LOG_E("__av_det_handler_fun "); */
}
#endif
#endif

static void __back_det_handler_fun(void *param)
{
	struct vic_det_dev *vic_det = (struct vic_det_dev *)param;
	rt_uint32_t ret = 0, level, last_status;

	spin_lock_irqsave(&vic_det->back_det_spinlock, level);
	rt_timer_stop(&vic_det->back_timer);
	last_status = vic_det->back_status;
	ret = pinctrl_gpio_get_value(vic_det->pctrl, vic_det->back_in_det_gpio);
	if (ret != last_back_val)
		shake_cnt = 0;
	/* LOG_E("__back_det_handler_fun %d %d\n", shake_cnt,ret); */
	if (ret) {
		if (LB_SYSMSG_BACK_OFF != last_status) {
			shake_cnt++;
			if (shake_cnt >= 8) {
				vic_det->back_status = LB_SYSMSG_BACK_OFF;
				lb_system_mq_send(LB_SYSMSG_BACK_OFF, NULL, 0, 0);
				LOG_E("LB_SYSMSG_BACK_OFF");
				shake_cnt = 0;
				__vic_det_change(vic_det, BACK_IN_DET, 1);
				goto VIC_EIXT;
			}
		}
	} else {
		if (LB_SYSMSG_BACK_ON != last_status) {
			if (LB_SYSMSG_AV_PLUGIN == vic_det->av_status) {
				shake_cnt++;
				if (shake_cnt >= 1) {
					vic_det->back_status = LB_SYSMSG_BACK_ON;
					lb_system_mq_send(LB_SYSMSG_BACK_ON, NULL, 0, 0);
					LOG_E("LB_SYSMSG_BACK_ON");
					shake_cnt = 0;
					__vic_det_change(vic_det, BACK_IN_DET, 1);
					goto VIC_EIXT;
				}
			}
		}
	}
	__vic_det_change(vic_det, BACK_IN_DET, 2);
VIC_EIXT:
	present_status.back_status = vic_det->back_status;
	last_back_val = ret;
	spin_unlock_irqrestore(&vic_det->back_det_spinlock, level);
}

#ifndef RT_USING_VIC_DET_SIGNAL
static void __av_in_det_irq_handler(void *data)
{
	struct vic_det_dev *vic_det = (struct vic_det_dev *)data;
	rt_uint32_t level;
	spin_lock_irqsave(&vic_det->av_det_spinlock, level);
	pinctrl_gpio_irq_disable(vic_det->pctrl, vic_det->av_in_det_gpio);
	rt_timer_start(&vic_det->av_timer);
	spin_unlock_irqrestore(&vic_det->av_det_spinlock, level);
	/* LOG_E("__av_in_det_irq_handler "); */
}
#ifdef ARCH_LOMBO_N7V1_SAR
static void __av_dms_det_irq_handler(void *data)
{
	struct vic_det_dev *vic_det = (struct vic_det_dev *)data;
	rt_uint32_t level;
	spin_lock_irqsave(&vic_det->dms_det_spinlock, level);
	pinctrl_gpio_irq_disable(vic_det->pctrl, vic_det->av_dms_det_gpio);
	rt_timer_start(&vic_det->dms_timer);
	spin_unlock_irqrestore(&vic_det->dms_det_spinlock, level);
	/* LOG_E("__av_dms_det_irq_handler "); */
}
#endif
#endif

static void __back_in_det_irq_handler(void *data)
{
	struct vic_det_dev *vic_det = (struct vic_det_dev *)data;
	rt_uint32_t level;
	/* LOG_E("__back_in_det_irq_handler "); */
	spin_lock_irqsave(&vic_det->back_det_spinlock, level);
	pinctrl_gpio_irq_disable(vic_det->pctrl, vic_det->back_in_det_gpio);
	rt_timer_start(&vic_det->back_timer);
	spin_unlock_irqrestore(&vic_det->back_det_spinlock, level);
}
rt_err_t __vic_det_dev_config(struct vic_det_dev *vic_det)
{
	rt_err_t ret = 0;

	vic_det->pctrl = pinctrl_get(VIC_DET_GPIO_MODULE);
	if (RT_NULL == vic_det->pctrl) {
		LOG_E("pinctrl_get Failed");
		return -RT_ERROR;
	}
#ifndef RT_USING_VIC_DET_SIGNAL
	vic_det->av_in_det_gpio = -RT_ERROR;
	ret = config_get_u32_array(VIC_DET_GPIO_MODULE, VIC_AV_IN_DET,
				vic_det->av_in_det_pctrl,
				ARRAY_SIZE(vic_det->av_in_det_pctrl));
	if (ret != ARRAY_SIZE(vic_det->av_in_det_pctrl)) {
		LOG_E("vic_det: get av_in_det_gpio error. ret:%d", ret);
		return -RT_ERROR;
	} else {
		vic_det->av_in_det_gpio = pinctrl_gpio_request(vic_det->pctrl,
			vic_det->av_in_det_pctrl[0], vic_det->av_in_det_pctrl[1]);
		if (vic_det->av_in_det_gpio < 0) {
			LOG_E("Failed to request av_in_det_gpio");
			return -RT_ERROR;
		} else {
#if 1
			ret = pinctrl_gpio_set_pud_mode(vic_det->pctrl,
					vic_det->av_in_det_gpio, PULL_DOWN);
			if (ret != RT_EOK) {
				LOG_E("Can't set av_in_det_gpio pud");
				return -RT_ERROR;
			}
#endif
			ret = pinctrl_gpio_direction_input(vic_det->pctrl,
					vic_det->av_in_det_gpio);
			if (ret != RT_EOK) {
				LOG_E("Can't set av_in_det_gpio direction");
				return -RT_ERROR;
			}
		}

	}

#ifdef ARCH_LOMBO_N7V1_SAR
	vic_det->av_dms_det_gpio = -RT_ERROR;
	ret = config_get_u32_array(VIC_DET_GPIO_MODULE, VIC_AV_DMS_DET,
				vic_det->av_dms_det_pctrl,
				ARRAY_SIZE(vic_det->av_dms_det_pctrl));
	if (ret != ARRAY_SIZE(vic_det->av_dms_det_pctrl)) {
		LOG_E("vic_det: get av_dms_det_gpio error. ret:%d", ret);
		return -RT_ERROR;
	} else {
		vic_det->av_dms_det_gpio = pinctrl_gpio_request(vic_det->pctrl,
			vic_det->av_dms_det_pctrl[0], vic_det->av_dms_det_pctrl[1]);
		if (vic_det->av_dms_det_gpio < 0) {
			LOG_E("Failed to request av_dms_det_gpio");
			return -RT_ERROR;
		} else {
			ret = pinctrl_gpio_direction_input(vic_det->pctrl,
					vic_det->av_dms_det_gpio);
			if (ret != RT_EOK) {
				LOG_E("Can't set av_dms_det_gpio direction");
				return -RT_ERROR;
			}
		}

	}
#endif
#endif
	vic_det->back_in_det_gpio = -RT_ERROR;
	ret = config_get_u32_array(VIC_DET_GPIO_MODULE, VIC_CAR_BACK_DET,
				vic_det->back_in_det_pctrl,
				ARRAY_SIZE(vic_det->back_in_det_pctrl));
	if (ret != ARRAY_SIZE(vic_det->back_in_det_pctrl)) {
		LOG_E("vic_det: get back_in_det_gpio error. ret:%d", ret);
		return -RT_ERROR;
	} else {
		vic_det->back_in_det_gpio = pinctrl_gpio_request(vic_det->pctrl,
			vic_det->back_in_det_pctrl[0], vic_det->back_in_det_pctrl[1]);
		if (vic_det->back_in_det_gpio < 0) {
			LOG_E("Failed to request back_in_det_gpio");
			return -RT_ERROR;
		} else {
#if 0
			ret = pinctrl_gpio_set_pud_mode(vic_det->pctrl,
						vic_det->back_in_det_gpio, PULL_DOWN);
			if (ret != RT_EOK) {
				LOG_E("Can't set back_in_det_gpio pud");
				return -RT_ERROR;
			}
#endif
			ret = pinctrl_gpio_direction_input(vic_det->pctrl,
					vic_det->back_in_det_gpio);
			if (ret != RT_EOK) {
				LOG_E("Can't set back_in_det_gpio direction");
				return -RT_ERROR;
			}
		}

	}
	return RT_EOK;
}

static void __vic_det_dev_unconfig(struct vic_det_dev *vic_det)
{
	rt_err_t ret;
	if (RT_NULL != vic_det->pctrl) {
#ifndef RT_USING_VIC_DET_SIGNAL
		if (vic_det->av_in_det_gpio != (-RT_ERROR)) {
			ret = pinctrl_gpio_free_irq(vic_det->pctrl,
						vic_det->av_in_det_gpio);
			if (ret != RT_EOK)
				LOG_E("free av_in_det_gpio irq Failed");
		}
#ifdef ARCH_LOMBO_N7V1_SAR
		if (vic_det->av_dms_det_gpio != (-RT_ERROR)) {
			ret = pinctrl_gpio_free_irq(vic_det->pctrl,
						vic_det->av_dms_det_gpio);
			if (ret != RT_EOK)
				LOG_E("free av_dms_det_gpio irq Failed");
		}
#endif
#endif
		if (vic_det->back_in_det_gpio != (-RT_ERROR)) {
			ret = pinctrl_gpio_free_irq(vic_det->pctrl,
					vic_det->back_in_det_gpio);
			if (ret != RT_EOK)
				LOG_E("free back_in_det_gpio irq Failed");
		}
		pinctrl_put(vic_det->pctrl);
		vic_det->pctrl = RT_NULL;
	}
}

static rt_err_t __vic_det_init(struct vic_det_dev *vic_det)
{
	rt_err_t ret = 0, leave = 0;
	rt_tick_t shake_time = 0;
 #if 0 //////////
#ifndef RT_USING_VIC_DET_SIGNAL
	vic_det->av_in_det_gpio_irq.handler = __av_in_det_irq_handler;
	vic_det->av_in_det_gpio_irq.irq_arg = vic_det;
	vic_det->av_in_det_gpio_irq.clock_src = GPIO_IRQ_HOSC_24MHZ;
	vic_det->av_in_det_gpio_irq.clock_src_div = 1;
	leave = pinctrl_gpio_get_value(vic_det->pctrl, vic_det->av_in_det_gpio);
	if (leave)
		vic_det->av_in_det_gpio_irq.trig_type = EINT_TRIG_LOW_LEVEL;
	else
		vic_det->av_in_det_gpio_irq.trig_type = EINT_TRIG_HIGH_LEVEL;
	shake_time = DET_TIMEOUT_TICK;
	do_div(shake_time, 1000);
	if (vic_det->av_in_det_gpio != (-RT_ERROR)) {
		spin_lock_init(&vic_det->av_det_spinlock);
		ret = pinctrl_gpio_request_irq(vic_det->pctrl, vic_det->av_in_det_gpio,
			&vic_det->av_in_det_gpio_irq);
		if (ret != RT_EOK) {
			LOG_E("Failed to request av det irq");
			goto off;
		}
		rt_timer_init(&vic_det->av_timer, "av_det",
			__av_in_det_handler_fun, vic_det,
			shake_time,
			RT_TIMER_FLAG_DEACTIVATED |
			RT_TIMER_FLAG_ONE_SHOT |
			RT_TIMER_FLAG_HARD_TIMER);
		/* leave = pinctrl_gpio_get_value(vic_det->pctrl,
						vic_det->av_in_det_gpio); */
		if (leave) {
			vic_det->av_status = LB_SYSMSG_AV_PLUGOUT;
			lb_system_mq_send(LB_SYSMSG_AV_PLUGOUT, NULL, 0, 0);
			LOG_E("LB_SYSMSG_AV_PLUGOUT");
		} else {
			vic_det->av_status = LB_SYSMSG_AV_PLUGIN;
			lb_system_mq_send(LB_SYSMSG_AV_PLUGIN, NULL, 0, 0);
			LOG_E("LB_SYSMSG_AV_PLUGIN");
		}
	}

#ifdef ARCH_LOMBO_N7V1_SAR
	vic_det->av_dms_det_gpio_irq.handler = __av_dms_det_irq_handler;
	vic_det->av_dms_det_gpio_irq.irq_arg = vic_det;
	vic_det->av_dms_det_gpio_irq.clock_src = GPIO_IRQ_HOSC_24MHZ;
	vic_det->av_dms_det_gpio_irq.clock_src_div = 1;
	leave = pinctrl_gpio_get_value(vic_det->pctrl, vic_det->av_dms_det_gpio);
	if (leave)
		vic_det->av_dms_det_gpio_irq.trig_type = EINT_TRIG_LOW_LEVEL;
	else
		vic_det->av_dms_det_gpio_irq.trig_type = EINT_TRIG_HIGH_LEVEL;
	shake_time = DET_TIMEOUT_TICK;
	do_div(shake_time, 1000);
	if (vic_det->av_dms_det_gpio != (-RT_ERROR)) {
		spin_lock_init(&vic_det->dms_det_spinlock);
		ret = pinctrl_gpio_request_irq(vic_det->pctrl, vic_det->av_dms_det_gpio,
			&vic_det->av_dms_det_gpio_irq);
		if (ret != RT_EOK) {
			LOG_E("Failed to request dms det irq");
			goto off;
		}
		rt_timer_init(&vic_det->dms_timer, "dms_det",
			__av_dms_det_handler_fun, vic_det,
			shake_time,
			RT_TIMER_FLAG_DEACTIVATED |
			RT_TIMER_FLAG_ONE_SHOT |
			RT_TIMER_FLAG_HARD_TIMER);
		/* leave = pinctrl_gpio_get_value(vic_det->pctrl,
						vic_det->av_dms_det_gpio); */
		if (leave) {
			vic_det->dms_status = LB_SYSMSG_DMS_PLUGOUT;
			lb_system_mq_send(LB_SYSMSG_DMS_PLUGOUT, NULL, 0, 0);
			LOG_E("LB_SYSMSG_DMS_PLUGOUT");
		} else {
			vic_det->dms_status = LB_SYSMSG_DMS_PLUGIN;
			lb_system_mq_send(LB_SYSMSG_DMS_PLUGIN, NULL, 0, 0);
			LOG_E("LB_SYSMSG_DMS_PLUGIN");
		}
	}
#endif
#else
	vic_det->av_status = LB_SYSMSG_AV_PLUGIN;
#ifdef ARCH_LOMBO_N7V1_SAR
	vic_det->dms_status = LB_SYSMSG_DMS_PLUGIN;
#endif
#endif

#else /////////////
		vic_det->av_status = LB_SYSMSG_AV_PLUGIN;
		lb_system_mq_send(LB_SYSMSG_AV_PLUGIN, NULL, 0, 0);
		LOG_E("LB_SYSMSG_AV_PLUGIN");
#endif

	vic_det->back_in_det_gpio_irq.handler = __back_in_det_irq_handler;
	vic_det->back_in_det_gpio_irq.irq_arg = vic_det;
	vic_det->back_in_det_gpio_irq.clock_src = GPIO_IRQ_HOSC_24MHZ;
	vic_det->back_in_det_gpio_irq.clock_src_div = 1;
	leave = 0;
	leave = pinctrl_gpio_get_value(vic_det->pctrl, vic_det->back_in_det_gpio);
	if (leave)
		vic_det->back_in_det_gpio_irq.trig_type = EINT_TRIG_LOW_LEVEL;
	else
		vic_det->back_in_det_gpio_irq.trig_type = EINT_TRIG_HIGH_LEVEL;
	shake_time = DET_TIMEOUT_TICK;
	do_div(shake_time, 1000);
	if (vic_det->back_in_det_gpio != (-RT_ERROR)) {
		spin_lock_init(&vic_det->back_det_spinlock);
		ret = pinctrl_gpio_request_irq(vic_det->pctrl, vic_det->back_in_det_gpio,
			&vic_det->back_in_det_gpio_irq);
		if (ret != RT_EOK) {
			LOG_E("Failed to request back det irq");
			goto off;
		}
		rt_timer_init(&vic_det->back_timer, "back_det",
			__back_det_handler_fun, vic_det,
			shake_time,
			RT_TIMER_FLAG_DEACTIVATED |
			RT_TIMER_FLAG_ONE_SHOT |
			RT_TIMER_FLAG_HARD_TIMER);
		/* leave = pinctrl_gpio_get_value(vic_det->pctrl,
						vic_det->back_in_det_gpio); */
		if (leave) {
			vic_det->back_status = LB_SYSMSG_BACK_OFF;
			lb_system_mq_send(LB_SYSMSG_BACK_OFF, NULL, 0, 0);
			LOG_D("LB_SYSMSG_BACK_OFF");
		} else {
			vic_det->back_status = LB_SYSMSG_BACK_ON;
			lb_system_mq_send(LB_SYSMSG_BACK_ON, NULL, 0, 0);
			LOG_D("LB_SYSMSG_BACK_ON");
		}
	}
off:
	return ret;

	
}


struct vic_det_dev *vic_det_dev_create(void)
{
	struct vic_det_dev *vic_det = RT_NULL;
	rt_err_t ret = 0, leave = 0;

	vic_det = rt_zalloc(sizeof(struct vic_det_dev));
	if (RT_NULL == vic_det)
		return RT_NULL;
	ret = __vic_det_dev_config(vic_det);
	if (RT_EOK != ret) {
		rt_free(vic_det);
		return RT_NULL;
	}
	leave = __vic_det_init(vic_det);
	if (leave != RT_EOK) {
		LOG_E("__vic_det_init error");
		rt_free(vic_det);
		return RT_NULL;
	}
	present_status.back_status = vic_det->back_status;
	present_status.av_status = vic_det->av_status;
#ifdef ARCH_LOMBO_N7V1_SAR
	present_status.dms_status = vic_det->dms_status;
#endif
	return vic_det;
}

void vic_det_dev_destory(struct vic_det_dev *vic_det)
{
	if (RT_NULL != vic_det) {
		__vic_det_dev_unconfig(vic_det);
		rt_free(vic_det);
	}
	rt_timer_detach(&vic_det->av_timer);
	rt_timer_detach(&vic_det->back_timer);
	present_status.back_status = LB_SYSMSG_BACK_OFF;
	present_status.av_status = LB_SYSMSG_AV_PLUGOUT;
#ifdef ARCH_LOMBO_N7V1_SAR
	present_status.dms_status = LB_SYSMSG_DMS_PLUGOUT;
#endif

}

struct vic_det_status vic_det_dev_get_sta(void)
{
	if ((0 == present_status.back_status) || (0 == present_status.av_status))
		LOG_E("vic det not be create");
	/* LOG_E("present_status.av_status:%x",present_status.av_status); */
	return present_status;
}
rt_err_t vic_det_dev_control(struct vic_det_dev *vic_det, rt_int32_t cmd,
							void *para)
{
	rt_err_t ret = RT_EOK;
	return ret;
}

void vic_det_dev_suspend(struct vic_det_dev *vic_det)
{
	LOG_D("vic_det dev suspend");
}

void vic_det_dev_resume(struct vic_det_dev *vic_det)
{
	LOG_D("vic_det dev resume");
}
#endif
#endif
