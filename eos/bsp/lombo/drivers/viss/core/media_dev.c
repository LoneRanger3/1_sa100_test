/*
 * media_dev.c - sensor top code for LomboTech
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

#define DBG_SECTION_NAME	"VISS-MD"
#define DBG_LEVEL		DBG_ERROR

#include <debug.h>
#include "vic_dev.h"
#include "isp_dev.h"
#include "mcsi_dev.h"
#include "media_dev.h"
#include "isp_dma_dev.h"
#ifdef ARCH_LOMBO_N7V1_CDR
#include "vic_det_dev.h"
#endif

#ifdef RT_USING_OV5640_DVP
#include "ov5640_dvp.h"
#endif
#ifdef RT_USING_OV5640_MIPI
#include "ov5640_mipi.h"
#endif
#ifdef RT_USING_OV2710_DVP
#include "ov2710_dvp.h"
#endif
#ifdef RT_USING_SC2363_MIPI
#include "sc2363_mipi.h"
#endif
#ifdef RT_USING_SC4353_MIPI
#include "sc4353_mipi.h"
#endif
#ifdef RT_USING_SC2363P_MIPI
#include "sc2363p_mipi.h"
#endif
#ifdef RT_USING_SC1335T_MIPI
#include "sc1335t_mipi.h"
#endif
#ifdef RT_USING_GC2053_MIPI
#include "gc2053_mipi.h"
#endif
#ifdef RT_USING_IMX307_MIPI
#include "imx307_mipi.h"
#endif
#ifdef RT_USING_RN6752_DVP
#include "rn6752_dvp.h"
#endif
#ifdef RT_USING_RN6752M_DVP
#include "rn6752m_dvp.h"
#endif
#ifdef RT_USING_TP9930_DVP
#include "tp9930_dvp.h"
#endif
#ifdef RT_USING_TP9950_DVP
#include "tp9950_dvp.h"
#endif
#ifdef RT_USING_BG0806_MIPI
#include "bg0806_mipi.h"
#endif
struct viss_md_vdev {
	struct video_dev *sensor;
	rt_int8_t use_stat;
};

static rt_err_t top_clk_init_time;
struct lombo_viss lombo_viss_dev;
struct viss_md_vdev g_all_sensor[] = {
#ifdef RT_USING_OV5640_DVP
	{&ov5640_dvp, 0},
#endif
#ifdef RT_USING_OV5640_MIPI
	{&ov5640_mipi, 0},
#endif
#ifdef RT_USING_OV2710_DVP
	{&ov2710_dvp, 0},
#endif
#ifdef RT_USING_SC2363_MIPI
	{&sc2363_mipi, 0},
#endif
#ifdef RT_USING_SC1335T_MIPI
	{&sc1335t_mipi, 0},
#endif
#ifdef RT_USING_SC4353_MIPI
	{&sc4353_mipi, 0},
#endif
#ifdef RT_USING_SC2363P_MIPI
	{&sc2363p_mipi, 0},
#endif
#ifdef RT_USING_GC2053_MIPI
	{&gc2053_mipi, 0},
#endif
#ifdef RT_USING_IMX307_MIPI
	{&imx307_mipi, 0},
#endif
#ifdef RT_USING_RN6752_DVP
	{&rn6752_dvp, 0},
#endif
#ifdef RT_USING_RN6752M_DVP
	{&rn6752m_dvp, 0},
#endif
#ifdef RT_USING_TP9930_DVP
	{&tp9930_dvp, 0},
#endif
#ifdef RT_USING_TP9950_DVP
	{&tp9950_dvp, 0},
#endif
#ifdef RT_USING_BG0806_MIPI
	{&bg0806_mipi, 0},
#endif
};

static const struct viss_clk vclk = {
	.sclk = {
		{
			.viss_sclk = CLK_NAME_VISS_SCLK0,
			.parrent = CLK_NAME_PERH0_PLL_DIV2,
		},
		{
			.viss_sclk = CLK_NAME_VISS_SCLK2,
			.parrent =  CLK_NAME_OSC24M,
		},
	},
	.viss_reset = CLK_NAME_AHB_VISS_RESET,
	.viss_gate = CLK_NAME_AHB_VISS_GATE,
	.viss_axi_gate = CLK_NAME_MAXI_VISS0_GATE,
};

void viss_md_top_clk_init(void)
{
	rt_int32_t i = 0;
	clk_handle_t chld = 0, self = 0, parrent = 0;
	if (0 == top_clk_init_time) {
		csp_viss_top_set_register_base((void *)(0x01400000 + 0x0));

		chld = clk_get(vclk.viss_reset);
		clk_enable(chld);
		chld = clk_get(vclk.viss_gate);
		clk_enable(chld);
		chld = clk_get(vclk.viss_axi_gate);
		clk_enable(chld);
		for (i = 0; i < VISS_MAX_SRCCLKS; i++) {
			if (RT_NULL != vclk.sclk[i].viss_sclk) {
				self = clk_get(vclk.sclk[i].viss_sclk);
				parrent = clk_get(vclk.sclk[i].parrent);
				clk_set_parent(self, parrent);
				clk_enable(self);
			}
		}
		top_clk_init_time++;
	} else
		top_clk_init_time++;
}

void viss_md_top_clk_exit(void)
{
	clk_handle_t chld = 0;
	rt_int32_t i = 0;
	if (0 < top_clk_init_time) {
		if (1 == top_clk_init_time) {
			chld = clk_get(vclk.viss_reset);
			clk_disable(chld);
			chld = clk_get(vclk.viss_gate);
			clk_disable(chld);
			chld = clk_get(vclk.viss_axi_gate);
			clk_disable(chld);
			for (i = 0; i < VISS_MAX_SRCCLKS; i++) {
				if (RT_NULL != vclk.sclk[i].viss_sclk) {
					chld = clk_get(vclk.sclk[i].viss_sclk);
					clk_disable(chld);
				}
			}
			top_clk_init_time--;
		} else
			top_clk_init_time--;
	}
}

void viss_md_mclk_init(rt_uint32_t type, rt_uint32_t freq)
{
	if (GRP_ID_VIC == type)
		csp_viss_top_vic_mclk_init(freq);
	else if (GRP_ID_MCSI == type)
		csp_viss_top_mcsi_mclk_init(freq);
}

/* only for isp use */
rt_err_t viss_md_streamon(struct viss_dev *vdev,
				rt_bool_t isp_out,  rt_int32_t ch)
{
	rt_err_t ret = 0;

	RT_ASSERT(RT_NULL != vdev);
	ret = vdev->streamon(vdev, isp_out, ch);
	if (vdev->sensor)
		ret = vdev->sensor->s_stream(vdev->sensor, RT_TRUE);
#if 0
	if (isp_out) {
		if (vdev->sensor)
			ret = vdev->sensor->s_stream(vdev->sensor, RT_TRUE);
	}

	if (RT_EOK == ret)
		ret = vdev->streamon(vdev, isp_out, ch);
#endif
	return ret;
}

rt_err_t viss_md_streamoff(struct viss_dev *vdev, rt_int32_t ch)
{
	rt_err_t ret = 0;

	if (vdev) {
		/* LOG_D("  "); */
		ret = vdev->streamoff(vdev, ch);
		if (RT_EOK != ret)
			LOG_E("viss_md_streamoff streamoff failed %d.", ret);
	}
#if 0
	if (vdev->sensor) {
		/* LOG_D("  "); */
		ret = vdev->sensor->s_stream(vdev->sensor, RT_FALSE);
		if (RT_EOK != ret)
			LOG_D("viss_md_streamoff s_stream failed %d.", ret);
	}
#endif
	return ret;
}

static struct video_dev *viss_md_probe_sensor(rt_uint32_t type,
						rt_int32_t index)
{
	struct video_dev *sensor = RT_NULL;
	struct viss_source_info info;
	rt_err_t ret = 0;
	/* rt_int32_t i = 0; */

	sensor = g_all_sensor[index].sensor;
	if (0 == g_all_sensor[index].use_stat) {
		viss_md_sensor_get_info(sensor, &info);
		if (info.mclk_freq > 0)
			viss_md_mclk_init(type, info.mclk_freq);
		ret = viss_md_sensor_init(sensor);
		if (RT_EOK != ret) {
			LOG_E("probe sensor:%s fail", sensor->name);
			viss_md_sensor_exit(sensor);
			return RT_NULL;
		}
		/* LOG_I("probe sensor:%s success", sensor->name); */
		g_all_sensor[index].use_stat = 1;
		return sensor;
	}
	LOG_E("probe sensor:%s fail", sensor->name);
	viss_md_sensor_exit(sensor);
	return RT_NULL;
}

#ifdef ARCH_LOMBO_N7V1_CDR
struct vic_det_dev *viss_det_create_links(void)
{
	struct vic_det_dev *det_dev = RT_NULL;
	det_dev = vic_det_dev_create();
	if (RT_NULL == det_dev)
		return RT_NULL;

	return det_dev;
}

void viss_det_destory_links(struct vic_det_dev *vdev)
{
	vic_det_dev_destory(vdev);
}

struct vic_det_status viss_det_get_sta(void)
{
	return vic_det_dev_get_sta();
}

static void __open_vic_det(void)
{
	rt_err_t ret = 0;
	rt_device_t  viss_det_dev;
	viss_det_dev = rt_device_find("vic_det");
	if (RT_NULL == viss_det_dev)
		LOG_E("Request %s device fail.", "vic_det");
	ret = rt_device_open(viss_det_dev, 0);
	if (RT_EOK != ret)
		LOG_E("Open %s device fail. ret:%d", "vic_det", ret);
}
#endif
/* Link control to sensor */
struct viss_dev *viss_md_create_links(rt_uint32_t type)
{
	/* LOG_D(""); */
	struct viss_dev *vdev = RT_NULL;
	/* struct viss_dev *isp = RT_NULL; */
	struct video_dev *sensor = RT_NULL;
	struct viss_source_info info;
	rt_err_t ret = 0;
	rt_int32_t i = 0;

	if (RT_NULL == lombo_viss_dev.lock)
		return RT_NULL;
	rt_mutex_take(lombo_viss_dev.lock, RT_WAITING_FOREVER);
	viss_md_top_clk_init();
	for (i = 0; i < ARRAY_SIZE(g_all_sensor); i++) {
		sensor = g_all_sensor[i].sensor;
		ret = viss_md_sensor_prepare(sensor);
		if (RT_EOK != ret)
			continue;
		viss_md_sensor_get_info(sensor, &info);
		/* LOG_D("%s>> path:%x group:%x",
			sensor->name, info.out_path, sensor->group_id); */
		if (VISS_IO_ISP != info.out_path) {
			if ((GRP_ID_VIC == sensor->group_id) && (GRP_ID_VIC == type)) {
				vdev = vic_dev_create(info.viss_top_freq);
				if (NULL == vdev) {
					LOG_E("Create vic device fail.");
					viss_md_top_clk_exit();
					rt_mutex_release(lombo_viss_dev.lock);
					return RT_NULL;
				}
				sensor = viss_md_probe_sensor(type, i);
				if (NULL == sensor) {
					LOG_E("Search sensor fail.");
					continue;
				}
				/* Create control to sensor link */
				vdev->sensor = sensor;
				#ifdef ARCH_LOMBO_N7V1_CDR
					__open_vic_det();
				#endif
				rt_mutex_release(lombo_viss_dev.lock);
				return vdev;
			} else if ((GRP_ID_MCSI == sensor->group_id) &&
							(GRP_ID_MCSI == type)) {
				vdev = mcsi_dev_create(info.viss_top_freq);
				if (NULL == vdev) {
					LOG_E("Create mcsi device fail.");
					viss_md_top_clk_exit();
					rt_mutex_release(lombo_viss_dev.lock);
					return RT_NULL;
				}
				sensor = viss_md_probe_sensor(type, i);
				if (NULL == sensor) {
					LOG_E("Search sensor fail.");
					continue;
				}
				/* Create control to sensor link */
				vdev->sensor = sensor;
				rt_mutex_release(lombo_viss_dev.lock);
				return vdev;
			}
		}
	}
	LOG_E("create_links error");
	viss_md_top_clk_exit();
	if (vdev) {
		if (GRP_ID_VIC == type)
			vic_dev_destory(vdev);
		else
			mcsi_dev_destory(vdev);
		vdev = RT_NULL;
	}
	rt_mutex_release(lombo_viss_dev.lock);
	return RT_NULL;
}

void viss_md_destory_links(struct viss_dev *vdev)
{
	rt_int32_t i = 0;
	/* if (lombo_viss_dev.isp != RT_NULL)
		lombo_viss_dev.isp->ref_count--; */
	/* LOG_D("lombo capture close.\n"); */
	/* if (RT_NULL == lombo_viss_dev.lock)
		return -RT_EINVAL; */
	RT_ASSERT(RT_NULL != lombo_viss_dev.lock);
	rt_mutex_take(lombo_viss_dev.lock, RT_WAITING_FOREVER);
	if (vdev) {
		for (i = 0; i < ARRAY_SIZE(g_all_sensor); i++) {
			if (g_all_sensor[i].sensor == vdev->sensor)
				g_all_sensor[i].use_stat = 0;
		}
		if (GRP_ID_VIC == vdev->group_id) {
			viss_md_sensor_exit(vdev->sensor);
			vic_dev_destory(vdev);
		} else if (GRP_ID_MCSI == vdev->group_id) {
			viss_md_sensor_exit(vdev->sensor);
			mcsi_dev_destory(vdev);
		} else
			LOG_E("Careful. unknow type device destory fail.");
		/*
		isp_dev_destory(lombo_viss_dev.isp);
		lombo_viss_dev.isp = RT_NULL;
		*/
		viss_md_top_clk_exit();
	}
	rt_mutex_release(lombo_viss_dev.lock);
}

struct viss_dev *viss_md_get_isp_dev()
{
	if (lombo_viss_dev.isp == RT_NULL)
		LOG_E("lombo_viss_dev.isp == RT_NULL");
	return lombo_viss_dev.isp;
}

struct video_dev *viss_md_get_isp_sensor()
{
	if (lombo_viss_dev.sensor == RT_NULL)
		LOG_E("lombo_viss_dev.sensor == RT_NULL");
	return lombo_viss_dev.sensor;
}

struct viss_dev *viss_md_get_csi_dev()
{
	if (lombo_viss_dev.vdev == RT_NULL)
		LOG_E("lombo_viss_dev.vdev == RT_NULL");
	return lombo_viss_dev.vdev;
}


void viss_md_dev_en(struct viss_dev *vdev, rt_int32_t en)
{
	if (GRP_ID_MCSI == vdev->sensor->group_id)
		mcsi_dev_reset(vdev, en);
	else if (GRP_ID_VIC == vdev->sensor->group_id)
		vic_dev_reset(vdev, en);
	else
		LOG_E("Wrong sensor group id.");
}

rt_err_t viss_md_sensor_set_mode(struct video_dev *sensor, rt_int32_t index)
{
	rt_err_t ret = 0;
	/* if (lombo_viss_dev.isp)
		LOG_D("viss_md_sensor_set_mode ref_count: %d.",
			lombo_viss_dev.isp->ref_count); */
	RT_ASSERT(RT_NULL != sensor);
	ret = sensor->s_mode(sensor, index);

	return ret;
}

struct dev_mode *viss_md_sensor_get_cur_mode(struct video_dev *sensor)
{
	struct dev_mode *mode = RT_NULL;

	RT_ASSERT(RT_NULL != sensor);
	mode = sensor->g_cur_mode(sensor);

	return mode;
}

void viss_md_sensor_get_all_mode(struct video_dev *sensor,
				struct dev_all_mode *all)
{
	RT_ASSERT(RT_NULL != sensor);
	all->mode = sensor->g_all_mode(sensor, &all->num);
}

rt_err_t viss_md_sensor_get_info(struct video_dev *sensor,
				struct viss_source_info *info)
{
	rt_err_t ret = 0;

	RT_ASSERT((RT_NULL != sensor) && (RT_NULL != info))
	ret = sensor->g_info(sensor, info);

	return ret;
}

rt_err_t viss_md_sensor_prepare(struct video_dev *sensor)
{
	rt_err_t ret = 0;

	RT_ASSERT(RT_NULL != sensor);
	ret = sensor->prepare(sensor);

	return ret;
}

rt_err_t viss_md_sensor_init(struct video_dev *sensor)
{
	rt_err_t ret = 0;

	RT_ASSERT(RT_NULL != sensor);
	ret = sensor->init(sensor);

	return ret;
}

rt_err_t viss_md_sensor_standby(struct video_dev *sensor, rt_bool_t enable)
{
	rt_err_t ret = 0;

	RT_ASSERT(RT_NULL != sensor);
	ret = sensor->s_stream(sensor, enable);

	return ret;
}

void viss_md_sensor_exit(struct video_dev *sensor)
{
	if (RT_NULL == sensor)
		return;
	sensor->exit(sensor);
}

rt_err_t viss_md_sensor_ioctl(struct video_dev *sensor,
				rt_int32_t cmd, void *para)
{
	rt_err_t ret = 0;

	if (RT_NULL == sensor)
		return -RT_EINVAL;
	if (sensor->ioctl)
		ret = sensor->ioctl(sensor, cmd, para);

	return ret;
}

rt_err_t viss_md_sensor_debug(struct video_dev *sensor,
				struct viss_dbg_register *reg,
	rt_int32_t rw)
{
	rt_err_t ret = 0;

	if (RT_NULL == sensor)
		return -RT_EINVAL;
	if (rw > 0)  {
		if (sensor->g_register)
			ret = sensor->g_register(sensor, reg);
	} else {
		if (sensor->s_register)
			ret = sensor->s_register(sensor, reg);
	}

	return ret;
}

struct viss_dev *viss_md_pipeline_get_entity(rt_uint32_t type)
{
	struct viss_dev *vdev = RT_NULL;

	if (RT_NULL == lombo_viss_dev.lock) {
		LOG_E("lombo_viss_dev.lock not init");
		return RT_NULL;
	}
	/* rt_mutex_take(lombo_viss_dev.lock, RT_WAITING_FOREVER); */

	if (GRP_ID_VISS == type) {
		if (lombo_viss_dev.csi_dev != RT_NULL)
			vdev = lombo_viss_dev.csi_dev;
	} else if (GRP_ID_ISP == type) {
		if (lombo_viss_dev.isp != RT_NULL)
			vdev = lombo_viss_dev.isp;
	} else if (GRP_ID_ISP_PREV == type) {
		if (lombo_viss_dev.prev_dev != RT_NULL)
			vdev = lombo_viss_dev.prev_dev;
	} else if (GRP_ID_ISP_CAP == type) {
		if (lombo_viss_dev.cap_dev != RT_NULL)
			vdev = lombo_viss_dev.cap_dev;
	} else
		LOG_E("viss_md_pipeline_get_entity wrong type.");
	/* rt_mutex_release(lombo_viss_dev.lock); */

	return vdev;
}

rt_err_t viss_md_pipeline_create(rt_uint32_t type)
{
	struct viss_dev *isp_dev = RT_NULL;
	struct viss_dev *prev_dev = RT_NULL;
	struct viss_dev *cap_dev = RT_NULL;
	struct viss_dev *csi_dev = RT_NULL;
	/* struct viss_dev *vdev = RT_NULL; */
	struct video_dev *sensor = RT_NULL;
	struct viss_source_info info;
	rt_err_t ret = 0;
	rt_int32_t i = 0;

	LOG_D("viss_md_pipeline_create in.");

	if (RT_NULL == lombo_viss_dev.lock)
		return -RT_EINVAL;
	rt_mutex_take(lombo_viss_dev.lock, RT_WAITING_FOREVER);

	if (GRP_ID_ISP_CAP == type) {
		cap_dev = isp_dma_dev_create(GRP_ID_ISP_CAP);
		if (RT_NULL == cap_dev) {
			isp_dma_dev_destory(cap_dev);
			LOG_E("create capture dev fail.");
			ret = -RT_ERROR;
			goto exit;
		}
		lombo_viss_dev.cap_dev = cap_dev;
	} else if (GRP_ID_ISP_PREV == type) {
		prev_dev = isp_dma_dev_create(GRP_ID_ISP_PREV);
		if (RT_NULL == prev_dev) {
			isp_dma_dev_destory(prev_dev);
			LOG_E("create preview dev fail.");
			ret = -RT_ERROR;
			goto exit;
		}
		lombo_viss_dev.prev_dev = prev_dev;
	} else {
		LOG_E("md pipeline create wrong type.");
		ret = -RT_ERROR;
		goto exit;
	}

	if ((RT_NULL != lombo_viss_dev.isp) && (0 != lombo_viss_dev.isp->create_count)) {
		lombo_viss_dev.isp->create_count++;
		LOG_D("isp dev has be created.");
		ret = RT_EOK;
		goto exit;
	}

	viss_md_top_clk_init();
	/* LOG_D("isp dev create first time."); */
#if 0
	isp_dev = isp_dev_create();
	if (RT_NULL == isp_dev) {
		if (GRP_ID_ISP_CAP == type)
			isp_cap_dev_destory(cap_dev);
		else if (GRP_ID_ISP_PREV == type)
			isp_prev_dev_destory(prev_dev);
		else
			;
		isp_dev_destory(isp_dev);
		LOG_E("create isp dev fail.");
		ret = -RT_ERROR;
		viss_md_top_clk_exit();
		goto exit;
	}
	isp_dev->create_count++;
#endif
	for (i = 0; i < ARRAY_SIZE(g_all_sensor); i++) {
		sensor = g_all_sensor[i].sensor;
		ret = viss_md_sensor_prepare(sensor);
		if (RT_EOK != ret)
			continue;
		viss_md_sensor_get_info(sensor, &info);
		/* LOG_D("%s>> path:%x group:%x",
			sensor->name, info.out_path, sensor->group_id); */
		if (VISS_IO_ISP == info.out_path) {
			if (GRP_ID_VIC == sensor->group_id) {
				csi_dev = vic_dev_create(info.viss_top_freq);
				if (NULL == csi_dev) {
					LOG_E("Create vic device fail.");
					continue;
				}
				sensor = viss_md_probe_sensor(GRP_ID_VIC, i);
				if (NULL == sensor) {
					LOG_E("Search sensor fail.");
					vic_dev_destory(csi_dev);
					continue;
				}
				rt_memcpy(&csi_dev->sensor_info, &info,
					sizeof(struct viss_source_info));
				csi_dev->sensor = sensor;
				goto success;
			} else if (GRP_ID_MCSI == sensor->group_id) {
				csi_dev = mcsi_dev_create(info.viss_top_freq);
				if (NULL == csi_dev) {
					LOG_E("Create mcsi device fail.");
					continue;
				}
				/* __mcsi_dev_hw_config(info.data_lanes); */
				sensor = viss_md_probe_sensor(GRP_ID_MCSI, i);
				if (NULL == sensor) {
					LOG_E("Search sensor fail.");
					mcsi_dev_destory(csi_dev);
					continue;
				}
				rt_memcpy(&csi_dev->sensor_info, &info,
					sizeof(struct viss_source_info));
				csi_dev->sensor = sensor;
				goto success;
			} else {
				continue;
			}
		}
	}
	LOG_E("probe sensor fail.");
	if (GRP_ID_ISP_CAP == type)
		isp_dma_dev_destory(cap_dev);
	else if (GRP_ID_ISP_PREV == type)
		isp_dma_dev_destory(prev_dev);
	else
		;
	/* isp_dev_destory(isp_dev); */
	viss_md_top_clk_exit();
	ret = -RT_ERROR;
	goto exit;
success:
	isp_dev = isp_dev_create(csi_dev->sensor_info.isp_top_freq);
	if (RT_NULL == isp_dev) {
		if (GRP_ID_ISP_CAP == type)
			isp_dma_dev_destory(cap_dev);
		else if (GRP_ID_ISP_PREV == type)
			isp_dma_dev_destory(prev_dev);
		else
			;
		/* isp_dev_destory(isp_dev); */
		LOG_E("create isp dev fail.");
		ret = -RT_ERROR;
		viss_md_top_clk_exit();
		goto exit;
	}
	isp_dev->create_count++;
	lombo_viss_dev.csi_dev = csi_dev;
	lombo_viss_dev.sensor = sensor;
	lombo_viss_dev.isp = isp_dev;
exit:
	LOG_D("viss_md_pipeline_create out.");
	/* LOG_D("end"); */
	rt_mutex_release(lombo_viss_dev.lock);

	return ret;
}

rt_err_t viss_md_pipeline_free(rt_uint32_t type)
{
	rt_err_t ret = RT_EOK;
	rt_int32_t i = 0;
	struct viss_dev *csi_dev = lombo_viss_dev.csi_dev;
	LOG_D("viss_md_pipeline_free in.");

	if (RT_NULL == lombo_viss_dev.lock)
		return -RT_EINVAL;
	rt_mutex_take(lombo_viss_dev.lock, RT_WAITING_FOREVER);

	if (GRP_ID_ISP_CAP == type) {
		isp_dma_dev_destory(lombo_viss_dev.cap_dev);
		lombo_viss_dev.cap_dev = RT_NULL;
	} else if (GRP_ID_ISP_PREV == type) {
		isp_dma_dev_destory(lombo_viss_dev.prev_dev);
		lombo_viss_dev.prev_dev = RT_NULL;
	} else {
		LOG_E("md pipeline free wrong type.");
		ret = -RT_ERROR;
		goto exit;
	}

	lombo_viss_dev.isp->create_count--;

	if ((RT_NULL != lombo_viss_dev.isp) && (0 != lombo_viss_dev.isp->create_count)) {
		LOG_D("isp pipeline has been used and can not free now.");
		ret = RT_EOK;
		goto exit;
	}

	/* LOG_D(" isp pipeline free "); */
	__isp_dev_exit(lombo_viss_dev.isp);
	isp_dev_destory(lombo_viss_dev.isp);
	lombo_viss_dev.isp = RT_NULL;

	if (csi_dev) {
		for (i = 0; i < ARRAY_SIZE(g_all_sensor); i++) {
			if (g_all_sensor[i].sensor == csi_dev->sensor)
				g_all_sensor[i].use_stat = 0;
		}
		if (GRP_ID_VIC == csi_dev->group_id) {
			viss_md_sensor_exit(csi_dev->sensor);
			vic_dev_destory(csi_dev);
		} else if (GRP_ID_MCSI == csi_dev->group_id) {
			viss_md_sensor_exit(csi_dev->sensor);
			mcsi_dev_destory(csi_dev);
		} else {
			LOG_E("Careful. unknow type device destory fail.");
		}
		viss_md_top_clk_exit();
	}
exit:
	/* LOG_D("end"); */
	rt_mutex_release(lombo_viss_dev.lock);
	LOG_D("viss_md_pipeline_free out.");
	return ret;
}

rt_err_t viss_md_pipeline_init(rt_uint32_t type)
{
	rt_err_t ret = RT_EOK;
	LOG_D(" ");

	if (RT_NULL == lombo_viss_dev.lock)
		return -RT_EINVAL;
	rt_mutex_take(lombo_viss_dev.lock, RT_WAITING_FOREVER);

	if (GRP_ID_ISP_CAP == type) {
		ret = __isp_dma_dev_init(lombo_viss_dev.cap_dev);
		if (ret != RT_EOK) {
			LOG_E(" isp capture device init failed.");
			goto exit;
		}
	}

	if (GRP_ID_ISP_PREV == type) {
		ret = __isp_dma_dev_init(lombo_viss_dev.prev_dev);
		if (ret != RT_EOK) {
			LOG_E(" isp preview device init failed.");
			goto exit;
		}
	}

	if ((RT_NULL != lombo_viss_dev.isp) && (0 != lombo_viss_dev.isp->init_count)) {
		lombo_viss_dev.isp->init_count++;
		/* LOG_D("viss_md_pipeline_init second."); */
		LOG_D("isp dev has be created.");
		ret = RT_EOK;
		goto exit;
	}
	lombo_viss_dev.isp->init_count++;
	/* LOG_D("viss_md_pipeline_init first."); */

	if (lombo_viss_dev.isp) {
		if (lombo_viss_dev.isp->init_count > 1) {
			LOG_D("isp pipeline has been init.");
			goto exit;
		} else
			ret = __isp_dev_init(lombo_viss_dev.isp);
	} else {
		ret = -RT_ERROR;
		LOG_E("isp device is empty.");
	}

exit:
	rt_mutex_release(lombo_viss_dev.lock);
	return ret;
}

rt_err_t viss_md_pipeline_exit(rt_uint32_t type)
{
	LOG_D("  ");
	rt_err_t ret = RT_EOK;

	if (RT_NULL == lombo_viss_dev.lock)
		return -RT_EINVAL;
	rt_mutex_take(lombo_viss_dev.lock, RT_WAITING_FOREVER);

	if (GRP_ID_ISP_CAP == type)
		__isp_dma_dev_exit(lombo_viss_dev.cap_dev);
	if (GRP_ID_ISP_PREV == type)
		__isp_dma_dev_exit(lombo_viss_dev.prev_dev);

	lombo_viss_dev.isp->init_count--;

	if (0 != lombo_viss_dev.isp->init_count) {
		LOG_D("isp pipeline has been used and can not exit now.");
		goto exit;
	} else {
		LOG_D("isp pipeline final exit");
		/* viss_md_sensor_exit(lombo_viss_dev.csi_dev->sensor); */
	}

exit:
	rt_mutex_release(lombo_viss_dev.lock);
	return ret;
}

rt_err_t viss_md_pipeline_streamon(rt_uint32_t type, rt_int32_t ch)
{
	rt_err_t ret = RT_EOK;
	/* struct video_dev *sensor = lombo_viss_dev.sensor; */
	struct viss_dev *isp_dev = lombo_viss_dev.isp;
	/* struct viss_dev *csi_dev = lombo_viss_dev.csi_dev; */
	struct viss_dev *cap_dev = lombo_viss_dev.cap_dev;
	struct viss_dev *prev_dev = lombo_viss_dev.prev_dev;
	LOG_D("  ");

	if (RT_NULL == lombo_viss_dev.lock)
		return -RT_EINVAL;
	rt_mutex_take(lombo_viss_dev.lock, RT_WAITING_FOREVER);

	if (0 == lombo_viss_dev.isp->stream_count) {
		LOG_I("reset isp ");
		isp_reset_hw(1);
		isp_reset_hw(0);
	}

	if (GRP_ID_ISP_CAP == type) {
		rt_mutex_release(lombo_viss_dev.lock);
		ret = cap_dev->streamon(cap_dev, RT_FALSE, ch);
		if (ret != RT_EOK) {
			LOG_E(" isp capture device stream on failed.");
			goto exit;
		}
	} else if (GRP_ID_ISP_PREV == type) {
		rt_mutex_release(lombo_viss_dev.lock);
		ret = prev_dev->streamon(prev_dev, RT_FALSE, ch);
		if (ret != RT_EOK) {
			LOG_E(" isp preview device stream on failed.");
			goto exit;
		}
	} else {
		rt_mutex_release(lombo_viss_dev.lock);
		LOG_E("viss_md_pipeline_streamon wrong type.");
	}

	rt_mutex_take(lombo_viss_dev.lock, RT_WAITING_FOREVER);
	lombo_viss_dev.isp->stream_count++;

	if (1 == lombo_viss_dev.isp->stream_count) {
		rt_mutex_release(lombo_viss_dev.lock);
		/* LOG_D("stream on first time."); */
		ret = isp_dev->streamon(isp_dev, RT_FALSE, ch);
	} else
		rt_mutex_release(lombo_viss_dev.lock);

	rt_mutex_take(lombo_viss_dev.lock, RT_WAITING_FOREVER);
	if (isp_dev->state[0] == ST_PREV_PENDING) {
		if (GRP_ID_ISP_CAP == type) {
			lombo_viss_dev.isp_ch = lombo_viss_dev.isp_ch | GRP_CH_ISP_CAP;
			__isp_dma_dev_start_streaming(GRP_CH_ISP_CAP);
		}
		if (GRP_ID_ISP_PREV == type) {
			lombo_viss_dev.isp_ch = lombo_viss_dev.isp_ch | GRP_CH_ISP_PREV;
			__isp_dma_dev_start_streaming(GRP_CH_ISP_PREV);
		}

	} else {
		if (GRP_ID_ISP_CAP == type)
			lombo_viss_dev.isp_ch = lombo_viss_dev.isp_ch | GRP_CH_ISP_CAP;
		if (GRP_ID_ISP_PREV == type)
			lombo_viss_dev.isp_ch = lombo_viss_dev.isp_ch | GRP_CH_ISP_PREV;
	}
	rt_mutex_release(lombo_viss_dev.lock);
exit:
	return ret;
}

rt_err_t viss_isp_pipeline_streamon(rt_int32_t ch)
{
	rt_err_t ret = RT_EOK;
	struct video_dev *sensor = lombo_viss_dev.sensor;
	/* struct viss_dev *isp_dev = lombo_viss_dev.isp; */
	struct viss_dev *csi_dev = lombo_viss_dev.csi_dev;
	/* struct viss_dev *cap_dev = lombo_viss_dev.cap_dev; */
	/* struct viss_dev *prev_dev = lombo_viss_dev.prev_dev; */

	LOG_D("viss_isp_pipeline_streamon");

	if (RT_NULL == lombo_viss_dev.lock)
		return -RT_EINVAL;
	rt_mutex_take(lombo_viss_dev.lock, RT_WAITING_FOREVER);

	if (lombo_viss_dev.isp_ch & GRP_CH_ISP_CAP)
		__isp_dma_dev_start_streaming(GRP_CH_ISP_CAP);
	if (lombo_viss_dev.isp_ch & GRP_CH_ISP_PREV)
		__isp_dma_dev_start_streaming(GRP_CH_ISP_PREV);

	/* ret = sensor->s_stream(sensor, RT_TRUE);
	if (ret != RT_EOK) {
			LOG_E("sensor s_stream failed.");
			goto exit;
	} */

	ret = csi_dev->streamon(csi_dev, RT_TRUE, ch);
	if (ret != RT_EOK) {
		LOG_E("csi device stream on failed.");
		goto exit;
	}

	ret = sensor->s_stream(sensor, RT_TRUE);
	if (ret != RT_EOK) {
			LOG_E("sensor s_stream failed.");
			goto exit;
	}

exit:
	rt_mutex_release(lombo_viss_dev.lock);
	return ret;
}

rt_err_t viss_md_pipeline_streamoff(rt_uint32_t type, rt_int32_t ch)
{
	rt_err_t ret = RT_EOK;
	/* struct video_dev *sensor = lombo_viss_dev.sensor; */
	struct viss_dev *isp_dev = lombo_viss_dev.isp;
	/* struct viss_dev *csi_dev = lombo_viss_dev.csi_dev; */
	struct viss_dev *cap_dev = lombo_viss_dev.cap_dev;
	struct viss_dev *prev_dev = lombo_viss_dev.prev_dev;

	LOG_D("viss_isp_pipeline_streamoff");

	if (RT_NULL == lombo_viss_dev.lock)
		return -RT_EINVAL;
	rt_mutex_take(lombo_viss_dev.lock, RT_WAITING_FOREVER);
	if (lombo_viss_dev.isp->stream_count > 0) {
		lombo_viss_dev.isp->stream_count--;
		if (0 == lombo_viss_dev.isp->stream_count) {
			rt_mutex_release(lombo_viss_dev.lock);
			LOG_D("final stream off");
			ret = isp_dev->streamoff(isp_dev, ch);
			if (ret != RT_EOK) {
				LOG_E("isp device stream off failed.");
				goto exit;
			}
			/*
			ret = sensor->s_stream(sensor, RT_FALSE);
			if (ret != RT_EOK) {
				LOG_E("sensor stream off failed.");
				goto exit;
			}
			*/
		} else {
			rt_mutex_release(lombo_viss_dev.lock);
			if (isp_dev->isp_drv_wait_sem)
				rt_sem_release(isp_dev->isp_drv_wait_sem);
		}

		rt_mutex_take(lombo_viss_dev.lock, RT_WAITING_FOREVER);
		if (GRP_ID_ISP_CAP == type) {
			/* LOG_D("capture stream off."); */
			lombo_viss_dev.isp_ch &= (~GRP_CH_ISP_CAP);
			rt_mutex_release(lombo_viss_dev.lock);
			ret = cap_dev->streamoff(cap_dev, ch);
			if (ret != RT_EOK) {
				LOG_E("isp capture device stream off failed.");
				goto exit;
			}
		} else if (GRP_ID_ISP_PREV == type) {
			/* LOG_D("preview stream off."); */
			lombo_viss_dev.isp_ch &= (~GRP_CH_ISP_PREV);
			rt_mutex_release(lombo_viss_dev.lock);
			ret = prev_dev->streamoff(prev_dev, ch);
			if (ret != RT_EOK) {
				LOG_E("isp preview device stream off failed.");
				goto exit;
			}
		} else {
			LOG_E("viss_md_pipeline_streamoff wrong type.");
			rt_mutex_release(lombo_viss_dev.lock);
		}
	}

exit:
	/* LOG_D("  "); */
	return ret;
}

rt_err_t viss_md_pipeline_stop_hw(rt_uint32_t type, rt_int32_t ch)
{
	rt_err_t ret = RT_EOK;
	/* struct video_dev *sensor = lombo_viss_dev.sensor; */
	struct viss_dev *isp_dev = lombo_viss_dev.isp;
	struct viss_dev *csi_dev = lombo_viss_dev.csi_dev;
	struct viss_dev *cap_dev = lombo_viss_dev.cap_dev;
	struct viss_dev *prev_dev = lombo_viss_dev.prev_dev;

	if (RT_NULL == lombo_viss_dev.lock)
		return -RT_EINVAL;
	rt_mutex_take(lombo_viss_dev.lock, RT_WAITING_FOREVER);
	if (RT_NULL != cap_dev)
		__isp_dma_dev_stop_hw(csi_dev);
	if (RT_NULL != prev_dev)
		__isp_dma_dev_stop_hw(csi_dev);
	if (RT_NULL != isp_dev)
		__isp_dev_stop_hw(isp_dev);
/* exit: */
	rt_mutex_release(lombo_viss_dev.lock);
	return ret;
}

rt_err_t viss_md_pipeline_restart_dma()
{
	rt_err_t ret = RT_EOK;
	if (RT_NULL == lombo_viss_dev.lock)
		return -RT_EINVAL;
	rt_mutex_take(lombo_viss_dev.lock, RT_WAITING_FOREVER);

	if (RT_NULL != lombo_viss_dev.cap_dev) {
		/* LOG_D("viss_md_pipeline_restart_dma capture streaming."); */
		__isp_dma_dev_start_streaming(GRP_CH_ISP_CAP);
	}
	if (RT_NULL != lombo_viss_dev.prev_dev) {
		/* LOG_D("viss_md_pipeline_restart_dma preview streaming."); */
		__isp_dma_dev_start_streaming(GRP_CH_ISP_PREV);
	}
	rt_mutex_release(lombo_viss_dev.lock);
	return ret;
}


rt_err_t viss_md_init()
{
	rt_err_t ret = RT_EOK;

	memset(&lombo_viss_dev, 0, sizeof(lombo_viss_dev));
	lombo_viss_dev.lock = rt_mutex_create("media_dev", RT_IPC_FLAG_FIFO);

	return ret;
}
rt_err_t viss_md_deinit()
{
	rt_err_t ret = RT_EOK;

	if (RT_NULL != lombo_viss_dev.lock)
		rt_mutex_delete(lombo_viss_dev.lock);
	memset(&lombo_viss_dev, 0, sizeof(lombo_viss_dev));

	return ret;
}


