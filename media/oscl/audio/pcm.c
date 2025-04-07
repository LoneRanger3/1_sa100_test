#define DBG_LEVEL		DBG_WARNING

#include "oscl.h"
#include <rtdevice.h>
#include "audio_core.h"
#include "asoundlib.h"

struct pcm {
	rt_device_t	snd_dev; /* sound device */
	pthread_mutex_t lock;
	int		p_open_cnt;
	int		r_open_cnt;
	int		p_mute;
	int		r_mute;
	int		out_period_time;
};

static struct pcm *g_pcm;

static int init_pcm()
{
	if (g_pcm == NULL) {
		g_pcm = oscl_zalloc(sizeof(struct pcm));
		if (!g_pcm) {
			OSCL_LOGE("malloc pcm err\n");
			return -1;
		}
		g_pcm->snd_dev = rt_device_find("sound0");
		if (g_pcm->snd_dev == NULL) {
			OSCL_LOGE("get sound dev err\n");
			return -1;
		}
		pthread_mutex_init(&g_pcm->lock, NULL);
		g_pcm->p_open_cnt = 0;
		g_pcm->r_open_cnt = 0;
		g_pcm->p_mute = 0;
		g_pcm->r_mute = 0;
	}
	return 0;
}

/* Open and close a stream */
void *pcm_open(unsigned int flags, pcm_config_t *config)
{
	int stream;
	struct rt_audio_caps caps;
	OMX_S32 ret = 0;
	OMX_U32 retry_cnt = 10;

	if (init_pcm()) {
		OSCL_LOGE("init pcm err\n");
		return NULL;
	}

retry_open:
	pthread_mutex_lock(&g_pcm->lock);
	if (flags == PCM_OUT) {
		if (g_pcm->p_open_cnt > 0) {
			pthread_mutex_unlock(&g_pcm->lock);
			if (retry_cnt) {
				retry_cnt--;
				oscl_mdelay(50);
				goto retry_open;
			} else {
				OSCL_LOGW("pcm out already opened\n");
				return NULL;
			}
		}
		/* open the device */
		ret = rt_device_open(g_pcm->snd_dev, RT_DEVICE_OFLAG_WRONLY);
		if (ret != RT_EOK) {
			OSCL_LOGE("rt_device_open error\n");
			goto exit;
		}
		g_pcm->p_open_cnt++;
		/* config the audio stream params */
		caps.udata.config.channels   = config->channels;
		caps.udata.config.samplerate = config->rate;
		caps.udata.config.samplefmt  = config->bitwidth;
		caps.main_type = AUDIO_TYPE_OUTPUT;
		caps.sub_type  = AUDIO_DSP_PARAM;
		ret = rt_device_control(g_pcm->snd_dev, AUDIO_CTL_CONFIGURE, &caps);
		if (ret) {
			OSCL_LOGE("audio config error ret %d", ret);
			goto exit;
		}

		stream = AUDIO_STREAM_REPLAY;
		ret = rt_device_control(g_pcm->snd_dev, AUDIO_CTL_START, &stream);
		if (ret) {
			OSCL_LOGE("audio ctl start err ret %d", ret);
			goto exit;
		}
		g_pcm->out_period_time = REPLAY_MP_BLOCK_SIZE * 1000 / (config->rate * 4);
	}

	if (flags == PCM_IN) {
		if (g_pcm->r_open_cnt > 0) {
			pthread_mutex_unlock(&g_pcm->lock);
			if (retry_cnt) {
				retry_cnt--;
				oscl_mdelay(50);
				goto retry_open;
			} else {
				OSCL_LOGW("pcm in already opened\n");
				return NULL;
			}
		}
		/* open the device */
		ret = rt_device_open(g_pcm->snd_dev, RT_DEVICE_OFLAG_RDONLY);
		if (ret != RT_EOK) {
			OSCL_LOGE("rt_device_open error\n");
			goto exit;
		}
		g_pcm->r_open_cnt++;
		/* config the audio stream params */
		caps.udata.config.channels   = config->channels;
		caps.udata.config.samplerate = config->rate;
		caps.udata.config.samplefmt  = config->bitwidth;
		caps.main_type = AUDIO_TYPE_INPUT;
		caps.sub_type  = AUDIO_DSP_PARAM;
		ret = rt_device_control(g_pcm->snd_dev, AUDIO_CTL_CONFIGURE, &caps);
		if (ret) {
			OSCL_LOGE("audio config error ret %d", ret);
			goto exit;
		}
	}

	pthread_mutex_unlock(&g_pcm->lock);

	return g_pcm;
exit:
	while (g_pcm->p_open_cnt) {
		rt_device_close(g_pcm->snd_dev);
		g_pcm->p_open_cnt--;
	}
	while (g_pcm->r_open_cnt) {
		rt_device_close(g_pcm->snd_dev);
		g_pcm->r_open_cnt--;
	}
	oscl_free(g_pcm);
	pthread_mutex_unlock(&g_pcm->lock);
	return NULL;
}
RTM_EXPORT(pcm_open);

int pcm_close(void *pcm, unsigned int flags)
{
	int stream;

	OSCL_LOGD("pcm close flag %x\n", flags);

	pthread_mutex_lock(&g_pcm->lock);
	if (flags == PCM_OUT) {
		if (g_pcm->p_open_cnt <= 0) {
			pthread_mutex_unlock(&g_pcm->lock);
			return 0;
		}
		/* stop audio codec */
		stream = AUDIO_STREAM_REPLAY;
		rt_device_control(g_pcm->snd_dev, AUDIO_CTL_STOP, &stream);

		/* close audio device */
		rt_device_close(g_pcm->snd_dev);
		g_pcm->p_open_cnt--;
	}

	if (flags == PCM_IN) {
		if (g_pcm->r_open_cnt <= 0) {
			pthread_mutex_unlock(&g_pcm->lock);
			return 0;
		}
		/* stop audio record */
		stream = AUDIO_STREAM_RECORD;
		rt_device_control(g_pcm->snd_dev, AUDIO_CTL_STOP, &stream);

		/* close audio device */
		rt_device_close(g_pcm->snd_dev);
		g_pcm->r_open_cnt--;
	}

	pthread_mutex_unlock(&g_pcm->lock);

	return 0;
}
RTM_EXPORT(pcm_close);

int pcm_write(void *pcm, void *data, unsigned int count)
{
	OMX_S32 ret = 0;
	OMX_U32 buf_to_write = 0;
	OMX_U32 inbuffer_left = count;
	OMX_U8 *tmp = (OMX_U8 *)data;

	if (!pcm  || g_pcm->p_open_cnt <= 0) {
		OSCL_LOGE("pcm not opened\n");
		return  -1;
	}
	while (inbuffer_left > 0 && g_pcm->p_open_cnt > 0) {
		if (inbuffer_left > REPLAY_MP_BLOCK_SIZE)
			buf_to_write = REPLAY_MP_BLOCK_SIZE;
		else
			buf_to_write = inbuffer_left;
		ret = rt_device_write(g_pcm->snd_dev, 0, tmp, buf_to_write);
		if (ret != buf_to_write) {
			OSCL_LOGE("write buf err\n");
			return -1;
		}
		inbuffer_left -= buf_to_write;
		tmp += buf_to_write;
	}

	return count;
}
RTM_EXPORT(pcm_write);

int pcm_read(void *pcm, void *data, unsigned int count)
{
	int ret;
	if (!pcm) {
		OSCL_LOGE("pcm null\n");
		return  -1;
	}
	ret = rt_device_read(g_pcm->snd_dev, 0, data, count);
	if (g_pcm->r_mute)
		memset(data, 0, count);
	return ret;
}
RTM_EXPORT(pcm_read);

int pcm_flush(void *pcm, unsigned int flags)
{
	if (!pcm) {
		OSCL_LOGE("pcm null\n");
		return  -1;
	}

	if (flags == PCM_OUT && g_pcm->p_open_cnt > 0)
		oscl_mdelay(g_pcm->out_period_time);

	return 0;
}
RTM_EXPORT(pcm_flush);

int pcm_set_volume(int volume, unsigned int flags)
{
	int ret;
	struct rt_audio_caps caps;

	if (init_pcm()) {
		OSCL_LOGE("init pcm err\n");
		return -1;
	}

	memset(&caps, 0, sizeof(caps));
	caps.udata.value = volume;
	caps.main_type = AUDIO_TYPE_MIXER;
	if (flags == PCM_OUT)
		caps.sub_type  = AUDIO_MIXER_VOLUME;
	else
		caps.sub_type  = AUDIO_MIXER_MIC;
	pthread_mutex_lock(&g_pcm->lock);
	ret = rt_device_control(g_pcm->snd_dev, AUDIO_CTL_CONFIGURE, &caps);
	pthread_mutex_unlock(&g_pcm->lock);

	return ret;

}
RTM_EXPORT(pcm_set_volume);

int pcm_set_mute(int mute, unsigned int flags)
{
	if (init_pcm()) {
		OSCL_LOGE("init pcm err\n");
		return -1;
	}
	pthread_mutex_lock(&g_pcm->lock);
	if (flags == PCM_IN)
		g_pcm->r_mute = mute;
	else
		g_pcm->p_mute = mute;
	pthread_mutex_unlock(&g_pcm->lock);

	return 0;
}
RTM_EXPORT(pcm_set_mute);

/* Returns the pcm latency in ms */
unsigned int pcm_get_latency(void *pcm, unsigned int flags)
{
	if (!pcm) {
		OSCL_LOGE("pcm null\n");
		return  -1;
	}

	return 0;
}

