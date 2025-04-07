/*
 * uap.c - the uart adjust protocol for isp and display
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <rtthread.h>
#include "finsh.h"
#include <debug.h>
#include <stdio.h>
#include <stdlib.h>
#include <dfs_posix.h>
#include <unistd.h>

/* define ISP */
/* {"status_return":"success","file_len":123456,"crc16":4321} */
#define UAP_ISPTOOL_NEED_ISP_FILE	0x00000001
#define UAP_ISPTOOL_GET_ISP_FILE_DATA	0x00000002/* {"status_return":"success"} */

#define UAP_ISPTOOL_SET_ISP_FILE_NAME	0x00000003/* {"status_return":"success"} */
#define UAP_ISPTOOL_SEND_ISP_FILE_DATA	0x00000004/* {"status_return":"success"} */

/* define display */
/* {"status_return":"success","file_len":123456,"crc16":4321} */
#define UAP_DSPTOOL_NEED_DSP_FILE_NAME	0x10000001
#define UAP_DSPTOOL_GET_DSP_FILE_DATA	0x10000002/* {"status_return":"success"} */

#define UAP_DSPTOOL_SET_DSP_FILE_NAME	0x10000003/* {"status_return":"success"} */
#define UAP_DSPTOOL_SEND_DSP_FILE_DATA	0x10000004/* {"status_return":"success"} */

/* #define UAP_DEBUG */
#ifdef UAP_DEBUG
static char g_debug_str[10240];
#define UAP_PRINT_SAVE(s) strcat(g_debug_str, s)
#define UAP_PRINT_DUMP do {\
				printf("%s", g_debug_str);\
				g_debug_str[0] = '\0';\
			} while (0)
#else
#define UAP_PRINT_SAVE(...)
#define UAP_PRINT_DUMP
#endif

#define UAP_STATUS_RET_FAIL	"{\"status_return\":\"fail\",\"dbg\":0}"
#define UAP_STATUS_RET_SUCCESS	"{\"status_return\":\"success\"}"

typedef int (*uap_function_t)(int argc, char **argv);

static rt_thread_t g_uap_tid;

struct uart_adjust_protocol_head {
	char flag;		/* 'r' is read; 'w' is write */
	char is_comp;		/* judge compress */
	unsigned short crc16;	/* data crc16 check */
	int data_len;		/* data lenght */
	int type;		/* data type */
};

static unsigned short uap_crc16(unsigned char *data, unsigned int num)
{
	int i;
	unsigned short crc = 0xFFFF;

	for (; num > 0; num--) {
		crc = crc ^ (*data++ << 8);
		for (i = 0; i < 8; i++) {
			if (crc & 0x8000)
				crc = (crc << 1) ^ 0x1021;
			else
				crc <<= 1;
		}
		crc &= 0xFFFF;
	}

	return crc;
}

static uap_function_t uap_get_func(char *func_name)
{
	struct finsh_syscall *index;
	uap_function_t func = RT_NULL;
	int size = strlen(func_name);

	for (index = _syscall_table_begin; index < _syscall_table_end;
		FINSH_NEXT_SYSCALL(index)) {
		if (strncmp(index->name, "__cmd_", 6) != 0)
			continue;

		if (strncmp(&index->name[6], func_name, size) == 0
			&& index->name[6 + size] == '\0') {
			func = (uap_function_t)index->func;
			break;
		}
	}

	return func;
}

static int uap_readstatus(rt_device_t dev, char *status_str, int timeout)
{
	int ret = -1;
	int cnt = 0;
	rt_tick_t start_time;

	start_time = rt_tick_get();

	if (timeout == -1)
		while (1) {
			if (rt_device_read(dev, 0, &status_str[cnt], 1) == 1) {
				if (status_str[cnt] == '\0') {
					ret = 0;
					break;
				}
				cnt++;
			}
	    }
	else
		while ((rt_tick_get() - start_time) < timeout/10) {
			if (rt_device_read(dev, 0, &status_str[cnt], 1) == 1) {
				if (status_str[cnt] == '\0') {
					ret = 0;
					break;
				}
				cnt++;
			}
	    }

	if (strstr(status_str, "fail") != NULL)
		return -1;
	else
		return ret;
}

static int uap_writestatus(rt_device_t dev, const char *status_str, int timeout)
{
	int len = 0;
	int cnt = 0;
	rt_tick_t start_time;

	start_time = rt_tick_get();
	len = strlen(status_str) + 1;

	if (timeout == -1)
		while (len != 0) {
			if (rt_device_write(dev, 0, &status_str[cnt], 1) == 1) {
				len--;
				cnt++;
			}
		}
	else
		while (len != 0 && (rt_tick_get() - start_time) < timeout/10) {
			if (rt_device_write(dev, 0, &status_str[cnt], 1) == 1) {
				len--;
				cnt++;
			}
		}

	if (len == 0)
		return 0;
	else
		return -1;
}

static int uap_readdata(rt_device_t dev, char *data_buf,
					int data_buf_len, int timeout)
{
	int cnt = 0;
	rt_tick_t start_time;

	start_time = rt_tick_get();

	if (timeout == -1)
		while (data_buf_len > 0) {
			if (rt_device_read(dev, 0, &data_buf[cnt], 1) == 1) {
				cnt++;
				data_buf_len--;
			}
		}
	else
		while (data_buf_len > 0 && (rt_tick_get() - start_time) < timeout/10) {
			if (rt_device_read(dev, 0, &data_buf[cnt], 1) == 1) {
				cnt++;
				data_buf_len--;
			}
		}

	if (data_buf_len == 0)
		return 0;
	else
		return -1;
}

static int uap_writedata(rt_device_t dev, const char *data_buf,
					int data_buf_len, int timeout)
{
	int cnt = 0;
	rt_tick_t start_time;

	start_time = rt_tick_get();

	if (timeout == -1)
		while (data_buf_len > 0) {
			if (rt_device_write(dev, 0, &data_buf[cnt], 1) == 1) {
				cnt++;
				data_buf_len--;
			}
		}
	else
		while (data_buf_len > 0 && (rt_tick_get() - start_time) < timeout/10) {
			if (rt_device_write(dev, 0, &data_buf[cnt], 1) == 1) {
				cnt++;
				data_buf_len--;
			}
		}

	if (data_buf_len == 0)
		return 0;
	else
		return -1;
}

static int uap_readhead(rt_device_t dev, struct uart_adjust_protocol_head *head)
{
	int ret = 0;

	if (head == NULL) {
		UAP_PRINT_SAVE("uap_readhead fail 0\n");
		return -1;
	}

	ret = uap_readdata(dev, (char *)&head->flag, 1, -1);
	if (ret) {
		UAP_PRINT_SAVE("uap_readhead timeout\n");
		return -1;
	}

	if (head->flag != 'r' && head->flag != 'w') {
		UAP_PRINT_SAVE("uap_readhead fail 1\n");
		return -1;
	}

	uap_readdata(dev, (char *)&head->is_comp, 1, 100);
	if (ret) {
		UAP_PRINT_SAVE("uap_readhead fail 2\n");
		return -1;
	}

	uap_readdata(dev, (char *)&head->crc16, 2, 100);
	if (ret) {
		UAP_PRINT_SAVE("uap_readhead fail 3\n");
		return -1;
	}

	uap_readdata(dev, (char *)&head->data_len, 4, 100);
	if (ret) {
		UAP_PRINT_SAVE("uap_readhead fail 4\n");
		return -1;
	}

	uap_readdata(dev, (char *)&head->type, 4, 100);
	if (ret) {
		UAP_PRINT_SAVE("uap_readhead fail 5\n");
		return -1;
	}

	return 0;
}

static int uap_getpack(rt_device_t dev, const struct uart_adjust_protocol_head *head,
		char *data_buf)
{
	int ret = -1;
	unsigned short crc16;
	int timeout;

	timeout = ((head->data_len+6*1024-1) / (6*1024))*1000;

	ret = uap_readdata(dev, data_buf, head->data_len, timeout);
	if (ret == 0) {
		crc16 = uap_crc16((unsigned char *)data_buf,
					(unsigned int)head->data_len);
		if (crc16 == head->crc16) {
			if (head->is_comp)
				ret = -1;/* todo */
			else
				data_buf[head->data_len] = '\0';
			ret = 0;
		}
	}

	return ret;
}

static void uap_thread_entry(void *parameter)
{
	int cmd;
	int file;
	struct stat f_stat;
	char ret_str[128];
	char path_str[128];
	unsigned short crc16;
	rt_device_t dev = RT_NULL;
	char *tran_buf = RT_NULL;
	struct uart_adjust_protocol_head head = {0};

	disable_console();

	/* find device */
	dev = rt_device_find(RT_CONSOLE_DEVICE_NAME);
	if (dev == RT_NULL)
		goto uap_thread_entry_fail;

	/* set device */
	if (rt_device_open(dev, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX
				| RT_DEVICE_FLAG_INT_TX))
		goto uap_thread_entry_fail;

	tran_buf = (char *)malloc(512*1024);
	if (tran_buf == RT_NULL) {
		uap_readdata(dev, tran_buf, 2048, 1000);
		uap_writestatus(dev, "{\"handshake\":\"fail\"}", 2*1000);
		goto uap_thread_entry_fail;
	} else {
		uap_readdata(dev, tran_buf, 2048, 1000);
		uap_writestatus(dev, "{\"handshake\":\"success\"}", 2*1000);
	}

	do {
		memset(&head, 0, sizeof(struct uart_adjust_protocol_head));
		UAP_PRINT_SAVE("1\n");
		if (uap_readhead(dev, &head)) {
			char debug_s[128];
			UAP_PRINT_SAVE("uap_readhead fail: ");
			sprintf(debug_s,
"{\"status_return\":\"fail\",\"dbg\":1,\"flag\":%d,\"type\":%d}", head.flag, head.type);
			uap_writestatus(dev, debug_s, 2*1000);
			goto uap_thread_entry_fail;
		}
		UAP_PRINT_SAVE("2\n");

		cmd = head.type;
		switch (head.type) {
		case UAP_DSPTOOL_NEED_DSP_FILE_NAME:
		case UAP_ISPTOOL_NEED_ISP_FILE:
			if (uap_getpack(dev, &head, path_str) == 0) {
				file = open(path_str, O_RDONLY);
				if (file == -1) {
					uap_writestatus(dev,
					"{\"status_return\":\"fail\",\"dbg\":2}", 2*1000);
					UAP_PRINT_SAVE("open fail: ");
					UAP_PRINT_SAVE(path_str);
					UAP_PRINT_SAVE("\n");
					goto uap_thread_entry_fail;
				} else {
					UAP_PRINT_SAVE("path ok\n");
					fstat(file, &f_stat);
					read(file, tran_buf, f_stat.st_size);
					close(file);
					crc16 = uap_crc16((unsigned char *)tran_buf,
							(unsigned int)f_stat.st_size);
					sprintf(ret_str,
"{\"status_return\":\"success\",\"file_len\":%ld,\"crc16\":%d}", f_stat.st_size, crc16);
					if (uap_writestatus(dev, ret_str, 2*1000)) {
						UAP_PRINT_SAVE("uap_writestatus fail\n");
						goto uap_thread_entry_fail;
					}
				}
			} else {
				uap_writestatus(dev,
					"{\"status_return\":\"fail\",\"dbg\":3}", 2*1000);
				UAP_PRINT_SAVE("uap_getpack fail\n");
				goto uap_thread_entry_fail;
			}
			break;
		case UAP_DSPTOOL_GET_DSP_FILE_DATA:
		case UAP_ISPTOOL_GET_ISP_FILE_DATA:
			file = open(path_str, O_RDONLY);
			if (file == -1) {
				uap_writestatus(dev,
					"{\"status_return\":\"fail\",\"dbg\":4}", 2*1000);
				UAP_PRINT_SAVE("open fail: ");
				UAP_PRINT_SAVE(path_str);
				UAP_PRINT_SAVE("\n");
				goto uap_thread_entry_fail;
			} else {
				UAP_PRINT_SAVE("open ok\n");
				fstat(file, &f_stat);
				read(file, tran_buf, f_stat.st_size);
				close(file);
				if (uap_writedata(dev, tran_buf, f_stat.st_size, -1)) {
					UAP_PRINT_SAVE("uap_writestatus fail\n");
					goto uap_thread_entry_fail;
				} else {
					if (uap_readstatus(dev, tran_buf,
					((f_stat.st_size+6*1024-1) / (6*1024))*1000)) {
						UAP_PRINT_SAVE("uap_readstatus fail\n");
						goto uap_thread_entry_fail;
					}
				}
			}
			break;
		case UAP_DSPTOOL_SET_DSP_FILE_NAME:
		case UAP_ISPTOOL_SET_ISP_FILE_NAME:
			if (uap_getpack(dev, &head, path_str) == 0) {
				if (uap_writestatus(dev, UAP_STATUS_RET_SUCCESS,
										2*1000)) {
					UAP_PRINT_SAVE("uap_writestatus fail\n");
					goto uap_thread_entry_fail;
				}
			} else {
				if (uap_writestatus(dev,
				"{\"status_return\":\"fail\",\"dbg\":5}", 2*1000)) {
					UAP_PRINT_SAVE("uap_writestatus fail\n");
					goto uap_thread_entry_fail;
				}
			}
			break;
		case UAP_DSPTOOL_SEND_DSP_FILE_DATA:
		case UAP_ISPTOOL_SEND_ISP_FILE_DATA:
			if (uap_getpack(dev, &head, tran_buf) == 0) {
				file = open(path_str, O_WRONLY | O_TRUNC);
				if (file == -1) {
					uap_writestatus(dev,
					"{\"status_return\":\"fail\",\"dbg\":6}", 2*1000);
					UAP_PRINT_SAVE("open fail: ");
					UAP_PRINT_SAVE(path_str);
					UAP_PRINT_SAVE("\n");
					goto uap_thread_entry_fail;
				}
				crc16 = write(file, tran_buf, head.data_len);
				sprintf(ret_str, "write ok:%d,%d\n",
								crc16, head.data_len);
				UAP_PRINT_SAVE(ret_str);
				close(file);
				if (uap_writestatus(dev, UAP_STATUS_RET_SUCCESS,
										2*1000)) {
					UAP_PRINT_SAVE("uap_writestatus fail\n");
					goto uap_thread_entry_fail;
				} else {
					uap_function_t fun = NULL;
					if (UAP_ISPTOOL_SEND_ISP_FILE_DATA == cmd)
						fun = uap_get_func("isp_reparse");
					else
						fun = uap_get_func("reload_disp_cjson");
					if (fun != NULL)
						fun(0, NULL);
				}
			} else {
				if (uap_writestatus(dev,
				"{\"status_return\":\"fail\",\"dbg\":7}", 2*1000)) {
					UAP_PRINT_SAVE("uap_writestatus fail\n");
					goto uap_thread_entry_fail;
				}
			}
			break;
		default:
		{
			char debug_s[64];
			UAP_PRINT_SAVE("don't find this type: ");
			sprintf(debug_s, "0x%x, %d, %d, %d, %d\n",
			head.flag, head.is_comp, head.crc16, head.data_len, head.type);
			UAP_PRINT_SAVE(debug_s);
			uap_writestatus(dev,
					"{\"status_return\":\"fail\",\"dbg\":8}", 2*1000);
			goto uap_thread_entry_fail;
		}
		}
	} while (1);

uap_thread_entry_fail:
	if (RT_NULL != dev) {
#if 0
		int i = 0;
		rt_tick_t delta_tick;
		char debug_s[64];

		delta_tick = rt_tick_get();
		for (i = 0; i < 100; i++)
			uap_writedata(dev, tran_buf, 1024, -1);

		sprintf(debug_s, "%d tick\n", rt_tick_get()-delta_tick);
		UAP_PRINT_SAVE(debug_s);
		sleep(1);
#endif
		rt_device_close(dev);
	}

	if (RT_NULL != tran_buf)
		free(tran_buf);

	enable_console();

	UAP_PRINT_DUMP;

	return;
}

static int uap_disable_console(int argc, char **argv)
{
	g_uap_tid = rt_thread_create("uap thread", uap_thread_entry, RT_NULL,
					4096, 10, 10);
	if (g_uap_tid != NULL)
		rt_thread_startup(g_uap_tid);

	return 0;
}

MSH_CMD_EXPORT(uap_disable_console, "disable console for uap");









