#include "wave.h"
#include "server.h"
#include "dispatcher.h"

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

static int serverSocket;
static int serverPort;

void serverInit(int port) {
    serverPort = port;
}

void serverStart() {
    struct sockaddr_in serverAddr;

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(serverPort);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Binding failed");
        exit(EXIT_FAILURE);
    }

    if (listen(serverSocket, MAX_CLIENTS) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", serverPort);

    while (1) {
        int clientSocket;
        struct sockaddr_in clientAddr;
        long clientAddrLen = sizeof(clientAddr);

        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
        if (clientSocket < 0) {
            perror("Accept failed");
            continue;
        }

        char buffer[BUFFER_SIZE];
        long bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesRead > 0) {
            buffer[bytesRead] = '\0';
            WaveRequest* request = requestCreate();
            WaveResponse* response = responseCreate();

            requestParse(request, buffer);
            serverDispatch(request, response);

            char* responseStr = responseToString(response);
            send(clientSocket, responseStr, strlen(responseStr), 0);

            free(responseStr);
            requestFree(request);
            responseFree(response);
        }

        close(clientSocket);
    }
}

void serverStop() {
    close(serverSocket);
    printf("Server stopped\n");
}