#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

typedef struct HttpResponse HttpResponse;

HttpResponse* http_response_create();
void http_response_set_status(HttpResponse* response, int status_code, const char* status_message);
void http_response_add_header(HttpResponse* response, const char* name, const char* value);
void http_response_set_body(HttpResponse* response, const char* body);
char* http_response_to_string(HttpResponse* response);
void http_response_destroy(HttpResponse* response);

#endif