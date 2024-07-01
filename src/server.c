#include "wave.h"
#include <sys/socket.h>
#include <arpa/inet.h>

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
            continue;
        }

        char buffer[MAX_REQUEST_SIZE] = {0};
        ssize_t valread = read(new_socket, buffer, MAX_REQUEST_SIZE - 1);
        if (valread < 0) {
            perror("read");
            close(new_socket);
            continue;
        }

        HttpRequest request;
        HttpResponse response;

        parse_request(buffer, &request);
        handle_request(framework, &request, &response);
        send_response(new_socket, &response);

        close(new_socket);
    }

    close(server_fd);
}