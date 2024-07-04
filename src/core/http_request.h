#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

typedef struct HttpRequest HttpRequest;

HttpRequest* http_request_parse(const char* raw_request);
const char* http_request_get_method(HttpRequest* request);
const char* http_request_get_path(HttpRequest* request);
void http_request_destroy(HttpRequest* request);

#endif
