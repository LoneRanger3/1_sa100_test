#define LOG_TAG		"cycle_malloc"

#include <string.h>
#include <strings.h>
#include "oscl.h"
#include <pthread.h>
#include <stdlib.h>
#include "lombo_cycle_buf.h"
#include "lombo_cycle_buf_priv.h"

#define IS_CBUF_VALID(hdl) ((hdl != NULL) && (hdl->buf != NULL) && (hdl->total_len))
#define IS_CBUF_OFFSET_VALID(hdl) \
	((((hdl)->beg_ofs + (hdl)->date_len)%(hdl->total_len)) == hdl->end_ofs)
#define __ofs_add(offset, len) (((offset) + (len) + cbuf->total_len)%(cbuf->total_len))
#define CYCLE_BUF_CONDI_TIMEOUT 3000

void print_cbuf_info(void *handle, int line)
{
	_cbuf_node_t *tmp_node = NULL;
	_cbuf_handle_t *cbuf = handle;
	int index = 0;
	int num_node_heap = 0;

	OSCL_PRINT("\n");
	OSCL_LOGE("====print circle buffer info in %s %d==", __FILE__, line);
	if (!IS_CBUF_VALID(cbuf))
		OSCL_LOGE("==== IS_CBUF_VALID fail!!! ==");
	if (!IS_CBUF_OFFSET_VALID(cbuf))
		OSCL_LOGE("==== IS_CBUF_OFFSET_VALID fail!!! ==");
	OSCL_LOGE("==cbuf buffer addr: %p(0x%lx), align:%d, total len:%d",
		cbuf->buf, cbuf->phy_addr, cbuf->align, cbuf->total_len);
	OSCL_LOGE("==beg_ofs: %x, date_len:%d, end_ofs:%x, flag:%x, blocking:%d",
		cbuf->beg_ofs, cbuf->date_len, cbuf->end_ofs,
		cbuf->blocking_flag, cbuf->inblock);
	OSCL_LOGE("==node: %p(end:%p), num:%d, node from heap:%d",
		cbuf->node_manager.buf_node,
		cbuf->node_manager.buf_node + sizeof(cbuf->node_manager.buf_node),
		cbuf->node_manager.ntotal, cbuf->node_manager.nfrom_heap);
	tmp_node = cbuf->node_manager.node_head;
	while (tmp_node) {
		OSCL_LOGE("==index:%d, node:%p, addr:%p, len:%d, next addr:%p, stat:%d",
			index, tmp_node, tmp_node->addr, tmp_node->len,
			tmp_node->addr + tmp_node->len, tmp_node->node_stat);
		index++;
		if (tmp_node->from_heap)
			num_node_heap++;
		tmp_node = tmp_node->next_node;
	}
	if (index != cbuf->node_manager.ntotal) {
		OSCL_LOGE("err !node in list is :%d, in node_manager is %d",
			index, cbuf->node_manager.ntotal);
	}
	if (num_node_heap != cbuf->node_manager.nfrom_heap) {
		OSCL_LOGE("err !num_node_heap != cbuf->node_manager.nfrom_heap (%d, %d)",
			num_node_heap, cbuf->node_manager.nfrom_heap);
	}
	OSCL_LOGE("*****print circle buffer info in %s %d end*****", __FILE__, line);
}

static _cbuf_node_t *__get_node(_cbuf_handle_t *cbuf, _cbuf_node_manager_t *manager)
{
	int i;
	_cbuf_node_t *tmp;
	int from_heap;

	i = 0;
	tmp = NULL;
	from_heap = 0;
	while (tmp == NULL && i < MAX_CBUF_NODE_BUF_NUM) {
		if (manager->buf_node[i].node_stat == NODE_FREE)
			tmp = &manager->buf_node[i];
		i++;
	}
	if (tmp == NULL) {
		OSCL_LOGI("get node from heap!");
		tmp = malloc(sizeof(_cbuf_node_t));
		from_heap = 1;
		manager->nfrom_heap++;
	}
	if (tmp) {
		memset(tmp, 0, sizeof(_cbuf_node_t));
		tmp->node_stat = NODE_DATA;
		tmp->from_heap = from_heap;
		tmp->next_node = NULL;
		if (manager->node_tail)
			manager->node_tail->next_node = tmp;
		manager->node_tail = tmp;
		if (manager->node_head == NULL)
			manager->node_head = tmp;
		manager->ntotal++;
	}
	return tmp;
}

static void __put_all_nodes(_cbuf_node_manager_t *manager)
{
	_cbuf_node_t *buf_node, *tmp;

	tmp = manager->node_head;
	while (tmp && manager->nfrom_heap) {
		buf_node = tmp;
		tmp = tmp->next_node;
		if (buf_node->from_heap) {
			OSCL_LOGI("free node from heap!");
			free(buf_node);
			manager->nfrom_heap--;
		}
	}

	manager->node_head = NULL;
	manager->node_tail = NULL;
	manager->ntotal = 0;
	manager->nfrom_heap = 0;
	memset(manager->buf_node, 0, sizeof(manager->buf_node));
}

int __condi_wait(_cbuf_handle_t *cbuf, int ms)
{
	struct timespec time;
	int ret = -1;

	if (cbuf->blocking_flag == 1) {
		clock_gettime(CLOCK_REALTIME, &time);
		time.tv_sec += ms/1000;
		time.tv_nsec += (ms%1000)*1000*1000;
		if (time.tv_nsec >= 1000 * 1000 * 1000) {
			time.tv_nsec -= 1000 * 1000 * 1000;
			time.tv_sec += 1;
		}
		ret = pthread_cond_timedwait(&cbuf->cond, &cbuf->mutex, &time);
	}
	return ret;
}

static int __wait_for_size(_cbuf_handle_t *cbuf, int size)
{
	int max_free = 0;
	while (max_free < size) {
		max_free = cbuf->total_len - cbuf->end_ofs;
		if (cbuf->end_ofs < cbuf->beg_ofs)
			max_free = cbuf->beg_ofs - cbuf->end_ofs;
		if (cbuf->date_len == cbuf->total_len)
			max_free = 0;
		if (max_free < size && cbuf->blocking_flag) {
			cbuf->inblock++;
			__condi_wait(cbuf, CYCLE_BUF_CONDI_TIMEOUT);
			cbuf->inblock--;
		}
		if (cbuf->blocking_flag == 0)
			break;
	}
	return max_free;
}

static _cbuf_node_t *__buffer_malloc(_cbuf_handle_t *cbuf, int size)
{
	_cbuf_node_t *tmpnode;
	int free_size;

	if (cbuf->total_len - cbuf->end_ofs < size) {
		OSCL_LOGE("fatal err!!");
		print_cbuf_info(cbuf, __LINE__);
		return NULL;
	}
	free_size = __wait_for_size(cbuf, size);
	if (free_size < size) {
		OSCL_LOGW("get buffer failed!");
		return NULL;
	}

	tmpnode = __get_node(cbuf, &cbuf->node_manager);
	if (tmpnode == NULL) {
		OSCL_LOGE("err! get node failed!");
		return NULL;
	}

	tmpnode->addr = cbuf->buf + cbuf->end_ofs;
	tmpnode->len = size;
	cbuf->date_len += size;
	cbuf->end_ofs = __ofs_add(cbuf->end_ofs, size);

	return tmpnode;
}

static _cbuf_node_t *__cycle_buffer_malloc(_cbuf_handle_t *cbuf, int size)
{
	_cbuf_node_t *tmpnode;
	int size_align;
	int padding_size;

	size_align = CBUF_ALIGN(size, cbuf->align);
	padding_size = cbuf->total_len - cbuf->end_ofs;
	if (size_align > padding_size) {
		tmpnode = __buffer_malloc(cbuf, padding_size);
		if (tmpnode == NULL)
			OSCL_LOGE("err! padding buffer failed!");
		else
			tmpnode->node_stat = NODE_PADDING;
	}
	tmpnode = __buffer_malloc(cbuf, size_align);
	if (cbuf->inblock)
		pthread_cond_signal(&cbuf->cond);
	if (!IS_CBUF_OFFSET_VALID(cbuf))
		OSCL_LOGE("cbuffer offset ERR!");
	return tmpnode;
}

static void __cbuf_free(_cbuf_handle_t *cbuf)
{
	_cbuf_node_t *node, *tmp;
	char is_from_heap;

	node = cbuf->node_manager.node_head;
	while (node != NULL) {
		if (node->node_stat == NODE_PADDING) {
			if (node->len > cbuf->date_len) {
				OSCL_LOGE("err! buf size > date len while freebuf");
				node->len = cbuf->date_len;
			}
			cbuf->beg_ofs = __ofs_add(cbuf->beg_ofs, node->len);
			cbuf->date_len -= node->len;
			cbuf->node_manager.node_head = node->next_node;
			if (cbuf->node_manager.node_head == NULL)
				cbuf->node_manager.node_tail = NULL;
			tmp = node;
			node = node->next_node;
			is_from_heap = tmp->from_heap;
			memset(tmp, 0, sizeof(_cbuf_node_t));
			cbuf->node_manager.ntotal--;
			if (is_from_heap) {
				cbuf->node_manager.nfrom_heap--;
				free(tmp);
				OSCL_LOGI("free node from heap!");
			}
			pthread_cond_signal(&cbuf->cond);
		} else
			break;
	}
	if (!IS_CBUF_OFFSET_VALID(cbuf))
		OSCL_LOGE("cbuffer offset ERR!");
}

void *cycle_buffer_creat(char *name, int buf_len, int align)
{
	int ret = 0;
	_cbuf_handle_t *cbuf;
	char *cname = name;

	cbuf = malloc(sizeof(_cbuf_handle_t));
	oscl_param_check_exit(cbuf != NULL, -1, NULL);
	memset(cbuf, 0, sizeof(_cbuf_handle_t));

	if (cname == NULL)
		cname = "unknown cbuffer";
	cbuf->name = oscl_strdup(cname);

	if (align > 4096) {
		OSCL_LOGE("align > 4096, set it to 4096");
		align = 4096;
	}
	buf_len = CBUF_ALIGN(buf_len, align);
#if 0
	cbuf->buf = lombo_malloc_cached(buf_len);
#else
	cbuf->buf = oscl_malloc(buf_len);
#endif
	oscl_param_check_exit(cbuf->buf != NULL, -1, NULL);
	cbuf->total_len = buf_len;
	cbuf->align = align;
	cbuf->phy_addr = (unsigned long)cbuf->buf;

	pthread_mutex_init(&cbuf->mutex, NULL);
	pthread_cond_init(&cbuf->cond, NULL);
	cbuf->blocking_flag = 1;
	cycle_buffer_reset(cbuf);

EXIT:
	if (ret) {
		OSCL_LOGE("cycle_buffer_creat %s failed!", name);
		if (cbuf) {
			cycle_buffer_destory(cbuf);
			cbuf = NULL;
		}
	}
	return cbuf;
}

void cycle_buffer_destory(void *hdle)
{
	_cbuf_handle_t *cbuf = hdle;
	if (hdle == NULL)
		return;
	if (cbuf->buf)
		cycle_buffer_reset(hdle);

	pthread_mutex_destroy(&cbuf->mutex);
	pthread_cond_destroy(&cbuf->cond);

	if (cbuf->buf) {
		oscl_free(cbuf->buf);
		cbuf->buf = NULL;
	}
	if (cbuf->name)
		oscl_free(cbuf->name);
	memset(cbuf, 0, sizeof(_cbuf_handle_t));
	free(cbuf);
}

int cycle_buffer_reset(void *handle)
{
	_cbuf_handle_t *cbuf = handle;

	oscl_param_check(cbuf != NULL, -1, NULL);
	oscl_param_check(cbuf->buf != NULL, -1, NULL);

	while (cbuf->inblock) {
		OSCL_LOGE("someone(%d) is in block, try wakup.", cbuf->inblock);
		cbuf->blocking_flag = 0;
		pthread_cond_signal(&cbuf->cond);
		oscl_mdelay(10);
	}
	pthread_mutex_lock(&cbuf->mutex);
	cbuf->beg_ofs = 0;
	cbuf->end_ofs = 0;
	cbuf->date_len = 0;
	cbuf->inblock = 0;
	cbuf->blocking_flag = 1;
	__put_all_nodes(&cbuf->node_manager);
	pthread_mutex_unlock(&cbuf->mutex);
	return 0;
}

/* 1:enable block. 0:disable block. default value:1 */
int cycle_buffer_set_block(void *handle, int block_enable)
{
	_cbuf_handle_t *cbuf = handle;

	oscl_param_check(cbuf != NULL, -1, NULL);
	oscl_param_check(cbuf->buf != NULL, -1, NULL);

	pthread_mutex_lock(&cbuf->mutex);
	cbuf->blocking_flag = block_enable;
	if (cbuf->blocking_flag == 0)
		pthread_cond_signal(&cbuf->cond);
	pthread_mutex_unlock(&cbuf->mutex);
	return 0;
}

void *cycle_buffer_malloc(void *handle, int size)
{
	_cbuf_handle_t *cbuf = handle;
	_cbuf_node_t *tmpnode;
	void *tmpbuf = NULL;

	oscl_param_check(cbuf != NULL, NULL, NULL);
	oscl_param_check(cbuf != NULL, NULL, NULL);
	oscl_param_check(size != 0, NULL, NULL);
	pthread_mutex_lock(&cbuf->mutex);
	if (size > cbuf->total_len) {
		OSCL_LOGE("err! size %d > cyc buf totallen:%d", size, cbuf->total_len);
		goto EXIT;
	}

	tmpnode = __cycle_buffer_malloc(cbuf, size);
	if (tmpnode)
		tmpbuf = tmpnode->addr;

EXIT:
	pthread_mutex_unlock(&cbuf->mutex);
	return tmpbuf;
}

void cycle_buffer_free(void *handle, void *ptr)
{
	_cbuf_handle_t *cbuf = handle;
	_cbuf_node_t *node = NULL;

	if (cbuf == NULL || ptr == NULL)
		return;
	pthread_mutex_lock(&cbuf->mutex);
	if (!IS_CBUF_VALID(cbuf) || !IS_CBUF_OFFSET_VALID(cbuf) ||
		cbuf->node_manager.node_head == NULL) {
		OSCL_LOGE("cycle_buffer_free %p err! ", ptr);
		goto EXIT;
	}

	node = cbuf->node_manager.node_head;
	while (node) {
		if (node->addr == ptr) {
			node->node_stat = NODE_PADDING;
			break;
		}
		node = node->next_node;
	}
	if (node != cbuf->node_manager.node_head) {
		if (node == NULL)
			OSCL_LOGW("err! can not find ptr:%p!!!", ptr);
		if (cbuf->node_manager.node_head->node_stat != NODE_PADDING)
			OSCL_LOGW("err! ptr:%p not malloc firstly. (%p)!!!", ptr, node);
	}
	__cbuf_free(cbuf);
EXIT:
	pthread_mutex_unlock(&cbuf->mutex);
	return;
}


void *cycle_buffer_realloc(void *handle, void *ptr, int size)
{
	_cbuf_handle_t *cbuf = handle;
	_cbuf_node_t *cur_node = NULL;
	_cbuf_node_t *new_node = NULL;
	void *tmpbuf = NULL;
	int size_align;
	int max_exsize;

	oscl_param_check(cbuf != NULL, NULL, NULL);
	oscl_param_check(cbuf->buf != NULL, NULL, NULL);

	if (size == 0) {
		OSCL_LOGE("cycle_buffer_realloc size = 0, will free it");
		cycle_buffer_free(handle, ptr);
		return NULL;
	}
	if (ptr == NULL)
		return cycle_buffer_malloc(handle, size);

	pthread_mutex_lock(&cbuf->mutex);
	if (cbuf->node_manager.node_tail->addr == ptr)
		cur_node = cbuf->node_manager.node_tail;
	if (cur_node == NULL)
		OSCL_LOGE("err! realloc buffer is not the last buffer");

	/* try to expands buffer size */
	size_align = CBUF_ALIGN(size, cbuf->align);
	max_exsize = 0;
	if (cur_node && cbuf->total_len - cbuf->end_ofs >= size_align - cur_node->len) {
		max_exsize = __wait_for_size(cbuf, size_align - cur_node->len);
		if (cbuf->blocking_flag == 0 && max_exsize < size_align - cur_node->len)
			goto EXIT;
	}
	if (cur_node && cur_node->len + max_exsize >= size_align) {
		tmpbuf = cur_node->addr;
		cbuf->date_len += (size_align - cur_node->len);
		cbuf->end_ofs = __ofs_add(cbuf->end_ofs, size_align - cur_node->len);
		cur_node->len = size_align;
		goto EXIT;
	}

	/* failed to expands, realloc buffer */
	if (cur_node == NULL) {
		cur_node = cbuf->node_manager.node_head;
		while (cur_node) {
			if (cur_node->addr == ptr)
				break;
			cur_node = cur_node->next_node;
		}
		/* buffer is not at the tail, and resize < orign size, do nothing */
		if (cur_node && cur_node->len > size_align)
			goto EXIT;
	}
	if (cur_node == NULL) {
		OSCL_LOGE("can not find %p in list", ptr);
		goto EXIT;
	}

	new_node = __cycle_buffer_malloc(cbuf, size_align);
	if (new_node) {
		tmpbuf = new_node->addr;
		memcpy(new_node->addr, cur_node->addr, cur_node->len);
	}
	cur_node->node_stat = NODE_PADDING;
	__cbuf_free(cbuf);
EXIT:
	pthread_mutex_unlock(&cbuf->mutex);
	return tmpbuf;
}

unsigned long cycle_buffer_vir_to_phy(void *handle, void *vir_addr)
{
	_cbuf_handle_t *cbuf = handle;
	unsigned long phy_addr = 0;
	oscl_param_check(cbuf != NULL, -1, NULL);
	oscl_param_check(cbuf->buf != NULL, -1, NULL);

	if (vir_addr >= cbuf->buf && vir_addr < cbuf->buf + cbuf->total_len)
		phy_addr = (cbuf->phy_addr + (vir_addr - cbuf->buf));
	return phy_addr;
}

void cycle_buffer_log(void *handle, int line)
{
	_cbuf_handle_t *cbuf = handle;
	pthread_mutex_lock(&cbuf->mutex);
	print_cbuf_info(cbuf, line);
	pthread_mutex_unlock(&cbuf->mutex);
}

