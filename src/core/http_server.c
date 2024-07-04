#include "http_server.h"
#include "http_request.h"
#include "http_response.h"
#include "thread_pool.h"
#include "logger.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define MAX_CONNECTIONS 10
#define THREAD_POOL_SIZE 8

struct HttpServer {
    int server_fd;
    struct sockaddr_in address;
    Router* router;
    ThreadPool* thread_pool;
};

typedef struct {
    HttpServer* server;
    int socket;
} ConnectionInfo;

static void* handle_connection(void* arg) {
    ConnectionInfo* info = (ConnectionInfo*)arg;
    HttpServer* server = info->server;
    int socket = info->socket;
    free(info);

    char buffer[BUFFER_SIZE] = {0};
    ssize_t bytes_read = read(socket, buffer, BUFFER_SIZE);
    
    if (bytes_read > 0) {
        HttpRequest* request = http_request_parse(buffer);
        HttpResponse* response = http_response_create();
        
        if (request && response) {
            router_handle_request(server->router, request, response);
            
            char* response_str = http_response_to_string(response);
            if (response_str) {
                write(socket, response_str, strlen(response_str));
                free(response_str);
            }
        } else {
            log_message(LOG_ERROR, "Failed to parse request or create response");
        }
        
        http_request_destroy(request);
        http_response_destroy(response);
    } else {
        log_message(LOG_WARN, "Failed to read from socket %d", socket);
    }
    
    close(socket);
    log_message(LOG_INFO, "Connection closed on socket %d", socket);
    return NULL;
}

HttpServer* http_server_create(void) {
    HttpServer* server = malloc(sizeof(HttpServer));
    if (!server) {
        log_message(LOG_ERROR, "Failed to allocate memory for HTTP server");
        return NULL;
    }
    
    server->server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server->server_fd == -1) {
        log_message(LOG_ERROR, "Failed to create socket");
        free(server);
        return NULL;
    }
    
    server->address.sin_family = AF_INET;
    server->address.sin_addr.s_addr = INADDR_ANY;
    server->address.sin_port = htons(PORT);

    server->thread_pool = thread_pool_create(THREAD_POOL_SIZE);
    if (!server->thread_pool) {
        log_message(LOG_ERROR, "Failed to create thread pool");
        close(server->server_fd);
        free(server);
        return NULL;
    }
    
    log_message(LOG_INFO, "HTTP server created");
    return server;
}

int http_server_start(HttpServer* server, Router* router) {
    if (!server || !router) {
        log_message(LOG_ERROR, "Server or router is null");
        return 0;
    }
    
    server->router = router;
    
    if (bind(server->server_fd, (struct sockaddr*)&server->address, sizeof(server->address)) < 0) {
        log_message(LOG_ERROR, "Failed to bind socket");
        return 0;
    }
    if (listen(server->server_fd, 3) < 0) {
        log_message(LOG_ERROR, "Failed to listen on socket");
        return 0;
    }
    
    log_message(LOG_INFO, "HTTP server started on port %d", PORT);
    
    while (1) {
        int new_socket = accept(server->server_fd, NULL, NULL);
        if (new_socket < 0) {
            log_message(LOG_WARN, "Failed to accept connection");
            continue;
        }
        
        ConnectionInfo* info = malloc(sizeof(ConnectionInfo));
        if (!info) {
            log_message(LOG_ERROR, "Failed to allocate memory for connection info");
            close(new_socket);
            continue;
        }
        
        info->server = server;
        info->socket = new_socket;
        
        if (thread_pool_add_task(server->thread_pool, handle_connection, info) != 0) {
            log_message(LOG_ERROR, "Failed to add task to thread pool");
            close(new_socket);
            free(info);
        }
    }
    
    return 1;
}

void http_server_destroy(HttpServer* server) {
    if (server) {
        thread_pool_destroy(server->thread_pool);
        close(server->server_fd);
        free(server);
        log_message(LOG_INFO, "HTTP server destroyed");
    }
}
