/*
 * rotation_neon.h - Rotation neon module head file
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

#ifndef __ROTATION_NEON_H__
#define __ROTATION_NEON_H__
void transpose_8bit_8row_neon(const unsigned char *src,
			      int src_line_stride,
			      unsigned char *dst,
			      int dst_line_stride,
			      int src_width);

void transpose_16bit_8row_neon(const unsigned char *src,
			       int src_line_stride,
			       unsigned char *dst,
			       int dst_line_stride,
			       int src_width);

void transpose_32bit_4row_neon(const unsigned char *src,
			       int src_line_stride,
			       unsigned char *dst,
			       int dst_line_stride,
			       int src_width);

void mirror_8bit_1row_neon(const unsigned char *src, unsigned char *dst, int width);
void mirror_16bit_1row_neon(const unsigned char *src, unsigned char *dst, int width);
void mirror_32bit_1row_neon(const unsigned char *src, unsigned char *dst, int width);

void copy_neon(const unsigned char *src, unsigned char *dst, int bytes);
#endif /* __ROTATION_NEON_H__ */
