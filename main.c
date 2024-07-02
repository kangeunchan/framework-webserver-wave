#include "include/wave.h"
#include <stdio.h>
#include <string.h>

// 라우트 핸들러 함수들
void homeHandler(WaveRequest *request, WaveResponse *response) {
    responseSetBody(response, "<h1>Welcome to Wave Framework!</h1>");
}

void aboutHandler(WaveRequest *request, WaveResponse *response) {
    responseSetBody(response, "<h1>About Wave Framework</h1><p>A simple web framework in C.</p>");
}

void templateHandler(WaveRequest *request, WaveResponse *response) {
    WaveTemplateContext *context = templateContextCreate();
    templateContextSet(context, "title", "Template Example");
    templateContextSet(context, "content", "This is a rendered template.");

    const char *templateString = "<h1>{{title}}</h1><p>{{content}}</p>";
    char *renderedHtml = templateRender(templateString, context);

    responseSetBody(response, renderedHtml);

    free(renderedHtml);
    templateContextFree(context);
}

// 미들웨어 함수
void loggingMiddleware(WaveRequest *request, WaveResponse *response) {
    char logMessage[256];
    snprintf(logMessage, sizeof(logMessage), "Request received: %s %s", requestGetMethod(request), requestGetPath(request));
    logInfo(logMessage);
}

int main() {
    // 설정 파일 로드
    configLoad("config.ini");
    int port = configGetInt("server_port");
    if (port == 0) port = 8080;  // 기본 포트

    // 로거 초기화
    logInit("server.log");

    // 데이터베이스 연결 (더미 함수)
    dbConnect("mysql://localhost/wavedb");

    // 라우터 초기화 및 라우트 추가
    routerInit();
    routerAddRoute("/", "GET", homeHandler);
    routerAddRoute("/about", "GET", aboutHandler);
    routerAddRoute("/template", "GET", templateHandler);

    // 미들웨어 추가
    middlewareAdd(loggingMiddleware);

    // 서버 초기화 및 시작
    serverInit(port);
    
    printf("Wave Framework server starting on port %d\n", port);
    logInfo("Server starting");

    serverStart();  // 이 함수는 서버가 중지될 때까지 반환되지 않습니다.

    // 서버 정리 (실제로는 이 코드에 도달하지 않습니다)
    logInfo("Server shutting down");
    dbClose();

    return 0;
}