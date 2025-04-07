/*
 * rn6752m_dvp.c - rn6752m driver code for LomboTech
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

#define DBG_SECTION_NAME	"RN6752M-DVP"
#define DBG_LEVEL		DBG_LOG

#include <debug.h>
#include "viss.h"
#include "vic_dev.h"
#include "viss_i2c.h"
#include "viss_cam_power.h"
#include <div.h>

#ifdef RT_USING_VIC_DET_SIGNAL
#include "system/system_mq.h"
static rt_uint8_t rn6752m_in;
#endif

struct sensor_reg {
	rt_uint16_t reg_add;
	rt_uint16_t reg_value;
};

#define DRV_VIC_RN6752M_NAME "rn6752m-dvp"
#define DRV_VIC_RN6752M_PWR_NAME "cam-pwr-en"
#define RN6752_DVP_REG_DELAY_FLAG  (0xffff)

static const struct sensor_reg rn6752m_yuv_1080p_30fps[] = {
	{ 0x80, 0x34 },
	{ RN6752_DVP_REG_DELAY_FLAG, 20},
	{ 0x81, 0x01 },
	{ 0xA3, 0x04 },
	{ 0xDF, 0xFE },
	{ 0xF0, 0xC0 },
	{ 0x88, 0x40 },
	{ 0xF6, 0x40 },
	{ 0xFF, 0x00 },
	{ 0x00, 0x20 },
	{ 0x06, 0x08 },
	{ 0x07, 0x63 },
	{ 0x2A, 0x01 },
	{ 0x3A, 0x00 },
	{ 0x3F, 0x10 },
	{ 0x4C, 0x37 },
	{ 0x4F, 0x03 },
	{ 0x50, 0x03 },
	{ 0x56, 0x02 },
	{ 0x5F, 0x44 },
	{ 0x63, 0xF8 },
	{ 0x59, 0x00 },
	{ 0x5A, 0x49 },
	{ 0x58, 0x01 },
	{ 0x59, 0x33 },
	{ 0x5A, 0x23 },
	{ 0x58, 0x01 },
	{ 0x51, 0xF4 },
	{ 0x52, 0x29 },
	{ 0x53, 0x15 },
	{ 0x5B, 0x01 },
	{ 0x5E, 0x08 },
	{ 0x6A, 0x87 },
	{ 0x28, 0x92 },
	{ 0x03, 0x80 },
	{ 0x04, 0x80 },
	{ 0x05, 0x04 },
	{ 0x57, 0x23 },
	{ 0x68, 0x00 },
	{ 0x37, 0x33 },
	{ 0x61, 0x6C },
	{ 0x8E, 0x00 },
	{ 0x8F, 0x80 },
	{ 0x8D, 0x31 },
	{ 0x89, 0x0A },
	{ 0x88, 0x41 },

};

static const struct sensor_reg rn6752m_yuv_1080p_25fps[] = {

	/* 1080P@25 BT656
	* Slave address is 0x58
	* Register, data
	* if clock source(Xin) of RN6752 is 26MHz, please add these procedu
	* 0xD2, 0x85, disable auto clock detect
	* 0xD6, 0x37, 27MHz default
	* 0xD8, 0x18, switch to 26MHz clock
	* delay(100), delay 100ms
	*/
	{ 0x80, 0x34 },
	{ RN6752_DVP_REG_DELAY_FLAG, 20},
	{ 0x81, 0x01 }, /* turn on video decoder */
	{ 0xA3, 0x04 },
	{ 0xDF, 0xFE }, /* enable HD format */
	{ 0xF0, 0xC0 },
	{ 0x88, 0x40 }, /* disable SCLK0B out */
	{ 0xF6, 0x40 }, /* disable SCLK3A out */
	{ 0xFF, 0x00 }, /* switch to ch0 (default; optional) */
	{ 0x00, 0x20 }, /* internal use* */
	{ 0x06, 0x08 }, /* internal use* */
	{ 0x07, 0x63 }, /* HD format */
	{ 0x2A, 0x01 }, /* filter control */
	{ 0x3A, 0x00 }, /* No Insert Channel ID in SAV/EAV code */
	{ 0x3F, 0x10 }, /* channel ID */
	{ 0x4C, 0x37 }, /* equalizer */
	{ 0x4F, 0x03 }, /* sync control */
	{ 0x50, 0x03 }, /* 1080p resolution */
	{ 0x56, 0x02 }, /* 144M and BT656 mode */
	{ 0x5F, 0x44 }, /* blank level */
	{ 0x63, 0xF8 }, /* filter control */
	{ 0x59, 0x00 }, /* extended register access */
	{ 0x5A, 0x48 }, /* data for extended register//48 */
	{ 0x58, 0x01 }, /* enable extended register write */
	{ 0x59, 0x33 }, /* extended register access */
	{ 0x5A, 0x23 }, /* data for extended register */
	{ 0x58, 0x01 }, /* enable extended register write */
	{ 0x51, 0xF4 }, /* scale factor1 */
	{ 0x52, 0x29 }, /* scale factor2 */
	{ 0x53, 0x15 }, /* scale factor3 */
	{ 0x5B, 0x01 }, /* H-scaling control */
	{ 0x5E, 0x08 }, /* enable H-scaling control */
	{ 0x6A, 0x87 }, /* H-scaling control */
	{ 0x28, 0x92 }, /* cropping */
	{ 0x03, 0x80 }, /* saturation */
	{ 0x04, 0x80 }, /* hue */
	{ 0x05, 0x00 }, /* sharpness	0x04 */
	{ 0x57, 0x23 }, /* black/white stretch */
	{ 0x68, 0x00 }, /* coring */
	{ 0x37, 0x33 },
	{ 0x61, 0x6C },
	{ 0x1A, 0x83 },
	{ 0x8E, 0x00 }, /* single channel output for VP */
	{ 0x8F, 0x80 }, /* 1080p mode for VP */
	{ 0x8D, 0x31 }, /* enable VP out */
	{ 0x89, 0x0A }, /* select 144MHz for SCLK */
	{ 0x88, 0x41 }, /* enable SCLK out */
};

static const struct sensor_reg rn6752m_yuv_720p_25fps[] = {
	/*
	* if clock source(Xin) of RN675x is 26MHz,
	* please add these procedures marked first
	* { 0xD2, 0x85 }, disable auto clock detect
	* { 0xD6, 0x37 },  27MHz default
	* { 0xD8, 0x18 },  switch to 26MHz clock
	* mdelay(100 },  delay 100ms
	*/
	{ 0x80, 0x34 },
	{ RN6752_DVP_REG_DELAY_FLAG, 20},
	/*  720P@25 BT656
	 *  Slave address is 0x58
	 *  Register, data
	 *  if clock source(Xin) of RN6752 is 26MHz, please add these proced
	 *  0xD2, 0x85, disable auto clock detect
	 *  0xD6, 0x37, 27MHz default
	 *  0xD8, 0x18, switch to 26MHz clock
	 *  delay(100), delay 100ms
	*/
	{ 0x81, 0x01 }, /* turn on video decoder */
	{ 0xA3, 0x04 },
	{ 0xDF, 0xFE }, /* enable HD format */
	{ 0x88, 0x40 }, /* disable SCLK0B out */
	{ 0xF6, 0x40 }, /* disable SCLK3A out */
	{ 0xFF, 0x00 }, /* switch to ch0 (default; optional) */
	{ 0x00, 0x20 }, /* internal use* */
	{ 0x06, 0x08 }, /* internal use* */
	{ 0x07, 0x63 }, /* HD format */
	{ 0x2A, 0x01 }, /* filter control */
	{ 0x3A, 0x00 }, /* No Insert Channel ID in SAV/EAV c */
	{ 0x3F, 0x10 }, /* channel ID */
	{ 0x4C, 0x37 }, /* equalizer */
	{ 0x4F, 0x03 }, /* sync control */
	{ 0x50, 0x02 }, /* 720p resolution */
	{ 0x56, 0x01 }, /* 72M mode and BT656 mode */
	{ 0x5F, 0x40 }, /* blank level	 */
	{ 0x63, 0xF5 }, /* filter control */
	{ 0x59, 0x00 }, /* extended register access */
	{ 0x5A, 0x42 }, /* data for extended register */
	{ 0x58, 0x01 }, /* enable extended register write */
	{ 0x59, 0x33 }, /* extended register access */
	{ 0x5A, 0x23 }, /* data for extended register */
	{ 0x58, 0x01 }, /* enable extended register write */
	{ 0x51, 0xE1 }, /* scale factor1 */
	{ 0x52, 0x88 }, /* scale factor2 */
	{ 0x53, 0x12 }, /* scale factor3 */
	{ 0x5B, 0x07 }, /* H-scaling control */
	{ 0x5E, 0x08 }, /* enable H-scaling control */
	{ 0x6A, 0x82 }, /* H-scaling control */
	{ 0x28, 0x92 }, /* cropping */
	{ 0x03, 0x80 }, /* saturation */
	{ 0x04, 0x80 }, /* hue	*/
	{ 0x05, 0x00 }, /* sharpness */
	{ 0x57, 0x23 }, /* black/white stretch	 */
	{ 0x68, 0x32 }, /* coring */
	{ 0x37, 0x33 },
	{ 0x61, 0x6C },
	{ 0x1A, 0x83 },
	/* { 0x00, 0xC0 }, /* 100%color bar enable */
	{ 0x8E, 0x00 }, /* single channel output for VP */
	{ 0x8F, 0x80 }, /* 720p mode for VP */
	{ 0x8D, 0x31 }, /* enable VP out */
	{ 0x89, 0x09 }, /* select 72MHz for SCLK */
	{ 0x88, 0x41 }, /* enable SCLK out */
};

static struct dev_mode rn6752m_dvp_mode[] = {
	{
		.index = 0,
		.inp_fmt = VISS_MBUS_FMT_UYVY8_2X8,
		.out_fmt = VISS_PIX_FMT_NV12,
		.colorspace = DEV_COLORSPACE_JPEG,
		.frame_rate = 25000,   /* 25fps */
		.frame_size.width = 1280,
		.frame_size.height = 720,
		.usr_data = (void *)rn6752m_yuv_720p_25fps,
		.usr_data_size = ARRAY_SIZE(rn6752m_yuv_720p_25fps),
	},

	{
		.index = 1,
		.inp_fmt = VISS_MBUS_FMT_UYVY8_2X8,
		.out_fmt = VISS_PIX_FMT_NV12,
		.colorspace = DEV_COLORSPACE_JPEG,
		.frame_rate = 25000,   /* 25fps */
		.frame_size.width = 1920,
		.frame_size.height = 1080,
		.usr_data = (void *)rn6752m_yuv_1080p_25fps,
		.usr_data_size = ARRAY_SIZE(rn6752m_yuv_1080p_25fps),
	},
	{
		.index = 2,
		.inp_fmt = VISS_MBUS_FMT_UYVY8_2X8,
		.out_fmt = VISS_PIX_FMT_NV12,
		.colorspace = DEV_COLORSPACE_JPEG,
		.frame_rate = 30000,   /* 30fps */
		.frame_size.width = 1920,
		.frame_size.height = 1080,
		.usr_data = (void *)rn6752m_yuv_1080p_30fps,
		.usr_data_size = ARRAY_SIZE(rn6752m_yuv_1080p_30fps),
	},
};

static rt_err_t __rn6752m_block_write(void *hdl, void *data, rt_int32_t size)
{
	struct video_dev *rn6752m = (struct video_dev *)hdl;
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
		ret = viss_i2c_write_reg_8bit(rn6752m->i2c_client,
					reg[i].reg_add, reg[i].reg_value);
		if (RT_EOK != ret) {
			LOG_E("reg[i].reg_add : %x\n", reg[i].reg_add);
			return ret;
		}
	}

	return RT_EOK;
}

static struct dev_mode *__rn6752m_cur_mode(void *hdl)
{
	struct video_dev *rn6752m = (struct video_dev *)hdl;

	return rn6752m->cur_mode;
}

static struct dev_mode *__rn6752m_get_all_mode(void *hdl, rt_int32_t *num)
{
	RT_ASSERT(RT_NULL != num);
	*num = (rt_int32_t)ARRAY_SIZE(rn6752m_dvp_mode);

	return rn6752m_dvp_mode;
}

static rt_err_t __rn6752m_set_power(void *hdl, rt_bool_t on)
{
	struct video_dev *rn6752m = (struct video_dev *)hdl;

	if (1 != rn6752m->rst_valid)
		return RT_EOK;

	if (RT_TRUE == on) {
		/* LOG_D("Rn6752m reset"); */
		pinctrl_gpio_set_value(rn6752m->pctrl, rn6752m->rst_gpio, 0);
		rt_thread_delay(1); /* 30ms 1*/
		pinctrl_gpio_set_value(rn6752m->pctrl, rn6752m->rst_gpio, 1);
		rt_thread_delay(6); /* 100ms 6*/
	} else {
		/* TO DO */
	}

	return RT_EOK;
}

static rt_err_t __rn6752m_set_mode(void *hdl, rt_int32_t index)
{
	struct video_dev *rn6752m = (struct video_dev *)hdl;
	rt_int32_t num = 0;
	rt_err_t ret = 0;

	num = (rt_int32_t)ARRAY_SIZE(rn6752m_dvp_mode);
	if (index >= num) {
		LOG_E("Input mode index error. num:%d index:%d\n", num, index);
		return -RT_EINVAL;
	}
	if (index >= ARRAY_SIZE(rn6752m_dvp_mode)) {
		LOG_E("Input para error. index:%d\n", index);
		return -RT_EINVAL;
	}
	/* LOG_D("set_mode index:%d", index); */
	rn6752m->cur_mode = &rn6752m_dvp_mode[index];
	ret = __rn6752m_block_write((void *)rn6752m, rn6752m->cur_mode->usr_data,
			rn6752m->cur_mode->usr_data_size);
	return ret;
}


static rt_err_t __rn6752m_set_register(void *hdl, struct viss_dbg_register *reg)
{
	return RT_EOK;
}

static rt_err_t __rn6752m_get_register(void *hdl, struct viss_dbg_register *reg)
{
	return RT_EOK;
}

static rt_err_t __rn6752m_set_stream(void *hdl, rt_bool_t enable)
{
	struct video_dev *rn6752m = (struct video_dev *)hdl;
	if (enable)
		viss_i2c_write_reg_8bit(rn6752m->i2c_client, 0x80, 0x30);
	else
		viss_i2c_write_reg_8bit(rn6752m->i2c_client, 0x80, 0x34);
	return RT_EOK;
}


static rt_err_t __rn6752m_ioctl(void *hdl, rt_int32_t cmd, void *para)
{
	rt_err_t ret = 0;
	rt_uint8_t tmp = 0;
	struct video_dev *rn6752m = (struct video_dev *)hdl;
	if (RT_NULL == hdl) {
		LOG_E("__rn6752m_ioctl fail.");
		return -RT_ERROR;
	}
	ret = viss_i2c_read_reg_8bit(rn6752m->i2c_client, 0x00, &tmp);
	if (tmp & (1<<4)) {
#ifdef RT_USING_VIC_DET_SIGNAL
		if (rn6752m_in != 0) {
			rn6752m_in = 0;
			lb_system_mq_send(LB_SYSMSG_AV_PLUGOUT, NULL, 0, 0);
			LOG_E("__rn6752m_ioctl LB_SYSMSG_AV_PLUGOUT.");
		}
#endif
		return 1;
	} else {
#ifdef RT_USING_VIC_DET_SIGNAL
		if (rn6752m_in != 1) {
			rn6752m_in = 1;
			lb_system_mq_send(LB_SYSMSG_AV_PLUGIN, NULL, 0, 0);
			LOG_E("__rn6752m_ioctl LB_SYSMSG_AV_PLUGIN.");
		}
#endif
		return 0;
	}
	return ret;
}

static rt_err_t __rn6752m_get_info(void *hdl, struct viss_source_info *info)
{
	struct video_dev *rn6752m = (struct video_dev *)hdl;

	if ((RT_NULL == hdl) || (RT_NULL == info)) {
		LOG_E("Get rn6752m information fail.");
		return -RT_ERROR;
	}
	rt_memcpy(info, &rn6752m->info, sizeof(struct viss_source_info));

	return RT_EOK;
}

static rt_err_t __rn6752m_parser_config(void *hdl)
{
	struct video_dev *rn6752m = (struct video_dev *)hdl;
	struct viss_source_info info;
	rt_uint32_t tmp_data = 0;
	const char *status;
	const char *i2c_bus_name;
	const char *module = DRV_VIC_RN6752M_NAME;
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
		LOG_W("rn6752m dvp disable.");
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
	rt_memcpy(&rn6752m->info, &info, sizeof(struct viss_source_info));

	/* reset pin */
	rn6752m->rst_valid = 1;
	ret = config_get_u32_array(module, "dvp-rst",
				rn6752m->rst_val, ARRAY_SIZE(rn6752m->rst_val));
	if (ret != ARRAY_SIZE(rn6752m->rst_val)) {
		LOG_E("vic: reset pin config error. ret:%d", ret);
		rn6752m->rst_valid = 0;
	}
#if 0
	/* mclk pin */
	rn6752m->mclk_valid = 1;
	ret = config_get_u32_array(module, "dvp-mclk",
				rn6752m->mclk_val, ARRAY_SIZE(rn6752m->mclk_val));
	if (ret != ARRAY_SIZE(rn6752m->mclk_val)) {
		LOG_E("vic: mclk pin config error. ret:%d", ret);
		rn6752m->mclk_valid = 0;
	}
#endif
	return RT_EOK;
}

static rt_err_t __rn6752m_prepare(void *hdl)
{
	return __rn6752m_parser_config(hdl);
}

/* Init sensor config and check chip id */
static rt_err_t __rn6752m_init(void *hdl)
{
	rt_int32_t ret = 0;
	rt_uint8_t tmp[2] = {0};
	rt_uint16_t id = 0;
	struct video_dev *rn6752m = (struct video_dev *)hdl;
	char *module = DRV_VIC_RN6752M_PWR_NAME;

	RT_ASSERT(RT_NULL != rn6752m);
	rn6752m->pctrl = pinctrl_get(DRV_VIC_RN6752M_NAME);
	if (RT_NULL == rn6752m->pctrl)
		return -RT_ERROR;

	/* Init gpio */
	/* cam_power_enable(); */
	camera_ldo_set(vol_1p85v);
	cam_power_set(module);
	if (1 == rn6752m->rst_valid) {
		rn6752m->rst_gpio = pinctrl_gpio_request(rn6752m->pctrl,
					rn6752m->rst_val[0], rn6752m->rst_val[1]);
		if (rn6752m->rst_gpio >= 0) {
			pinctrl_gpio_set_function(rn6752m->pctrl, rn6752m->rst_gpio,
						rn6752m->rst_val[2]);
			pinctrl_gpio_set_drv_level(rn6752m->pctrl, rn6752m->rst_gpio,
						rn6752m->rst_val[3]);
			pinctrl_gpio_set_pud_mode(rn6752m->pctrl, rn6752m->rst_gpio,
						rn6752m->rst_val[4]);
			pinctrl_gpio_set_pud_res(rn6752m->pctrl, rn6752m->rst_gpio,
						rn6752m->rst_val[5]);
			pinctrl_gpio_set_value(rn6752m->pctrl, rn6752m->rst_gpio,
						rn6752m->rst_val[6]);
		} else
			rn6752m->rst_valid = 0;
	}
	if (1 == rn6752m->mclk_valid) {
		rn6752m->mclk_gpio = pinctrl_gpio_request(rn6752m->pctrl,
					rn6752m->mclk_val[0], rn6752m->mclk_val[1]);
		if (rn6752m->mclk_gpio >= 0) {
			pinctrl_gpio_set_function(rn6752m->pctrl, rn6752m->mclk_gpio,
						rn6752m->mclk_val[2]);
			pinctrl_gpio_set_drv_level(rn6752m->pctrl, rn6752m->mclk_gpio,
						rn6752m->mclk_val[3]);
			pinctrl_gpio_set_pud_mode(rn6752m->pctrl, rn6752m->mclk_gpio,
						rn6752m->mclk_val[4]);
			pinctrl_gpio_set_pud_res(rn6752m->pctrl, rn6752m->mclk_gpio,
						rn6752m->mclk_val[5]);
			pinctrl_gpio_set_value(rn6752m->pctrl, rn6752m->mclk_gpio,
						rn6752m->mclk_val[6]);
		} else
			rn6752m->mclk_valid = 0;
	}
	__rn6752m_set_power(rn6752m, RT_TRUE);

	/* check chip id */
	rn6752m->i2c_client = rt_zalloc(sizeof(struct viss_i2c_client));
	if (RT_NULL == rn6752m->i2c_client)
		return -RT_ENOMEM;
	rn6752m->i2c_client->i2c_bus = rt_i2c_bus_device_find(rn6752m->info.i2c_bus_name);
	if (RT_NULL == rn6752m->i2c_client->i2c_bus) {
		LOG_E("can't find bus dev \"%s\"", rn6752m->info.i2c_bus_name);
		goto exit;
	}
	rn6752m->i2c_client->i2c_addr = rn6752m->info.i2c_addr;
	ret = viss_i2c_read_reg_8bit(rn6752m->i2c_client, 0xFD, &tmp[0]);
	/* LOG_D("ret: %d. 0xfd: %x", ret, tmp[0]); */
	ret = viss_i2c_read_reg_8bit(rn6752m->i2c_client, 0xFE, &tmp[1]);
	/* LOG_D("ret: %d. 0xfe: %x", ret, tmp[1]); */
	id = (tmp[1] << 8) | tmp[0];
	if (id != 0x0501) {
		LOG_E("ID wrong! (0x%x)\n", id);
		goto exit;
	}
	return RT_EOK;

exit:
	if (rn6752m->pctrl) {
		if (rn6752m->rst_valid)
			pinctrl_gpio_free(rn6752m->pctrl, rn6752m->rst_gpio);
		if (rn6752m->pwdn_valid)
			pinctrl_gpio_free(rn6752m->pctrl, rn6752m->pwdn_gpio);
		if (rn6752m->mclk_valid)
			pinctrl_gpio_free(rn6752m->pctrl, rn6752m->mclk_gpio);
		pinctrl_put(rn6752m->pctrl);
		rn6752m->pctrl = RT_NULL;
	}
	if (rn6752m->i2c_client) {
		rt_free(rn6752m->i2c_client);
		rn6752m->i2c_client = RT_NULL;
	}
	cam_power_exit(module);
	camera_ldo_exit();
	/* cam_power_disable(); */

	return -RT_ERROR;
}

static void __rn6752m_exit(void *hdl)
{
	struct video_dev *rn6752m = (struct video_dev *)hdl;
	char *module = DRV_VIC_RN6752M_PWR_NAME;

	__rn6752m_set_power(rn6752m, RT_FALSE);
	if (rn6752m->pctrl) {
		if (rn6752m->rst_valid)
			pinctrl_gpio_free(rn6752m->pctrl, rn6752m->rst_gpio);
		if (rn6752m->pwdn_valid)
			pinctrl_gpio_free(rn6752m->pctrl, rn6752m->pwdn_gpio);
		if (rn6752m->mclk_valid)
			pinctrl_gpio_free(rn6752m->pctrl, rn6752m->mclk_gpio);
		pinctrl_put(rn6752m->pctrl);
		rn6752m->pctrl = RT_NULL;
	}
	if (rn6752m->i2c_client) {
		rt_free(rn6752m->i2c_client);
		rn6752m->i2c_client = RT_NULL;
	}
	cam_power_exit(module);
	camera_ldo_exit();
	/* cam_power_disable(); */
}

struct video_dev rn6752m_dvp = {
	.name = DRV_VIC_RN6752M_NAME,
	.group_id = GRP_ID_VIC,
	.prepare = __rn6752m_prepare,
	.init = __rn6752m_init,
	.exit = __rn6752m_exit,
	.s_mode = __rn6752m_set_mode,
	.g_cur_mode = __rn6752m_cur_mode,
	.g_all_mode = __rn6752m_get_all_mode,
	.s_power = __rn6752m_set_power,
	.s_stream = __rn6752m_set_stream,
	.g_info = __rn6752m_get_info,
	.ioctl = __rn6752m_ioctl,
	.s_register = __rn6752m_set_register,
	.g_register = __rn6752m_get_register,
};

