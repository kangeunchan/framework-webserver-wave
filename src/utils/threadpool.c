#include "../include/wave.h"

#define MAX_QUEUE_SIZE 10000
#define TRUE 1

ThreadPool* threadPool(ThreadPoolOptions option, int thread, ThreadPool* pool, void* (*task)(void*), void* arg) {
    switch (option)
    {
        case CREATE: {
            create(thread);
            break;
        }            
        case ADD: {
            add(pool, task, arg);
            break;
        }

        case DESTORY: {
            
            break;
        }

        default: {
            log(ERROR, "THREAD OPTION ERROR");
            break;
        }
    }
}

static void* routine(void* arg) {
    ThreadPool* pool = (ThreadPool*)arg;

    while(TRUE) {
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

ThreadPool* create(int thread) {
    ThreadPool* pool = (ThreadPool*)malloc(sizeof(ThreadPool));
    if(pool == NULL) {
        log(ERROR, "THREAD POOL CREATE ERROR : <POOL IS NULL>");
        return NULL;
    }

    pool->threads = (pthread_t*)malloc(thread * sizeof(pthread_t));
    pool->task_queue = (Task*)malloc(MAX_QUEUE_SIZE * sizeof(Task));

    if(pool->threads == NULL || pool->task_queue == NULL) {
        free(pool->threads);
        free(pool->task_queue);
        free(pool);
        log(ERROR, "THREAD POOL CREATE ERROR : <THREADS IS NULL OR TASK QUEUE IS NULL>");
        return NULL;
    }

    pool->thread_count = thread;
    pool->queue_size = 0;
    pool->front = 0;
    pool->rear = 0;
    pool->stop = 0;

    if (pthread_mutex_init(&(pool->queue_mutex), NULL) != 0 ||
        pthread_cond_init(&(pool->queue_not_empty), NULL) != 0 ||
        pthread_cond_init(&(pool->queue_not_full), NULL) != 0) {
        log(ERROR, "THREAD POOL CREATE ERROR : <MUTEX OR CONDITION VARIABLE INIT ERROR>");
        free(pool->threads);
        free(pool->task_queue);
        free(pool);
        return NULL;
    }

    for (int i = 0; i < thread; i++) {
        if (pthread_create(&(pool->threads[i]), NULL, routine, (void*)pool) != 0) {
            log(ERROR, "THREAD POOL CREATE ERROR : <THREAD CREATE ERROR>");
            free(pool->threads);
            free(pool->task_queue);
            free(pool);
            return NULL;
        }
    }
}

void add(ThreadPool* pool, void* (*task)(void*), void* arg) {
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

void destroy(ThreadPool* pool) {
    if (pool == NULL) {
        log(ERROR, "THREAD POOL DESTROY ERROR : <POOL IS NULL>");
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