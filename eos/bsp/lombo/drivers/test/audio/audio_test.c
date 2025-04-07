#include <board.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <finsh.h>
#include <string.h>

#include "audio_test.h"

long test_audio(int argc, char **argv)
{
	if (!strcmp(argv[2], "play"))
		test_play(argc, argv);
	else if (!strcmp(argv[2], "record"))
		test_record(argc, argv);
	else if (!strcmp(argv[2], "volume"))
		test_volume(argc, argv);
	else
		return -1;
	return 0;
}

