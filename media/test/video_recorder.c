#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "OMX_Index.h"
#include "OMX_Core.h"
#include "OMX_IVCommon.h"
#include "OMX_Component.h"
#include "lombo_al_lib.h"
#include "lb_omx_core.h"
#include "venc_component.h"

static int out_width = 640;
static int out_height = 480;
static int in_width = 352;
static int in_height = 288;
static int video_bitrate = 2000000;
static int slice_mode;
static int input_mode = OMX_COLOR_FormatYUV420Planar;
static int plan_num = -1;

static int input_port_index = -1;
static int output_port_index = -1;
static int total_port_num = -1;

static int offset_y = -1;
static int offset_u = -1;
static int offset_v = -1;

static int recorder_omx_state = OMX_StateInvalid;

static int file_end_flag;
static int retry_cnt;
static int capture_time;

static FILE *filp_out_raw;
static FILE *filp_in;
static void *omx_handle;


#define CAP_BUF_NUM 5
#define STREAM_BUF_NUM 8
#define STREAM_BUF_SIZE (800*1024)
#define MAX_PORT_NUM 6

static int input_buffer_in_video_recorder = -1;
static int output_buffer_in_video_recorder = -1;


static unsigned long stream_phy_addr[STREAM_BUF_NUM];
static unsigned char *stream_vir_addr[STREAM_BUF_NUM];
OMX_BUFFERHEADERTYPE *stream_omx_buf_header[STREAM_BUF_NUM];

static unsigned long capture_phy_addr[CAP_BUF_NUM];
static unsigned char *capture_vir_addr[CAP_BUF_NUM];
OMX_BUFFERHEADERTYPE *capture_omx_buf_header[CAP_BUF_NUM];
static unsigned int capture_buf_size = -1;

static mmm_sem_t omx_sem;

/*
static omx_component_tbl_t omx_video_encode_component = {
	"OMX.LB.VIDEO.ENCODECOMPONENT",
	1,
	video_encoder_component_init, NULL, NULL,
	{
		"video_encoder.jpeg",
		"video_encoder.h264",
		NULL
	}
};
*/


static rt_int32_t check_encode_mem()
{
	rt_uint32_t total = 0;
	rt_uint32_t used = 0;
	rt_uint32_t max_used = 0;
	rt_memory_info(&total, &used, &max_used);
	OSCL_LOGE("total memory: %d -- %d -- %d\n", total, used, max_used);
	return 0;
}

static int get_capture_data(unsigned char *vir_addr);


OMX_ERRORTYPE video_encode_EventHandler(OMX_IN OMX_HANDLETYPE hComponent,
	OMX_IN OMX_PTR pAppData,
	OMX_IN OMX_EVENTTYPE eEvent, OMX_IN OMX_U32 nData1,
	OMX_IN OMX_U32 nData2, OMX_IN OMX_PTR pEventData)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;

	switch (eEvent) {
	case OMX_EventCmdComplete:
		switch (nData1) {
		case OMX_CommandStateSet:
			switch (nData2) {
			case OMX_StateLoaded:
				recorder_omx_state = OMX_StateLoaded;
				lombo_al_printf("omx state is Loaded\n");
				break;
			case OMX_StateIdle:
				recorder_omx_state = OMX_StateIdle;
				lombo_al_printf("omx state is idle\n");
				break;
			case OMX_StateExecuting:
				recorder_omx_state = OMX_StateExecuting;
				lombo_al_printf("omx state is executing\n");
				break;
			case OMX_StatePause:
				recorder_omx_state = OMX_StatePause;
				lombo_al_printf("omx state is pause\n");
				break;
			case OMX_StateInvalid:
				recorder_omx_state = OMX_StateInvalid;
				lombo_al_printf("omx state is Invalid\n");
				break;
			default:
				lombo_al_printf("not support 1\n");
				break;
			}
			lombo_al_sem_post(&omx_sem);
			break;
		case OMX_CommandFlush:
			lombo_al_printf("flush end\n");
			break;
		default:
			lombo_al_printf("not support 2\n");
			break;
		}
		break;
	case OMX_EventError:
		lombo_al_printf("OMX_EventError\n");
		break;
	default:
		lombo_al_printf("not support 3\n");
		break;
	}
	return ret;
}


OMX_ERRORTYPE video_encode_EmptyBufferDone(OMX_IN OMX_HANDLETYPE hComponent,
	OMX_IN OMX_PTR pAppData, OMX_IN OMX_BUFFERHEADERTYPE *pBuffer)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	lombo_al_printf("empty buffer done\n");
	input_buffer_in_video_recorder++;
	if (file_end_flag != 1) {
		get_capture_data(pBuffer->pBuffer);
		pBuffer->nFilledLen = capture_buf_size;
		pBuffer->nTimeStamp = capture_time;
		OMX_EmptyThisBuffer(omx_handle, pBuffer);
		input_buffer_in_video_recorder--;
	}
	return ret;
}


OMX_ERRORTYPE video_encode_FillBufferDone(OMX_OUT OMX_HANDLETYPE hComponent,
	OMX_OUT OMX_PTR pAppData, OMX_OUT OMX_BUFFERHEADERTYPE *pBuffer)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	output_buffer_in_video_recorder++;
	lombo_al_printf("fill buffer done : %d - %d - %x-%x\n", pBuffer->nFilledLen,
		output_buffer_in_video_recorder, pBuffer, pBuffer->pBuffer);
	if (file_end_flag != 1) {
		if ((filp_out_raw != NULL) && (pBuffer->nFilledLen != 0xFFFFFFFF))
			fwrite(pBuffer->pBuffer, 1, pBuffer->nFilledLen, filp_out_raw);
	}
	if (ret != OMX_ErrorNone) {
		lombo_al_printf("set omx_index_lombo_index_buffer_use_end error\n");
		return ret;
	}
	if (file_end_flag != 1) {
		OMX_FillThisBuffer(omx_handle, pBuffer);
		output_buffer_in_video_recorder--;
	}
	return ret;
}


static OMX_CALLBACKTYPE CallBacks = {
	video_encode_EventHandler,
	video_encode_EmptyBufferDone,
	video_encode_FillBufferDone
};



static int get_capture_data(unsigned char *vir_addr)
{
	int read_len = 0;
	unsigned char *y_data = vir_addr + offset_y;
	unsigned char *u_data = NULL;
	unsigned char *v_data = NULL;
	int y_len = in_width * in_height;
	int u_len = -1;
	int v_len = -1;

	lombo_al_printf("y_len : %d\n", y_len);
GET_FRAME:
	read_len = fread(y_data, 1, y_len, filp_in);
	if (read_len != y_len) {
		if (retry_cnt == 0) {
			lombo_al_printf("file end 1\n");
			file_end_flag = 1;
			return -1;
		} else {
			retry_cnt--;
			fseek(filp_in, 0, 0);
			goto GET_FRAME;
		}
	}
	if (input_mode == OMX_COLOR_FormatYUV420Planar) {
		u_len = in_width * in_height / 4;
		u_data = vir_addr + offset_u;
		lombo_al_printf("u_len : %d\n", u_len);
		read_len = fread(u_data, 1, u_len, filp_in);
		if (read_len != u_len) {
			if (retry_cnt == 0) {
				lombo_al_printf("file end 2\n");
				file_end_flag = 1;
				return -1;
			} else {
				retry_cnt--;
				fseek(filp_in, 0, 0);
				goto GET_FRAME;
			}
		}
		v_len = in_width * in_height / 4;
		v_data = vir_addr + offset_v;
		lombo_al_printf("v_len : %d\n", v_len);
		read_len = fread(v_data, 1, v_len, filp_in);
		if (read_len != v_len) {
			if (retry_cnt == 0) {
				lombo_al_printf("file end 3\n");
				file_end_flag = 1;
				return -1;
			} else {
				retry_cnt--;
				fseek(filp_in, 0, 0);
				goto GET_FRAME;
			}
		}
	} else if (input_mode == OMX_COLOR_FormatYUV420SemiPlanar) {
		u_len = in_width * in_height / 2;
		u_data = vir_addr + offset_u;
		read_len = fread(u_data, 1, u_len, filp_in);
		if (read_len != u_len) {
			if (retry_cnt == 0) {
				lombo_al_printf("file end 2\n");
				file_end_flag = 1;
				return -1;
			} else {
				retry_cnt--;
				fseek(filp_in, 0, 0);
				goto GET_FRAME;
			}
		}
	} else {
		lombo_al_printf("not support inputformat\n");
		return -1;
	}

	capture_time += 10;
	lombo_al_printf("y_data : %x - %x - %x\n", y_data, u_data, v_data);

	return 0;
}


static int input_mode_to_num_plan(int input_mode)
{
	int plan_count = -1;

	if (input_mode == OMX_COLOR_FormatYUV420Planar)
		plan_count = 3;
	else if (input_mode == OMX_COLOR_FormatYUV420SemiPlanar)
		plan_count = 2;
	else
		lombo_al_printf("not support the input_mode\n");

	return plan_count;
}


static void alloc_capture_buf()
{
	int i = 0;
	int cap_offset[3] = {0};
	int cap_buf_num = CAP_BUF_NUM;
	int size = in_width * in_height * 3 / 2 + 4096 + 4096;
	capture_buf_size = size;
	OMX_ERRORTYPE ret = OMX_ErrorNone;

	for (i = 0; i < cap_buf_num; i++) {
		capture_vir_addr[i] = lombo_al_malloc(size,
				AL_MEM_UC, &(capture_phy_addr[i]), __FILE__, __LINE__);
	}

	if (input_mode == OMX_COLOR_FormatYUV420Planar) {
		offset_y = 0;
		offset_u = (in_width * in_height + 4095) & 0xFFFFF000;
		offset_v = (offset_u + in_width * in_height / 4 + 4095) & 0xFFFFF000;
	} else if (input_mode == OMX_COLOR_FormatYUV420SemiPlanar) {
		offset_y = 0;
		offset_u = (in_width * in_height + 4095) & 0xFFFFF000;
		offset_v = -1;
	}

	{
		cap_offset[0] = offset_y;
		cap_offset[1] = offset_u;
		cap_offset[2] = offset_v;
		ret = OMX_SetParameter(omx_handle,
				omx_index_lombo_capture_plan_offsets, cap_offset);
		if (ret != OMX_ErrorNone)
			lombo_al_printf("set lombo_capture_plan_offsets error\n");
	}
	input_buffer_in_video_recorder = CAP_BUF_NUM;
}

static void use_capture_buf()
{
	int i = 0;
	int cap_buf_num = CAP_BUF_NUM;
	for (i = 0; i < cap_buf_num; i++)
		OMX_UseBuffer(omx_handle, &capture_omx_buf_header[i], input_port_index,
			NULL, capture_buf_size, capture_vir_addr[i]);

	for (i = 0; i < cap_buf_num; i++) {
		get_capture_data(capture_vir_addr[i]);
		capture_omx_buf_header[i]->nFilledLen = capture_buf_size;
		capture_omx_buf_header[i]->nTimeStamp = capture_time;
	}
}



static void alloc_stream_buf()
{
	int i = 0;
	int stream_buf_num = STREAM_BUF_NUM;
	for (i = 0; i < stream_buf_num; i++) {
		stream_vir_addr[i] = lombo_al_malloc(STREAM_BUF_SIZE, AL_MEM_UC,
				&(stream_phy_addr[i]), __FILE__, __LINE__);
		if (stream_vir_addr[i] == NULL)
			lombo_al_printf("malloc stream buffer error : %d\n", i);
		if (stream_vir_addr[i] == NULL)
			lombo_al_printf("malloc private buffer error : %d\n", i);
	}
	output_buffer_in_video_recorder = STREAM_BUF_NUM;
}

static void use_stream_buf()
{
	int i = 0;
	int stream_buf_num = STREAM_BUF_NUM;
	for (i = 0; i < stream_buf_num; i++) {
		OMX_UseBuffer(omx_handle, &stream_omx_buf_header[i], output_port_index,
			NULL, STREAM_BUF_SIZE, stream_vir_addr[i]);
	}
	for (i = 0; i < stream_buf_num; i++) {
		OMX_FillThisBuffer(omx_handle, stream_omx_buf_header[i]);
		output_buffer_in_video_recorder--;
	}

}

static void alloc_and_use_omxbuf()
{
	alloc_stream_buf();
	use_stream_buf();
	alloc_capture_buf();
	use_capture_buf();
}

static void free_buf(int port_index)
{

	if (port_index == input_port_index) {
		int i = 0;
		int cap_buf_num = CAP_BUF_NUM;

		for (i = 0; i < cap_buf_num; i++) {
			OMX_FreeBuffer(omx_handle, port_index,
				capture_omx_buf_header[i]);
			lombo_al_free(capture_vir_addr[i], AL_MEM_UC);
		}
		lombo_al_printf(" -- free_input_port\n");
	} else if (port_index == output_port_index) {
		int i = 0;
		int stream_buf_num = STREAM_BUF_NUM;
		for (i = 0; i < stream_buf_num; i++) {
			OMX_FreeBuffer(omx_handle, port_index,
				stream_omx_buf_header[i]);
			lombo_al_free(stream_vir_addr[i], AL_MEM_UC);
		}
		lombo_al_printf(" -- free_output_port\n");
	}



}

/*
argv[1]: 19 - 420planer
	   21 - 420semi

argv[2]: 0
*/

int video_recorder(int argc, char *argv[])
{
	FILE *flip = NULL;
	char file_name[1024];
	char *component_name = "OMX.LB.VIDEO.ENCODECOMPONENT";

	printf(" main\n");

	flip = fopen("/mnt/sdcard/movie/list.txt", "rb");
	if (flip == NULL) {
		printf("no list.txt\n");
		return -1;
	}


	/* omx_component_register(&omx_video_encode_component); */

	while (fgets((char *)file_name, 1024, flip) != NULL) {
		char fileout[1024] = "/mnt/sdcard/movie/raw/";
		char *p1 = NULL;
		char *p2 = NULL;
		int len = strlen(file_name);
		int imode;

		file_end_flag = 0;
		retry_cnt = 0;
		capture_time = 0;
		if ((file_name[len - 2] == '\r') && ((file_name[len - 1] == '\n')))
			file_name[len - 2] = '\0';

		p1 = strstr((char *)file_name, "/mnt/sdcard/movie/");
		p1 = p1 + strlen("/mnt/sdcard/movie/");
		p2 = strstr((char *)file_name, ".yuv");
		memcpy((char *)fileout + strlen((char *)fileout), p1, p2 - p1);
		strcat((char *)fileout, ".raw");

		printf(" -- filename : %s -- out file : %s\n", file_name, fileout);
		check_encode_mem();
		filp_in = fopen(file_name, "rb");
		if (filp_in == NULL) {
			printf(" open filp_in error\n ");
			continue;
		}

		filp_out_raw = fopen(fileout, "wb");
		if (filp_out_raw == NULL) {
			printf(" open filp_out_raw error\n");
			continue;
		}

		imode = atoi(argv[1]);
		slice_mode = atoi(argv[2]);

		if (imode == -1) {
			imode = input_mode;
			plan_num = input_mode_to_num_plan(imode);
		} else {
			plan_num = input_mode_to_num_plan(imode);
		}
		input_mode = imode;

		OMX_Init();
		lombo_al_sem_init(&omx_sem, 1);
		OMX_GetHandle(&omx_handle, component_name, NULL, &CallBacks);
		recorder_omx_state = OMX_StateLoaded;

		{
			int port_index = -1;
			OMX_ERRORTYPE ret = OMX_ErrorNone;
			for (port_index = 0; port_index < MAX_PORT_NUM; port_index++) {
				OMX_PARAM_PORTDEFINITIONTYPE port_def;
				port_def.nPortIndex = port_index;
				ret = OMX_GetParameter(omx_handle,
						OMX_IndexParamPortDefinition, &port_def);
				if (ret == OMX_ErrorNone) {
					if (port_def.eDir == OMX_DirInput)
						input_port_index = port_index;
					else
						output_port_index = port_index;
				} else if (ret == OMX_ErrorBadPortIndex) {
					total_port_num = port_index;
					break;
				} else {
					lombo_al_printf("getparameter error\n");
				}
			}
		}

		lombo_al_printf("input_port_index : %d\n", input_port_index);
		lombo_al_printf("output_port_index : %d\n", output_port_index);
		lombo_al_printf("total_port_num : %d\n", total_port_num);


		{
			OMX_PARAM_PORTDEFINITIONTYPE port_def;
			OMX_ERRORTYPE ret = OMX_ErrorNone;

			port_def.nPortIndex = input_port_index;
			ret = OMX_GetParameter(omx_handle,
					OMX_IndexParamPortDefinition, &port_def);
			if (ret == OMX_ErrorNone) {
				port_def.format.video.nFrameWidth = in_width;
				port_def.format.video.nFrameHeight = in_height;
				port_def.format.video.nStride = in_width;
				port_def.format.video.eColorFormat = input_mode;
				port_def.nBufferCountActual = CAP_BUF_NUM;
				ret = OMX_SetParameter(omx_handle,
						OMX_IndexParamPortDefinition, &port_def);
				if (ret != OMX_ErrorNone)
					lombo_al_printf("set input parm PortDef error\n");
			} else {
				lombo_al_printf("get input parm PortDef error\n");
			}

			port_def.nPortIndex = output_port_index;
			ret = OMX_GetParameter(omx_handle,
					OMX_IndexParamPortDefinition, &port_def);
			if (ret == OMX_ErrorNone) {
				port_def.format.video.nBitrate = video_bitrate;
				port_def.format.video.nFrameWidth = out_width;
				port_def.format.video.nFrameHeight = out_height;
				port_def.nBufferCountActual = STREAM_BUF_NUM;
				ret = OMX_SetParameter(omx_handle,
						OMX_IndexParamPortDefinition, &port_def);
				if (ret != OMX_ErrorNone)
					lombo_al_printf("set output parm PortDef err\n");
			} else {
				lombo_al_printf("get output parm PortDef error\n");
			}

			ret = OMX_SetParameter(omx_handle,
					omx_index_lombo_venc_slicemode, &slice_mode);
			if (ret != OMX_ErrorNone)
				lombo_al_printf("set venc_slicemode error\n");

			alloc_and_use_omxbuf();

			lombo_al_sem_wait(&omx_sem);
			OMX_SendCommand(omx_handle,
				OMX_CommandStateSet, OMX_StateIdle, NULL);
		}

			OMX_TIME_CONFIG_TIMESTAMPTYPE time_ref;
			time_ref.nTimestamp = 0;
			video_encoder_set_config(omx_handle,
				omx_index_lombo_config_cur_time, &time_ref);

			lombo_al_sem_wait(&omx_sem);
			OMX_SendCommand(omx_handle,
			OMX_CommandStateSet, OMX_StateExecuting, NULL);
			lombo_al_sem_wait(&omx_sem);

		{
			int i = 0;
			int cap_buf_num = CAP_BUF_NUM;
			for (i = 0; i < cap_buf_num; i++) {
				OMX_EmptyThisBuffer(omx_handle,
					capture_omx_buf_header[i]);
				input_buffer_in_video_recorder--;
			}
		}


		while (1) {
			lombo_al_printf("encoding\n");
			if (file_end_flag == 1)
				break;
			oscl_mdelay(100);
		}


		lombo_al_printf("encode end\n");


		OMX_SendCommand(omx_handle, OMX_CommandStateSet, OMX_StateIdle, NULL);
		lombo_al_sem_wait(&omx_sem);
		while (1) {
			if ((input_buffer_in_video_recorder == CAP_BUF_NUM)
				&& (output_buffer_in_video_recorder == STREAM_BUF_NUM)) {
				int port_index;
				for (port_index = 0; port_index < total_port_num;
					port_index++)
					free_buf(port_index);
				break;
			} else {
				oscl_mdelay(100);
				lombo_al_printf("wait buffer - in : %d - out : %d\n",
					input_buffer_in_video_recorder,
					output_buffer_in_video_recorder);
			}
		}

		OMX_SendCommand(omx_handle, OMX_CommandStateSet, OMX_StateLoaded, NULL);
		lombo_al_sem_wait(&omx_sem);
		OMX_FreeHandle(omx_handle);
		lombo_al_sem_del(&omx_sem);
		OMX_Deinit();
		fclose(filp_in);
		fclose(filp_out_raw);

	}

	fclose(flip);
	printf(" encode end end\n");
	check_encode_mem();
	return 0;
}

MSH_CMD_EXPORT(video_recorder, "video_recorder");
