/*
 * lb_ui.h - header file for load ui.
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */


#ifndef _LB_UI_H_
#define _LB_UI_H_

/*----------------------------------------------*
 * header files                           *
 *----------------------------------------------*/
#include "lb_common.h"

/*----------------------------------------------*
 * defines                  *
 *----------------------------------------------*/

/*----------------------------------------------*
 * structs                  *
 *----------------------------------------------*/

/*----------------------------------------------*
 * global variables                *
 *----------------------------------------------*/

/*----------------------------------------------*
 * extern functions                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * functions                                    *
 *----------------------------------------------*/

/**
 * lb_ui_init - EUI init
 *
 * This function use to init EUI - activate the EUI lib,register input and output devices.
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_ui_init(void);

/**
 * lb_ui_start - EUI start
 *
 * This function use to run EUI - run the EUI lib.
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_ui_start(void);

/**
 * lb_ui_customize_init - EUI customize init
 *
 * This function use to init EUI - activate the EUI customize part function.
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_ui_customize_init(void);

/**
 * lb_ui_rotate - set eui rotate.
 * @rot: the degress of rotate, 0 - no rotate, 1 - rotate 90°,
 * 2 - rotate 180°, 3 - rotate 270°.
 *
 * This function use to rotate EUI coordinate.
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_ui_rotate(lb_uint8 rot);

/**
 * lb_ui_offset - set eui offset.
 * @xoffset: the offset of x direction.
 * @yoffset: the offset of y direction.
 *
 * This function use to the offset of EUI coordinate.
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_ui_offset(lb_int32 xoffset, lb_int32 yoffset);

/**
 * lb_ui_load - EUI load
 * @p_ui_json_file: json file full path.
 *
 * This function use to load EUI - create message process and show static view.
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_ui_load(char *p_ui_json_file);

/**
 * lb_ui_unload - EUI unload
 *
 * This function use to exit EUI - delete all views and destroy meeage process.
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_ui_unload(void);

/**
 * lb_ui_static_init - init static view
 * @p_ui_json_file: json file full path.
 * @p_view: pointer to view pointer.
 *
 * This function use to init static view.
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_ui_static_init(char *p_ui_json_file, lb_view_t **p_view);

/**
 * lb_ui_static_add - add static view
 * @p_ui_json_file: json file full path.
 * @p_view: pointer to view pointer.
 *
 * This function use to add static view.
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_ui_static_add(char *p_ui_json_file, lb_view_t **p_view);

/**
 * lb_ui_static_exit_ex - exit static view
 * @
 * @
 *
 * This function use to exit static view.
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_ui_static_exit_ex(void *param);

/**
 * lb_ui_static_exit - exit static view
 * @p_ui_json_file: json file full path.
 * @p_view: pointer to view pointer.
 *
 * This function use to exit static view.
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_ui_static_exit(lb_view_t **p_view);

/**
 * lb_ui_isolate_init - init isolate view
 * @p_ui_json_file: json file full path.
 * @p_isolate_header: view pointer.
 *
 * This function use to init isolate view.
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_ui_isolate_init(char *p_ui_json_file, lb_view_t *p_isolate_header);

/**
 * lb_ui_isolate_exit - exit isolate view
 * @p_isolate_header: view pointer.
 *
 * This function use to exit isolate view.
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_ui_isolate_exit(lb_view_t *p_isolate_header);

/**
 * lb_ui_send_msg - EUI message send
 * @type: message type.
 * @msg_data: message data.
 * @msg_len: the length of message data.
 * @msg_tone: 0,without click tone;1,wiht click tone
 *
 * This function use to send EUI message.
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_ui_send_msg(lb_int32 type, void *msg_data,
			lb_int32 msg_len, lb_int32 msg_tone);

/**
 * lb_ui_play_tone - play tone
 *
 * This function use to play tone.
 */
void lb_ui_play_tone(void);

#endif


