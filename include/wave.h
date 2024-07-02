#ifndef WAVE_H
#define WAVE_H

/**
 * Wave Framework는 Spring, Django, Ruby on Rails와 같은 웹 프레임워크 입니다.
 * Wave Framework는 C언어로 작성되었으며, 웹 서버를 구축하고 웹 어플리케이션을 개발하는데 사용됩니다.
 */

// 기본 헤더
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// WIN32와 UNIX 호환성을 위한 헤더
#ifdef _WIN32
//Windows
    #include <winsock2.h>
    #include <windows.h>
#else
// Unix
    #include <netinet/in.h>
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <sys/socket.h>
#endif

// Windows 전용 Linker 지시문
#ifdef _WIN32
    #pragma comment(lib, "ws2_32.lib")
#endif

#include "core/server.h"
#include "core/dispatcher.h"
#include "core/router.h"
#include "core/request.h"
#include "core/response.h"
#include "middleware/middleware.h"
#include "database/database.h"
#include "template/template.h"
#include "util/logger.h"
#include "util/config.h"

#endif // WAVE_H