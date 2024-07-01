#include "wave.h"

void start_server(WebFramework* framework, int port) {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        char buffer[MAX_REQUEST_SIZE] = {0};
        read(new_socket, buffer, MAX_REQUEST_SIZE);

        HttpRequest request;
        HttpResponse response;

        parse_request(buffer, &request);
        handle_request(framework, &request, &response);
        send_response(new_socket, &response);

        close(new_socket);
    }
}

void send_response(int client_socket, HttpResponse* response) {
    char buffer[MAX_REQUEST_SIZE];
    snprintf(buffer, sizeof(buffer), "HTTP/1.1 %d OK\r\nContent-Length: %lu\r\n\r\n%s",
            response->status_code, strlen(response->content), response->content);
    send(client_socket, buffer, strlen(buffer), 0);
}
