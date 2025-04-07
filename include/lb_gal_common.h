/*
 * lb_gal_common.h - header file for show all the objects.
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

#ifndef _LB_GAL_COMMON_H_
#define _LB_GAL_COMMON_H_

/*----------------------------------------------*
 * header files                           *
 *----------------------------------------------*/
#include "lb_types.h"
#include "lb_common.h"
/*----------------------------------------------*
 * defines                  *
 *----------------------------------------------*/

/*----------------------------------------------*
 * structs                  *
 *----------------------------------------------*/

typedef struct tag_size {
	lb_int32     w;
	lb_int32     h;
} lb_size_t;

typedef struct tag_range {
	lb_int32     min;
	lb_int32     max;
} lb_range_t;

typedef struct _tag_lb_disp_info_t_ {
	uint32_t width;
	uint32_t height;
	uint32_t rot;
} lb_disp_info_t;

typedef struct tag_rectangle_t {
	lb_int32	x1;
	lb_int32	y1;
	lb_int32	x2;
	lb_int32	y2;
} lb_rect_t;

/*Scrollbar modes: shows when should the scrollbars be visible*/
typedef enum {
	LB_SB_MODE_OFF,    /*Never show scrollbars*/
	LB_SB_MODE_ON,     /*Always show scrollbars*/
	LB_SB_MODE_DRAG,   /*Show scrollbars when page is being dragged*/
	LB_SB_MODE_AUTO,   /*Show scrollbars when the scrollable container
				is large enough to be scrolled*/
	LB_SB_MODE_HIDE,   /*Hide the scroll bar temporally*/
	LB_SB_MODE_UNHIDE  /*Unhide the previously hidden scrollbar.
				Recover it's type too*/
} lb_sb_mode_t;

/*Layout options*/
typedef enum tag_lb_common_layout {
	LB_LAYOUT_OFF = 0,
	LB_LAYOUT_CENTER,
	LB_LAYOUT_COL_L,	/*Column left align*/
	LB_LAYOUT_COL_M,	/*Column middle align*/
	LB_LAYOUT_COL_R,	/*Column right align*/
	LB_LAYOUT_ROW_T,	/*Row top align*/
	LB_LAYOUT_ROW_M,	/*Row middle align*/
	LB_LAYOUT_ROW_B,	/*Row bottom align*/
	LB_LAYOUT_PRETTY,	/*Put as many object as possible
				in row and begin a new row*/
	LB_LAYOUT_GRID,	    /*Align same-sized object into a grid*/
} lb_common_layout_e;

/* Arc */
typedef enum tag_arc_update_part {
	LB_ARC_UPD_POS,
	LB_ARC_UPD_SIZE,
	LB_ARC_UPD_ANGLES,
	LB_ARC_UPD_COL,
	LB_ARC_UPD_WIDTH
} lb_arc_upd_part_e;

typedef struct tag_arc_angles {
	lb_int16     start;
	lb_int16     end;
} lb_arc_angles_t;


/* Bar */
typedef enum tag_bar_update_part {
	LB_BAR_UPD_POS,
	LB_BAR_UPD_SIZE,
	LB_BAR_UPD_RANGE,
	LB_BAR_UPD_CUR_VAL,
	LB_BAR_UPD_COL,
} lb_bar_upd_part_e;

typedef struct tag_bar_cols {
	lb_uint32    bg_col;
	lb_uint32    main_col;
	lb_uint32    grad_col;
} lb_bar_cols_t;


/* Btn */
typedef enum tag_btn_update_part {
	LB_BTN_UPD_POS,
	LB_BTN_UPD_SIZE,
	LB_BTN_UPD_TXT,
	LB_BTN_UPD_STATE,
	LB_BTN_UPD_BTN_COLOR
} lb_btn_upd_part_e;

/* Btnm */
typedef enum tag_btnm_update_part {
	LB_BTNM_UPD_POS,
	LB_BTNM_UPD_SIZE
} lb_btnm_upd_part_e;

/* Calendar */
typedef enum tag_lb_ca_update_part {
	LB_CA_UPD_POS,
	LB_CA_UPD_SIZE,
	LB_CA_UPD_TODAY,
	LB_CA_UPD_SHOWED,
	LB_CA_UPD_HILIGHT,
} lb_ca_update_part_e;

typedef struct tag_lb_calendar_hilights {
	lb_uint16                num;
	lb_calendar_date_t      *highlights;
} lb_calendar_hilights_t;


/* Canvas */
typedef enum tag_canvas_update_part {
	LB_CANVAS_UPD_POS,
	LB_CANVAS_UPD_SIZE,
	LB_CANVAS_UPD_DRAW_BG,
	LB_CANVAS_UPD_DRAW_POLYGON,
	LB_CANVAS_UPD_DRAW_CIRCLE
} lb_canvas_upd_part_e;

/* Chart */
typedef enum tag_lb_chart_update_part {
	LB_CHART_UPD_POS,
	LB_CHART_UPD_SIZE,
	LB_CHART_UPD_TYPE,
	LB_CHART_UPD_SERIAL_WIDTH,
	LB_CHART_UPD_Y_RANGE,
	LB_CHART_UPD_DIVS,
	LB_CHART_UPD_SERIES
} lb_chart_update_part_e;

typedef struct tag_lb_chart_divs {
	lb_uint8         hdivs;
	lb_uint8         vdivs;
} lb_chart_divs_t;

enum {
	LB_CHART_TYPE_LINE = 0x01,
	LB_CHART_TYPE_COLUMN = 0x02,
	LB_CHART_TYPE_POINT = 0x04,
};
typedef lb_uint8 lb_chart_type_t;


/* Checkbox */
typedef enum tag_lb_cb_update_part {
	LB_CB_UPD_POS,
	LB_CB_UPD_SIZE,
	LB_CB_UPD_STATE,
	LB_CB_UPD_TXT
} lb_cb_update_part_e;

typedef enum tag_lb_cb_state {
	LB_CB_UNCHECKED,
	LB_CB_CHECKED,
	LB_CB_INACTIVE
} lb_cb_state_e;

/* Cont */
typedef enum tag_lb_cont_update_part {
	LB_CONT_UPD_POS,
	LB_CONT_UPD_SIZE,
	LB_CONT_UPD_LAYOUT,
	LB_CONT_UPD_FIT,
	LB_CONT_UPD_HIDE
} lb_cont_update_part_e;

typedef struct tag_lb_cont_fit {
	bool hor_fit;
	bool ver_fit;
} lb_cont_fit_t;

/* Ddlist */
typedef enum tag_lb_ddlist_update_part {
	LB_DDLIST_UPD_POS,
	LB_DDLIST_UPD_SIZE,
} lb_ddlist_update_part_e;

/* Flist */
typedef enum tag_lb_flist_update_part {
	LB_FLIST_UPDATE_ALL_ELEMS,
	LB_FLIST_DELETE_ONE_ELEMS,
	LB_FLIST_DELETE_ALL_ELEMS,
	LB_FLIST_UPDATE_DEF_ELEMS,
	LB_FLIST_UPDATE_FILE_NAME,
	LB_FLIST_UPDATE_FILE_TYPE,
	LB_FLIST_UPDATE_FILE_LOCK,
	LB_FLIST_UPDATE_FILE_BIN,
	LB_FLIST_UPDATE_ONE_THUMB,
	LB_FLIST_UPDATE_ALL_THUMB,
	LB_FLIST_UPDATE_BTN_SELECTED
} lb_flist_update_part_e;

/* Gauge */
typedef enum tag_lb_gauge_update_part {
	LB_GAUGE_UPD_POS,
	LB_GAUGE_UPD_SIZE,
	LB_GAUGE_UPD_NEEDLE_COUNT,
	LB_GAUGE_UPD_VALUE,
	LB_GAUGE_UPD_SCALE
} lb_gauge_update_part_e;

typedef struct tag_lb_gauge_needle_count {
	lb_uint8	needle_count;
	lb_int32	needle_colors;
} lb_gauge_nendle_count_t;

typedef struct tag_lb_gauge_value {
	lb_uint8	needle_id;
	lb_int16	value;
} lb_gauge_value_t;

typedef struct tag_lb_gauge_scale {
	lb_uint16	angle;
	lb_uint8	line_cnt;
	lb_uint8	label_cnt;
} lb_gauge_scale_t;

/* Img */
typedef enum tag_img_update_part {
	LB_IMG_UPD_POS,
	LB_IMG_UPD_SIZE,
	LB_IMG_UPD_TXT,
	LB_IMG_UPD_SRC,
	LB_IMG_UPD_PAR,
} lb_img_upd_part_e;

/* Imgbtn */
typedef enum tag_imgbtn_update_part {
	LB_IMGBTN_UPD_POS,
	LB_IMGBTN_UPD_SIZE,
	LB_IMGBTN_UPD_TXT,
	LB_IMGBTN_UPD_SRC,
	LB_IMGBTN_UPD_STATE
} lb_imgbtn_upd_part_e;

/* Kb */
typedef enum tag_lb_kb_update_part {
	LB_KB_SET_TA,
	LB_KB_GET_TA
} lb_kb_update_part_e;

/* Label */
typedef enum tag_label_update_part {
	LB_LABEL_UPD_POS,
	LB_LABEL_UPD_SIZE,
	LB_LABEL_UPD_TXT,
	LB_LABEL_UPD_STYLE,
	LB_LABEL_UPD_HIDE
} lb_label_upd_part_e;

/* Led */
typedef enum tag_lb_led_update_part {
	LB_LED_UPD_POS,
	LB_LED_UPD_SIZE,
	LB_LED_UPD_BRIGHT,
	LB_LED_UPD_ON,
	LB_LED_UPD_OFF
} lb_led_update_part_e;

/* Line */
typedef enum tag_lb_line_update_part {
	LB_LINE_UPD_POS,
	LB_LINE_UPD_SIZE,
	LB_LINE_UPD_POINTS,
	LB_LINE_UPD_AUTO_SIZE,
	LB_LINE_UPD_Y_INVERT,
	LB_LINE_UPD_COLOR
} lb_line_update_part_e;

typedef enum tag_lb_line_y_invert {
	LB_LINE_Y_INVERT_ENABLE,
	LB_LINE_Y_INVERT_DISABLE
} lb_line_y_invert_e;

typedef enum tag_lb_line_auto_size {
	LB_LINE_AUTO_SIZE_ENABLE,
	LB_LINE_AUTO_SIZE_DISABLE
} lb_line_auto_size_e;

typedef struct tag_lb_line_points {
	lb_point_t	points[16];
	lb_uint16	num;
} lb_line_points_t;

/* List */
typedef enum tag_lb_list_update_part {
	LB_LIST_UPD_POS,
	LB_LIST_UPD_SIZE,
	LB_LIST_UPD_ANIM_TIME,
	LB_LIST_UPD_SB_MODE,
	LB_LIST_UPD_BTN_SELECTED,
	LB_LIST_UPD_UP,
	LB_LIST_UPD_DOWN
} lb_list_update_part_e;

/* Lmeter */
typedef enum tag_lb_lmeter_update_part {
	LB_LMETER_UPD_POS,
	LB_LMETER_UPD_SIZE,
	LB_LMETER_UPD_VALUE,
	LB_LMETER_UPD_RANGE,
	LB_LMETER_UPD_SCALE
} lb_lmeter_update_part_e;

/*Data of line meter*/
typedef struct {
	lb_uint16 scale_angle;		/* Angle of the scale in deg. (0..360) */
	lb_uint8 line_cnt;		/* Count of lines */
	lb_int16 cur_value;
	lb_int16 min_value;
	lb_int16 max_value;
} lb_lmeter_ext_t;

/* Msgbox */
typedef enum tag_lb_msgbox_update_part {
	LB_MSGBOX_UPD_POS,
	LB_MSGBOX_UPD_SIZE,
	LB_MSGBOX_UPD_TEXT,
	LB_MSGBOX_UPD_BTNM_TOGGLE
} lb_msgbox_update_part_e;

/* Page */
typedef enum tag_lb_page_update_part {
	LB_PAGE_UPD_POS,
	LB_PAGE_UPD_SIZE,
	LB_PAGE_UPD_SB_MODE,
	LB_PAGE_UPD_SCRL_FIT,
	LB_PAGE_UPD_SCRL_W,
	LB_PAGE_UPD_SCRL_H,
	LB_PAGE_UPD_SCRL_LAYOUT
} lb_page_update_part_e;

typedef struct {
	bool hor_en;
	bool ver_en;
} lb_page_scrl_fit_t;

/* Preload */
typedef enum tag_lb_preload_update_part {
	LB_PRELOAD_UPD_POS,
	LB_PRELOAD_UPD_SIZE,
	LB_PRELOAD_UPD_ARC_LENGTH,
	LB_PRELOAD_UPD_SPIN_TIME
} lb_preload_update_part_e;

/* Roller */
typedef enum tag_lb_roller_update_part {
	LB_ROLLER_UPD_POS,
	LB_ROLLER_UPD_SIZE,
	LB_ROLLER_UPD_OPTIONS,
	LB_ROLLER_UPD_SELECTED,
	LB_ROLLER_UPD_VISIBLE_ROW_COUNT,
	LB_ROLLER_UPD_HOR_FIT,
	LB_ROLLER_UPD_ANIM_TIME,
	LB_ROLLER_UPD_STYLE,
} lb_roller_update_part_e;

typedef struct tag_lb_roller_selected {
	bool		anim_en;
	lb_uint16	sel_opt;
} lb_roller_selected_t;

typedef struct tag_lb_roller_msg_data {
	lb_uint32	sel_opt_id;
	lb_uint32	sel_opt_id_ori;
	void		*pobj;
} lb_roller_msg_data_t;


/* Slider */
typedef enum tag_lb_slider_update_part {
	LB_SLIDER_UPD_POS,
	LB_SLIDER_UPD_SIZE,
	LB_SLIDER_UPD_VALUE,
	LB_SLIDER_UPD_VALUE_ANIM,
	LB_SLIDER_UPD_RANG
} lb_slider_update_part_e;

typedef struct tag_lb_slider_value_anim {
	lb_int16	value;
	lb_uint16	anim_time;
} lb_slider_value_anim_t;

typedef struct tag_lb_slider_rang {
	lb_int16	min;
	lb_int16	max;
} lb_slider_rang_t;

typedef struct tag_slider_status_msg {
	void			*pobj;
	lb_uint8		press_status; /* 1:press;*/
	lb_uint8		release_status; /* 1:release;*/
	lb_int16		tmp_val;
} lb_slider_status_msg_t;

/* Sw */
typedef enum tag_lb_sw_update_part {
	LB_SW_UPD_POS,
	LB_SW_UPD_SIZE,
	LB_SW_UPD_STATE
} lb_sw_update_part_e;

/* Ta */
typedef enum tag_lb_ta_update_part {
	LB_TA_UPD_POS,
	LB_TA_UPD_SIZE,
	LB_TA_SET_TEXT,
	LB_TA_GET_TEXT
} lb_ta_update_part_e;

/* Tabview */
typedef enum tag_lb_tabview_update_part {
	LB_TABVIEW_UPD_POS,
	LB_TABVIEW_UPD_SIZE,
	LB_TABVIEW_UPD_SLIDING,
	LB_TABVIEW_UPD_ANIM_TIME
} lb_tabview_update_part_e;

/* Win */
typedef enum tag_lb_win_update_part {
	LB_WIN_UPD_POS,
	LB_WIN_UPD_SIZE,
	LB_WIN_UPD_TITLE,
	LB_WIN_UPD_LAYOUT,
	LB_WIN_UPD_SB_MODE,
} lb_win_update_part_e;

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
 * lb_gal_update_arc - update arc object
 * @pobj: the arc object pointer.
 * @part: arc update part enum, details refer to 'lb_arc_upd_part_e'.
 * @data: the arc part update of data pointer.
 *
 * This function use to send update message and data when arc object part need update.
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_gal_update_arc(void *pobj, lb_arc_upd_part_e part, void *data);

/**
 * lb_gal_update_bar - update bar object
 * @pobj: the bar object pointer.
 * @part: bar update part enum, details refer to 'lb_bar_upd_part_e'.
 * @data: the bar part update of data pointer.
 *
 * This function use to send update message and data when bar object part need update.
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_gal_update_bar(void *pobj, lb_bar_upd_part_e part, void *data);

/**
 * lb_gal_update_btn - update btn object
 * @pobj: the btn object pointer.
 * @part: btn update part enum, details refer to 'lb_btn_upd_part_e'.
 * @data: the btn part update of data pointer.
 *
 * This function use to send update message and data when btn object part need update.
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_gal_update_btn(void *pobj, lb_btn_upd_part_e part, void *data);

/**
 * lb_gal_update_calendar - update calendar object
 * @pobj: the calendar object pointer.
 * @part: calendar update part enum, details refer to 'lb_ca_update_part_e'.
 * @data: the calendar part update of data pointer.
 *
 * This function use to send update message and data
 * when calendar object part need update.
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_gal_update_calendar(void *pobj, lb_ca_update_part_e part, void *data);

/**
 * lb_gal_update_canvas - update canvas object
 * @pobj: the canvas object pointer.
 * @part: canvas update part enum, details refer to 'lb_canvas_upd_part_e'.
 * @count: draw count.
 * @data: the canvas part update of data pointer.
 *
 * This function use to send update message and data when canvas object part need update.
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_gal_update_canvas(void *pobj, lb_canvas_upd_part_e part, lb_uint8 count,
					void *data);

/**
 * lb_canvas_set_background - set canvas background property
 * @id: object index, it is unique.
 * @color: color value.
 *
 * This function use to set canvas background property according to object index.
 *
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_canvas_set_background(lb_uint32 id, lb_uint32 color,
				lb_uint8 color_format);

/**
 * lb_gal_update_chart - update chart object
 * @pproperty: the chart object of property pointer, details refer to 'lb_chart_t'.
 * @pobj: the chart object pointer.
 * @part: chart update part enum, details refer to 'lb_chart_update_part_e'.
 * @series_index: the index of series.
 * @data: the chart part update of data pointer.
 *
 * This function use to send update message and data when chart object part need update.
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_gal_update_chart(lb_chart_t *pproperty, void *pobj,
	lb_chart_update_part_e part, lb_uint8 series_index, void *data);


/**
 * lb_gal_update_checkbox - update checkbox object
 * @pobj: the checkbox object pointer.
 * @part: checkbox update part enum, details refer to 'lb_cb_update_part_e'.
 * @data: the checkbox part update of data pointer.
 *
 * This function use to send update message and data
 * when checkbox object part need update.
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_gal_update_checkbox(void *pobj, lb_cb_update_part_e part, void *data);

/**
 * lb_gal_update_cont - update cont object
 * @pobj: the cont object pointer.
 * @part: cont update part enum, details refer to 'lb_cont_upd_part_e'.
 * @data: the cont part update of data pointer.
 *
 * This function use to send update message and data when cont object part need update.
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_gal_update_cont(void *pobj, lb_cont_update_part_e part, void *data);

/**
 * lb_gal_update_ddlist - update ddlist object
 * @pobj: the ddlist object pointer.
 * @part: ddlist update part enum, details refer to 'lb_ddlist_update_part_e'.
 * @data: the ddlist part update of data pointer.
 *
 * This function use to send update message and data when ddlist object part need update.
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_gal_update_ddlist(void *pobj, lb_ddlist_update_part_e part, void *data);

/**
 * lb_gal_update_flist - update flist object
 * @pproperty: the flist property pointer.
 * @part: flist update part enum, details refer to 'lb_flistt_update_part_e'.
 * @data: the flist part update of data pointer.
 *
 * This function use to send update message and data when flist object part need update.
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_gal_update_flist(lb_flist_t *pproperty,
	lb_flist_update_part_e part, void *data);

/**
 * lb_gal_update_gauge - update gauge object
 * @pobj: the gauge object pointer.
 * @part: gauge update part enum, details refer to 'lb_gauge_update_part_e'.
 * @data: the gauge part update of data pointer.
 *
 * This function use to send update message and data when gauge object part need update.
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_gal_update_gauge(void *pobj, lb_gauge_update_part_e part, void *data);

/**
 * lb_gal_update_img - update image object
 * @pobj: the image object pointer.
 * @part: image update part enum, details refer to 'lb_img_upd_part_e'.
 * @index: when part is LB_IMG_UPD_SRC, if index is 0xFF, hidden object;
 * otherwise, show object.
 * @data: the gauge part update of data pointer.
 *
 * This function use to send update message and data when image object part need update.
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_gal_update_img(lb_img_t *pproperty, void *pobj, lb_img_upd_part_e part,
				lb_uint8 index, void *data);

/**
 * lb_gal_update_imgbtn - update imgbtn object
 * @pobj: the imgbtn object pointer.
 * @part: imgbtn update part enum, details refer to 'lb_imgbtn_upd_part_e'.
 * @u_val: when part is LB_IMGBTN_UPD_SRC, if index is 0xFF, hidden object;
 * otherwise, show object.
 * @data: the imgbtn part update of data pointer.
 *
 * This function use to send update message and data when imgbtn object part need update.
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_gal_update_imgbtn(lb_imgbtn_t *pproperty, void *pobj,
					lb_imgbtn_upd_part_e part,
					lb_uint8 u_val, void *data);

/**
 * lb_gal_update_kb - update keyboard object
 * @pobj: the keyboard object pointer.
 * @part: keyboard update part enum, details refer to 'lb_kb_update_part_e'.
 * @data: the keyboard part update of data pointer.
 *
 * This function use to send update message and data
 * when keyboard object part need update.
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_gal_update_kb(void *p_kb, lb_kb_update_part_e part, void **data);

/**
 * lb_gal_update_label - update label object
 * @pobj: the label object pointer.
 * @part: label update part enum, details refer to 'lb_label_upd_part_e'.
 * @data: the label part update of data pointer.
 *
 * This function use to send update message and data when label object part need update.
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_gal_update_label(void *pobj, lb_label_upd_part_e part, void *data);

/**
 * lb_gal_label_get_style - get label style
 * @pobj: the label object pointer.
 * @p_lb_al_style: the label style of data pointer.
 *
 * This function use to get the style of an label object.
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_gal_label_get_style(void *pobj, lb_al_style_t *p_lb_al_style);

/**
 * lb_gal_update_led - update led object
 * @pobj: the led object pointer.
 * @part: led update part enum, details refer to 'lb_led_update_part_e'.
 * @data: the led part update of data pointer.
 *
 * This function use to send update message and data when led object part need update.
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_gal_update_led(void *pobj, lb_led_update_part_e part, void *data);

/**
 * lb_gal_update_line - update line object
 * @pobj: the line object pointer.
 * @part: line update part enum, details refer to 'lb_line_update_part_e'.
 * @data: the line part update of data pointer.
 *
 * This function use to send update message and data when line object part need update.
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_gal_update_line(void *pobj, lb_line_update_part_e part, void *data);

/**
 * lb_gal_update_list - update list object
 * @pproperty: the list object of property pointer, details refer to 'lb_list_t'.
 * @pobj: the list object pointer.
 * @part: list update part enum, details refer to 'lb_list_update_part_e'.
 * @data: the list part update of data pointer.
 *
 * This function use to send update message and data when list object part need update.
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_gal_update_list(lb_list_t *pproperty, void *pobj,
				lb_list_update_part_e part, void *data);

/**
 * lb_gal_list_get_btn_free_num - get the free number of the list of btn
 * @pobj: the list object pointer.
 * @index: free number.
 *
 * This function use to get the free number.
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_gal_list_get_btn_free_num(void *pobj, lb_uint32 *index);

/**
 * lb_gal_update_lmeter - update lmeter object
 * @pobj: the lmeter object pointer.
 * @part: lmeter update part enum, details refer to 'lb_lmeter_update_part_e'.
 * @data: the lmeter part update of data pointer.
 *
 * This function use to send update message and data when lmeter object part need update.
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_gal_update_lmeter(void *pobj, lb_lmeter_update_part_e part, void *data);

/**
 * lb_gal_update_msgbox - update msgbox object
 * @pobj: the msgbox object pointer.
 * @part: msgbox update part enum, details refer to 'lb_msgbox_update_part_e'.
 * @data: the msgbox part update of data pointer.
 *
 * This function use to send update message and data when msgbox object part need update.
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_gal_update_msgbox(void *pobj, lb_msgbox_update_part_e part, void *data);

/**
 * lb_gal_update_page - update page object
 * @pobj: the page object pointer.
 * @part: page update part enum, details refer to 'lb_page_update_part_e'.
 * @data: the page part update of data pointer.
 *
 * This function use to send update message and data when page object part need update.
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_gal_update_page(void *pobj, lb_page_update_part_e part, void *data);

/**
 * lb_gal_update_preload - update preload object
 * @pobj: the preload object pointer.
 * @part: preload update part enum, details refer to 'lb_preload_update_part_e'.
 * @data: the preload part update of data pointer.
 *
 * This function use to send update message and data when preload object part need update.
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_gal_update_preload(void *pobj, lb_preload_update_part_e part, void *data);

/**
 * lb_gal_update_roller - update roller object
 * @pobj: the roller object pointer.
 * @part: roller update part enum, details refer to 'lb_roller_update_part_e'.
 * @data: the roller part update of data pointer.
 *
 * This function use to send update message and data when roller object part need update.
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_gal_update_roller(void *pobj, lb_roller_update_part_e part,
			lb_uint32 u_val, void *data);

/**
 * lb_gal_roller_set_options - set the options on a roller
 * @pproperty: the roller object of property pointer, details refer to 'lb_roller_t'.
 * @options: string id pointer.
 * @num: number of string id.
 *
 * This function use to set specified roller object string value
 * according to object index.
 *
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_gal_roller_set_options(lb_roller_t *pproperty,
				lb_uint32 *options, lb_uint16 num);

/**
 * lb_gal_update_slider - update slider object
 * @pobj: the slider object pointer.
 * @part: slider update part enum, details refer to 'lb_slider_update_part_e'.
 * @data: the slider part update of data pointer.
 *
 * This function use to send update message and data when slider object part need update.
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_gal_update_slider(void *pobj, lb_slider_update_part_e part, void *data);

/**
 * lb_gal_get_slider_value - get slider value
 * @pobj: the slider object pointer.
 * @pval: slider value pointer.
 *
 * This function use to get slider value.
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_gal_get_slider_value(void *pobj, lb_int16 *pval);

/**
 * lb_gal_update_sw - update switch object
 * @pobj: the switch object pointer.
 * @part: switch update part enum, details refer to 'lb_sw_update_part_e'.
 * @data: the switch part update of data pointer.
 *
 * This function use to send update message and data when switch object part need update.
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_gal_update_sw(void *pobj, lb_sw_update_part_e part, void *data);

/**
 * lb_gal_update_ta - update textarea object
 * @pobj: the textarea object pointer.
 * @part: textarea update part enum, details refer to 'lb_ta_update_part_e'.
 * @data: the textarea part update of data pointer.
 *
 * This function use to send update message and data
 * when textarea object part need update.
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_gal_update_ta(void *pobj, lb_ta_update_part_e part, const char *data);

/**
 * lb_gal_update_tabview - update tabview object
 * @pobj: the tabview object pointer.
 * @part: tabview update part enum, details refer to 'lb_tabview_update_part_e'.
 * @data: the tabview part update of data pointer.
 *
 * This function use to send update message and data when tabview object part need update.
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_gal_update_tabview(void *pobj, lb_tabview_update_part_e part, void *data);

/**
 * lb_gal_update_win - update window object
 * @pobj: the window object pointer.
 * @part: window update part enum, details refer to 'lb_win_update_part_e'.
 * @data: the window part update of data pointer.
 *
 * This function use to send update message and data when window object part need update.
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_gal_update_win(void *pobj, lb_win_update_part_e part, void *data);

/**
 * lb_gal_set_obj_click - set object click
 * @pobj: the object pointer.
 * @en: make the object clickable.
 *
 * This function use to enable or disable the clicking of an object.
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_gal_set_obj_click(void *pobj, bool en);

/**
 * lb_gal_get_obj_hidden - get object hidden
 * @pobj: the object pointer.
 *
 * This function use to get object hidden value.
 * Returns 1: object is hidden; 0: object is show; -1: unknow error.
 */
lb_int32 lb_gal_get_obj_hidden(void *pobj);

/**
 * lb_gal_set_obj_hidden - set object hidden
 * @pobj: the object pointer.
 * @en: boolen value, true: hidden object; false: show object.
 *
 * This function use to set object show or hidden.
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_gal_set_obj_hidden(void *pobj, bool en);

/**
 * lb_gal_set_multi_obj_hidden - set object hidden
 * @pobj: the object pointer.
 * @en: boolen value, true: hidden object; false: show object.
 * @refre_flag: 0,frist object;1,medium object;2,last object.
 *
 * This function use to hide an object, it won't be visible and clickable.
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_gal_set_multi_obj_hidden(void *pobj, bool en, lb_uint8 refre_flag);

/**
 * lb_gal_set_language - set language
 * @lb_lang_index: language index value.
 *
 * This function use to set language for all objects.
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_gal_set_language(lb_uint32 lb_lang_index);

/**
 * lb_gal_refre_language - refresh language
 * @view: the view pointer.
 *
 * This function use to refresh view language.
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_gal_refre_language(lb_view_t *view);

/**
 * lb_gal_set_screen_standby_time - set screen_standby_time
 * @time_sec: time.
 *
 * This function use to set screen_standby_time.
 * Returns -0.
 */
lb_int32 lb_gal_set_screen_standby_time(lb_uint32 time_sec);

/**
 * lb_gal_set_screen_standby_enable - set screen_standby enable
 * @enable: true,enable;false,disable.
 *
 * This function use to set screen_standby enable.
 * Returns -0.
 */
void lb_gal_set_screen_standby_enable(bool enable);

/**
 * lb_gal_screen_standby_switch - screen_standby switch
 *
 * This function use to screen_standby switch.
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_gal_screen_standby_switch(void);

/**
 * lb_gal_get_screen_standby_status - get screen_standby status
 *
 * This function use to get screen_standby status.
 * Returns -0.
 */
lb_uint8 lb_gal_get_screen_standby_status(void);

/**
 * lb_get_font_lang_idx - get language idx
 *
 * This function use to get current language index.
 * Returns language index.
 */
lb_int8 lb_get_font_lang_idx(void);

/**
 * lb_set_font_lang_idx - set language idx
 * @lang_idx: luanguage index.
 *
 * This function use to set language idx.
 * Returns no value.
 */
void lb_set_font_lang_idx(lb_uint8 lang_idx);

/**
 * lb_get_disp_info - get disp info
 * @lb_disp_info: struct disp_info.
 *
 * This function use to get screen width height and rotate status from disp.
 * Returns no value.
 */
void lb_get_disp_info(lb_disp_info_t *lb_disp_info);

/**
 * lb_play_tone - play tone
 *
 * This function use to play tone when press button.
 * Returns no value.
 */
void lb_play_tone(void);

/**
 * lb_play_warningtone - play tone
 * @filepath: tone file path.
 *
 * This function use to play warning tone by tone file.
 * Returns -SUCCESS if there is no error; otherwise, return the error code.
 */
lb_int32 lb_play_warningtone(char *filepath);

/**
 * lb_set_tone_flag - set tone flag
 * @tone_flag: play tone flag, if 1 box can play tone.
 *
 * This function use play tone flag.
 * Returns no value.
 */
void lb_set_tone_flag(lb_uint8 tone_flag);

lb_int32 lb_gal_bl_draw_init(void);
lb_int32 lb_gal_bl_draw_show(void);
lb_int32 lb_gal_bl_draw_hide(void);
lb_int32 lb_gal_bl_draw_request(lb_int32 idx);
lb_int32 lb_gal_bl_draw_release(lb_int32 idx);
lb_int32 lb_gal_bl_draw_add(lb_int32 idx);
lb_int32 lb_gal_bl_draw_remove(lb_int32 idx);
lb_int32 lb_gal_bl_draw_buff(lb_int32 idx, lb_rect_t rect, lb_uint8 *buff);

#endif

