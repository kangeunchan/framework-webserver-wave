#ifndef REQUEST_H
#define REQUEST_H

typedef enum {
    GET,
    POST,
    PUT,
    DELETE,
    PATCH,
    HEAD,
    OPTIONS,
    UNKNOWN
} HttpMethod;

typedef struct {
    HttpMethod method;
    char* path;
    char* body;
} Request;

Request* parseRequest(char* request);
void freeRequest(Request* request);

#endif // REQUEST_H