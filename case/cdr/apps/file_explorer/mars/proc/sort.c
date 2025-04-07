/*
 * sort.c - the sort of list code for LomboTech
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

#include <mars/proc/sort.h>

static lb_uint8 exte_val[128] = {
	0,   1,   2,   3,   4,   5,   6,   7,   8,   9,   10,  11,  12,  13,  14,  15,
	16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,
	32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,
	48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,

	64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,
	80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95,
	96,  97,  98,  99,  100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
	112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127
};

static lb_uint8 ascend_val[128] = {
	0,   1,   2,   3,   4,   5,   6,   7,   8,   9,   10,  11,  12,  13,  14,  15,
	16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,
	32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,
	48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,

	64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,
	80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95,
	96,  97,  98,  99,  100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
	112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127
};

static lb_uint8 descend_val[128] = {
	0,   1,   2,   3,   4,   5,   6,   7,   8,   9,   10,  11,  12,  13,  14,  15,
	16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,
	32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,
	57,  56,  55,  54,  53,  52,  51,  50,  49,  48,  58,  59,  60,  61,  62,  63,

	64,  90,  89,  88,  87,  86,  85,  84,  83,  82,  81,  80,  79,  78,  77,  76,
	75,  74,  73,  72,  71,  70,  69,  68,  67,  66,  65,  91,  92,  93,  94,  95,
	96,  122, 121, 120, 119, 118, 117, 116, 115, 114, 113, 112, 111, 110, 109, 108,
	107, 106, 105, 104, 103, 102, 101, 100, 99,  98,  97,  123, 124, 125, 126, 127
};

#if 0
static char lower[] = {
	/* 97-122 */
	'a', 'b', 'c', 'd', 'e',
	'f', 'g', 'h', 'i', 'j',
	'k', 'l', 'm', 'n', 'o',
	'p', 'q', 'r', 's', 't',
	'u', 'v', 'w', 'x', 'y',
	'z'
};

static char upper[] = {
	/* 65-90 */
	'A', 'B', 'C', 'D', 'E',
	'F', 'G', 'H', 'I', 'J',
	'K', 'L', 'M', 'N', 'O',
	'P', 'Q', 'R', 'S', 'T',
	'U', 'V', 'W', 'X', 'Y',
	'Z'
};

static char ditigal[] = {
	/* 48-57 */
	'0', '1', '2', '3', '4',
	'9', '8', '7', '6', '5'
};
#endif

static lb_uint8 sort_map[128];
static lb_uint8 *sort_space;

static lb_int32 map_create(sort_type_t sort_type)
{
	lb_uint8 i;
	lb_uint8 trans[128];

	for (i = 0; i < 128; i++) {
		switch (sort_type) {
		case SORT_ASCENDING:
			trans[i] = ascend_val[i];
			break;
		case SORT_DESCENDING:
			trans[i] = descend_val[i];
			break;
		case SORT_EXTENSION:
			trans[i] = exte_val[i];
			break;
		default:
			trans[i] = exte_val[i];
			break;
		}
		sort_map[trans[i]] = i;
	}

	return 0;
}

static lb_int32 name_comp(void *ptr1, void *ptr2)
{
	lb_int32 i;
	list_node_t *node1;
	list_node_t *node2;

	node1 = (list_node_t *)ptr1;
	node2 = (list_node_t *)ptr2;

	i = 0;
	while (node1->name[i] != '\0' && node2->name[i] != '\0') {
		if (node1->name[i] != node2->name[i]) {
			lb_uint8 pos1;
			lb_uint8 pos2;

			pos1 = (lb_uint8)node1->name[i];
			pos2 = (lb_uint8)node2->name[i];

			return sort_map[pos1] - sort_map[pos2];
		}
		i++;
	}

	if (node1->name[i] == '\0')
		return -1;

	if (node2->name[i] == '\0')
		return 1;

	return 0;
}

static lb_int32 nd_type_comp(void *ptr1, void *ptr2)
{
	list_node_t *node1;
	list_node_t *node2;

	node1 = (list_node_t *)ptr1;
	node2 = (list_node_t *)ptr2;

	return (lb_int32)(node1->nd_type - node2->nd_type);
}

#if 0
static lb_int32 me_type_comp(void *ptr1, void *ptr2)
{
	list_node_t *node1;
	list_node_t *node2;

	node1 = (list_node_t *)ptr1;
	node2 = (list_node_t *)ptr2;

	return (lb_int32)(node1->me_type - node2->me_type);
}
#endif

static lb_int32 sing_merge(void *ptr, lb_int32 left, lb_int32 mid, lb_int32 right,
	lb_int32(*comp)(void *, void *))
{
	list_node_t *temp0;
	list_node_t *temp1;
	list_node_t **list_node;
	list_node_t *temp_node;
	lb_int32 i, j;
	lb_int32 len, pos;

	list_node = (list_node_t **)ptr;
	temp_node = (list_node_t *)list_node;

	temp0 = (list_node_t *)sort_space;
	temp1 = 0;

	len = right - left + 1;
	pos = 0;

	i = left;
	j = mid + 1;

	while (i <= mid && j <= right) {
		if (comp(temp_node + i, temp_node + j) <= 0) {
			temp1 = temp0 + pos++;
			memcpy(temp1, (temp_node + i++), sizeof(list_node_t));
		} else {
			temp1 = temp0 + pos++;
			memcpy(temp1, (temp_node + j++), sizeof(list_node_t));
		}
	}

	while (i <= mid) {
		temp1 = temp0 + pos++;
		memcpy(temp1, (temp_node + i++), sizeof(list_node_t));
	}

	while (j <= right) {
		temp1 = temp0 + pos++;
		memcpy(temp1, (temp_node + j++), sizeof(list_node_t));
	}

	for (i = 0; i < len; i++) {
		temp1 = temp0 + i;
		memcpy((temp_node + left++), temp1, sizeof(list_node_t));
	}

	return 0;
}

static lb_int32 sort_merge(void *ptr, lb_int32 left, lb_int32 right,
	lb_int32(*comp)(void *, void *))
{
	lb_int32 mid;

	if (left == right)
		return 0;

	mid = (left + right) >> 1;

	if (sort_merge(ptr, left, mid, comp) != 0)
		return -1;

	if (sort_merge(ptr, mid + 1, right, comp) != 0)
		return -1;

	if (sing_merge(ptr, left, mid, right, comp) != 0)
		return -1;

	return 0;
}

static lb_int32 space_alloc(lb_int32 num)
{
	lb_int32 ret = 0;

	if (num) {
		sort_space = mars_mem_alloc(num * sizeof(list_node_t));
		if (sort_space == NULL) {
			printf("[%s,%d]alloc mem for sorting failed\n",
				__func__, __LINE__);
			ret = -1;
			goto exit;
		}
	}

exit:
	return ret;
}

static lb_int32 space_free(file_list_t *file_list)
{
	lb_int32 ret = 0;

	if (sort_space) {
		mars_mem_free(sort_space);
		sort_space = NULL;
	}

	return ret;
}

lb_int32 sort_list(file_list_t *file_list)
{
	lb_int32 ret = 0;

	map_create(file_list->st_type);

	ret = space_alloc(file_list->act_num);
	if (ret != 0) {
		printf("[%s,%d]alloc space for sorting failed\n",
			__func__, __LINE__);
		ret = -1;
		goto exit;
	}

	if (file_list->dir_num > 1 && file_list->file_num > 1)
		sort_merge((void *)file_list->list_node, 0,
			file_list->act_num - 1, nd_type_comp);

	if (file_list->dir_num > 1)
		sort_merge((void *)file_list->list_node, 0,
			file_list->dir_num - 1, name_comp);

	if (file_list->file_num > 1)
		sort_merge((void *)file_list->list_node, file_list->dir_num,
			file_list->act_num - 1, name_comp);

	space_free(file_list);

exit:
	return ret;
}
