#include "config_api.h"

#ifndef __CONFIG_PRIVATE__
#define __CONFIG_PRIVATE__


#ifdef __cplusplus
extern "C"
{
#endif

#define CONFIG_MAGIC		(0x12345678)
#define CONFIG_VERSION	(0x10)
#define CONFIG_RESERVE_BLOCK_SIZE	(32)

#define CFG_INIT_BUF_SIZE (1024*8)
#define config_malloc(len) malloc(len)
#define config_free(x) free(x)

#define CONFIG_ALIGN(x, a)		(((x) + (a) - 1) & ~((a) - 1))
#define CONFIG_TAGALIGN_SIZE	(16)
#define CONFIG_TAGALIGN(x)		(CONFIG_ALIGN((x), 16))
#define CONFIG_SIZE_OFFSET		(sizeof(config_offset_t))
#define CONFIG_MAX_PINS		(128)

#if defined(_MSC_VER)
#define strcasecmp _stricmp
#endif

typedef struct config_header {
	int magic;
	int version;
	int totalsize;
	int off_module_idx;
	int off_structs;
	int off_strings;
	int size_module_idx;
	int size_strings;
	int size_structs;
} config_header_t;

#define CONFIG_TYPE_MODULE 0x1
#define CONFIG_TYPE_INT 0x2
#define CONFIG_TYPE_STRING 0x3
#define CONFIG_TYPE_STRING_ARRAY 0x4
#define CONFIG_TYPE_INT_ARRAY 0x5
#define CONFIG_TYPE_GPIO 0x6
#define CONFIG_TYPE_NULL 0x7
#define CONFIG_TYPE_INVALID 0xFF

typedef struct {
	u8 npins;
	u8 func;
	u8 data;
	u8 drv_level;
	u8 pull_updown;
	u8 pull_resisters;
} _blob_gpio_t;

union {
	char string[0];
	int intval[0];
	char raw_data[0];
} prop_data;

typedef struct {
	u16 tag;
	u16 len;
	u16 count;
	u16 name_off;
} _blob_prop_t;

typedef struct module_index {
	config_offset_t offset;
	char name[2];
} blob_mod_idx_t;

#ifdef __cplusplus
}
#endif

#endif
