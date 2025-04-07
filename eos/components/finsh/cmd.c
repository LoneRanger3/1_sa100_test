
/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2006-04-30     Bernard      first implementation
 * 2006-05-04     Bernard      add list_thread,
 *                                 list_sem,
 *                                 list_timer
 * 2006-05-20     Bernard      add list_mutex,
 *                                 list_mailbox,
 *                                 list_msgqueue,
 *                                 list_event,
 *                                 list_fevent,
 *                                 list_mempool
 * 2006-06-03     Bernard      display stack information in list_thread
 * 2006-08-10     Bernard      change version to invoke rt_show_version
 * 2008-09-10     Bernard      update the list function for finsh syscall
 *                                 list and sysvar list
 * 2009-05-30     Bernard      add list_device
 * 2010-04-21     yi.qiu       add list_module
 * 2012-04-29     goprife      improve the command line auto-complete feature.
 * 2012-06-02     lgnq         add list_memheap
 * 2012-10-22     Bernard      add MS VC++ patch.
 * 2016-06-02     armink       beautify the list_thread command
 * 2018-11-22     Jesven       list_thread add smp support
 * 2018-12-27     Jesven       Fix the problem that disable interrupt too long in list_thread 
 *                             Provide protection for the "first layer of objects" when list_*
 */

#include <rthw.h>
#include <rtthread.h>

#ifdef RT_USING_FINSH

#include "finsh.h"

#define LIST_FIND_OBJ_NR 8

long hello(void)
{
    rt_kprintf("Hello RT-Thread!\n");

    return 0;
}
FINSH_FUNCTION_EXPORT(hello, say hello world);

extern void rt_show_version(void);
long version(void)
{
    rt_show_version();

    return 0;
}
FINSH_FUNCTION_EXPORT(version, show RT-Thread version information);
MSH_CMD_EXPORT(version, show RT-Thread version information);

rt_inline void object_split(int len)
{
    while (len--) rt_kprintf("-");
}

typedef struct
{
    rt_list_t *list;
    rt_list_t **array;
    rt_uint8_t type;
    int nr;             /* input: max nr, can't be 0 */
    int nr_out;         /* out: got nr */
} list_get_next_t;

static void list_find_init(list_get_next_t *p, rt_uint8_t type, rt_list_t **array, int nr)
{
    struct rt_object_information *info;
    rt_list_t *list;

    info = rt_object_get_information((enum rt_object_class_type)type);
    list = &info->object_list;

    p->list = list;
    p->type = type;
    p->array = array;
    p->nr = nr;
    p->nr_out = 0;
}

static rt_list_t *list_get_next(rt_list_t *current, list_get_next_t *arg)
{
    int first_flag = 0;
    rt_ubase_t level;
    rt_list_t *node, *list;
    rt_list_t **array;
    int nr;

    arg->nr_out = 0;

    if (!arg->nr || !arg->type)
    {
        return (rt_list_t *)RT_NULL;
    }

    list = arg->list;

    if (!current) /* find first */
    {
        node = list;
        first_flag = 1;
    }
    else
    {
        node = current;
    }

    level = rt_hw_interrupt_disable();

    if (!first_flag)
    {
        struct rt_object *obj;
        /* The node in the list? */
        obj = rt_list_entry(node, struct rt_object, list);
        if ((obj->type & ~RT_Object_Class_Static) != arg->type)
        {
            rt_hw_interrupt_enable(level);
            return (rt_list_t *)RT_NULL;
        }
    }

    nr = 0;
    array = arg->array;
    while (1)
    {
        node = node->next;

        if (node == list)
        {
            node = (rt_list_t *)RT_NULL;
            break;
        }
        nr++;
        *array++ = node;
        if (nr == arg->nr)
        {
            break;
        }
    }
    
    rt_hw_interrupt_enable(level);
    arg->nr_out = nr;
    return node;
}

long list_thread(void)
{
    rt_ubase_t level;
    list_get_next_t find_arg;
    rt_list_t *obj_list[LIST_FIND_OBJ_NR];
    rt_list_t *next = (rt_list_t*)RT_NULL;
    const char *item_title = "thread";
    int maxlen;
#ifdef ARCH_LOMBO
	rt_ubase_t level2;
	level2 = rt_hw_interrupt_disable();
#endif

    list_find_init(&find_arg, RT_Object_Class_Thread, obj_list, sizeof(obj_list)/sizeof(obj_list[0]));

    maxlen = RT_NAME_MAX;

#ifdef RT_USING_SMP
    rt_kprintf("%-*.s cpu pri  status      sp     stack size max used left tick  error\n", maxlen, item_title); object_split(maxlen);
    rt_kprintf(     " --- ---  ------- ---------- ----------  ------  ---------- ---\n");
#else
    rt_kprintf("%-*.s pri  status      sp     stack size max used left tick  error\n", maxlen, item_title); object_split(maxlen);
    rt_kprintf(     " ---  ------- ---------- ----------  ------  ---------- ---\n");
#endif /*RT_USING_SMP*/

    do
    {
        next = list_get_next(next, &find_arg);
        {
            int i;
            for (i = 0; i < find_arg.nr_out; i++)
            {
                struct rt_object *obj;
                struct rt_thread thread_info, *thread;

                obj = rt_list_entry(obj_list[i], struct rt_object, list);
                level = rt_hw_interrupt_disable();

                if ((obj->type & ~RT_Object_Class_Static) != find_arg.type)
                {
                    rt_hw_interrupt_enable(level);
                    continue;
                }
                /* copy info */
                memcpy(&thread_info, obj, sizeof thread_info);
                rt_hw_interrupt_enable(level);

                thread = (struct rt_thread*)obj;
                {
                    rt_uint8_t stat;
                    rt_uint8_t *ptr;

#ifdef RT_USING_SMP
                    if (thread->oncpu != RT_CPU_DETACHED)
                        rt_kprintf("%-*.*s %3d %3d ", maxlen, RT_NAME_MAX, thread->name, thread->oncpu, thread->current_priority);
                    else
                        rt_kprintf("%-*.*s N/A %3d ", maxlen, RT_NAME_MAX, thread->name, thread->current_priority);

#else
                    rt_kprintf("%-*.*s %3d ", maxlen, RT_NAME_MAX, thread->name, thread->current_priority);
#endif /*RT_USING_SMP*/
                    stat = (thread->stat & RT_THREAD_STAT_MASK);
                    if (stat == RT_THREAD_READY)        rt_kprintf(" ready  ");
                    else if (stat == RT_THREAD_SUSPEND) rt_kprintf(" suspend");
                    else if (stat == RT_THREAD_INIT)    rt_kprintf(" init   ");
                    else if (stat == RT_THREAD_CLOSE)   rt_kprintf(" close  ");
                    else if (stat == RT_THREAD_RUNNING) rt_kprintf(" running");

#if defined(ARCH_CPU_STACK_GROWS_UPWARD)
                    ptr = (rt_uint8_t *)thread->stack_addr + thread->stack_size - 1;
                    while (*ptr == '#')ptr --;

                    rt_kprintf(" 0x%08x 0x%08x    %02d%%   0x%08x %03d\n",
                            ((rt_ubase_t)thread->sp - (rt_ubase_t)thread->stack_addr),
                            thread->stack_size,
                            ((rt_ubase_t)ptr - (rt_ubase_t)thread->stack_addr) * 100 / thread->stack_size,
                            thread->remaining_tick,
                            thread->error);
#else
                    ptr = (rt_uint8_t *)thread->stack_addr;
                    while (*ptr == '#')ptr ++;

                    rt_kprintf(" 0x%08x 0x%08x    %02d%%   0x%08x %03d\n",
                            thread->stack_size + ((rt_ubase_t)thread->stack_addr - (rt_ubase_t)thread->sp),
                            thread->stack_size,
                            (thread->stack_size - ((rt_ubase_t) ptr - (rt_ubase_t) thread->stack_addr)) * 100
                            / thread->stack_size,
                            thread->remaining_tick,
                            thread->error);
#endif
                }
            }
        }
    }
    while (next != (rt_list_t*)RT_NULL);

#ifdef ARCH_LOMBO
	rt_hw_interrupt_enable(level2);
#endif
    return 0;
}
FINSH_FUNCTION_EXPORT(list_thread, list thread);
MSH_CMD_EXPORT(list_thread, list thread);

static void show_wait_queue(struct rt_list_node *list)
{
    struct rt_thread *thread;
    struct rt_list_node *node;

    for (node = list->next; node != list; node = node->next)
    {
        thread = rt_list_entry(node, struct rt_thread, tlist);
        rt_kprintf("%s", thread->name);

        if (node->next != list)
            rt_kprintf("/");
    }
}

#ifdef RT_USING_SEMAPHORE
long list_sem(void)
{
    rt_ubase_t level;
    list_get_next_t find_arg;
    rt_list_t *obj_list[LIST_FIND_OBJ_NR];
    rt_list_t *next = (rt_list_t*)RT_NULL;

    int maxlen;
    const char *item_title = "semaphore";
#ifdef ARCH_LOMBO
	rt_ubase_t level2;
	level2 = rt_hw_interrupt_disable();
#endif

    list_find_init(&find_arg, RT_Object_Class_Semaphore, obj_list, sizeof(obj_list)/sizeof(obj_list[0]));

    maxlen = RT_NAME_MAX;

/* #if defined(ARCH_LOMBO) && defined(RT_USING_HOOK) && defined(RT_DEBUG) */
#if 0 /* libs should be re-compiled after rt_ipc_object modified, so comment it temply */
	rt_kprintf("%-*.s v   owner_thread suspend thread\n", maxlen, item_title);
	object_split(maxlen);
	rt_kprintf(" --- ------------ --------------\n");
#else
    rt_kprintf("%-*.s v   suspend thread\n", maxlen, item_title); object_split(maxlen);
    rt_kprintf(     " --- --------------\n");
#endif

    do
    {
        next = list_get_next(next, &find_arg);
        {
            int i;
            for (i = 0; i < find_arg.nr_out; i++)
            {
                struct rt_object *obj;
                struct rt_semaphore *sem;

                obj = rt_list_entry(obj_list[i], struct rt_object, list);
                level = rt_hw_interrupt_disable();
                if ((obj->type & ~RT_Object_Class_Static) != find_arg.type)
                {
                    rt_hw_interrupt_enable(level);
                    continue;
                }
                rt_hw_interrupt_enable(level);

                sem = (struct rt_semaphore*)obj;
                if (!rt_list_isempty(&sem->parent.suspend_thread))
                {
/* #if defined(ARCH_LOMBO) && defined(RT_USING_HOOK) && defined(RT_DEBUG) */
#if 0 /* libs should be re-compiled after rt_ipc_object modified, so comment it temply */
		rt_kprintf("%-*.*s %03d ", maxlen, RT_NAME_MAX,
			sem->parent.parent.name, sem->value);
		if (0 == sem->parent.owner[0])
			rt_kprintf("(NULL)       %d:",
				rt_list_len(&sem->parent.suspend_thread));
		else
			rt_kprintf("%-12s %d:", sem->parent.owner,
				rt_list_len(&sem->parent.suspend_thread));
#else
                    rt_kprintf("%-*.*s %03d %d:",
                            maxlen, RT_NAME_MAX,
                            sem->parent.parent.name,
                            sem->value,
                            rt_list_len(&sem->parent.suspend_thread));
#endif
                    show_wait_queue(&(sem->parent.suspend_thread));
                    rt_kprintf("\n");
                }
                else
                {
/* #if defined(ARCH_LOMBO) && defined(RT_USING_HOOK) && defined(RT_DEBUG) */
#if 0 /* libs should be re-compiled after rt_ipc_object modified, so comment it temply */
		rt_kprintf("%-*.*s %03d ", maxlen, RT_NAME_MAX,
			sem->parent.parent.name, sem->value);
		if (0 == sem->parent.owner[0])
			rt_kprintf("(NULL)       %d\n",
				rt_list_len(&sem->parent.suspend_thread));
		else
			rt_kprintf("%-12s %d\n", sem->parent.owner,
				rt_list_len(&sem->parent.suspend_thread));
#else
                    rt_kprintf("%-*.*s %03d %d\n",
                            maxlen, RT_NAME_MAX,
                            sem->parent.parent.name,
                            sem->value,
                            rt_list_len(&sem->parent.suspend_thread));
#endif
                }
            }
        }
    }
    while (next != (rt_list_t*)RT_NULL);

#ifdef ARCH_LOMBO
	rt_hw_interrupt_enable(level2);
#endif
    return 0;
}
FINSH_FUNCTION_EXPORT(list_sem, list semaphore in system);
MSH_CMD_EXPORT(list_sem, list semaphore in system);
#endif

#ifdef RT_USING_EVENT
long list_event(void)
{
    rt_ubase_t level;
    list_get_next_t find_arg;
    rt_list_t *obj_list[LIST_FIND_OBJ_NR];
    rt_list_t *next = (rt_list_t*)RT_NULL;

    int maxlen;
    const char *item_title = "event";

    list_find_init(&find_arg, RT_Object_Class_Event, obj_list, sizeof(obj_list)/sizeof(obj_list[0]));

    maxlen = RT_NAME_MAX;

    rt_kprintf("%-*.s      set    suspend thread\n", maxlen, item_title); object_split(maxlen);
    rt_kprintf(     "  ---------- --------------\n");

    do
    {
        next = list_get_next(next, &find_arg);
        {
            int i;
            for (i = 0; i < find_arg.nr_out; i++)
            {
                struct rt_object *obj;
                struct rt_event *e;

                obj = rt_list_entry(obj_list[i], struct rt_object, list);
                level = rt_hw_interrupt_disable();
                if ((obj->type & ~RT_Object_Class_Static) != find_arg.type)
                {
                    rt_hw_interrupt_enable(level);
                    continue;
                }

                rt_hw_interrupt_enable(level);

                e = (struct rt_event *)obj;
                if (!rt_list_isempty(&e->parent.suspend_thread))
                {
                    rt_kprintf("%-*.*s  0x%08x %03d:",
                            maxlen, RT_NAME_MAX,
                            e->parent.parent.name,
                            e->set,
                            rt_list_len(&e->parent.suspend_thread));
                    show_wait_queue(&(e->parent.suspend_thread));
                    rt_kprintf("\n");
                }
                else
                {
                    rt_kprintf("%-*.*s  0x%08x 0\n",
                            maxlen, RT_NAME_MAX, e->parent.parent.name, e->set);
                }
            }
        }
    }
    while (next != (rt_list_t*)RT_NULL);

    return 0;
}
FINSH_FUNCTION_EXPORT(list_event, list event in system);
MSH_CMD_EXPORT(list_event, list event in system);
#endif

#ifdef RT_USING_MUTEX
long list_mutex(void)
{
    rt_ubase_t level;
    list_get_next_t find_arg;
    rt_list_t *obj_list[LIST_FIND_OBJ_NR];
    rt_list_t *next = (rt_list_t*)RT_NULL;

    int maxlen;
    const char *item_title = "mutex";
#ifdef ARCH_LOMBO
	rt_ubase_t level2;
	level2 = rt_hw_interrupt_disable();
#endif

    list_find_init(&find_arg, RT_Object_Class_Mutex, obj_list, sizeof(obj_list)/sizeof(obj_list[0]));

    maxlen = RT_NAME_MAX;

/* #if defined(ARCH_LOMBO) && defined(RT_USING_HOOK) && defined(RT_DEBUG) */
#if 0 /* libs should be re-compiled after rt_ipc_object modified, so comment it temply */
	rt_kprintf("%-*.s   owner  hold owner_thread suspend thread\n",
		maxlen, item_title);
	object_split(maxlen);
	rt_kprintf(" -------- ---- ------------ --------------\n");
#else
    rt_kprintf("%-*.s   owner  hold suspend thread\n", maxlen, item_title); object_split(maxlen);
    rt_kprintf(     " -------- ---- --------------\n");
#endif

    do
    {
        next = list_get_next(next, &find_arg);
        {
            int i;
            for (i = 0; i < find_arg.nr_out; i++)
            {
                struct rt_object *obj;
                struct rt_mutex *m;

                obj = rt_list_entry(obj_list[i], struct rt_object, list);
                level = rt_hw_interrupt_disable();
                if ((obj->type & ~RT_Object_Class_Static) != find_arg.type)
                {
                    rt_hw_interrupt_enable(level);
                    continue;
                }

                rt_hw_interrupt_enable(level);

                m = (struct rt_mutex *)obj;
/* #if defined(ARCH_LOMBO) && defined(RT_USING_HOOK) && defined(RT_DEBUG) */
#if 0 /* libs should be re-compiled after rt_ipc_object modified, so comment it temply */
		rt_kprintf("%-*.*s %-8.*s %04d ", maxlen, RT_NAME_MAX,
			m->parent.parent.name, RT_NAME_MAX, m->owner->name, m->hold);
		if (0 != m->parent.owner[0])
			rt_kprintf("%-12s %d\n", m->parent.owner,
				rt_list_len(&m->parent.suspend_thread));
		else
			rt_kprintf("(NULL)       %d\n",
				rt_list_len(&m->parent.suspend_thread));
#else
                rt_kprintf("%-*.*s %-8.*s %04d %d\n",
                        maxlen, RT_NAME_MAX,
                        m->parent.parent.name,
                        RT_NAME_MAX,
                        m->owner->name,
                        m->hold,
                        rt_list_len(&m->parent.suspend_thread));
#endif
            }
        }
    }
    while (next != (rt_list_t*)RT_NULL);

#ifdef ARCH_LOMBO
	rt_hw_interrupt_enable(level2);
#endif
    return 0;
}
FINSH_FUNCTION_EXPORT(list_mutex, list mutex in system);
MSH_CMD_EXPORT(list_mutex, list mutex in system);
#endif

#ifdef RT_USING_MAILBOX
long list_mailbox(void)
{
    rt_ubase_t level;
    list_get_next_t find_arg;
    rt_list_t *obj_list[LIST_FIND_OBJ_NR];
    rt_list_t *next = (rt_list_t*)RT_NULL;

    int maxlen;
    const char *item_title = "mailbox";

    list_find_init(&find_arg, RT_Object_Class_MailBox, obj_list, sizeof(obj_list)/sizeof(obj_list[0]));

    maxlen = RT_NAME_MAX;

    rt_kprintf("%-*.s entry size suspend thread\n", maxlen, item_title); object_split(maxlen);
    rt_kprintf(     " ----  ---- --------------\n");

    do
    {
        next = list_get_next(next, &find_arg);
        {
            int i;
            for (i = 0; i < find_arg.nr_out; i++)
            {
                struct rt_object *obj;
                struct rt_mailbox *m;

                obj = rt_list_entry(obj_list[i], struct rt_object, list);
                level = rt_hw_interrupt_disable();
                if ((obj->type & ~RT_Object_Class_Static) != find_arg.type)
                {
                    rt_hw_interrupt_enable(level);
                    continue;
                }

                rt_hw_interrupt_enable(level);

                m = (struct rt_mailbox *)obj;
                if (!rt_list_isempty(&m->parent.suspend_thread))
                {
                    rt_kprintf("%-*.*s %04d  %04d %d:",
                            maxlen, RT_NAME_MAX,
                            m->parent.parent.name,
                            m->entry,
                            m->size,
                            rt_list_len(&m->parent.suspend_thread));
                    show_wait_queue(&(m->parent.suspend_thread));
                    rt_kprintf("\n");
                }
                else
                {
                    rt_kprintf("%-*.*s %04d  %04d %d\n",
                            maxlen, RT_NAME_MAX,
                            m->parent.parent.name,
                            m->entry,
                            m->size,
                            rt_list_len(&m->parent.suspend_thread));
                }

            }
        }
    }
    while (next != (rt_list_t*)RT_NULL);

    return 0;
}
FINSH_FUNCTION_EXPORT(list_mailbox, list mail box in system);
MSH_CMD_EXPORT(list_mailbox, list mail box in system);
#endif

#ifdef RT_USING_MESSAGEQUEUE
long list_msgqueue(void)
{
    rt_ubase_t level;
    list_get_next_t find_arg;
    rt_list_t *obj_list[LIST_FIND_OBJ_NR];
    rt_list_t *next = (rt_list_t*)RT_NULL;

    int maxlen;
    const char *item_title = "msgqueue";

    list_find_init(&find_arg, RT_Object_Class_MessageQueue, obj_list, sizeof(obj_list)/sizeof(obj_list[0]));

    maxlen = RT_NAME_MAX;

    rt_kprintf("%-*.s entry suspend thread\n", maxlen, item_title); object_split(maxlen);
    rt_kprintf(     " ----  --------------\n");
    do
    {
        next = list_get_next(next, &find_arg);
        {
            int i;
            for (i = 0; i < find_arg.nr_out; i++)
            {
                struct rt_object *obj;
                struct rt_messagequeue *m;

                obj = rt_list_entry(obj_list[i], struct rt_object, list);
                level = rt_hw_interrupt_disable();
                if ((obj->type & ~RT_Object_Class_Static) != find_arg.type)
                {
                    rt_hw_interrupt_enable(level);
                    continue;
                }

                rt_hw_interrupt_enable(level);

                m = (struct rt_messagequeue *)obj;
                if (!rt_list_isempty(&m->parent.suspend_thread))
                {
                    rt_kprintf("%-*.*s %04d  %d:",
                            maxlen, RT_NAME_MAX,
                            m->parent.parent.name,
                            m->entry,
                            rt_list_len(&m->parent.suspend_thread));
                    show_wait_queue(&(m->parent.suspend_thread));
                    rt_kprintf("\n");
                }
                else
                {
                    rt_kprintf("%-*.*s %04d  %d\n",
                            maxlen, RT_NAME_MAX,
                            m->parent.parent.name,
                            m->entry,
                            rt_list_len(&m->parent.suspend_thread));
                }
            }
        }
    }
    while (next != (rt_list_t*)RT_NULL);

    return 0;
}
FINSH_FUNCTION_EXPORT(list_msgqueue, list message queue in system);
MSH_CMD_EXPORT(list_msgqueue, list message queue in system);
#endif

#ifdef RT_USING_MEMHEAP
long list_memheap(void)
{
    rt_ubase_t level;
    list_get_next_t find_arg;
    rt_list_t *obj_list[LIST_FIND_OBJ_NR];
    rt_list_t *next = (rt_list_t*)RT_NULL;

    int maxlen;
    const char *item_title = "memheap";

    list_find_init(&find_arg, RT_Object_Class_MemHeap, obj_list, sizeof(obj_list)/sizeof(obj_list[0]));

    maxlen = RT_NAME_MAX;

    rt_kprintf("%-*.s  pool size  max used size available size\n", maxlen, item_title); object_split(maxlen);
    rt_kprintf(      " ---------- ------------- --------------\n");
    do
    {
        next = list_get_next(next, &find_arg);
        {
            int i;
            for (i = 0; i < find_arg.nr_out; i++)
            {
                struct rt_object *obj;
                struct rt_memheap *mh;

                obj = rt_list_entry(obj_list[i], struct rt_object, list);
                level = rt_hw_interrupt_disable();
                if ((obj->type & ~RT_Object_Class_Static) != find_arg.type)
                {
                    rt_hw_interrupt_enable(level);
                    continue;
                }

                rt_hw_interrupt_enable(level);

                mh = (struct rt_memheap *)obj;

                rt_kprintf("%-*.*s %-010d %-013d %-05d\n",
                        maxlen, RT_NAME_MAX,
                        mh->parent.name,
                        mh->pool_size,
                        mh->max_used_size,
                        mh->available_size);

            }
        }
    }
    while (next != (rt_list_t*)RT_NULL);

    return 0;
}
FINSH_FUNCTION_EXPORT(list_memheap, list memory heap in system);
MSH_CMD_EXPORT(list_memheap, list memory heap in system);
#endif

#ifdef RT_USING_MEMPOOL
long list_mempool(void)
{
    rt_ubase_t level;
    list_get_next_t find_arg;
    rt_list_t *obj_list[LIST_FIND_OBJ_NR];
    rt_list_t *next = (rt_list_t*)RT_NULL;

    int maxlen;
    const char *item_title = "mempool";

    list_find_init(&find_arg, RT_Object_Class_MemPool, obj_list, sizeof(obj_list)/sizeof(obj_list[0]));

    maxlen = RT_NAME_MAX;

    rt_kprintf("%-*.s block total free suspend thread\n", maxlen, item_title); object_split(maxlen);
    rt_kprintf(     " ----  ----  ---- --------------\n");
    do
    {
        next = list_get_next(next, &find_arg);
        {
            int i;
            for (i = 0; i < find_arg.nr_out; i++)
            {
                struct rt_object *obj;
                struct rt_mempool *mp;

                obj = rt_list_entry(obj_list[i], struct rt_object, list);
                level = rt_hw_interrupt_disable();
                if ((obj->type & ~RT_Object_Class_Static) != find_arg.type)
                {
                    rt_hw_interrupt_enable(level);
                    continue;
                }

                rt_hw_interrupt_enable(level);

                mp = (struct rt_mempool *)obj;
                if (mp->suspend_thread_count > 0)
                {
                    rt_kprintf("%-*.*s %04d  %04d  %04d %d:",
                            maxlen, RT_NAME_MAX,
                            mp->parent.name,
                            mp->block_size,
                            mp->block_total_count,
                            mp->block_free_count,
                            mp->suspend_thread_count);
                    show_wait_queue(&(mp->suspend_thread));
                    rt_kprintf("\n");
                }
                else
                {
                    rt_kprintf("%-*.*s %04d  %04d  %04d %d\n",
                            maxlen, RT_NAME_MAX,
                            mp->parent.name,
                            mp->block_size,
                            mp->block_total_count,
                            mp->block_free_count,
                            mp->suspend_thread_count);
                }
            }
        }
    }
    while (next != (rt_list_t*)RT_NULL);

    return 0;
}
FINSH_FUNCTION_EXPORT(list_mempool, list memory pool in system)
MSH_CMD_EXPORT(list_mempool, list memory pool in system);
#endif

long list_timer(void)
{
    rt_ubase_t level;
    list_get_next_t find_arg;
    rt_list_t *obj_list[LIST_FIND_OBJ_NR];
    rt_list_t *next = (rt_list_t*)RT_NULL;

    int maxlen;
    const char *item_title = "timer";

    list_find_init(&find_arg, RT_Object_Class_Timer, obj_list, sizeof(obj_list)/sizeof(obj_list[0]));

    maxlen = RT_NAME_MAX;

    rt_kprintf("%-*.s  periodic   timeout       flag\n", maxlen, item_title); object_split(maxlen);
    rt_kprintf(     " ---------- ---------- -----------\n");
    do {
        next = list_get_next(next, &find_arg);
        {
            int i;
            for (i = 0; i < find_arg.nr_out; i++)
            {
                struct rt_object *obj;
                struct rt_timer *timer;

                obj = rt_list_entry(obj_list[i], struct rt_object, list);
                level = rt_hw_interrupt_disable();
                if ((obj->type & ~RT_Object_Class_Static) != find_arg.type)
                {
                    rt_hw_interrupt_enable(level);
                    continue;
                }

                rt_hw_interrupt_enable(level);

                timer = (struct rt_timer *)obj;
                rt_kprintf("%-*.*s 0x%08x 0x%08x ",
                        maxlen, RT_NAME_MAX,
                        timer->parent.name,
                        timer->init_tick,
                        timer->timeout_tick);
                if (timer->parent.flag & RT_TIMER_FLAG_ACTIVATED)
                    rt_kprintf("activated\n");
                else
                    rt_kprintf("deactivated\n");

            }
        }
    }
    while (next != (rt_list_t*)RT_NULL);

    rt_kprintf("current tick:0x%08x\n", rt_tick_get());

    return 0;
}
FINSH_FUNCTION_EXPORT(list_timer, list timer in system);
MSH_CMD_EXPORT(list_timer, list timer in system);

#ifdef RT_USING_DEVICE
static char *const device_type_str[] =
{
    "Character Device",
    "Block Device",
    "Network Interface",
    "MTD Device",
    "CAN Device",
    "RTC",
    "Sound Device",
    "Graphic Device",
    "I2C Bus",
    "USB Slave Device",
    "USB Host Bus",
    "SPI Bus",
    "SPI Device",
    "SDIO Bus",
    "PM Pseudo Device",
    "Pipe",
    "Portal Device",
    "Timer Device",
    "Miscellaneous Device",
    "Sensor Device",
    "Unknown"
};

long list_device(void)
{
    rt_ubase_t level;
    list_get_next_t find_arg;
    rt_list_t *obj_list[LIST_FIND_OBJ_NR];
    rt_list_t *next = (rt_list_t*)RT_NULL;

    int maxlen;
    const char *item_title = "device";

    list_find_init(&find_arg, RT_Object_Class_Device, obj_list, sizeof(obj_list)/sizeof(obj_list[0]));

    maxlen = RT_NAME_MAX;

    rt_kprintf("%-*.s         type         ref count\n", maxlen, item_title); object_split(maxlen);
    rt_kprintf(     " -------------------- ----------\n");
    do
    {
        next = list_get_next(next, &find_arg);
        {
            int i;
            for (i = 0; i < find_arg.nr_out; i++)
            {
                struct rt_object *obj;
                struct rt_device *device;

                obj = rt_list_entry(obj_list[i], struct rt_object, list);
                level = rt_hw_interrupt_disable();
                if ((obj->type & ~RT_Object_Class_Static) != find_arg.type)
                {
                    rt_hw_interrupt_enable(level);
                    continue;
                }

                rt_hw_interrupt_enable(level);

                device = (struct rt_device *)obj;
                rt_kprintf("%-*.*s %-20s %-8d\n",
                        maxlen, RT_NAME_MAX,
                        device->parent.name,
                        (device->type <= RT_Device_Class_Unknown) ?
                        device_type_str[device->type] :
                        device_type_str[RT_Device_Class_Unknown],
                        device->ref_count);

            }
        }
    }
    while (next != (rt_list_t*)RT_NULL);

    return 0;
}
FINSH_FUNCTION_EXPORT(list_device, list device in system);
MSH_CMD_EXPORT(list_device, list device in system);
#endif

long list(void)
{
#ifndef FINSH_USING_MSH_ONLY
    struct finsh_syscall_item *syscall_item;
    struct finsh_sysvar_item *sysvar_item;
#endif

    rt_kprintf("--Function List:\n");
    {
        struct finsh_syscall *index;
        for (index = _syscall_table_begin;
                index < _syscall_table_end;
                FINSH_NEXT_SYSCALL(index))
        {
            /* skip the internal command */
            if (strncmp((char *)index->name, "__", 2) == 0) continue;

#ifdef FINSH_USING_DESCRIPTION
            rt_kprintf("%-16s -- %s\n", index->name, index->desc);
#else
            rt_kprintf("%s\n", index->name);
#endif
        }
    }

#ifndef FINSH_USING_MSH_ONLY
    /* list syscall list */
    syscall_item = global_syscall_list;
    while (syscall_item != NULL)
    {
        rt_kprintf("[l] %s\n", syscall_item->syscall.name);
        syscall_item = syscall_item->next;
    }

    rt_kprintf("--Variable List:\n");
    {
        struct finsh_sysvar *index;
        for (index = _sysvar_table_begin;
                index < _sysvar_table_end;
                FINSH_NEXT_SYSVAR(index))
        {
#ifdef FINSH_USING_DESCRIPTION
            rt_kprintf("%-16s -- %s\n", index->name, index->desc);
#else
            rt_kprintf("%s\n", index->name);
#endif
        }
    }

    sysvar_item = global_sysvar_list;
    while (sysvar_item != NULL)
    {
        rt_kprintf("[l] %s\n", sysvar_item->sysvar.name);
        sysvar_item = sysvar_item->next;
    }
#endif

    return 0;
}
FINSH_FUNCTION_EXPORT(list, list all symbol in system)

#ifndef FINSH_USING_MSH_ONLY
static int str_is_prefix(const char *prefix, const char *str)
{
    while ((*prefix) && (*prefix == *str))
    {
        prefix ++;
        str ++;
    }

    if (*prefix == 0)
        return 0;

    return -1;
}

static int str_common(const char *str1, const char *str2)
{
    const char *str = str1;

    while ((*str != 0) && (*str2 != 0) && (*str == *str2))
    {
        str ++;
        str2 ++;
    }

    return (str - str1);
}

void list_prefix(char *prefix)
{
    struct finsh_syscall_item *syscall_item;
    struct finsh_sysvar_item *sysvar_item;
    rt_uint16_t func_cnt, var_cnt;
    int length, min_length;
    const char *name_ptr;

    func_cnt = 0;
    var_cnt  = 0;
    min_length = 0;
    name_ptr = RT_NULL;

    /* checks in system function call */
    {
        struct finsh_syscall *index;
        for (index = _syscall_table_begin;
                index < _syscall_table_end;
                FINSH_NEXT_SYSCALL(index))
        {
            /* skip internal command */
            if (str_is_prefix("__", index->name) == 0) continue;

            if (str_is_prefix(prefix, index->name) == 0)
            {
                if (func_cnt == 0)
                {
                    rt_kprintf("--function:\n");

                    if (*prefix != 0)
                    {
                        /* set name_ptr */
                        name_ptr = index->name;

                        /* set initial length */
                        min_length = strlen(name_ptr);
                    }
                }

                func_cnt ++;

                if (*prefix != 0)
                {
                    length = str_common(name_ptr, index->name);
                    if (length < min_length)
                        min_length = length;
                }

#ifdef FINSH_USING_DESCRIPTION
                rt_kprintf("%-16s -- %s\n", index->name, index->desc);
#else
                rt_kprintf("%s\n", index->name);
#endif
            }
        }
    }

    /* checks in dynamic system function call */
    syscall_item = global_syscall_list;
    while (syscall_item != NULL)
    {
        if (str_is_prefix(prefix, syscall_item->syscall.name) == 0)
        {
            if (func_cnt == 0)
            {
                rt_kprintf("--function:\n");
                if (*prefix != 0 && name_ptr == NULL)
                {
                    /* set name_ptr */
                    name_ptr = syscall_item->syscall.name;

                    /* set initial length */
                    min_length = strlen(name_ptr);
                }
            }

            func_cnt ++;

            if (*prefix != 0)
            {
                length = str_common(name_ptr, syscall_item->syscall.name);
                if (length < min_length)
                    min_length = length;
            }

            rt_kprintf("[l] %s\n", syscall_item->syscall.name);
        }
        syscall_item = syscall_item->next;
    }

    /* checks in system variable */
    {
        struct finsh_sysvar *index;
        for (index = _sysvar_table_begin;
                index < _sysvar_table_end;
                FINSH_NEXT_SYSVAR(index))
        {
            if (str_is_prefix(prefix, index->name) == 0)
            {
                if (var_cnt == 0)
                {
                    rt_kprintf("--variable:\n");

                    if (*prefix != 0 && name_ptr == NULL)
                    {
                        /* set name_ptr */
                        name_ptr = index->name;

                        /* set initial length */
                        min_length = strlen(name_ptr);

                    }
                }

                var_cnt ++;

                if (*prefix != 0)
                {
                    length = str_common(name_ptr, index->name);
                    if (length < min_length)
                        min_length = length;
                }

#ifdef FINSH_USING_DESCRIPTION
                rt_kprintf("%-16s -- %s\n", index->name, index->desc);
#else
                rt_kprintf("%s\n", index->name);
#endif
            }
        }
    }

    /* checks in dynamic system variable */
    sysvar_item = global_sysvar_list;
    while (sysvar_item != NULL)
    {
        if (str_is_prefix(prefix, sysvar_item->sysvar.name) == 0)
        {
            if (var_cnt == 0)
            {
                rt_kprintf("--variable:\n");
                if (*prefix != 0 && name_ptr == NULL)
                {
                    /* set name_ptr */
                    name_ptr = sysvar_item->sysvar.name;

                    /* set initial length */
                    min_length = strlen(name_ptr);
                }
            }

            var_cnt ++;

            if (*prefix != 0)
            {
                length = str_common(name_ptr, sysvar_item->sysvar.name);
                if (length < min_length)
                    min_length = length;
            }

            rt_kprintf("[v] %s\n", sysvar_item->sysvar.name);
        }
        sysvar_item = sysvar_item->next;
    }

    /* only one matched */
    if (name_ptr != NULL)
    {
        rt_strncpy(prefix, name_ptr, min_length);
    }
}
#endif

#if defined(FINSH_USING_SYMTAB) && !defined(FINSH_USING_MSH_ONLY)
static int dummy = 0;
FINSH_VAR_EXPORT(dummy, finsh_type_int, dummy variable for finsh);
#endif

#ifdef ARCH_LOMBO

#include <debug.h>

#if defined(RT_DEBUG) && defined(DUMP_EXCP_STACK)
/**
 * dump_stack_all: Dump all thread's stack
 *
 * This function should be used with gcc option -fno-omit-frame-pointer
 */
long dump_stack_all(void)
{
	rt_list_t *obj_list[LIST_FIND_OBJ_NR];
	rt_list_t *next = (rt_list_t *)RT_NULL;
	struct rt_thread *thread;
	struct rt_object *obj;
	list_get_next_t find_arg;
	rt_ubase_t level;
	int i;

	level = rt_hw_interrupt_disable();

	list_find_init(&find_arg, RT_Object_Class_Thread, obj_list,
		sizeof(obj_list) / sizeof(obj_list[0]));

	do {
		next = list_get_next(next, &find_arg);
		for (i = 0; i < find_arg.nr_out; i++) {
			obj = rt_list_entry(obj_list[i], struct rt_object, list);

			if ((obj->type & ~RT_Object_Class_Static) != find_arg.type)
				continue;

			thread = (struct rt_thread *)obj;

			/* dump the stack info */
			dump_stack_thread(thread);
			rt_kprintf("\n");
		}
	} while (next != (rt_list_t *)RT_NULL);

	rt_hw_interrupt_enable(level);
	return 0;
}
FINSH_FUNCTION_EXPORT(dump_stack_all, dump all thread stack);
MSH_CMD_EXPORT(dump_stack_all, dump all thread stack);

#endif /* RT_DEBUG && DUMP_EXCP_STACK */

#ifdef THREAD_STATS

/**
 * __dump_thread_loading: Dump the thread loading
 * @thread: the thread handle
 * @para: a buf that contain:
 *	buf[0] is total_time (not used)
 *	buf[1] is print_buf
 *	buf[2] is print_buf_size
 *	buf[3] is print_offset
 */
void __dump_thread_loading(struct rt_thread *thread, void *para)
{
	char *buf = RT_NULL;
	rt_size_t size, off = 0; /* offset in print buf */
	float loading;

	if (thread->loading < 0.1) /* too little to dump */
		return;

	RT_ASSERT(RT_NULL != para);
	buf = (char *)((int *)para)[1];
	size = (rt_size_t)((int *)para)[2];
	off = (rt_size_t)((int *)para)[3];

	off += snprintf(buf + off, size - off, "%-16s  ", thread->name);

	loading = thread->loading;
	if (RT_CPU_DETACHED != thread->oncpu) {
		off += snprintf(buf + off, size - off, "%-4d %-9f on cpu%d  ",
			thread->current_priority, loading, thread->oncpu);
#ifdef SCHED_LOCK_DEBUG
		off += snprintf(buf + off, size - off, "%-16d %-18f ",
			thread->tmp_sched_lock_period,
			thread->sched_lock_loading);
#endif
#ifdef SPIN_LOCK_DEBUG
		off += snprintf(buf + off, size - off, "%-16d %-16f ",
			thread->tmp_spin_lock_period,
			thread->spin_lock_loading);
#endif
	} else {
		off += snprintf(buf + off, size - off, "%-4d %-18f ",
			thread->current_priority, loading);
#ifdef SCHED_LOCK_DEBUG
		off += snprintf(buf + off, size - off, "%-16d %-18f ",
			thread->tmp_sched_lock_period,
			thread->sched_lock_loading);
#endif
#ifdef SPIN_LOCK_DEBUG
		off += snprintf(buf + off, size - off, "%-16d %-16f ",
			thread->tmp_spin_lock_period,
			thread->spin_lock_loading);
#endif
	}

	/* add tail \n */
	off += snprintf(buf + off, size - off, "\n");

	/* update the print offset */
	((int *)para)[3] = off;
}

/**
 * __threads_iterate: Iterate all threads for special operation
 * @cmd: the operation type
 * @para: para for cmd
 *   THREAD_STATIST_DUMP: para is a buf
 *	buf[0] is total_time
 *	buf[1] is print_buf
 *	buf[2] is print_buf_size
 *	buf[3] is print_offset
 *   THREAD_STATIST_CALC: para is total_time
 *   THREAD_STATIST_INIT: para has no use
 */
void __threads_iterate(int cmd, void *para)
{
	rt_list_t *obj_list[LIST_FIND_OBJ_NR];
	rt_list_t *next = (rt_list_t *)RT_NULL;
	struct rt_thread *thread;
	struct rt_object *obj;
	list_get_next_t find_arg;
	int i, total = 0, temp_ms;
#ifdef SCHED_LOCK_DEBUG
	int temp_sched_lock_us;
#endif
#ifdef SPIN_LOCK_DEBUG
	int temp_spin_lock_us;
#endif

	list_find_init(&find_arg, RT_Object_Class_Thread, obj_list,
		sizeof(obj_list) / sizeof(obj_list[0]));

	do {
		next = list_get_next(next, &find_arg);
		for (i = 0; i < find_arg.nr_out; i++) {
			obj = rt_list_entry(obj_list[i], struct rt_object, list);

			if ((obj->type & ~RT_Object_Class_Static) != find_arg.type)
				continue;

			thread = (struct rt_thread *)obj;

			/* calca total running time during a period */
			temp_ms = get_thread_busy_time(thread);
#ifdef SCHED_LOCK_DEBUG
			temp_sched_lock_us = get_thread_sched_lock_time(thread);
#endif
#ifdef SPIN_LOCK_DEBUG
			temp_spin_lock_us = get_thread_spin_lock_time(thread);
#endif
			if (THREAD_STATIST_INIT == cmd) {
				thread->tmp_total_period = 0;
				thread->tmp_last_stamp = temp_ms;
#ifdef SCHED_LOCK_DEBUG
				thread->tmp_sched_lock_period = 0;
				thread->tmp_sched_lock_last = temp_sched_lock_us;
				thread->sched_lock_loading = 0;
#endif
#ifdef SPIN_LOCK_DEBUG
				thread->tmp_spin_lock_period = 0;
				thread->tmp_spin_lock_last = temp_spin_lock_us;
				thread->spin_lock_loading = 0;
#endif
			} else if (THREAD_STATIST_CALC == cmd) {
				/* get total time ms */
				total = (int)para;

				/* update tmp total running time for the period */
				thread->tmp_total_period +=
						temp_ms - thread->tmp_last_stamp;

				/* calc thread loading */
				thread->loading =
					(thread->tmp_total_period * 1.0 / total) * 100;
#ifdef SCHED_LOCK_DEBUG
				thread->tmp_sched_lock_period =
					temp_sched_lock_us - thread->tmp_sched_lock_last;
				thread->sched_lock_loading =
					thread->tmp_sched_lock_period * 0.001 /* to ms */
					* 100 / total;
#endif
#ifdef SPIN_LOCK_DEBUG
				thread->tmp_spin_lock_period =
					temp_spin_lock_us - thread->tmp_spin_lock_last;
				thread->spin_lock_loading =
					thread->tmp_spin_lock_period * 0.001 /* to ms */
					* 100 / total;
#endif

				/* update timestamp and clear tmp value */
				thread->tmp_total_period = 0;
				thread->tmp_last_stamp = get_thread_busy_time(thread);
#ifdef SCHED_LOCK_DEBUG
				/* thread->tmp_sched_lock_period = 0; need for dump */
				/* last total_sched_lock */
				thread->tmp_sched_lock_last = temp_sched_lock_us;
#endif
#ifdef SPIN_LOCK_DEBUG
				/* thread->tmp_spin_lock_period = 0; need for dump */
				/* last total_spin_lock */
				thread->tmp_spin_lock_last = temp_spin_lock_us;
#endif
			} else if (THREAD_STATIST_DUMP == cmd) {
				/* get total time ms, no use */
				/* total = ((int *)para)[0]; */

				__dump_thread_loading(thread, para);
			} else {
				printf("%s err: cmd %d invalid, thread %s\n",
					__func__, cmd, thread->name);
				break;
			}
		}
	} while (next != (rt_list_t *)RT_NULL);
}

#endif /* THREAD_STATS */

int list_fd(void);

#ifdef RT_USING_MODULE
int list_symbols(void);
int list_module(void);
#endif

/**
 * dump_all: Dump all context information
 * @in_excpt: if in exception mode, 0 - no, 1 - yes
 *
 * Include all threads' stack, timers, devices, ipc objects, etc.
 */
void dump_all(int in_excpt)
{
	rt_uint8_t old_sta = disable_async_print();

	rt_kprintf("======= dump all context start =======\n\n");

#if defined(RT_DEBUG) && defined(DUMP_EXCP_STACK)
	/* all threads stack */
	if (!in_excpt)
		dump_stack();
	dump_stack_all();
#endif

	/* thread info */
	list_thread();
	rt_kprintf("\n");

	/* ipc objects */
	list_sem();
	rt_kprintf("\n");
	list_mutex();
	rt_kprintf("\n");
	list_event();
	rt_kprintf("\n");
	list_msgqueue();
	rt_kprintf("\n");
	list_mailbox();
	rt_kprintf("\n");

#ifdef RT_USING_MODULE
	/* list_symbols(); */

	list_module();
	rt_kprintf("\n");
#endif

	list_fd();
	rt_kprintf("\n");

	list_mempool();
	rt_kprintf("\n");

	list_device();
	rt_kprintf("\n");

	list_timer();
	rt_kprintf("\n");

	rt_kprintf("======= dump all context end =======\n");

	enable_async_print(old_sta);
}
FINSH_FUNCTION_EXPORT(dump_all, dump all contexts);
MSH_CMD_EXPORT(dump_all, dump all contexts);

#endif /* ARCH_LOMBO */

#endif /* RT_USING_FINSH */

