#ifndef RESPONSE_H
#define RESPONSE_H

typedef enum {
    Plain,
    Json,
} ResponseType;

typedef struct {
    int status;
    char* body;
} Response;

Response* create_response(int status, const char* body);
char* serialize_response(const Response* response);
void free_response(Response* response);

#endif // RESPONSE_H