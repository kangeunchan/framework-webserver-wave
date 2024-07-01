#include "wave.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void json_to_string(JsonObject* obj, char* output, size_t output_size) {
    char* cur = output;
    int remaining = output_size;
    int written = snprintf(cur, remaining, "{");
    cur += written;
    remaining -= written;

    for (int i = 0; i < obj->field_count; i++) {
        written = snprintf(cur, remaining, "\"%s\":\"%s\"%s", 
                        obj->fields[i].key, obj->fields[i].value,
                        (i < obj->field_count - 1) ? "," : "");
        cur += written;
        remaining -= written;
        if (remaining <= 0) break;
    }

    snprintf(cur, remaining, "}");
}

void set_json_response_from_object(HttpResponse* response, int status_code, JsonObject* json_obj) {
    response->status_code = status_code;
    json_to_string(json_obj, response->content, sizeof(response->content));
}

void send_response(int client_socket, HttpResponse* response) {
    char response_str[MAX_REQUEST_SIZE];
    char* status_text;

    switch (response->status_code) {
        case 200: status_text = "OK"; break;
        case 201: status_text = "Created"; break;
        case 204: status_text = "No Content"; break;
        case 400: status_text = "Bad Request"; break;
        case 404: status_text = "Not Found"; break;
        case 500: status_text = "Internal Server Error"; break;
        default: status_text = "Unknown";
    }

    snprintf(response_str, sizeof(response_str),
            "HTTP/1.1 %d %s\r\n"
            "Content-Type: application/json\r\n"
            "Content-Length: %lu\r\n"
            "\r\n"
            "%s",
            response->status_code, status_text,
            strlen(response->content),
            response->content);

    write(client_socket, response_str, strlen(response_str));
}