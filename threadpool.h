#ifndef _THREADPOOL_H
#define _THREADPOOL_H



#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


#define THREAD_NUM 4

typedef struct zxc_task_s {
    void (*func)(void *);
    void *arg;
    struct zxc_task_s *next;
} zxc_task_t;

typedef struct {
    pthread_mutex_t lock;
    pthread_cond_t cond;
    pthread_t *threads;
    zxc_task_t *head;
    int thread_count;
    int queue_size;
    int shutdown;
    int started;
} zxc_threadpool_t;

typedef enum {
    zxc_tp_invalid   = -1,
    zxc_tp_lock_fail = -2,
    zxc_tp_already_shutdown  = -3,
    zxc_tp_cond_broadcast    = -4,
    zxc_tp_thread_fail       = -5,
    
} zxc_threadpool_error_t;

zxc_threadpool_t *threadpool_init(int thread_num);

int threadpool_add(zxc_threadpool_t *pool, void (*func)(void *), void *arg);

int threadpool_destroy(zxc_threadpool_t *pool, int gracegul);



#endif
