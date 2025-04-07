#define DBG_LEVEL         DBG_ERR

#include <oscl.h>
#include <base_component.h>
#include "slib.h"
#include "demuxer_component.h"

/* #define DYNAMIC_LOADING */
#define SINK_OUTPUT_DEFAULT_SIZE (16*1024)
#define AUDIO_NUM 32
#define VIDEO_NUM 32
#define BUF_ALIGN 0x1000
#define OMX_VIDEO_BUF_SIZE (1024*1024)

static int demuxer_init(OMX_IN OMX_HANDLETYPE cmp_handle)
{
	demuxer_component_private_t *demuxer_private = NULL;
	lb_omx_component_t *component = (lb_omx_component_t *)cmp_handle;
	base_port_t *aport;
	base_port_t *vport;

	demuxer_private = (demuxer_component_private_t *)(component->component_private);
	memset(&demuxer_private->medie_info, 0, sizeof(media_info_t));
#ifdef DYNAMIC_LOADING
	char shared_lib[100] = "\0";
	demuxer_plugin_t *(*func_get_plugin_info)(void);
	char *lib_suffix = ".so";
	char *plugin_name = "avd_";

	strcat(shared_lib, plugin_name);
	switch (demuxer_private->role_type) {
	case MP4_DEMUXER:
		strcat(shared_lib, "mp4");
		break;
	default:
		OSCL_LOGE("not support the codec type\n");
		break;
	}

	strcat(shared_lib, lib_suffix);
	OSCL_LOGE("shared_lib : %s\n", shared_lib);

	if (demuxer_private->lib_handle != NULL) {
		unload_library(demuxer_private->lib_handle);
		demuxer_private->lib_handle = NULL;
	}
	if (demuxer_private->lib_handle == NULL) {
		demuxer_private->lib_handle = load_library(shared_lib);
		if (demuxer_private->lib_handle == NULL) {
			OSCL_LOGE("dlopen lib error\n");
			return -1;
		}
		func_get_plugin_info = get_library_entry(demuxer_private->lib_handle,
				"get_plugin_info");
		if (func_get_plugin_info == NULL) {
			OSCL_LOGE("get_library_entry error\n");
			return -1;
		}
		demuxer_private->demuxer_plugin =
			(demuxer_plugin_t *)func_get_plugin_info();
		if (demuxer_private->demuxer_plugin == NULL) {
			OSCL_LOGE("func_get_plugin_info error\n");
			return -1;
		}
		if (demuxer_private->handle == NULL) {
			demuxer_private->handle = demuxer_private->demuxer_plugin->open(
					demuxer_private->src_file_name,
					&(demuxer_private->medie_info));
		}
		if (demuxer_private->handle == NULL) {
			OSCL_LOGE("video_handle error\n");
			return -1;
		}
	}
#else
	demuxer_private->handle = demuxer_open(demuxer_private->src_file_name,
			&(demuxer_private->medie_info));
	if (demuxer_private->handle == NULL) {
		OSCL_LOGE("demuxer_open error\n");
		return -1;
	}
#endif
	if (demuxer_private->medie_info.audio_streams) {
		int fmt_detected = 1;
		aport = &component->port[AUDIO_PORT];
		switch (demuxer_private->medie_info.demuxer_audio.codec_id) {
		case AV_CODEC_TYPE_AAC:
			aport->port_param.format.audio.eEncoding = OMX_AUDIO_CodingAAC;
			break;
		case AV_CODEC_TYPE_PCM_S16LE:
		case AV_CODEC_TYPE_PCM_S16BE:
		case AV_CODEC_TYPE_PCM_U16LE:
		case AV_CODEC_TYPE_PCM_U16BE:
		case AV_CODEC_TYPE_PCM_S8:
		case AV_CODEC_TYPE_PCM_U8:
		case AV_CODEC_TYPE_PCM_MULAW:
		case AV_CODEC_TYPE_PCM_ALAW:
		case AV_CODEC_TYPE_PCM_S32LE:
		case AV_CODEC_TYPE_PCM_S32BE:
		case AV_CODEC_TYPE_PCM_U32LE:
		case AV_CODEC_TYPE_PCM_U32BE:
		case AV_CODEC_TYPE_PCM_S24LE:
		case AV_CODEC_TYPE_PCM_S24BE:
		case AV_CODEC_TYPE_PCM_U24LE:
		case AV_CODEC_TYPE_PCM_U24BE:
			aport->port_param.format.audio.eEncoding = OMX_AUDIO_CodingPCM;
			break;
		case AV_CODEC_TYPE_ADPCM_IMA_WAV:
			aport->port_param.format.audio.eEncoding = OMX_AUDIO_CodingADPCM;
			break;
		default:
			fmt_detected = 0;
			OSCL_LOGI("not supported ecode format\n");
			break;
		}
		OSCL_LOGI("audio eEncoding:%d", aport->port_param.format.audio.eEncoding);

		if (fmt_detected) {
			/* notify FormatDetected */
			(*(component->callbacks.EventHandler))
			(&component->base_comp,
				component->callback_data,
				OMX_EventPortFormatDetected,
				OMX_IndexParamAudioPortFormat,
				AUDIO_PORT,
				NULL);
			/* notify portsetting changed */
			(*(component->callbacks.EventHandler))
			(&component->base_comp,
				component->callback_data,
				OMX_EventPortSettingsChanged,
				0,
				AUDIO_PORT,
				NULL);
		}
	}
	if (demuxer_private->medie_info.video_streams) {
		int video_detected = 1;
		vport = &component->port[VIDEO_PORT];

		switch (demuxer_private->medie_info.demuxer_video.codec_id) {
		case AV_CODEC_TYPE_H264:
			vport->port_param.format.video.eCompressionFormat =
				OMX_VIDEO_CodingAVC;
			vport->port_param.format.video.nFrameWidth =
				demuxer_private->medie_info.demuxer_video.width;
			vport->port_param.format.video.nFrameHeight =
				demuxer_private->medie_info.demuxer_video.height;
			break;
		case AV_CODEC_TYPE_H263:
		case AV_CODEC_TYPE_MPEG1VIDEO:
		case AV_CODEC_TYPE_MPEG2VIDEO:
		case AV_CODEC_TYPE_MPEG4:
		case AV_CODEC_TYPE_VC1:
		case AV_CODEC_TYPE_HEVC:
		case AV_CODEC_TYPE_INVALID:
			video_detected = 0;
			OSCL_LOGE("video fmt %d not support\n",
				demuxer_private->medie_info.demuxer_video.codec_id);
			((component->callbacks.EventHandler))
				(&component->base_comp,
				component->callback_data, OMX_EventError,
				-1, -1, NULL);
			return -1;
		default:
			video_detected = 0;
			OSCL_LOGI("format not supportted yet\n");
			break;
		}
		if (video_detected) {
			/* notify FormatDetected */
			(*(component->callbacks.EventHandler))
			(&component->base_comp,
				component->callback_data,
				OMX_EventPortFormatDetected,
				OMX_IndexParamVideoPortFormat,
				VIDEO_PORT,
				NULL);
			/* notify portsetting changed */
			(*(component->callbacks.EventHandler))
			(&component->base_comp,
				component->callback_data,
				OMX_EventPortSettingsChanged,
				0,
				VIDEO_PORT,
				NULL);
		}
	}
	return 0;
}

static int demuxer_deinit(OMX_IN OMX_HANDLETYPE cmp_handle)
{
	demuxer_component_private_t *demuxer_private = NULL;
	lb_omx_component_t *component = (lb_omx_component_t *)cmp_handle;

	demuxer_private = (demuxer_component_private_t *)(component->component_private);

	demuxer_private->audio_extra = 0;
	demuxer_private->video_extra = 0;
	demuxer_private->flame_flag = 0;

#ifdef DYNAMIC_LOADING
	if (demuxer_private->handle != NULL)
		demuxer_private->demuxer_plugin->dispose(demuxer_private->handle);
	if (demuxer_private->lib_handle != NULL)
		unload_library(demuxer_private->lib_handle);
#else
	demuxer_dispose(&demuxer_private->handle);
#endif
	return 0;
}

OMX_ERRORTYPE demuxer_get_parameter(OMX_IN OMX_HANDLETYPE     hComp,
	OMX_IN OMX_INDEXTYPE    paramIndex,
	OMX_INOUT OMX_PTR       paramData)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	lb_omx_component_t *component;
	demuxer_component_private_t *demuxer_private = NULL;

	component = get_lb_component(hComp);
	demuxer_private = (demuxer_component_private_t *)(component->component_private);

	switch ((OMX_U32)paramIndex) {
	case omx_index_vendor_get_port_number: {
		*((int *)paramData) = component->num_ports;
	}
	break;
	case omx_index_media_duration:
		*((long *)paramData) = (long)demuxer_private->medie_info.duration;
		break;
	case OMX_IndexConfigTimePosition: {
		OMX_TIME_CONFIG_TIMESTAMPTYPE *timestamptype =
			(OMX_TIME_CONFIG_TIMESTAMPTYPE *)paramData;
		timestamptype->nTimestamp = demuxer_private->video_time;
	}
	break;
	case OMX_IndexParamAudioPcm: {
		OMX_AUDIO_PARAM_PCMMODETYPE *audio_params;
		audio_params = (OMX_AUDIO_PARAM_PCMMODETYPE *)paramData;
		audio_params->nChannels =
			demuxer_private->medie_info.demuxer_audio.channels;
		audio_params->nBitPerSample =
			demuxer_private->medie_info.demuxer_audio.bits_per_coded_sample;
		audio_params->nSamplingRate =
			demuxer_private->medie_info.demuxer_audio.sample_rate;
	}
	break;
	case OMX_IndexParamAudioAac: {
		OMX_AUDIO_PARAM_AACPROFILETYPE *audio_params;
		audio_params = (OMX_AUDIO_PARAM_AACPROFILETYPE *)paramData;
		audio_params->nBitRate =
			demuxer_private->medie_info.demuxer_audio.bit_rate;
		audio_params->nChannels =
			demuxer_private->medie_info.demuxer_audio.channels;
		audio_params->nSampleRate =
			demuxer_private->medie_info.demuxer_audio.sample_rate;
	}
	break;
	default:
		ret = base_get_parameter(hComp, paramIndex, paramData);
		if (ret != OMX_ErrorNone)
			OSCL_LOGE("base get parameter %d error\n", paramIndex);
		break;
	}

	return ret;
}

OMX_ERRORTYPE demuxer_set_parameter(OMX_IN OMX_HANDLETYPE  hComp,
	OMX_IN OMX_INDEXTYPE  paramIndex,
	OMX_IN OMX_PTR        paramData)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	lb_omx_component_t *component;
	demuxer_component_private_t *demuxer_private = NULL;

	component = get_lb_component(hComp);
	demuxer_private = (demuxer_component_private_t *)(component->component_private);

	switch ((OMX_U32)paramIndex) {
	case omx_index_vendor_input_filename: {
		char *input_file = (char *)paramData;
		int file_size = strlen(input_file) + 1;
		if (file_size > MAX_FILE_SIZE) {
			OSCL_LOGE("the file name size can not be too long\n");
			return OMX_ErrorOverflow;
		}
		memcpy(demuxer_private->src_file_name, input_file, file_size);
		OSCL_LOGI("demuxer_private->src_file_name : %s\n",
			demuxer_private->src_file_name);
	}
	break;
	case OMX_IndexConfigTimePosition: {
		OMX_TIME_CONFIG_TIMESTAMPTYPE *timestamptype =
			(OMX_TIME_CONFIG_TIMESTAMPTYPE *)paramData;

		pthread_mutex_lock(&demuxer_private->demuxer_mutex);
		demuxer_private->seekflag = 1;
		demuxer_private->nTimestamp = timestamptype->nTimestamp;
		pthread_mutex_unlock(&demuxer_private->demuxer_mutex);
	}
	break;
	default:
		ret = base_set_parameter(hComp, paramIndex, paramData);
		if (ret != OMX_ErrorNone)
			OSCL_LOGE("base set parameter %d error\n", paramIndex);
		break;
	}
	return ret;
}

OMX_ERRORTYPE demuxer_set_state(OMX_HANDLETYPE hComp,
	OMX_U32 dest_state)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	demuxer_component_private_t *demuxer_private = NULL;
	lb_omx_component_t *component;
	OMX_STATETYPE pre_state;

	oscl_param_check(hComp != NULL, OMX_ErrorBadParameter, NULL);
	component = get_lb_component(hComp);
	demuxer_private = component->component_private;

	if (dest_state == OMX_StateExecuting && component->state == OMX_StateIdle) {
		if (demuxer_private->lib_init_flag == 0) {
			ret = demuxer_init(component);
			if (ret != 0) {
				OSCL_LOGE("demuxer_init error\n");
				return ret;
			}
			demuxer_private->lib_init_flag = 1;
		} else {
			OSCL_LOGE("lib has been opened, need debug\n");
		}
		component->eos_flag = OMX_FALSE;
	}

	if (dest_state == OMX_StateExecuting && component->state == OMX_StatePause)
		OSCL_LOGD("port0 buffer %d, port1 %d\n",
			component->port[0].buffer_queue.count,
			component->port[1].buffer_queue.count);

	pre_state = component->state;
	ret = base_component_set_state(hComp, dest_state);

	if (dest_state == OMX_StateIdle &&
		(pre_state == OMX_StateExecuting || pre_state == OMX_StatePause)) {
		if (demuxer_private->lib_init_flag == 1) {
			ret = demuxer_deinit(component);
			if (ret != 0) {
				OSCL_LOGE("demuxer deinit error\n");
				return ret;
			}
			demuxer_private->lib_init_flag = 0;
		} else {
			OSCL_LOGE("lib has been disposed, need debug\n");
		}
		OSCL_LOGW("demuxer exit\n");
	}
	return ret;
}

void demuxer_buffer_handle(OMX_HANDLETYPE cmp_handle,
	OMX_BUFFERHEADERTYPE *inbuffer,
	OMX_BUFFERHEADERTYPE *outbuffer)
{
	demuxer_component_private_t *demuxer_private = NULL;
	lb_omx_component_t *component = (lb_omx_component_t *)cmp_handle;
	packet_t out_buf;
	demuxer_video_t *demuxer_video;
	demuxer_audio_t *demuxer_audio;

	demuxer_private = (demuxer_component_private_t *)(component->component_private);
	demuxer_video = &demuxer_private->medie_info.demuxer_video;
	demuxer_audio = &demuxer_private->medie_info.demuxer_audio;

	if (demuxer_private->video_extra == 0) {
		if ((demuxer_video->extradata_size != 0) &&
			(outbuffer->nOutputPortIndex == VIDEO_PORT)) {
			memcpy(outbuffer->pBuffer, demuxer_video->extradata,
				demuxer_video->extradata_size);
			outbuffer->nFilledLen = demuxer_video->extradata_size;
			outbuffer->nFlags = OMX_BUFFERFLAG_CODECCONFIG;
			demuxer_private->video_extra = 1;
			OSCL_LOGI("video extradata size:%d", outbuffer->nFilledLen);
			return;
		}
	}

	if (demuxer_private->audio_extra == 0) {
		if ((demuxer_audio->extradata_size != 0) &&
			(outbuffer->nOutputPortIndex == AUDIO_PORT)) {
			memcpy(outbuffer->pBuffer, demuxer_audio->extradata,
				demuxer_audio->extradata_size);
			outbuffer->nFilledLen = demuxer_audio->extradata_size;
			outbuffer->nFlags = OMX_BUFFERFLAG_CODECCONFIG;
			demuxer_private->audio_extra = 1;
			OSCL_LOGI("audio extradata size:%d", outbuffer->nFilledLen);
			return;
		}
	}

	pthread_mutex_lock(&demuxer_private->demuxer_mutex);

	memset(&out_buf, 0, sizeof(packet_t));
	out_buf.data = outbuffer->pBuffer;
	out_buf.size = outbuffer->nFilledLen;
	out_buf.flags = outbuffer->nFlags;
	out_buf.stream_index = outbuffer->nOutputPortIndex;

	if (demuxer_private->handle != NULL) {
		demuxer_read(demuxer_private->handle, &out_buf);
		outbuffer->nFlags = out_buf.flags;
	}

	pthread_mutex_unlock(&demuxer_private->demuxer_mutex);
}

void *demuxer_buffer_manager(void *param)
{
	lb_omx_component_t *component;
	demuxer_component_private_t *demuxer_private = NULL;
	demuxer_video_t *demuxer_video;
	demuxer_audio_t *demuxer_audio;
	packet_t out_buf = {0};
	base_port_t *outport;
	OMX_BUFFERHEADERTYPE *outbuffer = NULL;
	int port_index = 0;
	OSCL_TRACE(" %p\n", param);
	oscl_param_check((param != NULL), NULL, NULL);
	component = get_lb_component(param);
	demuxer_private = (demuxer_component_private_t *)(component->component_private);
	demuxer_video = &demuxer_private->medie_info.demuxer_video;
	demuxer_audio = &demuxer_private->medie_info.demuxer_audio;
	outport = &component->port[0];
	component->dbg_flag = set_debug_state(component->dbg_flag,
			DEBUG_BUF_MGNT_SHT, DEBUG_THREAD_START);

	OSCL_LOGW("%s buffer_manager:%s\n", component->name, rt_thread_self()->name);

	/* checks if the component is in a state able to receive buffers */
	while (component->state == OMX_StateIdle
		|| component->state == OMX_StateExecuting
		|| component->state == OMX_StatePause) {
		component->dbg_wdog = 0;

		/*Wait till the ports are being flushed*/
		pthread_mutex_lock(&component->flush_mutex);
		while (outport->is_flushed) {
			pthread_mutex_unlock(&component->flush_mutex);
			if (outbuffer != NULL) {
				outport->return_buffer(outport, outbuffer);
				outbuffer = NULL;
				OSCL_LOGW("retrun buffer while flushing port");
			}
			sem_post(component->mgmt_flush_sem);
			sem_wait(component->flush_sem);
			pthread_mutex_lock(&component->flush_mutex);
			OSCL_LOGW("port0 count %d, port1 cnt %d, state %d\n",
				component->port[0].buffer_queue.count,
				component->port[1].buffer_queue.count,
				component->state);
		}
		pthread_mutex_unlock(&component->flush_mutex);

		if (component->state != OMX_StateExecuting) {
			OSCL_LOGD("==========%d\n", component->buf_mgnt_sem->sem->value);
			sem_wait(component->buf_mgnt_sem);
			continue;
		}
		if (component->eos_flag) {
			if (((component->eos_flag & (1 << AUDIO_PORT)) ||
					!demuxer_private->medie_info.audio_streams) &&
				((component->eos_flag & (1 << VIDEO_PORT)) ||
					!demuxer_private->medie_info.video_streams)) {
				oscl_mdelay(10);
				continue;
			}
		}

		if (demuxer_private->seekflag == 1) {
#ifdef DYNAMIC_LOADING
			if (demuxer_private->handle != NULL)
				demuxer_private->demuxer_plugin->seek(
					demuxer_private->handle,
					demuxer_private->nTimestamp);
#else
			if (demuxer_private->handle != NULL) {
				OSCL_LOGE("seek:%lld", demuxer_private->nTimestamp);
				demuxer_seek(demuxer_private->handle,
					demuxer_private->nTimestamp);

				OSCL_LOGE("seek:end");
			} else {
				OSCL_LOGE("seek:demuxer_private->handle = NULL!");
			}

#endif
			demuxer_private->flame_flag = 0;
			demuxer_private->seekflag = 0;
		}

		/* extradata */
		if ((demuxer_private->audio_extra == 0) ||
			(demuxer_private->video_extra == 0)) {
			if (demuxer_private->flame_flag == 0) {
				if (demuxer_private->video_extra == 0) {
					if (demuxer_video->extradata_size != 0) {
						port_index = VIDEO_PORT;
						demuxer_private->flame_flag = 1;
					} else
						demuxer_private->video_extra = 1;
				}
			}

			if (demuxer_private->flame_flag == 0) {
				if (demuxer_private->audio_extra == 0) {
					if (demuxer_audio->extradata_size != 0) {
						port_index = AUDIO_PORT;
						demuxer_private->flame_flag = 1;
					} else
						demuxer_private->audio_extra = 1;
				}
			}
		}

		if (demuxer_private->flame_flag == 0) {
			memset(&out_buf, 0, sizeof(out_buf));
			port_index = demuxer_frame(demuxer_private->handle, &out_buf);
		}
		if ((port_index >= 0) && (port_index < OTHER_PORT)) {
			demuxer_private->flame_flag = 1;
			outport = &component->port[port_index];
		} else {
			OSCL_LOGE("err por_index:%d\n", port_index);
			break;
		}

		if (outbuffer == NULL)
			outbuffer = oscl_queue_dequeue(&outport->buffer_queue);

		if (outbuffer == NULL) {
			OSCL_LOGD("waiting buffer:%d\n",
				component->buf_mgnt_sem->sem->value);
			sem_wait(component->buf_mgnt_sem);
			continue;
		}

		if (component->state == OMX_StateExecuting) {
			if (component->buf_handle) {
				outbuffer->nFilledLen = out_buf.size;
				outbuffer->nFlags = out_buf.flags;
				outbuffer->nTimeStamp = out_buf.pts;
				if (outbuffer->nAllocLen < out_buf.size) {
					outbuffer->nFilledLen = outbuffer->nAllocLen;
					out_buf.size -= outbuffer->nAllocLen;
					outbuffer->nFlags &= ~OMX_BUFFERFLAG_ENDOFFRAME;
				} else {
					outbuffer->nFilledLen = out_buf.size;
					demuxer_private->flame_flag = 0;
					outbuffer->nFlags |= OMX_BUFFERFLAG_ENDOFFRAME;
				}
				(component->buf_handle)(component, NULL, outbuffer);
			}
		}

		if ((outbuffer->nFlags & OMX_BUFFERFLAG_EOS) == OMX_BUFFERFLAG_EOS) {
			OSCL_LOGW("Detected EOS flags, filled len=%d\n",
				(int)outbuffer->nFilledLen);
			(*(component->callbacks.EventHandler))
			(&component->base_comp,
				component->callback_data,
				OMX_EventBufferFlag, /* The command was completed */
				outport->port_param.nPortIndex,
				outbuffer->nFlags,
				NULL);
			component->eos_flag |=  1 << outbuffer->nOutputPortIndex;
			OSCL_LOGI("eos:%d", component->eos_flag);
		}

		if ((outbuffer->nFilledLen != 0)
			|| (outbuffer->nFlags & OMX_BUFFERFLAG_EOS)) {
			outport->return_buffer(outport, outbuffer);
			outbuffer = NULL;
		}
	}
	oscl_queue_flush(&component->port[AUDIO_PORT].buffer_queue);
	oscl_queue_flush(&component->port[VIDEO_PORT].buffer_queue);
	OSCL_LOGW("exit from buffer_manager:%s", rt_thread_self()->name);
	OSCL_LOGW("exit, buf left port0 count %d, port1 cnt %d\n",
		component->port[0].buffer_queue.count,
		component->port[1].buffer_queue.count);
	OSCL_TRACE(" %p\n", param);
	component->dbg_flag = set_debug_state(component->dbg_flag,
			DEBUG_BUF_MGNT_SHT, DEBUG_THREAD_EXIT);
	return NULL;
}



OMX_ERRORTYPE demuxer_component_deinit(OMX_IN OMX_HANDLETYPE hComp)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	lb_omx_component_t *component;
	demuxer_component_private_t *demuxer_private = NULL;

	component = get_lb_component(hComp);
	demuxer_private = (demuxer_component_private_t *)(component->component_private);

	pthread_mutex_destroy(&demuxer_private->demuxer_mutex);

	if (component->component_private != NULL) {
		oscl_free(component->component_private);
		component->component_private = NULL;
	}
	base_port_deinit(&component->port[AUDIO_PORT]);
	base_port_deinit(&component->port[VIDEO_PORT]);
	ret = base_component_deinit(hComp);
	if (ret != OMX_ErrorNone)
		OSCL_LOGE("base_component_deinit error\n");
	return ret;
}


OMX_ERRORTYPE demuxer_component_init(lb_omx_component_t *cmp_handle)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	demuxer_component_private_t *demuxer_private = NULL;
	OMX_COMPONENTTYPE *base_cmp = &(cmp_handle->base_comp);

	ret = base_component_init(cmp_handle);
	if (ret != OMX_ErrorNone) {
		OSCL_LOGE(" base init component error\n");
		return ret;
	}

	demuxer_private = oscl_zalloc(sizeof(demuxer_component_private_t));
	if (demuxer_private == NULL) {
		OSCL_LOGE("malloc demuxer_private error\n");
		ret = OMX_ErrorInsufficientResources;
		goto EXIT;
	}
	memset(demuxer_private, 0, sizeof(demuxer_component_private_t));

	pthread_mutex_init(&demuxer_private->demuxer_mutex, NULL);

	cmp_handle->name = "OMX.LB.SOURCE.DEMUXER";
	cmp_handle->component_private = (OMX_PTR)demuxer_private;
	cmp_handle->buf_manager = demuxer_buffer_manager;
	cmp_handle->buf_handle = demuxer_buffer_handle;
	cmp_handle->do_state_set = demuxer_set_state;
	cmp_handle->num_ports = 2;

	ret = base_port_init(cmp_handle, &cmp_handle->port[AUDIO_PORT],
			AUDIO_PORT,
			OMX_DirOutput);
	if (ret != OMX_ErrorNone) {
		oscl_free(demuxer_private);
		base_component_deinit(cmp_handle);
		return ret;
	}
	ret = base_port_init(cmp_handle, &cmp_handle->port[VIDEO_PORT],
			VIDEO_PORT,
			OMX_DirOutput);
	if (ret != OMX_ErrorNone) {
		oscl_free(demuxer_private);
		base_component_deinit(cmp_handle);
		cmp_handle->port[AUDIO_PORT].deinit(
			&cmp_handle->port[AUDIO_PORT]);
		return ret;
	}
	cmp_handle->port[AUDIO_PORT].port_param.nBufferSize =
		SINK_OUTPUT_DEFAULT_SIZE;
	cmp_handle->port[AUDIO_PORT].port_param.eDomain =
		OMX_PortDomainAudio;
	cmp_handle->port[AUDIO_PORT].port_param.nBufferAlignment =
		BUF_ALIGN;
	cmp_handle->port[AUDIO_PORT].port_param.bBuffersContiguous =
		OMX_TRUE;
	cmp_handle->port[AUDIO_PORT].port_param.nBufferCountActual =
		AUDIO_NUM;
	cmp_handle->port[VIDEO_PORT].port_param.nBufferSize =
		OMX_VIDEO_BUF_SIZE;
	cmp_handle->port[VIDEO_PORT].port_param.eDomain =
		OMX_PortDomainVideo;
	cmp_handle->port[VIDEO_PORT].port_param.nBufferAlignment =
		BUF_ALIGN;
	cmp_handle->port[VIDEO_PORT].port_param.bBuffersContiguous =
		OMX_TRUE;
	cmp_handle->port[VIDEO_PORT].port_param.nBufferCountActual =
		VIDEO_NUM;

	base_cmp->SetParameter        = demuxer_set_parameter;
	base_cmp->GetParameter        = demuxer_get_parameter;
	base_cmp->ComponentDeInit     = demuxer_component_deinit;

	return ret;

EXIT:

	return ret;
}
