#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lombo_lib.h"
#include "lombo_jpegdec_plugin.h"
#include "jpeg_dec_interface.h"

#define STREAM_BUF_SIZE (1024*1024)

int jpeg_decode_start(char *jpeg_path, jpeg_param_t *jpg_param)
{
	jpeg_dec_packet_t jpeg_dec_packet = { 0 };
	jpeg_dec_parm_t jpeg_dec_parm = { 0 };
	jpeg_dec_buf_handle_t jpeg_dec_buf_handle = { 0 };
	int jpeg_file = -1;
	struct stat f_stat;
	int str_len;
	int len, i, ret, f_size;
	void *handle;

#ifdef MEDIA_PRINT_INF_ON
	rt_tick_t rt_start;
	rt_start = rt_tick_get();
#endif
	str_len = strlen(jpeg_path);
	if (strcmp(jpeg_path + str_len - 3, "jpg")
			&& strcmp(jpeg_path + str_len - 3, "jpeg")) {
		MD_ERR("only support jpg or jpeg");
		return -1;
	}

	jpeg_file = open(jpeg_path, O_RDONLY);
	if (jpeg_file == -1) {
		MD_ERR("file open failed: %s", jpeg_path);
		return -1;
	}
#ifdef MEDIA_PRINT_INF_ON
	MD_LOG("open time = %d", rt_tick_get()-rt_start);
	rt_start = rt_tick_get();
#endif
	if (fstat(jpeg_file, &f_stat)) {
		MD_ERR("fstat error");
		goto jpeg_decode_start_exit;
	}
	f_size = f_stat.st_size;
	f_size += (72 * 2);
#ifdef MEDIA_PRINT_INF_ON
	MD_LOG("fstat time = %d, %d", rt_tick_get()-rt_start, f_size);
	rt_start = rt_tick_get();
#endif

	MD_LOG("lombo_al_malloc");
	jpeg_dec_packet.file_addr = (unsigned char *) lombo_al_malloc
				(f_size, MEM_VIRT, NULL, __FILE__, __LINE__);
	MD_LOG("lombo_al_malloc");
	if (jpeg_dec_packet.file_addr == NULL) {
		MD_ERR("malloc jpeg_dec_packet.file_addr error");
		goto jpeg_decode_start_exit;
	}
#ifdef MEDIA_PRINT_INF_ON
	MD_LOG("lombo_al_malloc time = %d", rt_tick_get()-rt_start);
	rt_start = rt_tick_get();
#endif

	len = read(jpeg_file, jpeg_dec_packet.file_addr, f_size);
	if (len == f_size - 72 * 2) {
		for (i = 0; i < 72; i++) {
			jpeg_dec_packet.file_addr[len + i * 2 + 0] = 0xFF;
			jpeg_dec_packet.file_addr[len + i * 2 + 1] = 0xD0;
		}
		len += 72 * 2;
	} else {
		MD_ERR("read failed: %s, %d", jpeg_path, len);
		goto jpeg_decode_start_exit;
	}

	jpeg_dec_packet.file_buf_len = f_size;
	jpeg_dec_packet.file_offset = 0;
#ifdef MEDIA_PRINT_INF_ON
	MD_LOG("read time = %d", rt_tick_get()-rt_start);
	rt_start = rt_tick_get();
#endif
	jpeg_dec_packet.vir_addr = (unsigned char *) lombo_al_malloc
			(STREAM_BUF_SIZE, MEM_UC, &(jpeg_dec_packet.phy_addr),
			__FILE__, __LINE__);
	if (jpeg_dec_packet.vir_addr == NULL) {
		MD_ERR("malloc jpeg_dec_packet.vir_addr error");
		goto jpeg_decode_start_exit;
	}
#ifdef MEDIA_PRINT_INF_ON
	MD_LOG("lombo_al_malloc time = %d", rt_tick_get()-rt_start);
	rt_start = rt_tick_get();
#endif
	jpeg_dec_packet.head.packet_len = STREAM_BUF_SIZE;

	jpeg_dec_parm.sec_output_format = 2;
	jpeg_dec_parm.down_scale_ratio = 0;
	jpeg_dec_parm.rotation_degree = 0;
	jpeg_dec_parm.luma_stride = 0;
	jpeg_dec_parm.chroma_stride = 0;
	jpeg_dec_parm.chroma_offset = 0;
	handle = jpeg_dec_open(&jpeg_dec_parm, &jpeg_dec_packet);
	if (handle == NULL) {
		MD_ERR("jpeg_dec_open fail!");
		goto jpeg_decode_start_exit;
	}
	jpeg_dec_buf_handle.head.width_stride = jpeg_dec_parm.stride_luma;
	jpeg_dec_buf_handle.head.output_height = jpeg_dec_parm.output_height;
	jpg_param->output_w = jpeg_dec_parm.stride_luma;
	jpg_param->output_h = jpeg_dec_parm.output_height;
	jpg_param->real_w = jpeg_dec_parm.crop_out_width;
	jpg_param->real_h = jpeg_dec_parm.crop_out_height;
#ifdef MEDIA_PRINT_INF_ON
	MD_LOG("jpeg_dec_open time = %d", rt_tick_get()-rt_start);
	rt_start = rt_tick_get();
#endif

	MD_LOG("jpg inf(%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)",
		jpeg_dec_parm.crop_out_width, jpeg_dec_parm.crop_out_height,
		jpeg_dec_parm.crop_left_offset, jpeg_dec_parm.crop_top_offset,
		jpeg_dec_parm.output_height, jpeg_dec_parm.output_width,
		jpeg_dec_parm.stride_luma, jpeg_dec_parm.stride_chroma,
		jpeg_dec_parm.one_frame_size, jpeg_dec_parm.offset_luma,
		jpeg_dec_parm.offset_chroma, jpeg_dec_parm.size_luma,
		jpeg_dec_parm.size_chroma);

#ifdef MEDIA_PRINT_INF_ON
	MD_LOG("MD_LOG time = %d", rt_tick_get()-rt_start);
	rt_start = rt_tick_get();
#endif
	jpeg_dec_buf_handle.vir_addr = (unsigned char *) lombo_al_malloc(
			jpeg_dec_parm.one_frame_size, MEM_WT/* MEM_UC */,
			&(jpeg_dec_buf_handle.phy_addr),
			__FILE__, __LINE__);
	if (jpeg_dec_buf_handle.vir_addr == NULL) {
		MD_ERR("malloc jpeg_dec_buf_handle.vir_addr error\n");
		goto jpeg_decode_start_exit;
	}

	MD_LOG("jpeg_dec_frame start");
	ret = jpeg_dec_frame(handle, &jpeg_dec_packet, &jpeg_dec_buf_handle);
	if (ret) {
		MD_ERR("decode frame error");
		goto jpeg_decode_start_exit;
	}
	MD_LOG("jpeg_dec_frame end");
#ifdef MEDIA_PRINT_INF_ON
	MD_LOG("jpg dec time = %d", rt_tick_get()-rt_start);
	rt_start = rt_tick_get();
#endif
	jpg_param->jpeg_file = 1;
	jpg_param->output_data = jpeg_dec_buf_handle.vir_addr;
	jpg_param->offset_luma = jpeg_dec_parm.offset_luma;
	jpg_param->size_luma = jpeg_dec_parm.size_luma;
	jpg_param->offset_chroma = jpeg_dec_parm.offset_chroma;
	jpg_param->size_chroma = jpeg_dec_parm.size_chroma;

	MD_LOG("jpeg_dec_dispose start");
	jpeg_dec_dispose(handle);
	MD_LOG("jpeg_dec_dispose end");
#ifdef MEDIA_PRINT_INF_ON
	MD_LOG("jpg free = %d", rt_tick_get()-rt_start);
	rt_start = rt_tick_get();
#endif
	if (jpeg_dec_packet.file_addr != NULL) {
		lombo_al_free(jpeg_dec_packet.file_addr, MEM_VIRT);
		jpeg_dec_packet.file_addr = NULL;
	}
	if (jpeg_dec_packet.vir_addr != NULL) {
		lombo_al_free(jpeg_dec_packet.vir_addr, MEM_UC);
		jpeg_dec_packet.vir_addr = NULL;
	}
#ifdef MEDIA_PRINT_INF_ON
	MD_LOG("lombo_al_free = %d", rt_tick_get()-rt_start);
	rt_start = rt_tick_get();
#endif
	close(jpeg_file);
#ifdef MEDIA_PRINT_INF_ON
	MD_LOG("fclose = %d", rt_tick_get()-rt_start);
	rt_start = rt_tick_get();
#endif
#if 0
	rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH|RT_HW_CACHE_INVALIDATE,
			(void *)jpeg_dec_buf_handle.vir_addr,
			jpeg_dec_parm.one_frame_size);
	rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH|RT_HW_CACHE_INVALIDATE,
			(void *)jpeg_dec_buf_handle.vir_addr,
			jpeg_dec_parm.one_frame_size);
#endif
	return 0;

jpeg_decode_start_exit:
	if (jpeg_dec_buf_handle.vir_addr != NULL) {
		lombo_al_free(jpeg_dec_buf_handle.vir_addr, MEM_UC);
		jpeg_dec_buf_handle.vir_addr = NULL;
	}
	if (jpeg_dec_packet.file_addr != NULL) {
		lombo_al_free(jpeg_dec_packet.file_addr, MEM_VIRT);
		jpeg_dec_packet.file_addr = NULL;
	}
	if (jpeg_dec_packet.vir_addr != NULL) {
		lombo_al_free(jpeg_dec_packet.vir_addr, MEM_UC);
		jpeg_dec_packet.vir_addr = NULL;
	}
	close(jpeg_file);

	return -1;
}

int jpeg_decode_end(jpeg_param_t *jpg_param)
{
	if (jpg_param && jpg_param->output_data) {
		lombo_al_free(jpg_param->output_data, MEM_WT/* MEM_UC */);
		jpg_param->output_data = NULL;
	}
	return 0;
}
