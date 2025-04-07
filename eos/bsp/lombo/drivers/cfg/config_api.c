/*
 * config_api.c - Standard functionality for the config.bin read/write API.
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

#define DBG_LEVEL	0
#include <debug.h>
#include <rtthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "json_parser.h"
#include "config_api.h"

typedef struct {
	char *blob;
	config_header_t *header;
	blob_mod_idx_t *module_idx;
	int module_cnt;
} config_rw_t;

config_rw_t g_config_rw;

int _is_header_valid(config_header_t *header)
{
	if (header == NULL
		|| header->magic != CONFIG_MAGIC
		|| header->version != CONFIG_VERSION
		|| (header->off_module_idx < sizeof(config_header_t))
		|| (header->off_structs < header->off_module_idx)
		|| (header->off_strings < header->off_structs)
		|| (header->totalsize < header->off_strings)
		|| (header->size_module_idx < 0)
		|| (header->size_strings < 0)
		|| (header->size_structs < 0))
		return 0;
	return 1;
}

blob_mod_idx_t *_get_blob_module_index(int index)
{
	if (index < 0 || index >= g_config_rw.module_cnt)
		return NULL;
	return &g_config_rw.module_idx[index];

}

_blob_prop_t *_get_blob_struct(int offset)
{
	if (offset > g_config_rw.header->size_structs)
		return NULL;
	if ((offset % CONFIG_TAGALIGN_SIZE) != 0)
		return NULL;

	return (_blob_prop_t *)(g_config_rw.blob
			+ g_config_rw.header->off_structs + offset);
}

void *_get_blob_private_data(_blob_prop_t *prop)
{
	return (char *)prop + sizeof(_blob_prop_t);
}

char *_get_blob_string(int offset)
{
	if (offset < 0 || offset >= g_config_rw.header->size_strings)
		return NULL;
	return g_config_rw.blob + g_config_rw.header->off_strings + offset;
}

_blob_prop_t *_get_module_by_name(const char *modulename)
{
	int i;
	blob_mod_idx_t *module_idx;
	_blob_prop_t *module;
	char *tmpname;

	if (modulename == NULL || g_config_rw.blob == NULL)
		return NULL;

	for (i = 0; i < g_config_rw.module_cnt; i++) {
		module_idx = _get_blob_module_index(i);
		if ((module_idx != NULL)
				&& (module_idx->name[0] == modulename[0])
				&& (module_idx->name[1] == modulename[1])) {
			module = _get_blob_struct(module_idx->offset);
			if (module == NULL || module->tag != CONFIG_TYPE_MODULE)
				break;
			tmpname = _get_blob_string(module->name_off);
			if (tmpname && (strcasecmp(tmpname, modulename) == 0))
				return module;
		}
	}

	return NULL;
}

static inline _blob_prop_t *_get_next_prop(_blob_prop_t *prop)
{
	char *tmp;
	tmp = (char *)prop;
	tmp += CONFIG_TAGALIGN(sizeof(_blob_prop_t) + prop->len);
	return (_blob_prop_t *)(tmp);
}

static int _is_prop_valid(_blob_prop_t *prop)
{
	int data_len = 0;
	int is_valid = 1;
	char *start;
	char *end;

	start = g_config_rw.blob + g_config_rw.header->off_structs;
	if (prop == NULL || (char *)prop < start)
		return 0;

	switch (prop->tag) {
	case CONFIG_TYPE_INT:
		data_len = sizeof(int);
		if (prop->count != 1)
			is_valid = 0;
		break;

	case CONFIG_TYPE_STRING:
		data_len = CONFIG_SIZE_OFFSET;
		if (prop->count != 1)
			is_valid = 0;
		break;

	case CONFIG_TYPE_INT_ARRAY:
		data_len = sizeof(int) * prop->count;
		break;

	case CONFIG_TYPE_STRING_ARRAY:
		data_len = CONFIG_SIZE_OFFSET * prop->count;
		break;

	case CONFIG_TYPE_GPIO:
		data_len = CONFIG_ALIGN(sizeof(_blob_gpio_t),
			sizeof(gpio_pin_t)) + prop->count * sizeof(gpio_pin_t);
		break;

	default:
		data_len = -1;
		break;
	}

	end = g_config_rw.blob + g_config_rw.header->off_strings;
	if (data_len < 0 || (data_len != prop->len)
			|| (char *)prop + data_len > end)
		is_valid = 0;

	return is_valid;
}

_blob_prop_t *_get_config_by_name(const char *modulename, const char *name)
{
	int i;
	_blob_prop_t *prop;
	_blob_prop_t *end;
	char *tmpname;
	_blob_prop_t *module;

	if (modulename == NULL || name == NULL || g_config_rw.blob == NULL)
		return NULL;

	module = _get_module_by_name(modulename);
	if (module == NULL || module->count <= 0)
		return NULL;

	end = (_blob_prop_t *)(g_config_rw.blob
			+ g_config_rw.header->off_structs
			+ g_config_rw.header->size_structs);

	prop = module;
	for (i = 0; i < module->count; i++) {
		prop = _get_next_prop(prop);
		if (prop == NULL || prop >= end
			|| prop->tag == CONFIG_TYPE_MODULE) {
			prop = NULL;
			break;
		}
		tmpname = _get_blob_string(prop->name_off);
		if (tmpname && (strcasecmp(tmpname, name) == 0))
			break;
	}
	if (i == module->count)
		prop = NULL;
	if (!_is_prop_valid(prop))
		prop = NULL;

	return prop;
}

int _config_get_string_array(const char *modulename,
				const char *propname,
				const char **out_strs,
				int count)
{
	_blob_prop_t *prop;
	config_offset_t *offset;
	int tmp_cnt;
	int i;

	if (modulename == NULL || propname == NULL
			|| out_strs == NULL || count <= 0)
		return 0;

	prop = _get_config_by_name(modulename, propname);
	if (prop == NULL)
		return 0;
	if ((prop->tag != CONFIG_TYPE_STRING)
		&& (prop->tag != CONFIG_TYPE_STRING_ARRAY))
		return 0;
	memset(out_strs, 0, count * sizeof(char *));
	tmp_cnt = prop->count;
	if (tmp_cnt > count)
		tmp_cnt = count;

	offset = _get_blob_private_data(prop);
	for (i = 0; i < tmp_cnt; i++) {
		out_strs[i] = _get_blob_string(offset[i]);
		if (out_strs[i] == NULL) {
			tmp_cnt = 0;
			memset(out_strs, 0, count * sizeof(char *));
			break;
		}
	}
	return tmp_cnt;
}

int _config_get_u32_array(const char *modulename,
				const char *propname,
				u32 *out_values,
				int count)
{
	_blob_prop_t *prop;
	void *data;
	int tmp_cnt;

	if (modulename == NULL || propname == NULL
			|| out_values == NULL || count <= 0)
		return 0;

	prop = _get_config_by_name(modulename, propname);
	if (prop == NULL)
		return 0;
	if ((prop->tag != CONFIG_TYPE_INT)
		&& (prop->tag != CONFIG_TYPE_INT_ARRAY))
		return 0;
	tmp_cnt = prop->count;
	if (tmp_cnt > count)
		tmp_cnt = count;

	data = _get_blob_private_data(prop);
	memcpy(out_values, data, tmp_cnt * sizeof(int));
	return tmp_cnt;
}

/**
 * config_init - initialize the data structures in with blob data
 * @blob:	blob data being initialized
 * @length:	length of blob buffer
 *
 * Returns 0 on success, -EERROR otherwise..
 */
int __config_init(void *blob, int length)
{
	config_header_t *header = blob;
	char *string_end;
	if (!_is_header_valid(blob))
		return -1;
	if (length < header->off_strings + header->size_strings)
		return -1;
	string_end = (char *)blob + header->off_strings
			+ header->size_strings - 1;
	*string_end = '\0';

	memset(&g_config_rw, 0, sizeof(g_config_rw));
	g_config_rw.blob = blob;
	g_config_rw.header = header;
	g_config_rw.module_idx = (blob_mod_idx_t *)(g_config_rw.blob
					+ header->off_module_idx);
	g_config_rw.module_cnt = header->size_module_idx/sizeof(blob_mod_idx_t);
	return 0;
}

void _print_prop(_blob_prop_t *prop)
{
	int *data;
	config_offset_t *offset;
	int i;
	const _blob_gpio_t *gpio;
	const gpio_pin_t *pins;

	LOG_RAW("\t\t{\"%s\": ", _get_blob_string(prop->name_off));
	switch (prop->tag) {
	case CONFIG_TYPE_INT:
		data = _get_blob_private_data(prop);
		LOG_RAW("0x%x", data[0]);
		break;

	case CONFIG_TYPE_STRING:
		offset = _get_blob_private_data(prop);
		LOG_RAW("\"%s\"", _get_blob_string(offset[0]));
		break;

	case CONFIG_TYPE_INT_ARRAY:
		data = _get_blob_private_data(prop);
		LOG_RAW("[");
		for (i = 0; i < prop->count; i++) {
			LOG_RAW("0x%x", data[i]);
			if (i != prop->count - 1)
				LOG_RAW(", ");
		}
		LOG_RAW("]");
		break;

	case CONFIG_TYPE_STRING_ARRAY:
		offset = _get_blob_private_data(prop);
		LOG_RAW("[");
		for (i = 0; i < prop->count; i++) {
			LOG_RAW("\"%s\"", _get_blob_string(offset[i]));
			if (i != prop->count - 1)
				LOG_RAW(", ");
		}
		LOG_RAW("]");
		break;

	case CONFIG_TYPE_GPIO:
		LOG_RAW("[\n");
		gpio = _get_blob_private_data(prop);
		if (gpio->npins >= CONFIG_MAX_PINS)
			return;
		pins = (gpio_pin_t *)((char *)gpio
			+ CONFIG_ALIGN(sizeof(_blob_gpio_t), sizeof(gpio_pin_t)));
		LOG_RAW("\t\t\t{\"pins\" : [");
		for (i = 0; i < prop->count; i++) {
			LOG_RAW("\"gp%c-%d\"",
				'a' + pins[i].port, pins[i].pin);
			if (i != prop->count - 1)
				LOG_RAW(", ");
		}
		LOG_RAW("]},\n");
		if (gpio->func != 0xFF)
			LOG_RAW("\t\t\t{\"pin-function\" : %d},\n", gpio->func);
		if (gpio->data != 0xFF)
			LOG_RAW("\t\t\t{\"pin-data\" : %d},\n", gpio->data);
		if (gpio->drv_level != 0xFF)
			LOG_RAW("\t\t\t{\"pin-drv\" : %d},\n", gpio->drv_level);
		if (gpio->pull_updown != 0xFF)
			LOG_RAW("\t\t\t{\"pin-pud\" : %d},\n", gpio->pull_updown);
		if (gpio->pull_resisters != 0xFF)
			LOG_RAW("\t\t\t{\"pin-res\" : %d},\n", gpio->pull_resisters);
		LOG_RAW("\t\t\t]\n");
		LOG_RAW("\t\t");
		break;

	default:
		LOG_RAW("unknown type:%d", prop->tag);
		break;
	}
	LOG_RAW("}");
}

void _print_module(_blob_prop_t *module)
{
	int i;
	_blob_prop_t *prop;
	_blob_prop_t *end;

	if (module == NULL)
		return;
	LOG_RAW("\t\"%s\":[\n", _get_blob_string(module->name_off));
	end = (_blob_prop_t *)(g_config_rw.blob
			+ g_config_rw.header->off_structs
			+ g_config_rw.header->size_structs);
	prop = module;
	for (i = 0; i < module->count; i++) {
		prop = _get_next_prop(prop);
		if (prop == NULL || prop >= end
			|| prop->tag == CONFIG_TYPE_MODULE) {
			prop = NULL;
			break;
		}
		_print_prop(prop);
		if (i != module->count - 1)
			LOG_RAW(",");
		LOG_RAW("\n");
	}
	LOG_RAW("\t]");
}

void config_get(int argc, char **argv)
{
	int i;
	blob_mod_idx_t *module_idx;
	const char *modulename = NULL;
	const char *propname = NULL;
	_blob_prop_t *module;
	_blob_prop_t *prop;

	if (argc > 3) {
		LOG_RAW("err input!");
		return;
	}
	if (g_config_rw.blob == NULL)
		return;

	if (argc == 2) {
		modulename = argv[1];
		module = _get_module_by_name(modulename);
		if (module == NULL) {
			LOG_RAW("module %s not found\n", modulename);
			LOG_RAW("use command as \"%s [module] [prop]\"\n",
				argv[0]);
		} else {
			_print_module(module);
			LOG_RAW("\n");
		}
		return;
	}
	if (argc == 3) {
		modulename = argv[1];
		propname = argv[2];
		prop = _get_config_by_name(modulename, propname);
		if (prop == NULL) {
			LOG_RAW("module %s, property %s not found\n",
				modulename, propname);
			LOG_RAW("use command as \"%s [module] [prop]\"\n",
				argv[0]);
		} else {
			LOG_RAW("\t\"%s\":[\n", modulename);
			_print_prop(prop);
			LOG_RAW("\t]\n");
		}
		return;
	}


	LOG_RAW("\n{");
	for (i = 0; i < g_config_rw.module_cnt; i++) {
		module_idx = _get_blob_module_index(i);
		if (module_idx == NULL)
			break;
		module = _get_blob_struct(module_idx->offset);
		if (module == NULL || module->tag != CONFIG_TYPE_MODULE)
			break;
		_print_module(module);
		if (i != g_config_rw.module_cnt - 1)
			LOG_RAW(",");
		LOG_RAW("\n");
	}
	LOG_RAW("}\n");
}

MSH_CMD_EXPORT(config_get, get module configs);


/**
 * config_get_u32 - get a u32 value for specific module and property
 * @modulename:	module name
 * @propname:	property name
 * @out_value:	output value
 *
 * Returns 0 on success, -EERROR otherwise..
 */
int config_get_u32(const char *modulename,
				const char *propname,
				u32 *out_value)
{
	int ret;
	ret = _config_get_u32_array(modulename, propname, out_value, 1);
	return (ret == 1) ? 0 : -1;
}

/**
 * config_get_string - get a string for specific module and property
 * @modulename:	module name
 * @propname:	property name
 * @out_string:	output string
 *
 * Returns 0 on success, -EERROR otherwise..
 */
int config_get_string(const char *modulename,
				const char *propname,
				const char **out_string)
{
	int ret;
	ret = _config_get_string_array(modulename, propname, out_string, 1);
	return (ret == 1) ? 0 : -1;
}

/**
 * config_count_elems - get count of elems
 * @modulename:	module name
 * @propname:	property name
 *
 * Returns count of elems of the specific property
 */
int config_count_elems(const char *modulename,
				const char *propname)
{
	_blob_prop_t *prop;
	int count = -1;

	prop = _get_config_by_name(modulename, propname);
	if (prop != NULL)
		count = prop->count;

	return count;
}

/**
 * config_get_gpio - get gpios for specific module and property
 * @modulename:	module name
 * @propname:	property name
 * @gpio:	output gpio
 *
 * Returns 0 on success, -EERROR otherwise..
 */
int config_get_gpio(const char *modulename,
				const char *propname,
				config_gpio_t *gpio)
{
	_blob_prop_t *prop;
	void *data;
	gpio_pin_t *pins;

	if (modulename == NULL || propname == NULL
			|| gpio == NULL)
		return -1;

	prop = _get_config_by_name(modulename, propname);
	if (prop == NULL || prop->tag != CONFIG_TYPE_GPIO)
		return -1;

	data = _get_blob_private_data(prop);
	if (data == NULL)
		return -1;

	memcpy(gpio, data, sizeof(_blob_gpio_t));
	if (gpio->npins >= CONFIG_MAX_PINS)
		return -1;

	pins = (gpio_pin_t *)((char *)data
			+ CONFIG_ALIGN(sizeof(_blob_gpio_t), sizeof(gpio_pin_t)));
	gpio->pins = pins;

	return 0;
}

/**
 * config_get_u32_array - get u32 array for specific module and property
 * @modulename:	module name
 * @propname:	property name
 * @out_values:	output u32 array. out_values array is alloc by the caller
 * @count:	the sizeof output array
 *
 * Returns count of array members.
 */
int config_get_u32_array(const char *modulename,
				const char *propname,
				u32 *out_values,
				int count)
{
	return _config_get_u32_array(modulename, propname, out_values, count);
}

/**
 * config_get_string_array - get string array for specific module and property
 * @modulename:	module name
 * @propname:	property name
 * @out_strs:	output strings array. output strings array is alloc by the caller
 * @count:	the sizeof output array
 *
 * Returns count of array members.
 */
int config_get_string_array(
				const char *modulename,
				const char *propname,
				const char *out_strs[],
				int count)
{
	return _config_get_string_array(modulename, propname, out_strs, count);
}

/**
 * config_init - initialize the data structures
 */
void config_init(void)
{
	__config_init(&config_data, &config_data_end - &config_data);
}

