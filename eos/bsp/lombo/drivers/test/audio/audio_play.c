#define DBG_SECTION_NAME	"AUDIOT"
#define DBG_LEVEL		DBG_INFO
#include <debug.h>

#include <board.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <finsh.h>
#include <drivers/audio.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define PLAY_THREAD_STACK_SIZE (8 * 1024)
#define TMP_WRITE_SIZE		4096

static rt_device_t snd;

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

static char file_name[32];

static rt_err_t audio_device_write_done(struct rt_device *device, void *ptr)
{
	if (!ptr) {
		LOG_E("device buf_release NULL");
		return -RT_ERROR;
	}
	rt_mp_free(ptr);
	return RT_EOK;
}

void wavplay_thread_entry(void *parameter)
{
	int ret;
	FILE *fp = NULL;
	struct wav_info *info = NULL;
	struct rt_audio_caps caps;
	int stream;
	void *tmp_buf = NULL;

	fp = fopen(file_name, "rb");
	if (!fp) {
		LOG_I("open file failed !");
		goto __exit;
	}

	info = (struct wav_info *) malloc(sizeof(*info));
	if (!info)
		goto __exit;

	if (fread(&(info->header), sizeof(struct RIFF_HEADER_DEF), 1, fp) != 1)
		goto __exit;
	if (fread(&(info->fmt_block), sizeof(struct FMT_BLOCK_DEF), 1, fp) != 1)
		goto __exit;
	if (fread(&(info->data_block), sizeof(struct DATA_BLOCK_DEF), 1, fp) != 1)
		goto __exit;

	LOG_I("wav information:");
	LOG_I("samplerate %d", info->fmt_block.wav_format.samples_per_sec);
	LOG_I("channel %d", info->fmt_block.wav_format.channels);
	LOG_I("bit width %d", info->fmt_block.wav_format.bits_per_sample);

	snd = rt_device_find("sound0");
	if (snd == NULL) {
		LOG_E("sound device not found");
		goto __exit;
	}
	/* set tx complete call back function */
	rt_device_set_tx_complete(snd, audio_device_write_done);
	ret = rt_device_open(snd, RT_DEVICE_OFLAG_WRONLY);
	if (ret) {
		LOG_I("audio dev open error ret %d", ret);
		goto __exit;
	}
	caps.udata.config.channels = info->fmt_block.wav_format.channels;
	caps.udata.config.samplerate = info->fmt_block.wav_format.samples_per_sec;
	caps.udata.config.samplefmt = info->fmt_block.wav_format.bits_per_sample;
	caps.main_type = AUDIO_TYPE_OUTPUT;
	caps.sub_type = AUDIO_DSP_PARAM;
	ret = rt_device_control(snd, AUDIO_CTL_CONFIGURE, &caps);
	if (ret) {
		LOG_I("audio config error ret %d", ret);
		goto error1;
	}

	stream = AUDIO_STREAM_REPLAY;
	/*ret = rt_device_control(snd, AUDIO_CTL_START, &stream);
	if (ret) {
		LOG_I("audio ctl start err ret %d", ret);
		goto error1;
	}*/
	tmp_buf = malloc(TMP_WRITE_SIZE);
	if (!tmp_buf) {
		LOG_E("alloc tmp buffer err\n");
		goto error1;
	}
	while (!feof(fp)) {
		int length;
		/* fill the buffer and write it to the driver */
		memset(tmp_buf, 0, TMP_WRITE_SIZE);
		length = fread(tmp_buf, 1, TMP_WRITE_SIZE, fp);
		LOG_D("read length %d\n", length);
		if (length > 0)
			rt_device_write(snd, 0, tmp_buf, length);
		else if (length < 0) {
			LOG_E("read wav file err %d!\n", length);
			rt_device_control(snd, AUDIO_CTL_STOP, &stream);
		}
	}

	rt_device_control(snd, AUDIO_CTL_STOP, &stream);
error1:
	rt_device_close(snd);
__exit:
	if (fp)
		fclose(fp);
	if (info)
		free(info);
	if (tmp_buf)
		free(tmp_buf);
}

long test_play(int argc, char **argv)
{
	rt_thread_t tid = RT_NULL;

	memset(file_name, 0, sizeof(file_name));
	memcpy(file_name, argv[3], strlen(argv[3]));

	LOG_I("file name %s", argv[3]);

	tid = rt_thread_create("wayplay", wavplay_thread_entry,
		RT_NULL, PLAY_THREAD_STACK_SIZE, 22, 10);
	if (tid != RT_NULL)
		rt_thread_startup(tid);

	return 0;
}
