#include "thread_pool.h"
#include "logger.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

typedef struct Task {
    void* (*function)(void*);
    void* argument;
    struct Task* next;
} Task;

typedef struct ThreadPool {
    pthread_mutex_t lock;
    pthread_cond_t notify;
    pthread_t* threads;
    Task* task_queue;
    int thread_count;
    int shutdown;
} ThreadPool;

static void* thread_pool_worker(void* arg) {
    ThreadPool* pool = (ThreadPool*)arg;
    Task* task;

    while (1) {
        pthread_mutex_lock(&(pool->lock));

        while (pool->task_queue == NULL && !pool->shutdown) {
            pthread_cond_wait(&(pool->notify), &(pool->lock));
        }

        if (pool->shutdown && pool->task_queue == NULL) {
            pthread_mutex_unlock(&(pool->lock));
            pthread_exit(NULL);
        }

        task = pool->task_queue;
        pool->task_queue = task->next;

        pthread_mutex_unlock(&(pool->lock));

        task->function(task->argument);
        free(task);
    }
}

ThreadPool* thread_pool_create(int thread_count) {
    ThreadPool* pool = malloc(sizeof(ThreadPool));
    if (!pool) {
        log_message(LOG_ERROR, "Failed to allocate memory for thread pool");
        return NULL;
    }

    pool->threads = malloc(sizeof(pthread_t) * thread_count);
    if (!pool->threads) {
        free(pool);
        log_message(LOG_ERROR, "Failed to allocate memory for threads");
        return NULL;
    }

    pool->thread_count = thread_count;
    pool->shutdown = 0;
    pool->task_queue = NULL;

    pthread_mutex_init(&(pool->lock), NULL);
    pthread_cond_init(&(pool->notify), NULL);

    for (int i = 0; i < thread_count; ++i) {
        if (pthread_create(&(pool->threads[i]), NULL, thread_pool_worker, pool) != 0) {
            thread_pool_destroy(pool);
            return NULL;
        }
    }

    log_message(LOG_INFO, "Thread pool created with %d threads", thread_count);
    return pool;
}

int thread_pool_add_task(ThreadPool* pool, void* (*function)(void*), void* argument) {
    if (!pool || !function) {
        log_message(LOG_ERROR, "Invalid pool or function pointer");
        return;
    }

    Task* new_task = malloc(sizeof(Task));
    if (!new_task) {
        log_message(LOG_ERROR, "Failed to allocate memory for new task");
        return;
    }

    new_task->function = function;
    new_task->argument = argument;
    new_task->next = NULL;

    pthread_mutex_lock(&(pool->lock));

    if (pool->task_queue == NULL) {
        pool->task_queue = new_task;
    } else {
        Task* last_task = pool->task_queue;
        while (last_task->next) {
            last_task = last_task->next;
        }
        last_task->next = new_task;
    }

    pthread_cond_signal(&(pool->notify));
    pthread_mutex_unlock(&(pool->lock));
}

void thread_pool_destroy(ThreadPool* pool) {
    if (!pool) return;

    pthread_mutex_lock(&(pool->lock));
    pool->shutdown = 1;
    pthread_cond_broadcast(&(pool->notify));
    pthread_mutex_unlock(&(pool->lock));

    for (int i = 0; i < pool->thread_count; ++i) {
        pthread_join(pool->threads[i], NULL);
    }

    while (pool->task_queue) {
        Task* task = pool->task_queue;
        pool->task_queue = task->next;
        free(task);
    }

    pthread_mutex_destroy(&(pool->lock));
    pthread_cond_destroy(&(pool->notify));
    free(pool->threads);
    free(pool);
    log_message(LOG_INFO, "Thread pool destroyed");
}
