#define DBG_LEVEL         DBG_INFO

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <dirent.h>
#include <string.h>
#include <oscl.h>
#include "media_extractor.h"

int extractor_test(int argc, char *argv[])
{
	int out_file;
	void *extractor = NULL;
	media_format_t *mformat = NULL;
	unsigned char *buffer;
	int buffer_size;
	int ret;

	OSCL_LOGI("infile %s, outfile %s\n", argv[1], argv[2]);
	out_file = open(argv[2], O_WRONLY | O_CREAT);
	if (out_file < 0) {
		OSCL_LOGE("open file error\n");
		return -1;
	}

	extractor = mextractor_open(argv[1]);
	if (extractor == NULL) {
		OSCL_LOGE("open mextractor error\n");
		goto err1;
	}

	mformat = mextractor_get_format(extractor);
	if (mformat == NULL) {
		OSCL_LOGE("open mformat error\n");
		goto err2;
	}
	OSCL_LOGI("format width %d, height %d\n", mformat->width, mformat->height);
	buffer_size = mformat->width * mformat->height * 3 / 2;
	buffer = oscl_malloc(buffer_size);
	if (!buffer) {
		OSCL_LOGE("malloc buffer error\n");
		goto err2;
	}

	ret = mextractor_get_thumbnail(extractor, buffer);
	if (!ret)
		write(out_file, buffer, buffer_size);
	oscl_free(buffer);

err2:
	mextractor_close(extractor);
err1:
	close(out_file);
	return 0;
}

MSH_CMD_EXPORT(extractor_test, "extractor_test");

