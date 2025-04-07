/*
 * rotation_sw.h - Rotation sw module head file
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

#ifndef __ROTATION_SW_H__
#define __ROTATION_SW_H__

/*
 * mode:
 * 0: copy from the src to the dst
 * 1: rotate the src imgae by 90 degree
 * 2: rotate the src image by 180 degree
 * 3: rotate the src image by 270 degree
 * 4: horizontal flip the src image
 * 5: horizontal flip and then rotate the src image by 90 degree
 * 6: vertical flip the src image
 * 7: transpose the src image
 */

/**
 * sw_transform - transform image data
 * @src:             the buffer address of source image
 * @src_line_stride: the line stride of source image
 * @dst:             the buffer address of destination image
 * @dst_line_stride: the line stride of destination image
 * @src_width:       the horizontal resolution of source image
 * @src_height:      the vertical resolution of source image
 * @mode:            the transform mode
 * @bytes_per_pixel: the bytes of data for each pixel
 *
 * Return Zero when success, other for failture.
 */
int sw_transform(const unsigned char *src,
		int src_line_stride,
		unsigned char *dst,
		int dst_line_stride,
		int src_width, int src_height,
		int mode, int bytes_per_pixel);

#endif /* __ROTATION_SW_H__ */
