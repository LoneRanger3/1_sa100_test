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


char *read_jsonfile(const char *filename)
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

int  main(int argc, char *argv[])
{
	char *content;
	void *blob = NULL;
	int retval;
	int length;
	FILE *file = NULL;
	int write_len;

	if (argc != 3) {
		printf("input param err! shoud be jsont2blob json blob\n");
		return -1;
	}

	/* read json file */
	content = read_jsonfile(argv[1]);
	retval = json_to_blob(content, &blob, &length);
	if (retval < 0 || blob == NULL || length == 0) {
		retval = -1;
		printf("%s %d open file failed!\n", __FILE__, __LINE__);
		goto quit;
	}

	/* open in read binary mode */
	file = fopen(argv[2], "wb+");
	if (file == NULL) {
		printf("%s %d: open blob file %s failed!\n",
			__FILE__, __LINE__, argv[2]);
		retval = -1;
		goto quit;
	}
	write_len = fwrite(blob, sizeof(char), (size_t)length, file);
	if ((long)write_len != length) {
		retval = -1;
		printf("%s %d: write blob failed!\n", __FILE__, __LINE__);
	}

quit:
	if (file != NULL)
		fclose(file);
	if (content != NULL)
		config_free(content);
	if (blob != NULL)
		config_free(blob);
	return;
}


