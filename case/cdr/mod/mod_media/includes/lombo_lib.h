#ifndef LOMBO_LIB_H
#define LOMBO_LIB_H
#include <pthread.h>
#include <semaphore.h>

struct _mmm_task {
	pthread_t mmm_thread;
};

struct _mmm_mutex {
	pthread_mutex_t mmm_mutex;
};

struct _mmm_sem {
	sem_t mmm_sem;
};

struct _mmm_cond {
	pthread_cond_t mmm_cond;
};

typedef struct _mmm_task mmm_task_t;
typedef struct _mmm_sem mmm_sem_t;
typedef struct _mmm_mutex mmm_mutex_t;
typedef struct _mmm_cond mmm_cond_t;

int mmm_task_dispose(mmm_task_t *mmm_task);
int mmm_task_exit();
int mmm_task_creat(mmm_task_t *mmm_task, void *(*task)(void *param),
			void *param, int prio, int realtime, int stack_size);

int mmm_sem_init(mmm_sem_t *sem , unsigned int cnt);
int mmm_sem_wait(mmm_sem_t *sem);
int mmm_sem_post(mmm_sem_t *sem);
int mmm_sem_del(mmm_sem_t *sem);

int mmm_cond_create(mmm_cond_t *cond);
int mmm_cond_wait(mmm_cond_t *cond, mmm_mutex_t *mutex);
int mmm_cond_post(mmm_cond_t *cond);
int mmm_cond_del(mmm_cond_t *cond);

int mmm_mutex_init(mmm_mutex_t *mutex);
int mmm_mutex_lock(mmm_mutex_t *mutex);
int mmm_mutex_unlock(mmm_mutex_t *mutex);
int mmm_mutex_del(mmm_mutex_t *mutex);

void *load_library(char *lib_name);
void *get_library_entry(void *lib_handle, const char *symbol);
void unload_library(void *lib_handle);

void *lombo_al_malloc(int size, int flag, unsigned int *phy_addr,
			char *file, int line);
void lombo_al_free(void *ptr, int flag);
void *lombo_malloc(int size, int flag, unsigned int *phy_addr,
			char *file, int line);
void lombo_free(void *ptr, int flag);
int lombo_printf(const char *format, ...);
int lombo_sprintf(char **ret, const char *format, ...);
int lombo_warn(const char *file, int line, const char *fmt, ...);
void *lombo_memcpy(void *dst, void *src, int size);
void *lombo_memset(void *prt, int val, int size);
void *lombo_calloc(int num, int size, char *file, int line);
int lombo_cache_flush(unsigned int phy_addr);
unsigned int lombo_vir_to_fd(void *vir_addr_align);
unsigned int lombo_vir_to_phy(void *vir_addr);
unsigned int lombo_fd_to_phy(unsigned int fd);

enum mem_type_t {
	MEM_VIRT = 0,
	MEM_WB,
	MEM_WT,
	MEM_UC,
};
#endif
