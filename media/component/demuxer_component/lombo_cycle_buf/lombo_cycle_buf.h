#ifndef __LOMBO_CYCLE_BUFFER_H__
#define __LOMBO_CYCLE_BUFFER_H__

void *cycle_buffer_creat(char *name, int buf_len, int align);
void cycle_buffer_destory(void *hdle);
int cycle_buffer_reset(void *handle);
/* 1:enable block. 0:disable block. default value:1 */
int cycle_buffer_set_block(void *handle, int block_enable);
void *cycle_buffer_malloc(void *handle, int size);
void cycle_buffer_free(void *handle, void *ptr);
void *cycle_buffer_realloc(void *handle, void *ptr, int size);
unsigned long cycle_buffer_vir_to_phy(void *handle, void *vir_addr);
void cycle_buffer_log(void *handle, int line);
#endif
