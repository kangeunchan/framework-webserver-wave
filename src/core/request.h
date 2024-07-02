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

Request* parse_request(const char* raw_request);
void free_request(Request* request);

#endif // REQUEST_H