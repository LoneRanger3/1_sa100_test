/*
 * lb_omx_debug.h - debug api used by lombo omx.
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef __LB_OSCL_DEBUG_H__
#define __LB_OSCL_DEBUG_H__

#ifndef DBG_LEVEL
#define DBG_LEVEL		1
#endif
#ifndef DBG_SECTION_NAME
#define DBG_SECTION_NAME	"LB "
#endif
#include <debug.h>

#define OSCL_ASSERT(...) RT_ASSERT(__VA_ARGS__)

#define OSCL_PRINT(...) rt_kprintf(__VA_ARGS__)

#define OSCL_LOGD(...) LOG_D(__VA_ARGS__)
#define OSCL_LOGI(...) LOG_I(__VA_ARGS__)
#define OSCL_LOGW(...) LOG_W(__VA_ARGS__)
#define OSCL_LOGE(...) LOG_E(__VA_ARGS__)

#define OSCL_TRACE(...) LOG_D(__VA_ARGS__)

#define oscl_param_check_exit(C, V, S) do { \
	if (!(C)) {\
		ret = V;\
		if (S) \
			OSCL_LOGW("check fail:" #C" - error: 0x%x - %s", V, S);\
		else \
			OSCL_LOGW("check fail:" #C" - error: 0x%x", V); \
		goto EXIT; } \
	} while (0)

#define oscl_param_check(C, V, S) do { \
	if (!(C)) {\
		if (S) \
			OSCL_LOGW("check fail:" #C" - error: 0x%x - %s", V, S);\
		else \
			OSCL_LOGW("check fail:" #C" - error: 0x%x", V); \
		return V; } \
	} while (0)

#define dbg_log_port(port, lvl, fmt, ...)		\
	do {							\
		rt_base_t level;				\
		if (port && port->log_level < lvl)		\
			break;					\
		level = rt_hw_interrupt_disable();		\
		if (lvl == DBG_LOG)\
			_DBG_LOG_HDR("D", 0);			\
		if (lvl == DBG_INFO)\
			_DBG_LOG_HDR("I", 32);			\
		if (lvl == DBG_WARNING)\
			_DBG_LOG_HDR("W", 33);			\
		if (lvl == DBG_ERROR)\
			_DBG_LOG_HDR("E", 31);			\
		if (port && port->log_level >= lvl)		\
			rt_kprintf("port:%s-%d ", get_port_name(port),\
				    port->port_param.nPortIndex); \
		rt_kprintf(fmt, ##__VA_ARGS__);			\
		_DBG_LOG_X_END;					\
		rt_hw_interrupt_enable(level);			\
	} while (0)

#define dbg_log_comp(cmp_hdl, lvl, fmt, ...)	\
	do {							\
		rt_base_t level;				\
		lb_omx_component_t *_log_com = NULL;	\
		if (cmp_hdl)				\
			_log_com = get_lb_component(cmp_hdl);	\
		if (_log_com && _log_com->log_level < lvl)		\
			break;					\
		level = rt_hw_interrupt_disable();		\
		if (lvl == DBG_LOG)\
			_DBG_LOG_HDR("D", 0);			\
		if (lvl == DBG_INFO)\
			_DBG_LOG_HDR("I", 32);			\
		if (lvl == DBG_WARNING)\
			_DBG_LOG_HDR("W", 33);			\
		if (lvl == DBG_ERROR)\
			_DBG_LOG_HDR("E", 31);			\
		if (_log_com && _log_com->log_level >= lvl)		\
			rt_kprintf("comp:%s ", _log_com->name); \
		rt_kprintf(fmt, ##__VA_ARGS__);			\
		_DBG_LOG_X_END;					\
		rt_hw_interrupt_enable(level);			\
	} while (0)


#define DEBUG_THREAD_START 1
#define DEBUG_THREAD_STOP 2
#define DEBUG_THREAD_EXIT 3
#define DEBUG_THREAD_MASK 0x7
#define DEBUG_MSG_SHT 0
#define DEBUG_BUF_MGNT_SHT 3
#define get_debug_state(flag, thread_sht)  (((flag)>>(thread_sht))&DEBUG_THREAD_MASK)
#define set_debug_state(flag, thread_sht, val)  \
	(((flag)&~((DEBUG_THREAD_MASK)<<thread_sht))|((val)<<(thread_sht)))
#endif /* __LB_OSCL_DEBUG_H__ */
