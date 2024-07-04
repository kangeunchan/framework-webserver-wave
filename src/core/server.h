#ifndef SERVER_H
#define SERVER_H

#include "../utils/threadpool.h"

typedef struct {
    int PORT;
    int SERVER_FILE_DESCRIPTOR;
    ThreadPool* THREAD_POOL;
} ServerInfo;

typedef enum {
    START,
    STOP
} ServerOptions;

void server(ServerInfo infos, ServerOptions options);
void _server_start(ServerInfo infos);
void _server_stop();


#endif // SERVER_H