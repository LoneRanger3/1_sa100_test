/*
 * fileexp_common.c - file sub_system common code for LomboTech
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

#include "file_explorer.h"
#include "fileexp_common.h"
#include "lb_ui.h"
#include "lb_gal_common.h"

#include "system/system.h"
#include "thumb_image.h"
#include "thumb_video.h"
#include "view_stack.h"
#include "view_node.h"
#include "mars.h"
#include "video.h"
#include "image.h"

static lb_int32 file_realloc_res(void *param);
static lb_int32 file_alloc_name(void *param);
static lb_int32 file_free_name(void *param);
static lb_int32 file_get_image(void *param);
static lb_int32 file_cd_list(void *param);
static lb_int32 file_set_mode(void *param);

explore_t explore;
static void *tab_ext;
static void *video_prop;
static void *image_prop;

lb_int32 file_get_tab(void **ext)
{

	if (tab_ext == NULL) {
		lb_view_get_obj_ext_by_id(103, ext);
		if (*ext) {
			tab_ext = *ext;
			explore.tab_ext = *ext;
		}
	} else {
		*ext = tab_ext;
		explore.tab_ext = tab_ext;
	}

	return SUCCESS;
}

lb_int32 file_show_tab(void)
{
	void *ext = NULL;

	file_get_tab(&ext);
	if (NULL == ext) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	lb_gal_set_obj_hidden(ext, 0);

	return SUCCESS;
}

lb_int32 file_hide_tab(void)
{
	void *ext = NULL;

	file_get_tab(&ext);
	if (NULL == ext) {
		APP_LOG_W("failed\n");
		return FAIL;
	}

	lb_gal_set_obj_hidden(ext, 1);

	return SUCCESS;
}

lb_int32 file_get_list(lb_uint16 tab_cur, void **pproperty)
{
	switch (tab_cur) {
	case 0: {
		if (video_prop == NULL) {
			lb_view_get_obj_property_by_id(105, pproperty);
			if (*pproperty) {
				video_prop = *pproperty;
				explore.video_list = *pproperty;
			}
		} else {
			*pproperty = video_prop;
			explore.video_list = video_prop;
		}
	}
	break;
	case 1: {
		if (image_prop == NULL) {
			lb_view_get_obj_property_by_id(106, pproperty);
			if (*pproperty) {
				image_prop = *pproperty;
				explore.image_list = *pproperty;
			}
		} else {
			*pproperty = image_prop;
			explore.image_list = image_prop;
		}
	}
	break;

	default:
		break;

	}

	return SUCCESS;
}

lb_int32 file_exp_init(void *param)
{
	lb_obj_t *lb_obj;
	lb_tabview_t *pproperty;

	lb_obj = (lb_obj_t *)param;
	if (NULL == lb_obj) {
		APP_LOG_W("failed\n");
		return LB_ERROR_NO_MEM;
	}

	pproperty = lb_obj->property;

	explore.tab_cur = pproperty->tab_cur;

	return SUCCESS;
}

lb_int32 file_exp_exit(void *param)
{
	lb_obj_t *lb_obj;
	lb_tabview_t *pproperty;

	lb_obj = (lb_obj_t *)param;
	if (NULL == lb_obj) {
		APP_LOG_W("failed\n");
		return LB_ERROR_NO_MEM;
	}

	pproperty = lb_obj->property;

	explore.tab_cur = pproperty->tab_cur;
	video_prop = NULL;
	image_prop = NULL;

	return SUCCESS;
}

lb_int32 file_exp_change(void *param)
{
	lb_tabview_t	*pproperty = NULL;

	pproperty = param;
	if (NULL == pproperty) {
		APP_LOG_W("failed\n");
		return LB_ERROR_NO_MEM;
	}

	/* Get the current active sub-interfave */
	explore.tab_cur = pproperty->tab_cur;

	return SUCCESS;
}

lb_int32 file_exp_return(void *param)
{
	lb_tabview_t	*pproperty = NULL;

	pproperty = param;
	if (NULL == pproperty) {
		APP_LOG_W("failed\n");
		return LB_ERROR_NO_MEM;
	}

	file_list_return();

	return SUCCESS;
}

lb_int32 file_exp_enter(void *param)
{
	lb_tabview_t	*pproperty = NULL;

	pproperty = param;
	if (NULL == pproperty) {
		APP_LOG_W("failed\n");
		return LB_ERROR_NO_MEM;
	}

	file_list_enter();

	return SUCCESS;
}

lb_int32 video_list_init(void *param)
{
	lb_flist_t *pproperty;

	/* Find the list by id because params are buttons not the list we need */
	file_get_list(0, (void **)&pproperty);
	if (NULL == pproperty) {
		APP_LOG_W("failed\n");
		return LB_ERROR_NO_MEM;
	}

	if (NULL != pproperty->file_pri.cur_desert) {
		APP_LOG_W("failed\n");
		return SUCCESS;
	}

	/* This part can be done in json later */
	pproperty->file_pri.cur_type = ALL_MODE;
	pproperty->file_pri.cur_path = mars_mem_alloc(strlen(MARS_FD_PATH) + 1);
	strcpy(pproperty->file_pri.cur_path, MARS_FD_PATH);

	/* Retrieve the file list name and resource */
	file_realloc_res(pproperty);

	return SUCCESS;
}

lb_int32 video_list_exit(void *param)
{
	lb_flist_t *pproperty;

	/* Find the list by id because params are buttons not the list we need */
	file_get_list(0, (void **)&pproperty);
	if (NULL == pproperty) {
		APP_LOG_W("failed\n");
		return LB_ERROR_NO_MEM;
	}

	if (pproperty->file_pri.cur_path) {
		mars_mem_free(pproperty->file_pri.cur_path);
		pproperty->file_pri.cur_path = NULL;
	}

	if (pproperty->file_pri.cur_desert) {
		mars_free_list(pproperty->file_pri.cur_desert);
		pproperty->file_pri.cur_desert = NULL;
	}

	file_free_name(pproperty);

	return SUCCESS;
}

lb_int32 file_video_init(void *param)
{
	lb_int32 ret = 0;

	ret = video_list_init(param);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		return ret;
	}

	return SUCCESS;
}

lb_int32 file_video_exit(void *param)
{
	return SUCCESS;
}

lb_int32 image_list_init(void *param)
{
	lb_flist_t *pproperty;

	/* Find the list by id because params are buttons not the list we need */
	file_get_list(1, (void **)&pproperty);
	if (NULL == pproperty) {
		APP_LOG_W("failed\n");
		return LB_ERROR_NO_MEM;
	}

	if (NULL != pproperty->file_pri.cur_desert) {
		APP_LOG_W("failed\n");
		return SUCCESS;
	}

	/* This part can be done in json later */
	pproperty->file_pri.cur_type = PIC_MODE;
	pproperty->file_pri.cur_path = mars_mem_alloc(strlen(MARS_FD_PATH) + 1);
	strcpy(pproperty->file_pri.cur_path, MARS_FD_PATH);

	/* Retrieve the file list name and resource */
	file_realloc_res(pproperty);

	return SUCCESS;
}

lb_int32 image_list_exit(void *param)
{
	lb_flist_t *pproperty;

	/* Find the list by id because params are buttons not the list we need */
	file_get_list(1, (void **)&pproperty);
	if (NULL == pproperty) {
		APP_LOG_W("failed\n");
		return LB_ERROR_NO_MEM;
	}

	if (pproperty->file_pri.cur_path) {
		mars_mem_free(pproperty->file_pri.cur_path);
		pproperty->file_pri.cur_path = NULL;
	}

	if (pproperty->file_pri.cur_desert) {
		mars_free_list(pproperty->file_pri.cur_desert);
		pproperty->file_pri.cur_desert = NULL;
	}

	file_free_name(pproperty);

	return SUCCESS;
}

lb_int32 file_image_init(void *param)
{
	lb_int32 ret = 0;

	ret = image_list_init(param);
	if (ret != 0) {
		APP_LOG_W("failed\n");
		return ret;
	}

	return SUCCESS;
}

lb_int32 file_image_exit(void *param)
{
	return SUCCESS;
}

static lb_int32 file_realloc_res(void *param)
{
	lb_int32	ret = -1;
	lb_flist_t	*pproperty;

	pproperty = param;
	if (NULL == pproperty) {
		APP_LOG_W("failed\n");
		return LB_ERROR_NO_MEM;
	}

	file_free_name(param);

	ret = file_cd_list(param);
	if (0 != ret) {
		APP_LOG_W("failed\n");
		return LB_ERROR_NO_MEM;
	}

	ret = file_alloc_name(param);
	if (0 != ret) {
		APP_LOG_W("failed\n");
		return LB_ERROR_NO_MEM;
	}

	ret = file_get_image(param);
	if (0 != ret) {
		APP_LOG_W("failed\n");
		return LB_ERROR_NO_MEM;
	}

	return SUCCESS;
}

static lb_int32 file_alloc_name(void *param)
{
	lb_flist_t *pproperty;
	lb_int32	i = 0;

	pproperty = param;
	if (NULL == pproperty) {
		APP_LOG_W("failed\n");
		return LB_ERROR_NO_MEM;
	}

	if (!pproperty->file_pri.cur_desert) {
		APP_LOG_W("failed\n");
		return SUCCESS;
	}

	pproperty->file_num =
		mars_get_node_num(pproperty->file_pri.cur_desert);
	if (0 >= pproperty->file_num) {
		APP_LOG_W("there are nothing in dir..\n");
		return SUCCESS;
	}

	pproperty->file_array = (lb_file_array_t *)mars_mem_alloc
		(sizeof(lb_file_array_t) * pproperty->file_num);
	if (NULL == pproperty->file_array) {
		APP_LOG_W("failed\n");
		return LB_ERROR_NO_MEM;
	}

	for (i = 0; i < pproperty->file_num; i++)
		mars_get_node_name(pproperty->file_pri.cur_desert, i,
			&pproperty->file_array[i].file_name);

	return SUCCESS;
}

static lb_int32 file_free_name(void *param)
{
	lb_flist_t	*pproperty;

	pproperty = param;
	if (NULL == pproperty) {
		APP_LOG_W("failed\n");
		return LB_ERROR_NO_MEM;
	}

	if (pproperty->file_array) {
		mars_mem_free(pproperty->file_array);
		pproperty->file_array = NULL;
	}
	pproperty->file_num = 0;

	return SUCCESS;
}

static lb_int32 file_get_image(void *param)
{
	lb_flist_t *pproperty = NULL;
	void *desert = NULL;
	node_type_t node_type;
	media_type_t media_type;
	lock_type_t lock_type;
	lb_int32 i;

	pproperty = param;
	if (NULL == pproperty) {
		APP_LOG_W("failed\n");
		return LB_ERROR_NO_MEM;
	}

	desert = pproperty->file_pri.cur_desert;
	if (NULL == desert) {
		APP_LOG_W("failed\n");
		return LB_ERROR_NO_MEM;
	}

	for (i = 0; i < pproperty->file_num; i++) {
		pproperty->file_array[i].file_index = i;

		pproperty->file_array[i].file_type = MAX_TYPE;
		pproperty->file_array[i].type_used = 0;

		pproperty->file_array[i].file_lock  = LOCK_MAX;
		pproperty->file_array[i].lock_used = 0;

		pproperty->file_array[i].file_bin = BIN_MAX;
		pproperty->file_array[i].bin_used = 0;

		node_type = mars_get_node_type(desert, i);
		lock_type = mars_get_node_lock(desert, i);
		media_type = mars_get_node_media(desert, i);

		if (node_type == NODE_FILE) {
			if (media_type == MEDIA_PIC) {
				pproperty->file_array[i].file_type = PIC_TYPE;
				pproperty->file_array[i].type_used = 1;

				#ifndef FILE_BIN_OMIT
				pproperty->file_array[i].file_bin = BIN_FILE;
				pproperty->file_array[i].bin_used = 1;
				#endif

			} else if (media_type == MEDIA_VIDEO) {
				pproperty->file_array[i].file_type = VIDEO_TYPE;
				pproperty->file_array[i].type_used = 1;

				if (lock_type == LOCKED) {
					#ifndef FILE_LOCK_OMIT
					pproperty->file_array[i].file_lock  =
						LOCKED_FILE;
					pproperty->file_array[i].lock_used = 1;
					#endif

					#ifndef FILE_BIN_OMIT
					pproperty->file_array[i].file_bin =
						BIN_MAX;
					pproperty->file_array[i].bin_used = 0;
					#endif
				} else if (lock_type == UNLOCK) {
					#ifndef FILE_LOCK_OMIT
					pproperty->file_array[i].file_lock  =
						UNLOCK_FILE;
					pproperty->file_array[i].lock_used = 1;
					#endif

					#ifndef FILE_BIN_OMIT
					pproperty->file_array[i].file_bin =
						BIN_FILE;
					pproperty->file_array[i].bin_used = 1;
					#endif
				} else {
					#ifndef FILE_LOCK_OMIT
					pproperty->file_array[i].file_lock  =
						LOCK_MAX;
					pproperty->file_array[i].lock_used = 0;
					#endif

					#ifndef FILE_BIN_OMIT
					pproperty->file_array[i].file_bin =
						BIN_MAX;
					pproperty->file_array[i].bin_used = 0;
					#endif
				}
			} else {
				pproperty->file_array[i].file_type = FILE_TYPE;
				pproperty->file_array[i].type_used = 1;
			}
		} else {
			pproperty->file_array[i].file_type = DIR_TYPE;
			pproperty->file_array[i].type_used = 1;
		}
	}

	return SUCCESS;
}

static lb_int32 file_cd_list(void *param)
{
	lb_int32 ret = 0;
	lb_flist_t *pproperty = NULL;
	void *desert = NULL;

	pproperty = param;
	if (NULL == pproperty) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	desert = pproperty->file_pri.cur_desert;
	if (desert) {
		/* Back the list from file management module */
		mars_free_list(desert);
		desert = NULL;
	}

	/* Inform the mode of list we need to file management module */
	file_set_mode(param);

	/* Obtain the list from file management module */
	if (0 != mars_alloc_list(&desert)) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}
	pproperty->file_pri.cur_desert = desert;

	if (pproperty->file_pri.cur_type == PIC_MODE) {
		image_thumb_set(pproperty->file_pri.cur_desert);
		image_thumb_init(param);
	} else if (pproperty->file_pri.cur_type == VIDEO_MODE) {
		video_thumb_set(pproperty->file_pri.cur_desert);
		video_thumb_init(param);
	}

exit:
	return ret;
}

static lb_int32 file_set_mode(void *param)
{
	lb_flist_t	*pproperty;

	pproperty = param;
	if (NULL == pproperty) {
		APP_LOG_W("failed\n");
		return LB_ERROR_NO_MEM;
	}

	/* Inform the path of list that needs to mars */
	mars_set_path(pproperty->file_pri.cur_path);

	if (pproperty->file_pri.cur_type == ALL_MODE) {
		/* Inform the sort order of files to mars */
		mars_set_sort(SORT_ASCENDING);

		/* Inform the  type of media maybe ALL/JPEG/MP4 */
		mars_set_media(MEDIA_ALL);

		/* Inform the type of node maybe DIR/FILE */
		mars_set_node(NODE_ALL);

		/* Inform the type of query maybe recursive and non */
		mars_set_query(NON_RECUR);

		if (strstr(pproperty->file_pri.cur_path, "LOCK"))
			mars_set_lock(LOCKED);
		else
			mars_set_lock(UNLOCK);
	} else if (pproperty->file_pri.cur_type == VIDEO_MODE) {
		/* Inform the sort order of files to mars */
		mars_set_sort(SORT_DESCENDING);

		/* As above mentioned */
		mars_set_media(MEDIA_ALL);

		/* As above mentioned */
		mars_set_node(NODE_ALL);

		/* As above mentioned */
		mars_set_query(NON_RECUR);

		/* Inform the type of lock maybe locked and unlock */
		mars_set_lock(ALTYPE);
	} else if (pproperty->file_pri.cur_type == PIC_MODE) {
		/* Inform the sort order of files to mars */
		mars_set_sort(SORT_DESCENDING);

		/* As above mentioned */
		mars_set_media(MEDIA_PIC);

		/* As above mentioned */
		mars_set_node(NODE_FILE);

		/* As above mentioned */
		mars_set_query(RECURSIVE);

		/* Inform the type of lock maybe locked and unlock */
		mars_set_lock(ALTYPE);
	}

	return SUCCESS;
}

lb_int32 file_list_enter(void)
{
	lb_int32 ret = -1;
	lb_flist_t *pproperty = NULL;
	node_type_t nd_type;
	media_type_t md_type;
	char *path;

	/* Find the list by id because params are buttons not the list we need */
	file_get_list(explore.tab_cur, (void **)&pproperty);
	if (NULL == pproperty) {
		APP_LOG_W("failed\n");
		return LB_ERROR_NO_MEM;
	}

	mars_get_node_path(pproperty->file_pri.cur_desert,
		pproperty->file_pri.row_index, &path);
	nd_type = mars_get_node_type(pproperty->file_pri.cur_desert,
			pproperty->file_pri.row_index);
	md_type = mars_get_node_media(pproperty->file_pri.cur_desert,
			pproperty->file_pri.row_index);


	if (nd_type == NODE_DIR) {
		pproperty->file_pri.cur_path =
			mars_mem_realloc(pproperty->file_pri.cur_path,
				strlen(path) + 1);
		strcpy(pproperty->file_pri.cur_path, path);

		if (pproperty->file_pri.cur_type == ALL_MODE)
			pproperty->file_pri.cur_type = VIDEO_MODE;
		else if (pproperty->file_pri.cur_type == PIC_MODE)
			pproperty->file_pri.cur_type = PIC_MODE;

		/* Retrieve the file list name and resource */
		ret = file_realloc_res(pproperty);
		if (0 != ret) {
			APP_LOG_W("failed\n");
			return LB_ERROR_NO_MEM;
		}

		/* Display the file list name and resource */
		lb_gal_update_flist(pproperty,
			LB_FLIST_UPDATE_ALL_ELEMS, NULL);
	} else if (nd_type == NODE_FILE) {
		void *obj = NULL;

		file_hide_tab();

		if (md_type == MEDIA_VIDEO) {
			lb_view_get_obj_ext_by_id(105, &obj);
			video_set(pproperty->file_pri.cur_desert,
				pproperty->file_pri.row_index);
		} else if (md_type == MEDIA_PIC) {
			lb_view_get_obj_ext_by_id(106, &obj);
			image_set(pproperty->file_pri.cur_desert,
				pproperty->file_pri.row_index);
		}

		if (obj)
			lb_ui_send_msg(LB_MSG_ENTER_LINKAGE, &obj, sizeof(obj), 0);

	}

	return SUCCESS;
}

lb_int32 file_list_return(void)
{
	lb_int32 ret = -1;
	lb_flist_t *pproperty = NULL;
	char *path;

	/* Find the list by id because params are buttons not the list we need */
	file_get_list(explore.tab_cur, (void **)&pproperty);
	if (NULL == pproperty) {
		APP_LOG_W("failed\n");
		return LB_ERROR_NO_MEM;
	}

	/* Get the desired path from the file management module */
	mars_get_parent_path(pproperty->file_pri.cur_desert, &path);
	if (NULL == path)
		return LB_ERROR_NO_MEM;

	pproperty->file_pri.cur_path = mars_mem_realloc(pproperty->file_pri.cur_path,
			strlen(path) + 1);
	strcpy(pproperty->file_pri.cur_path, path);

	if (pproperty->file_pri.cur_type == PIC_MODE)
		image_thumb_exit((void *)0);
	else if (pproperty->file_pri.cur_type == VIDEO_MODE)
		video_thumb_exit((void *)0);

	if (pproperty->file_pri.cur_type == VIDEO_MODE)
		pproperty->file_pri.cur_type = ALL_MODE;
	else if (pproperty->file_pri.cur_type == PIC_MODE)
		pproperty->file_pri.cur_type = PIC_MODE;

	/* Retrieve the file list name and resource */
	ret = file_realloc_res(pproperty);
	if (0 != ret) {
		APP_LOG_W("failed\n");
		return LB_ERROR_NO_MEM;
	}

	/* Display the file list name and resource */
	lb_gal_update_flist(pproperty,
		LB_FLIST_UPDATE_ALL_ELEMS, NULL);

	return SUCCESS;
}

static lb_int32 file_list_move(void *param)
{
	lb_int32 ret = 0;
	lb_flist_t *pproperty = param;

	char par_path[128];
	char old_path[128];
	char new_path[128];
	char new_name[128];
	char *par_name = NULL;
	char *old_name = NULL;
	char *temp = NULL;
	char *suffix = NULL;
	lb_int32 len = 0;

	mars_get_current_path(pproperty->file_pri.cur_desert, &temp);
	if (NULL == temp) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	strcpy(par_path, temp);
	par_name = strrchr(par_path, '/');
	if (NULL == par_name) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}
	par_name++;

	#if 0
	if (par_name) {
		APP_LOG_W("par_path:%s\n", par_path);
		APP_LOG_W("par_name:%s\n", par_name);
	}
	#endif

	mars_get_node_path(pproperty->file_pri.cur_desert,
		pproperty->file_pri.row_index, &temp);
	if (NULL == temp) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	strcpy(old_path, temp);
	old_name = strrchr(old_path, '/') + 1;
	if (NULL == old_name) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	#if 0
	if (old_name) {
		APP_LOG_W("old_path:%s\n", old_path);
		APP_LOG_W("old_name:%s\n", old_name);
	}
	#endif

	strcpy(new_path, MARS_FD_PATH);
	strcat(new_path, "/");

	if ((strcmp(par_name, F_UNLOCK_PATH) == 0) ||
		(strcmp(par_name, R_UNLOCK_PATH) == 0)) {

		if (strcmp(par_name, F_UNLOCK_PATH) == 0)
			strcat(new_path, F_LOCK_PATH);
		else if (strcmp(par_name, R_UNLOCK_PATH) == 0)
			strcat(new_path, R_LOCK_PATH);

		suffix = strrchr(old_name, '.');
		if (NULL == suffix) {
			APP_LOG_W("failed\n");
			ret = -1;
			goto exit;
		}

		len = suffix - old_name;
		if (len) {
			memcpy(new_name, old_name, len);
			new_name[len] = '\0';
		}

		strcat(new_name, "_lock");
		strcat(new_name, suffix);
		strcat(new_path, "/");
		strcat(new_path, new_name);
	} else if ((strcmp(par_name, F_LOCK_PATH) == 0) ||
		(strcmp(par_name, R_LOCK_PATH) == 0)) {
		if (strcmp(par_name, F_LOCK_PATH) == 0)
			strcat(new_path, F_UNLOCK_PATH);
		else if (strcmp(par_name, R_LOCK_PATH) == 0)
			strcat(new_path, R_UNLOCK_PATH);

		temp = strstr(old_name, "_lock");
		if (NULL == temp) {
			APP_LOG_W("failed\n");
			ret = -1;
			goto exit;
		}

		len = temp - old_name;
		if (len) {
			memcpy(new_name, old_name, len);
			new_name[len] = '\0';
		}

		suffix = strrchr(old_name, '.');
		strcat(new_name, suffix);

		strcat(new_path, "/");
		strcat(new_path, new_name);
	} else {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	ret = mars_rename_file(pproperty->file_pri.cur_desert, old_path, new_path);
	if (0 != ret) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

exit:
	return ret;
}

static lb_int32 file_list_lock(void *param)
{
	lb_int32 ret = 0;
	lb_flist_t *pproperty = param;

	image_thumb_stop((void *)0);
	video_thumb_stop((void *)0);

	/* Find the list by id because params are buttons not the list we need */
	file_get_list(explore.tab_cur, (void **)&pproperty);
	if (NULL == pproperty) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	ret = file_list_move(pproperty);
	if (0 != ret) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	/* Display the file list name and resource */
	lb_gal_update_flist(pproperty,
		LB_FLIST_DELETE_ONE_ELEMS, NULL);

exit:
	video_thumb_start((void *)0);
	image_thumb_start((void *)0);

	return ret;
}

static lb_int32 file_list_del(void *param)
{
	lb_int32 ret = 0;
	lb_flist_t *pproperty = param;
	char *path = NULL;

	image_thumb_stop((void *)0);
	video_thumb_stop((void *)0);

	/* Find the list by id because params are buttons not the list we need */
	file_get_list(explore.tab_cur, (void **)&pproperty);
	if (NULL == pproperty) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	mars_get_node_path(pproperty->file_pri.cur_desert,
		pproperty->file_pri.row_index, &path);
	if (NULL == path) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	ret = mars_delete_file(pproperty->file_pri.cur_desert, path);
	if (0 != ret) {
		APP_LOG_W("failed\n");
		ret = -1;
		goto exit;
	}

	/* Display the file list name and resource */
	lb_gal_update_flist(pproperty,
		LB_FLIST_DELETE_ONE_ELEMS, NULL);

exit:
	video_thumb_start((void *)0);
	image_thumb_start((void *)0);

	return ret;
}

static lb_int32 video_bg_click(void *param)
{
	void *p_obj = NULL;
	lb_int32 ret = 0;
	lb_uint32 id = 0;

	id = 310;
	ret = lb_view_get_obj_ext_by_id(id, &p_obj);
	if (ret != 0) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return ret;
	}

	if (lb_gal_get_obj_hidden(p_obj) == 0)
		lb_gal_set_obj_hidden(p_obj, 1);
	else if (lb_gal_get_obj_hidden(p_obj) == 1)
		lb_gal_set_obj_hidden(p_obj, 0);

	id = 306;
	ret = lb_view_get_obj_ext_by_id(id, &p_obj);
	if (ret != 0) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return ret;
	}

	if (lb_gal_get_obj_hidden(p_obj) == 0)
		lb_gal_set_obj_hidden(p_obj, 1);
	else if (lb_gal_get_obj_hidden(p_obj) == 1)
		lb_gal_set_obj_hidden(p_obj, 0);

	return 0;
}

static lb_int32 image_bg_click(void *param)
{
	void *p_obj = NULL;
	lb_int32 ret = 0;
	lb_uint32 id = 0;

	id = 310;
	ret = lb_view_get_obj_ext_by_id(id, &p_obj);
	if (ret != 0) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return ret;
	}

	if (lb_gal_get_obj_hidden(p_obj) == 0)
		lb_gal_set_obj_hidden(p_obj, 1);
	else if (lb_gal_get_obj_hidden(p_obj) == 1)
		lb_gal_set_obj_hidden(p_obj, 0);

	id = 306;
	ret = lb_view_get_obj_ext_by_id(id, &p_obj);
	if (ret != 0) {
		printf("%s,%d,failed\n", __FILE__, __LINE__);
		return ret;
	}

	if (lb_gal_get_obj_hidden(p_obj) == 0)
		lb_gal_set_obj_hidden(p_obj, 1);
	else if (lb_gal_get_obj_hidden(p_obj) == 1)
		lb_gal_set_obj_hidden(p_obj, 0);

	return 0;
}

lb_int32 file_init_funcs(void)
{
	lb_int32 err = SUCCESS;

	err |= lb_fmngr_reg_init_func("lb_file_exp_init", file_exp_init);
	err |= lb_fmngr_reg_exit_func("lb_file_exp_exit", file_exp_exit);
	err |= lb_fmngr_reg_init_func("lb_file_video_init", file_video_init);
	err |= lb_fmngr_reg_exit_func("lb_file_video_exit", file_video_exit);
	err |= lb_fmngr_reg_init_func("lb_file_image_init", file_image_init);
	err |= lb_fmngr_reg_exit_func("lb_file_image_exit", file_image_exit);

	return err;
}
lb_int32 file_uninit_funcs(void)
{
	lb_int32 err = SUCCESS;

	err |= lb_fmngr_unreg_init_func(file_exp_init);
	err |= lb_fmngr_unreg_exit_func(file_exp_exit);
	err |= lb_fmngr_unreg_init_func(file_video_init);
	err |= lb_fmngr_unreg_exit_func(file_video_exit);
	err |= lb_fmngr_unreg_init_func(file_image_init);
	err |= lb_fmngr_unreg_exit_func(file_image_exit);

	return err;
}

lb_int32 file_resp_funcs(void)
{
	lb_int32 err = SUCCESS;

	err |= lb_reg_resp_msg_func(LB_MSG_FILEEXP_CHANGE, file_exp_change);
	err |= lb_reg_resp_msg_func(LB_MSG_FILEEXP_RETURN, file_exp_return);
	err |= lb_reg_resp_msg_func(LB_MSG_FILEEXP_ENTER, file_exp_enter);
	err |= lb_reg_resp_msg_func(LB_MSG_FLIST_LOCK, file_list_lock);
	err |= lb_reg_resp_msg_func(LB_MSG_FLIST_DEL, file_list_del);

	err |= lb_reg_resp_msg_func(LB_MSG_FILEEXP_VIDEO_BG_CLICK, video_bg_click);
	err |= lb_reg_resp_msg_func(LB_MSG_FILEEXP_IMAGE_BG_CLICK, image_bg_click);

	return err;
}

lb_int32 file_unresp_funcs(void)
{
	lb_int32 err = SUCCESS;

	err |= lb_unreg_resp_msg_func(file_exp_change);
	err |= lb_unreg_resp_msg_func(file_exp_return);
	err |= lb_unreg_resp_msg_func(file_exp_enter);
	err |= lb_unreg_resp_msg_func(file_list_lock);
	err |= lb_unreg_resp_msg_func(file_list_del);

	err |= lb_unreg_resp_msg_func(video_bg_click);
	err |= lb_unreg_resp_msg_func(image_bg_click);

	return err;
}
