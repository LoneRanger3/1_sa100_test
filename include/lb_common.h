/*
 * lb_common.h - header file for common property.
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

#ifndef _LB_COMMON_H_
#define _LB_COMMON_H_

#include <stdbool.h>
#include "lb_types.h"

/*----------------------------------------------*
 * header files                           *
 *----------------------------------------------*/

/*----------------------------------------------*
 * defines                  *
 *----------------------------------------------*/

#define LB_USE_SINGLE_THREAD_MODE		1

/* Constant */
#define LB_MBOX_MAX_BTNS                            3

/* Error Code */
#define LB_ERROR_OFFSET                             0x80000000
#define SUCCESS                                     0
#define	FAIL					    -1
#define LB_ERROR_INVALID_PARAM                      (LB_ERROR_OFFSET|1)
#define LB_ERROR_NO_MEM                             (LB_ERROR_OFFSET|2)
#define LB_ERROR_NO_LINK                            (LB_ERROR_OFFSET|3)
#define LB_ERROR_NO_VIEW                            (LB_ERROR_OFFSET|3)

#define LB_ERROR_OPEN_FILE                          (LB_ERROR_OFFSET|10)
#define LB_ERROR_SEEK_FILE                          (LB_ERROR_OFFSET|11)
#define LB_ERROR_TELL_FILE                          (LB_ERROR_OFFSET|12)
#define LB_ERROR_EMPTY_FILE                         (LB_ERROR_OFFSET|13)
#define LB_ERROR_READ_FILE                          (LB_ERROR_OFFSET|14)
#define LB_ERROR_WRITE_FILE                         (LB_ERROR_OFFSET|15)
#define LB_ERROR_CLOSE_FILE                         (LB_ERROR_OFFSET|16)

#define LB_ERROR_CJSON_NULL                         (LB_ERROR_OFFSET|20)
#define LB_ERROR_CJSON_FORMAT                       (LB_ERROR_OFFSET|21)
#define LB_ERROR_CJSON_NO_BG                        (LB_ERROR_OFFSET|22)
#define LB_ERROR_CJSON_NO_OBJS                      (LB_ERROR_OFFSET|23)
#define LB_ERROR_CJSON_INVALID_VALUE                (LB_ERROR_OFFSET|24)

#define LB_ERROR_UNKOWN                             (LB_ERROR_OFFSET|50)

#define LB_ERROR_VFONT_INIT                         (LB_ERROR_OFFSET|70)
#define LB_ERROR_VFONT_NOT_SUPPORT                  (LB_ERROR_OFFSET|71)
#define LB_ERROR_VFONT_NEW_FACE                     (LB_ERROR_OFFSET|72)
#define LB_ERROR_VFONT_GET_CHAR_LIST                (LB_ERROR_OFFSET|73)
#define LB_ERROR_VFONT_NO_FILE                      (LB_ERROR_OFFSET|74)
#define LB_ERROR_VFONT_SET_SIZE                     (LB_ERROR_OFFSET|75)
#define LB_ERROR_VFONT_GET_CHAR_INDEX               (LB_ERROR_OFFSET|76)
#define LB_ERROR_VFONT_LOAD_GLYPH                   (LB_ERROR_OFFSET|77)
#define LB_ERROR_VFONT_RENDER_GLYPH                 (LB_ERROR_OFFSET|78)

#define LB_ERROR_STYLE_NO_ID                        (LB_ERROR_OFFSET|80)

#define LB_ERROR_STR_NO_FOUND                       (LB_ERROR_OFFSET|90)

#define LB_ERROR_OBJ_TYPE_NO_FOUND                  (LB_ERROR_OFFSET|100)
#define LB_ERROR_OBJ_INIT_FUNC_NO_FOUND             (LB_ERROR_OFFSET|101)
#define LB_ERROR_OBJ_RESP_FUNC_NO_FOUND             (LB_ERROR_OFFSET|102)
#define LB_ERROR_BG_TYPE_ILLEGAL                    (LB_ERROR_OFFSET|103)
#define LB_ERROR_LIST_LINKS_NUM                     (LB_ERROR_OFFSET|104)

#define LB_ERROR_RESP_MSG_NOT_FOUND                 (LB_ERROR_OFFSET|120)
#define LB_ERROR_OBJ_PROPERTY_NOT_FOUND             (LB_ERROR_OFFSET|121)
#define LB_ERROR_OBJ_EXT_NOT_FOUND                  (LB_ERROR_OFFSET|122)
#define LB_ERROR_OBJ_CHART_NO_SPECIFIED_POINTS      (LB_ERROR_OFFSET|123)
#define LB_ERROR_NO_OBJ_TYPE                        (LB_ERROR_OFFSET|124)
#define LB_ERROR_NO_OBJ_ID                          (LB_ERROR_OFFSET|125)
#define LB_ERROR_WRONG_ALIGN_MODE                   (LB_ERROR_OFFSET|126)
#define LB_ERROR_NO_CURRENT_VIEW                    (LB_ERROR_OFFSET|127)
#define LB_ERROR_IMG_TYPE                           (LB_ERROR_OFFSET|128)

/* Messages. */
#define LB_MSG_BASE                                 (0xE000)
#define LB_MSG_ENTER_LINKAGE                        (LB_MSG_BASE|1)
#define LB_MSG_RETURN_PARENT                        (LB_MSG_BASE|2)
#define LB_MSG_ENTER_LIST_LINKAGE                   (LB_MSG_BASE|3)
#define LB_MSG_ENTER_ISOLATE			    (LB_MSG_BASE|4)
#define LB_MSG_UI_STATIC_EXIT			    (LB_MSG_BASE|5)
#define LB_MSG_UI_KEY_TONE			    (LB_MSG_BASE|6)
#define LB_MSG_UI_UNREG_FUNC			    (LB_MSG_BASE|7)

/* objx update messages */
#define LB_MSG_OBJX_UPDATE_BASE			(LB_MSG_BASE|0x400)
#define LB_MSG_ARC_UPDATE			(LB_MSG_OBJX_UPDATE_BASE|1)
#define LB_MSG_BAR_UPDATE			(LB_MSG_OBJX_UPDATE_BASE|2)
#define LB_MSG_BTN_UPDATE			(LB_MSG_OBJX_UPDATE_BASE|3)
#define LB_MSG_BTNM_UPDATE			(LB_MSG_OBJX_UPDATE_BASE|4)
#define LB_MSG_CALENDAR_UPDATE			(LB_MSG_OBJX_UPDATE_BASE|5)
#define LB_MSG_CHART_UPDATE			(LB_MSG_OBJX_UPDATE_BASE|6)
#define LB_MSG_CHECKBOX_UPDATE			(LB_MSG_OBJX_UPDATE_BASE|7)
#define LB_MSG_CONT_UPDATE			(LB_MSG_OBJX_UPDATE_BASE|8)
#define LB_MSG_DDLIST_UPDATE			(LB_MSG_OBJX_UPDATE_BASE|9)
#define LB_MSG_FLIST_UPDATE			(LB_MSG_OBJX_UPDATE_BASE|10)
#define LB_MSG_GAUGE_UPDATE			(LB_MSG_OBJX_UPDATE_BASE|11)
#define LB_MSG_IMG_UPDATE			(LB_MSG_OBJX_UPDATE_BASE|12)
#define LB_MSG_IMGBTN_UPDATE			(LB_MSG_OBJX_UPDATE_BASE|13)
#define LB_MSG_KB_UPDATE			(LB_MSG_OBJX_UPDATE_BASE|14)
#define LB_MSG_LABEL_UPDATE			(LB_MSG_OBJX_UPDATE_BASE|15)
#define LB_MSG_LED_UPDATE			(LB_MSG_OBJX_UPDATE_BASE|16)
#define LB_MSG_LINE_UPDATE			(LB_MSG_OBJX_UPDATE_BASE|17)
#define LB_MSG_LIST_UPDATE			(LB_MSG_OBJX_UPDATE_BASE|18)
#define LB_MSG_LMETER_UPDATE			(LB_MSG_OBJX_UPDATE_BASE|19)
#define LB_MSG_MSGBOX_UPDATE			(LB_MSG_OBJX_UPDATE_BASE|20)
#define LB_MSG_PAGE_UPDATE			(LB_MSG_OBJX_UPDATE_BASE|21)
#define LB_MSG_PRELOAD_UPDATE			(LB_MSG_OBJX_UPDATE_BASE|22)
#define LB_MSG_ROLLER_UPDATE			(LB_MSG_OBJX_UPDATE_BASE|23)
#define LB_MSG_SLIDER_UPDATE			(LB_MSG_OBJX_UPDATE_BASE|24)
#define LB_MSG_SW_UPDATE			(LB_MSG_OBJX_UPDATE_BASE|25)
#define LB_MSG_TA_UPDATE			(LB_MSG_OBJX_UPDATE_BASE|26)
#define LB_MSG_TABVIEW_UPDATE			(LB_MSG_OBJX_UPDATE_BASE|27)
#define LB_MSG_WIN_UPDATE			(LB_MSG_OBJX_UPDATE_BASE|28)
#define LB_MSG_HIDDE_MULTI_IMGBTN		(LB_MSG_OBJX_UPDATE_BASE|29)
#define LB_MSG_HIDDE_OBJX			(LB_MSG_OBJX_UPDATE_BASE|30)
#define LB_MSG_HIDDE_MULTI_OBJX			(LB_MSG_OBJX_UPDATE_BASE|31)
#define LB_MSG_CANVAS_UPDATE			(LB_MSG_OBJX_UPDATE_BASE|32)
#define LB_MSG_CLICK_OBJX			(LB_MSG_OBJX_UPDATE_BASE|33)


/* App Messages. */
#define LB_MSG_HOME_BASE			    (LB_MSG_BASE|0x500)
#define LB_MSG_CAR_RECORDER_BASE		    (LB_MSG_BASE|0x600)
#define LB_MSG_FILEEXP_BASE			    (LB_MSG_BASE|0x700)
#define LB_MSG_MEDIA_PLAY_BASE			    (LB_MSG_BASE|0x800)
#define LB_MSG_CDR_SETTING_BASE			    (LB_MSG_BASE|0x900)
#define LB_MSG_SYSTEM_SETTING_BASE		    (LB_MSG_BASE|0xA00)
#define LB_MSG_SMART_DRIVE_BASE			    (LB_MSG_BASE|0xB00)

/* Cont Events*/
#define LB_MSG_CONT_EVENT			    (LB_MSG_CAR_RECORDER_BASE|0x01)
#define LB_MSG_CONT_EVENT_RIGHT			    (LB_MSG_CAR_RECORDER_BASE|0x01)
#define LB_MSG_CONT_EVENT_LEFT			    (LB_MSG_CAR_RECORDER_BASE|0x02)
#define LB_MSG_CONT_EVENT_UP			    (LB_MSG_CAR_RECORDER_BASE|0x03)
#define LB_MSG_CONT_EVENT_DOWN			    (LB_MSG_CAR_RECORDER_BASE|0x04)
#define LB_MSG_CONT_EVENT_STOP			    (LB_MSG_CAR_RECORDER_BASE|0x05)


/* File explore. */
#define LB_MSG_FILEEXP_CHANGE			    (LB_MSG_FILEEXP_BASE|0x00)
#define LB_MSG_FILEEXP_RETURN			    (LB_MSG_FILEEXP_BASE|0x01)
#define LB_MSG_FILEEXP_ENTER			    (LB_MSG_FILEEXP_BASE|0x11)
#define LB_MSG_FLIST_LOCK			    (LB_MSG_FILEEXP_BASE|0x13)
#define LB_MSG_FLIST_DEL			    (LB_MSG_FILEEXP_BASE|0x14)
#define LB_MSG_FORMAT_RETURN			    (LB_MSG_FILEEXP_BASE|0x20)
#define LB_MSG_DETECT_RETURN			    (LB_MSG_FILEEXP_BASE|0x21)
#define LB_MSG_SCH_KB_OK			    (LB_MSG_FILEEXP_BASE|0x30)
#define LB_MSG_SCH_KB_CLOSE			    (LB_MSG_FILEEXP_BASE|0x31)
#define LB_MSG_FORMAT_CARD_CONFIRM		    (LB_MSG_FILEEXP_BASE|0x42)
#define LB_MSG_DETECT_CARD_CONFIRM		    (LB_MSG_FILEEXP_BASE|0x43)
#define LB_MSG_FORMAT_CARD_START		    (LB_MSG_FILEEXP_BASE|0x44)
#define LB_MSG_DETECT_CARD_START		    (LB_MSG_FILEEXP_BASE|0x45)


#define LB_MSG_INVALID                              (LB_MSG_BASE|0xFFF)
#define LB_SYSTEM_MSG                               (0xF000)

#define LB_MSG_SIZE				128
#define LB_MSG_MAX_NUM				128

/*----------------------------------------------*
 * structs                  *
 *----------------------------------------------*/

typedef lb_int32(*pobj_init)(void *param);
typedef lb_int32(*pobj_resp_func)(void *msg_data);
typedef lb_int32(*psys_resp_func)(lb_uint32 msgtype, void *msg_data);

/* Message data definition */

typedef struct tag_lb_key_msg_data {
	lb_uint32	code;
	lb_int32	value;
} lb_key_msg_data_t;

typedef enum en_lang {
	LB_LANG_ENGLISH,
	LB_LANG_SIM_CHI,
	LB_LANG_TRA_CHI,
	LB_LANG_JAPANESE,
	LB_LANG_RUSSIAN,
	LB_LANG_SPANISH,
	LB_LANG_MAX
} lb_lang_e;

/* Style */
typedef struct tag_lb_al_style {
	lb_uint8 glass : 1;

	struct {
		lb_uint32 main_color;
		lb_uint32 grad_color;
		lb_int32 radius;
		lb_uint8 opa;

		struct {
			lb_uint32 color;
			lb_int32 width;
			lb_uint8 part;
			lb_uint8 opa;
		} border;

		struct {
			lb_uint32 color;
			lb_int32 width;
			lb_uint8 type;
		} shadow;

		struct {
			lb_int32 ver;
			lb_int32 hor;
			lb_int32 inner;
		} padding;

		lb_uint8 empty : 1;
	} body;


	struct {
		lb_uint32 color;
		const void *font;
		lb_int32 letter_space;
		lb_int32 line_space;
		lb_uint8 opa;
	} text;

	struct {
		lb_uint32 color;
		lb_uint8 intense;
		lb_uint8 opa;
	} image;

	struct {
		lb_uint32 color;
		lb_int32 width;
		lb_uint8 opa;
		lb_uint8 rounded:1;
	} line;
} lb_al_style_t;

/* Common */
typedef struct tag_lb_img_info {
	lb_uint8             type;
	char               *p_img_src;
	lb_uint8             src_list_count;
	char               **src_list;
} lb_img_info_t;

typedef struct tag_lb_point {
	lb_int32         x;
	lb_int32         y;
} lb_point_t;

typedef struct tag_lb_obj_comm_prop {
	lb_int32       x;
	lb_int32       y;
	lb_int32       w;
	lb_int32       h;
	lb_uint8       align_mode;
	lb_uint16      align_with;
	lb_int32       align_x_offset;
	lb_int32       align_y_offset;
	lb_uint8       in_which_tab;
	lb_uint8       in_which_btn;
	lb_uint8	click;
} lb_obj_comm_prop_t;

/* object property */
/* Arc */
typedef struct tag_lb_arc {
	lb_obj_comm_prop_t	comms;		/* object common property */
	lb_uint16		start_angle;	/* start angle value */
	lb_uint16		end_angle;	/* end angle value */
	lb_uint32		color;		/* color value */
	lb_int32		width;		/* width value */
	lb_uint16		style_main_idx;	/* main style index */
} lb_arc_t;

/* Bar */
typedef struct tab_lb_bar {
	lb_obj_comm_prop_t	comms;		/* object common property */
	lb_int16		min;		/* min value */
	lb_int16		max;		/* max value */
	lb_int16		cur;		/* current value */
	lb_uint32		bg_color;	/* background color value */
	lb_uint32		indic_maincolor;/* indic main color value */
	lb_uint32		indic_gradcolor;/* indic grad color value */
	lb_uint16		style_bg_idx;	/* background style index */
	lb_uint16		style_indic_idx;/* indic style index */
} lb_bar_t;

/* Btn */
typedef enum tag_lb_btn_state {
	LB_BTN_STATE_REL,
	LB_BTN_STATE_PR,
	LB_BTN_STATE_TGL_REL,
	LB_BTN_STATE_TGL_PR,
	LB_BTN_STATE_INA
} lb_btn_state_e;

typedef struct tag_lb_btn {
	lb_obj_comm_prop_t	comms;			/* object common property */
	lb_uint16		ink_in_time;		/* ink in time(ms) */
	lb_uint16		ink_wait_time;		/* ink wait time(ms) */
	lb_uint16		ink_out_time;		/* ink out time(ms) */
	lb_uint8		state;			/* btn state, details refer
							 * to 'lv_btn_state_t' */
	void			*p_label;		/* label object pointer */
	char			*txt;			/* btn text, dynamic allocates
							 * memory based on the length of
							 * text in different languages */
	lb_uint8		font_size;		/* font size */
	lb_uint32		str_id;			/* string id */
	char			*link;			/* the full path to the json file
							 * for the next level view of
							 * the btn link */
	lb_uint8		b_has_link;		/* 1. btn has link, 0. no link */
	lb_uint8		b_is_ret;		/* 1. return btn, 0. other btn */
	lb_uint32		click_resp_msg;		/* click response message */
	lb_uint32		pr_resp_msg;		/* press response message */
	lb_uint32		long_pr_resp_msg;	/* long press response message */
	lb_uint32		long_pr_repeat_msg;	/* long press repeat message */
	lb_uint16		style_rel_idx;		/* btn release style index */
	lb_uint16		style_pr_idx;		/* btn press style index */
	lb_uint16		style_tgl_rel_idx;	/* btn toggle release
							 * style index */
	lb_uint16		style_tgl_pr_idx;	/* btn toggle press style index */
	lb_uint16		style_ina_idx;		/* btn inactive style index */

	lb_uint8		has_dialog;		/* 1. a dialog, 0. no dialog */
	lb_int32		iso_dialog;		/* isocaite dialog id */
	lb_uint8		hidden;			/* 1. hidden, 0. show */
} lb_btn_t;

/* Btnm */
typedef struct lb_btnm_btn {
	lb_uint8		b_active:1;		/* 1. active, 0. inactive */
	lb_uint8		b_repeat:1;		/* 1. repeat, 0. not repeat */
	lb_uint8		b_hidden:1;		/* 1. hidden, 0. show*/
	lb_uint8		b_line_end:1;		/* 1. change to next line,
							 * 0. don't change to next line.
							 * it mean btn itself doesn't
							 * display, next btn display new
							 * line */
	lb_uint8		width:3;		/* btn width in units */
	char			*ptxt;			/* btn text */
	lb_uint32		str_id;			/* string id */
} lb_btnm_btn_t;

typedef struct tag_lb_btnm {
	lb_obj_comm_prop_t	comms;			/* object common property */
	lb_uint16		btn_num;		/* number of btn */
	char			**p_btn_map;		/* pointer a string array, details
							 * refer to lv_btnm_set_map
							 * function */
	lb_btnm_btn_t		*btns;			/* btn information struct */
	lb_uint32		click_resp_msg;		/* click response message */
	lb_uint16		style_bg_idx;		/* background style index */
	lb_uint16		style_rel_idx;		/* btn release style index */
	lb_uint16		style_pr_idx;		/* btn press style index */
	lb_uint16		style_tgl_rel_idx;	/* btn toggle release
							 * style index */
	lb_uint16		style_tgl_pr_idx;	/* btn toggle press style index */
	lb_uint16		style_ina_idx;		/* btn inactive style index */
} lb_btnm_t;

/* Calendar */
typedef struct tag_lb_calendar_date {
	lb_uint16        year;
	lb_int8         month;
	lb_int8         day;
} lb_calendar_date_t;

typedef struct tag_lb_calendar {
	lb_obj_comm_prop_t	comms;			/* object common property */
	lb_calendar_date_t	today;			/* today's date */
	lb_calendar_date_t	showed;			/* the date to display */
	lb_calendar_date_t	*highlight;		/* highlight date */
	lb_uint8		hilight_nums;		/* number of highlight*/
	lb_uint16		style_bg_idx;		/* background style index */
	lb_uint16		style_header_idx;	/* header style index */
	lb_uint16		style_header_pr_idx;	/* press header style index */
	lb_uint16		style_day_names_idx;	/* day name style index */
	lb_uint16		style_highlighted_days_idx;/* highlighted days
							    * style index */
	lb_uint16		style_inactive_days_idx;/* inactive days style index */
	lb_uint16		style_week_box_idx;	/* week box style index */
	lb_uint16		style_today_box_idx;	/* today box style index */
} lb_calendar_t;

/* Canvas */
typedef struct tag_lb_canvas_polygon {
	lb_point_t		points[8];	/* points array, current max support
						 * 8 points */
	lb_uint8		point_nums;	/* number of points*/
	lb_uint32		line_color;	/* polygon line color(0-0xFFFFFF) */
	lb_uint8		en_fill;	/* enable fill polygon area color,
						 * 1. enable, 0. disable */
	lb_uint32		fill_color;	/* fill color(0-0xFFFFFFFF) */
} lb_canvas_polygon_t;

typedef struct tag_lb_canvas_circle {
	lb_point_t		coordinate;	/* coordinate position */
	lb_int32		radius;		/* radius value */
	lb_uint32		border_color;	/* border color */
	lb_uint8		en_fill;	/* enable fill circle area color,
						 * 1. enable, 0. disable */
	lb_uint32		fill_color;	/* fill color(0-0xFFFFFFFF) */
} lb_canvas_circle_t;

typedef struct tag_lb_canvas_image {
	lb_point_t		coordinate;	/* start position */
	void			*p_image_dsc;	/* image file data */
	char			*image_src;	/* image source pointer */
} lb_canvas_image_t;

typedef struct tag_lb_canvas {
	lb_obj_comm_prop_t	comms;		/* object common property */
	lb_uint32		bg_color;	/* bg color(0-0xFFFFFF) */
	lb_uint8		color_format;	/* color format(6-10), default is 4,
						 * if it is 6, support alpha color,
						 * details refer to 'lv_img_cf_t'*/
	void			*buffer;	/* bg pixel array */
	lb_uint8		image_count;	/* image count */
	lb_canvas_image_t	*image_list;	/* image data pointer */
	lb_uint8		polygon_count;	/* polygon count */
	lb_canvas_polygon_t	*polygon_list;	/* polygon data pointer */
	lb_uint8		circle_count;	/* circle count */
	lb_canvas_circle_t	*circle_list;	/* circle data pointer */
	lb_uint8		hidden;		/* 1. hidden, 0. show */
	lb_uint16		style_main_idx;	/* main style index */
} lb_canvas_t;

/* Chart */
typedef struct tag_lb_chart_series {
	lb_int32			*points;	/* in the y direction value */
	lb_uint32			color;		/* series line color */
} lb_chart_series_t;

typedef struct tag_lb_chart {
	lb_obj_comm_prop_t	comms;		/* object common property */
	lb_int32		series_width;	/* the width of a line in a line
						 * graph and the radius of a point
						 * in a point graph */
	lb_uint8		series_opa;	/* the opa of the line */
	lb_uint8		series_dark;	/* dark levels at the bottom of
						 * apoint or pillar */
	lb_uint8		chart_type;	/* chart type, details refer to
						 * 'lv_chart_type_t' */
	lb_int32		ymin;		/* y-axis minimum */
	lb_int32		ymax;		/* y-axis maximum */
	lb_uint16		point_count;	/* Point number in a data line */
	lb_uint8		series_num;	/* Number of data lines */
	lb_uint8		hdivs;		/* the horizontal compoent */
	lb_uint8		vdivs;		/* the verticial compoent */
	lb_chart_series_t	*p_series;	/* point array */
	void			**chart_series;	/* pointer to the allocated data series */
	lb_uint16		style_idx;	/* style index */
} lb_chart_t;

/* Checkbox */
typedef struct tag_lb_checkbox {
	lb_obj_comm_prop_t	comms;			/* object common property */
	char			*ptxt;			/* checkbox text */
	lb_uint32		str_id;			/* string id */
	lb_uint8		state;			/* checkbox state: 0. release,
							 * 1. toggle release,
							 * 2. inactive */
	lb_uint32		click_resp_msg;		/* click response message */
	lb_uint16		style_bg_idx;		/* background style index */
	lb_uint16		style_box_rel_idx;	/* box release style index */
	lb_uint16		style_box_pr_idx;	/* box press style index */
	lb_uint16		style_box_tgl_rel_idx;	/* box toggle release
							 * style index */
	lb_uint16		style_box_tgl_pr_idx;	/* box toggle press style index */
	lb_uint16		style_box_ina_idx;	/* box inactive style index */
} lb_checkbox_t;

typedef struct tag_cont_event_info {
	lb_int32		x_offset;
	lb_int32		y_offset;
} lb_cont_event_info_t;

/* Container */
typedef struct tag_container {
	lb_obj_comm_prop_t	comms;		/* object common property */
	lb_uint8		hidden;		/* 1. hidden, 0. show */
	lb_uint32		click_resp_msg;	/* click response message */
	lb_uint8		catch_event;	/* 1. enable catch events */
	char			*link;		/* the full path to the json file
						 * for the next level view of
						 * the btn link */
	lb_uint16		style_idx;	/* style index */
} lb_cont_t;

/* Ddlist */
typedef struct tag_lb_ddlist {
	lb_obj_comm_prop_t	comms;		/* object common property */
	lb_int16		anim_time;	/* animation time(ms)*/
	lb_uint32		*p_options;	/* array of string indexed values */
	lb_uint16		options;	/* number of options */
	char			*p_ops_str;	/* options string */
	lb_uint32		click_resp_msg;	/* click response message */
	lb_uint16		style_bg_idx;	/* background style index */
	lb_uint16		style_sel_idx;	/* select style index */
	lb_uint16		style_sb_idx;	/* scroll bar style index */
} lb_ddlist_t;

/* Flist */
typedef enum tag_lb_file_type {
	DIR_TYPE	= 0x00,
	FILE_TYPE	= 0x01,
	VIDEO_TYPE	= 0x02,
	PIC_TYPE	= 0x03,
	AUDIO_TYPE	= 0x04,
	EBOOK_TYPE	= 0x05,
	MAX_TYPE	= 0x06,
} lb_file_type_t;

typedef enum tag_lb_file_bin {
	BIN_FILE	= 0x00,
	BIN_MAX		= 0x04,
} lb_file_bin_t;

typedef enum tag_lb_file_lock {
	LOCKED_FILE	= 0x00,
	UNLOCK_FILE	= 0x01,
	LOCK_MAX	= 0x01,

} lb_file_lock_t;

typedef struct tag_lb_file_attr {
	lb_int16	name_align;	/* file name label align type, details
					 * refer to lv_align_t */
	lb_int32	name_x_offset;	/* file name label x direction offest */
	lb_int32	name_y_offset;	/* file name label y direction offest */
	lb_uint32	name_has_dialog;/* 1. a dialog, 0. no dialog */
	lb_uint32	name_iso_dialog;/* name isolate dialog id */

	lb_img_info_t	*type_src;	/* file type picture info */
	void		**type_data;	/* file type picture array pointer */
	lb_int16	type_num;	/* number of file type picture */
	lb_int16	type_align;	/* type imgbtn align type, details
					 * refer to lv_align_t */
	lb_int32	type_x_offset;	/* type imgbtn x direction offest */
	lb_int32	type_y_offset;	/* type imgbtn y direction offest */
	lb_uint32	type_has_dialog;/* 1. a dialog, 0. no dialog */
	lb_uint32	type_iso_dialog;/* type isolate dialog id */

	lb_img_info_t	*lock_src;	/* file lock picture info */
	void		**lock_data;	/* file lock picture array pointer */
	lb_int16	lock_num;	/* number of file lock picture */
	lb_int16	lock_align;	/* lock imgbtn align type, details
					 * refer to lv_align_t */
	lb_int32	lock_x_offset;	/* lock imgbtn x direction offest */
	lb_int32	lock_y_offset;	/* lock imgbtn y direction offest */
	lb_uint32	lock_has_dialog;/* 1. a dialog, 0. no dialog */
	lb_uint32	lock_iso_dialog;/* lock isolate dialog id */
	lb_uint32	unlock_iso_dialog;/* unlock isolate dialog id */

	lb_img_info_t	*bin_src;	/* file bin picture info */
	void		**bin_data;	/* file bin picture array pointer */
	lb_int16	bin_num;	/* number of file bin picture */
	lb_int16	bin_align;	/* bin imgbtn align type, details
					 * refer to lv_align_t */
	lb_int32	bin_x_offset;	/* bin imgbtn x direction offest */
	lb_int32	bin_y_offset;	/* bin imgbtn y direction offest */
	lb_uint32	bin_has_dialog;	/* 1. a dialog, 0. no dialog */
	lb_uint32	bin_iso_dialog;	/* bin isolate dialog id */
} lb_file_attr_t;

typedef struct tag_lb_file_array {
	void		*btn;		/* pointer to a file of current btn */
	char		*file_name;	/* file name */
	lb_int16	file_index;	/* fiel index */
	lb_int8		type_used;	/* file type used or not */
	lb_file_type_t	file_type;	/* file type, 0. file, 1. dir */
	lb_int8		lock_used;	/* file lock used or not */
	lb_file_lock_t	file_lock;	/* lock file, 0. lock, 1. unlock */
	lb_int8		bin_used;	/* file bin used or not */
	lb_file_bin_t	file_bin;	/* bin file */
	lb_int8		thumb_used;	/* file thumb used or not */
	lb_int8		databs_flag;	/* file thumb from databs or not */
	lb_int8		decode_flag;	/* file thumb from decode or not */
	void		*thumb_data;	/* file thumb */
} lb_file_array_t;

typedef struct tag_lb_file_pri {
	void		*cur_list;	/* pointer to the list object */
	void		*cur_desert;	/* pointer to a file of current type */
	lb_int16	sand_flag;	/* sand flag */
	char		*sand_str;	/* sand string */
	lb_int16	cur_type;	/* current list file type */
	char		*cur_path;	/* current path */
	void		*cur_elem;	/* reserved */
	lb_int32	row_index;	/* row index */
	void		*row_obj;	/* pointer to row object */
	lb_int32	col_index;	/* col index */
	void		*col_obj;	/* pointer to col object */
} lb_file_pri_t;

typedef struct tag_lb_file_dialog {
	void		*link_pext;
	void	(*require_func)(void *);
	void		*require_param;
} lb_file_dialog_t;


typedef struct tag_lb_flist {
	lb_obj_comm_prop_t	comms;	/* object common property */
	lb_uint16		options_h;	/* the height of options */
	char			*link;		/* the full path to the json file
						 * for the next level view of
						 * the btn link */
	lb_uint8		sb_mode;	/* scroll bar mode*/
	lb_uint8		b_has_link;	/* 1. btn has link, 0. no link */
	lb_file_attr_t		file_attr;	/* file attribute */
	lb_file_pri_t		file_pri;	/**/
	lb_file_array_t		*file_array;	/* dynamic linked list of file info */
	lb_file_dialog_t	file_dialog;	/* file dialog */
	lb_uint32		file_num;	/* number of files */
	lb_int32		sel_btn;
	lb_uint16		style_flist_bg_idx;	/* flist background style index */
	lb_uint16		style_flist_scrl_idx;	/* flist scroll sections
							 * style index */
	lb_uint16		style_flist_sb_idx;	/* flist scroll bar style index */
	lb_uint16		style_flist_rel_idx;	/* flist release style index */
	lb_uint16		style_flist_pr_idx;	/* flist press style index */
	lb_uint16		style_flist_tgl_rel_idx;/* flist toggle release
							 * style index */
	lb_uint16		style_flist_tgl_pr_idx;	/* flist toggle press
							 * style index */
	lb_uint16		style_flist_ina_idx;	/* flist inactive style index */

	lb_uint16		style_name_label_idx;	/* name label style index */

	lb_uint16		style_list_btn_rel_idx;	/* element list btn release
							 * style index */
	lb_uint16		style_list_btn_pr_idx;	/* element list btn press
							 * style index */
	lb_uint16		style_list_btn_tgl_rel_idx;/* element list btn toggle
							    * release style index */
	lb_uint16		style_list_btn_tgl_pr_idx;/* element list btn toggle
							   * press style index */
	lb_uint16		style_list_btn_ina_idx;	/* element list btn inactive
							 * style index */

	lb_uint16		style_type_rel_idx;	/* type imgbtn release style
							 * index */
	lb_uint16		style_type_pr_idx;	/* type imgbtn press style
							 * index */
	lb_uint16		style_type_tgl_rel_idx;	/* type imgbtn toggle release
							 * style index */
	lb_uint16		style_type_tgl_pr_idx;	/* type imgbtn toggle press style
							 * index */
	lb_uint16		style_type_ina_idx;	/* type imgbtn active style
							 * index */

	lb_uint16		style_lock_rel_idx;	/* lock imgbtn release style
							 * index */
	lb_uint16		style_lock_pr_idx;	/* lock imgbtn press style
							 * index */
	lb_uint16		style_lock_tgl_rel_idx;	/* lock imgbtn toggle release
							 * style index */
	lb_uint16		style_lock_tgl_pr_idx;	/* lock imgbtn toggle press style
							 * index */
	lb_uint16		style_lock_ina_idx;	/* lock imgbtn active style
							 * index */

	lb_uint16		style_bin_rel_idx;	/* bin imgbtn release style
							 * index */
	lb_uint16		style_bin_pr_idx;	/* bin imgbtn press style
							 * index */
	lb_uint16		style_bin_tgl_rel_idx;	/* bin imgbtn toggle release
							 * style index */
	lb_uint16		style_bin_tgl_pr_idx;	/* bin imgbtn toggle press style
							 * index */
	lb_uint16		style_bin_ina_idx;	/* bin imgbtn active style
							 * index */
} lb_flist_t;

/* Gauge */
typedef struct tag_lb_gauge {
	lb_obj_comm_prop_t	comms;		/* object common property */
	lb_uint8		needle_cnt;	/* count of needle */
	lb_uint32		*needle_colors;	/* needle color array */
	void			*p_colors;	/* converted color pointer */
	lb_int16		*needle_values;	/* needle value array */
	lb_int16		critical_value;	/* critical value */
	lb_int16		min;		/* minimun value */
	lb_int16		max;		/* maximun value */
	lb_uint16		angle;		/* angle value */
	lb_uint8		line_cnt;	/* number of line */
	lb_uint8		label_cnt;	/* number of label */
	lb_uint16		style_idx;	/* style index */
} lb_gauge_t;

/* Img */
typedef struct tag_lb_img {
	lb_obj_comm_prop_t	comms;		/* object common property */
	void			*src;		/* image source pointer */
	lb_uint8		src_list_count;	/* count of source */
	void			*p_img_dsc;	/* image file data */
	char			**src_list;	/* image source array pointer */
	lb_uint8		src_type:2;	/* image source type, 0. image variable
						 * 1. image file, 2. symbol*/
	lb_uint8		auto_size:1;	/* automatically resizes images, 1. auot
						 * 0. normal */
	lb_uint8		color_format:5;	/* color format, value range(0~14) */
	lb_uint16		style_idx;	/* style index */
	lb_uint8		hidden;
} lb_img_t;

/* Imgbtn */
typedef struct tag_lb_imgbtn {
	lb_obj_comm_prop_t	comms;			/* object common property */
	lb_img_info_t		rel_img;		/* release state image info */
	lb_img_info_t		pr_img;			/* press state image info */
	lb_img_info_t		tgl_rel_img;		/* toggle release state
							 * image info */
	lb_img_info_t		tgl_pr_img;		/* toggle press state
							 * image info */
	lb_img_info_t		ina_img;		/* inactive state image info */
	void			*p_img_rel;		/* release state image data */
	void			*p_img_pr;		/* press state image data */
	void			*p_img_tgl_rel;		/* toggle release state
							 * image data */
	void			*p_img_tgl_pr;		/* toggle release state
							 * image data */
	void			*p_img_ina;		/* inactive state image data */
	void			*p_label;		/* label object pointer */
	char			*ptxt;			/* imgbtn text */
	lb_uint8		font_size;		/* font size */
	lb_uint32		txt_id;			/* string id */
	lb_uint8		txt_layout;		/* align type, value range(0-8) */
	lb_int32		txt_offset_x;		/* x direction offest */
	lb_int32		txt_offset_y;		/* y direction offest */
	lb_uint8		txt_long_mode;		/* long text mode, details refer
							 * to 'lv_label_long_mode_t' */
	lb_uint8		txt_align;		/* txt align mode */
	char			*link;			/* the full path to the json file
							 * for the next level view of
							 * the btn link */
	lb_uint8		b_has_link;		/* 1. btn has link, 0. no link */
	lb_uint8		b_is_ret;		/* 1. return btn, 0. other btn */
	lb_uint32		click_resp_msg;		/* click response message */
	lb_uint32		pr_resp_msg;		/* press response message */
	lb_uint32		long_pr_resp_msg;	/* long press response message */
	lb_uint32		long_pr_repeat_msg;	/* long press repeat message */
	lb_uint16		style_rel_idx;		/* btn release style index */
	lb_uint16		style_pr_idx;		/* btn press style index */
	lb_uint16		style_tgl_rel_idx;	/* btn toggle release
							 * style index */
	lb_uint16		style_tgl_pr_idx;	/* btn toggle press style index */
	lb_uint16		style_ina_idx;		/* btn inactive style index */

	lb_uint8		has_dialog;		/* 1. a dialog, 0. no dialog */
	lb_int32		iso_dialog;		/* isocaite dialog id */
	lb_uint8		pr_init;		/* init function id */
	lb_uint8		hidden;
	lb_uint8		state;
} lb_imgbtn_t;

/* Kb */
typedef struct tag_lb_kb {
	lb_obj_comm_prop_t	comms;			/* object common property */
	lb_uint32		ok_msg;			/* the value of the message sent
							 * when click ok btn */
	lb_uint32		hide_msg;		/* the value of the message sent
							 * when click hide btn */
	lb_uint8		mode;			/* keyboard mode, details refer to
							 * 'lv_kb_mode_t' */
	lb_uint16		style_bg_idx;		/* background style index */
	lb_uint16		style_rel_idx;		/* btn release style index */
	lb_uint16		style_pr_idx;		/* btn press style index */
	lb_uint16		style_tgl_rel_idx;	/* btn toggle release
							 * style index */
	lb_uint16		style_tgl_pr_idx;	/* btn toggle press style index */
	lb_uint16		style_ina_idx;		/* btn inactive style index */
} lb_kb_t;

/* Label */
typedef struct tag_lb_label {
	lb_obj_comm_prop_t	comms;		/* object common property */
	lb_uint32		str_id;		/* string id */
	lb_uint8		long_mode;	/* long text mode, details refer to
						 * 'lv_label_long_mode_t' */
	lb_uint8		text_align;
	lb_uint8		font_size;	/* font size value */
	char			*txt;		/* label text */
	lb_uint16		style_idx;	/* style index */
	lb_uint8		hidden;		/* 1. hidden, 0. show */
} lb_label_t;

/* Led */
typedef struct tag_lb_led {
	lb_obj_comm_prop_t	comms;		/* object common property */
	lb_uint8		bright;		/* bright value(0..255)*/
	lb_uint8		on;		/* led state, 0. off, 1. on */
	lb_uint16		style_idx;	/* style index */
} lb_led_t;

/* Line */
typedef struct tag_lb_line {
	lb_obj_comm_prop_t	comms;		/* object common property */
	lb_uint16		point_num;	/* number of line */
	lb_point_t		*points;	/* points array*/
	lb_uint8		auto_size:1;	/* automatic sizing, 1. auto, 0. normal */
	lb_uint8		y_inv:1;	/* y direction flip, 1. yes, 0. no */
	lb_uint16		style_idx;	/* style index */
	lb_uint8		hidden;		/* 1. hidden, 0. show*/
} lb_line_t;

/* List */
typedef struct tag_lb_list {
	lb_obj_comm_prop_t	comms;			/* object common property */
	lb_uint16		anim_time;		/* animation time */
	lb_uint8		sb_mode:3;		/* scroll bar mode */
	lb_uint8		b_has_link:1;		/* 1. btn has link, 0. no link */
	char			**options_links;	/* option link to the path list
							 * of the json file in the
							 * next view */
	void			**options;
	lb_uint8		options_num;		/* number of options */
	lb_int32		options_w;		/* option's width */
	lb_int32		options_h;		/* option's heigth */
	lb_int32		sel_btn;		/* select button index */
	lb_uint32		click_resp_msg;		/* click response message value */
	lb_uint8		reserved:2;		/* reserved */

	lb_uint16		style_bg_idx;		/* background style index */
	lb_uint16		style_scrl_idx;		/* scroll section style index */
	lb_uint16		style_sb_idx;		/* scroll bar style index */
	lb_uint16		style_rel_idx;		/* btn release style index */
	lb_uint16		style_pr_idx;		/* btn press style index */
	lb_uint16		style_tgl_rel_idx;	/* btn toggle release
							 * style index */
	lb_uint16		style_tgl_pr_idx;	/* btn toggle press style index */
	lb_uint16		style_ina_idx;		/* btn inactive style index */
} lb_list_t;

/* Lmeter */
typedef struct tag_lb_lmeter {
	lb_obj_comm_prop_t	comms;		/* object common property */
	lb_uint16		angle;		/* angle value(0..360) */
	lb_uint8		line_cnt;	/* count of line */
	lb_int16		min;		/* minimum value */
	lb_int16		max;		/* maximum value */
	lb_int16		cur;		/* currenr value */
	lb_uint16		style_idx;	/* style index */
} lb_lmeter_t;

/* Msgbox */
typedef struct tag_lb_msgbox {
	lb_obj_comm_prop_t	comms;			/* object common property */
	char			*txt;			/* msgbox text */
	lb_uint16		delay;			/* auto close time(ms) */
	lb_int32		str_id;			/* string id */
	lb_int32		toggle_id;			/* count of btns */
	lb_int32		offset;			/* if btns is 0, it is the
							 * distance of between text and
							 * the top of object; otherwise,
							 * it is the distance of between
							 * text and btnm  */
	lb_uint8		btns;			/* count of btns */
	lb_uint32		*btn_str_ids;		/* btn string index pointer */
	/* btn string array pointer */
	char			*btn_names[LB_MBOX_MAX_BTNS + 1];
	lb_uint32		close_msg;		/* the value of the message sent
							 * when msgbox closed */
	lb_uint16		style_bg_idx;		/* background style index */
	lb_uint16		style_btn_bg_idx;	/* btn background style index */
	lb_uint16		style_rel_idx;		/* btn release style index */
	lb_uint16		style_pr_idx;		/* btn press style index */
	lb_uint16		style_tgl_rel_idx;	/* btn toggle release
							 * style index */
	lb_uint16		style_tgl_pr_idx;	/* btn toggle press style index */
	lb_uint16		style_ina_idx;		/* btn inactive style index */
} lb_msgbox_t;

/* Page */
typedef struct tag_lb_page {
	lb_obj_comm_prop_t	comms;		/* object common property */
	lb_uint8		sb_mode;	/* scroll bar mode*/
	lb_uint8		en_arrow;	/* allow the arrow, 1. enable,
						 * 0. disable */
	lb_uint8		en_hor_fit;	/* horizontal adaptation, 1. enable,
						 * 0. disable */
	lb_uint8		en_ver_fit;	/* vertical adaptation, 1. enable,
						 * 0. disable */
	lb_int32		scrl_width;	/* the width of scroll bar */
	lb_int32		scrl_height;	/* the height of scroll bar */
	lb_uint8		layout;		/* align type(0-8),details refer to
						 * 'lv_align_t' */
	lb_uint32		rel_msg;	/* the value of message sent when
						 * released*/
	lb_uint32		pr_msg;		/* the value of message sent when
						 * pressed*/
	lb_uint16		style_bg_idx;	/* background style index */
	lb_uint16		style_scrl_idx;	/* scroll section style index */
	lb_uint16		style_sb_idx;	/* scroll bar style index */
} lb_page_t;

/* Preload */
typedef struct tag_lb_preload {
	lb_obj_comm_prop_t	comms;		/* object common property */
	lb_uint16		arc_len;	/* the length of arc */
	lb_uint16		spin_time;	/* time of one round in milliseconds */
	lb_uint16		style_main_idx;	/* main style index */
} lb_preload_t;

/* Roller */
typedef struct tag_lb_roller {
	lb_obj_comm_prop_t	comms;		/* object common property */
	lb_uint8		visible_cnt;	/* the count of visible sections */
	lb_uint8		en_hor_fit;	/* horizontal adaptation, 1. enable,
						 * 0. disable */
	lb_uint16		selected;
	lb_uint16		anim_time;	/* animation time(ms) */
	lb_uint16		ops_num;	/* number of options */
	lb_uint32		*p_ops_ids;	/* options text array */
	char			*p_ops_str;	/* options string */
	lb_uint32		sel_msg;	/* the value of message sent
						 * when selected */
	lb_uint16		style_bg_idx;	/* background style index */
	lb_uint16		style_sel_idx;	/* select style inedx */
} lb_roller_t;

/* Slider */
typedef struct tag_lb_slider {
	lb_obj_comm_prop_t	comms;		/* object common property */
	lb_uint8		knob_in:1;	/* 1. the knob is drawn always in
						 * the slider; 0. the knob can be
						 * out on the edges*/
	lb_uint8		reserved:7;	/* reserved */
	lb_int16		min;		/* minimum value */
	lb_int16		max;		/* maximum value */
	lb_int16		cur;		/* current value */
	lb_uint32		ch_msg;		/* the value of message sent when slide
						 * position changed */
	lb_uint16		style_bg_idx;	/* background style index */
	lb_uint16		style_indic_idx;/* indic style index */
	lb_uint16		style_knob_idx;	/* knob style index */
} lb_slider_t;

/* Sw */
typedef struct tag_lb_sw {
	lb_obj_comm_prop_t	comms;			/* object common property */
	lb_uint32		sw_msg;			/* the value of message sent
							 * when the state of the
							 * switch changed */
	lb_uint8		def_state;		/* default state, 1. on, 0. off */
	lb_uint16		style_bg_idx;		/* background style index */
	lb_uint16		style_indic_idx;	/* indic style index */
	lb_uint16		style_knob_off_idx;	/* knob off style index */
	lb_uint16		style_knob_on_idx;	/* knob on style index */
} lb_sw_t;

/* Ta */
typedef struct tag_lb_ta {
	lb_obj_comm_prop_t	comms;		/* object common property */
	lb_uint16		max_len;	/* maximum length of text that
						 * can be displayed */
	lb_uint8		pwd_mode:1;	/* passward mode, 1. passward, all
						 * display '*', 0. normal */
	lb_uint8		one_line_mode:1;/* single mode, 1. single line,
						 * 0. multi-line */
	lb_uint8		cursor_type:2;	/* cursor type, details refer to
						 * 'lv_cursor_type_t' */
	char			*p_text;	/* textarea text */
	lb_uint32		click_msg;	/* the value of message sent when
						 * clicked */
	lb_uint16		style_bg_idx;	/* background style index */
	lb_uint16		style_sb_idx;	/* scroll bar style index */
	lb_uint16		style_cursor_idx;/* cursor style index */
} lb_ta_t;

/* Tabview */
typedef struct tag_lb_tabview {
	lb_obj_comm_prop_t	comms;			/* object common property */
	lb_uint16		tab_cnt;		/* count of tabview */
	lb_uint16		tab_cur;		/* current tabview */
	lb_uint32		*p_tab_name_id;		/* tanview array of indexed
							 * values for string */
	lb_uint16		anim_time;		/* animation time(ms) */
	lb_uint8		en_sliding:1;		/* enable sliding, 1. enable,
							 * 0. disable */
	lb_uint8		draging:1;		/* enable draging, 1. enable,
							 * 0. disable */
	lb_uint8		drag_hor:1;		/* enable draging horizontal,
							 * 1. enable, 0. disable */
	lb_uint8		btn_pos:1;		/* the position of tab select
							 * buttons*/
	lb_uint32		load_msg;		/* the value of message sent
							 * when clicked */
	lb_uint16		style_tab_bg_idx;	/* tab background style index */
	lb_uint16		style_tab_scrl_idx;	/* tab scrollable style index */
	lb_uint16		style_bg_idx;		/* background style index */
	lb_uint16		style_indic_idx;	/* indic style index */
	lb_uint16		style_btn_bg_idx;	/* btn background style index */
	lb_uint16		style_btn_rel_idx;	/* btn release style index */
	lb_uint16		style_btn_pr_idx;	/* btn press style index */
	lb_uint16		style_btn_tgl_rel_idx;	/* btn toggle release
							 * style index */
	lb_uint16		style_btn_tgl_pr_idx;	/* btn toggle press style index */
} lb_tabview_t;

/* Win */
typedef struct tag_lb_win {
	lb_obj_comm_prop_t	comms;			/* object common property */
	lb_uint8		sb_mode;		/* scroll bar mode */
	lb_uint8		layout;			/* details refer to
							 * 'lv_layout_t' */
	lb_uint32		title_id;		/* title string id */
	lb_uint32		content_id;		/* content string id */
	char			*p_title;		/* title string */
	char			*p_content;		/* content string */
	lb_uint16		style_bg_idx;		/* background style index */
	lb_uint16		style_content_bg_idx;	/* content background style
							 * index */
	lb_uint16		style_content_scrl_idx;	/* content scroll section style
							 * index */
	lb_uint16		style_sb_idx;		/* scroll bar style index */
	lb_uint16		style_header_idx;	/* header style index */
	lb_uint16		style_btn_rel_idx;	/* btn release style index */
	lb_uint16		style_btn_pr_idx;	/* btn press style index */
} lb_win_t;

/* View */
typedef enum tag_in_type {
	LB_IN_TYPE_VIEW = 0,	/* the object is not in any container. */
	LB_IN_TYPE_CONTAINER,	/* the object is in a container object. */
	LB_IN_TYPE_PAGE,	/* the object is in a page object. */
	LB_IN_TYPE_TABVIEW,	/* the object is in a tab view object. */
	LB_IN_TYPE_WIN,		/* the object is in a window object. */
	LB_IN_TYPE_LIST,	/* the object is in a list object. */
	LB_IN_TYPE_MAX
} lb_in_type_e;

typedef enum en_view_type {
	LB_VIEW_TYPE_STATIC,
	LB_VIEW_TYPE_ISOLATE,
	LB_VIEW_MAX
} lb_view_type_e;

typedef enum en_obj_type {
	LB_ARC,
	LB_BAR,
	LB_BUTTON,
	LB_BTNM,
	LB_CALENDAR,
	LB_CHECKBOX,
	LB_CHART,
	LB_CONTAINER,
	LB_DDLIST,
	LB_GAUGE,
	LB_IMG,
	LB_IMGBTN,
	LB_KB,
	LB_LABEL,
	LB_LED,
	LB_LINE,
	LB_LIST,
	LB_LMETER,
	LB_MSGBOX,
	LB_PAGE,
	LB_PRELOAD,
	LB_ROLLER,
	LB_SLIDER,
	LB_SW,
	LB_TA,
	LB_TABVIEW,
	LB_WINDOW,
	LB_FILE_LIST,
	LB_CANVAS,
	LB_MAX_TYPE
} lb_obj_type_e;

typedef struct tag_lb_view_bg {
	lb_obj_type_e		type;		/* object type, details refer to
						 * 'lb_obj_type_e' */
	void			*property;	/* background object property */
} lb_view_bg_t;

typedef struct tag_lb_obj {
	lb_obj_type_e		type;		/* object type, details refer to
						 * 'lb_obj_type_e' */
	lb_uint32		id;		/* object id */
	void			*property;	/* object property */
	lb_in_type_e		in_type;	/* a type that object in a container-class
						 * object, details refer to
						 * 'lb_in_type_e' */
	lb_uint32		in_which_obj;	/* container-class object id */
	lb_uint32		in_which_btn;	/* button object index */
	void			*pext;		/* reserved to save the pointer of the
						 * actual object to show */
	void			*pview;		/**/
	pobj_init		p_init_func;	/* init function, it called when
						 * show object */
	pobj_init		p_exit_func;	/* exit function, it called when
						 * delete object */
	lb_uint8		have_init_func; /* wheter need exit function */
	struct tag_lb_obj	*next;		/* pointer next object */
} lb_obj_t;

typedef struct tag_lb_view {
	lb_view_type_e		type;
	lb_uint8		layer;	/**/
	lb_view_bg_t		bg;	/* the view of background infomation */
	lb_obj_t		*pobjs;	/* pointer the view of objects */
	void			*pext;	/* pointer the current view of ext */
	struct tag_lb_view	*child;	/* pointer child view */
	struct tag_lb_view	*par;	/* pointer parent view */
	struct tag_lb_view	*next;	/* pointer next view */

	void	*link_pext;		/* Maybe current view need to switch
					 * to another view */
	void	(*require_func)(void *);/* Maybe current view need to do something
					 * when dialog closed */
	void	*require_param;		/* Maybe current view need to do something
					 * when dialog closed */

} lb_view_t;

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
 * lb_style_init - init sytle
 * @filename: style json file.
 *
 * This function use to init style.
 *
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_style_init(char *filename);

/**
 * lb_reg_resp_msg_func - register response message function
 * @msg: message value.
 * @pobj_resp_func: response function.
 *
 * This function use to register response message function for app.
 *
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_reg_resp_msg_func(lb_uint32 msg, pobj_resp_func pfunc);

/**
 * lb_unreg_resp_msg_func - unregister response message function
 * @pobj_resp_func: response function.
 *
 * This function use to unregister response message function for app.
 *
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_unreg_resp_msg_func(pobj_resp_func pfunc);

/**
 * lb_reg_resp_sysmsg_func - register system response message function
 * @msg: system message value.
 * @pobj_resp_func: system response function.
 *
 * This function use to register system response message function for app.
 *
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_reg_resp_sysmsg_func(lb_uint32 msg, psys_resp_func pfunc);

/**
 * lb_unreg_resp_sysmsg_func - unregister response system message function
 * @pobj_resp_func: system response function.
 *
 * This function use to unregister system response message function for app.
 *
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_unreg_resp_sysmsg_func(psys_resp_func pfunc);

/**
 * lb_fmngr_reg_init_func - register init function
 * @p_name: init function name string.
 * @pfunc: init fuction.
 *
 * This function use to register init function for app.
 *
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_fmngr_reg_init_func(char *p_name, pobj_init pfunc);

/**
 * lb_fmngr_reg_exit_func - register exit function
 * @p_name: exit function name string..
 * @pfunc: exit function.
 *
 * This function use to register exit function for app.
 *
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_fmngr_reg_exit_func(char *p_name, pobj_init pfunc);

/**
 * lb_fmngr_unreg_exit_func - unregister exit function
 * @pfunc: exit function.
 *
 * This function use to unregister exit function for app.
 *
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_fmngr_unreg_exit_func(pobj_init pfunc);

/**
 * lb_fmngr_unreg_init_func - unregister init function
 * @pfunc: init fuction.
 *
 * This function use to unregister init function for app.
 *
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_fmngr_unreg_init_func(pobj_init pfunc);

/**
 * lb_view_get_static_head - get static view head
 *
 * This function use to get static view head.
 *
 * Returns view pointer.
 */
lb_view_t *lb_view_get_static_head(void);

/**
 * lb_view_get_parent - get parent view
 *
 * This function use to get parent view.
 *
 * Returns view pointer.
 */
lb_view_t *lb_view_get_parent(void);

/**
 * lb_view_get_current - get current view
 *
 * This function use to get current view.
 *
 * Returns view pointer.
 */
lb_view_t *lb_view_get_current(void);

/**
 * lb_view_set_require_func - set view require function
 * @view: view pointer.
 * @require_func: require function.
 *
 * This function use to set specified view of require function.
 *
 * Returns no value.
 */
void lb_view_set_require_func(lb_view_t *view, void *require_func);

/**
 * lb_view_set_require_param - set view require param
 * @view: view pointer.
 * @require_param: require param.
 *
 * This function use to set specified view of require param.
 *
 * Returns no value.
 */
void lb_view_set_require_param(lb_view_t *view, void *require_param);

/**
 * lb_view_check_isolate_is_openning - check isolate view whether openning
 * @idx: isolate view index.
 *
 * This function use to check check isolate view open status by index.
 *
 * Returns -true, it is openning; otherwise, it is closed or not exist.
 */
bool lb_view_check_isolate_is_openning(lb_uint16 idx);

/**
 * lb_view_close_isolate - close isolate view
 *
 * This function use to close isolate view and then show its parent view.
 *
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_view_close_isolate(void);

/**
 * lb_view_show_isolate - show isolate view
 * @idx: isolate view id.
 *
 * This function use to show isolate view through isolate view id.
 *
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_view_show_isolate(lb_uint16 idx);

/**
 * lb_view_get_obj_ext_by_id - get current view object ext
 * @id: object id value.
 * @ext: current view ext.
 *
 * This function use to get current view object ext through object id.
 *
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_view_get_obj_ext_by_id(lb_uint32 id, void **ext);

/**
 * lb_view_get_obj_property_by_ext - get current view object property
 * @pext: object ext.
 * @p_property: pointer to object property pointer.
 *
 * This function use to get current view object property through object ext.
 *
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_view_get_obj_property_by_ext(void *pext, void **p_property);

/**
 * lb_view_get_obj_property_by_id - get current view object property
 * @pext: object id.
 * @p_property: pointer to object property pointer.
 *
 * This function use to get current view object property through object id.
 *
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_view_get_obj_property_by_id(lb_uint32 id, void **p_property);

/**
 * lb_view_set_cur_view_id - set current view id
 * @view_id: view index.
 *
 * This function use to set current view id.
 *
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
void lb_view_set_cur_view_id(lb_uint32 view_id);

/**
 * lb_view_get_cur_view_id - get current view id
 * @view_id: view index.
 *
 * This function use to get current view id.
 *
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_uint32 lb_view_get_cur_view_id(void);

/**
 * lb_view_get_last_view_id - get last view id
 * @view_id: view index.
 *
 * This function use to get last view id.
 *
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_uint32 lb_view_get_last_view_id(void);

/**
* eimage_create_img_buf - malloc buf for image file
* @path: image file path
*
* return NULL : fail
*/
void *eimage_create_img_buf(const char *path);

/**
* eimage_destory_img_buf - free buf for image file
* @img_buf: pointer of image
*
* return -1 : fail
*/
int eimage_destory_img_buf(void *img_buf);

/**
* eimage_create_img_buf_thumb - malloc buf for thumb buffer
* @thumb_data_buf: thumb buffer
*
* return NULL : fail
*/
void *eimage_create_img_buf_thumb(void *thumb_data_buf);

/**
* eimage_destory_img_buf_thumb - free buf for thumb buffer
* @thumb_img_buf: pointer of image for thumb buffer
*
* return -1 : fail
*/
int eimage_destory_img_buf_thumb(void *thumb_img_buf);

/**
* Create a file with a constant data
* @param fn name of the file (directories are not supported)
* @param const_p pointer to a constant data
* @param len length of the data pointed by 'const_p' in bytes
* @return 0: no error, the file is read, otherwise, return the error code
*/
int efs_create(const char *path_iso);

/**
* destory a efs
* @return 0: no error, the file is read, otherwise, return the error code
*/
int efs_destory(void);

/**
* elang_get_utf8_string_josn - get utf8 string
* @json_str: string in json
*
* return -1 if fail, don't free the pointer
*/
const char *elang_get_utf8_string_josn(const char *json_str);

/**
* elang_get_string_id_josn - get utf8 string index
* @json_str: string in json
*
* return -1 if fail, don't free the pointer
*/
unsigned int elang_get_string_id_josn(const char *json_str);

#endif

