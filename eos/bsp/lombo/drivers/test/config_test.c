/*
 * config_test.c - Standard functionality for the config.bin read/write API.
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
#include "json_parser.h"
#define DBG_LEVEL	0
#include <debug.h>
#include <rtthread.h>

#ifdef _CJSON_TEST
#include "cJSON.h"
#endif

#define printf(...)		 do { dbg_raw(__VA_ARGS__) } while (0)

#define UNITY_TEST_ASSERT(condition, line, format...) \
do { \
	if (!(condition) && (unity.line_failed == 0)) {\
		printf(format); \
		printf("\n"); \
		printf("======%s:%d:%s:failed at %d\n", unity.file, \
			unity.cur_test_line,\
			unity.name, line); \
		unity.line_failed = line; \
		return; \
	} \
} while (0)

#define TEST_ASSERT(condition, format...) \
do {int line = __LINE__; \
		UNITY_TEST_ASSERT((condition), line, format); \
} while (0)

#define TEST_ASSERT_ON(condition) do { int line = __LINE__; \
	UNITY_TEST_ASSERT((condition), line,\
	" Expression Evaluated To FALSE"); } while (0)

#define TEST_ASSERT_EQUAL(expected, actual) do { int line = __LINE__; \
		UNITY_TEST_ASSERT((expected == actual), line,\
		"expected:%d, actual:%d\n", expected, actual); \
	} while (0)

#define test_run(func, func_line) \
do { \
	unity.name = #func; \
	unity.cur_test_line = func_line; \
	unity.ntests++; \
	unity.line_failed = 0; \
	func(); \
	if (!unity.line_failed) \
		printf("====%s:%d:%s:pass\n", unity.file, \
			unity.cur_test_line, unity.name); \
	else \
		unity.nfails++; \
} while (0)

struct UNITY_STORAGE_T {
	const char *file;
	const char *name;
	int ntests;
	int cur_test_line;
	int nfails;
	int line_failed;
};
struct UNITY_STORAGE_T unity;

#define test_init(void) \
do { \
	unity.file = __FILE__; \
	unity.name = NULL; \
	unity.cur_test_line = 0; \
	unity.ntests = 0; \
	unity.nfails = 0; \
	unity.line_failed = 0; \
} while (0)

#define test_output(void) \
do {\
	printf("================\n");\
	printf("number of tests:%d ----\n", unity.ntests);\
	printf("nfails:%d\n", unity.nfails);\
	if (unity.nfails == 0U)\
		printf("=======ok=========\n");\
	else\
		printf("=======fail=========\n");\
	return 0;\
} while (0)


#ifdef _CJSON_TEST
char *read_file(const char *filename)
{
	FILE *file = NULL;
	long length = 0;
	char *content = NULL;
	size_t read_chars = 0;

	/* open in read binary mode */
	file = fopen(filename, "rb");
	if (file == NULL)
		goto cleanup;

	/* get the length */
	if (fseek(file, 0, SEEK_END) != 0)
		goto cleanup;
	length = ftell(file);

	if (length < 0)
		goto cleanup;

	if (fseek(file, 0, SEEK_SET) != 0)
		goto cleanup;

	/* allocate content buffer */
	content = (char *)malloc((size_t)length + sizeof(""));
	if (content == NULL)
		goto cleanup;

	/* read the file into memory */
	read_chars = fread(content, sizeof(char), (size_t)length, file);
	if ((long)read_chars != length) {
		config_free(content);
		content = NULL;
		goto cleanup;
	}
	content[read_chars] = '\0';

cleanup:
	if (file != NULL)
		fclose(file);

	return content;
}

struct json_suit {
	char *json;
	int result;
};
struct json_suit test_json[] = {
	{"{}", -1},
	{"{uart\":3}", -1},
	{"{\"uart\":3}", -1},
	{"{\"uart\":[1,2,3]}", -1},
	{"{\"module\":[{\"zero\":0}]}", 0},
	{"{\"module\":[{\"valhex\":0x1234}]}", 0},
	{"{\"module\":[{\"negative\":-1}]}", 0},
	{"{\"module\":[{\"negative overflow\":-0x80000000}]}", -1},
	{"{\"module\":[{\"positive overflow\":0x100000000}]}", -1},
	{"{\"module\":[{\"config_string\":\"para string\"}]}", 0},
	{"{\"module\":[{\"config_gpio\":[{\"pin-pud\":1}]}]}", -1},
	{"{\"module\":[{\"config_gpio\":[{\"pins\":\"gpa-1\"},{\"pin-pud\":null}]}]}",
		-1},
	{"{\"module\":[{\"config_gpio\":[{\"pins\":\"gpa-1\"}]}]}", 0},
	{"{\"module\":[{\"config_gpio\":[{\"pins\":\"gpa-1\"},{\"pin-pud\":1}]}]}", 0},
	{"{\"module\":[{\"config_gpio\":[{\"pin-pud\":1},{\"pins\":\"gpa-1\"}]}]}", 0},
};

int test_json2blob(void)
{
	char *content;
	void *blob = NULL;
	int length;
	int i;
	int retval;
	int failed = 0;
	int succeed = 0;

	for (i = 0; i < sizeof(test_json)/sizeof(test_json[0]); i++) {
		content = config_malloc(strlen(test_json[i].json) + 1);
		strcpy(content, test_json[i].json);
		retval = json_to_blob(content, &blob, &length);
		printf("test_json[i].json:%s\n", test_json[i].json);
		TEST_ASSERT_EQUAL(test_json[i].result, retval);
		config_free(content);
	}

	if (blob != NULL)
		config_free(blob);

	return 0;
}
#endif


void test_config_u32(void)
{
	unsigned int value_u32;
	int retval;
	int count;

	retval = config_get_u32("test_int", "zero", &value_u32);
	TEST_ASSERT(retval == 0, "get test_int zero failed");
	TEST_ASSERT_EQUAL(0, value_u32);
	count = config_count_elems("test_int", "zero");
	TEST_ASSERT_EQUAL(1, count);

	retval = config_get_u32("test_int", "valhex", &value_u32);
	TEST_ASSERT(retval == 0, "get test_int valhex failed");
	TEST_ASSERT_EQUAL(0xFFFFFFFF, value_u32);
	count = config_count_elems("test_int", "valhex");
	TEST_ASSERT_EQUAL(1, count);

	retval = config_get_u32("test_int", "negative", &value_u32);
	TEST_ASSERT(retval == 0, "get test_int negative failed");
	TEST_ASSERT_EQUAL(-0x7FFFFFFF, value_u32);
	count = config_count_elems("test_int", "negative");
	TEST_ASSERT_EQUAL(1, count);

	retval = config_get_u32("test_int", "positive", &value_u32);
	TEST_ASSERT(retval == 0, "get test_int positive failed");
	TEST_ASSERT_EQUAL(1234, value_u32);
	count = config_count_elems("test_int", "positive");
	TEST_ASSERT_EQUAL(1, count);

	retval = config_get_u32("test_int", "predef", &value_u32);
	TEST_ASSERT(retval == 0, "get test_int predef failed");
	TEST_ASSERT_EQUAL(1, value_u32);
	count = config_count_elems("test_int", "predef");
	TEST_ASSERT_EQUAL(1, count);
}

void test_config_u32array(void)
{
	unsigned int value_array[5];
	int retval;
	int count;

	memset(value_array, 0, sizeof(value_array));
	retval = config_get_u32_array("test_intarray", "one member", value_array, 5);
	TEST_ASSERT(retval == 1, "get test_intarray one member failed");
	TEST_ASSERT_EQUAL(1, value_array[0]);
	TEST_ASSERT_EQUAL(0, value_array[1]);
	count = config_count_elems("test_intarray", "one member");
	TEST_ASSERT_EQUAL(1, count);

	memset(value_array, 0, sizeof(value_array));
	retval = config_get_u32_array("test_intarray", "ten member", value_array, 4);
	TEST_ASSERT(retval == 4, "get test_intarray ten member failed");
	TEST_ASSERT_EQUAL(-1, value_array[0]);
	TEST_ASSERT_EQUAL(0, value_array[1]);
	TEST_ASSERT_EQUAL(1, value_array[2]);
	TEST_ASSERT_EQUAL(2, value_array[3]);
	TEST_ASSERT_EQUAL(0, value_array[4]);
	count = config_count_elems("test_intarray", "ten member");
	TEST_ASSERT_EQUAL(10, count);

}



void test_config_gpio(void)
{
	int retval;
	config_gpio_t gpio;
	int count;

	retval = config_get_gpio("test_gpio", "onepin", &gpio);
	TEST_ASSERT(retval == 0, "get test_int onepin failed");
	TEST_ASSERT_EQUAL(1, gpio.npins);
	TEST_ASSERT_EQUAL(3, gpio.func);
	TEST_ASSERT_EQUAL(0, gpio.data);
	TEST_ASSERT_EQUAL(1, gpio.drv_level);
	TEST_ASSERT_EQUAL(1, gpio.pull_updown);
	TEST_ASSERT_EQUAL(0, gpio.pull_resisters);
	TEST_ASSERT_EQUAL(0, gpio.pins[0].port);
	TEST_ASSERT_EQUAL(6, gpio.pins[0].pin);
	count = config_count_elems("test_gpio", "onepin");
	TEST_ASSERT_EQUAL(1, count);


	retval = config_get_gpio("test_gpio", "twopin", &gpio);
	TEST_ASSERT(retval == 0, "get test_int twopin failed");
	TEST_ASSERT_EQUAL(2, gpio.npins);
	TEST_ASSERT_EQUAL(1, gpio.func);
	TEST_ASSERT_EQUAL(1, gpio.data);
	TEST_ASSERT_EQUAL(1, gpio.drv_level);
	TEST_ASSERT_EQUAL(1, gpio.pull_updown);
	TEST_ASSERT_EQUAL(0, gpio.pull_resisters);
	TEST_ASSERT_EQUAL(0, gpio.pins[0].port);
	TEST_ASSERT_EQUAL(6, gpio.pins[0].pin);
	count = config_count_elems("test_gpio", "twopin");
	TEST_ASSERT_EQUAL(2, count);

	retval = config_get_gpio("test_gpio", "onlypin", &gpio);
	TEST_ASSERT(retval == 0, "get test_int onlypin failed");
	TEST_ASSERT_EQUAL(1, gpio.npins);
	TEST_ASSERT_EQUAL(0xFF, gpio.func);
	TEST_ASSERT_EQUAL(0xFF, gpio.data);
	TEST_ASSERT_EQUAL(0xFF, gpio.drv_level);
	TEST_ASSERT_EQUAL(0xFF, gpio.pull_updown);
	TEST_ASSERT_EQUAL(0xFF, gpio.pull_resisters);
	TEST_ASSERT_EQUAL(1, gpio.pins[0].port);
	TEST_ASSERT_EQUAL(1, gpio.pins[0].pin);
	count = config_count_elems("test_gpio", "onlypin");
	TEST_ASSERT_EQUAL(1, count);
}

void test_config_string(void)
{
	const char *value_string;
	int retval;
	int count;

	retval = config_get_string("test_string", "null", &value_string);
	TEST_ASSERT(retval < 0, "get test_int zero failed");
	count = config_count_elems("test_string", "NULL");
	TEST_ASSERT_EQUAL(-1, count);

	retval = config_get_string("test_string", "char", &value_string);
	TEST_ASSERT(retval == 0, "get test_int zero failed");
	TEST_ASSERT_EQUAL(0, strcmp(value_string, "a"));
	count = config_count_elems("test_string", "char");
	TEST_ASSERT_EQUAL(1, count);

	retval = config_get_string("test_string", "predef", &value_string);
	TEST_ASSERT(retval == 0, "get test_int zero failed");
	TEST_ASSERT_EQUAL(0, strcmp(value_string, "predef test string"));
	count = config_count_elems("test_string", "predef");
	TEST_ASSERT_EQUAL(1, count);

	retval = config_get_string("test_string", "special", &value_string);
	TEST_ASSERT(retval == 0, "get test_string special failed");
	TEST_ASSERT_EQUAL(0, strcmp(value_string,
		"~!@#$%^&*()`-={}[];'\\:\"|<>?,./"));
	count = config_count_elems("test_string", "special");
	TEST_ASSERT_EQUAL(1, count);

}

void test_config_stringarray(void)
{
	const char *value_string[5];
	int retval;
	int count;

	retval = config_get_string_array("test_stringarray", "null",
		&value_string[0], 5);
	TEST_ASSERT(retval == 0, "get test_stringarray null failed");
	count = config_count_elems("test_stringarray", "null");
	TEST_ASSERT_EQUAL(-1, count);

	memset(value_string, 0, sizeof(value_string));
	retval = config_get_string_array("test_stringarray", "one",
		&value_string[0], 5);
	TEST_ASSERT(retval == 1, "get test_stringarray one failed");
	TEST_ASSERT(0 == value_string[1], "get test_stringarray one failed");
	TEST_ASSERT_EQUAL(0, strcmp(value_string[0], "abc"));
	count = config_count_elems("test_stringarray", "one");
	TEST_ASSERT_EQUAL(1, count);

	memset(value_string, 0, sizeof(value_string));
	retval = config_get_string_array("test_stringarray", "multi",
		&value_string[0], 5);
	TEST_ASSERT(retval == 4, "get test_stringarray multi failed");
	TEST_ASSERT(0 == value_string[4], "get test_stringarray one failed");
	TEST_ASSERT_EQUAL(0, strcmp(value_string[0], "a"));
	TEST_ASSERT_EQUAL(0, strcmp(value_string[1], "predef test string"));
	TEST_ASSERT_EQUAL(0, strcmp(value_string[2], "abc"));
	TEST_ASSERT_EQUAL(0, strcmp(value_string[3], "bcd"));

	count = config_count_elems("test_stringarray", "multi");
	TEST_ASSERT_EQUAL(4, count);

}


#ifdef _CJSON_TEST
int main(int argc, char *argv[])
{
	char *content;
	void *blob = NULL;
	int retval;
	int length;

	if (argc >= 2)
		content = read_file(argv[1]);
	else
		content = read_file("d://test.json");
	retval = json_to_blob(content, &blob, &length);
	if (retval < 0 || blob == NULL || length == 0) {

		printf("open file failed!\n");
		config_free(content);
		return 0;
	}
	config_init(blob, length);

	test_init();
	test_run(test_json2blob, __LINE__);
	test_run(test_config_u32, __LINE__);
	test_run(test_config_gpio, __LINE__);
	test_run(test_config_string, __LINE__);
	test_run(test_config_stringarray, __LINE__);

	test_output();

	if (blob != NULL)
		config_free(blob);

	return;
}
#else
long test_config(int argc, char **argv)
{
	test_init();
	test_run(test_config_u32, __LINE__);
	test_run(test_config_u32array, __LINE__);
	test_run(test_config_gpio, __LINE__);
	test_run(test_config_string, __LINE__);
	test_run(test_config_stringarray, __LINE__);
	test_output();
}
#endif

