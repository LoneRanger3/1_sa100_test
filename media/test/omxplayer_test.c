#define DBG_LEVEL         DBG_INFO

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <dirent.h>
#include <string.h>
#include <oscl.h>
#include "display/oscl_display.h"

#include "omx_mediaplayer.h"

#define MAX_FILE_CNT	8
#define TEST_PAUSE_COUNT 1000
#define TEST_CYCLE_COUNT 10000
#define TEST_SEEK_COUNT  1000
#define TEST_DISPLAY_WIDTH 320
#define TEST_DISPLAY_HEIGHT 800

static int loop;
static int file_cnt;
static int cur_file_index;
static char files[MAX_FILE_CNT][512];
static char *cur_file;
void *omxplayer;

static void on_completion(void *handle)
{
	OSCL_LOGI("------on_completion--------\n");
	if (loop == 1)
		omxmp_start(handle);
	if (loop == 2) {
		omxmp_stop(handle);
		omxmp_reset(handle);
		cur_file_index++;
		cur_file_index = cur_file_index % file_cnt;
		omxmp_set_data_source(omxplayer, files[cur_file_index]);
		omxmp_prepare(omxplayer);
		omxmp_start(omxplayer);
	}
}
static void on_error(void *handle, int omx_err)
{
	omxmp_stop(handle);
	OSCL_LOGI("------on_error %d--------\n", omx_err);
}
static void on_prepared(void *handle)
{
	OSCL_LOGI("------on_prepared--------\n");
}
static void on_seek_complete(void *handle)
{
	OSCL_LOGI("------on_seek_complete--------\n");
}
static void on_video_size_changed(void *handle, int width, int height)
{
	OSCL_LOGI("on_video_size_changed width=%d, height=%d\n", width, height);
}
static void on_timed_text(void *handle, char *text)
{
	OSCL_LOGI("------on_timed_text %s--------\n", text);
}

static mp_callback_ops_t cb_ops = {
	.on_completion            = on_completion,
	.on_error                 = on_error,
	.on_prepared              = on_prepared,
	.on_seek_complete         = on_seek_complete,
	.on_video_size_changed    = on_video_size_changed,
	.on_timed_text            = on_timed_text,
};

static void show_help(void)
{
	const char *arg_format = "  %-20s %s%s\n";

	OSCL_PRINT("usage: omxplayer [argument]\n");
	OSCL_PRINT("arguments:\n");
	OSCL_PRINT(arg_format, "--file filename", "set input filename", "");
	OSCL_PRINT(arg_format, "--prepare", "prepare", "");
	OSCL_PRINT(arg_format, "--start", "start player", "");
	OSCL_PRINT(arg_format, "--stop", "stop player", "");
	OSCL_PRINT(arg_format, "--pause", "pause player", "");
	OSCL_PRINT(arg_format, "--quit", "quit player", "");
	OSCL_PRINT(arg_format, "--help", "show this help info", "");
	OSCL_PRINT(arg_format, "--seek", "seek to a certan position", "");
	OSCL_PRINT(arg_format, "--rotate", "set rotate mode", "");
}


struct option play_long_options[] = {
	{"file", 1, 0, 'f'},
	{"prepare", 0, 0, 'p'},
	{"start", 0, 0, 'b'},
	{"stop", 0, 0, 'e'},
	{"pause", 0, 0, 'a'},
	{"quit", 0, 0, 'q'},
	{"seek", 1, 0, 'k'},
	{"release", 0, 0, 'r'},
	{"rotate", 1, 0, 't'},
	{"test_pause", 1, 0, 'P'},
	{"test_seek", 1, 0, 'S'},
	{"test_cycle", 1, 0, 'C'},
	{"test_loop", 1, 0, 'L'},
	{"test_dir", 1, 0, 'D'},
	{"help", 0, 0, 'h'}
};

static void *test_pause_thread(void *param)
{
	int ret;
	int count = 0;
	void *player = param;

	while (count < TEST_PAUSE_COUNT) {
		if (omxmp_is_playing(player)) {
			oscl_mdelay(3000);
			ret = omxmp_pause(player);
			if (ret) {
				OSCL_LOGE("pause err\n");
				continue;
			}
			oscl_mdelay(2000);
			ret = omxmp_start(player);
			if (ret)
				OSCL_LOGE("start err\n");
			count++;
			OSCL_LOGI("pause start test %d\n", count);
		}
	}

	omxmp_release(omxplayer);
	omxplayer = NULL;
	loop = 0;

	OSCL_LOGI("pause-start test end\n");
	return NULL;
}

static void *test_cycle_thread(void *param)
{
	int ret;
	int count = 0;
	void *player = param;

	while (count < TEST_CYCLE_COUNT) {
		ret = omxmp_set_data_source(player, cur_file);
		if (ret)
			OSCL_LOGE("set data url err\n");
		ret = omxmp_prepare(player);
		if (ret)
			OSCL_LOGE("omxmp_prepare err\n");
		ret = omxmp_start(player);
		if (ret)
			OSCL_LOGE("omxmp_start err\n");
		oscl_mdelay(5000);
		ret = omxmp_stop(player);
		if (ret)
			OSCL_LOGE("omxmp_stop err\n");
		ret = omxmp_reset(player);
		if (ret)
			OSCL_LOGE("omxmp_reset err\n");
		count++;
		OSCL_LOGI("test cycle count %d\n", count);
	}

	omxmp_release(omxplayer);
	oscl_free(cur_file);
	omxplayer = NULL;

	OSCL_LOGI("cycle test end\n");
	return NULL;
}

static int get_mp4files(char *file_path)
{
	DIR *dir = NULL;
	struct dirent *entry = NULL;
	char *suffix = NULL;

	dir = opendir(file_path);
	if (dir == NULL) {
		OSCL_LOGE("open dir err\n");
		return -1;
	}
	file_cnt = 0;
	while ((entry = readdir(dir))) {
		if (entry->d_type == 1) {
			suffix = strrchr(entry->d_name, '.');
			OSCL_LOGI("suffix %s\n", suffix);
			if (strcmp(suffix, ".mp4") == 0) {
				memset(files[file_cnt], 0, 511);
				snprintf(files[file_cnt], 511, "%s%s%s",
					file_path, "/", entry->d_name);
				OSCL_LOGI("get mp4 file %s\n", files[file_cnt]);
				file_cnt++;
			}
		}
		if (file_cnt >= MAX_FILE_CNT)
			break;
	}
	closedir(dir);
	if (file_cnt > 0)
		return 0;
	else
		return -1;
}

const char *short_opts = "f:k:pbeaqhrt:P:C:L:S:D:";
int omxplayer_test(int argc, char *argv[])
{
	int          ret = 0;
	int          c;
	int          index;
	disp_size_t  scn_size;
	void         *disp_hdl;
	omxmp_win_t  win;
	pthread_attr_t t_attr;
	pthread_t      thread_id;

	if (omxplayer == NULL) {
		omxplayer = omxmp_create(&cb_ops);
		if (omxplayer == NULL) {
			OSCL_LOGE("create omxplayer error!\n");
			return -1;
		}
		/* get screen size */
		memset(&scn_size, 0, sizeof(scn_size));
		disp_hdl = oscl_open_disp_engine();
		oscl_param_check(disp_hdl != NULL, -1, "open disp engine err\n");
		ret = oscl_disp_get_screen_size(disp_hdl, &scn_size);
		oscl_param_check(ret == OMX_ErrorNone, -1, "get screen size err\n");
		oscl_close_disp_engine(disp_hdl);
		OSCL_LOGE("scrn w %d, h %d\n", scn_size.width, scn_size.height);

		/* set win */
#ifdef ARCH_LOMBO_N7V1_CDR
		/* for cdr */
		win.left = 0;
		win.top  = 240;
		win.width = TEST_DISPLAY_WIDTH;
		win.height = TEST_DISPLAY_HEIGHT;
#else
		win.left = 0;
		win.top  = 0;
		win.width = scn_size.width;
		win.height = scn_size.height;
#endif
		ret = omxmp_set_window(omxplayer, &win);
		oscl_param_check(ret == 0, -1, "set win err\n");
		/* set scaling mode */
		ret = omxmp_set_scaling_mode(omxplayer,
			OMXMP_WINDOW_FULL_SCREEN_SCREEN_RATIO);
		oscl_param_check(ret == 0, -1, "set scale mode err\n");
#ifdef ARCH_LOMBO_N7V1_CDR
		/* set rotate mode */
		ret = omxmp_set_rotation(omxplayer, OMXMP_ROTATE_90);
		oscl_param_check(ret == 0, -1, "set rotate mode err\n");
#endif
	}

	loop = 0;

	while (1) {
		c = getopt_long(argc, argv, short_opts, play_long_options, &index);
		if (c == -1) {
			OSCL_LOGE("get args end, exit!\n");
			break;
		}
		OSCL_LOGE("option:%c", c);
		OSCL_LOGE("option:%c, optarg:%s", c, (optarg == NULL) ? "null" : optarg);
		switch (c) {
		case 'h':
			show_help();
			break;
		case 'f':
			ret = omxmp_set_data_source(omxplayer, optarg);
			break;
		case 'p': {
			ret = omxmp_prepare(omxplayer);
			break;
		}
		case 'b': {
			ret = omxmp_start(omxplayer);
			break;
		}
		case 'e': {
			ret = omxmp_stop(omxplayer);
			break;
		}
		case 'a': {
			ret = omxmp_pause(omxplayer);
			break;
		}
		case 'q': {
			omxmp_reset(omxplayer);
			break;
		}
		case 'k': {
			int position = atoi(optarg);
			OSCL_LOGI("seek to %d s\n", position);
			ret = omxmp_seek_to(omxplayer, (long)(position*1000));
			break;
		}
		case 'r': {
			omxmp_release(omxplayer);
			omxplayer = NULL;
			break;
		}
		case 't': {
			int rotate = atoi(optarg);
			OSCL_LOGI("set rotate %d\n", rotate);
			omxmp_set_rotation(omxplayer, rotate);
			break;
		}
		case 'P':
			/* init thread attr for test thread */
			pthread_attr_init(&t_attr);
			pthread_attr_setstacksize(&t_attr, 0x2000);
			ret = omxmp_set_data_source(omxplayer, optarg);
			oscl_param_check_exit(ret == 0, -1, "set data url err!\n");
			loop = 1;
			ret = omxmp_prepare(omxplayer);
			oscl_param_check_exit(ret == 0, -1, "omxmp_prepare err!\n");
			ret = omxmp_start(omxplayer);
			oscl_param_check_exit(ret == 0, -1, "omxmp_start err!\n");
			ret = pthread_create(&thread_id, &t_attr,
				test_pause_thread, omxplayer);
			oscl_param_check_exit(ret == 0, -1, "thread create err!\n");
			break;
		case 'S': {
			int i = 0;
			int position;
			int duration;

			loop = 1;
			ret = omxmp_set_data_source(omxplayer, optarg);
			oscl_param_check_exit(ret == 0, -1, "set data url err!\n");
			ret = omxmp_prepare(omxplayer);
			oscl_param_check_exit(ret == 0, -1, "omxmp_prepare err!\n");
			duration = omxmp_get_duration(omxplayer);
			OSCL_LOGI("duration: %d\n", duration);
			oscl_param_check_exit(duration > 0, -1, "get duration err!\n");
			duration = duration / 1000;
			if (duration < 20) {
				OSCL_LOGW("media stream duration is shorter than 20s\n");
				ret = omxmp_reset(omxplayer);
				goto EXIT;
			}
			ret = omxmp_start(omxplayer);
			oscl_param_check_exit(ret == 0, -1, "omxmp_start err!\n");
			for (i = 0; i < TEST_SEEK_COUNT; i++) {
				oscl_mdelay(5000);
				position = rand() % (duration - 10);
				OSCL_LOGI("seek to %d s\n", position);
				ret = omxmp_seek_to(omxplayer, (long)(position*1000));
				if (ret)
					OSCL_LOGE("seek err\n");
			}
			oscl_mdelay(4000);
			ret = omxmp_stop(omxplayer);
			ret = omxmp_reset(omxplayer);
			loop = 0;
			OSCL_LOGI("seek test end\n");
			break;
		}
		case 'L':
			ret = omxmp_set_data_source(omxplayer, optarg);
			oscl_param_check_exit(ret == 0, -1, "set data url err!\n");
			loop = 1;
			ret = omxmp_prepare(omxplayer);
			oscl_param_check_exit(ret == 0, -1, "omxmp_prepare err!\n");
			ret = omxmp_start(omxplayer);
			break;
		case 'C':
			loop = 0;
			cur_file = oscl_strdup(optarg);
			/* init thread attr for test thread */
			pthread_attr_init(&t_attr);
			pthread_attr_setstacksize(&t_attr, 0x2000);
			ret = pthread_create(&thread_id, &t_attr,
				test_cycle_thread, omxplayer);
			oscl_param_check_exit(ret == 0, -1, "thread create err!\n");
			break;
		case 'D': {
			loop = 2;

			if (get_mp4files(optarg))
				break;
			cur_file_index = 0;
			ret = omxmp_set_data_source(omxplayer, files[cur_file_index]);
			oscl_param_check_exit(ret == 0, -1, "set data url err!\n");
			ret = omxmp_prepare(omxplayer);
			oscl_param_check_exit(ret == 0, -1, "omxmp_prepare err!\n");
			ret = omxmp_start(omxplayer);
			oscl_param_check_exit(ret == 0, -1, "omxmp_start err!\n");
			break;
		}
		default:
			OSCL_LOGE("Unexcepted case, please let me know, option '%c'",
				optopt);
			break;
		}
	}
EXIT:
	OSCL_LOGI("option:%c, ret %d\n", c, ret);
	optind = 0;
	return 0;
}

MSH_CMD_EXPORT(omxplayer_test, "omxplayer_test");

