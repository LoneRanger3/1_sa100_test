/*
 * power_drv.c - power driver module realization
 *
 * Copyright (C) 2016-2019, LomboTech Co.Ltd.
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
#include <unistd.h>
#include <irq_numbers.h>
#include "clk/clk.h"
#include "power_drv.h"
#include "gpadc_drv.h"
#include "../rtc/rtc_csp.h"
#include "system/system_mq.h"
#include "config_api.h"
#include "gpio/pinctrl.h"
#include "stdlib.h"
#include <boot_param.h>

#define LOMBO_ACC_SIO5


#define PWR_GPADC_AUX_INDEX		1	/* use gpadc1 for bat power */
#define SAVE_CHARGE_STATUS_BIT		0	/* bit for save charge status */
#define PWR_KEY_EVENTS_PER_PACKET	2

#define BAT_HIGH_TH_DEFAULT		2000//3900
#define BAT_MIDDLE_TH_DEFAULT	1640	//3810

#define AAC_SIO_PORT			GPIO_PORT_SIO
#define AAC_SIO_PIN			GPIO_PIN_5


#define SD_SIO_PORT			GPIO_PORT_E
#define SD_SIO_PIN			GPIO_PIN_14

static struct pinctrl *sd_pin_ctrl = RT_NULL;
static int sd_pin_num = -1;



static struct input_dev _pwr_k_dev;
static int _gs_boot;
#ifdef LOMBO_ACC_SIO5
static struct pinctrl *_aac_pin_ctrl = RT_NULL;
static int _aac_pin_num = -1;
#endif

static u32 _bat_high_th;
static u32 _bat_middle_th;
#define BAT_HIGH_16V		2350  //high power

/* quick shutdown */
#define QUICK_SHUTDOWN_GPADC_TH		2450//3840

static int disable_reset;
static rt_mutex_t pwr_mutex;

#ifndef LOMBO_ACC_SIO5
	static	int acc_test_value = 1;
#else
	static struct rt_timer sio5_timer;
	static int is_acc_line;
	static int sio5_lower_bat_num;
	static int sio5_lower_bat_cnt;
#endif

/* get battery voltage from gpadc1 value */
static float get_bat_voltage()
{
	reg_gpadc_ain1dat_t dat_reg1;
	u32 range = 4095;	/* gpadc 12bit for 3.3v */

	dat_reg1.val = READREG32(VA_GPADC_AIN1DAT);
	LOG_D("VA_GPADC_AIN1DAT dat = %d", dat_reg1.bits.dat);

	/* 1.03 base on the actual measurement value */
	float gpadc_v = (float)dat_reg1.bits.dat / range * 3.3 * 1.03;
	LOG_FLOAT("GPADC1 Voltage: %.2f", gpadc_v);

	/* 1.7 base on the actual measurement value */
	float bat_v = gpadc_v * 1.7;
	return bat_v;
}

/*
 * bat_low_power_irq - GPADC AUXIN interrupt handler, for low power prompt
 * @vector: index in isr_table[]
 * @param: parament for handle
 *
 */
static void bat_low_power_irq(int vector, void *param)
{
	if (csp_aux_get_th_pend(PWR_GPADC_AUX_INDEX)) {
		LOG_D("Low power tip ......");
		csp_aux_clr_th_pend(PWR_GPADC_AUX_INDEX);

		/* only prompt once */
		csp_aux_set_th_en(PWR_GPADC_AUX_INDEX, RT_FALSE);

		/* to do: send low power message to application */
	}
}

/* set the charge status */
static void set_charge_status(rt_bool_t charging)
{
	/* use alive_reg0 register 0 bit to save charge status */
	reg_rtc_rtc_alive_reg0_t reg;
	reg.val = READREG32(VA_RTC_RTC_ALIVE_REG0);

	if (charging)
		reg.val |= BIT(SAVE_CHARGE_STATUS_BIT);
	else
		reg.val &= ~(BIT(SAVE_CHARGE_STATUS_BIT));

	WRITEREG32(VA_RTC_RTC_ALIVE_REG0, reg.val);
}

static rt_bool_t get_pwr_conn_stat()
{
	u32 val;

	val = csp_rtc_get_pwr_con();
	if (val)
		return RT_TRUE;
	return RT_FALSE;
}

static rt_bool_t get_pwr_conn_stat_legacy()
{
	int connected = RT_FALSE;
	u32 pin_val = 0;
	reg_rtc_sio_func_r0_t sf_reg, old_sf_reg;
	reg_rtc_sio_data_r0_t d_reg;
	rt_err_t ret;

	ret = rt_mutex_take(pwr_mutex, RT_WAITING_FOREVER);
	if (ret != RT_EOK) {
		LOG_E("pwr_mutex rt_mutex_take error");
	}

	/* change pwr_wake function to GPIO INPUT */
	sf_reg.val = readl(VA_RTC_SIO_FUNC_R0);
	old_sf_reg.val = sf_reg.val;

	/* SIO0 1: INPUT, 3: PWR_WAKE */
	sf_reg.bits.sio0 = 1;
	writel(sf_reg.val, VA_RTC_SIO_FUNC_R0);

	/* get GPIO DATA register value */
	d_reg.val = readl(VA_RTC_SIO_DATA_R0);
	pin_val = d_reg.val & 0x1;

	/* get actual usb power connect status */
	if (pin_val)
		connected = RT_TRUE;

	/* set back to the original value */
	old_sf_reg.bits.sio0 = 3;
	writel(old_sf_reg.val, VA_RTC_SIO_FUNC_R0);
	//LOG_E("%d", connected);

	if (ret == RT_EOK) {
		rt_mutex_release(pwr_mutex);
	}

	return connected;
}

/* get the charge status */
static rt_bool_t get_charge_status()
{
	u32 p1, p2;
	RT_ASSERT(!lombo_func2(&p1, &p2));
	if (1 == p1)
		return get_pwr_conn_stat_legacy();
	else
		return get_pwr_conn_stat();
}

static void pwr_gpadc_cfg()
{
	csp_aux_set_en(PWR_GPADC_AUX_INDEX, RT_TRUE);
	/* set GPADC1 threshold value for low power interrupt notify */
	csp_aux_set_th_data(PWR_GPADC_AUX_INDEX, _bat_middle_th);
	/* set interrupt enable */
	csp_aux_set_th_en(PWR_GPADC_AUX_INDEX, RT_TRUE);

	rt_hw_interrupt_install(INT_GPADC, bat_low_power_irq,
		RT_NULL, "low_power_irq");
	rt_hw_interrupt_umask(INT_GPADC);
}

/* register power key as an input device to input core */
static rt_err_t register_power_key()
{
	rt_err_t ret;

	_pwr_k_dev.name = "cdr_power_key";
	rt_list_init(&(_pwr_k_dev.node));
	rt_list_init(&(_pwr_k_dev.h_list));

	/* set the device generate event and key type */
	set_bit(EV_KEY, _pwr_k_dev.evbit);
	set_bit(KEY_POWER, _pwr_k_dev.keybit);

	_pwr_k_dev.num_vals = 0;
	_pwr_k_dev.max_vals = PWR_KEY_EVENTS_PER_PACKET + 2;
	_pwr_k_dev.vals = rt_malloc(sizeof(struct input_value) * _pwr_k_dev.max_vals);
	if (_pwr_k_dev.vals == RT_NULL) {
		LOG_E("rt_malloc for input_dev vals error");
		return -RT_ENOMEM;
	}

	ret = input_register_device(&_pwr_k_dev);
	if (ret != RT_EOK) {
		LOG_E("input_register_device error");
		goto err_exit;
	}

	return RT_EOK;

err_exit:
	rt_free(_pwr_k_dev.vals);
	return ret;
}

/* for debug */
void reg_dump()
{
	
	reg_gpadc_ain1dat_t dat_reg1;
	dat_reg1.val = READREG32(VA_GPADC_AIN1DAT);
	LOG_D("VA_GPADC_AIN1DAT = %d", dat_reg1.bits.dat);

	reg_gpadc_aux1hdat_t hdat_reg1;
	hdat_reg1.val = READREG32(VA_GPADC_AUX1HDAT);
	LOG_D("VA_GPADC_AUX1HDAT = %d", hdat_reg1.bits.auxin1hdat);

	rt_bool_t charging = get_charge_status();
	if (charging)
		LOG_D("Charging ing ing ing ....");
	else
		LOG_D("no no no charge!");

	float v = get_bat_voltage();
	LOG_FLOAT("Batteray Voltage: %.2f", v);
}

#define KEY_TYPE_COUNT			4	/* power key event type */
#define WAIT_INT_CLEAR_INTERVAL		300	/* us */
static u64 last_key_time[KEY_TYPE_COUNT] = {0};
static u64 last_con_time;
static u64 last_discon_time;

/*
 * power_key_handle - this function will call when handle the power key
 * @param: none
 *
 */
void power_key_handle(KEY_STATUS_TYPE type)
{
	u64 current;

	/* rtc clear interrupt pending take time about 30~60us,
	add a interval for avoid report a lot of event repeatedly in this time period */
	current = rt_time_get_usec();
	if (current - last_key_time[type] > WAIT_INT_CLEAR_INTERVAL) {
		/* report event by input core */
		if (KEY_LONG_PRESS == type)
			disable_reset = 1;
		LOG_E("power_key_handle type: %d", type);
		input_report_key(&_pwr_k_dev, KEY_POWER, type);
		input_sync(&_pwr_k_dev);

		last_key_time[type] = current;

		/* to do: also can send the message to application */
	}
}

/*
 * power_disconnect - this function will call when usb power connect
 * @param: none
 *
 */
void power_connect()
{
	u64 current;

	/* rtc clear interrupt pending take time about 30~60us,
	add a interval for avoid send message repeatedly in this time period */
	current = rt_time_get_usec();

	/* save the charge status to register */
	set_charge_status(RT_TRUE);

	/* no need prompt low power when charging */
	csp_aux_set_th_en(PWR_GPADC_AUX_INDEX, RT_FALSE);

	/* send message to notify application the power connected */
	if (current - last_con_time > WAIT_INT_CLEAR_INTERVAL) {
		LOG_D("Power connect connect connect!");
		lb_system_mq_send(LB_SYSMSG_USB_POWER_CONNECT, NULL, 0, 0);
		last_con_time = current;
	}
}
RTM_EXPORT(power_connect);

/*
 * power_disconnect - this function will call when usb power disconnect
 * @param: none
 *
 */
void power_disconnect()
{
	u64 current;

	/* rtc clear interrupt pending take time about 30~60us,
	add a interval for avoid send message repeatedly in this time period */
	current = rt_time_get_usec();

	/* save the charge status to register */
	set_charge_status(RT_FALSE);

	/* open the interrupt for low power prompt */
	csp_aux_set_th_en(PWR_GPADC_AUX_INDEX, RT_TRUE);

	/* send message to notify application the power disconnected */
	if (current - last_discon_time > WAIT_INT_CLEAR_INTERVAL) {
		LOG_D("Power DIS DIS DIS DIS disconnect!");
		lb_system_mq_send(LB_SYSMSG_USB_POWER_DISCONNECT, NULL, 0, 0);
		last_discon_time = current;
	}
}
RTM_EXPORT(power_disconnect);

void usb_connect_status_judge()
{
	u32 original, pin_val;
	reg_rtc_sio_func_r0_t sf_reg;
	reg_rtc_sio_data_r0_t d_reg;

	/* change pwr_wake function to GPIO INPUT */
	sf_reg.val = READREG32(VA_RTC_SIO_FUNC_R0);
	original = sf_reg.val;

	/* SIO0 1: INPUT, 3: PWR_WAKE */
	sf_reg.bits.sio0 = 1;
	WRITEREG32(VA_RTC_SIO_FUNC_R0, sf_reg.val);

	/* get GPIO DATA register value */
	d_reg.val = READREG32(VA_RTC_SIO_DATA_R0);
	pin_val = d_reg.val & BIT(0);
	LOG_D("VA_RTC_SIO_DATA_R0 SIO0 value: %d", pin_val);

	/* save actual usb power connect status */
	if (pin_val)
		set_charge_status(RT_TRUE);
	else
		set_charge_status(RT_FALSE);

	/* set back to the original value */
	WRITEREG32(VA_RTC_SIO_FUNC_R0, original);
}

/* mark this is gsensor wake up boot */
void power_mark_gs_boot()
{
	_gs_boot = 1;
}

#define GSENSOR_HANDLE_INTERVAL		100	/* 100ms */
static int last_gs_handle_time;
void gsensor_irq_handle()
{
	int current;
	LOG_D("Gsensor int !!!");

	current = rt_time_get_msec();
	/* gsensor interrupt use temporary latched mode,
	avoid process lots of interrupt repeatedly,
	add time interval to ignore some interrupt. */
	if (current - last_gs_handle_time > GSENSOR_HANDLE_INTERVAL) {
		/* to do: send message to notify application the gsensor interrupt */
#ifdef ARCH_LOMBO_N7V1_CDR
		if (gsensor_det_sem)
			rt_sem_release(gsensor_det_sem);
#endif
		last_gs_handle_time = current;
	}
}
RTM_EXPORT(gsensor_irq_handle);

static void set_sio6_hight(void)
{
	reg_rtc_sio_func_r0_t func_reg;
	reg_rtc_sio_data_r0_t d_reg;

	/* function output */
	func_reg.val = READREG32(VA_RTC_SIO_FUNC_R0);
	func_reg.bits.sio6 = 0x2;
	WRITEREG32(VA_RTC_SIO_FUNC_R0, func_reg.val);

	/* set GPIO DATA register value */
	d_reg.val = READREG32(VA_RTC_SIO_DATA_R0);
	d_reg.val |= (BIT(6));
	WRITEREG32(VA_RTC_SIO_DATA_R0, d_reg.val);
}

static void set_sio6_low(void)
{
	reg_rtc_sio_func_r0_t func_reg;
	reg_rtc_sio_data_r0_t d_reg;

	/* function output */
	func_reg.val = READREG32(VA_RTC_SIO_FUNC_R0);
	func_reg.bits.sio6 = 0x2;
	WRITEREG32(VA_RTC_SIO_FUNC_R0, func_reg.val);

	/* set GPIO DATA register value */
	d_reg.val = READREG32(VA_RTC_SIO_DATA_R0);
	d_reg.val &= (~(BIT(6)));
	WRITEREG32(VA_RTC_SIO_DATA_R0, d_reg.val);
}


/*
 * get_bat_level - get the battery level
 * @param: none
 *
 * return battery level
 */
BAT_STATUS_TYPE get_bat_level()
{
	BAT_STATUS_TYPE t;
	reg_gpadc_ain1dat_t dat_reg1;
	u32 gpadc1;

	dat_reg1.val = READREG32(VA_GPADC_AIN1DAT);
	// LOG_W("VA_GPADC_AIN1DAT dat = %d", dat_reg1.bits.dat); 
	gpadc1 = dat_reg1.bits.dat;

	/* quick shutdown flag */
	if (gpadc1) {
		if (!boot_get_boot_type()) {
			if (gpadc1 < QUICK_SHUTDOWN_GPADC_TH) {
				rt_kprintf("set_sio6_hight\n");
				set_sio6_hight();
			} else if (temp_sensor_get_val() < 0) {
				rt_kprintf("set_sio6_hight temp %d\n",
					temp_sensor_get_val());
				set_sio6_hight();
			} else
				set_sio6_low();
		}
	}
/*	 rt_kprintf("gpadc1 %d\n", gpadc1); */
	if (get_charge_status()) {
		t = BATTERY_CHARGE;
		return t;
	}

	gpadc1 = dat_reg1.bits.dat;
	if (gpadc1 > _bat_high_th)
		t = BATTERY_LEVEL_3;
	else if (gpadc1 > _bat_middle_th)
		t = BATTERY_LEVEL_2;
	else
		t =BATTERY_LEVEL_1;
	return t;
}
RTM_EXPORT(get_bat_level);

# if 1 //B+ low power
/*
 * get_bat_level - get the battery level
 * @param: none
 *
 * return battery level
 */
u8 value_power_flag=0;
int _get_bat_level()
{
	int t;
	static int old_t=0;
	static u32 cnt=0;
	reg_gpadc_ain1dat_t dat_reg1;
	u32 gpadc1;

	dat_reg1.val = READREG32(VA_GPADC_AIN1DAT);
	// LOG_W("VA_GPADC_AIN1DAT dat = %d", dat_reg1.bits.dat); 
	gpadc1 = dat_reg1.bits.dat;

/*	 rt_kprintf("gpadc1 %d\n", gpadc1); */

	gpadc1 = dat_reg1.bits.dat;
	if (gpadc1 > BAT_HIGH_16V) // high poweroff
		t = BATTERY_LEVEL_4;
   else if (gpadc1 > _bat_high_th)
		t = BATTERY_LEVEL_3;
	else if (gpadc1 > _bat_middle_th)
		t = BATTERY_LEVEL_2;
	else
		t =BATTERY_LEVEL_1;

  if(old_t==t)
     cnt++;
  else{
	old_t=t;
    cnt=0;
  }
  if(cnt>3){
    cnt=0;
	if(old_t==1 || old_t==4)
	value_power_flag=1;
  }
//printf("\ngpadc1==%d====t=%d====old_t==%d===value_power_flag=%d===cnt=%d\n",gpadc1,t,old_t,value_power_flag,cnt);
	return t;
}
RTM_EXPORT(_get_bat_level);
#endif

#ifndef LOMBO_ACC_SIO5
int acc_test(int argc, char **argv)
{
	if (argc == 2)
		acc_test_value =  atoi(argv[1]);
	else
		LOG_E("argc num error");

	return 0;
}
MSH_CMD_EXPORT(acc_test, "acc_test");
#endif

/*
 * get_aac_sio_val - get the sio5 pin level
 * @param: none
 *
 * return 1: high level; 0: low level; other: error
 */
int get_acc_sio_val()
{
	int val = 0;
#ifndef LOMBO_ACC_SIO5
	val = acc_test_value;
#else
	if (_aac_pin_ctrl == RT_NULL) {
		LOG_W("_aac_pin_ctrl == RT_NULL");
		return 1;
	}

	if (_aac_pin_num < 0) {
		LOG_W("_aac_pin_num < 0");
		return 1;
	}

	return pinctrl_gpio_get_value(_aac_pin_ctrl, _aac_pin_num);
	
	if (!(sio5_timer.parent.flag & RT_TIMER_FLAG_ACTIVATED)) {
		val = pinctrl_gpio_get_value(_aac_pin_ctrl, _aac_pin_num);
		if (val == 0) {
			if (is_acc_line == 1) {
				rt_timer_start(&sio5_timer);
				sio5_lower_bat_num = 0;
				sio5_lower_bat_cnt = 0;
			} else
				val = 1;
		} else
			is_acc_line = 1;
	} else
		val = 0;
#endif
	return val;
}
RTM_EXPORT(get_acc_sio_val);

/*
 * set_acc_line - set acc line status
 * @able: 0: disable; 1: able
 *
 * return none
 */
void set_acc_line(int able)
{
#ifdef LOMBO_ACC_SIO5
	is_acc_line = able;
#endif
}
RTM_EXPORT(set_acc_line);

#ifdef LOMBO_ACC_SIO5

#define APP_POWER_KEY	116
typedef struct tag_app_key_msg_data {
	unsigned int	code;
	unsigned int	value;
} app_key_msg_data_t;

static void sio5_irq_handler(void *data)
{
	int val;

	val = pinctrl_gpio_get_value(_aac_pin_ctrl, _aac_pin_num);
	/* rt_kprintf("I am sio5 irq handler %d\n", val); */
	if (val == 1 || sio5_lower_bat_num != 0) {
		sio5_lower_bat_num++;
		if (val == 1)
			sio5_lower_bat_cnt++;
	}

	if (30 == sio5_lower_bat_num) {
		//if (sio5_lower_bat_cnt == sio5_lower_bat_num)
			rt_timer_stop(&sio5_timer);
		//else if (sio5_lower_bat_cnt < sio5_lower_bat_num/2) {
		//	app_key_msg_data_t sysmsg_addr;

		//	sysmsg_addr.code = APP_POWER_KEY;
		//	sysmsg_addr.value = 3;
		//	lb_system_mq_send
		//	(LB_SYSMSG_KEY, &sysmsg_addr, sizeof(app_key_msg_data_t), 0);
		//	rt_kprintf("sio5_irq_handler low power\n");
		//}
		/* rt_kprintf("sio5_lower_bat %d\n", sio5_lower_bat_cnt); */
		sio5_lower_bat_num = 0;
		sio5_lower_bat_cnt = 0;
	}
}
#endif

void rt_hw_power_disable_reset(void)
{
	disable_reset = 1;
}

void setup_sio5_wake_en(u32 enable)
{
#ifdef LOMBO_ACC_SIO5
	rt_err_t ret;
#if 0
	if (get_acc_sio_val() == 0 || enable == 0)
		csp_rtc_pm_int_enable(PM_TYPE_RING_KEY, enable);
	else {
		if (is_acc_line) {
			int file;
			file = open("/mnt/data/acc.bin", O_WRONLY | O_TRUNC);
			if (file != -1)
				close(file);
			csp_rtc_pm_int_enable(PM_TYPE_RING_KEY_LOW, enable);
		}
	}
#endif
#if 0
		if (get_acc_sio_val() == 0 || enable == 0)
			csp_rtc_pm_int_enable(PM_TYPE_RING_KEY, enable);
		else {
			if (is_acc_line) {
				int file;
				file = open("/mnt/data/acc.bin", O_WRONLY | O_TRUNC);
				if (file != -1)
					close(file);
				csp_rtc_pm_int_enable(PM_TYPE_RING_KEY_LOW, enable);
			}
		}
#else /* for ZCD TDR SDK */
//printf("\nenable====%d===,disable_reset====%d\n",enable,disable_reset);
	if (disable_reset == 0 && enable == 1 && pinctrl_gpio_get_value(_aac_pin_ctrl, _aac_pin_num) == 1 
	 && !value_power_flag) {
		csp_rtc_pm_int_enable(PM_TYPE_RING_KEY, enable);
		LOG_W("\n\npower_reboot\n");
		lb_hw_reboot();
		return;
	} else{
	//	if (get_acc_sio_val() == 0 || enable == 0)
		if (disable_reset == 0 && !value_power_flag)
			csp_rtc_pm_int_enable(PM_TYPE_RING_KEY, enable);	//不执行该句，只能按键开机
	}
	
#endif

	if (enable == 0) {
		/* sio5 for aac */
		_aac_pin_ctrl = pinctrl_get("acc-sio");
		if (_aac_pin_ctrl == RT_NULL) {
			LOG_W("SIO 5  pinctrl_get return RT_NULL");
			return;
		}

		_aac_pin_num = pinctrl_gpio_request(_aac_pin_ctrl,
							AAC_SIO_PORT, AAC_SIO_PIN);
		if (_aac_pin_num < 0) {
			LOG_W("SIO 5  pinctrl_gpio_request pin_num < 0");
			return;
		}

		ret = pinctrl_gpio_direction_input(_aac_pin_ctrl, _aac_pin_num);
		if (ret != RT_EOK) {
			LOG_W("pinctrl_gpio_direction_input error: %d", ret);
			return;
		}

		is_acc_line = 0;

		rt_timer_init(&sio5_timer, "sio5_timer",
				sio5_irq_handler, NULL,
				5,
				RT_TIMER_FLAG_DEACTIVATED |
				RT_TIMER_FLAG_PERIODIC |
				RT_TIMER_FLAG_HARD_TIMER);
	}
#else
	if (disable_reset == 0 && 1 == enable && get_charge_status()) {
		LOG_W("\n\npower reboot\n");
		lb_hw_reboot();
	}
#endif
}
RTM_EXPORT(setup_sio5_wake_en);
void sd_enable(void)
{
        rt_err_t ret;
        sd_pin_ctrl = pinctrl_get("sd-en");
		if (sd_pin_ctrl == RT_NULL) {
			LOG_E("sd_en  pinctrl_get return RT_NULL");
			return;
		}

		sd_pin_num = pinctrl_gpio_request(sd_pin_ctrl,
							SD_SIO_PORT, SD_SIO_PIN);
		if (sd_pin_num < 0) {
			LOG_E("sd_en  pinctrl_gpio_request pin_num < 0");
			return;
		}

		ret = pinctrl_gpio_direction_output(sd_pin_ctrl, sd_pin_num);
		if (ret != RT_EOK) {
			LOG_E("pinctrl_gpio_direction_output error: %d", ret);
			return;
		}
		 pinctrl_gpio_set_value(sd_pin_ctrl, sd_pin_num,0);
		

}
/*
 * is_gs_wakeup_boot - whether the device was wake up by gsensor
 * @param: none
 *
 * return 1: wake up by gsensor, 0: no
 */
int is_gs_wakeup_boot()
{
	return _gs_boot;
}
RTM_EXPORT(is_gs_wakeup_boot);

int rt_hw_power_init()
{
	rt_err_t ret;
	int cfg_ret;
	u32 p1, p2;
	/* get battery threshold from config */
	cfg_ret = config_get_u32("bat_th", "bat_high_th", &_bat_high_th);
	if (cfg_ret != 0) {
		LOG_W("config_get_u32 bat_high_th value error: %d", cfg_ret);
		_bat_high_th = BAT_HIGH_TH_DEFAULT;
	}

	cfg_ret = config_get_u32("bat_th", "bat_middle_th", &_bat_middle_th);
	if (cfg_ret != 0) {
		LOG_W("config_get_u32 bat_middle_th value error: %d", cfg_ret);
		_bat_middle_th = BAT_MIDDLE_TH_DEFAULT;
	}

	pwr_mutex = rt_mutex_create("pwr_mutex", RT_IPC_FLAG_FIFO);
	set_sio6_low();

	/* clear sio5 wakeup rtc register config */
	setup_sio5_wake_en(0);

	/* initialized gpadc register  */
	pwr_gpadc_cfg();

	/* register power key to input core */
	ret = register_power_key();
	if (ret != RT_EOK) {
		LOG_E("register_power_key error");
		return -RT_ERROR;
	}

	RT_ASSERT(!lombo_func2(&p1, &p2));
	if (1 == p1)
		usb_connect_status_judge();

    sd_enable();

	if (p1 != 1) {
		/* set 10s for super long press */
		csp_rtc_set_key_time(TIME_TYPE_SLONG, 10 * 1000);
	}	

	return 0;
}

//#if defined(ARCH_LOMBO_N7V0_CDR) || defined(ARCH_LOMBO_N7V1_CDR)
INIT_DEVICE_EXPORT(rt_hw_power_init);
//#endif

