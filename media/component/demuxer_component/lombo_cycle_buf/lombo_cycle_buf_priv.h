#ifndef __LOMBO_CYCLE_BUF_PRIVATE__
#define __LOMBO_CYCLE_BUF_PRIVATE__

#define CBUF_ALIGN(x, a) (((x) + (a) - 1) & ~((a) - 1))

#define MAX_CBUF_NODE_BUF_NUM 256

typedef enum {
	NODE_FREE = 0,
	NODE_DATA,
	NODE_PADDING,
} _node_stat_e;

typedef struct _cbuf_node {
	struct _cbuf_node *next_node;
	char *addr;
	int len;
	int index;
	_node_stat_e node_stat;
	char from_heap;
} _cbuf_node_t;

typedef struct {
	_cbuf_node_t *node_head;
	_cbuf_node_t *node_tail;
	int ntotal;
	int nfrom_heap;
	_cbuf_node_t buf_node[MAX_CBUF_NODE_BUF_NUM];
} _cbuf_node_manager_t;

typedef struct cycle_buf_handle {
	char *name;
	void *buf;
	unsigned long phy_addr;
	int total_len;
	int align;

	int beg_ofs;
	int date_len;
	int end_ofs;

	_cbuf_node_manager_t node_manager;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	int blocking_flag;
	int inblock;
} _cbuf_handle_t;

void print_cbuf_info(void *handle, int line);

#endif
