
#ifndef __CONFIG_API__
#define __CONFIG_API__
#include "soc_define.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef config_offset_t
typedef unsigned short config_offset_t;
#endif

extern char config_data, config_data_end;

typedef struct {
	u16 port;
	u16 pin;
} gpio_pin_t;

typedef struct {
	u8 npins;
	u8 func;
	u8 data;
	u8 drv_level;
	u8 pull_updown;
	u8 pull_resisters;
	const gpio_pin_t *pins;
} config_gpio_t;

/**
 * config_init - initialize the data structures
 */
void config_init(void);

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
				unsigned int *out_value);

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
				const char **out_string);

/**
 * config_count_elems - get count of elems
 * @modulename:	module name
 * @propname:	property name
 *
 * Returns count of elems of the specific property
 */
int config_count_elems(const char *modulename,
				const char *propname);

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
				config_gpio_t *gpio);

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
				int count);

/**
 * config_get_string_array - get string array for specific module and property
 * @modulename:	module name
 * @propname:	property name
 * @out_strs:	output strings array. output strings array is alloc by the caller
 * @count:	the sizeof output array
 *
 * Returns count of array members.
 */
int config_get_string_array(const char *modulename,
				const char *propname,
				const char **out_strs,
				int count);

#ifdef __cplusplus
}
#endif

#endif
