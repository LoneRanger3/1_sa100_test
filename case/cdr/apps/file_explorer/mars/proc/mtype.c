/*
 * mtype.c - media type code for LomboTech
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

#include <mars/proc/mtype.h>

static mformat_t fmt_table[] = {
	{ ".TXT", MEDIA_EBOOK },
	{ ".INI", MEDIA_EBOOK },
	{ ".IRC", MEDIA_EBOOK },
	{ ".SRT", MEDIA_EBOOK },
	{ ".SSA", MEDIA_EBOOK },
	{ ".ASS", MEDIA_EBOOK },

	{ ".AVI", MEDIA_VIDEO },
	{ ".DIVX", MEDIA_VIDEO },
	{ ".PMP", MEDIA_VIDEO },
	{ ".3GP", MEDIA_VIDEO },
	{ ".MP4", MEDIA_VIDEO },
	{ ".M4V", MEDIA_VIDEO },
	{ ".F4V", MEDIA_VIDEO },
	{ ".MPG", MEDIA_VIDEO },
	{ ".MPEG", MEDIA_VIDEO },
	{ ".VOB", MEDIA_VIDEO },
	{ ".DAT", MEDIA_VIDEO },
	{ ".RM", MEDIA_VIDEO },
	{ ".RAM", MEDIA_VIDEO },
	{ ".RMVB", MEDIA_VIDEO },
	{ ".FLV", MEDIA_VIDEO },
	{ ".WMV", MEDIA_VIDEO },
	{ ".ASF", MEDIA_VIDEO },
	{ ".MKV", MEDIA_VIDEO },
	{ ".PSR", MEDIA_VIDEO },
	{ ".TP", MEDIA_VIDEO },
	{ ".TRP", MEDIA_VIDEO },
	{ ".TS", MEDIA_VIDEO },
	{ ".M2TS", MEDIA_VIDEO },
	{ ".MOV", MEDIA_VIDEO },

	{ ".MP3", MEDIA_AUDIO },
	{ ".MP2", MEDIA_AUDIO },
	{ ".MP1", MEDIA_AUDIO },
	{ ".WMA", MEDIA_AUDIO },
	{ ".OGG", MEDIA_AUDIO },
	{ ".APE", MEDIA_AUDIO },
	{ ".FLAC", MEDIA_AUDIO },
	{ ".AC3", MEDIA_AUDIO },
	{ ".DTS", MEDIA_AUDIO },
	{ ".RA", MEDIA_AUDIO },
	{ ".AAC", MEDIA_AUDIO },
	{ ".AMR", MEDIA_AUDIO },
	{ ".ATRC", MEDIA_AUDIO },
	{ ".MID", MEDIA_AUDIO },
	{ ".WAV", MEDIA_AUDIO },

	{ ".JFIF", MEDIA_PIC },
	{ ".JPEG", MEDIA_PIC },
	{ ".PNG", MEDIA_PIC },
	{ ".BMP", MEDIA_PIC },
	{ ".JPG", MEDIA_PIC },
	{ ".GIF", MEDIA_PIC },

	{ NULL, MEDIA_UNKNOWN }
};

lb_int32 suffix_cmp(const char *s1, const char *s2)
{
	lb_uint8  x2 = 0;
	lb_uint8  x1 = 0;

	while (*s1 && *s2) {
		x1 = *s1;
		x2 = *s2;

		if (x1 < 122 && x1 > 97)
			x1 -= 32;
		if (x2 < 122 && x2 > 97)
			x2 -= 32;

		if (x2 != x1)
			break;
		s1++;
		s2++;
	}

	return x1 - x2;
}

media_type_t suffix_to_mtype(const char *path)
{
	char *suffix;
	mformat_t *mfmt;

	if (NULL == path)
		return MEDIA_UNKNOWN;

	/* seek to extended name */
	suffix = strrchr(path, '.');
	if (NULL == suffix)
		return MEDIA_UNKNOWN;

	/* look in ebook valid format table */
	mfmt = fmt_table;
	while (mfmt->suffix) {
		if (suffix_cmp(suffix, mfmt->suffix) == 0)
			return mfmt->mtype;

		mfmt++;
	}

	return MEDIA_UNKNOWN;
}
