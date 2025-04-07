/*
 * da380_gsensor.c - Gsensor module realization
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

#include <rthw.h>
#include <debug.h>

#include "da380_gsensor.h"
#include "gpio/pinctrl.h"
#include "bitops.h"
#include "rtc_csp.h"
#include <rtthread.h>
#include "board.h"

#define DA380_GSENSOR_MODULE_NAME	"da380-gsensor"

#if defined(ARCH_LOMBO_N7V0_CDR) || defined(ARCH_LOMBO_N7V1_CDR)
#define GSENSOR_I2C_HOST		2
#elif defined(ARCH_LOMBO_N7V0_EVB) || defined(ARCH_LOMBO_N7V1_EVB)
#define GSENSOR_I2C_HOST		0
#else
#define GSENSOR_I2C_HOST		0
#endif

#define DA380_GSENSOR_INT_PORT		GPIO_PORT_E
#define DA380_GSENSOR_PIN		GPIO_PIN_9

#define DA380_GSENSOR_ADDR		0x27

/* DA380 Register */
#define SOFT_RESET			0x00
#define CHIPID				0x01

#define ACC_X_LSB			0x02
#define ACC_X_MSB			0x03

#define ACC_Y_LSB			0x04
#define ACC_Y_MSB			0x05

#define ACC_Z_LSB			0x06
#define ACC_Z_MSB			0x07

#define MOTION_FLAG			0x09
#define NEWDATA_FLAG			0x0A

#define TAP_ACTIVE_STATUS		0x0B
#define ORIENT_STATUS			0x0C

/*
 Resolution
 bit[3:2] -- 00:14bit, 01:12bit, 10:10bit, 11:8bit
 FS bit[1:0] -- 00:+/-2g, 01:+/-4g, 10:+/-8g, 11:+/-16g
*/
#define RESOLUTION_RANGE		0x0F

#define ODR_AXIS			0x10
#define MODE_BW				0x11
#define SWAP_POLARITY			0x12
#define INT_SET1			0x16
#define INT_SET2			0x17
#define INT_MAP1			0x19
#define INT_MAP2			0x1A
#define INT_CONFIG			0x20
#define INT_LATCH			0x21
#define FREEFALL_DUR			0x22
#define FREEFALL_THS			0x23
#define FREEFALL_HYST			0x24
#define ACTIVE_DUR			0x27
#define ACTIVE_THS			0x28
#define TAP_DUR				0x2A
#define TAP_THS				0x2B
#define ORIENT_HYST			0x2C
#define Z_BLOCK				0x2D
#define SELF_TEST			0x32
#define ENGINEERING_MODE		0x7f

#define SAVE_PARK_MONITOR_CFG_BIT	1	/* bit for gsensor park monitor config */

/* i2c device for gsensor */
static struct rt_i2c_bus_device *i2c_gsensor_dev = RT_NULL;

/**
 * gsensor_write - write a 8bit data to gsensor register by i2c
 * @reg: register of gsensor to write
 * @data: the data to write
 *
 * return RT_EOK if success, RT_ERROR if failed
 */
static rt_err_t gsensor_write(u8 reg, u8 data)
{
	struct rt_i2c_msg msgs;
	rt_uint8_t buf[2] = {reg, data};

	if (i2c_gsensor_dev == RT_NULL) {
		LOG_E("Can't find Gsensor I2C device");
		return RT_ERROR;
	}

	msgs.addr  = DA380_GSENSOR_ADDR;
	msgs.flags = RT_I2C_WR;
	msgs.buf   = buf;
	msgs.len   = 2;

	if (rt_i2c_transfer(i2c_gsensor_dev, &msgs, 1) != 1) {
		LOG_E("rt_i2c_transfer error");
		return RT_ERROR;
	}

	return RT_EOK;
}

/**
 * gsensor_read - read a 8bit data from gsensor register by i2c
 * @reg: register of gsensor to read
 * @redata: the data to store
 *
 * return RT_EOK if success, RT_ERROR if failed
 */
static rt_err_t gsensor_read(u8 reg, u8 *redata)
{
	struct rt_i2c_msg msgs[2];

	if (i2c_gsensor_dev == RT_NULL) {
		LOG_E("Can't find Gsensor I2C device");
		return RT_ERROR;
	}

	msgs[0].addr  = DA380_GSENSOR_ADDR;
	msgs[0].flags = RT_I2C_WR;
	msgs[0].buf   = &reg;
	msgs[0].len   = 1;

	msgs[1].addr  = DA380_GSENSOR_ADDR;
	msgs[1].flags = RT_I2C_RD;
	msgs[1].buf   = redata;
	msgs[1].len   = 1;

	if (rt_i2c_transfer(i2c_gsensor_dev, msgs, 2) != 2) {
		LOG_E("rt_i2c_transfer error");
		return RT_ERROR;
	}

	return RT_EOK;
}

/**
 * gsensor_set_bit - set one bit value for register of gsensor
 * @reg: register of gsensor to set
 * @n: bit position
 * @bv: bit value
 *
 * return RT_EOK if success, RT_ERROR if failed
 */
static rt_err_t gsensor_set_bit(u8 reg, u8 n, u8 bv)
{
	u8 val, mask;
	rt_err_t ret;

	RT_ASSERT((bv == 0x00) || (bv == 0x01));
	ret = gsensor_read(reg, &val);
	if (ret != RT_EOK) {
		LOG_E("gsensor_set_bit error");
		return RT_ERROR;
	}

	mask = ~(1u << n);
	val = (val & mask) | (bv << n);

	ret = gsensor_write(reg, val);
	if (ret != RT_EOK) {
		LOG_E("gsensor_set_bit error");
		return RT_ERROR;
	}

	return RT_EOK;
}

/**
 * gsensor_get_bit - get one bit value from register of gsensor
 * @reg: register of gsensor
 * @n: bit position
 * @bv: bit value to store
 *
 * return RT_EOK if success, RT_ERROR if failed
 */
static rt_err_t gsensor_get_bit(u8 reg, u8 n, u8 *bv)
{
	rt_err_t ret;
	u8 val, msk;

	RT_ASSERT(n <= 7);
	ret = gsensor_read(reg, &val);
	if (ret != RT_EOK) {
		LOG_E("gsensor_get_bit error");
		return RT_ERROR;
	}

	msk = (u8)(1u << n);
	*bv = ((u8)(val & msk)) >> n;
	return RT_EOK;
}

/* set gsensor measure range: 2g, 4g, 8g, 16g */
rt_err_t gsensor_set_measure_range(int type)
{
	u8 val;
	rt_err_t ret;

	/* unused | unused | unused | unused | Resolution[1:0] | FS[1:0] */
	ret = gsensor_read(RESOLUTION_RANGE, &val);
	if (ret != RT_EOK) {
		LOG_E("gsensor_read error");
		return ret;
	}

	/* clear FS[1:0] */
	val = val & 0xfc;

	/* FS[1:0] -- 00: 2g, 01: 4g, 10: 8g, 11: 16g */
	switch (type) {
	case GS_MEASURE_RANGE_2G: //H
		// val = val | 0x00;      
		 val = val | 0x02;      
		break;

	case GS_MEASURE_RANGE_4G:
		val = val | 0x01;
		break;

	case GS_MEASURE_RANGE_8G://M
		val = val | 0x02;
		break;

	case GS_MEASURE_RANGE_16G://L
		val = val | 0x03;
		break;

	default:
	{
		LOG_E("Set_measure_range error, unknown range type: %d", type);
		return -RT_EINVAL;
	}
		break;
	}
   
	ret = gsensor_write(RESOLUTION_RANGE, val);
	if (ret != RT_EOK)
		LOG_E("gsensor_set_measure_range error");

	#if 1	
	if(type == GS_MEASURE_RANGE_2G)	
    ret = gsensor_write(ACTIVE_THS, 0x1F);//0X21
	else
	 ret = gsensor_write(ACTIVE_THS, 0x21);//0X21	
	#endif
	//printf("\ntype==================%d   ====== %x\n",type,val);
	return ret;
}
RTM_EXPORT(gsensor_set_measure_range)

/* set gsensor power mode */
rt_err_t gsensor_set_pwr_mode(int type)
{
	u8 val;
	rt_err_t ret;

	/* pwr_mode [1:0] | unused | low_power_bw [3:0] | unused*/
	ret = gsensor_read(MODE_BW, &val);
	if (ret != RT_EOK) {
		LOG_E("gsensor_read error");
		return ret;
	}

	/* clear pwr_mode[1:0] */
	val = val & 0x3f;

	/* pwr_mode [1:0] -- 00:normal mode, 01: low power mode, 1x: suspend mode */
	switch (type) {
	case GS_MODE_NORMAL:
		val = val | 0x00;
		break;

	case GS_MODE_LOW_POWER:
		val = val | 0x40;
		break;

	case GS_MODE_SUSPEND:
		val = val | 0x80;
		break;

	default:
	{
		LOG_E("gsensor_set_pwr_mode error, unknown mode type: %d", type);
		return -RT_EINVAL;
	}
		break;
	}

	ret = gsensor_write(MODE_BW, val);
	if (ret != RT_EOK)
		LOG_E("gsensor_set_measure_range error");

	return ret;
}
RTM_EXPORT(gsensor_set_pwr_mode);


/* set gsensor axis disable */
rt_err_t gsensor_set_axis_disable(int type, rt_bool_t disable)
{
	u8 n, val;
	rt_err_t ret;

	/* X-axis_disable | Y-axis_disable | Z-axis_disable | unused | ODR[3:0] */
	ret = gsensor_read(ODR_AXIS, &val);
	if (ret != RT_EOK) {
		LOG_E("gsensor_read error");
		return ret;
	}

	val = disable ? 0x01 : 0x00;

	switch (type) {
	case GS_AXIS_DISABLE_X:
		n = 7;
		break;

	case GS_AXIS_DISABLE_Y:
		n = 6;
		break;

	case GS_AXIS_DISABLE_Z:
		n = 5;
		break;

	default:
	{
		LOG_E("gsensor_set_axis_disable error, unknown type: %d", type);
		return -RT_EINVAL;
	}
		break;
	}

	ret = gsensor_set_bit(ODR_AXIS, n, val);
	if (ret != RT_EOK)
		LOG_E("gsensor_set_axis_disable error");

	return ret;
}

/* set gsensor interrupt enable */
rt_err_t gsensor_set_int_en(int type, rt_bool_t en)
{
	rt_err_t ret;
	u8 n, reg, val;

	/* 0: disable the orient interrupt, 1:enable the orient interrupt */
	val = en ? 0x01 : 0x00;

	/*
	INT_SET1 register
	unused | Orient_int_en | S_tap_int_en | d_tap_int_en
	unused | active_int_en_z | active_int_en_y | active_int_en_x

	INT_SET2 register
	unused | unused | unused |New_data_int_en
	Freefall_int_en | unused | unused | unused
	*/
	switch (type) {
	case GS_ORIENT_INT_EN:
		n = 6;
		reg = INT_SET1;
		break;

	case GS_S_TAP_INT_EN:
		n = 5;
		reg = INT_SET1;
		break;

	case GS_D_TAP_INT_EN:
		n = 4;
		reg = INT_SET1;
		break;

	case GS_ACTIVE_INT_EN_Z:
		n = 2;
		reg = INT_SET1;
		break;

	case GS_ACTIVE_INT_EN_Y:
		n = 1;
		reg = INT_SET1;
		break;

	case GS_ACTIVE_INT_EN_X:
		n = 0;
		reg = INT_SET1;
		break;

	case GS_NEW_DATA_INT_EN:
		n = 4;
		reg = INT_SET2;
		break;

	case GS_FREEFALL_INT_EN:
		n = 3;
		reg = INT_SET2;
		break;

	default:
		LOG_E("gsensor_set_int_en error, unknown type: %d", type);
		return -RT_EINVAL;
	}

	ret = gsensor_set_bit(reg, n, val);
	if (ret != RT_EOK)
		LOG_E("gsensor_set_int_en error");

	return ret;
}

/* get gsensor interrupt enable */
rt_err_t gsensor_get_int_en(int type, u8 *en)
{
	rt_err_t ret;
	u8 n, reg;

	switch (type) {
	case GS_ORIENT_INT_EN:
		n = 6;
		reg = INT_SET1;
		break;

	case GS_S_TAP_INT_EN:
		n = 5;
		reg = INT_SET1;
		break;

	case GS_D_TAP_INT_EN:
		n = 4;
		reg = INT_SET1;
		break;

	case GS_ACTIVE_INT_EN_Z:
		n = 2;
		reg = INT_SET1;
		break;

	case GS_ACTIVE_INT_EN_Y:
		n = 1;
		reg = INT_SET1;
		break;

	case GS_ACTIVE_INT_EN_X:
		n = 0;
		reg = INT_SET1;
		break;

	case GS_NEW_DATA_INT_EN:
		n = 4;
		reg = INT_SET2;
		break;

	case GS_FREEFALL_INT_EN:
		n = 3;
		reg = INT_SET2;
		break;

	default:
		LOG_E("gsensor_get_int_en error, unknown type: %d", type);
		return -RT_EINVAL;
	}

	ret = gsensor_get_bit(reg, n, en);
	if (ret != RT_EOK)
		LOG_E("gsensor_get_int_en error");

	return ret;
}

/* set gsensor INT map */
rt_err_t gsensor_set_int_map(int type, rt_bool_t mp)
{
	rt_err_t ret;
	u8 n, reg, val;

	/* 0:doesn't mapping orient interrupt to INT, 1:mapping orient interrupt to INT */
	val = mp ? 0x01 : 0x00;

	/*
	INT_MAP1
	x | Int_orient | Int_s_tap | Int_d_tap | x | Int_active | x | Int_freefall

	INT_MAP2
	x | x | x | x | x | x | x | Int_new_data
	*/
	switch (type) {
	case GS_INT_ORIENT:
		n = 6;
		reg = INT_MAP1;
		break;

	case GS_INT_S_TAP:
		n = 5;
		reg = INT_MAP1;
		break;

	case GS_INT_D_TAP:
		n = 4;
		reg = INT_MAP1;
		break;

	case GS_INT_ACTIVE:
		n = 2;
		reg = INT_MAP1;
		break;

	case GS_INT_FREEFALL:
		n = 0;
		reg = INT_MAP1;
		break;

	case GS_INT_NEW_DATA:
		n = 0;
		reg = INT_MAP2;
		break;

	default:
		LOG_E("gsensor_set_int_map error, unknown type: %d", type);
		return -RT_EINVAL;
	}

	ret = gsensor_set_bit(reg, n, val);
	if (ret != RT_EOK)
		LOG_E("gsensor_set_int_map error");

	return ret;
}

/* get gsensor interrupt flag */
rt_err_t gsensor_get_motion_flag(int type, u8 *flag)
{
	rt_err_t ret;
	u8 val, n;

	/*
	unused |Orient_int |S_tap_int |D_tap_int |
	unused |Active_int |unused |Freefall_int
	*/
	ret = gsensor_read(MOTION_FLAG, &val);
	if (ret != RT_EOK) {
		LOG_E("gsensor_read error");
		return RT_ERROR;
	}

	switch (type) {
	case GS_MOTION_FLAG_ORIENT:
		n = 6;
		break;

	case GS_MOTION_FLAG_S_TAP:
		n = 5;
		break;

	case GS_MOTION_FLAG_D_TAP:
		n = 4;
		break;

	case GS_MOTION_FLAG_ACTIVE:
		n = 2;
		break;

	case GS_MOTION_FLAG_FREEFALL:
		n = 0;
		break;

	default:
		LOG_E("gsensor_get_motion_flag error, unknown type: %d", type);
		return RT_ERROR;
	}

	ret = gsensor_get_bit(MOTION_FLAG, n, flag);
	if (ret != RT_EOK)
		LOG_E("gsensor_get_motion_flag error");

	return ret;
}

/* get gsensor acc value */
s16 gsensor_get_acc(int type)
{
	s16 acc;
	u8 lsb, msb;

	switch (type) {
	case GS_ACC_X:
		gsensor_read(ACC_X_LSB, &lsb);
		gsensor_read(ACC_X_MSB, &msb);
		break;

	case GS_ACC_Y:
		gsensor_read(ACC_Y_LSB, &lsb);
		gsensor_read(ACC_Y_MSB, &msb);
		break;

	case GS_ACC_Z:
		gsensor_read(ACC_Z_LSB, &lsb);
		gsensor_read(ACC_Z_MSB, &msb);
		break;

	default:
		LOG_E("gsensor_get_acc error, unknown type: %d", type);
		return -1;
	}

	acc = ((s16)(msb << 8 | lsb)) >> 2;
	return acc;
}

/* reset gsensor register to default value */
rt_err_t gsensor_soft_reset()
{
	return gsensor_write(SOFT_RESET, 0x20);
}

/* reset all latched int */
void gsensor_clr_int_status()
{
	gsensor_set_bit(INT_LATCH, 7, 0x01);
}

static rt_err_t init_gsensor_i2c()
{
	char i2c_name[6] = {0};
	rt_sprintf(i2c_name, "%s%d", "i2c", GSENSOR_I2C_HOST);

	/* find i2c device for gsensor */
	i2c_gsensor_dev = rt_i2c_bus_device_find(i2c_name);
	if (i2c_gsensor_dev == RT_NULL) {
		LOG_E("I2C Can't find bus dev \"%s\"", i2c_name);
		return -RT_ERROR;
	}
	return RT_EOK;
}

/* log gsensor acc data, for test */
void test_gsensor_acc_data()
{
	s16 x, y, z;
	u8 flag, val, sign;
	rt_err_t ret;

	x = gsensor_get_acc(GS_ACC_X);
	y = gsensor_get_acc(GS_ACC_Y);
	z = gsensor_get_acc(GS_ACC_Z);

	ret = gsensor_get_motion_flag(GS_MOTION_FLAG_ACTIVE, &flag);
	if (ret != RT_EOK) {
		LOG_E("Get GS_MOTION_FLAG_ACTIVE Flag error");
		return;
	}

	if (flag) {
		/* active first and active sign */
		ret = gsensor_read(TAP_ACTIVE_STATUS, &val);
		if (ret != RT_EOK)
			LOG_E("Read TAP_ACTIVE_STATUSg error");
		else {
			/* find the first active axis */
			if (val & 0x01) {
				/* active_first_z */
				rt_kprintf("active_first -> z\n");
			} else if (val & 0x02) {
				/* active_first_y */
				rt_kprintf("active_first -> y\n");
			} else if (val & 0x04) {
				/* active_first_x */
				rt_kprintf("active_first -> x\n");
			}

			sign = (val & 0x08) >> 3;
			LOG_D("active_sign = %d", sign);
		}

		gsensor_clr_int_status();
		rt_kprintf("test_gsensor_acc_data -> x = %d y = %d z = %d\n", x, y, z);
	}
}

void gsensor_dump()
{
	u8 val;

	rt_kprintf("******************** gsensor_dump **********************\n");

	gsensor_read(SOFT_RESET, &val);
	rt_kprintf("SOFT_RESET = %x\n", val);

	gsensor_read(RESOLUTION_RANGE, &val);
	rt_kprintf("RESOLUTION_RANGE = %x\n", val);

	gsensor_read(MODE_BW, &val);
	rt_kprintf("MODE_BW = %x\n", val);

	gsensor_read(MOTION_FLAG, &val);
	rt_kprintf("MOTION_FLAG = %x\n", val);

	gsensor_read(INT_CONFIG, &val);
	rt_kprintf("INT_CONFIG = %x\n", val);

	gsensor_read(INT_MAP1, &val);
	rt_kprintf("INT_MAP1 = %x\n", val);

	gsensor_read(INT_SET1, &val);
	rt_kprintf("INT_SET1 = %x\n", val);

	gsensor_read(ACTIVE_THS, &val);
	rt_kprintf("ACTIVE_THS = %x\n", val);

	gsensor_read(INT_LATCH, &val);
	rt_kprintf("INT_LATCH = %x\n", val);

	gsensor_read(ACTIVE_DUR, &val);
	rt_kprintf("ACTIVE_DUR = %x\n", val);

	rt_kprintf("*******************************************************\n");
}

/* set gsensor park monitor config */
static void __set_park_monitor_cfg(rt_bool_t open)
{
	/* use alive_reg0 register 0 bit to save charge status */
	reg_rtc_rtc_alive_reg0_t reg;
	reg.val = READREG32(VA_RTC_RTC_ALIVE_REG0);

	if (open)
		reg.val |= BIT(SAVE_PARK_MONITOR_CFG_BIT);
	else
		reg.val &= ~(BIT(SAVE_PARK_MONITOR_CFG_BIT));

	WRITEREG32(VA_RTC_RTC_ALIVE_REG0, reg.val);
}

/* get gsensor park monitor config */
static rt_bool_t __get_park_monitor_cfg()
{
	reg_rtc_rtc_alive_reg0_t reg;
	reg.val = READREG32(VA_RTC_RTC_ALIVE_REG0);

	if ((reg.val & BIT(SAVE_PARK_MONITOR_CFG_BIT)) == 0)
		return RT_FALSE;

	return RT_TRUE;
}

static rt_err_t setup_gsensor()
{
	rt_err_t ret;
	u8 val;

	ret = gsensor_soft_reset();	/* reset register value */
	if (ret != RT_EOK)
		return ret;

	/* add delay after reset for avoid i2c transfer error */
	mdelay(10);

	ret = gsensor_set_measure_range(GS_MEASURE_RANGE_2G);
	if (ret != RT_EOK)
		return ret;

	/* set power mode */
	ret = gsensor_set_pwr_mode(GS_MODE_NORMAL);
	if (ret != RT_EOK)
		return ret;

	/* Int_lvl [0] for pin INT, 0: level low; 1: level high */
	ret = gsensor_write(INT_CONFIG, 0x01);
	if (ret != RT_EOK)
		return ret;

	/* set active INT map */
	ret = gsensor_set_int_map(GS_INT_ACTIVE, RT_TRUE);
	if (ret != RT_EOK)
		return ret;

	/* active x, y, z interrupt */
	ret = gsensor_set_int_en(GS_ACTIVE_INT_EN_X, RT_TRUE);
	if (ret != RT_EOK)
		return ret;

	ret = gsensor_set_int_en(GS_ACTIVE_INT_EN_Y, RT_TRUE);
	if (ret != RT_EOK)
		return ret;

	ret = gsensor_set_int_en(GS_ACTIVE_INT_EN_Z, RT_TRUE);
	if (ret != RT_EOK)
		return ret;

	/*
	latch_int, interrupt mode
	1111: latched, 0000: non-latched, 0011: temporary latched 1s
	1011: temporary latched 25ms
	*/
	ret = gsensor_write(INT_LATCH, 0x8B);
	if (ret != RT_EOK)
		return ret;

	/* active duration time */
	ret = gsensor_write(ACTIVE_DUR, 0x03);
	if (ret != RT_EOK)
		return ret;

	/* active interrupt threshold */
	ret = gsensor_write(ACTIVE_THS, 0x21);//0X21
	if (ret != RT_EOK)
		return ret;

	/* set odr */
	ret = gsensor_read(ODR_AXIS, &val);
	if (ret != RT_EOK)
		return ret;

	/* clear odr[3:0] */
	val &= 0xf0;
	val |= 0x05;	/* 0101: 31.25Hz */
	ret = gsensor_write(ODR_AXIS, val);
	if (ret != RT_EOK)
		return ret;

	return RT_EOK;
}

int gsensor_init()
{
	rt_err_t ret;

	ret = init_gsensor_i2c();
	if (ret != RT_EOK) {
		LOG_E("gsensor_init error: %d", ret);
		return ret;
	}

	ret = setup_gsensor();
	if (ret != RT_EOK) {
		LOG_E("setup_gsensor error: %d", ret);
		return ret;
	}

	/* LOG_I("gsensor_init finished"); */
	return 0;
}

void gsensor_open_monitor()
{
	csp_rtc_pm_int_enable(PM_TYPE_GS, 1);
}
RTM_EXPORT(gsensor_open_monitor);

void gsensor_close_monitor()
{
	csp_rtc_pm_int_enable(PM_TYPE_GS, 0);
}
RTM_EXPORT(gsensor_close_monitor);

/*
 * gsensor_set_park_monitor_cfg - set gsensor park monitor config
 * @open: bool value, open or close park monitor
 *
 */
void gsensor_set_park_monitor_cfg(rt_bool_t open)
{
	__set_park_monitor_cfg(open);
}
RTM_EXPORT(gsensor_set_park_monitor_cfg);

/*
 * gsensor_get_park_monitor_cfg - get gsensor park monitor config
 *
 *  return: 1: park monitor open, 0: close
 */
int gsensor_get_park_monitor_cfg()
{
	return  __get_park_monitor_cfg() ? 1 : 0;
}
RTM_EXPORT(gsensor_get_park_monitor_cfg);


#ifdef ARCH_LOMBO_N7
INIT_DEVICE_EXPORT(gsensor_init);
#endif

