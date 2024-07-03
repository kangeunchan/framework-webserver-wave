#ifndef THREAD_POOL_H
#define THREAD_POOL_H

typedef struct ThreadPool ThreadPool;

ThreadPool* createThreadPool(int num_threads);
void addTaskToThreadPool(ThreadPool* pool, void* (*task)(void*), void* arg);
void destroyThreadPool(ThreadPool* pool);

#endif // THREAD_POOL_H