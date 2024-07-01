#include "wave.h"

void parse_request(char* request_str, HttpRequest* request) {
    char  method_str[8];
    sscanf(request_str, "%s %s", method_str, request->path);

    if (strcmp(method_str, "GET") == 0) {
        request->method = GET;
    } else if (strcmp(method_str, "POST") == 0) {
        request->method = POST;
    } else if (strcmp(method_str, "PUT") == 0) {
        request->method = PUT;
    } else if (strcmp(method_str, "DELETE") == 0) {
        request->method = DELETE;
    } else if (strcmp(method_str, "PATCH") == 0) {
        request->method = PATCH;
    }

    char* body_start = strstr(request_str, "\r\n\r\n");
    if (body_start) {
        strcpy(request->body, body_start + 4);
    }
}

const char* method_to_string(HttpMethod method) {
    switch (method) {
        case GET: return "GET";
        case POST: return "POST";
        case PUT: return "PUT";
        case DELETE: return "DELETE";
        case PATCH: return "PATCH";
        default: return "UNKNOWN";
    }
}
