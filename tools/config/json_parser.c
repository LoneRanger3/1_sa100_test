/*
* flaten_prop.c - Standard functionality for the config.bin read/write API.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cJSON.h"
#include "json_parser.h"

#define CJSON_NAME(p) ((p && p->string) ? (p->string) : ("NULL_NAME"))

#define GPIO_NAME_PINS 1
#define GPIO_NAME_FUNC 2
#define GPIO_NAME_DRV 3
#define GPIO_NAME_PUD 4
#define GPIO_NAME_RES 5
#define GPIO_NAME_DATA 6
#define GPIO_NAME_INVALID 7
struct gpio_config_map {
	const char *name;
	int setting;
};
struct gpio_config_map gpio_name_map[] = {
	{"pins", GPIO_NAME_PINS},
	{"pin-function", GPIO_NAME_FUNC},
	{"pin-data", GPIO_NAME_DATA},
	{"pin-drv", GPIO_NAME_DRV},
	{"pin-pud", GPIO_NAME_PUD},
	{"pin-pud-res", GPIO_NAME_RES}
};

typedef struct {
	void *buffer;
	int length;
	int offset;
} rawbuffer_t;

typedef struct {
	cJSON *predef;
	rawbuffer_t cfg_module_idx;
	rawbuffer_t cfg_structs;
	rawbuffer_t cfg_strings;
} config_data_t;

static inline int rawbuffer_alloc(rawbuffer_t *buf, int len)
{
	int retval = 0;

	buf->length = CONFIG_ALIGN(len, sizeof(long long));
	buf->offset = 0;
	buf->buffer = (void *)config_malloc(buf->length);
	if (buf->buffer == NULL) {
		printf("malloc for %d failed\n", buf->length);
		retval = -1;
	}
	return retval;
}

static inline void rawbuffer_free(rawbuffer_t *buf)
{
	buf->length = 0;
	buf->offset = 0;
	if (buf->buffer)
		config_free(buf->buffer);
	buf->buffer = NULL;
	return;
}

int rawbuffer_realloc(rawbuffer_t *buf, int len)
{
	rawbuffer_t newbuffer;
	if (buf == NULL)
		return 0;
	if (len < buf->length)
		return 0;

	rawbuffer_alloc(&newbuffer, len);
	if (newbuffer.buffer == NULL) {
		rawbuffer_free(buf);
		return -1;
	}

	memcpy(newbuffer.buffer, buf->buffer, buf->length);
	newbuffer.offset = buf->offset;
	rawbuffer_free(buf);
	memcpy(buf, &newbuffer, sizeof(rawbuffer_t));
	return 0;
}

static void *_rawbuffer_grab_space(rawbuffer_t *buf, int len)
{
	int retval = 0;
	int offset = buf->offset;
	if (offset + len > buf->length) {
		printf("realloc buffer: buflen:%d, offset:%d, strlen:%d\n",
		buf->length, buf->offset, len);
		retval = rawbuffer_realloc(buf, buf->length
			+ CFG_INIT_BUF_SIZE);
		if (retval < 0)
			return NULL;
	}
	buf->offset += len;
	return (char *)buf->buffer + offset;
}

void printf_parser_err(void)
{
	char *err_string;
	int i = 0;

	err_string = (char *)cJSON_GetErrorPtr();
	while ((*err_string++ != '\0')) {
		if (*err_string == '{')
			i++;
		if (i != 0 && (*err_string == '}')) {
			*(err_string+1) = '\0';
			break;
		}
	}
	if (cJSON_GetErrorPtr())
		printf("parser json failed at: %s\n", cJSON_GetErrorPtr());
}

void dump_rawbuf(rawbuffer_t *buf)
{
	int i;
	int index;
	int *p;
	if (buf == NULL || buf->buffer == NULL)
		return;
	if (buf->length == 0 || buf->offset == 0)
		return;

	p = buf->buffer;
	printf("=== offset:%08d, length:%08d===\n", buf->offset, buf->length);
	for (i = 0; i < buf->offset; i += 16) {
		index = i/4;
		printf("0x%08x:", index);
		printf("0x%08x  0x%08x  0x%08x  0x%08x\n",
		p[index], p[index+1], p[index+2], p[index+3]);
	}
}

void config_del(config_data_t *config)
{
	if (config != NULL) {
		rawbuffer_free(&config->cfg_structs);
		rawbuffer_free(&config->cfg_strings);
		rawbuffer_free(&config->cfg_module_idx);
		config_free(config);
	}
}
config_data_t *config_create(void)
{
	config_data_t *config_data;

	config_data = config_malloc(sizeof(config_data_t));
	if (config_data == NULL) {
		printf("%d, malloc failed!\n", __LINE__);
		return NULL;
	}
	memset(config_data, 0, sizeof(config_data_t));

	rawbuffer_alloc(&config_data->cfg_structs, CFG_INIT_BUF_SIZE);
	rawbuffer_alloc(&config_data->cfg_strings, CFG_INIT_BUF_SIZE);
	rawbuffer_alloc(&config_data->cfg_module_idx, CFG_INIT_BUF_SIZE/4);

	if (config_data->cfg_structs.buffer == NULL
	|| config_data->cfg_strings.buffer == NULL
	|| config_data->cfg_module_idx.buffer == NULL) {
		config_del(config_data);
		config_data = NULL;
		printf("%d, malloc failed!\n", __LINE__);
	}

	return config_data;
}

static char *_buf_find_string(char *strtab, int tabsize, char *s)
{
	int len = strlen(s) + 1;
	char *last = strtab + tabsize - len;
	char *p;

	for (p = strtab; p <= last; p++)
		if (memcmp(p, s, len) == 0)
			return p;
	return NULL;
}

static int _buf_find_add_str(rawbuffer_t *str_buf, char *s)
{
	char *strtab;
	int strtabsize;
	char *p;
	int len;

	/* find at string buffer */
	strtab = (char *)str_buf->buffer;
	strtabsize = str_buf->offset;
	p = _buf_find_string(strtab, strtabsize, s);
	if (p)
		return p - strtab;

	/* Add it */
	len = strlen(s) + 1;
	p = (char *)_rawbuffer_grab_space(str_buf, len);
	if (p == NULL) {
		printf("%s %d failed!\n", __FILE__, __LINE__);
		return -1;
	}
	memcpy(p, s, len);
	return p - strtab;
}


int _buf_add_prop(config_data_t *config, _blob_prop_t *p, void *data)
{
	int offset;
	void *blob;

	blob = _rawbuffer_grab_space(&config->cfg_structs,
	CONFIG_TAGALIGN(sizeof(*p) + p->len));
	if (!blob)
		return -1;

	memcpy(blob, p, sizeof(*p));
	if ((data != NULL) && (p->len != 0))
		memcpy((char *)blob + sizeof(*p), data, p->len);
	offset = (char *)blob - (char *)config->cfg_structs.buffer;

	return offset;
}

static cJSON *_get_predef_item(cJSON *predf, char *name)
{
	cJSON *cur_element = NULL;
	cJSON *out = NULL;

	if ((predf == NULL) || (name == NULL))
		return NULL;

	cur_element = predf->child;
	while ((cur_element != NULL)
		&& (cur_element->child != NULL)
		&& (cur_element->type == cJSON_Object)) {
		if (strcasecmp(name, (cur_element->child->string)) == 0) {
			out = cur_element->child;
			break;
		}
		cur_element = cur_element->next;
	}

	return out;
}

static int _get_item_tag(cJSON *item, cJSON *predef)
{
	int tag = CONFIG_TYPE_INVALID;
	int child_tag = CONFIG_TYPE_INVALID;
	cJSON *pre_def_item = NULL;
	if (item == NULL)
		return CONFIG_TYPE_INVALID;

	switch ((item->type) & 0xFF) {
	case cJSON_NULL:
		tag = CONFIG_TYPE_NULL;
		break;

	case cJSON_False:
	case cJSON_True:
	case cJSON_Number:
		tag = CONFIG_TYPE_INT;
		break;

	case cJSON_String:
		tag = CONFIG_TYPE_STRING;
		if (predef)
			pre_def_item = _get_predef_item(predef, item->valuestring);
		if (pre_def_item)
			tag = _get_item_tag(pre_def_item, NULL);
		break;

	case cJSON_Array:
		if (cJSON_IsArray(item->child))
			return CONFIG_TYPE_INVALID;
		child_tag = _get_item_tag(item->child, predef);
		if (child_tag == CONFIG_TYPE_STRING)
			tag = CONFIG_TYPE_STRING_ARRAY;
		if (child_tag == CONFIG_TYPE_INT)
			tag = CONFIG_TYPE_INT_ARRAY;
		if (child_tag == CONFIG_TYPE_INVALID)
			tag = CONFIG_TYPE_GPIO;
		break;
	case cJSON_Object:
	default:
		break;
	}
	return tag;
}

static int _get_gpio_setting_type(cJSON *item)
{
	int retval = GPIO_NAME_INVALID;
	int i;
	for (i = 0; i < sizeof(gpio_name_map)/sizeof(gpio_name_map[0]); i++) {
		if (strcasecmp(gpio_name_map[i].name, item->string) == 0) {
			retval = gpio_name_map[i].setting;
			break;
		}
	}
	return retval;
}

static int _pin_from_string(char *string, gpio_pin_t *pin)
{
	char *newstring;
	char *tmp;
	int length;

	if (string == NULL)
		return -1;
	tmp = string;
	newstring = string;
	while (*tmp != '\0') {
		if (*tmp >= 'A' && *tmp <= 'Z')
			*newstring++ = *tmp + ('a'-'A');
		if (*tmp != ' ')
			*newstring++ = *tmp;
		tmp++;
	}
	*newstring = '\0';

	length = strlen(string);
	if (length < 5 || length > 6)
		return -1;

	/* pin string format should be gpa-1 or gpa-13:
	* char 0,1,3 must be 'g','p','-'
	* char 2 must be a charactor
	* char 4, 5(if it's exist) must be a number
	*/
	if (string[0] != 'g' || string[1] != 'p' || string[3] != '-')
		return -1;
	if (string[2] < 'a' || string[2] > 'z')
		return -1;
	if (string[4] < '0' || string[4] > '9')
		return -1;
	if (string[5] != '\0' && (string[5] < '0' || string[5] > '9'))
		return -1;
	pin->port = string[2] - 'a';
	pin->pin = string[4] - '0';
	if (string[5] != '\0')
		pin->pin = pin->pin*10 + (string[5] - '0');
	return 0;
}


static int _pins_from_json(cJSON *predef,
		cJSON *item, gpio_pin_t **pin, unsigned char *count)
{
	cJSON *pin_item = NULL;
	cJSON *predef_item = NULL;
	gpio_pin_t *tmp_pin;
	int size;
	int i;
	int retval = 0;
	int tag;

	if (item == NULL || pin == NULL || count == NULL) {
		printf("line:%d, para err!\n", __LINE__);
		return -1;
	}

	tag = _get_item_tag(item, predef);
	if (tag == CONFIG_TYPE_STRING) {
		size = 1;
		pin_item = item;
	}
	if (tag == CONFIG_TYPE_STRING_ARRAY) {
		size = cJSON_GetArraySize(item);
		pin_item = item->child;
	}
	if (pin_item == NULL || size == 0) {
		*pin = 0;
		*count = 0;
		return 0;
	}
	tmp_pin = config_malloc(sizeof(gpio_pin_t)*size);
	if (tmp_pin == NULL)
		return -1;

	for (i = 0; (i < size) && (pin_item); i++) {
		predef_item = _get_predef_item(predef, pin_item->valuestring);
		if (predef_item)
			retval = _pin_from_string(predef_item->valuestring,
				&tmp_pin[i]);
		else
			retval = _pin_from_string(pin_item->valuestring,
				&tmp_pin[i]);
		if (retval < 0) {
			*pin = 0;
			*count = 0;
			config_free(tmp_pin);
			break;
		}
		pin_item = pin_item->next;
	}

	if (retval >= 0) {
		*pin = tmp_pin;
		*count = size;
	}
	return retval;
}


static int _gpio_array_from_json(cJSON *predef,
		cJSON *item, _blob_prop_t *gpioprop, void **data)
{
	cJSON *config_item = NULL;
	int nconfig = 0;
	gpio_pin_t *pins = NULL;
	int configtype;
	int i;
	int retval = 0;
	cJSON *predef_item = NULL;
	_blob_gpio_t gpio;
	unsigned char *pconfig = NULL;
	char *outbuffer = NULL;

	if (item == NULL || gpioprop == NULL || data == NULL) {
		printf("line:%d, para err!\n", __LINE__);
		return -1;
	}
	memset(&gpio, 0xFF, sizeof(gpio));
	*data = 0;

	gpioprop->tag = CONFIG_TYPE_GPIO;
	nconfig = cJSON_GetArraySize(item);
	item = item->child;
	gpio.npins = 0;

	retval = -1;
	for (i = 0; (i < nconfig); i++) {
		if (item == NULL || !cJSON_IsObject(item))
			break;
		config_item = item->child;
		configtype = _get_gpio_setting_type(config_item);
		pconfig = NULL;
		switch (configtype) {
		case GPIO_NAME_PINS:
			retval = _pins_from_json(predef,
			config_item, &pins, &gpio.npins);
			pconfig = NULL;
			break;
		case GPIO_NAME_FUNC:
			pconfig = &gpio.func;
			break;
		case GPIO_NAME_DATA:
			pconfig = &gpio.data;
			break;
		case GPIO_NAME_DRV:
			pconfig = &gpio.drv_level;
			break;
		case GPIO_NAME_PUD:
			pconfig = &gpio.pull_updown;
			break;
		case GPIO_NAME_RES:
			pconfig = &gpio.pull_resisters;
			break;
		default:
			pconfig = NULL;
			retval = -1;
			break;
		}
		if (pconfig != NULL) {
			retval = 0;
			predef_item = _get_predef_item(predef,
			config_item->valuestring);
			if (predef_item)
				*pconfig = predef_item->valueint;
			else if (cJSON_IsNumber(config_item))
				*pconfig = config_item->valueint;
			else {
				printf("unsuport config of %s\n",
						CJSON_NAME(config_item));
				retval = -1;
			}
		}
		item = item->next;
		if (retval < 0) {
			printf("get gpio config: \"%s\" failed!\n",
			CJSON_NAME(config_item));
			break;
		}
	}

	if (gpio.npins == 0) {
		printf("gpio config must define pins\n");
		retval = -1;
	}
	if (retval >= 0) {
		outbuffer = config_malloc(
			CONFIG_ALIGN(sizeof(gpio), sizeof(gpio_pin_t))
			+ gpio.npins*sizeof(gpio_pin_t));
		if (outbuffer == NULL) {
			config_free(pins);
			return -1;
		}
	memcpy(outbuffer, &gpio, sizeof(gpio));
	if (pins != NULL && gpio.npins != 0)
		memcpy(outbuffer +
			CONFIG_ALIGN(sizeof(gpio), sizeof(gpio_pin_t)),
			pins, gpio.npins*sizeof(gpio_pin_t));
		*data = outbuffer;
		gpioprop->count = gpio.npins;
		gpioprop->len = CONFIG_ALIGN(sizeof(gpio), sizeof(gpio_pin_t))
			+ gpio.npins*sizeof(gpio_pin_t);
	}
	if (pins)
		config_free(pins);

	return retval;
}


static int _int_array_from_json(cJSON *predef,
		cJSON *item, _blob_prop_t *arrayprop, void **data)
{
	cJSON *jarray_item = NULL;
	cJSON *tmp_item = NULL;
	int *buffer;
	int value;
	int i;

	if (item == NULL || arrayprop == NULL || data == NULL) {
		printf("line:%d, para err!\n", __LINE__);
		return -1;
	}

	/* get array size*/
	arrayprop->tag = CONFIG_TYPE_INT_ARRAY;
	arrayprop->count = cJSON_GetArraySize(item);
	jarray_item = item->child;
	if (jarray_item == NULL || arrayprop->count == 0) {
		arrayprop->len = 0;
		arrayprop->count = 0;
		return 0;
	}
	arrayprop->len = arrayprop->count*sizeof(int);

	buffer = config_malloc(arrayprop->len);
	if (buffer == NULL)
		return -1;

	for (i = 0; (i < arrayprop->count) && (jarray_item); i++) {
		if (cJSON_IsString(jarray_item)) {
			tmp_item = _get_predef_item(predef,
			jarray_item->valuestring);
			if (tmp_item == NULL) {
				printf("err! find string in intarray (%s)!\n",
					CJSON_NAME(item));
				config_free(buffer);
				*data = 0;
				return -2;
			}
			value = tmp_item->valueint;
		} else {
			value = jarray_item->valueint;
		}
		buffer[i] = value;
		jarray_item = jarray_item->next;
	}
	*data = (void *)buffer;
	return 0;
}

static int _string_array_from_json(config_data_t *config,
		cJSON *item, _blob_prop_t *arrayprop, void **data)
{
	cJSON *jarray_item = NULL;
	cJSON *tmp_item = NULL;
	void *buffer;
	int value;
	int i;

	if (item == NULL || arrayprop == NULL || data == NULL) {
		printf("line:%d, para err!\n", __LINE__);
		return -1;
	}

	arrayprop->tag = CONFIG_TYPE_STRING_ARRAY;
	arrayprop->count = cJSON_GetArraySize(item);
	jarray_item = item->child;
	if (jarray_item == NULL || arrayprop->count == 0) {
		arrayprop->len = 0;
		arrayprop->count = 0;
		return 0;
	}
	arrayprop->len = arrayprop->count * CONFIG_SIZE_OFFSET;

	buffer = config_malloc(arrayprop->len);
	if (buffer == NULL)
		return -1;

	for (i = 0; (i < arrayprop->count) && (jarray_item); i++) {
		tmp_item = _get_predef_item(config->predef,
				jarray_item->valuestring);
		if (tmp_item == NULL)
			tmp_item = jarray_item;
		if (!cJSON_IsString(tmp_item)) {
			printf("error! find a none-string in array (%s)!\n",
				CJSON_NAME(item));
			config_free(buffer);
			*data = 0;
			return -2;
		}
		value = _buf_find_add_str(
		&config->cfg_strings, tmp_item->valuestring);
		if (value < 0) {
			config_free(buffer);
			*data = 0;
			return -1;
		}
		memcpy((char *)buffer + i*CONFIG_SIZE_OFFSET,
			&value, CONFIG_SIZE_OFFSET);
		jarray_item = jarray_item->next;
	}
	*data = buffer;
	return 0;
}

/**
 * config_save_module - save prop to buffer
 * @config:	config buffers
 * @module:	json items contains module configs.
 *
 * Returns 0 on success, -EERROR otherwise..
 */
static int config_save_prop(config_data_t *config, cJSON *item)
{
	int offset = 0;
	int retval = 0;
	_blob_prop_t prop;
	void *data;
	void *private_data = NULL;
	cJSON *predef_item = NULL;

	memset(&prop, 0 , sizeof(prop));
	prop.tag = _get_item_tag(item, config->predef);
	if (prop.tag == CONFIG_TYPE_INVALID) {
		printf("property %s has invalid type:%d\n",
		CJSON_NAME(item), prop.tag);
		return -1;
	}

	/* property has not a name , just return*/
	if (item->string == NULL) {
		printf("find a orphan property and will be ignored!");
		return -1;
	}
	offset = _buf_find_add_str(&config->cfg_strings, item->string);
	if (offset < 0)
		return -1;
	prop.name_off = offset;

	switch (prop.tag) {
	case CONFIG_TYPE_INT:
		prop.count = 1;
		prop.len = sizeof(item->valueint);
		predef_item = _get_predef_item(config->predef,
				item->valuestring);
		if (predef_item)
			data = &predef_item->valueint;
		else
			data = &item->valueint;
		break;

	case CONFIG_TYPE_STRING:
		prop.count = 1;
		prop.len = CONFIG_SIZE_OFFSET;
		predef_item = _get_predef_item(config->predef,
					item->valuestring);
		if (predef_item)
			offset = _buf_find_add_str(&config->cfg_strings,
			predef_item->valuestring);
		else
			offset = _buf_find_add_str(&config->cfg_strings,
			item->valuestring);
		if (offset < 0)
			return offset;
		data = &offset;
		break;

	case CONFIG_TYPE_INT_ARRAY:
		retval = _int_array_from_json(config->predef,
				item, &prop, &data);
		private_data = data;
		break;

	case CONFIG_TYPE_STRING_ARRAY:
		retval = _string_array_from_json(config,
				item, &prop, &data);
		private_data = data;
		break;

	case CONFIG_TYPE_GPIO:
		retval = _gpio_array_from_json(config->predef,
				item, &prop, &data);
		private_data = data;
		break;

	case CONFIG_TYPE_INVALID:
	default:
		prop.tag = CONFIG_TYPE_NULL;
		prop.count = 0;
		prop.len = 0;
		data = NULL;
		printf("property %s has invalid type:%d\n",
			CJSON_NAME(item), item->type);
		break;
	}
	if (retval >= 0)
		retval = _buf_add_prop(config, &prop, data);
	if (private_data != NULL)
		config_free(private_data);
	return retval;
}

/**
 * config_save_module - save module to buffer
 * @config:	config buffers
 * @module:	json items contains module configs.
 *
 * Returns 0 on success, -EERROR otherwise..
 */
static int config_save_module(config_data_t *config, cJSON *module)
{
	_blob_prop_t mheader;
	int mstart;
	cJSON *prop = NULL;
	int size = 0;
	int retval = 0;
	int nameoff = 0;
	blob_mod_idx_t *module_index;

	if (!module->child) {
		printf("warning: module %s will be ignored!\n",
		CJSON_NAME(module));
		return 0;
	}
	if (!cJSON_IsArray(module)) {
		printf("module %s is not a array!\n", CJSON_NAME(module));
		return -1;
	}

	size = cJSON_GetArraySize(module);
	mheader.count = size;
	mheader.len = CONFIG_SIZE_OFFSET;
	mheader.tag = CONFIG_TYPE_MODULE;
	nameoff = _buf_find_add_str(&config->cfg_strings, module->string);
	if (nameoff < 0)
		return nameoff;
	mheader.name_off = nameoff;

	mstart = _buf_add_prop(config, &mheader, &size);
	if (mstart < 0)
		return mstart;

	prop = module->child;
	while ((size > 0) && (prop != NULL) && (retval >= 0)) {
		if (!cJSON_IsObject(prop)) {
			printf("module %s has a invalid prop!\n",
				CJSON_NAME(module));
			retval = -1;
			break;
		}
		retval = config_save_prop(config, prop->child);
		if (retval < 0)
			printf("config_save_prop %s failed!\n",
				CJSON_NAME(prop->child));
		prop = prop->next;
	}
	if (retval >= 0) {
		module_index = _rawbuffer_grab_space(&config->cfg_module_idx,
		sizeof(blob_mod_idx_t));
		module_index->offset = mstart;
		module_index->name[0] = module->string[0];
		module_index->name[1] = module->string[1];
	}
	return retval;
}

/**
 * config_to_blob - save all data in the blob buffer
 * @config:	config buffers
 * @output:	output buffer, malloc in the func.
 * @length:	output buffer length
 *
 * Returns 0 on success, -EERROR otherwise..
 */
static int config_to_blob(config_data_t *config, void **output, int *length)
{
	void *blob;
	config_header_t header;

	int tmpsize;
	int offset;
	if (config == NULL || output == NULL || length == NULL)
		return -1;
	*output = NULL;
	*length = 0;
	header.magic = CONFIG_MAGIC;
	header.version = CONFIG_VERSION;
	offset = CONFIG_TAGALIGN(sizeof(config_header_t));
	header.off_module_idx = offset;

	tmpsize = config->cfg_module_idx.offset;
	header.size_module_idx = tmpsize;
	offset = tmpsize + header.off_module_idx + CONFIG_RESERVE_BLOCK_SIZE;
	offset = CONFIG_TAGALIGN(offset);
	header.off_structs = offset;

	tmpsize = config->cfg_structs.offset;
	header.size_structs = tmpsize;
	offset = tmpsize + header.off_structs + CONFIG_RESERVE_BLOCK_SIZE;
	offset = CONFIG_TAGALIGN(offset);
	header.off_strings = offset;

	tmpsize = config->cfg_strings.offset;
	header.size_strings = tmpsize;
	offset = tmpsize + header.off_strings + CONFIG_RESERVE_BLOCK_SIZE;
	offset = CONFIG_TAGALIGN(offset);
	header.totalsize = offset;
	if (header.off_strings <= 0
			|| header.size_structs <= 0
			|| header.size_module_idx <= 0)
		return -1;

	blob = config_malloc(header.totalsize);
	if (blob == NULL) {
		printf("malloc for %d failed\n", header.totalsize);
		return -1;
	}
	*output = blob;
	*length = header.totalsize;

	memcpy(blob, &header, sizeof(header));

	memcpy((char *)blob + header.off_module_idx,
	config->cfg_module_idx.buffer, config->cfg_module_idx.offset);

	memcpy((char *)blob + header.off_structs,
	config->cfg_structs.buffer, config->cfg_structs.offset);

	memcpy((char *)blob + header.off_strings,
	config->cfg_strings.buffer, config->cfg_strings.offset);

	return 0;
}


/**
 * json_to_blob - transform json strings to blob
 * @content:	json strings
 * @blob:	output buffer, malloc in the func. must free by user before quit
 * @length:	output buffer length
 *
 * Returns 0 on success, -EERROR otherwise..
 */
int json_to_blob(char *content, void **blob, int *length)
{
	cJSON *tree = NULL;
	cJSON *item = NULL;
	config_data_t *config;
	int retval = 0;

	if (content == NULL || blob == NULL || length == NULL)
		return -1;
	config = config_create();
	if (config == NULL)
		return -1;

	/* read and parse test */
	cJSON_Minify(content);
	tree = cJSON_Parse(content);
	if (tree == NULL) {
		printf_parser_err();
		config_del(config);
		return -1;
	}

	config->predef = cJSON_GetObjectItemCaseSensitive(tree, "pre-define");
	item = tree->child;
	while ((item != NULL) && (retval >= 0)) {
		retval = config_save_module(config, item);
		if (retval < 0)
			printf("get module %s config failed!\n",
				CJSON_NAME(item));
		item = item->next;
	}
	if (retval >= 0)
		retval = config_to_blob(config, blob, length);

	if (tree != NULL)
		cJSON_Delete(tree);
	config_del(config);
	return retval;
}


