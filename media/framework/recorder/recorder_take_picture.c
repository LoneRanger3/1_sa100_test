/*
 * recorder_take_picture.c - Standard functionality for take picture.
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
#define DBG_LEVEL         3
#include <oscl.h>
#include <base_component.h>
#include "vrender_component.h"
#include "vrec_component.h"
#include "framework_common.h"
#include "lb_recorder.h"
#include "recorder_private.h"
#include "omx_vendor_lb.h"
#include "watermark/watermark.h"
#include "lombo_jpegenc_plugin.h"

#define JPEG_INFO_SIZE  (8*1024)
#define JPEG_STREAM_BUF_SIZE (800*1024)
#define MAX_PIC_ENC 3

typedef struct pic_msg {
	al_frame_t *frame;
	unsigned char *extra_data;
	int extra_data_len;
	jpeg_enc_packet_t packet;
	char *file_name;
	void *watermark_private;
	win_rect_t enc_rect;
	int offset[3];
} pic_msg_t;

typedef struct pic_private {
	sem_t frame_sem;
	sem_t pic_taken_sem;
	pthread_mutex_t lock;
	oscl_queue_t queue;
	int num_finished;
	int total_encoding;
	int thread_id;
	pthread_t enc_thread;
	int stop_flag;
	pic_msg_t pic[MAX_PIC_ENC];
} pic_private_t;


static int OmxFormat_to_Enc(int OmxFormat)
{
	int EncodeFormat = -1;

	if (OmxFormat == OMX_COLOR_FormatYUV420Planar)
		EncodeFormat = VC_YUV420P;
	else if (OmxFormat == OMX_COLOR_FormatYUV420SemiPlanar)
		EncodeFormat = VC_YUV420SP;

	if (EncodeFormat == -1)
		OSCL_LOGE("not support the OmxFormat\n");

	return EncodeFormat;
}
static int OmxFormat_to_Blending(int OmxFormat)
{
	int EncodeFormat = -1;

	if (OmxFormat == OMX_COLOR_Format32bitARGB8888)
		EncodeFormat = VC_ARGB8888;
	else if (OmxFormat == OMX_COLOR_Format32bitBGRA8888)
		EncodeFormat = VC_BGRA8888;
	else if (OmxFormat == OMX_COLOR_Format32BitRGBA8888)
		EncodeFormat = VC_RGBA8888;

	if (EncodeFormat == -1)
		OSCL_LOGE("not support the OmxFormat blending\n");
	return EncodeFormat;
}

static int __set_enc_area(pic_msg_t *pic, jpeg_enc_parm_t *encode_parm)
{
	int src_height;
	int src_width;
	pic->offset[0] = 0;
	pic->offset[1] = 0;
	pic->offset[2] = 0;

	src_height = pic->frame->info.video.height;
	src_width = pic->frame->info.video.width;
	if (pic->enc_rect.width > src_width || pic->enc_rect.width <= 0)
		pic->enc_rect.width = src_width;
	if (pic->enc_rect.height > src_height || pic->enc_rect.height <= 0)
		pic->enc_rect.height = src_height;
	encode_parm->in_width = src_width;
	encode_parm->in_height = src_height;
	encode_parm->l_stride = (encode_parm->in_width + 15) / 16 * 16;
	encode_parm->out_width = pic->enc_rect.width;
	encode_parm->out_height = pic->enc_rect.height;
	encode_parm->input_mode = OmxFormat_to_Enc(pic->frame->info.video.color_fmt);
	encode_parm->buf_info = pic->extra_data;
	encode_parm->buf_info_size = pic->extra_data_len;
	encode_parm->quality = 50;

	if (pic->enc_rect.width != src_width || pic->enc_rect.height != src_height) {
		pic->offset[0] = src_width * pic->enc_rect.y;
		pic->offset[0] += pic->enc_rect.x;
		if (encode_parm->input_mode == VC_YUV420SP) {
			pic->offset[1] = src_width * pic->enc_rect.y / 2;
			pic->offset[1] += pic->enc_rect.x;
		}
		if (encode_parm->input_mode == VC_YUV420P) {
			pic->offset[1] = src_width * pic->enc_rect.y / 4;
			pic->offset[1] += pic->enc_rect.x / 2;
			pic->offset[2] = pic->offset[1];
		}
		encode_parm->in_width = pic->enc_rect.width;
		encode_parm->in_height = pic->enc_rect.height;
		encode_parm->out_width = pic->enc_rect.width;
		encode_parm->out_height = pic->enc_rect.height;
	}
	return 0;
}

int jpeg_encode_frame(pic_msg_t *pic)
{
	FILE *flip = NULL;
	capbuf_t capbuf;
	jpeg_enc_parm_t encode_parm;
	void *handle;
	int ret = -1;
	jpeg_enc_packet_t *pic_packet;
	int nmark;
	numeral_output_t *mark;
	numeral_output_t blending;
	int i;

	oscl_param_check(pic != NULL, -1, NULL);
	oscl_param_check(pic->frame != NULL, -1, NULL);
	pic_packet = &pic->packet;
	oscl_param_check(pic_packet->vir_addr != NULL, -1, NULL);

	__set_enc_area(pic, &encode_parm);
	memset(&capbuf, 0, sizeof(capbuf_t));
	capbuf.vir_addr[0] = pic->frame->info.video.addr[0] + pic->offset[0];
	capbuf.vir_addr[1] = pic->frame->info.video.addr[1] + pic->offset[1];
	capbuf.vir_addr[2] = pic->frame->info.video.addr[2] + pic->offset[2];

	capbuf.phy_addr[0] = oscl_virt_to_phys(pic->frame->info.video.addr[0]);
	if (pic->frame->info.video.addr[1] != NULL)
		capbuf.phy_addr[1] = oscl_virt_to_phys(pic->frame->info.video.addr[1]);
	if (pic->frame->info.video.addr[2] != NULL)
		capbuf.phy_addr[2] = oscl_virt_to_phys(pic->frame->info.video.addr[2]);
	capbuf.phy_addr[0] += pic->offset[0];
	capbuf.phy_addr[1] += pic->offset[1];
	capbuf.phy_addr[2] += pic->offset[2];

	handle = jpeg_enc_open(&encode_parm, NULL);
	if (handle == NULL) {
		OSCL_LOGE("open jpeg encoder failed!");
		goto EXIT;
	}

	nmark = watermark_get_markarray(pic->watermark_private, &mark);
	if (nmark != 0 && mark != NULL) {
		if (nmark > MAX_JPG_BLENDING_NUM)
			nmark = MAX_JPG_BLENDING_NUM;
		OSCL_LOGI("set watermark to jpeg encoder!");
	}
	for (i = 0; i < nmark; i++) {
		if (mark[i].numeral_picture.picture_size != 0) {
			memcpy(&blending, &mark[i], sizeof(numeral_output_t));
			blending.colorspace = OmxFormat_to_Blending(mark[i].colorspace);
			jpeg_enc_ops(handle, SET_BLENDING_PICTURE, &blending);
		} else
			jpeg_enc_ops(handle, DISABLE_BLENDING, &i);
	}

	ret = jpeg_enc_frame(handle, &capbuf, &pic->packet, &encode_parm);
	watermark_put_markarray(pic->watermark_private, mark);
	jpeg_enc_dispose(handle);
	if (ret < 0)
		OSCL_LOGE("jpeg encode frame err:%d!", ret);

	if (pic->file_name)
		flip = fopen(pic->file_name, "wb");
	if (flip == NULL) {
		OSCL_LOGE("open out file err!\n");
		goto EXIT;
	}
	ret = fwrite(pic->packet.vir_addr, 1, pic->packet.data_size, flip);
	oscl_param_check_exit(ret == pic->packet.data_size, -1, NULL);
	ret = 0;

EXIT:
	if (flip)
		fclose(flip);
	return ret;
}

void *picture_encoder_task(void *param)
{
	video_recorder_t *video_rec = param;
	pic_private_t *pic_private = video_rec->pic_private;
	pic_msg_t *pic;

	while (pic_private->stop_flag == 0) {
		sem_wait(&pic_private->frame_sem);
		pic = oscl_queue_dequeue(&pic_private->queue);
		if (pic == NULL)
			continue;
		jpeg_encode_frame(pic);
		app_empty_buffer_done(pic->frame);
		OSCL_LOGI("take pic,sem_post");
		sem_post(&pic_private->pic_taken_sem);
	}
	sem_post(&pic_private->pic_taken_sem);
	return NULL;
}

pic_msg_t *_get_msg(pic_private_t *pic_private)
{
	int i;
	pic_msg_t *msg = NULL;

	if (pic_private == NULL)
		return NULL;
	for (i = 0; i < MAX_PIC_ENC; i++)
		if (pic_private->pic[i].frame == NULL && pic_private->pic[i].extra_data)
			break;
	if (i != MAX_PIC_ENC)
		msg = &pic_private->pic[i];
	return msg;
}


static int _pic_get_buffer(void *eplayer, al_frame_t *frame)
{
	pic_msg_t *pic = NULL;
	int ret = 0;
	video_recorder_t *video_rec = eplayer;
	pic_private_t *pic_private = video_rec->pic_private;

	pthread_mutex_lock(&pic_private->lock);
	if (pic_private->num_finished >= pic_private->total_encoding) {
		pthread_mutex_unlock(&pic_private->lock);
		ret = app_empty_buffer_done(frame);
		goto EXIT;
	}
	pic = _get_msg(pic_private);
	if (pic) {
		pic->frame = frame;
		oscl_queue_queue(&pic_private->queue, pic);
		pic_private->num_finished++;
		sem_post(&pic_private->frame_sem);
	}
	pthread_mutex_unlock(&pic_private->lock);
EXIT:
	return ret;
}

void _deinit_msg(pic_msg_t *pic)
{
	if (pic->extra_data)
		oscl_free(pic->extra_data);
	if (pic->packet.vir_addr)
		oscl_free_align(pic->packet.vir_addr);
	if (pic->file_name)
		oscl_free(pic->file_name);
	memset(pic, 0, sizeof(pic_msg_t));
}

int _init_msg(pic_msg_t *pic, char *file, video_recorder_t *video_rec)
{
	int ret = 0;
	void *watermark_private = video_rec->watermark_private;
	pic->frame = NULL;
	pic->file_name = oscl_strdup(file);
	oscl_param_check_exit(pic->file_name, -1, NULL);

	pic->extra_data_len = JPEG_INFO_SIZE;
	pic->extra_data = oscl_zalloc(JPEG_INFO_SIZE);
	oscl_param_check_exit(pic->extra_data, -1, NULL);

	pic->packet.buf_size = JPEG_STREAM_BUF_SIZE;
	pic->packet.vir_addr = oscl_malloc_align(JPEG_STREAM_BUF_SIZE, 4096);
	pic->packet.phy_addr = oscl_virt_to_phys(pic->packet.vir_addr);
	oscl_param_check_exit(pic->packet.vir_addr, -1, NULL);

	pic->watermark_private = watermark_private;
	memcpy(&pic->enc_rect, &video_rec->rec_para.enc_rect, sizeof(win_rect_t));
EXIT:
	if (ret)
		_deinit_msg(pic);
	return ret;
}


int video_rec_take_picture(video_recorder_t *video_rec, char *file)
{
	int ret = 0;
	app_frame_cb_t cb;
	pic_private_t *pic_private;
	vsrc_camera_t *enc_cam = NULL;

	OSCL_TRACE("start");
	oscl_param_check_exit(video_rec, -1, NULL);
	oscl_param_check_exit(video_rec->pic_private, -1, NULL);
	pic_private = video_rec->pic_private;

	enc_cam = get_enc_cam(video_rec);
	/* If already get source port, there is taking-picture task unfinished, just
	 * return failed. Otherwise get a camera port.
	*/
	pthread_mutex_lock(&pic_private->lock);
	if (video_rec->src_pic != NULL) {
		ret = -1;
		pthread_mutex_unlock(&pic_private->lock);
		goto EXIT;
	}
	video_rec->src_pic = vsrc_camera_getport(enc_cam, VDISP_ROTATE_NONE);
	pthread_mutex_unlock(&pic_private->lock);
	oscl_param_check_exit(video_rec->src_pic, -1, NULL);

	/* initial pic message info */
	_init_msg(&pic_private->pic[0], file, video_rec);
	pic_private->total_encoding = 1;

	/* setup raw data callback to camera port and enable it. */
	cb.app_data = video_rec;
	cb.buf_handle = _pic_get_buffer;
	cb.type = AL_VIDEO_RAW_FRAME;
	al_untunnel_setup_cb(video_rec->src_pic, &cb);
	ret = vsrc_camera_enable_port(enc_cam, video_rec->src_pic);

	/* wait for take picture finished and then unset and free camera port */
	sem_wait(&pic_private->pic_taken_sem);
	OSCL_LOGI("take picture finished");
	vsrc_camera_disable_port(enc_cam, video_rec->src_pic);
	al_untunnel_unset_cb(video_rec->src_pic);
	video_rec->src_pic->cb.buf_handle = NULL;
	video_rec->src_pic->cb.app_data = NULL;
	vsrc_camera_putport(enc_cam, video_rec->src_pic);

	_deinit_msg(&pic_private->pic[0]);
	pic_private->total_encoding = 0;
	pic_private->num_finished = 0;

	pthread_mutex_lock(&pic_private->lock);
	video_rec->src_pic = NULL;
	pthread_mutex_unlock(&pic_private->lock);
	ret = 0;

EXIT:
	put_enc_cam(video_rec, enc_cam);
	return ret;
}

int take_pic_init(video_recorder_t *video_rec)
{
	pic_private_t *pic_private;
	pthread_attr_t thread_attr;
	struct sched_param shed_param = {0};

	oscl_param_check(video_rec, -1, NULL);
	oscl_param_check(video_rec->pic_private == NULL, -1, NULL);

	pic_private = oscl_zalloc(sizeof(pic_private_t));
	oscl_param_check(pic_private, -1, NULL);
	sem_init(&pic_private->frame_sem, 0, 0);
	sem_init(&pic_private->pic_taken_sem, 0, 0);
	pthread_mutex_init(&pic_private->lock, NULL);
	oscl_queue_init(&pic_private->queue);
	video_rec->pic_private = pic_private;

	pthread_attr_init(&thread_attr);
	pthread_attr_setstacksize(&thread_attr, 0x1000);
	shed_param.sched_priority = 15;
	pthread_attr_setschedparam(&thread_attr, &shed_param);
	pic_private->thread_id = pthread_create(&pic_private->enc_thread, &thread_attr,
			picture_encoder_task, video_rec);
	if (pic_private->thread_id < 0)
		return OMX_ErrorInsufficientResources;

	return 0;
}

void take_pic_deinit(video_recorder_t *video_rec)
{
	pic_private_t *pic_private;

	if (video_rec == NULL || video_rec->pic_private == NULL)
		return;
	pic_private = video_rec->pic_private;

	while (video_rec->src_pic != NULL) {
		OSCL_LOGE("taking picture while exit take-pic thread, waiting");
		oscl_mdelay(10);
	}

	if (pic_private->thread_id == 0) {
		pic_private->stop_flag = 1;
		sem_post(&pic_private->frame_sem);
		pthread_join(pic_private->enc_thread, NULL);
		pic_private->thread_id = -1;
	}
	video_rec->pic_private = NULL;
	sem_destroy(&pic_private->frame_sem);
	sem_destroy(&pic_private->pic_taken_sem);
	pthread_mutex_destroy(&pic_private->lock);
	oscl_queue_deinit(&pic_private->queue);
	oscl_free(pic_private);
}

