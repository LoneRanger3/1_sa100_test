/*
 * isp_dev_gc2053.c - gc2053 driver code for LomboTech
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

#define DBG_SECTION_NAME	"IMX307-MIPI"
#define DBG_LEVEL		DBG_INFO

#include <debug.h>
#include "viss.h"
#include "mcsi_dev.h"
#include "viss_i2c.h"
#include "viss_cam_power.h"
#include <div.h>

struct sensor_reg {
	rt_uint16_t reg_add;
	rt_uint8_t reg_value;
};
static u32 cur_expline = -1;
struct video_dev *imx307_global;
u32 SENSOR_MAX_GAIN = 15.5 * 31;

static u32 cur_again = -1;
static u32 cur_dgain = -1;

pthread_t imx307_tid;
rt_uint32_t imx307_exit_flag;
rt_sem_t imx307_switch_gain_sem; /* wait isp irq feed dog */

#define DRV_MCSI_IMX307_NAME "imx307-mipi"
#define DRV_MCSI_IMX307_PWR_NAME "cam-pwr-en"
#define IMX307_MIPI_REG_DELAY_FLAG  (0xfff)
#define MAX_AGAIN (15 * 1024 + 512)
#define MAX_DGAIN (12 * 1024)
#define VMAX (0x465)

/* static struct viss_i2c_client *imx307_mipi_i2c_client = RT_NULL; */

/*
 * 2lane YUV init
 */
static const struct sensor_reg imx307_reg_list[] = {
	/* sensor stanby */
	{0x3000, 0x01},
	{0x3001, 0x01},
	{0x3002, 0x01},
	/* setting */
	{0x3005, 0x01},
	{0x3007, 0x00},
	{0x3009, 0x02},
	{0x300A, 0xF0},
	{0x300B, 0x00},
	{0x3011, 0x0A},
	{0x3018, VMAX&0xFF},/* V max */
	{0x3019, VMAX>>8},
	{0x301C, 0x30},
	{0x301D, 0x11},/* H max */
	{0x3046, 0x01},
	{0x304B, 0x0A},
	{0x305C, 0x18},
	{0x305D, 0x03},
	{0x305E, 0x20},
	{0x305F, 0x01},
	{0x309E, 0x4A},
	{0x309F, 0x4A},

	{0x311C, 0x0E},
	{0x3128, 0x04},
	{0x3129, 0x00},
	{0x313B, 0x41},
	{0x315E, 0x1A},
	{0x3164, 0x1A},
	{0x317C, 0x00},
	{0x31EC, 0x0E},

	/* These registers are set in CSI-2 interface only. */
	{0x3405, 0x10},
	{0x3407, 0x01},
	{0x3414, 0x0A},
	{0x3418, 0x49},
	{0x3419, 0x04},/* y-out size */
	{0x3441, 0x0C},/* 0x0A: RAW10, 0x0C: RAW12 */
	{0x3442, 0x0C},/* 0x0A: RAW10, 0x0C: RAW12 */
	{0x3443, 0x01},
	{0x3444, 0x20},
	{0x3445, 0x25},
	{0x3446, 0x57},
	{0x3447, 0x00},
	{0x3448, 0x37},
	{0x3449, 0x00},
	{0x344A, 0x1F},
	{0x344B, 0x00},
	{0x344C, 0x1F},
	{0x344D, 0x00},
	{0x344E, 0x1F},
	{0x344F, 0x00},
	{0x3450, 0x77},
	{0x3451, 0x00},
	{0x3452, 0x1F},
	{0x3453, 0x00},
	{0x3454, 0x17},
	{0x3455, 0x00},
	{0x3472, 0x9c},
	{0x3473, 0x07},/* x-out size */
	{0x3480, 0x49},

	/* stanby cancel */
	/* {0x3001, 0x00}, */
	{0x3002, 0x00},
	/*
	{0x3000, 0x00},
	{0x3001, 0x00},
	{0x3002, 0x00},
	*/
	/* 0x308c, 0x11, */
	/* gain: 0x3014 0x3009 */
	/* shutter: 0x3020 0x3021 0x3022*/

	/* gain: 0x34a8, 0x34a9 */
	/* shutter: 0x34b1 0x34b2 0x34b3 */

};

struct dev_mode *cur_imx307_mipi_mode = RT_NULL;
static struct dev_mode imx307_mipi_mode[] = {
	{
		.index = 0,
		.inp_fmt = VISS_MBUS_FMT_YUYV8_2X8,
		.out_fmt = VISS_PIX_FMT_NV12,
		.colorspace = DEV_COLORSPACE_JPEG,
		.frame_rate = 30000,   /* 30fps */
		.frame_size.width = 1920,
		.frame_size.height = 1080,
		.usr_data = (void *)imx307_reg_list,
		.usr_data_size = ARRAY_SIZE(imx307_reg_list),
	},
	{
		.index = 1,
		.inp_fmt = VISS_MBUS_FMT_YUYV8_2X8,
		.out_fmt = VISS_PIX_FMT_NV12,
		.colorspace = DEV_COLORSPACE_JPEG,
		.frame_rate = 30000,   /* 30fps */
		.frame_size.width = 1280,
		.frame_size.height = 720,
		.usr_data = (void *)imx307_reg_list,
		.usr_data_size = ARRAY_SIZE(imx307_reg_list),
	},
};

static rt_err_t __imx307_mipi_wake_up(void *hdl)
{
	/* todo */
	struct video_dev *imx307 = (struct video_dev *)hdl;
	return 0;
}

static rt_err_t __imx307_mipi_block_write(void *hdl,
						void *data, rt_int32_t size)
{
	struct video_dev *imx307 = (struct video_dev *)hdl;
	struct sensor_reg *reg = (struct sensor_reg *)data;
	rt_int32_t i = 0;
	rt_err_t ret = 0;
	rt_int32_t delay = 0, temp = 0;

	for (i = 0; i < size; i++) {
		if (IMX307_MIPI_REG_DELAY_FLAG == reg[i].reg_add) {
			temp = reg[i].reg_value + 9;
			do_div(temp, 10);
			delay = temp * 10;
			LOG_D("delay: %d", delay);
			do_div(delay, 10);
			rt_thread_delay(delay);
			continue;
		}
		ret = viss_i2c_write_reg_16bit(imx307->i2c_client,
					reg[i].reg_add, reg[i].reg_value);
		if (RT_EOK != ret) {
			LOG_E("i2c write %x value:%x fail.\n",
				reg[i].reg_add, reg[i].reg_value);
			return ret;
		}
	}

	return RT_EOK;
}

static struct dev_mode *__imx307_mipi_cur_mode(void *hdl)
{
	struct video_dev *imx307 = (struct video_dev *)hdl;
	return imx307->cur_mode;
}

static struct dev_mode *__imx307_mipi_get_all_mode(void *hdl, rt_int32_t *num)
{
	RT_ASSERT(RT_NULL != num);
	*num = (rt_int32_t)ARRAY_SIZE(imx307_mipi_mode);

	return imx307_mipi_mode;
}
int reg_writed;
static rt_err_t __imx307_mipi_set_mode(void *hdl, rt_int32_t index)
{
	struct video_dev *imx307 = (struct video_dev *)hdl;
	rt_int32_t num = 0;
	rt_err_t ret = 0;

	num = (rt_int32_t)ARRAY_SIZE(imx307_mipi_mode);
	if (index >= num) {
		LOG_E("Input mode index error. num:%d index:%d\n", num, index);
		return -RT_EINVAL;
	}
	if (index >= ARRAY_SIZE(imx307_mipi_mode)) {
		LOG_E("Input para error. index:%d\n", index);
		return -RT_EINVAL;
	}
	imx307->cur_mode = &imx307_mipi_mode[index];
	if (reg_writed)
		return RT_EOK;

	reg_writed = 1;
	ret = __imx307_mipi_block_write(hdl, imx307->cur_mode->usr_data,
				imx307->cur_mode->usr_data_size);
	if (RT_EOK != ret)
		return ret;

	return __imx307_mipi_wake_up(hdl);
}

static void imx307_set_shutter(void *hdl, u32 texp)
{

	int ret = 0;
	u32 vmax, shs;
	struct video_dev *imx307 = (struct video_dev *)hdl;

	if (texp < 2)
		texp = 2;

	if (texp > VMAX - 2)
		vmax = texp + 2;
	else
		vmax = VMAX;
	ret = viss_i2c_write_reg_16bit(imx307->i2c_client,
		0x3018, ((vmax >> 0) & 0xff));
	ret += viss_i2c_write_reg_16bit(imx307->i2c_client,
		0x3019, ((vmax >> 8) & 0xff));

	shs = vmax - (texp + 1);

	ret += viss_i2c_write_reg_16bit(imx307->i2c_client,
		0x3020, ((shs >> 0) & 0xff));
	ret += viss_i2c_write_reg_16bit(imx307->i2c_client,
		0x3021, ((shs >> 8) & 0xff));
	ret += viss_i2c_write_reg_16bit(imx307->i2c_client,
		0x3022, ((shs >> 16) & 0x3));
}

static u32 gain_table[231] = {
	   256,    264,    274,    283,    293,    304,    314,    326,
	   337,    349,    361,    374,    387,    401,    415,    429,
	   444,    460,    476,    493,    510,    528,    547,    566,
	   586,    607,    628,    650,    673,    697,    721,    746,
	   773,    800,    828,    857,    887,    918,    951,    984,
	  1019,   1054,   1092,   1130,   1170,   1211,   1253,   1297,
	  1343,   1390,   1439,   1490,   1542,   1596,   1652,   1710,
	  1771,   1833,   1897,   1964,   2033,   2104,   2178,   2255,
	  2334,   2416,   2501,   2589,   2680,   2774,   2872,   2973,
	  3077,   3185,   3297,   3413,   3533,   3657,   3786,   3919,
	  4057,   4199,   4347,   4500,   4658,   4822,   4991,   5167,
	  5348,   5536,   5731,   5932,   6141,   6356,   6580,   6811,
	  7050,   7298,   7555,   7820,   8095,   8379,   8674,   8979,
	  9294,   9621,   9959,  10309,  10671,  11046,  11435,  11836,
	 12252,  12683,  13129,  13590,  14068,  14562,  15074,  15604,
	 16152,  16720,  17307,  17915,  18545,  19197,  19871,  20570,
	 21293,  22041,  22816,  23617,  24447,  25306,  26196,  27116,
	 28069,  29056,  30077,  31134,  32228,  33361,  34533,  35747,
	 37003,  38303,  39649,  41043,  42485,  43978,  45523,  47123,
	 48779,  50494,  52268,  54105,  56006,  57974,  60012,  62121,
	 64304,  66564,  68903,  71324,  73831,  76425,  79111,  81891,
	 84769,  87748,  90832,  94024,  97328, 100748, 104289, 107954,
	111748, 115675, 119740, 123948, 128303, 132812, 137480, 142311,
	147312, 152489, 157848, 163395, 169137, 175081, 181234, 187603,
	194195, 201020, 208084, 215397, 222966, 230802, 238913, 247309,
	256000, 264996, 274308, 283948, 293927, 304256, 314948, 326016,
	337473, 349333, 361609, 374317, 387471, 401088, 415183, 429773,
	444877, 460510, 476694, 493446, 510787, 528737, 547318, 566552,
	586462, 607071, 628405, 650489, 673348, 697011, 721506
};

static char gain_mode_buf = 0x02;

static void imx307_set_gain(void *hdl, u32 gain)
{

	int ret;
	u8 rdval, db_idx;
	char gain_mode = 0x02;
	struct video_dev *imx307 = (struct video_dev *)hdl;

	if (cur_dgain == gain)
		return;
	cur_dgain  = gain;

	ret = viss_i2c_read_reg_16bit(imx307->i2c_client, 0x3009, &rdval);
	if (ret != 0)
		return ret;

	if (gain < 1 * 256)
		gain = 256;

	for (db_idx = 0; db_idx < 231; db_idx++) {
		if (gain <= gain_table[db_idx])
			break;
	}

	/* 21*0.3db = 6.3db > 2x gain */
	if (db_idx >= 21) {
		gain_mode = rdval | 0x10;
		ret = viss_i2c_write_reg_16bit(imx307->i2c_client, 0x3014, db_idx - 20);
	} else {
		gain_mode = rdval & 0xef;
		ret = viss_i2c_write_reg_16bit(imx307->i2c_client, 0x3014, db_idx);
	}

	gain_mode_buf = gain_mode;
	rt_sem_release(imx307_switch_gain_sem);
}

static void *sensor_gain_switch(void *hdl)
{
	struct video_dev *imx307 = (struct video_dev *)hdl;
	rt_err_t ret = 0;

	while (!imx307_exit_flag) {
		ret = rt_sem_take(imx307_switch_gain_sem, RT_WAITING_FOREVER);
		if (!imx307_exit_flag) {
			rt_thread_delay(5); /* 50ms */
			/* HCG and LCG mode switch */
			viss_i2c_write_reg_16bit(imx307->i2c_client,
			0x3009, gain_mode_buf);
		}

	}
}

rt_err_t __imx307_exp_ctrl(void *hdl, struct isp_exp_gain *exp_gain)
{
	struct video_dev *imx307 = (struct video_dev *)hdl;
	rt_err_t ret = 0;
	u32 gain;
	u32 again, dgain;

	if ((RT_NULL == hdl) || (RT_NULL == exp_gain)) {
		LOG_E("set imx307_exp_ctrl fail.");
		return -RT_ERROR;
	}

	LOG_D("**********exp_ctrl: %d, %d.", exp_gain->gain, exp_gain->exp);

	imx307_set_shutter(imx307, exp_gain->exp / 16);
	imx307_set_gain(imx307, exp_gain->gain);

	return ret;
}
static rt_err_t __imx307_isp_get_sensor_info(void *hdl,
	struct isp_sensor_info *isp_sensor_info)
{
	isp_sensor_info->sensor_name = "imx307";
	isp_sensor_info->pclk = 149*1000*1000;
	isp_sensor_info->vts = 1125;
	isp_sensor_info->hts = 4400;
	isp_sensor_info->input_width = 1920;
	isp_sensor_info->input_height = 1080;
	isp_sensor_info->output_widht = 1920;
	isp_sensor_info->output_height = 1080;
	isp_sensor_info->bayer_mode = ISP_BPAT_RGRGGBGB;

	return RT_EOK;
}
static rt_err_t __imx307_mipi_set_register(void *hdl,
					struct viss_dbg_register *reg)
{
	return RT_EOK;
}

static rt_err_t __imx307_mipi_get_register(void *hdl,
					struct viss_dbg_register *reg)
{
/*
	rt_int32_t i = 0;
	rt_uint8_t reg_value;

	LOG_D("imx307_reg_list>>");
	for (i = 0; i < ARRAY_SIZE(imx307_reg_list); i++) {
		viss_i2c_read_reg_16bit(imx307_mipi_i2c_client,
				imx307_reg_list[i].reg_add, &reg_value);
		rt_kprintf("%x: %x\n", imx307_reg_list[i].reg_add, reg_value);
	}
*/
	return RT_EOK;
}

static rt_err_t __imx307_mipi_set_power(void *hdl, rt_bool_t on)
{
	struct video_dev *imx307 = (struct video_dev *)hdl;

	if ((1 != imx307->pwdn_valid) || (1 != imx307->rst_valid))
		return RT_EOK;
	LOG_E("__imx307_mipi_set_power 1111");

	if (RT_TRUE == on) {
		/* LOG_D("__imx307_mipi_set_power 2222"); */
		pinctrl_gpio_set_value(imx307->pctrl, imx307->rst_gpio, 0);
		rt_thread_delay(1); /* 10ms */
		pinctrl_gpio_set_value(imx307->pctrl, imx307->pwdn_gpio, 0);
		rt_thread_delay(1); /* 10ms */
		pinctrl_gpio_set_value(imx307->pctrl, imx307->pwdn_gpio, 1);
		rt_thread_delay(1); /* 10ms */
		pinctrl_gpio_set_value(imx307->pctrl, imx307->rst_gpio, 1);
	} else {
		/* LOG_D("__imx307_mipi_set_power 222 "); */
		/* TO DO */
	}
	/* LOG_D("__imx307_mipi_set_power 3333"); */

	return RT_EOK;
}

static rt_err_t __imx307_mipi_set_stream(void *hdl, rt_bool_t enable)
{
	rt_int32_t ret = 0;

	struct video_dev *imx307 = (struct video_dev *)hdl;
	if (enable) {
		ret = viss_i2c_write_reg_16bit(imx307->i2c_client, 0x3001, 0x00);
		/* ret = viss_i2c_write_reg_16bit(imx307->i2c_client, 0x3002, 0x00); */
		ret = viss_i2c_write_reg_16bit(imx307->i2c_client, 0x3000, 0x00);
		rt_thread_delay(10);

	} else {
		ret = viss_i2c_write_reg_16bit(imx307->i2c_client, 0x3001, 0x01);
		ret = viss_i2c_write_reg_16bit(imx307->i2c_client, 0x3000, 0x01);
		/* ret = viss_i2c_write_reg_16bit(imx307->i2c_client, 0x3002, 0x01); */
	}
	return ret;
}

rt_err_t __imx307_mipi_ioctl(void *hdl, rt_int32_t cmd, void *para)
{
	rt_err_t ret = 0;
	struct isp_exp_gain *imx307_exp = RT_NULL;
	struct isp_sensor_info *imx307_isp_sensor_info = RT_NULL;

	switch (cmd) {
	case ISP_GET_SENSOR_INFO:
		imx307_isp_sensor_info = (struct isp_sensor_info *)para;
		__imx307_isp_get_sensor_info(hdl, imx307_isp_sensor_info);
		break;
	case ISP_SET_EXP_GAIN:
		imx307_exp = (struct isp_exp_gain *)para;
		__imx307_exp_ctrl(hdl, imx307_exp);
		/* LOG_FLOAT("ISP_SET_EXP_GAIN: exp: %d, gain: %f.",
			ov2710_exp->exp, ov2710_exp->gain); */
		break;
	default:
		return -RT_ERROR;
	}
	return ret;
}

static rt_err_t __imx307_mipi_get_info(void *hdl,
			struct viss_source_info *info)
{
	struct video_dev *imx307 = (struct video_dev *)hdl;
	/* LOG_D("__imx307_mipi_get_info in"); */

	if ((RT_NULL == hdl) || (RT_NULL == info)) {
		LOG_E("Get imx307 information fail.");
		return -RT_ERROR;
	}
	rt_memcpy(info, &imx307->info, sizeof(struct viss_source_info));
	/* LOG_D("__imx307_mipi_get_info out"); */

	return RT_EOK;
}

static rt_err_t __imx307_mipi_parser_config(void *hdl)
{
	struct video_dev *imx307 = (struct video_dev *)hdl;
	struct viss_source_info info;
	rt_uint32_t tmp_data = 0;
	const char *status;
	const char *i2c_bus_name;
	const char *module = DRV_MCSI_IMX307_NAME;
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
		LOG_W("imx307 mipi disable.");
		return -RT_ERROR;
	}
	ret = config_get_string(module, "i2c-bus", &i2c_bus_name);
	if (-1 == ret) {
		LOG_E("Get i2c_bus fail.");
		return -RT_ERROR;
	}
	strncpy(info.i2c_bus_name, i2c_bus_name, sizeof(info.i2c_bus_name) - 1);
	LOG_D("info.i2c_bus_name:%s\n", info.i2c_bus_name);

	ret = config_get_u32(module, "if-type", &info.if_type);
	if (-1 == ret)
		LOG_E("Get if-type fail.");

	LOG_D("info.if_type:%d", info.if_type);
	ret = config_get_u32(module, "out-path", &tmp_data);
	if (-1 == ret) {
		LOG_E("Get out-path fail.");
	} else {
		if (1 == tmp_data)
			info.out_path = VISS_IO_ISP;
		else
			info.out_path = VISS_IO_DMA;
	}
	ret = config_get_u32(module, "data-lanes", &tmp_data);

	if (-1 == ret) {
		LOG_E("Get data-lanes fail.");
		return -RT_ERROR;
	} else {
		if (1 <= tmp_data)
			info.data_lanes = tmp_data;
		else {
			LOG_E("data-lanes config error.");
			return -RT_ERROR;
		}
	}

	ret = config_get_u32(module, "if-mode", &info.if_mode);
	if (-1 == ret)
		info.if_mode = -1;
	ret = config_get_u32(module, "mipi-csi-freq", &info.mipi_csi_clock);
	if (-1 == ret) {
		LOG_E("Get mipi-csi-freq fail.");
		return -RT_ERROR;
	}

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

	info.channel_num = 1;
	ret = config_get_u32(module, "reg", &info.i2c_addr);
	if (-1 == ret)
		LOG_E("Get i2c_addr fail.");
	ret = config_get_u32(module, "mclk-freq", &info.mclk_freq);
	if (-1 == ret)
		LOG_E("Get mclk_freq fail.");
	rt_memcpy(&imx307->info, &info, sizeof(struct viss_source_info));
	/* LOG_D("imx307->info.if_type:%d, lane_num: %d.",
		imx307->info.if_type, imx307->info.data_lanes); */

	/* power pin */
	imx307->pwdn_valid = 1;
	ret = config_get_u32_array(module, "mcsi-pwdn",
				imx307->pwdn_val, ARRAY_SIZE(imx307->pwdn_val));
	if (ret != ARRAY_SIZE(imx307->pwdn_val)) {
		LOG_E("mcsi: power pin config error. ret:%d", ret);
		imx307->pwdn_valid = 0;
	}
	/* LOG_D("imx307 config power"); */

	/* reset pin */
	imx307->rst_valid = 1;
	ret = config_get_u32_array(module, "mcsi-rst",
				imx307->rst_val, ARRAY_SIZE(imx307->rst_val));
	if (ret != ARRAY_SIZE(imx307->rst_val)) {
		LOG_E("mcsi: reset pin config error. ret:%d", ret);
		imx307->rst_valid = 0;
	}
	/* LOG_D("imx307 config reset"); */

	/* mclk pin */
	imx307->mclk_valid = 1;
	ret = config_get_u32_array(module, "mcsi-mclk",
				imx307->mclk_val, ARRAY_SIZE(imx307->mclk_val));
	if (ret != ARRAY_SIZE(imx307->mclk_val)) {
		LOG_E("mcsi: mclk pin config error. ret:%d", ret);
		imx307->mclk_valid = 0;
	}
	/* #endif */
	/* LOG_D("imx307 config finish"); */

	return RT_EOK;
}

static rt_err_t __imx307_mipi_prepare(void *hdl)
{
	return __imx307_mipi_parser_config(hdl);
}

static rt_err_t __imx307_mipi_init(void *hdl)
{
	rt_int32_t ret = 0;
	rt_uint8_t tmp = 0;
	rt_uint16_t id = 0;
	struct video_dev *imx307 = (struct video_dev *)hdl;
	imx307_global = imx307;
	char *module = DRV_MCSI_IMX307_PWR_NAME;
#if 1
	RT_ASSERT(RT_NULL != imx307);
	imx307->pctrl = pinctrl_get(DRV_MCSI_IMX307_NAME);
	if (RT_NULL == imx307->pctrl)
		return -RT_ERROR;
	#ifdef ARCH_LOMBO_N7V1_CDR
	/* Init gpio */
	/* cam_power_enable(); */
	camera_ldo_set(vol_1p85v);
	cam_power_set(module);
	#endif
	if (1 == imx307->pwdn_valid) {
		imx307->pwdn_gpio = pinctrl_gpio_request(imx307->pctrl,
					imx307->pwdn_val[0], imx307->pwdn_val[1]);
		if (imx307->pwdn_gpio >= 0) {
			/* LOG_D("%d %d %d %d %d", imx307->pwdn_val[2],
				imx307->pwdn_val[3], imx307->pwdn_val[4],
				imx307->pwdn_val[5], imx307->pwdn_val[6]); */
			pinctrl_gpio_set_function(imx307->pctrl, imx307->pwdn_gpio,
						imx307->pwdn_val[2]);
			pinctrl_gpio_set_drv_level(imx307->pctrl, imx307->pwdn_gpio,
						imx307->pwdn_val[3]);
			pinctrl_gpio_set_pud_mode(imx307->pctrl, imx307->pwdn_gpio,
						imx307->pwdn_val[4]);
			pinctrl_gpio_set_pud_res(imx307->pctrl, imx307->pwdn_gpio,
						imx307->pwdn_val[5]);
			pinctrl_gpio_set_value(imx307->pctrl, imx307->pwdn_gpio,
						imx307->pwdn_val[6]);
		} else
			imx307->pwdn_valid = 0;
	}

	if (1 == imx307->rst_valid) {
		imx307->rst_gpio = pinctrl_gpio_request(imx307->pctrl,
					imx307->rst_val[0], imx307->rst_val[1]);
		if (imx307->rst_gpio >= 0) {
			/* LOG_D("%d %d %d %d %d", imx307->rst_val[2],
				imx307->rst_val[3], imx307->rst_val[4],
				imx307->rst_val[5], imx307->rst_val[6]); */
			pinctrl_gpio_set_function(imx307->pctrl, imx307->rst_gpio,
						imx307->rst_val[2]);
			pinctrl_gpio_set_drv_level(imx307->pctrl, imx307->rst_gpio,
						imx307->rst_val[3]);
			pinctrl_gpio_set_pud_mode(imx307->pctrl, imx307->rst_gpio,
						imx307->rst_val[4]);
			pinctrl_gpio_set_pud_res(imx307->pctrl, imx307->rst_gpio,
						imx307->rst_val[5]);
			pinctrl_gpio_set_value(imx307->pctrl, imx307->rst_gpio,
						imx307->rst_val[6]);
		} else
			imx307->rst_valid = 0;
	}

	if (1 == imx307->mclk_valid) {
		imx307->mclk_gpio = pinctrl_gpio_request(imx307->pctrl,
					imx307->mclk_val[0], imx307->mclk_val[1]);
		if (imx307->mclk_gpio >= 0) {
			pinctrl_gpio_set_function(imx307->pctrl, imx307->mclk_gpio,
						imx307->mclk_val[2]);
			pinctrl_gpio_set_drv_level(imx307->pctrl, imx307->mclk_gpio,
						imx307->mclk_val[3]);
			pinctrl_gpio_set_pud_mode(imx307->pctrl, imx307->mclk_gpio,
						imx307->mclk_val[4]);
			pinctrl_gpio_set_pud_res(imx307->pctrl, imx307->mclk_gpio,
						imx307->mclk_val[5]);
			pinctrl_gpio_set_value(imx307->pctrl, imx307->mclk_gpio,
						imx307->mclk_val[6]);
		} else
			imx307->mclk_valid = 0;
	}
	__imx307_mipi_set_power(imx307, RT_TRUE);
#endif
	rt_thread_delay(1); /* 10ms */

	imx307->i2c_client = rt_zalloc(sizeof(struct viss_i2c_client));
	if (RT_NULL == imx307->i2c_client)
		return -RT_ENOMEM;
	imx307->i2c_client->i2c_bus = rt_i2c_bus_device_find(imx307->info.i2c_bus_name);
	if (RT_NULL == imx307->i2c_client->i2c_bus) {
		LOG_E("can't find bus dev \"%s\"", imx307->info.i2c_bus_name);
		goto exit;
	}

	imx307->i2c_client->i2c_addr = imx307->info.i2c_addr;
	ret = viss_i2c_read_reg_16bit(imx307->i2c_client,  0x31DC, &tmp);
	LOG_E("add: %x, ret: %d. 0x3107: %x", imx307->i2c_client->i2c_addr, ret, tmp);


	id = tmp & 0x06;
	if (id != 0x4) {
		LOG_E("ID wrong! (0x%x)\n", id);
		goto exit;
	}

	imx307_exit_flag = 0;
	imx307_switch_gain_sem = rt_sem_create("imx_switch_gain_sem",
		0, RT_IPC_FLAG_FIFO);
	ret = pthread_create(&imx307_tid, RT_NULL, sensor_gain_switch, imx307);
	if (ret  < 0)
		LOG_E("pthread_create fail.");

	/* __imx307_mipi_block_write((void *)imx307,
		(void *)imx307_reg_list, ARRAY_SIZE(imx307_reg_list)); */

	return RT_EOK;
exit:
	if (imx307->pctrl) {
		if (imx307->rst_valid)
			pinctrl_gpio_free(imx307->pctrl, imx307->rst_gpio);
		if (imx307->pwdn_valid)
			pinctrl_gpio_free(imx307->pctrl, imx307->pwdn_gpio);
		if (imx307->mclk_valid)
			pinctrl_gpio_free(imx307->pctrl, imx307->mclk_gpio);
		pinctrl_put(imx307->pctrl);
		imx307->pctrl = RT_NULL;
	}
	if (imx307->i2c_client) {
		rt_free(imx307->i2c_client);
		imx307->i2c_client = RT_NULL;
	}
	#ifdef ARCH_LOMBO_N7V1_CDR
	/* cam_power_disable(); */
	cam_power_exit(module);
	camera_ldo_exit();
	#endif
	return -RT_ERROR;
}

static void __imx307_mipi_exit(void *hdl)
{
	struct video_dev *imx307 = (struct video_dev *)hdl;
	char *module = DRV_MCSI_IMX307_PWR_NAME;

	__imx307_mipi_set_power(imx307, RT_FALSE);
	if (imx307->pctrl) {
		if (imx307->rst_valid)
			pinctrl_gpio_free(imx307->pctrl, imx307->rst_gpio);
		if (imx307->pwdn_valid)
			pinctrl_gpio_free(imx307->pctrl, imx307->pwdn_gpio);
		if (imx307->mclk_valid)
			pinctrl_gpio_free(imx307->pctrl, imx307->mclk_gpio);
		pinctrl_put(imx307->pctrl);
		imx307->pctrl = RT_NULL;
	}
	if (imx307->i2c_client) {
		rt_free(imx307->i2c_client);
		imx307->i2c_client = RT_NULL;
	}

	reg_writed = 0;
	imx307_exit_flag = 1;
	rt_sem_release(imx307_switch_gain_sem);
	if (RT_NULL != imx307_tid)
		pthread_join(imx307_tid, RT_NULL);
	if (imx307_switch_gain_sem) {
		rt_sem_delete(imx307_switch_gain_sem);
		imx307_switch_gain_sem = RT_NULL;
	}

	#ifdef ARCH_LOMBO_N7V1_CDR
	cam_power_exit(module);
	camera_ldo_exit();
	/* cam_power_disable(); */
	#endif
}

struct video_dev imx307_mipi = {
	.name = DRV_MCSI_IMX307_NAME,
	.group_id = GRP_ID_MCSI,
	.prepare = __imx307_mipi_prepare,
	.init = __imx307_mipi_init,
	.exit = __imx307_mipi_exit,
	.s_mode = __imx307_mipi_set_mode,
	.g_cur_mode = __imx307_mipi_cur_mode,
	.g_all_mode = __imx307_mipi_get_all_mode,
	.s_power = __imx307_mipi_set_power,
	.s_stream = __imx307_mipi_set_stream,
	.g_info = __imx307_mipi_get_info,
	.ioctl = __imx307_mipi_ioctl,
	.s_register = __imx307_mipi_set_register,
	.g_register = __imx307_mipi_get_register,
};

