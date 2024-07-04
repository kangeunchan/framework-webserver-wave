// http_server.h
#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include "router.h"

typedef struct HttpServer HttpServer;

HttpServer* http_server_create(void);
int http_server_start(HttpServer* server, Router* router);
void http_server_destroy(HttpServer* server);

#endif