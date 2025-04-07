/*
 * rn6752_dvp.c - rn6752 driver code for LomboTech
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

#define DBG_SECTION_NAME	"RN6752-DVP"
#define DBG_LEVEL		DBG_LOG

#include <debug.h>
#include "viss.h"
#include "vic_dev.h"
#include "viss_i2c.h"
#include "viss_cam_power.h"
#include <div.h>

#ifdef RT_USING_VIC_DET_SIGNAL
#include "system/system_mq.h"
static rt_uint8_t rn6752_in;
#endif

struct sensor_reg {
	rt_uint16_t reg_add;
	rt_uint16_t reg_value;
};

#define DRV_VIC_RN6752_NAME "rn6752-dvp"
#define DRV_VIC_RN6752_PWR_NAME "cam-pwr-en"
#define RN6752_DVP_REG_DELAY_FLAG  (0xffff)

#if 0
static const struct sensor_reg rn6752_yuv_720p_30fps[] = {
	/*
	 * if clock source(Xin) of RN675x is 26MHz,
	 * please add these procedures marked first
	 * { 0xD2, 0x85 }, disable auto clock detect
	 * { 0xD6, 0x37 }, 27MHz default
	 * { 0xD8, 0x18 },  switch to 26MHz clock
	 * mdelay(100 },  delay 100ms
	 */
	{ 0x81, 0x01 },  /* turn on video decoder */
	{ 0xA3, 0x04 },  /* enable 72MHz sampling */
	{ 0xDB, 0x8F },  /* internal use */
	{ 0xFF, 0x00 },  /* switch to ch0 (default; optional) */
	{ 0x2C, 0x30 },  /* select sync slice points */
	{ 0x50, 0x02 },  /* 720p resolution select for BT.601 */
	{ 0x56, 0x00 },  /* disable SAV & EAV for BT601;
			  * 0x00 enable SAV & EAV for BT656 */
	{ 0x63, 0xBD },  /* filter control */
	{ 0x59, 0x00 },  /* extended register access */
	{ 0x5A, 0x04 },  /* data for extended register */
	{ 0x58, 0x01 },  /* enable extended register write */
	{ 0x07, 0x28 },  /* 720p format 23 */
	{ 0x2F, 0x04 },  /* internal use */
	{ 0x5E, 0x0B },  /* enable H-scaling control */
	{ 0x51, 0x44 },  /* scale factor1 */
	{ 0x52, 0x86 },  /* scale factor2 */
	{ 0x53, 0x22 },  /* scale factor3 */
	{ 0x3A, 0x04 },  /* no channel information insertion;
			  * invert VBLK for frame valid */
	{ 0x3E, 0x32 },  /* AVID & VBLK out for BT.601 */
	{ 0x40, 0x04 },  /* no channel information insertion;
			  * invert VBLK for frame valid */
	{ 0x46, 0x23 },  /* AVID & VBLK out for BT.601 */
	{ 0x28, 0x92 },  /* cropping */
	{ 0x00, 0x20 },  /* internal use */
	{ 0x2D, 0xF2 },  /* cagc adjust */
	{ 0x0D, 0x20 },  /* cagc initial value */
	{ 0x05, 0x00 },  /* sharpness */
	{ 0x04, 0x80 },  /* hue */
	{ 0x37, 0x33 },
	{ 0x61, 0x6C },
	{ 0xDF, 0xFE },  /* enable 720p format */
	{ 0x8E, 0x00 },  /* single channel output for VP */
	{ 0x8F, 0x80 },  /* 720p mode for VP */
	{ 0x8D, 0x31 },  /* enable VP out */
	{ 0x89, 0x09 },  /* select 72MHz for SCLK */
	{ 0x88, 0xC1 },  /* enable SCLK out */
	{ 0x81, 0x01 },  /* turn on video decoder */
	{ 0x96, 0x00 },  /* select AVID & VBLK as status indicator */
	{ 0x97, 0x0B },  /* enable status indicator out on AVID,VBLK & VSYNC */
	{ 0x98, 0x00 },  /* video timing pin status */
	{ 0x9A, 0x40 },  /* select AVID & VBLK as status indicator */
	{ 0x9B, 0xE1 },  /* enable status indicator out on HSYNC */
	{ 0x9C, 0x00 },  /* video timing pin status */
};
#endif

static const struct sensor_reg rn6752_yuv_720p_25fps[] = {
	/*
	 * if clock source(Xin) of RN675x is 26MHz,
	 * please add these procedures marked first
	 * { 0xD2, 0x85 }, disable auto clock detect
	 * { 0xD6, 0x37 },  27MHz default
	 * { 0xD8, 0x18 },  switch to 26MHz clock
	 * mdelay(100 },  delay 100ms
	 */
	/* { 0x80, 0x34 }, */
	/* { RN6752_DVP_REG_DELAY_FLAG, 20}, */
	{ 0x81, 0x01 }, /* turn on video decoder */
	{ 0xA3, 0x04 }, /* enable 72MHz sampling */
	{ 0xDB, 0x8F },  /* internal use  */
	{ 0xFF, 0x00 },  /* switch to ch0 (default; optional) */
	{ 0x2C, 0x30 },  /* select sync slice points */
	{ 0x50, 0x02 },  /* 720p resolution select for BT.601 */
	{ 0x56, 0x00 }, /* disable SAV & EAV for BT601;
			 * 0x00 enable SAV & EAV for BT656 */
	{ 0x63, 0xBD },  /* filter control */
	{ 0x59, 0x00 },  /* extended register access */
	{ 0x5A, 0x02 },  /* data for extended register */
	{ 0x58, 0x01 },  /* enable extended register write */
	{ 0x07, 0x23 },  /* 720p format */
	{ 0x2F, 0x04 },  /* internal use */
	{ 0x5E, 0x0B },  /* enable H-scaling control */
	{ 0x51, 0x44 },  /* scale factor1 */
	{ 0x52, 0x86 },  /* scale factor2 */
	{ 0x53, 0x22 },  /* scale factor3 */
	{ 0x3A, 0x04 },  /* no channel information insertion;
			  * invert VBLK for frame valid */
	{ 0x3E, 0x32 },  /* AVID & VBLK out for BT.601 */
	{ 0x40, 0x04 },  /* no channel information insertion;
			  * invert VBLK for frame valid */
	{ 0x46, 0x23 },  /* AVID & VBLK out for BT.601 */
	{ 0x28, 0x92 },  /* cropping */
	{ 0x00, 0x20 },  /* internal use  */
	{ 0x2D, 0xF2 },  /* cagc adjust */
	{ 0x0D, 0x20 },  /* cagc initial value */
	{ 0x05, 0x00 },  /* sharpness */
	/* { 0x02, 0x80 }, */  /* Contrast */
	/* { 0x03, 0x80 }, */  /* Saturation */
	/* { 0x04, 0x80 }, */  /* hue */
	{ 0x37, 0x33 },
	{ 0x61, 0x6C },
	{ 0x1A, 0x83 },
	{ 0x01, 0x46 },  /* Brightness */
	{ 0xDF, 0xFE },  /* enable 720p format */
	{ 0x8E, 0x00 },  /* single channel output for VP */
	{ 0x8F, 0x80 }, /* 720p mode for VP */
	{ 0x8D, 0x31 },  /* enable VP out */
	{ 0x89, 0x09 },  /* select 72MHz for SCLK */
	{ 0x88, 0xC1 },  /* enable SCLK out */
	{ 0x81, 0x01 },  /* turn on video decoder */
	{ 0x96, 0x00 },  /* select AVID & VBLK as status indicator */
	{ 0x97, 0x0B },  /* enable status indicator out on AVID,VBLK & VSYNC */
	{ 0x98, 0x00 },  /* video timing pin status */
	{ 0x9A, 0x40 },  /* select AVID & VBLK as status indicator */
	{ 0x9B, 0xE1 },  /* enable status indicator out on HSYNC */
	{ 0x9C, 0x00 },  /* video timing pin status */
};

static struct dev_mode rn6752_dvp_mode[] = {
	{
		.index = 0,
		.inp_fmt = VISS_MBUS_FMT_UYVY8_2X8,
		.out_fmt = VISS_PIX_FMT_NV12,
		.colorspace = DEV_COLORSPACE_JPEG,
		.frame_rate = 25000,   /* 25fps */
		.frame_size.width = 1280,
		.frame_size.height = 720,
		.usr_data = (void *)rn6752_yuv_720p_25fps,
		.usr_data_size = ARRAY_SIZE(rn6752_yuv_720p_25fps),
	},
#if 0  /* 30fps still have problems */
	{
		.index = 1,
		.inp_fmt = VISS_MBUS_FMT_UYVY8_2X8,
		.out_fmt = VISS_PIX_FMT_NV12,
		.colorspace = DEV_COLORSPACE_JPEG,
		.frame_rate = 30000,   /* 30fps */
		.frame_size.width = 1280,
		.frame_size.height = 720,
		.usr_data = (void *)rn6752_yuv_720p_30fps,
		.usr_data_size = ARRAY_SIZE(rn6752_yuv_720p_30fps),
	},
#endif
};

static rt_err_t __rn6752_block_write(void *hdl, void *data, rt_int32_t size)
{
	struct video_dev *rn6752 = (struct video_dev *)hdl;
	struct sensor_reg *reg = (struct sensor_reg *)data;
	rt_int32_t i = 0;
	rt_err_t ret = 0;
	rt_int32_t delay = 0, temp = 0;

	for (i = 0; i < size; i++) {
		if (RN6752_DVP_REG_DELAY_FLAG == reg[i].reg_add) {
			temp = reg[i].reg_value + 9;
			do_div(temp, 10);
			delay = temp * 10;
			/* LOG_D("delay: %d", delay); */
			do_div(delay, 10);
			rt_thread_delay(delay);
			continue;
		}
		ret = viss_i2c_write_reg_8bit(rn6752->i2c_client,
					reg[i].reg_add, reg[i].reg_value);
		if (RT_EOK != ret) {
			LOG_E("reg[i].reg_add : %x\n", reg[i].reg_add);
			return ret;
		}
	}

	return RT_EOK;
}

static struct dev_mode *__rn6752_cur_mode(void *hdl)
{
	struct video_dev *rn6752 = (struct video_dev *)hdl;

	return rn6752->cur_mode;
}

static struct dev_mode *__rn6752_get_all_mode(void *hdl, rt_int32_t *num)
{
	RT_ASSERT(RT_NULL != num);
	*num = (rt_int32_t)ARRAY_SIZE(rn6752_dvp_mode);

	return rn6752_dvp_mode;
}

static rt_err_t __rn6752_set_power(void *hdl, rt_bool_t on)
{
	struct video_dev *rn6752 = (struct video_dev *)hdl;

	if (1 != rn6752->rst_valid)
		return RT_EOK;

	if (RT_TRUE == on) {
		/* LOG_D("Rn6752 reset"); */
		pinctrl_gpio_set_value(rn6752->pctrl, rn6752->rst_gpio, 0);
		rt_thread_delay(1); /* 30ms */
		pinctrl_gpio_set_value(rn6752->pctrl, rn6752->rst_gpio, 1);
		rt_thread_delay(6); /* 100ms */
	} else {
		/* TO DO */
	}

	return RT_EOK;
}

static rt_err_t __rn6752_set_mode(void *hdl, rt_int32_t index)
{
	struct video_dev *rn6752 = (struct video_dev *)hdl;
	rt_int32_t num = 0;
	rt_err_t ret = 0;

	num = (rt_int32_t)ARRAY_SIZE(rn6752_dvp_mode);
	if (index >= num) {
		LOG_E("Input mode index error. num:%d index:%d\n", num, index);
		return -RT_EINVAL;
	}
	if (index >= ARRAY_SIZE(rn6752_dvp_mode)) {
		LOG_E("Input para error. index:%d\n", index);
		return -RT_EINVAL;
	}
	/* LOG_D("set_mode index:%d", index); */
	rn6752->cur_mode = &rn6752_dvp_mode[index];
	ret = __rn6752_block_write((void *)rn6752, rn6752->cur_mode->usr_data,
			rn6752->cur_mode->usr_data_size);
	return ret;
}


static rt_err_t __rn6752_set_register(void *hdl, struct viss_dbg_register *reg)
{
	return RT_EOK;
}

static rt_err_t __rn6752_get_register(void *hdl, struct viss_dbg_register *reg)
{
	return RT_EOK;
}

static rt_err_t __rn6752_set_stream(void *hdl, rt_bool_t enable)
{
	struct video_dev *rn6752 = (struct video_dev *)hdl;
	/* rt_int32_t ret = 0; */
	/* rt_uint8_t tmp = 0; */
	if (enable) {
		viss_i2c_write_reg_8bit(rn6752->i2c_client, 0x80, 0x30);
#if 0
		rt_thread_delay(10);
		while (1) {
			ret = viss_i2c_read_reg_8bit(rn6752->i2c_client, 0x00, &tmp);
			LOG_E("tmp:%x\n", tmp);
			if (0 == (tmp & (0x10)))
				break;
			else
				rt_thread_delay(2);
		}
#endif

	} else
		viss_i2c_write_reg_8bit(rn6752->i2c_client, 0x80, 0x34);
	return RT_EOK;
}


static rt_err_t __rn6752_ioctl(void *hdl, rt_int32_t cmd, void *para)
{
	rt_err_t ret = 0;
	rt_uint8_t tmp = 0;
	struct video_dev *rn6752 = (struct video_dev *)hdl;
	if (RT_NULL == hdl) {
		LOG_E("__rn6752_ioctl fail.");
		return -RT_ERROR;
	}
	ret = viss_i2c_read_reg_8bit(rn6752->i2c_client, 0x00, &tmp);
	if (tmp & (1<<4)) {
#ifdef RT_USING_VIC_DET_SIGNAL
		if (rn6752_in != 0) {
			rn6752_in = 0;
			lb_system_mq_send(LB_SYSMSG_AV_PLUGOUT, NULL, 0, 0);
			LOG_E("__rn6752_ioctl LB_SYSMSG_AV_PLUGOUT.");
		}
#endif
		return 1;
	} else {
#ifdef RT_USING_VIC_DET_SIGNAL
		if (rn6752_in != 1) {
			rn6752_in = 1;
			lb_system_mq_send(LB_SYSMSG_AV_PLUGIN, NULL, 0, 0);
			LOG_E("__rn6752_ioctl LB_SYSMSG_AV_PLUGIN.");
		}
#endif
		return 0;
	}
	return ret;
}

static rt_err_t __rn6752_get_info(void *hdl, struct viss_source_info *info)
{
	struct video_dev *rn6752 = (struct video_dev *)hdl;

	if ((RT_NULL == hdl) || (RT_NULL == info)) {
		LOG_E("Get rn6752 information fail.");
		return -RT_ERROR;
	}
	rt_memcpy(info, &rn6752->info, sizeof(struct viss_source_info));

	return RT_EOK;
}

static rt_err_t __rn6752_parser_config(void *hdl)
{
	struct video_dev *rn6752 = (struct video_dev *)hdl;
	struct viss_source_info info;
	rt_uint32_t tmp_data = 0;
	const char *status;
	const char *i2c_bus_name;
	const char *module = DRV_VIC_RN6752_NAME;
	rt_err_t ret = 0;
	rt_int32_t  use_enabel = 0;

	rt_memset(&info, 0, sizeof(info));
	ret = config_get_string(module, "status", &status);
	if (-1 == ret) {
		LOG_E("Get status fail.");
	} else {
		if (0 == strcmp(status, "okay"))
			use_enabel = 1;
	}
	if (0 == use_enabel) {
		LOG_W("rn6752 dvp disable.");
		return -RT_ERROR;
	}
	ret = config_get_string(module, "i2c-bus", &i2c_bus_name);
	if (-1 == ret) {
		LOG_E("Get i2c_bus fail.");
		return -RT_ERROR;
	}
	strncpy(info.i2c_bus_name, i2c_bus_name, sizeof(info.i2c_bus_name) - 1);
	/* LOG_D("info.i2c_bus_name:%s\n", info.i2c_bus_name); */

	ret = config_get_u32(module, "if-type", &info.if_type);
	if (-1 == ret)
		LOG_E("Get if-type fail.");
	ret = config_get_u32(module, "if-mode", &info.if_mode);
	if (-1 == ret)
		LOG_E("Get if-mode fail.");
	ret = config_get_u32(module, "out-path", &tmp_data);
	if (-1 == ret) {
		LOG_E("Get out-path fail.");
	} else {
		if (1 == tmp_data)
			info.out_path = VISS_IO_ISP;
		else
			info.out_path = VISS_IO_DMA;
	}

	ret = config_get_u32(module, "channel-num", &info.channel_num);
	if (-1 == ret)
		LOG_E("Get channel_num fail.");

	info.frame_buf_type = 0;
	if (((VISS_IF_TYPE_ITU_656 == info.if_type) ||
		(VISS_IF_TYPE_ITU_1120 == info.if_type)) && (info.channel_num > 1)) {
		ret = config_get_u32(module, "frame-buf-type", &info.frame_buf_type);
		if (-1 == ret)
			LOG_E("Get frame_buf_type fail.");
	}
	ret = config_get_u32(module, "reg", &info.i2c_addr);
	if (-1 == ret)
		LOG_E("Get i2c_addr fail.");
	ret = config_get_u32(module, "hsync-active", &info.href);
	if (-1 == ret)
		LOG_E("Get href fail.");
	ret = config_get_u32(module, "vsync-active", &info.vref);
	if (-1 == ret)
		LOG_E("Get vref fail.");
	ret = config_get_u32(module, "interlaced", &info.interlaced);
	if (-1 == ret)
		LOG_E("Get vref fail.");
	ret = config_get_u32(module, "pclk-sample", &info.pclk);
	if (-1 == ret)
		LOG_E("Get pclk fail.");
	ret = config_get_u32(module, "field-sel", &info.field_sel);
	if (-1 == ret)
		LOG_E("Get field_sel fail.");
#if 0
	ret = config_get_u32(module, "mclk-freq", &info.mclk_freq);
	if (-1 == ret)
		LOG_E("Get mclk_freq fail.");
#endif
	ret = config_get_u32(module, "viss_top_freq", &info.viss_top_freq);
	if (-1 == ret) {
		LOG_E("Get viss_top_freq fail.");
		return -RT_ERROR;
	}

	if (info.out_path == VISS_IO_ISP) {
		ret = config_get_u32(module, "isp_top_freq", &info.isp_top_freq);
		if (-1 == ret) {
			LOG_E("Get isp_top_freq fail.");
			return -RT_ERROR;
		}
	}
	rt_memcpy(&rn6752->info, &info, sizeof(struct viss_source_info));

	/* reset pin */
	rn6752->rst_valid = 1;
	ret = config_get_u32_array(module, "dvp-rst",
				rn6752->rst_val, ARRAY_SIZE(rn6752->rst_val));
	if (ret != ARRAY_SIZE(rn6752->rst_val)) {
		LOG_E("vic: reset pin config error. ret:%d", ret);
		rn6752->rst_valid = 0;
	}
#if 0
	/* mclk pin */
	rn6752->mclk_valid = 1;
	ret = config_get_u32_array(module, "dvp-mclk",
				rn6752->mclk_val, ARRAY_SIZE(rn6752->mclk_val));
	if (ret != ARRAY_SIZE(rn6752->mclk_val)) {
		LOG_E("vic: mclk pin config error. ret:%d", ret);
		rn6752->mclk_valid = 0;
	}
#endif
	return RT_EOK;
}

static rt_err_t __rn6752_prepare(void *hdl)
{
	return __rn6752_parser_config(hdl);
}

/* Init sensor config and check chip id */
static rt_err_t __rn6752_init(void *hdl)
{
	rt_int32_t ret = 0;
	rt_uint8_t tmp[2] = {0};
	rt_uint16_t id = 0;
	struct video_dev *rn6752 = (struct video_dev *)hdl;
	char *module = DRV_VIC_RN6752_PWR_NAME;

	RT_ASSERT(RT_NULL != rn6752);
	rn6752->pctrl = pinctrl_get(DRV_VIC_RN6752_NAME);
	if (RT_NULL == rn6752->pctrl)
		return -RT_ERROR;

	/* Init gpio */
	/* cam_power_enable(); */
	camera_ldo_set(vol_1p85v);
	cam_power_set(module);
	if (1 == rn6752->rst_valid) {
		rn6752->rst_gpio = pinctrl_gpio_request(rn6752->pctrl,
					rn6752->rst_val[0], rn6752->rst_val[1]);
		if (rn6752->rst_gpio >= 0) {
			pinctrl_gpio_set_function(rn6752->pctrl, rn6752->rst_gpio,
						rn6752->rst_val[2]);
			pinctrl_gpio_set_drv_level(rn6752->pctrl, rn6752->rst_gpio,
						rn6752->rst_val[3]);
			pinctrl_gpio_set_pud_mode(rn6752->pctrl, rn6752->rst_gpio,
						rn6752->rst_val[4]);
			pinctrl_gpio_set_pud_res(rn6752->pctrl, rn6752->rst_gpio,
						rn6752->rst_val[5]);
			pinctrl_gpio_set_value(rn6752->pctrl, rn6752->rst_gpio,
						rn6752->rst_val[6]);
		} else
			rn6752->rst_valid = 0;
	}
	if (1 == rn6752->mclk_valid) {
		rn6752->mclk_gpio = pinctrl_gpio_request(rn6752->pctrl,
					rn6752->mclk_val[0], rn6752->mclk_val[1]);
		if (rn6752->mclk_gpio >= 0) {
			pinctrl_gpio_set_function(rn6752->pctrl, rn6752->mclk_gpio,
						rn6752->mclk_val[2]);
			pinctrl_gpio_set_drv_level(rn6752->pctrl, rn6752->mclk_gpio,
						rn6752->mclk_val[3]);
			pinctrl_gpio_set_pud_mode(rn6752->pctrl, rn6752->mclk_gpio,
						rn6752->mclk_val[4]);
			pinctrl_gpio_set_pud_res(rn6752->pctrl, rn6752->mclk_gpio,
						rn6752->mclk_val[5]);
			pinctrl_gpio_set_value(rn6752->pctrl, rn6752->mclk_gpio,
						rn6752->mclk_val[6]);
		} else
			rn6752->mclk_valid = 0;
	}
	__rn6752_set_power(rn6752, RT_TRUE);

	/* check chip id */
	rn6752->i2c_client = rt_zalloc(sizeof(struct viss_i2c_client));
	if (RT_NULL == rn6752->i2c_client)
		return -RT_ENOMEM;
	rn6752->i2c_client->i2c_bus = rt_i2c_bus_device_find(rn6752->info.i2c_bus_name);
	if (RT_NULL == rn6752->i2c_client->i2c_bus) {
		LOG_E("can't find bus dev \"%s\"", rn6752->info.i2c_bus_name);
		goto exit;
	}
	rn6752->i2c_client->i2c_addr = rn6752->info.i2c_addr;
	ret = viss_i2c_read_reg_8bit(rn6752->i2c_client, 0xFD, &tmp[0]);
	/* LOG_D("ret: %d. 0xfd: %x", ret, tmp[0]); */
	ret = viss_i2c_read_reg_8bit(rn6752->i2c_client, 0xFE, &tmp[1]);
	/* LOG_D("ret: %d. 0xfe: %x", ret, tmp[1]); */
	id = (tmp[1] << 8) | tmp[0];
	if (id != 0x0401) {
		LOG_E("ID wrong! (0x%x)\n", id);
		goto exit;
	}
	return RT_EOK;

exit:
	if (rn6752->pctrl) {
		if (rn6752->rst_valid)
			pinctrl_gpio_free(rn6752->pctrl, rn6752->rst_gpio);
		if (rn6752->pwdn_valid)
			pinctrl_gpio_free(rn6752->pctrl, rn6752->pwdn_gpio);
		if (rn6752->mclk_valid)
			pinctrl_gpio_free(rn6752->pctrl, rn6752->mclk_gpio);
		pinctrl_put(rn6752->pctrl);
		rn6752->pctrl = RT_NULL;
	}
	if (rn6752->i2c_client) {
		rt_free(rn6752->i2c_client);
		rn6752->i2c_client = RT_NULL;
	}
	cam_power_exit(module);
	camera_ldo_exit();
	/* cam_power_disable(); */

	return -RT_ERROR;
}

static void __rn6752_exit(void *hdl)
{
	struct video_dev *rn6752 = (struct video_dev *)hdl;
	char *module = DRV_VIC_RN6752_PWR_NAME;
	__rn6752_set_power(rn6752, RT_FALSE);
	if (rn6752->pctrl) {
		if (rn6752->rst_valid)
			pinctrl_gpio_free(rn6752->pctrl, rn6752->rst_gpio);
		if (rn6752->pwdn_valid)
			pinctrl_gpio_free(rn6752->pctrl, rn6752->pwdn_gpio);
		if (rn6752->mclk_valid)
			pinctrl_gpio_free(rn6752->pctrl, rn6752->mclk_gpio);
		pinctrl_put(rn6752->pctrl);
		rn6752->pctrl = RT_NULL;
	}
	if (rn6752->i2c_client) {
		rt_free(rn6752->i2c_client);
		rn6752->i2c_client = RT_NULL;
	}
	cam_power_exit(module);
	camera_ldo_exit();
	/* cam_power_disable(); */
}

struct video_dev rn6752_dvp = {
	.name = DRV_VIC_RN6752_NAME,
	.group_id = GRP_ID_VIC,
	.prepare = __rn6752_prepare,
	.init = __rn6752_init,
	.exit = __rn6752_exit,
	.s_mode = __rn6752_set_mode,
	.g_cur_mode = __rn6752_cur_mode,
	.g_all_mode = __rn6752_get_all_mode,
	.s_power = __rn6752_set_power,
	.s_stream = __rn6752_set_stream,
	.g_info = __rn6752_get_info,
	.ioctl = __rn6752_ioctl,
	.s_register = __rn6752_set_register,
	.g_register = __rn6752_get_register,
};

