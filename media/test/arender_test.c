#define DBG_LEVEL		DBG_INFO /* DBG_INFO */

#include <oscl.h>
#include <base_component.h>
#include <lb_omx_core.h>
#include <omx_test.h>
#include "arender_component.h"

#define DEFAULT_ARENDER_BUF_CNT		4
#define AUDIO_INBUF_DEFAULT_SIZE	16384

/* RIFF HEADER of wav file */
struct RIFF_HEADER_DEF {
	char riff_id[4];     /* 'R', 'I', 'F', 'F' */
	uint32_t riff_size;
	char riff_format[4]; /* 'W', 'A', 'V', 'E' */
};

struct WAVE_FORMAT_DEF {
	uint16_t format_tag;
	uint16_t channels; /* channels of the audio stream */
	uint32_t samples_per_sec; /* sample rate */
	uint32_t avg_bytes_per_sec;
	uint16_t block_align;
	uint16_t bits_per_sample; /* bits per sample */
};

struct FMT_BLOCK_DEF {
	char fmt_id[4];    /* 'f', 'm', 't', ' ' */
	uint32_t fmt_size;
	struct WAVE_FORMAT_DEF wav_format;
};

struct DATA_BLOCK_DEF {
	char data_id[4];     /* 'R', 'I', 'F', 'F' */
	uint32_t data_size;
};

struct wav_info {
	struct RIFF_HEADER_DEF header;
	struct FMT_BLOCK_DEF   fmt_block;
	struct DATA_BLOCK_DEF  data_block;
};

typedef struct aplayer_test {
	OMX_COMPONENTTYPE *sink;

	OMX_PARAM_PORTDEFINITIONTYPE port_para;
	oscl_queue_t buffer_queue;

	sem_t sem_event;
	sem_t sem_eos;
	sem_t sem_buf;

	OMX_S32 buf_num;
	OMX_BUFFERHEADERTYPE **buffer;
	int stat;
	int nbuf_done;

	FILE *infile;
	struct wav_info audio_info;
	int read_thread_exit;
} aplayer_test_t;

static aplayer_test_t aplayer_priv;
static OMX_U32 error_value = 0u;

/* Callbacks implementation */
static OMX_ERRORTYPE event_handler(
	OMX_HANDLETYPE hComponent,
	OMX_PTR pAppData,
	OMX_EVENTTYPE eEvent,
	OMX_U32 Data1,
	OMX_U32 Data2,
	OMX_PTR pEventData)
{
	aplayer_test_t *private = (aplayer_test_t *)pAppData;
	char *name;

	OSCL_TRACE("hComponent:%p", hComponent);
	if (hComponent == private->sink)
		name = "sink";
	if (eEvent == OMX_EventCmdComplete) {
		if (Data1 == OMX_CommandStateSet) {
			switch ((int)Data2) {
			case OMX_StateInvalid:
				OSCL_LOGI("%s StateSet OMX_StateInvalid", name);
				break;
			case OMX_StateLoaded:
				OSCL_LOGI("%s StateSet OMX_StateLoaded", name);
				break;
			case OMX_StateIdle:
				OSCL_LOGI("%s StateSet OMX_StateIdle", name);
				break;
			case OMX_StateExecuting:
				OSCL_LOGI("%s StateSet OMX_StateExecuting", name);
				break;
			case OMX_StatePause:
				OSCL_LOGI("%s StateSet OMX_StatePause", name);
				break;
			case OMX_StateWaitForResources:
				OSCL_LOGI("%s StateSet WaitForResources", name);
				break;
			default:
				OSCL_LOGE("%s StateSet unkown state", name);
				break;
			}
			sem_post(&private->sem_event);
		} else if (Data1 == OMX_CommandPortEnable) {
			OSCL_LOGI("%s CmdComplete OMX_CommandPortEnable", name);
			sem_post(&private->sem_event);
		} else if (Data1 == OMX_CommandPortDisable) {
			OSCL_LOGI("%s CmdComplete OMX_CommandPortDisable", name);
			sem_post(&private->sem_event);
		}
	} else if (eEvent == OMX_EventBufferFlag) {
		if ((int)Data2 == OMX_BUFFERFLAG_EOS) {
			OSCL_LOGI("%s BufferFlag OMX_BUFFERFLAG_EOS", name);
			if (hComponent == private->sink) {
				OSCL_LOGE("end of sink component");
				sem_post(&private->sem_eos);
			}
		} else
			OSCL_LOGI("%s OMX_EventBufferFlag %x", name, Data2);
	}  else if (eEvent == OMX_EventError) {
		error_value = Data1;
		OSCL_LOGE("Receive error event. value:%x", error_value);
		sem_post(&private->sem_event);
	} else
		OSCL_LOGI("%s parm:%x %x", name, Data1, Data2);
	return OMX_ErrorNone;
}

static OMX_ERRORTYPE untunnel_empty_buffer_done(
	OMX_HANDLETYPE hComponent,
	OMX_PTR pAppData,
	OMX_BUFFERHEADERTYPE *pBuffer)
{
	OMX_ERRORTYPE err;
	aplayer_test_t *priv = pAppData;

	if (pBuffer == NULL || pAppData == NULL) {
		OSCL_LOGE("err: buffer header is null");
		return OMX_ErrorBadParameter;
	}
	OSCL_LOGD("empty buffer done>> %p, %d, input:%d output:%d", pBuffer->pBuffer,
		pBuffer->nFlags, pBuffer->nInputPortIndex, pBuffer->nOutputPortIndex);

	err = oscl_queue_queue(&priv->buffer_queue, pBuffer);
	if (err != OMX_ErrorNone)
		OSCL_LOGE("queue buffer err: %x", err);

	return err;
}

static OMX_CALLBACKTYPE untunnel_callbacks = {
	.EventHandler = event_handler,
	.EmptyBufferDone = untunnel_empty_buffer_done,
	.FillBufferDone = NULL,
};

static OMX_ERRORTYPE alloc_buffer(aplayer_test_t *priv, OMX_S32 num, OMX_S32 size)
{
	OMX_S32 i = 0;
	OMX_S32 port_index = BASE_INPUT_PORT;
	OMX_ERRORTYPE ret = OMX_ErrorNone;

	priv->buffer = oscl_zalloc(num * sizeof(OMX_BUFFERHEADERTYPE *));
	if (NULL == priv->buffer)
		return OMX_ErrorBadParameter;
	for (i = 0; i < num; i++) {
		priv->buffer[i] = NULL;
		ret = OMX_AllocateBuffer(priv->sink, &priv->buffer[i],
				port_index, priv, size);
		OSCL_LOGI("AllocateBuffer %p on port %i", priv->buffer[i], port_index);
		if (ret != OMX_ErrorNone) {
			OSCL_LOGE("Error on AllocateBuffer %p on port %i\n",
				&priv->buffer[i], port_index);
			break;
		}
		ret = oscl_queue_queue(&priv->buffer_queue, priv->buffer[i]);
		if (ret != 0) {
			OSCL_LOGE("queue buffer %d error!\n", i);
			break;
		}
	}

	return ret;
}

static void free_buffer(aplayer_test_t *priv)
{
	OMX_S32 i = 0;
	OMX_ERRORTYPE ret = OMX_ErrorNone;

	if (priv->buffer) {
		for (i = 0; i < priv->buf_num; i++) {
			if (priv->buffer[i]) {
				ret = OMX_FreeBuffer(priv->sink,
						BASE_INPUT_PORT,
						priv->buffer[i]);
				if (ret != OMX_ErrorNone)
					OSCL_LOGE("port %d ,freebuffer:%d failed",
						BASE_INPUT_PORT, i);
			}
			priv->buffer[i] = NULL;
		}
		oscl_free(priv->buffer);
		priv->buffer = NULL;
	}
}


static int untunnel_config_sink_component(aplayer_test_t *priv)
{
	int ret = OMX_ErrorNone;
	OMX_PARAM_PORTDEFINITIONTYPE port_def;
	OMX_AUDIO_PARAM_PCMMODETYPE audio_params;
	struct WAVE_FORMAT_DEF *wave_fmt;

	memset(&port_def, 0, sizeof(port_def));
	priv->buf_num = DEFAULT_ARENDER_BUF_CNT;
	port_def.nBufferCountActual = priv->buf_num;
	port_def.bBuffersContiguous = 1;
	port_def.nPortIndex = BASE_INPUT_PORT;
	port_def.eDomain = OMX_PortDomainAudio;
	port_def.nBufferSize = AUDIO_INBUF_DEFAULT_SIZE;
	ret = OMX_SetParameter(priv->sink, OMX_IndexParamPortDefinition,
			&port_def);
	if (ret) {
		OSCL_LOGE("set port params error!");
		return ret;
	}
	port_def.nPortIndex = BASE_INPUT_PORT;
	ret = OMX_GetParameter(priv->sink, OMX_IndexParamPortDefinition, &port_def);
	if (ret != OMX_ErrorNone) {
		OSCL_LOGE("get params error!\n");
		return ret;
	}
	OSCL_LOGI("count %d, buffersize %d\n",
		port_def.nBufferCountActual, port_def.nBufferSize);

	wave_fmt = &priv->audio_info.fmt_block.wav_format;
	memset(&audio_params, 0, sizeof(OMX_AUDIO_PARAM_PCMMODETYPE));
	audio_params.nChannels = wave_fmt->channels;
	audio_params.nBitPerSample = wave_fmt->bits_per_sample;
	audio_params.nSamplingRate = wave_fmt->samples_per_sec;
	ret = OMX_SetParameter(priv->sink, OMX_IndexParamAudioPcm,
			&audio_params);
	if (ret) {
		OSCL_LOGE("set audio record params error!");
		return ret;
	}
	OSCL_LOGI("channels %d, bits %d, samplerate %d\n", audio_params.nChannels,
		audio_params.nBitPerSample, audio_params.nSamplingRate);

	ret = alloc_buffer(priv, port_def.nBufferCountActual, port_def.nBufferSize);

	return ret;
}

static void untunel_exit(aplayer_test_t *priv)
{
	OMX_FreeHandle(priv->sink);
	priv->sink = NULL;
	sem_destroy(&priv->sem_event);
	sem_destroy(&priv->sem_eos);
	sem_destroy(&priv->sem_buf);
	oscl_queue_deinit(&priv->buffer_queue);
	OSCL_LOGI("here");
}

static void *wave_read_thread(void *params)
{
	OMX_BUFFERHEADERTYPE *buffer;
	aplayer_test_t *priv = (aplayer_test_t *)params;
	int rlen;
	int ret = OMX_ErrorNone;
	OMX_BOOL eof = OMX_FALSE;

	while (1) {
		if (priv->read_thread_exit || eof) {
			OSCL_LOGI("exit read loop!\n");
			break;
		}
		/* dequeue one buffer */
		buffer = oscl_queue_dequeue(&priv->buffer_queue);
		if (buffer == NULL)
			continue;

		/* read wav file */
		rlen = fread(buffer->pBuffer, 1, buffer->nAllocLen, priv->infile);
		OSCL_LOGD("read  len %d\n", rlen);
		if (rlen <= 0) {
			OSCL_LOGE("read file error rlen %d!\n", rlen);
			eof = OMX_TRUE;
			buffer->nFilledLen = 0;
			buffer->nOffset = 0;
			buffer->nFlags = OMX_BUFFERFLAG_EOS;
		} else {
			buffer->nFilledLen = rlen;
			buffer->nOffset = 0;
			buffer->nFlags = 0;
		}
		buffer->nInputPortIndex = BASE_INPUT_PORT;
		ret = OMX_EmptyThisBuffer(priv->sink, buffer);
		if (ret != OMX_ErrorNone) {
			OSCL_LOGE("empty this buffer error!\n");
			break;
		}
	}
	OSCL_LOGI("end of wave read thread\n");
	return NULL;
}

int omx_test_arender_untunnel(char *filename)
{
	int ret = OMX_ErrorNone;
	aplayer_test_t *priv = &aplayer_priv;
	pthread_t wavthread_id = NULL;

	OSCL_LOGD("here");
	memset(priv, 0, sizeof(aplayer_test_t));
	oscl_queue_init(&priv->buffer_queue);

	priv->infile = fopen(filename, "rb");
	if (!priv->infile) {
		OSCL_LOGE("open file error!\n");
		return -1;
	}

	if (fread(&(priv->audio_info.header),
		sizeof(struct RIFF_HEADER_DEF), 1, priv->infile) != 1) {
		OSCL_LOGE("read header info error");
		fclose(priv->infile);
		return -1;
	}

	if (fread(&(priv->audio_info.fmt_block),
		sizeof(struct FMT_BLOCK_DEF), 1, priv->infile) != 1) {
		OSCL_LOGE("read fmt_block info error");
		fclose(priv->infile);
		return -1;
	}

	if (fread(&(priv->audio_info.data_block),
		sizeof(struct DATA_BLOCK_DEF), 1, priv->infile) != 1) {
		OSCL_LOGE("read data_block info error");
		fclose(priv->infile);
		return -1;
	}

	/* 1. get component handle */
	OMX_GetHandle((void **)&priv->sink,
		"OMX.LB.SINK.ARENDER", priv, &untunnel_callbacks);
	OSCL_LOGD("here");
	if (priv->sink == NULL) {
		OSCL_LOGE("get handle failed! %x",
			priv->sink);
		fclose(priv->infile);
		return -1;
	}
	OSCL_LOGD("here");

	sem_init(&priv->sem_event, 0, 0);
	sem_init(&priv->sem_eos, 0, 0);
	sem_init(&priv->sem_buf, 0, 0);

	/* 2. config sink component */
	ret = untunnel_config_sink_component(priv);
	oscl_param_check_exit(ret == OMX_ErrorNone, ret, NULL);

	/* 3. set component stat to idle */
	ret = OMX_SendCommand(priv->sink, OMX_CommandStateSet, OMX_StateIdle, NULL);
	oscl_param_check_exit((ret == OMX_ErrorNone), ret, NULL);
	sem_wait(&priv->sem_event);

	/* 4. set component stat to executing */
	pthread_create(&wavthread_id,
				NULL,
				wave_read_thread,
				(void *)priv);
	ret = OMX_SendCommand(priv->sink, OMX_CommandStateSet, OMX_StateExecuting, NULL);
	oscl_param_check_exit((ret == OMX_ErrorNone), ret, NULL);
	sem_wait(&priv->sem_event);
#if 1
	OSCL_LOGI("wait for sem eos\n");
	sem_wait(&priv->sem_eos);
	OSCL_LOGI("we have eos signal\n");
#else
	rt_thread_delay(1000); /* display 15s */
	OSCL_LOGI("quit test...");
#endif

	/* 5. set component stat to idle */
	ret = OMX_SendCommand(priv->sink, OMX_CommandStateSet, OMX_StateIdle, NULL);
	oscl_param_check_exit((ret == OMX_ErrorNone), ret, NULL);
	sem_wait(&priv->sem_event);
	/* end the wavread thread */
	priv->read_thread_exit = 1;
	pthread_join(wavthread_id, NULL);

	/* 7. free buffer */
	free_buffer(priv);

	/* 8. set component stat to loaded */
	ret = OMX_SendCommand(priv->sink, OMX_CommandStateSet, OMX_StateLoaded, NULL);
	oscl_param_check_exit((ret == OMX_ErrorNone), ret, NULL);
	sem_wait(&priv->sem_event);

EXIT:
	fclose(priv->infile);
	untunel_exit(priv);
	return ret;
}

static omx_component_tbl_t omx_arender_component = {
	"OMX.LB.SINK.ARENDER",
	1,
	arender_component_init, NULL, NULL,
	{
		"sink.arender",
		NULL
	}
};

int arender_component_test(int argc, char **argv)
{
	/* init omx and regster components */
	OMX_Init();
	omx_component_register(&omx_arender_component);

	/* test untunnel audio receiver component */
	omx_test_arender_untunnel(argv[1]);

	return 0;
}

MSH_CMD_EXPORT(arender_component_test, "arender_component_test");

