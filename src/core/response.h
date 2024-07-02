#ifndef WAVE_RESPONSE_H
#define WAVE_RESPONSE_H

typedef struct WaveResponse WaveResponse;

WaveResponse* responseCreate();
void responseSetStatus(WaveResponse *response, int statusCode);
void responseSetHeader(WaveResponse *response, const char *key, const char *value);
void responseSetBody(WaveResponse *response, const char *body);
char* responseToString(WaveResponse *response);
void responseFree(WaveResponse *response);

#endif // WAVE_RESPONSE_H