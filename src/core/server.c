#include "server.h"
#include "request.h"
#include "response.h"
#include "router.h"
#include "../utils/logger.h"
#include "../utils/thread_pool.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>

#define MAX_CONNECTIONS 1000
#define THREAD_POOL_SIZE 20

static int server_fd;
static ThreadPool* thread_pool;
static volatile sig_atomic_t keep_running = 1;

void signal_handler(int signum) {
    keep_running = 0;
}

void* handle_client(void* arg) {
    int client_socket = *(int*)arg;
    free(arg);

    char buffer[1024] = {0};
    ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
    
    if (bytes_read > 0) {
        Request* request = parse_request(buffer);
        Response* response = handle_request(request);

        char* response_str = serialize_response(response);
        write(client_socket, response_str, strlen(response_str));

        free(response_str);
        free_request(request);
        free_response(response);
    }

    close(client_socket);
    return NULL;
}

void serverStart(int port) {
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    signal(SIGINT, signal_handler);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        log_error("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        log_error("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, MAX_CONNECTIONS) < 0) {
        log_error("Listen failed");
        exit(EXIT_FAILURE);
    }

    thread_pool = createThreadPool(THREAD_POOL_SIZE);
    if (!thread_pool) {
        log_error("Thread pool creation failed");
        exit(EXIT_FAILURE);
    }

    log_info("Server is running on port %d", port);

    while (keep_running) {
        int new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        if (new_socket < 0) {
            if (keep_running) {
                log_error("Accept failed");
            }
            continue;
        }

        int* client_sock = malloc(sizeof(int));
        *client_sock = new_socket;
        addTaskToThreadPool(thread_pool, handle_client, client_sock);
    }

    log_info("Server shutting down...");
    destroyThreadPool(thread_pool);
    close(server_fd);
}