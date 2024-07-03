#include "thread_pool.h"
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/logger.h"

#define MAX_QUEUE_SIZE 10000

typedef struct {
    void* (*function)(void*);
    void* arg;
} Task;

struct ThreadPool {
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
};

static void* worker(void* arg) {
    ThreadPool* pool = (ThreadPool*)arg;

    while (1) {
        pthread_mutex_lock(&(pool->queue_mutex));

        while (pool->queue_size == 0 && !pool->stop) {
            pthread_cond_wait(&(pool->queue_not_empty), &(pool->queue_mutex));
        }

        if (pool->stop) {
            pthread_mutex_unlock(&(pool->queue_mutex));
            pthread_exit(NULL);
        }

        Task task = pool->task_queue[pool->front];
        pool->front = (pool->front + 1) % MAX_QUEUE_SIZE;
        pool->queue_size--;

        pthread_cond_signal(&(pool->queue_not_full));
        pthread_mutex_unlock(&(pool->queue_mutex));

        (*(task.function))(task.arg);
    }

    return NULL;
}

ThreadPool* createThreadPool(int num_threads) {
    ThreadPool* pool = (ThreadPool*)malloc(sizeof(ThreadPool));
    if (!pool) {
        log_error("Failed to allocate memory for thread pool");
        return NULL;
    }

    pool->threads = (pthread_t*)malloc(num_threads * sizeof(pthread_t));
    pool->task_queue = (Task*)malloc(MAX_QUEUE_SIZE * sizeof(Task));

    if (!pool->threads || !pool->task_queue) {
        log_error("Failed to allocate memory for threads or task queue");
        free(pool->threads);
        free(pool->task_queue);
        free(pool);
        return NULL;
    }

    pool->thread_count = num_threads;
    pool->queue_size = 0;
    pool->front = 0;
    pool->rear = 0;
    pool->stop = 0;

    if (pthread_mutex_init(&(pool->queue_mutex), NULL) != 0 ||
        pthread_cond_init(&(pool->queue_not_empty), NULL) != 0 ||
        pthread_cond_init(&(pool->queue_not_full), NULL) != 0) {
        log_error("Failed to initialize mutex or condition variables");
        free(pool->threads);
        free(pool->task_queue);
        free(pool);
        return NULL;
    }

    for (int i = 0; i < num_threads; i++) {
        if (pthread_create(&(pool->threads[i]), NULL, worker, pool) != 0) {
            log_error("Failed to create thread");
            destroyThreadPool(pool);
            return NULL;
        }
    }

    return pool;
}

void addTaskToThreadPool(ThreadPool* pool, void* (*task)(void*), void* arg) {
    pthread_mutex_lock(&(pool->queue_mutex));

    while (pool->queue_size == MAX_QUEUE_SIZE) {
        pthread_cond_wait(&(pool->queue_not_full), &(pool->queue_mutex));
    }

    Task new_task = {task, arg};
    pool->task_queue[pool->rear] = new_task;
    pool->rear = (pool->rear + 1) % MAX_QUEUE_SIZE;
    pool->queue_size++;

    pthread_cond_signal(&(pool->queue_not_empty));
    pthread_mutex_unlock(&(pool->queue_mutex));
}

void destroyThreadPool(ThreadPool* pool) {
    if (pool == NULL) {
        return;
    }

    pthread_mutex_lock(&(pool->queue_mutex));
    pool->stop = 1;
    pthread_cond_broadcast(&(pool->queue_not_empty));
    pthread_mutex_unlock(&(pool->queue_mutex));

    for (int i = 0; i < pool->thread_count; i++) {
        pthread_join(pool->threads[i], NULL);
    }

    pthread_mutex_destroy(&(pool->queue_mutex));
    pthread_cond_destroy(&(pool->queue_not_empty));
    pthread_cond_destroy(&(pool->queue_not_full));

    free(pool->threads);
    free(pool->task_queue);
    free(pool);
}