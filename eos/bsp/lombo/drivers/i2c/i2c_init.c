/*
 * i2c_init.c - I2C host driver code for LomboTech
 * i2c hardware init operation
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
#ifndef ___I2C___INIT__C___
#define ___I2C___INIT__C___

/* set parameter of I2CC according current configuration */
u32 __i2c_setup(struct lombo_i2c *i2c)
{
	u32 half_cycl_cnt = 0;
	u32 base_addr = 0;
	u32 h_delta_cnt = 0;
	u32 mc_half_cycl_cnt = 0;
	u32 hs_h_delta_cnt = 0;
	int ret = 0;

	PRT_TRACE_BEGIN("i2c->idx=%d", i2c->idx);

	base_addr = (u32)i2c->base;

	ret = clk_set_parent(i2c->clk_i2c, i2c->clk_parent);
	if (ret != 0) {
		ret = __LINE__;
		goto out;
	}

	/* ic clock rate should not smaller than 11 times of i2c clock rate */
	if (i2c->baudrate <= 1000000) {
		ret = clk_set_rate(i2c->clk_i2c, 24000000);
		if (ret != 0) {
			ret = __LINE__;
			goto out;
		}
		i2c->ic_clkrate = 24000000;
	} else {
		ret = clk_set_rate(i2c->clk_i2c, 50000000);
		if (ret != 0) {
			ret = __LINE__;
			goto out;
		}
		i2c->ic_clkrate = 50000000;
	}

	/*
	 * baudrate and bus speed mode configure
	 */

	/* BaudRate_cfg
	 * SS	freq <= 100K
	 * FS	100K <= freq <= 400K
	 * FS+	400K <= freq <= 1M
	 * HS	1M <= freq <= 3.4M
	 */
	half_cycl_cnt = i2c->ic_clkrate / i2c->baudrate / 2;
	h_delta_cnt = i2c->fs_spk_len + 3 + i2c->h_adj_cnt;

	mc_half_cycl_cnt = i2c->ic_clkrate / I2C_HS_MASTERCODE_FREQ / 2;
	hs_h_delta_cnt = i2c->hs_spk_len + 3 + i2c->h_adj_cnt;

	if (i2c->baudrate <= 100000) {	/* SS mode */
		if ((half_cycl_cnt <= h_delta_cnt) ||
			(half_cycl_cnt <= i2c->l_adj_cnt)) {
			ret = __LINE__;
			goto out;
		}
		csp_i2c_set_speed_mode(base_addr, I2C_SS_MODE);
		csp_i2c_set_ss_baudrate(base_addr,
			half_cycl_cnt - h_delta_cnt,
			half_cycl_cnt - i2c->l_adj_cnt);
	} else if (i2c->baudrate <= 1000000) {	/* FS/FS+ mode, the same to I2CC */
		if ((half_cycl_cnt <= h_delta_cnt) ||
			(half_cycl_cnt <= i2c->l_adj_cnt + I2C_LCNT_MIN)) {
			ret = __LINE__;
			goto out;
		}
		csp_i2c_set_speed_mode(base_addr, I2C_FS_MODE);
		csp_i2c_set_fs_baudrate(base_addr,
			half_cycl_cnt - h_delta_cnt,
			half_cycl_cnt - i2c->l_adj_cnt);
	} else {
		u32 hs_1_3_rate_cnt;
		u32 hs_2_3_rate_cnt;

		csp_i2c_enable_hs_sclpp(base_addr, 1);

		 hs_1_3_rate_cnt = i2c->ic_clkrate / i2c->baudrate / 3;
		 hs_2_3_rate_cnt = hs_1_3_rate_cnt * 2;

		hs_h_delta_cnt = i2c->hs_spk_len + 3 + i2c->h_adj_cnt;
		if ((hs_1_3_rate_cnt <= hs_h_delta_cnt) ||
			(hs_2_3_rate_cnt <= i2c->l_adj_cnt + I2C_LCNT_MIN)) {
			ret = __LINE__;
			goto out;
		}
		csp_i2c_set_speed_mode(base_addr, I2C_HS_MODE);
		csp_i2c_set_hs_baudrate(base_addr,
			hs_1_3_rate_cnt - hs_h_delta_cnt,
			hs_2_3_rate_cnt - i2c->l_adj_cnt);

		if ((mc_half_cycl_cnt <= h_delta_cnt) ||
			(mc_half_cycl_cnt <= i2c->l_adj_cnt + I2C_LCNT_MIN)) {
			ret = __LINE__;
			goto out;
		}
		csp_i2c_set_fs_baudrate(base_addr,
			mc_half_cycl_cnt - h_delta_cnt,
			mc_half_cycl_cnt - i2c->l_adj_cnt);
	}

	/*
	 * spike len configure
	 */
	csp_i2c_cfg_spk_len(base_addr, i2c->fs_spk_len,
				i2c->hs_spk_len);

	/*
	 * timing
	 */

	/* Tx_hold & Rx_hold
	 * SS_mode 3.45us > hold_time > 300ns =  7.3 cycle(24MHz)
	 * FS_mode 0.9 us > hold_time > 300ns
	 * HS_mode 0.45us > hold_time > 300ns
	 */
	/* setup_time
	 * SS_mode   set_time > 250ns =  6 cycle (24Mhz)
	 * FS_mode   set_time > 100ns
	 * HS_mode   set_time > 50 ns
	 */
	csp_i2c_set_timing(base_addr, half_cycl_cnt / 2,
			half_cycl_cnt / 2, half_cycl_cnt / 2);

	/*
	 * operate mode
	 */

	/* master-en & slave-dis */
	csp_i2c_set_master_mode(base_addr);

	/* restart function enable */
	csp_i2c_enable_restart(base_addr, 1);

	/* hold the SCL low to single the slave not send data when RX_FIFO full*/
	csp_i2c_enable_rx_ffh(base_addr, 1);

	/*
	 * INT_behavior
	 */

	/* raise STOP_DET pending only when master is activate */
	csp_i2c_master_stop_det(base_addr, I2C_STOP_DET_IN_MASTER_ACTIV);

	/* raise TX_EMPTY when TX_FIFO is empty
	 *  and last command has been finished */
	csp_i2c_tx_empty_mode(base_addr, I2C_TX_EMPTY_IN_CMD_DONE);

	PRT_TRACE_END("ret=0");
	return 0;

out:
	if (ret != 0) {
		LOG_E("ret=%d,i2c->idx=%d,boudrate=%d,\n\t"
			"fs_spk_len=%d,hs_spk_len=%d,ic_clkrate=%d",
			ret, i2c->idx, i2c->baudrate,
			i2c->fs_spk_len, i2c->hs_spk_len,
			i2c->ic_clkrate);
	}
	PRT_TRACE_END("ret=%d", ret);
	return ret;
}

/* init the I2C controller hardware */
u32 i2c_init(struct lombo_i2c *i2c)
{
	char i2c_node[6] = {0};
	const char *clk_gate = NULL;
	const char *clk_reset = NULL;
	const char *clk_i2c = NULL;
	const char *clk_parent = NULL;
	u32 temp;
	int ret = 0;

	if (NULL == i2c) {
		LOG_E("i2c=0x%08x", i2c);
		return __LINE__;
	}
	PRT_TRACE_BEGIN("i2c->idx=%d", i2c->idx);

	rt_snprintf(i2c_node, 6, "i2c%d", i2c->idx);

	/* get module register base */
	ret = config_get_u32(i2c_node, "reg", &temp);
	if (ret != 0) {
		LOG_E("i2c%d: failed to get register base", i2c->idx);
		ret = __LINE__;
		goto error;
	}
	i2c->base = (void *)(temp + VA_I2C);

	switch (i2c->idx) {
	case 0:
		i2c->irq = INT_I2C0;
		break;
	case 1:
		i2c->irq = INT_I2C1;
		break;
	case 2:
		i2c->irq = INT_I2C2;
		break;
	case 3:
		i2c->irq = INT_I2C3;
		break;
	default:
		ret = __LINE__;
		goto error;
	}

	/* get i2c baud-rate */
	ret = config_get_u32(i2c_node, "baud-rate", &temp);
	if (ret != 0) {
		LOG_E("i2c%d: failed to get \"baud-rate\"", i2c->idx);
		ret = __LINE__;
		goto error;
	}
	LOG_D("i2c%d: \"baud-rate\" = %d", i2c->idx, temp);

	if (temp == 0) {
		LOG_E("i2c%d: please set proper \"baud-rate\"", i2c->idx);
		ret = __LINE__;
		goto error;
	} else if (temp > 3400000) {
		LOG_W("i2c%d: \"baud-rate\" too large (%d), set to 3400000",
			i2c->idx, temp);
		temp = 3400000;
	}
	i2c->baudrate = temp;

	i2c->fs_spk_len = 2;
	i2c->hs_spk_len = 4;

	i2c->h_adj_cnt = 1;
	i2c->l_adj_cnt = 1;

	/* get clock gate */
	ret = config_get_string(i2c_node, "clock-gate", &clk_gate);
	if (ret != 0) {
		LOG_E("i2c%d: failed to get \"clock-gate\"", i2c->idx);
		ret = __LINE__;
		goto error;
	}
	LOG_D("i2c%d: \"clock-gate\" = %s", i2c->idx, clk_gate);

	/* get clock reset */
	ret = config_get_string(i2c_node, "clock-reset", &clk_reset);
	if (ret != 0) {
		LOG_E("i2c%d: failed to get \"clock-reset\"", i2c->idx);
		ret = __LINE__;
		goto error;
	}
	LOG_D("i2c%d: \"clock-reset\" = %s", i2c->idx, clk_reset);

	/* get module clock */
	ret = config_get_string(i2c_node, "clock", &clk_i2c);
	if (ret != 0) {
		LOG_E("i2c%d: failed to get \"clock\"", i2c->idx);
		ret = __LINE__;
		goto error;
	}
	LOG_D("i2c%d: \"clock\" = %s", i2c->idx, clk_i2c);

	if (i2c->baudrate <= 1000000) {
		ret = config_get_string(i2c_node, "clock-parent0", &clk_parent);
		if (ret != 0) {
			LOG_E("i2c%d: failed to get \"clock-parent0\"",
				i2c->idx);
			ret = __LINE__;
			goto error;
		}
		LOG_D("i2c%d: \"clock-parent0\" = %s", i2c->idx, clk_parent);
	} else {
		ret = config_get_string(i2c_node, "clock-parent1", &clk_parent);
		if (ret != 0) {
			LOG_E("i2c%d: failed to get \"clock-parent1\"",
				i2c->idx);
			ret = __LINE__;
			goto error;
		}
		LOG_D("i2c%d: \"clock-parent1\" = %s", i2c->idx, clk_parent);
	}

	i2c->clk_gate = clk_get(clk_gate);
	if (i2c->clk_gate < 0) {
		LOG_E("i2c%d: failed to get clk_gate", i2c->idx);
		ret = __LINE__;
		goto error;
	}

	i2c->clk_reset = clk_get(clk_reset);
	if (i2c->clk_gate < 0) {
		LOG_E("i2c%d: failed to get clk_reset", i2c->idx);
		ret = __LINE__;
		goto error_release_clk;
	}

	i2c->clk_i2c = clk_get(clk_i2c);
	if (i2c->clk_i2c < 0) {
		LOG_E("i2c%d: failed to get clk_i2c", i2c->idx);
		ret = __LINE__;
		goto error_release_clk;
	}

	i2c->clk_parent = clk_get(clk_parent);
	if (i2c->clk_parent < 0) {
		LOG_E("i2c%d: failed to get clk_parent", i2c->idx);
		ret = __LINE__;
		goto error_release_clk;
	}

	ret = clk_enable(i2c->clk_gate);
	if (ret < 0) {
		LOG_E("i2c%d: failed to enable clk gate", i2c->idx);
		ret = __LINE__;
		goto error_release_clk;
	}

	ret = clk_enable(i2c->clk_reset);
	if (ret < 0) {
		LOG_E("i2c%d: failed to enable clk gate", i2c->idx);
		ret = __LINE__;
		goto error_disable_clk_gate;
	}

	/* set parameter of I2CC according current configuration */
	ret = __i2c_setup(i2c);
	if (ret != 0) {
		LOG_E("i2c->idx=%d\n", i2c->idx);
		ret = __LINE__;
		goto error_disable_clk_reset;
	}

	ret = clk_enable(i2c->clk_i2c);
	if (ret != 0) {
		LOG_E("i2c%d: failed to enable clk i2c", i2c->idx);
		ret = __LINE__;
		goto error_disable_clk_reset;
	}

	PRT_TRACE_END("ret=0");
	return 0;

error_disable_clk_reset:
	clk_disable(i2c->clk_reset);
error_disable_clk_gate:
	clk_disable(i2c->clk_gate);
error_release_clk:
	if (i2c->clk_parent > 0)
		clk_put(i2c->clk_parent);
	if (i2c->clk_i2c > 0)
		clk_put(i2c->clk_i2c);
	if (i2c->clk_reset > 0)
		clk_put(i2c->clk_reset);
	if (i2c->clk_gate > 0)
		clk_put(i2c->clk_gate);
error:
	PRT_TRACE_END("ret=%d", ret);
	return ret;
}

/* deinit the I2C hardware */
void i2c_deinit(struct lombo_i2c *i2c)
{
	PRT_TRACE_BEGIN("i2c->idx=%d", i2c->idx);

	clk_disable(i2c->clk_i2c);
	clk_disable(i2c->clk_reset);
	clk_disable(i2c->clk_gate);
	clk_put(i2c->clk_parent);
	clk_put(i2c->clk_i2c);
	clk_put(i2c->clk_reset);
	clk_put(i2c->clk_gate);

	PRT_TRACE_END("");
}

#endif /* ___I2C___INIT__C___ */
