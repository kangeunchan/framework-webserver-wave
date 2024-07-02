#ifndef WAVE_REQUEST_H
#define WAVE_REQUEST_H

typedef struct WaveRequest WaveRequest;

WaveRequest* requestCreate();
void requestParse(WaveRequest *request, const char *rawRequest);
char* requestGetHeader(WaveRequest *request, const char *key);
char* requestGetParam(WaveRequest *request, const char *key);
void requestFree(WaveRequest *request);

#endif // WAVE_REQUEST_H