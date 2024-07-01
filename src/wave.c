#include "wave.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // read, write, close 함수를 사용하기 위해 필요
#include <netinet/in.h>

// 웹 프레임워크 초기화
void init_framework(WebFramework* framework) {
    framework->route_count = 0;
}

// 라우트 추가
void add_route(WebFramework* framework, HttpMethod method, const char* path, RouteHandler handler) {
    if (framework->route_count < MAX_ROUTE_COUNT) {
        Route* route = &framework->routes[framework->route_count++];
        route->method = method;
        strncpy(route->path, path, sizeof(route->path));
        route->handler = handler;
    }
}

// 요청 처리
void handle_request(WebFramework* framework, HttpRequest* request, HttpResponse* response) {
    for (int i = 0; i < framework->route_count; i++) {
        Route* route = &framework->routes[i];
        if (route->method == request->method && strcmp(route->path, request->path) == 0) {
            route->handler(request, response);
            return;
        }
    }
    response->status_code = 404;
    strcpy(response->content, "Not Found");
}

// 서버 시작
void start_server(WebFramework* framework, int port) {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // 소켓 생성
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 주소 설정
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // 소켓 바인드
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // 리슨
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server started on port %d\n", port);

    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        char buffer[MAX_REQUEST_SIZE] = {0};
        read(new_socket, buffer, MAX_REQUEST_SIZE);

        // 요청 파싱 (간단한 예제)
        HttpRequest request;
        if (strncmp(buffer, "GET", 3) == 0) {
            request.method = GET;
        } else if (strncmp(buffer, "POST", 4) == 0) {
            request.method = POST;
        }
        sscanf(buffer, "%*s %s", request.path);

        HttpResponse response;
        handle_request(framework, &request, &response);

        char response_buffer[1024];
        sprintf(response_buffer, "HTTP/1.1 %d OK\r\nContent-Length: %lu\r\n\r\n%s", response.status_code, strlen(response.content), response.content);
        write(new_socket, response_buffer, strlen(response_buffer));
        close(new_socket);
    }
}
