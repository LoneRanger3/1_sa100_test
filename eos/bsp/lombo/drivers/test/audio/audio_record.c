#define DBG_SECTION_NAME	"ARCORDT"
#define DBG_LEVEL		DBG_LOG
#include <debug.h>

#include <board.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <finsh.h>
#include <drivers/audio.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define TEST_RATE		48000
#define TEST_CHANNELS		2
#define TEST_FMT_WIDTH		16

#define DEFAULT_WBUF_CNT	4

#define WRITE_THREAD_STACH_SIZE (8 * 1024)

static rt_device_t snd_dev;
static char file_name[32];
static rt_uint8_t  *rec_buff[DEFAULT_WBUF_CNT]; /* buffer array for record */
static rt_uint8_t  cur_index; /* index of the record buffers */
static rt_bool_t   abort_write;
static struct rt_data_queue wqueue; /* queue of buffer ready to write */
static int rec_buf_size; /* record buffer size */

struct speech_wav_header {
	char riff_id[4];            /* "RIFF" */
	uint32_t size0;             /* file len - 8 */
	char wave_fmt[8];           /* "WAVEfmt " */
	uint32_t size1;             /* 0x10 */
	uint16_t fmttag;            /* 0x01 */
	uint16_t channel;           /* 1 */
	uint32_t samplespersec;     /* 8000 */
	uint32_t bytepersec;        /* 8000 * 2 */
	uint16_t blockalign;        /* 1 * 16 / 8 */
	uint16_t bitpersamples;     /* 16 */
	char data_id[4];            /* "data" */
	uint32_t size2;             /* file len - 44 */
};

static void speech_wav_init_header(struct speech_wav_header *header,
	rt_uint16_t channel, int samplespersec, int bitpersamples)
{
	memcpy(header->riff_id, "RIFF", 4);
	header->size0           = 0;       /* Final file size not known yet, write 0 */
	memcpy(header->wave_fmt, "WAVEfmt ", 8);
	header->size1           = 16;               /* Sub-chunk size, 16 for PCM */
	header->fmttag          = 1;                /* AudioFormat, 1 for PCM */
	/* Number of channels, 1 for mono, 2 for stereo */
	header->channel         = channel;
	header->samplespersec   = samplespersec;    /* Sample rate */
	/*Byte rate */
	header->bytepersec      = samplespersec * bitpersamples * channel / 8;
	/* Block align, NumberOfChannels*BitsPerSample/8 */
	header->blockalign      = channel * bitpersamples / 8;
	header->bitpersamples   = bitpersamples;
	memcpy(header->data_id, "data", 4);
	header->size2           = 0;
}

static void speech_wav_upgrade_size(struct speech_wav_header *header,
	rt_uint32_t paylodsize)
{
	header->size0           = paylodsize + 36;
	header->size2           = paylodsize;
}

static rt_err_t audio_rx_ind(rt_device_t dev, rt_size_t size)
{
	/* LOG_I("rx indicate size %d", size); */
	return RT_EOK;
}

void write_thread_entry(void *parameter)
{
	int ret;
	struct speech_wav_header wav_header;
	rt_uint32_t wav_len = 0;
	void *rx_buffer = NULL;
	rt_uint32_t rx_size;
	FILE *fp = NULL;

	fp = fopen(file_name, "wb+");
	if (!fp) {
		LOG_E("open file failed!");
		return;
	}

	/* init & write the wav file header */
	speech_wav_init_header(&wav_header, TEST_CHANNELS, TEST_RATE, TEST_FMT_WIDTH);
	fwrite(&wav_header, sizeof(struct speech_wav_header), 1, fp);

	while (!abort_write) {
		rt_data_queue_pop(&wqueue, (const void **)&rx_buffer,
			(rt_size_t *)&rx_size, 100);
		if (rx_buffer) {
			ret = fwrite(rx_buffer, rx_size, 1, fp);
			if (ret != 1)
				LOG_E("write rx buffer error");
			wav_len += rx_size;
		}
	}

	/* upgrage wav header */
	fseek(fp, 0, SEEK_SET);
	speech_wav_upgrade_size(&wav_header, wav_len);
	fwrite(&wav_header, sizeof(struct speech_wav_header), 1, fp);

	fclose(fp);

	LOG_I("=========end of write thread=========");
}


void record_thread(void *parameter)
{
	int ret, i, j, rdlen;
	struct rt_audio_caps caps;
	rt_thread_t write_thread;
	int stream;

	snd_dev = rt_device_find("sound0");
	if (snd_dev == NULL) {
		LOG_E("sound device not found");
		return;
	}

	/* set rx complete call back function */
	rt_device_set_rx_indicate(snd_dev, audio_rx_ind);
	ret = rt_device_open(snd_dev, RT_DEVICE_OFLAG_RDONLY);
	if (ret) {
		LOG_E("audio dev open error ret %d", ret);
		return;
	}

	/* config the audio stream params */
	caps.udata.config.channels   = TEST_CHANNELS;
	caps.udata.config.samplerate = TEST_RATE;
	caps.udata.config.samplefmt  = TEST_FMT_WIDTH;
	caps.main_type = AUDIO_TYPE_INPUT;
	caps.sub_type  = AUDIO_DSP_PARAM;
	ret = rt_device_control(snd_dev, AUDIO_CTL_CONFIGURE, &caps);
	if (ret) {
		LOG_E("audio config error ret %d", ret);
		goto error1;
	}

	/* malloc the buffer array for record */
	rec_buf_size = RECORD_MP_BLOCK_SIZE;
	for (i = 0; i < DEFAULT_WBUF_CNT; i++) {
		rec_buff[i] = (rt_uint8_t *)rt_malloc(rec_buf_size);
		if (!rec_buff[i]) {
			LOG_E("alloc rec buffer error");
			goto error2;
		}
	}

	/* init the write data queue */
	ret = rt_data_queue_init(&wqueue, DEFAULT_WBUF_CNT, 0, NULL);
	if (ret) {
		LOG_E("rt_data_queue_init wqueue error!!");
		goto error2;
	}

	write_thread = rt_thread_create("write_wav", write_thread_entry,
			RT_NULL, WRITE_THREAD_STACH_SIZE, 22, 10);
	if (write_thread != RT_NULL)
		rt_thread_startup(write_thread);
	else {
		LOG_E("rt_thread_create error!!");
		goto error2;
	}

	cur_index   = 0;
	abort_write = RT_FALSE;

	while (i++ < 500) {
		/* read the record buffer from driver and push to the wqueue */
		rdlen = rt_device_read(snd_dev, 0, rec_buff[cur_index], rec_buf_size);
		if (rdlen <= 0) {
			LOG_E("read buffer error!!");
			continue;
		}
		rt_data_queue_push(&wqueue, rec_buff[cur_index], rec_buf_size, 0);
		cur_index++;
		cur_index = cur_index % DEFAULT_WBUF_CNT;
	}

	abort_write = RT_TRUE;

	stream = AUDIO_STREAM_RECORD;
	rt_device_control(snd_dev, AUDIO_CTL_STOP, &stream);

	rt_device_close(snd_dev);
	for (i = 0; i < DEFAULT_WBUF_CNT; i++)
		rt_free(rec_buff[i]);
	LOG_I("=========main end===========");

	return;
error2:
	for (j = 0; j < i; j++)
		rt_free(rec_buff[j]);
error1:
	rt_device_close(snd_dev);
}


long test_record(int argc, char **argv)
{
	rt_thread_t tid;

	memset(file_name, 0, sizeof(file_name));
	memcpy(file_name, argv[3], strlen(argv[3]));
	tid = rt_thread_create("record_test", record_thread,
			RT_NULL, WRITE_THREAD_STACH_SIZE, 22, 10);
	if (tid != RT_NULL)
		rt_thread_startup(tid);
	else
		LOG_E("rt_thread_create error!!");
	return 0;
}

