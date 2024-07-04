#ifndef RESPONSE_H
#define RESPONSE_H

typedef struct {
    int status_code;
    char* headers;
    char* body;
} Response;

Response* createResponse();
void setStatusCode(Response* response, int status_code);
void addHeader(Response* response, const char* key, const char* value);
void setBody(Response* response, const char* body);
void destroyResponse(Response* response);

#endif // RESPONSE_H