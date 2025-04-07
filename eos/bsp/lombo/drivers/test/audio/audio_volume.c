#define DBG_SECTION_NAME	"AUDIOT"
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

long test_volume(int argc, char **argv)
{
	rt_device_t snd;
	int ret;
	struct rt_audio_caps caps;

	snd = rt_device_find("sound0");
	if (snd == NULL) {
		LOG_E("sound device not found");
		return -1;
	}

	LOG_I("set volume %d\n", atoi(argv[3]));
	caps.udata.value = atoi(argv[3]);
	caps.main_type = AUDIO_TYPE_MIXER;
	caps.sub_type = AUDIO_MIXER_VOLUME;
	ret = rt_device_control(snd, AUDIO_CTL_CONFIGURE, &caps);
	if (ret)
		LOG_I("audio config error ret %d", ret);

	return ret;
}

