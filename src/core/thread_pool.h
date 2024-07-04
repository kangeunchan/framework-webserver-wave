#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <pthread.h>

typedef struct ThreadPool ThreadPool;

ThreadPool* thread_pool_create(int thread_count);
void thread_pool_destroy(ThreadPool* pool);
int thread_pool_add_task(ThreadPool* pool, void* (*task)(void*), void* arg);

#endif
