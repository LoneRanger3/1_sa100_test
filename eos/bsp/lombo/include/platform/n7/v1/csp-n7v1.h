/*
 * csp-n7v1.h - register operation head file for n7v1
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

#ifndef __CSP_N7V1_H
#define __CSP_N7V1_H

/*
 * macros for module selection.
 * open the macro if you need the module for compiling
 */

#define DEF_N7V1_CPU_CTRL
#define DEF_N7V1_DPU_DC
/* #define DEF_N7V1_DPU_DIT */
#define DEF_N7V1_DPU_ROT
#define DEF_N7V1_DPU_SE
/* #define DEF_N7V1_DPU_TOP */
#define DEF_N7V1_GIC
#define DEF_N7V1_GPADC
#define DEF_N7V1_GPIO
#define DEF_N7V1_GTIMER
#define DEF_N7V1_IR
#define DEF_N7V1_PRCM
#define DEF_N7V1_PTIMER
#define DEF_N7V1_PWM
#define DEF_N7V1_RTC
#define DEF_N7V1_WDOG

/*
 * other common defination for all modules can be put here..
 */

/*
 * phys to virt offset for registers
 */
#if defined(ENABLE_MMU) && defined(__EOS__)
#define VA_OFF_N7V1	0
#else
#define VA_OFF_N7V1	0
#endif

#define VA_AC		VA_OFF_N7V1
#define VA_AES		VA_OFF_N7V1
#define VA_CPU_CTRL	VA_OFF_N7V1
#define VA_DMAC		VA_OFF_N7V1
#define VA_MIPI_DSI	VA_OFF_N7V1
#define VA_TCON0	VA_OFF_N7V1
#define VA_DOSS		VA_OFF_N7V1
#define VA_DC		VA_OFF_N7V1
#define VA_DIT		VA_OFF_N7V1
#define VA_ROT		VA_OFF_N7V1
#define VA_SE		VA_OFF_N7V1
#define VA_DPU		VA_OFF_N7V1
#define VA_EFUSE	VA_OFF_N7V1
#define VA_GICD		VA_OFF_N7V1
#define VA_GICC		VA_OFF_N7V1
#define VA_GMAC		VA_OFF_N7V1
#define VA_GPADC	VA_OFF_N7V1
#define VA_GPIO		VA_OFF_N7V1
#define VA_GTIMER	VA_OFF_N7V1
#define VA_I2C		VA_OFF_N7V1
#define VA_I2S		VA_OFF_N7V1
#define VA_I2S1		VA_OFF_N7V1
#define VA_IR		VA_OFF_N7V1
#define VA_MEM_CTRL	VA_OFF_N7V1
#define VA_PRCM		VA_OFF_N7V1
#define VA_PTIMER	VA_OFF_N7V1
#define VA_PWM		VA_OFF_N7V1
#define VA_RTC		VA_OFF_N7V1
#define VA_SAU		VA_OFF_N7V1
#define VA_SDC		VA_OFF_N7V1
#define VA_SPI		VA_OFF_N7V1
#define VA_UART		VA_OFF_N7V1
#define VA_USB		VA_OFF_N7V1
#define VA_VISS		VA_OFF_N7V1
#define VA_TVD		VA_OFF_N7V1
#define VA_VIC		VA_OFF_N7V1
#define VA_MIPI_CSI	VA_OFF_N7V1
#define VA_WDOG		VA_OFF_N7V1

/*
 * add the module head files if it was defined
 */

#ifdef DEF_N7V1_AC
#include "ac/include.h"
#endif

#ifdef DEF_N7V1_AES
#include "aes/include.h"
#endif

#ifdef DEF_N7V1_CPU_CTRL
#include "cpu_ctrl/include.h"
#endif

#ifdef DEF_N7V1_DMAC
#include "dmac/include.h"
#endif

#ifdef DEF_N7V1_DOSS_MIPI_DSI
#include "doss/mipi_dsi/include.h"
#endif

#ifdef DEF_N7V1_DOSS_TCON0
#include "doss/tcon0/include.h"
#endif

#ifdef DEF_N7V1_DOSS_TOP
#include "doss/top/include.h"
#endif

#ifdef DEF_N7V1_DPU_DC
#include "dpu/dc/include.h"
#endif

#ifdef DEF_N7V1_DPU_DIT
#include "dpu/dit/include.h"
#endif

#ifdef DEF_N7V1_DPU_ROT
#include "dpu/rot/include.h"
#endif

#ifdef DEF_N7V1_DPU_SE
#include "dpu/se/include.h"
#endif

#ifdef DEF_N7V1_DPU_TOP
#include "dpu/top/include.h"
#endif

#ifdef DEF_N7V1_EFUSE
#include "efuse/include.h"
#endif

#ifdef DEF_N7V1_GIC
#include "gic/gicc/include.h"
#include "gic/gicd/include.h"
#endif

#ifdef DEF_N7V1_GMAC
#include "gmac/include.h"
#endif

#ifdef DEF_N7V1_GPADC
#include "gpadc/include.h"
#endif

#ifdef DEF_N7V1_GPIO
#include "gpio/include.h"
#endif

#ifdef DEF_N7V1_GTIMER
#include "gtimer/include.h"
#endif

#ifdef DEF_N7V1_I2C
#include "i2c/include.h"
#endif

#ifdef DEF_N7V1_I2S
#include "i2s/include.h"
#endif

#ifdef DEF_N7V1_I2S1
#include "i2s1/include.h"
#endif

#ifdef DEF_N7V1_IR
#include "ir/include.h"
#endif

#ifdef DEF_N7V1_MEM_CTRL
#include "mem_ctrl/include.h"
#endif

#ifdef DEF_N7V1_PRCM
#include "prcm/include.h"
#endif

#ifdef DEF_N7V1_PTIMER
#include "ptimer/include.h"
#endif

#ifdef DEF_N7V1_PWM
#include "pwm/include.h"
#endif

#ifdef DEF_N7V1_RTC
#include "rtc/include.h"
#endif

#ifdef DEF_N7V1_SAU
#include "sau/include.h"
#endif

#ifdef DEF_N7V1_SDC
#include "sdc/include.h"
#endif

#ifdef DEF_N7V1_SPI
#include "spi/include.h"
#endif

#ifdef DEF_N7V1_UART
#include "uart/include.h"
#endif

#ifdef DEF_N7V1_USB
#include "usb/include.h"
#endif

#ifdef DEF_N7V1_VISS_TOP
#include "viss/top/include.h"
#endif

#ifdef DEF_N7V1_VISS_TVD
#include "viss/tvd/include.h"
#endif

#ifdef DEF_N7V1_VISS_VIC
#include "viss/vic/include.h"
#endif

#ifdef DEF_N7V1_VISS_MIPI_CSI
#include "viss/mipi_csi/include.h"
#endif

#ifdef DEF_N7V1_WDOG
#include "wdog/include.h"
#endif

#endif /* __CSP_N7V1_H */
