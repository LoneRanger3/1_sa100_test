/*
 * lombo_ir.c - lombo ir driver
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

#include <irq_numbers.h>
#include "input.h"
#include <debug.h>
#include <csp.h>
#include "clk/clk.h"
#include "system/system_mq.h"
#define IR_KEY_EVENTS_PER_PACKET	2

static struct input_dev _ir_k_dev;

/* IR interrupt type */
#define K_IR_TO			1
#define K_IR_F_ERR		2
#define K_IR_REPEAT		3
#define K_IR_FRAME		4
#define K_IR_START		5

/* IR protocol type */
#define	K_IR_PROTOCOL_NEC			0
#define	K_IR_PROTOCOL_SONY_SIRC			1
#define	K_IR_PROTOCOL_RC5			2
#define	K_IR_PROTOCOL_RC6			3

#define IR_SIRC_12		0
#define IR_SIRC_15		1
#define IR_SIRC_20		2
#define IR_SIRC_AUTO		3

#define	K_IR_FRAME_GAP_NEC_MS_CNT		110
#define	K_IR_FRAME_GAP_SIRC_MS_CNT		44
#define	K_IR_FRAME_GAP_RC5_MS_CNT		110
#define	K_IR_FRAME_GAP_RC6_MS_CNT		106

#define	K_IR_BIT_PHASE_NEC_US_CNT		560
#define	K_IR_BIT_PHASE_SIRC_US_CNT		600
#define	K_IR_BIT_PHASE_RC5_US_CNT		889
#define	K_IR_BIT_PHASE_RC6_US_CNT		444

#define	K_IR_EFHOSC_FREQ			24000000	/* 24M */
#define	K_IR_WORK_CLK_FREQ			100000		/* 100k */

/* control nec cmd value */
#define IR_CONTROl_ADDR6		0x86
#define IR_CONTROl_ADDR6_VIDEO_SELECT	0x1b
#define IR_CONTROl_ADDR6_MODE_SELECT	0x1a
#define IR_CONTROl_ADDR6_LEFT		0x04
#define IR_CONTROl_ADDR6_RIGHT		0x06
#define IR_CONTROl_ADDR6_MENU		0x05
#define IR_CONTROl_ADDR6_POWER		0x12

#define IR_CONTROl_ADDR11		0x0
#define IR_CONTROl_ADDR11_UP		0x6
#define IR_CONTROl_ADDR11_DOWN		0x5
#define IR_CONTROl_ADDR11_LEFT		0xd
#define IR_CONTROl_ADDR11_RIGHT		0xe
#define IR_CONTROl_ADDR11_OK		0x0
#define IR_CONTROl_ADDR11_POWER		0x2
#define IR_CONTROl_ADDR11_BACK		0x8
#define IR_CONTROl_ADDR11_AV3		0x1e
#define IR_CONTROl_ADDR11_AV4		0x1d
#define IR_CONTROl_ADDR11_AV1		0x9
#define IR_CONTROl_ADDR11_AV2		0xa


union tag_ir_frame {
	u32 val;

	/* nec */
	struct {
		u32 addr:8;
		u32 addr_invert:8;
		u32 cmd:8;
		u32 cmd_invert:8;
	} nec;

	struct {
		u32 addr:16;
		u32 cmd:8;
		u32 cmd_invert:8;
	} nec_ext;

	/* rc5 */
	struct {
		u32 cmd:6;
		u32 addr:5;
		u32 t:1;
		u32 s2:1;
		u32 s1:1;
		u32 rsvd:18;
	} rc5;

	/* rc6 */
	struct {
		u32 info:8;
		u32 ctrl:8;
		u32 tr:1;
		u32 mod_bit:3;
		u32 sb:1;
		u32 rsvd:11;
	} rc6;

	/* sirc_12 */
	struct {
		u32 frame_size:5;
		u32 rsvd:15;
		u32 cmd:7;
		u32 addr:5;
	} sirc;

	/* sirc_15 */
	struct {
		u32 frame_size:5;
		u32 rsvd:12;
		u32 cmd:7;
		u32 addr:8;

	} sirc15;

	/* sirc_20 */
	struct {
		u32 frame_size:5;
		u32 rsvd:7;
		u32 cmd:7;
		u32 addr:5;
		u32 ext:8;
	} sirc20;
};

/* rx ir data */
typedef union tag_ir_frame  ir_frame_t;

/* current process nec protocol */
static int CURRENT_PROTOCOL = K_IR_PROTOCOL_NEC;

/* set IR module enable */
static void csp_ir_set_en(rt_bool_t en)
{
	reg_ir_ctl_t reg;

	reg.val = READREG32(VA_IR_CTL);
	reg.bits.ir_en = en ? 1 : 0;

	WRITEREG32(VA_IR_CTL, reg.val);
}

/* set IR interrupt enable */
static void csp_ir_set_int_en(int t, rt_bool_t en)
{
	reg_ir_int_en_t reg;

	reg.val = READREG32(VA_IR_INT_EN);
	switch (t) {
	case K_IR_TO:
		/* IR frame timeout interrupt enable */
		reg.bits.ir_to = en ? 1 : 0;
		break;
	case K_IR_F_ERR:
		/* IR frame error interrupt enable */
		reg.bits.ir_f_err = en ? 1 : 0;
		break;
	case K_IR_REPEAT:
		/* IR repeat interrupt enable */
		reg.bits.ir_repeat = en ? 1 : 0;
		break;
	case K_IR_FRAME:
		/* IR frame interrupt enable */
		reg.bits.ir_frame = en ? 1 : 0;
		break;
	case K_IR_START:
		/* IR start interrupt enable */
		reg.bits.ir_start = en ? 1 : 0;
		break;
	default:
		LOG_E("Unknown interrupt type: %d", t);
	}

	WRITEREG32(VA_IR_INT_EN, reg.val);
}

/* get IR interrupt pending */
static u32 csp_ir_get_int_pending(int t)
{
	reg_ir_int_pd_t reg;
	u32 val;

	reg.val = READREG32(VA_IR_INT_PD);
	switch (t) {
	case K_IR_TO:
		val = reg.bits.ir_to;
		break;
	case K_IR_F_ERR:
		val = reg.bits.ir_f_err;
		break;
	case K_IR_REPEAT:
		val = reg.bits.ir_repeat;
		break;
	case K_IR_FRAME:
		val = reg.bits.ir_frame;
		break;
	case K_IR_START:
		val = reg.bits.ir_start;
		break;
	default:
		LOG_E("Unknown interrupt type: %d", t);
		val = 0;
	}

	return val;
}

/* clear IR interrupt */
static void csp_ir_int_clr(int t)
{
	reg_ir_int_clr_t reg;

	switch (t) {
	case K_IR_TO:
		reg.bits.ir_to = 1;
		break;
	case K_IR_F_ERR:
		reg.bits.ir_f_err = 1;
		break;
	case K_IR_REPEAT:
		reg.bits.ir_repeat = 1;
		break;
	case K_IR_FRAME:
		reg.bits.ir_frame = 1;
		break;
	case K_IR_START:
		reg.bits.ir_start = 1;
		break;
	default:
		LOG_E("Unknown clear int type: %d", t);
	}

	WRITEREG32(VA_IR_INT_CLR, reg.val);
}

/* set IR protocol */
static void csp_ir_set_protocol(int p)
{
	reg_ir_ctl_t reg;

	reg.val = READREG32(VA_IR_CTL);
	switch (p) {
	case K_IR_PROTOCOL_NEC:
		reg.bits.protocol_sel = 0;
		break;
	case K_IR_PROTOCOL_SONY_SIRC:
		reg.bits.protocol_sel = 1;
		break;
	case K_IR_PROTOCOL_RC5:
		reg.bits.protocol_sel = 2;
		break;
	case K_IR_PROTOCOL_RC6:
		reg.bits.protocol_sel = 3;
		break;
	default:
		LOG_E("Unknown protocol type: %d", p);
	}

	WRITEREG32(VA_IR_CTL, reg.val);
}

/* SIRC protocol select */
static void csp_ir_set_sirc_ext(u32 ext)
{
	reg_ir_ctl_t reg;

	reg.val = READREG32(VA_IR_CTL);
	reg.bits.sirc_ext = ext;

	WRITEREG32(VA_IR_CTL, reg.val);
}

/* set IR clock diver */
static void csp_ir_set_clk_div(u32 div)
{
	reg_ir_ctl_t reg;

	reg.val = READREG32(VA_IR_CTL);
	reg.bits.clk_div = div - 1;

	WRITEREG32(VA_IR_CTL, reg.val);
}

/* set IR timeout threshold */
static void csp_ir_set_timeout_th(u32 th)
{
	reg_ir_timeout_th_t reg;

	reg.val = READREG32(VA_IR_TIMEOUT_TH);
	reg.bits.thd = th;

	WRITEREG32(VA_IR_TIMEOUT_TH, reg.val);
}

/* set IR noise threshold  */
static void csp_ir_set_noise_th(u32 th)
{
	reg_ir_noise_th_t reg;

	reg.val = READREG32(VA_IR_NOISE_TH);
	reg.bits.thd = th;

	WRITEREG32(VA_IR_NOISE_TH, reg.val);
}

/* get IR received data */
static u32 csp_ir_get_data(void)
{
	reg_ir_data_t reg;

	/* this value store the received byte of the IR frame */
	reg.val = READREG32(VA_IR_DATA);
	return reg.bits.data;
}

/* print ir register info for debug */
static void dump_ir_reg(void)
{
	reg_ir_ctl_t ctl_reg;
	reg_ir_timeout_th_t to_th_reg;
	reg_ir_noise_th_t n_th_reg;
	reg_ir_int_en_t en_reg;

	ctl_reg.val = READREG32(VA_IR_CTL);
	LOG_I("ir_en:%d, protocol_sel:%d, sirc_ext:%d, clk_div:%d",
		ctl_reg.bits.ir_en, ctl_reg.bits.protocol_sel,
		ctl_reg.bits.sirc_ext, ctl_reg.bits.clk_div);

	to_th_reg.val = READREG32(VA_IR_TIMEOUT_TH);
	LOG_I("VA_IR_TIMEOUT_TH thd:%d", to_th_reg.bits.thd);

	n_th_reg.val = READREG32(VA_IR_NOISE_TH);
	LOG_I("VA_IR_NOISE_TH thd:%d", n_th_reg.bits.thd);

	en_reg.val = READREG32(VA_IR_INT_EN);
	LOG_I("ir_to:%d, ir_f_err:%d, ir_repeat:%d, ir_frame:%d, ir_start:%d",
		en_reg.bits.ir_to, en_reg.bits.ir_f_err, en_reg.bits.ir_repeat,
		en_reg.bits.ir_frame, en_reg.bits.ir_start);
}

/* report control key envet */
static void report_ir_event_addr6(int cmd)
{
	int code;
	switch (cmd) {
	case IR_CONTROl_ADDR6_VIDEO_SELECT:
		code = KEY_ESC;
		LOG_D("IR control press -> KEY_ESC");
		break;
	case IR_CONTROl_ADDR6_MODE_SELECT:
		code = KEY_M;
		LOG_D("IR control press -> KEY_M");
		break;
	case IR_CONTROl_ADDR6_LEFT:
		code = KEY_UP;
		LOG_D("IR control press -> KEY_UP");
		break;
	case IR_CONTROl_ADDR6_RIGHT:
		code = KEY_DOWN;
		LOG_D("IR control press -> KEY_DOWN");
		break;
	case IR_CONTROl_ADDR6_MENU:
		code = KEY_MENU;
		LOG_D("IR control press -> KEY_MENU");
		break;
	case IR_CONTROl_ADDR6_POWER:
		code = KEY_POWER;
		LOG_D("IR control press -> KEY_POWER");
		break;
	default:
		code = KEY_UNKNOWN;
		LOG_E("IR control press -> undefine");
		break;
	}
	input_report_key(&_ir_k_dev, code, 1);
	input_sync(&_ir_k_dev);
	input_report_key(&_ir_k_dev, code, 0);
	input_sync(&_ir_k_dev);
}

static void report_ir_event_addr11(int cmd)
{
	int code;
	switch (cmd) {
	case IR_CONTROl_ADDR11_UP:
		code = KEY_ESC;
		LOG_D("IR control press -> KEY_ESC");
		break;
	case IR_CONTROl_ADDR11_DOWN:
		code = KEY_M;
		LOG_D("IR control press -> KEY_M");
		break;
	case IR_CONTROl_ADDR11_LEFT:
		code = KEY_UP;
		LOG_D("IR control press -> KEY_UP");
		break;
	case IR_CONTROl_ADDR11_RIGHT:
		code = KEY_DOWN;
		LOG_D("IR control press -> KEY_DOWN");
		break;
	case IR_CONTROl_ADDR11_OK:
		code = KEY_MENU;
		LOG_D("IR control press -> KEY_MENU");
		break;
	case IR_CONTROl_ADDR11_POWER:
		code = KEY_POWER;
		LOG_D("IR control press -> KEY_POWER");
		break;
	case IR_CONTROl_ADDR11_BACK:
		code = KEY_MENU;
		LOG_D("IR control press -> KEY_MENU");
		break;
	case IR_CONTROl_ADDR11_AV1:
		code = KEY_F1;
		break;
	case IR_CONTROl_ADDR11_AV2:
		code = KEY_F2;
		break;
	case IR_CONTROl_ADDR11_AV3:
		code = KEY_F3;
		break;
	case IR_CONTROl_ADDR11_AV4:
		code = KEY_F4;
		break;
	default:
		code = KEY_UNKNOWN;
		LOG_E("IR control press -> undefine");
		break;
	}
	input_report_key(&_ir_k_dev, code, 1);
	input_sync(&_ir_k_dev);
	input_report_key(&_ir_k_dev, code, 0);
	input_sync(&_ir_k_dev);
}

/* decode the frame by protocol */
static void decode_ir_frame(ir_frame_t frame, int protocol)
{

	LOG_W("frame.val:%x\n", frame.val);
	if (protocol == K_IR_PROTOCOL_NEC) {
		/* nec*/
		if (((frame.nec.cmd_invert + frame.nec.cmd) != 0xff) &&
			((frame.nec.addr_invert + frame.nec.addr) != 0xff)) {
				LOG_W("WARN !!!! nec-frame-invalid %d %d %d %d\n",
					frame.nec.cmd_invert, frame.nec.cmd,
					frame.nec.addr_invert, frame.nec.addr);
			}
		else {
			LOG_W("ir-nec-frame: addr %x, cmd %x",
				frame.nec.addr, frame.nec.cmd);
			if (frame.nec.addr == 0x86)
				report_ir_event_addr6(frame.nec.cmd);
			else if (frame.nec.addr == 0x0)
				report_ir_event_addr11(frame.nec.cmd);
			else
				LOG_E("undefine addr:0x%x", frame.nec.addr);
		}
	} else if (protocol == K_IR_PROTOCOL_SONY_SIRC) {
		/* sony-sirc */
		if (frame.sirc.frame_size == 12)
			LOG_I("ir-sirc-frame: frame_size %d,addr %x,cmd %x",
				frame.sirc.frame_size, frame.sirc.addr,
				frame.sirc.cmd);
		else if (frame.sirc.frame_size == 15)
			LOG_I("ir-sirc-frame: frame_size %d,addr %x,cmd %x",
				frame.sirc.frame_size, frame.sirc15.addr,
				frame.sirc15.cmd);
		else if (frame.sirc.frame_size == 20)
			LOG_I("ir-sirc-frame: frame_size %d,addr %x,cmd %x",
				frame.sirc.frame_size, frame.sirc20.addr,
				frame.sirc20.cmd);
		else
			LOG_W("ir-sirc-frame: ?????????");

	} else if (protocol == K_IR_PROTOCOL_RC5) {
		/* rc5 */
		LOG_I("ir-rc5-frame: s1 %d, s2 %d, t %d, addr %x, cmd %x",
			frame.rc5.s1, frame.rc5.s2, frame.rc5.t,
			frame.rc5.addr, frame.rc5.cmd);
	} else if (protocol == K_IR_PROTOCOL_RC6) {
		/* rc6 */
		LOG_I("ir-rc6-frame: s%d, mode%d, tr %d, ctrl %x, info %x",
			frame.rc6.sb, frame.rc6.mod_bit, frame.rc6.tr,
			frame.rc6.ctrl, frame.rc6.info);
	}
}


/* initialize IR register */
static void csp_ir_init(void)
{
	u32 src_freq, ir_clk_div, ir_work_freq, protocol;

	u32 frame_gap_ms_cnt;
	u32 frame_timeout_ms_cnt;
	u32 frame_timeout_reg_val;
	u32 bif_phase_us_cnt;
	u32 noise_th_reg_val;

	protocol = CURRENT_PROTOCOL;

	LOG_I("--------- csp_ir_init ---------");
	/* dump_ir_reg(); */

	/* protocol select */
	csp_ir_set_protocol(protocol);
	if (protocol == K_IR_PROTOCOL_SONY_SIRC)
		csp_ir_set_sirc_ext(IR_SIRC_AUTO);

	/* ir driver cfg */
	src_freq = K_IR_EFHOSC_FREQ;
	ir_work_freq = K_IR_WORK_CLK_FREQ;

	ir_clk_div = src_freq / ir_work_freq;
	csp_ir_set_clk_div(ir_clk_div);

	/* ir timeout threshold */
	if (protocol == K_IR_PROTOCOL_NEC)
		frame_gap_ms_cnt = K_IR_FRAME_GAP_NEC_MS_CNT;
	else if (protocol == K_IR_PROTOCOL_SONY_SIRC)
		frame_gap_ms_cnt = K_IR_FRAME_GAP_SIRC_MS_CNT;
	else if (protocol == K_IR_PROTOCOL_RC5)
		frame_gap_ms_cnt = K_IR_FRAME_GAP_RC5_MS_CNT;
	else if (protocol == K_IR_PROTOCOL_RC6)
		frame_gap_ms_cnt = K_IR_FRAME_GAP_RC6_MS_CNT;

	/* frame-timne-out = 2/3*frame-gap */
	frame_timeout_ms_cnt = frame_gap_ms_cnt * 2 / 3;

	/* (val+1) * 128 * 1/freq = timeout-ms-cnt/1000 */
	frame_timeout_reg_val = (frame_timeout_ms_cnt*ir_work_freq
		/ 1000 / 128) - 1;
	csp_ir_set_timeout_th(frame_timeout_reg_val);

	/* ir noise-threshold */
	if (protocol == K_IR_PROTOCOL_NEC)
		bif_phase_us_cnt = K_IR_BIT_PHASE_NEC_US_CNT;	/* 560us */
	else if (protocol == K_IR_PROTOCOL_SONY_SIRC)
		bif_phase_us_cnt = K_IR_BIT_PHASE_SIRC_US_CNT;	/* 600us */
	else if (protocol == K_IR_PROTOCOL_RC5)
		bif_phase_us_cnt = K_IR_BIT_PHASE_RC5_US_CNT;	/* 889us */
	else if (protocol == K_IR_PROTOCOL_RC6)
		bif_phase_us_cnt = K_IR_BIT_PHASE_RC6_US_CNT;	/* 444us */

	/* noise-th = 1/10 bit-phase */
	noise_th_reg_val = ((bif_phase_us_cnt * ir_work_freq
		/ 1000000 / 10) + 1);
	csp_ir_set_noise_th(noise_th_reg_val);

	/* set enable */
	csp_ir_set_en(RT_TRUE);
	csp_ir_set_int_en(K_IR_TO, RT_TRUE);
	csp_ir_set_int_en(K_IR_F_ERR, RT_TRUE);
	csp_ir_set_int_en(K_IR_REPEAT, RT_TRUE);
	csp_ir_set_int_en(K_IR_FRAME, RT_TRUE);
	csp_ir_set_int_en(K_IR_START, RT_TRUE);

	dump_ir_reg();
}

/* IR interrupt handle function */
static void lombo_ir_irq(int vector, void *param)
{
	ir_frame_t frame;

	/* timeout interrupt */
	if (csp_ir_get_int_pending(K_IR_TO)) {
		/* LOG_E("<K_IR_TO>"); */
		csp_ir_int_clr(K_IR_TO);
	}

	/* frame error interrupt */
	if (csp_ir_get_int_pending(K_IR_F_ERR)) {
		LOG_E("<K_IR_F_ERR>");
		csp_ir_int_clr(K_IR_F_ERR);
	}

	/* repeat interrupt */
	if (csp_ir_get_int_pending(K_IR_REPEAT)) {
		LOG_E("<K_IR_REPEAT>");
		csp_ir_int_clr(K_IR_REPEAT);
	}

	/* start interrupt */
	if (csp_ir_get_int_pending(K_IR_START)) {
		LOG_E("<K_IR_START>");
		csp_ir_int_clr(K_IR_START);
	}

	/* frame interrupt */
	if (csp_ir_get_int_pending(K_IR_FRAME)) {
		int i = 0;
		LOG_E("<K_IR_FRAME>");
		csp_ir_int_clr(K_IR_FRAME);
		/* handle the ir frame after clear the pending */
		while (i < 10) {
			if (csp_ir_get_int_pending(K_IR_FRAME)) {
				csp_ir_int_clr(K_IR_FRAME);
				i++;
				LOG_E("try to clear pending: %d", i);
			} else
				break;
		}

		frame.val = csp_ir_get_data();
		decode_ir_frame(frame, CURRENT_PROTOCOL);
	}
}

static rt_err_t ir_clk_init()
{
	clk_handle_t clk_gate, clk_reset, clk_module;
	rt_err_t ret;

	/* apb_ir_gate */
	clk_gate = clk_get(CLK_NAME_APB_IR_GATE);
	if (clk_gate == RT_NULL) {
		LOG_E("clk_get CLK_NAME_APB_IR_GATE return NULL");
		return -RT_ERROR;
	}

	ret = clk_enable(clk_gate);
	if (ret != 0) {
		LOG_E("clk_enable clk_gate error");
		return -RT_ERROR;
	}

	/* apb_ir_reset */
	clk_reset = clk_get(CLK_NAME_APB_IR_RESET);
	if (clk_reset == RT_NULL) {
		LOG_E("clk_get CLK_NAME_APB_IR_RESET return NULL");
		return -RT_ERROR;
	}

	ret = clk_enable(clk_reset);
	if (ret != 0) {
		LOG_E("clk_enable clk_reset error");
		return -RT_ERROR;
	}

	/* ir_clk */
	clk_module = clk_get(CLK_NAME_IR_CLK);
	if (clk_module == RT_NULL) {
		LOG_E("clk_get CLK_NAME_IR_CLK return NULL");
		return -RT_ERROR;
	}

	ret = clk_enable(clk_module);
	if (ret != 0) {
		LOG_E("clk_enable clk_module error");
		return -RT_ERROR;
	}

	return RT_EOK;
}

static rt_err_t register_ir_key()
{
	rt_err_t ret;

	_ir_k_dev.name = "ir_key";
	rt_list_init(&(_ir_k_dev.node));
	rt_list_init(&(_ir_k_dev.h_list));

	/* set the device generate event and key type */
	set_bit(EV_KEY, _ir_k_dev.evbit);
	set_bit(KEY_ESC, _ir_k_dev.keybit);
	set_bit(KEY_M, _ir_k_dev.keybit);
	set_bit(KEY_UP, _ir_k_dev.keybit);
	set_bit(KEY_MENU, _ir_k_dev.keybit);
	set_bit(KEY_DOWN, _ir_k_dev.keybit);
	set_bit(KEY_POWER, _ir_k_dev.keybit);
	set_bit(KEY_F1, _ir_k_dev.keybit);
	set_bit(KEY_F2, _ir_k_dev.keybit);
	set_bit(KEY_F3, _ir_k_dev.keybit);
	set_bit(KEY_F4, _ir_k_dev.keybit);

	_ir_k_dev.num_vals = 0;
	_ir_k_dev.max_vals = IR_KEY_EVENTS_PER_PACKET + 2;
	_ir_k_dev.vals = rt_malloc(sizeof(struct input_value) * _ir_k_dev.max_vals);
	if (_ir_k_dev.vals == RT_NULL) {
		LOG_E("rt_malloc for input_dev vals error");
		return -RT_ENOMEM;
	}

	ret = input_register_device(&_ir_k_dev);
	if (ret != RT_EOK) {
		LOG_E("input_register_device error");
		goto err_exit;
	}

	return RT_EOK;

err_exit:
	rt_free(_ir_k_dev.vals);
	return ret;
}

int lombo_ir_init(void)
{
	rt_err_t ret;

	/* ir clock init */
	ret = ir_clk_init();
	if (ret != RT_EOK) {
		LOG_E("ir_clk_init error");
		return -RT_ERROR;
	}

	/* init ir */
	csp_ir_init();

	/* interrupt handler */
	rt_hw_interrupt_install(INT_IR_RX, lombo_ir_irq,
				RT_NULL, "ir_irq");
	rt_hw_interrupt_umask(INT_IR_RX);

	/* register ir key to input core */
	ret = register_ir_key();
	if (ret != RT_EOK) {
		LOG_E("register_ir_key error");
		return -RT_ERROR;
	}
	LOG_I("===== lombo_ir_init finished =====");
	return 0;
}

INIT_COMPONENT_EXPORT(lombo_ir_init);
