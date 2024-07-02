#ifndef WAVE_H
#define WAVE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #include <winsock2.h>
    #include <windows.h>
#else
    #include <netinet/in.h>
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <sys/socket.h>
#endif
#ifdef _WIN32
    #pragma warning(disable: 4819)
    #pragma comment(lib, "ws2_32.lib")
#endif

#include "../src/core/server.h"
#include "../src/core/dispatcher.h"
#include "../src/core/router.h"
#include "../src/core/request.h"
#include "../src/core/response.h"
#include "../src/middleware/middleware.h"
#include "../src/database/database.h"
#include "../src/template/template.h"
#include "../src/util/logger.h"
#include "../src/util/config.h"

#endif 