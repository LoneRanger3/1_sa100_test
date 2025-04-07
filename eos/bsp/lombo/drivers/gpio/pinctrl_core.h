/*
 * pinctrl_core.h - Gpio module head file
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

#ifndef __PINCTRL_CORE_H__
#define __PINCTRL_CORE_H__

#include <rtconfig.h>
#include <csp.h>
#include <list.h>

#define	PINCTRL_NAME_MAX	16

/* maximum length of a pin in pin descriptor (example: "gpa0") */
#define PIN_NAME_LENGTH		PINCTRL_NAME_MAX

/**
 * enum pincfg_type - pin configuration types supported.
 * @PINCFG_TYPE_FUNC: Function configuration.
 * @PINCFG_TYPE_PUD: Pull up/down configuration.
 * @PINCFG_TYPE_DRV: Drive strength configuration.
 * @PINCFG_TYPE_DAT: Pin value configuration.
 * @PINCFG_TYPE_PUD_REG: Pull up/down resistor configuration.
 */
enum pincfg_type {
	PINCFG_TYPE_FUNC,
	PINCFG_TYPE_PUD,
	PINCFG_TYPE_DRV,
	PINCFG_TYPE_DAT,
	PINCFG_TYPE_PUD_REG,
	PINCFG_TYPE_NUM
};

/**
 * enum eint_type - bank external interrupt types
 * @EINT_TYPE_NONE: bank does not support external interrupts
 * @EINT_TYPE_GPIO: bank supportes external gpio interrupts
 * @EINT_TYPE_WKUP: bank supportes external wakeup interrupts
 */
enum eint_type {
	EINT_TYPE_NONE,
	EINT_TYPE_GPIO,
	EINT_TYPE_WKUP,
};

typedef	void (*irq_handler_t)(void *data);

struct pinctrl_irq_desc {
	irq_handler_t handler;
	void *data;
};

#define FUNCTION_ENABLE		1
#define FUNCTION_DISABLE	0

#define REG_WRITE	1
#define REG_READ	0

#define PIN_GROUP_ENABLE	1
#define PIN_GROUP_DISABLE	0

/**
 * bank registers number define.
 * 4 function + 4 pull up/down + 4 driver + 1 data + 1 pull up/down resistor
 */
#define BANK_REGISTERS_NUM 14

/* registers address step*/
#define ADDR_STEP	0x4

/* register bit width */
#define REG_BIT_WIDTH	32

/* function number for pin as gpio output */
#define FUN_INPUT	0x1
#define FUN_OUTPUT	0x2

/*
 * pin configuration (pull up/down and drive strength) type and its value are
 * packed together into a 16-bits. The upper 8-bits represent the configuration
 * type and the lower 8-bits hold the value of the configuration type.
 */
#define PINCFG_TYPE_MASK		0xFF
#define PINCFG_VALUE_SHIFT		8
#define PINCFG_VALUE_MASK		(0xFF << PINCFG_VALUE_SHIFT)
#define PINCFG_PACK(type, value)	(((value) << PINCFG_VALUE_SHIFT) | type)
#define PINCFG_UNPACK_TYPE(cfg)		((cfg) & PINCFG_TYPE_MASK)
#define PINCFG_UNPACK_VALUE(cfg)	(((cfg) & PINCFG_VALUE_MASK) >> \
						PINCFG_VALUE_SHIFT)

/**
 * struct pin_bank_type: pin bank type description
 * @fld_width: widths of config bitfields
 * @reg_offset: offsets of registers
 */
struct pin_bank_type {
	u8 fld_width[PINCFG_TYPE_NUM];
	u8 reg_offset[PINCFG_TYPE_NUM];
};

/**
 * struct pin_bank_data: represent a controller pin-bank (init data).
 * @type: type of the bank (register offsets and bitfield widths)
 * @pctl_offset: starting offset of the pin-bank registers.
 * @nr_pins: number of pins included in this bank.
 * @eint_func: function to set in CON register to configure pin as EINT.
 * @eint_type: type of the external interrupt supported by the bank.
 * @eint_mask: bit mask of pins which support EINT function.
 * @eint_nr: irq number of the bank
 * @eint_banks_nr: how may banks share this irq
 * @name: name to be prefixed for each pin in this pin bank.
 */
struct pin_bank_data {
	const char	*name;
	const struct pin_bank_type *type;
	u32		pctl_offset;
	u8		npins;
	u8		eint_func;
	enum eint_type	eint_type;
	u32		eint_mask;
	u32		eint_nr;
	u32		eint_banks_nr;
};

/**
 * struct pinctrl_pin_desc - boards/machines provide information on their
 * pins, pads or other muxable units in this struct
 * @number: unique pin number from the global pin number space
 * @name: a name for this pin
 */
struct pinctrl_pin_desc {
	u32 number;
	const char *name;
	void *owner;
};

/**
 * struct lombo_pin_bank: represent a controller pin-bank.
 * @type: type of the bank (register offsets and bitfield widths)
 * @pctl_offset: starting offset of the pin-bank registers.
 * @npins: number of pins included in this bank.
 * @eint_func: function to set in CON register to configure pin as EINT.
 * @eint_type: type of the external interrupt supported by the bank.
 * @eint_mask: bit mask of pins which support EINT function.
 * @eint_nr: irq number for this bank.
 * @eint_banks_nr: how many banks share this irq.
 * @name: name to be prefixed for each pin in this pin bank.
 * @pin_base: starting pin number of the bank.
 * @soc_priv: per-bank private data for SoC-specific code.
 */
struct pin_bank {
	const struct pin_bank_type	*type;
	u32				pctl_offset;
	u8				npins;
	u8				eint_func;
	enum eint_type			eint_type;
	u32				eint_mask;
	u32				eint_nr;
	u32				eint_banks_nr;
	const char			*name;
	u32				pin_base;
	struct pinctrl_irq_desc		*irqdesc;
	void				*soc_priv;
};

struct pinctrl_setting {
	struct list_head node;
	u32 *pins; /* Pin number array of this group */
	u32 num_pins;
	unsigned long *configs;
	unsigned long num_configs;
};

struct pinctrl_group {
	struct list_head node;
	char name[PINCTRL_NAME_MAX];
	u32 state;
	struct pinctrl_setting data;
};

struct pinctrl_dev {
	char name[PINCTRL_NAME_MAX];
	struct list_head groups;
	u32 group_num;
};

/**
 * struct pinctrl: pin controller driver data
 */
struct pinctrl {
	struct list_head node;
	struct pinctrl_dev *dev;
	void *drv_data;
};

#endif/* __PINCTRL_CORE_H__ */

