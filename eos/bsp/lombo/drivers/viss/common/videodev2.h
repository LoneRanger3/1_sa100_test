/*
 * videodev2.h - video device common head file
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

#ifndef __VIDEODEV2_H__
#define __VIDEODEV2_H__

/*  Four-character-code (FOURCC) */
#define viss_fourcc(a, b, c, d)\
	((rt_uint32_t)(a) | ((rt_uint32_t)(b) << 8) |      \
	((rt_uint32_t)(c) << 16) | ((rt_uint32_t)(d) << 24))

/*      Pixel format         FOURCC                          depth  Description  */

/* RGB formats */
#define VISS_PIX_FMT_RGB332  viss_fourcc('R', 'G', 'B', '1') /*  8  RGB-3-3-2     */
#define VISS_PIX_FMT_RGB444  viss_fourcc('R', '4', '4', '4') /* 16  xxxxrrrr ggggbbbb */
#define VISS_PIX_FMT_RGB555  viss_fourcc('R', 'G', 'B', 'O') /* 16  RGB-5-5-5     */
#define VISS_PIX_FMT_RGB565  viss_fourcc('R', 'G', 'B', 'P') /* 16  RGB-5-6-5     */
#define VISS_PIX_FMT_RGB555X viss_fourcc('R', 'G', 'B', 'Q') /* 16  RGB-5-5-5 BE  */
#define VISS_PIX_FMT_RGB565X viss_fourcc('R', 'G', 'B', 'R') /* 16  RGB-5-6-5 BE  */
#define VISS_PIX_FMT_BGR666  viss_fourcc('B', 'G', 'R', 'H') /* 18  BGR-6-6-6	  */
#define VISS_PIX_FMT_BGR24   viss_fourcc('B', 'G', 'R', '3') /* 24  BGR-8-8-8     */
#define VISS_PIX_FMT_RGB24   viss_fourcc('R', 'G', 'B', '3') /* 24  RGB-8-8-8     */
#define VISS_PIX_FMT_BGR32   viss_fourcc('B', 'G', 'R', '4') /* 32  BGR-8-8-8-8   */
#define VISS_PIX_FMT_RGB32   viss_fourcc('R', 'G', 'B', '4') /* 32  RGB-8-8-8-8   */

/* Grey formats */
#define VISS_PIX_FMT_GREY    viss_fourcc('G', 'R', 'E', 'Y') /*  8  Greyscale     */
#define VISS_PIX_FMT_Y4      viss_fourcc('Y', '0', '4', ' ') /*  4  Greyscale     */
#define VISS_PIX_FMT_Y6      viss_fourcc('Y', '0', '6', ' ') /*  6  Greyscale     */
#define VISS_PIX_FMT_Y10     viss_fourcc('Y', '1', '0', ' ') /* 10  Greyscale     */
#define VISS_PIX_FMT_Y12     viss_fourcc('Y', '1', '2', ' ') /* 12  Greyscale     */
#define VISS_PIX_FMT_Y16     viss_fourcc('Y', '1', '6', ' ') /* 16  Greyscale     */

/* Grey bit-packed formats */
#define VISS_PIX_FMT_Y10BPACK    viss_fourcc('Y', '1', '0', 'B') /* 10  Greyscale
								  * bit-packed */

/* Palette formats */
#define VISS_PIX_FMT_PAL8    viss_fourcc('P', 'A', 'L', '8') /*  8  8-bit palette */

/* Chrominance formats */
#define VISS_PIX_FMT_UV8     viss_fourcc('U', 'V', '8', ' ') /*  8  UV 4:4 */

/* Luminance+Chrominance formats */
#define VISS_PIX_FMT_YVU410  viss_fourcc('Y', 'V', 'U', '9') /*  9  YVU 4:1:0     */
#define VISS_PIX_FMT_YVU420  viss_fourcc('Y', 'V', '1', '2') /* 12  YVU 4:2:0     */
#define VISS_PIX_FMT_YUYV    viss_fourcc('Y', 'U', 'Y', 'V') /* 16  YUV 4:2:2     */
#define VISS_PIX_FMT_YYUV    viss_fourcc('Y', 'Y', 'U', 'V') /* 16  YUV 4:2:2     */
#define VISS_PIX_FMT_YVYU    viss_fourcc('Y', 'V', 'Y', 'U') /* 16 YVU 4:2:2 */
#define VISS_PIX_FMT_UYVY    viss_fourcc('U', 'Y', 'V', 'Y') /* 16  YUV 4:2:2     */
#define VISS_PIX_FMT_VYUY    viss_fourcc('V', 'Y', 'U', 'Y') /* 16  YUV 4:2:2     */
#define VISS_PIX_FMT_YUV422P viss_fourcc('4', '2', '2', 'P') /* 16  YVU422 planar */
#define VISS_PIX_FMT_YUV411P viss_fourcc('4', '1', '1', 'P') /* 16  YVU411 planar */
#define VISS_PIX_FMT_Y41P    viss_fourcc('Y', '4', '1', 'P') /* 12  YUV 4:1:1     */
#define VISS_PIX_FMT_YUV444  viss_fourcc('Y', '4', '4', '4') /* 16  xxxxyyyy uuuuvvvv */
#define VISS_PIX_FMT_YUV555  viss_fourcc('Y', 'U', 'V', 'O') /* 16  YUV-5-5-5     */
#define VISS_PIX_FMT_YUV565  viss_fourcc('Y', 'U', 'V', 'P') /* 16  YUV-5-6-5     */
#define VISS_PIX_FMT_YUV32   viss_fourcc('Y', 'U', 'V', '4') /* 32  YUV-8-8-8-8   */
#define VISS_PIX_FMT_YUV410  viss_fourcc('Y', 'U', 'V', '9') /*  9  YUV 4:1:0     */
#define VISS_PIX_FMT_YUV420  viss_fourcc('Y', 'U', '1', '2') /* 12  YUV 4:2:0     */
#define VISS_PIX_FMT_HI240   viss_fourcc('H', 'I', '2', '4') /*  8  8-bit color   */
#define VISS_PIX_FMT_HM12    viss_fourcc('H', 'M', '1', '2') /*  8  YUV 4:2:0 16x16
							      * macroblocks */
#define VISS_PIX_FMT_M420    viss_fourcc('M', '4', '2', '0') /* 12  YUV 4:2:0 2 lines y,
							      * 1 line uv interleaved */

/* two planes -- one Y, one Cr + Cb interleaved  */
#define VISS_PIX_FMT_NV12    viss_fourcc('N', 'V', '1', '2') /* 12  Y/CbCr 4:2:0  */
#define VISS_PIX_FMT_NV21    viss_fourcc('N', 'V', '2', '1') /* 12  Y/CrCb 4:2:0  */
#define VISS_PIX_FMT_NV16    viss_fourcc('N', 'V', '1', '6') /* 16  Y/CbCr 4:2:2  */
#define VISS_PIX_FMT_NV61    viss_fourcc('N', 'V', '6', '1') /* 16  Y/CrCb 4:2:2  */
#define VISS_PIX_FMT_NV24    viss_fourcc('N', 'V', '2', '4') /* 24  Y/CbCr 4:4:4  */
#define VISS_PIX_FMT_NV42    viss_fourcc('N', 'V', '4', '2') /* 24  Y/CrCb 4:4:4  */

/* two non contiguous planes - one Y, one Cr + Cb interleaved  */
#define VISS_PIX_FMT_NV12M   viss_fourcc('N', 'M', '1', '2') /* 12  Y/CbCr 4:2:0  */
#define VISS_PIX_FMT_NV21M   viss_fourcc('N', 'M', '2', '1') /* 21  Y/CrCb 4:2:0  */
#define VISS_PIX_FMT_NV12MT  viss_fourcc('T', 'M', '1', '2') /* 12  Y/CbCr 4:2:0 64x32
							      * macroblocks */
#define VISS_PIX_FMT_NV12MT_16X16 viss_fourcc('V', 'M', '1', '2') /* 12  Y/CbCr 4:2:0
								   * 16x16 macroblocks */

/* three non contiguous planes - Y, Cb, Cr */
#define VISS_PIX_FMT_YUV420M viss_fourcc('Y', 'M', '1', '2') /* 12  YUV420 planar */
#define VISS_PIX_FMT_YVU420M viss_fourcc('Y', 'M', '2', '1') /* 12  YVU420 planar */

/* Bayer formats - see http://www.siliconimaging.com/RGB%20Bayer.htm */
#define VISS_PIX_FMT_SBGGR8  viss_fourcc('B', 'A', '8', '1') /*  8  BGBG.. GRGR.. */
#define VISS_PIX_FMT_SGBRG8  viss_fourcc('G', 'B', 'R', 'G') /*  8  GBGB.. RGRG.. */
#define VISS_PIX_FMT_SGRBG8  viss_fourcc('G', 'R', 'B', 'G') /*  8  GRGR.. BGBG.. */
#define VISS_PIX_FMT_SRGGB8  viss_fourcc('R', 'G', 'G', 'B') /*  8  RGRG.. GBGB.. */
#define VISS_PIX_FMT_SBGGR10 viss_fourcc('B', 'G', '1', '0') /* 10  BGBG.. GRGR.. */
#define VISS_PIX_FMT_SGBRG10 viss_fourcc('G', 'B', '1', '0') /* 10  GBGB.. RGRG.. */
#define VISS_PIX_FMT_SGRBG10 viss_fourcc('B', 'A', '1', '0') /* 10  GRGR.. BGBG.. */
#define VISS_PIX_FMT_SRGGB10 viss_fourcc('R', 'G', '1', '0') /* 10  RGRG.. GBGB.. */
#define VISS_PIX_FMT_SBGGR12 viss_fourcc('B', 'G', '1', '2') /* 12  BGBG.. GRGR.. */
#define VISS_PIX_FMT_SGBRG12 viss_fourcc('G', 'B', '1', '2') /* 12  GBGB.. RGRG.. */
#define VISS_PIX_FMT_SGRBG12 viss_fourcc('B', 'A', '1', '2') /* 12  GRGR.. BGBG.. */
#define VISS_PIX_FMT_SRGGB12 viss_fourcc('R', 'G', '1', '2') /* 12  RGRG.. GBGB.. */
/* 10bit raw bayer a-law compressed to 8 bits */
#define VISS_PIX_FMT_SBGGR10ALAW8 viss_fourcc('a', 'B', 'A', '8')
#define VISS_PIX_FMT_SGBRG10ALAW8 viss_fourcc('a', 'G', 'A', '8')
#define VISS_PIX_FMT_SGRBG10ALAW8 viss_fourcc('a', 'g', 'A', '8')
#define VISS_PIX_FMT_SRGGB10ALAW8 viss_fourcc('a', 'R', 'A', '8')
/* 10bit raw bayer DPCM compressed to 8 bits */
#define VISS_PIX_FMT_SBGGR10DPCM8 viss_fourcc('b', 'B', 'A', '8')
#define VISS_PIX_FMT_SGBRG10DPCM8 viss_fourcc('b', 'G', 'A', '8')
#define VISS_PIX_FMT_SGRBG10DPCM8 viss_fourcc('B', 'D', '1', '0')
#define VISS_PIX_FMT_SRGGB10DPCM8 viss_fourcc('b', 'R', 'A', '8')
/*
 * 10bit raw bayer, expanded to 16 bits
 * xxxxrrrrrrrrrrxxxxgggggggggg xxxxggggggggggxxxxbbbbbbbbbb...
 */
#define VISS_PIX_FMT_SBGGR16 viss_fourcc('B', 'Y', 'R', '2') /* 16  BGBG.. GRGR.. */

/* compressed formats */
#define VISS_PIX_FMT_MJPEG    viss_fourcc('M', 'J', 'P', 'G') /* Motion-JPEG   */
#define VISS_PIX_FMT_JPEG     viss_fourcc('J', 'P', 'E', 'G') /* JFIF JPEG     */
#define VISS_PIX_FMT_DV       viss_fourcc('d', 'v', 's', 'd') /* 1394          */
#define VISS_PIX_FMT_MPEG     viss_fourcc('M', 'P', 'E', 'G') /* MPEG-1/2/4 Multiplexed */
#define VISS_PIX_FMT_H264     viss_fourcc('H', '2', '6', '4') /* H264 with start codes */
#define VISS_PIX_FMT_H264_NO_SC viss_fourcc('A', 'V', 'C', '1') /* H264 without
								 * start codes */
#define VISS_PIX_FMT_H264_MVC viss_fourcc('M', '2', '6', '4') /* H264 MVC */
#define VISS_PIX_FMT_H263     viss_fourcc('H', '2', '6', '3') /* H263          */
#define VISS_PIX_FMT_MPEG1    viss_fourcc('M', 'P', 'G', '1') /* MPEG-1 ES     */
#define VISS_PIX_FMT_MPEG2    viss_fourcc('M', 'P', 'G', '2') /* MPEG-2 ES     */
#define VISS_PIX_FMT_MPEG4    viss_fourcc('M', 'P', 'G', '4') /* MPEG-4 ES     */
#define VISS_PIX_FMT_XVID     viss_fourcc('X', 'V', 'I', 'D') /* Xvid           */
#define VISS_PIX_FMT_VC1_ANNEX_G viss_fourcc('V', 'C', '1', 'G') /* SMPTE 421M Annex
								  * G compliant stream */
#define VISS_PIX_FMT_VC1_ANNEX_L viss_fourcc('V', 'C', '1', 'L') /* SMPTE 421M Annex
								  * L compliant stream */
#define VISS_PIX_FMT_VP8      viss_fourcc('V', 'P', '8', '0') /* VP8 */

/*  Vendor-specific formats   */
#define VISS_PIX_FMT_CPIA1    viss_fourcc('C', 'P', 'I', 'A') /* cpia1 YUV */
#define VISS_PIX_FMT_WNVA     viss_fourcc('W', 'N', 'V', 'A') /* Winnov hw compress */
#define VISS_PIX_FMT_SN9C10X  viss_fourcc('S', '9', '1', '0') /* SN9C10x compression */
#define VISS_PIX_FMT_SN9C20X_I420 viss_fourcc('S', '9', '2', '0') /* SN9C20x YUV 4:2:0 */
#define VISS_PIX_FMT_PWC1     viss_fourcc('P', 'W', 'C', '1') /* pwc older webcam */
#define VISS_PIX_FMT_PWC2     viss_fourcc('P', 'W', 'C', '2') /* pwc newer webcam */
#define VISS_PIX_FMT_ET61X251 viss_fourcc('E', '6', '2', '5') /* ET61X251 compression */
#define VISS_PIX_FMT_SPCA501  viss_fourcc('S', '5', '0', '1') /* YUYV per line */
#define VISS_PIX_FMT_SPCA505  viss_fourcc('S', '5', '0', '5') /* YYUV per line */
#define VISS_PIX_FMT_SPCA508  viss_fourcc('S', '5', '0', '8') /* YUVY per line */
#define VISS_PIX_FMT_SPCA561  viss_fourcc('S', '5', '6', '1') /* compressed GBRG bayer */
#define VISS_PIX_FMT_PAC207   viss_fourcc('P', '2', '0', '7') /* compressed BGGR bayer */
#define VISS_PIX_FMT_MR97310A viss_fourcc('M', '3', '1', '0') /* compressed BGGR bayer */
#define VISS_PIX_FMT_JL2005BCD viss_fourcc('J', 'L', '2', '0') /* compressed RGGB bayer */
#define VISS_PIX_FMT_SN9C2028 viss_fourcc('S', 'O', 'N', 'X') /* compressed GBRG bayer */
#define VISS_PIX_FMT_SQ905C   viss_fourcc('9', '0', '5', 'C') /* compressed RGGB bayer */
#define VISS_PIX_FMT_PJPG     viss_fourcc('P', 'J', 'P', 'G') /* Pixart 73xx JPEG */
#define VISS_PIX_FMT_OV511    viss_fourcc('O', '5', '1', '1') /* ov511 JPEG */
#define VISS_PIX_FMT_OV518    viss_fourcc('O', '5', '1', '8') /* ov518 JPEG */
#define VISS_PIX_FMT_STV0680  viss_fourcc('S', '6', '8', '0') /* stv0680 bayer */
#define VISS_PIX_FMT_TM6000   viss_fourcc('T', 'M', '6', '0') /* tm5600/tm60x0 */
#define VISS_PIX_FMT_CIT_YYVYUY viss_fourcc('C', 'I', 'T', 'V') /* one line of Y then 1
								 * line of VYUY */
#define VISS_PIX_FMT_KONICA420  viss_fourcc('K', 'O', 'N', 'I') /* YUV420 planar in
								 * blocks of 256 pixels */
#define VISS_PIX_FMT_JPGL	viss_fourcc('J', 'P', 'G', 'L') /* JPEG-Lite */
#define VISS_PIX_FMT_SE401      viss_fourcc('S', '4', '0', '1') /* se401 janggu
								 * compressed rgb */
#define VISS_PIX_FMT_S5C_UYVY_JPG viss_fourcc('S', '5', 'C', 'I') /* S5C73M3 interleaved
								   * UYVY/JPEG */

#endif /* __VIDEODEV2_H__ */
