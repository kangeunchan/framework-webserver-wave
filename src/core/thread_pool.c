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

struct ThreadPool {
    pthread_mutex_t lock;
    pthread_cond_t notify;
    pthread_t* threads;
    Task* task_queue;
    int thread_count;
    int shutdown;
};

static void* thread_pool_worker(void* arg) {
    ThreadPool* pool = (ThreadPool*)arg;
    while (1) {
        pthread_mutex_lock(&(pool->lock));
        log_message(LOG_DEBUG, "Thread %ld acquired lock", pthread_self());

        while (pool->task_queue == NULL && !pool->shutdown) {
            log_message(LOG_DEBUG, "Thread %ld waiting for task", pthread_self());
            pthread_cond_wait(&(pool->notify), &(pool->lock));
        }

        if (pool->shutdown) {
            pthread_mutex_unlock(&(pool->lock));
            log_message(LOG_INFO, "Thread %ld shutting down", pthread_self());
            pthread_exit(NULL);
        }

        Task* task = pool->task_queue;
        pool->task_queue = task->next;
        pthread_mutex_unlock(&(pool->lock));
        log_message(LOG_DEBUG, "Thread %ld released lock", pthread_self());

        log_message(LOG_INFO, "Thread %ld started task with argument %p", pthread_self(), task->argument);
        (*(task->function))(task->argument);
        log_message(LOG_INFO, "Thread %ld finished task with argument %p", pthread_self(), task->argument);

        free(task);
        log_message(LOG_DEBUG, "Thread %ld freed task memory", pthread_self());
    }
    pthread_exit(NULL);
}

ThreadPool* thread_pool_create(int thread_count) {
    log_message(LOG_INFO, "Creating thread pool with %d threads", thread_count);

    ThreadPool* pool = malloc(sizeof(ThreadPool));
    if (pool == NULL) {
        log_message(LOG_ERROR, "Failed to allocate memory for thread pool");
        return NULL;
    }

    pool->thread_count = thread_count;
    pool->shutdown = 0;
    pool->task_queue = NULL;

    pool->threads = malloc(sizeof(pthread_t) * thread_count);
    if (pool->threads == NULL) {
        log_message(LOG_ERROR, "Failed to allocate memory for thread pool threads");
        free(pool);
        return NULL;
    }

    pthread_mutex_init(&(pool->lock), NULL);
    pthread_cond_init(&(pool->notify), NULL);

    for (int i = 0; i < thread_count; i++) {
        if (pthread_create(&(pool->threads[i]), NULL, thread_pool_worker, (void*)pool) != 0) {
            log_message(LOG_ERROR, "Failed to create thread %d", i);
            thread_pool_destroy(pool);
            return NULL;
        }
        log_message(LOG_INFO, "Thread %d created", i);
    }

    log_message(LOG_INFO, "Thread pool created successfully with %d threads", thread_count);
    return pool;
}

void thread_pool_destroy(ThreadPool* pool) {
    if (pool == NULL) return;

    log_message(LOG_INFO, "Destroying thread pool");

    pthread_mutex_lock(&(pool->lock));
    pool->shutdown = 1;
    pthread_cond_broadcast(&(pool->notify));
    pthread_mutex_unlock(&(pool->lock));

    for (int i = 0; i < pool->thread_count; i++) {
        pthread_join(pool->threads[i], NULL);
        log_message(LOG_INFO, "Thread %d joined", i);
    }

    free(pool->threads);

    while (pool->task_queue != NULL) {
        Task* task = pool->task_queue;
        pool->task_queue = task->next;
        free(task);
    }

    pthread_mutex_destroy(&(pool->lock));
    pthread_cond_destroy(&(pool->notify));
    free(pool);

    log_message(LOG_INFO, "Thread pool destroyed successfully");
}

int thread_pool_add_task(ThreadPool* pool, void* (*task)(void*), void* arg) {
    if (pool == NULL || task == NULL) {
        log_message(LOG_WARN, "Attempt to add task to null pool or null task");
        return -1;
    }

    Task* new_task = malloc(sizeof(Task));
    if (new_task == NULL) {
        log_message(LOG_ERROR, "Failed to allocate memory for new task");
        return -1;
    }

    new_task->function = task;
    new_task->argument = arg;
    new_task->next = NULL;

    pthread_mutex_lock(&(pool->lock));
    log_message(LOG_DEBUG, "Adding task to pool with argument %p", arg);

    Task* last_task = pool->task_queue;
    if (last_task == NULL) {
        pool->task_queue = new_task;
    } else {
        while (last_task->next != NULL) {
            last_task = last_task->next;
        }
        last_task->next = new_task;
    }

    pthread_cond_signal(&(pool->notify));
    pthread_mutex_unlock(&(pool->lock));

    log_message(LOG_INFO, "Task added to pool with argument %p", arg);
    return 0;
}
