/*
 * panel.c - Panel module common driver code for LomboTech
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
#include "panel.h"
#include <pwm.h>

#define DBG_SECTION_NAME	"PANEL_COMMON"
#define DBG_LEVEL		DBG_ERROR
#include <debug.h>

#define GPIO_UNINITAL_VAL	-1

static int rst_gpio, bl_gpio, rglt_gpio, vref_gpio;

static struct pwm_device *bl_pwm;
static rt_uint32_t pwm_para[3]; /*min duty, max duty, period */

static tcon_host_t device_tcon_host;
static vo_device_t device_vo_dev;

static struct list_head panel_list;
struct list_head *panel_list_head;

/* panel request io */
int panel_io_request(const char *module_name, const char *group_name, int *rq_gpio)
{
	int ret = RT_EOK;
	int array_num;
	struct pinctrl *pctrl;
	rt_int32_t gpio;	/* reset io */
	rt_uint32_t rst_val[7];	/*port, pin, function, drv_level, pud, pud_res, data*/

	pctrl = pinctrl_get(module_name);
	if (RT_NULL == pctrl) {
		LOG_E("disp pinctrl_get err");
		return RT_ERROR;
	}
	array_num = config_get_u32_array(module_name, group_name,
				rst_val, ARRAY_SIZE(rst_val));
	if (array_num != ARRAY_SIZE(rst_val)) {
		ret = RT_ERROR;
		LOG_E("config_get_u32_array error. array_num:%d", array_num);
		goto exit;
	}

	gpio = pinctrl_gpio_request(pctrl, rst_val[0], rst_val[1]);

	if (gpio >= 0) {
		pinctrl_gpio_set_function(pctrl, gpio, rst_val[2]);
		pinctrl_gpio_set_drv_level(pctrl, gpio, rst_val[3]);
		pinctrl_gpio_set_pud_mode(pctrl, gpio, rst_val[4]);
		pinctrl_gpio_set_pud_res(pctrl, gpio, rst_val[5]);
	} else {
		ret = RT_ERROR;
		LOG_E("pinctrl_gpio_request error. ret:%d", ret);
		goto exit;
	}

	*rq_gpio = gpio;

exit:
	return ret;
}

/* panel release io */
int panel_io_release(const char *module_name, const char *group_name, int rl_gpio)
{
	int ret = RT_EOK;
	struct pinctrl *pctrl;

	pctrl = pinctrl_get(module_name);
	if (RT_NULL == pctrl) {
		LOG_E("disp pinctrl_get err");
		return RT_ERROR;
	}

	ret = pinctrl_gpio_free(pctrl, rl_gpio);

	return ret;
}

int panel_rst_io_init(const char *module_name, const char *group_name)
{
	rst_gpio = GPIO_UNINITAL_VAL;
	return panel_io_request(module_name, group_name, &rst_gpio);
}

int panel_rst_io_exit(const char *module_name, const char *group_name)
{
	if (GPIO_UNINITAL_VAL == rst_gpio)
		return RT_ERROR;

	return panel_io_release(module_name, group_name, rst_gpio);
}

int panel_bl_io_init(const char *module_name, const char *group_name)
{
	bl_gpio = GPIO_UNINITAL_VAL;
	return panel_io_request(module_name, group_name, &bl_gpio);
}

int panel_bl_io_exit(const char *module_name, const char *group_name)
{
	if (GPIO_UNINITAL_VAL == bl_gpio)
		return RT_ERROR;

	return panel_io_release(module_name, group_name, bl_gpio);
}

int panel_rglt_io_init(const char *module_name, const char *group_name)
{
	rglt_gpio = GPIO_UNINITAL_VAL;
	return panel_io_request(module_name, group_name, &rglt_gpio);
}

int panel_rglt_io_exit(const char *module_name, const char *group_name)
{
	if (GPIO_UNINITAL_VAL == rglt_gpio)
		return RT_ERROR;

	return panel_io_release(module_name, group_name, rglt_gpio);
}

int panel_vref_io_init(const char *module_name, const char *group_name)
{
	vref_gpio = GPIO_UNINITAL_VAL;
	return panel_io_request(module_name, group_name, &vref_gpio);
}

int panel_vref_io_exit(const char *module_name, const char *group_name)
{
	if (GPIO_UNINITAL_VAL == vref_gpio)
		return RT_ERROR;

	return panel_io_release(module_name, group_name, vref_gpio);
}

int panel_io_set(const char *module_name, u32 value, int gpio)
{
	struct pinctrl *pctrl;

	pctrl = pinctrl_get(module_name);
	if (RT_NULL == pctrl) {
		LOG_E("disp pinctrl_get err");
		return RT_ERROR;
	}

	if (gpio <= 0) {
		LOG_E("illegal rst_gpio");
		return RT_ERROR;
	}

	if (pinctrl_gpio_set_value(pctrl, gpio, value) != RT_EOK) {
		LOG_E("pinctrl_gpio_set_value err");
		return RT_ERROR;
	}

	return RT_EOK;
}

int panel_io_rst_set(const char *module_name, u32 value)
{
	if (rst_gpio <= 0) {
		LOG_E("illegal rst_gpio");
		return RT_ERROR;
	}

	return panel_io_set(module_name, value, rst_gpio);
}

int panel_io_bl_set(const char *module_name, u32 value)
{
	if (bl_gpio <= 0) {
		LOG_E("illegal bl_gpio");
		return RT_ERROR;
	}

	return panel_io_set(module_name, value, bl_gpio);
}

int panel_io_rglt_set(const char *module_name, u32 value)
{
	if (rglt_gpio <= 0) {
		LOG_E("illegal rglt_gpio");
		return RT_ERROR;
	}

	return panel_io_set(module_name, value, rglt_gpio);
}

int panel_io_vref_set(const char *module_name, u32 value)
{
	if (vref_gpio <= 0) {
		LOG_E("illegal vref_gpio");
		return RT_ERROR;
	}

	return panel_io_set(module_name, value, vref_gpio);
}

void panel_pwm_init(const char *module_name)
{
	int ret;
	struct pinctrl *pctrl;
	int array_num;

	pctrl = pinctrl_get(module_name);
	if (RT_NULL == pctrl) {
		LOG_E("disp pinctrl_get err");
		return;
	}

	array_num = config_get_u32_array(module_name, DISP_BL_PWM_RANGE,
				pwm_para, ARRAY_SIZE(pwm_para));
	if (array_num != ARRAY_SIZE(pwm_para)) {
		LOG_E("config_get_u32_array error. array_num:%d", array_num);
		return;
	}

	LOG_D("array_num[%d] min[%d] max[%d] period[%d]", array_num,
				pwm_para[0], pwm_para[1], pwm_para[2]);

	ret = pinctrl_enable_group(pctrl, DISP_GPIO_BL_PWM_GROUP);
	if (ret != RT_EOK) {
		LOG_E("pinctrl_enable_group[%s] err", DISP_GPIO_BL_PWM_GROUP);
		return;
	}

	bl_pwm = pwm_cfg_request(module_name, DISP_BL_PWM_GROUP);
	if (NULL == bl_pwm) {
		LOG_E("pwm_cfg_request[%s] err", DISP_BL_PWM_GROUP);
		return;
	}
	//pwm_enable(bl_pwm);
}

void panel_pwm_exit(const char *module_name)
{
	int ret;
	struct pinctrl *pctrl;

	pctrl = pinctrl_get(module_name);
	if (RT_NULL == pctrl) {
		LOG_E("disp pinctrl_get err");
		return;
	}

	pwm_disable(bl_pwm);
	pwm_free(bl_pwm);
	bl_pwm = NULL;

	ret = pinctrl_disable_group(pctrl, DISP_GPIO_BL_PWM_GROUP);
	if (ret != RT_EOK) {
		LOG_E("pinctrl_disable_group[%s] err", DISP_GPIO_BL_PWM_GROUP);
		return;
	}
}

int panel_set_backlight_value(const char *module_name, u32 value)
{
	int ret;
	u32 duty_min = pwm_para[0], duty_max = pwm_para[1], duty_period = 0;
	u32 step = (duty_max - duty_min) / 100;

	if (NULL == bl_pwm)
		return RT_ERROR;

	if (value >= 100)
		value = 100;

	//duty_period = duty_max - step * value;
	duty_period = duty_min + step * value;
	ret = pwm_config(bl_pwm, duty_period, pwm_para[2]);
   // printf("\nduty_period=========%d\n",duty_period);
	return ret;
}

int panel_set_backlight_state(bool state)
{
	if (state) {
		if (bl_pwm)
			pwm_enable(bl_pwm);
		else
			return RT_ERROR;
	} else {
		if (bl_pwm)
			pwm_disable(bl_pwm);
		else
			return RT_ERROR;
	}

	return RT_EOK;
}

s32 mipi_dsi_dcs_write_array(u32 index, u8 vc, u8 *reg, u32 len, u8 delay_flag)
{
	u32 i = 0;
	while (i < len) {
		if (delay_flag == reg[i]) {
			udelay(reg[i + 1] * 1000);
			i += reg[i] + 2;
		} else if (1 == reg[i]) {
			csp_mipi_dsi_dcs_sw_0p(index, vc, reg[i + 1]);
			i += reg[i] + 1;
		} else if (2 == reg[i]) {
			csp_mipi_dsi_dcs_sw_1p(index, vc,
				reg[i + 1], reg[i + 2]);
			i += reg[i] + 1;
		} else {
			csp_mipi_dsi_dcs_lw(index, vc, &reg[i + 1], reg[i]);
			i += reg[i] + 1;
		}
	}
	/* this delay may be very important for some panel such as panel zt1180 */
	udelay(100 * 1000);
	return 0;
}

/* find the finally valid panel */
int panel_find_valid(disp_panel_ctl_t **pctl_p)
{
	bool panel_exit = false;
	disp_panel_ctl_t *pctl = NULL;

	if (NULL == panel_list_head) {
		LOG_E("panel_list_head is NULL");
		return RT_ERROR;
	}

	list_for_each_entry(pctl, panel_list_head, node) {
		if (pctl->is_okay) {
			panel_exit = true;
			*pctl_p = pctl;
			rt_kprintf("panel_find_valid: panel[%s] width[%d] height[%d]\n",
					pctl->name, pctl->width, pctl->height);
			break;
		}
	}

	if (!panel_exit) {
		LOG_E("not panel exit");
		return RT_ERROR;
	}

	return RT_EOK;
}

int panel_dev_probe(vo_device_t **vo_dev)
{
	int ret = RT_EOK;
	disp_panel_ctl_t *pctl = NULL;
	vo_dev_ops_t *dev_ops;

	ret = panel_find_valid(&pctl);
	if (ret != RT_EOK) {
		LOG_E("fail to find valid panel");
		return ret;
	}

	/* struct init */
	memset(&device_vo_dev, 0, sizeof(vo_device_t));
	memset(&device_tcon_host, 0, sizeof(tcon_host_t));

	*vo_dev = &device_vo_dev;
	device_vo_dev.tcon_host = &device_tcon_host;
	dev_ops = &(device_vo_dev.dev_ops);

	dev_ops->dev_init = pctl->ops.dev_init;
	dev_ops->dev_exit = pctl->ops.dev_exit;
	dev_ops->dev_set_backlight_state = pctl->ops.dev_set_backlight_state;
	dev_ops->dev_set_backlight_value = pctl->ops.dev_set_backlight_value;

	pctl->ops.dev_config(&device_vo_dev);

	return ret;
}

void panel_dev_disprobe(void)
{
	/* struct uninit */
	memset(&device_vo_dev, 0, sizeof(vo_device_t));
	memset(&device_tcon_host, 0, sizeof(tcon_host_t));
}

void panel_get_dev(vo_device_t **vo_dev)
{
	*vo_dev = &device_vo_dev;
}

int panel_get_size(u32 *width, u32 *height)
{
	u32 w = 0, h = 0;
	disp_panel_ctl_t *pctl = NULL;

	list_for_each_entry(pctl, panel_list_head, node) {
		if (pctl->is_okay) {
			w = pctl->width;
			h = pctl->height;
		}
	}

	if (w <= 0 || h <= 0)
		return RT_ERROR;

	*width = w;
	*height = h;

	return RT_EOK;
}

const char *panel_get_module_name(void)
{
	const char *module_name = NULL;
	disp_panel_ctl_t *pctl = NULL;

	list_for_each_entry(pctl, panel_list_head, node) {
		if (pctl->is_okay) {
			module_name = pctl->name;
			break;
		}
	}

	return module_name;
}

static int rt_panel_init(void)
{
	panel_list_head = &panel_list;
	INIT_LIST_HEAD(panel_list_head);

	return 0;
}

void panel_status_init(const char *module_name, disp_panel_ctl_t *pctl)
{
	const char *strings[1];
	int count;

	pctl->is_okay = false;
	pctl->name = NULL;

	count = config_get_string_array(module_name, PANEL_STATUS, strings, 1);
	if (count != 1) {
		rt_kprintf("get panel[%s] msg error\n", module_name);
		return;
	}

	if (strcmp(strings[0], "okay") == 0) {
		rt_kprintf("panel[%s] status:%s\n", module_name, strings[0]);
		pctl->is_okay = true;
		pctl->name = module_name;
		return;
	}

	rt_kprintf("panel[%s] is diabled\n", module_name);
}

INIT_DEVICE_EXPORT(rt_panel_init);
