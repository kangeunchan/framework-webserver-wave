#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <pthread.h>

typedef enum {
    CREATE,
    ADD,
    DESTORY
} ThreadPoolOptions;

typedef struct {
    void* (*function)(void*);
    void* arg;
} Task;

typedef struct {
    pthread_t* threads;
    Task* task_queue;
    int thread_count;
    int queue_size;
    int front;
    int rear;
    pthread_mutex_t queue_mutex;
    pthread_cond_t queue_not_empty;
    pthread_cond_t queue_not_full;
    int stop;
} ThreadPool;


ThreadPool* threadPool(ThreadPoolOptions option, int thread, ThreadPool* pool, void* (*task)(void*), void* arg);
ThreadPool* create(int thread);
void add(ThreadPool* pool, void* (*task)(void*), void* arg);



#endif // THREADPOOL_H