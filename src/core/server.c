#include "../include/wave.h"

static volatile int runningFlag = 1;

#define MAX_CONNECTIONS 1000
#define THREAD_POOL_SIZE 20
#define BUFFER_SIZE 1024

void signalHandler(int signal) {
    runningFlag = 0;
}

void* clientHandler(void* arg) {
    log(INFO, "clientHandler called");
    if (arg == NULL) {
        log(ERROR, "NULL ARGUMENT IN CLIENT HANDLER");
        return NULL;
    }

    int socket = *(int *)arg;
    free(arg);
    log(INFO, "Client socket: %d");

    char buffer[BUFFER_SIZE] = { 0 };
    ssize_t bytes_read = read(socket, buffer, BUFFER_SIZE - 1);
    log(INFO, "Bytes read: %zd");
    log(INFO, "Buffer content: %s");

    if (bytes_read < 0) {
        log(ERROR, "ERROR READING FROM SOCKET");
        close(socket);
        return NULL;
    }

    if (bytes_read == 0) {
        log(INFO, "CONNECTION CLOSED");
        close(socket);
        return NULL;
    }

    log(INFO, "REQUEST RECEIVED: %s");

    // Parse the request
    Request* req = parseRequest(buffer);
    if (req == NULL) {
        log(ERROR, "Failed to parse request");
        close(socket);
        return NULL;
    }
    log(INFO, "Request parsed successfully");

    // Create a response
    Response* res = createResponse();
    if (res == NULL) {
        log(ERROR, "Failed to create response");
        freeRequest(req);
        close(socket);
        return NULL;
    }

    log(INFO, "Routing request");
    // Route the request
    dispatch(app->router, req, res);

    // Generate response string
    const char* response_str = generateResponseString(res);
    if (response_str == NULL) {
        log(ERROR, "Failed to generate response string");
        freeRequest(req);
        destroyResponse(res);
        close(socket);
        return NULL;
    }

    log(INFO, "Sending response");
    ssize_t bytes_written = write(socket, response_str, strlen(response_str));
    log(INFO, "Bytes written: %zd");
    log(INFO, "RESPONSE SENT");

    // Clean up
    freeRequest(req);
    destroyResponse(res);
    close(socket);
    log(INFO, "clientHandler completed");
    return NULL;
}

void server(ServerInfo infos, ServerOptions options) {
    switch (options) {
        case START:
            log(INFO, "STARTING SERVER");
            _server_start(infos);
            break;
        case STOP:
            log(INFO, "STOPPING SERVER");
            _server_stop();
            break;
        default:
            log(ERROR, "INVALID SERVER OPTION");
            break;
    }
}

void _server_start(ServerInfo infos) {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        log(ERROR, "SOCKET CREATION ERROR");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(infos.PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        log(ERROR, "BIND FAILED");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        log(ERROR, "LISTEN FAILED");
        exit(EXIT_FAILURE);
    }

    log(INFO, "SERVER STARTED");

    while (runningFlag) {
        log(INFO, "WAITING FOR NEW CONNECTION");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            log(ERROR, "ACCEPT FAILED");
            continue;
        }

        log(INFO, "NEW CONNECTION ACCEPTED");
        int* client_sock = malloc(sizeof(int));
        if (client_sock == NULL) {
            log(ERROR, "MEMORY ALLOCATION FAILED FOR CLIENT SOCKET");
            close(new_socket);
            continue;
        }
        *client_sock = new_socket;
        threadPool(ADD, 0, infos.THREAD_POOL, clientHandler, client
