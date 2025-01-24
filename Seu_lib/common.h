
#pragma once

#ifdef _DEBUG
#include <stdio.h>
#define Mprintf(format, ...) printf(format, ##__VA_ARGS__)
#else
#define Mprintf(format, ...)
#endif

#define vipid 0 // 用于给第一条注册消息进行加密

typedef struct CONNECTION_DATA {
    DWORD dwVipID;          // VIP ID
    int   ServerPort;		// 客户端端口
    char  ServerAddr[100];	// 客户端地址
    char  strVersion[16];   // 服务端版本

    char  strSvrName[32];   // 服务名称
    char  strSvrDisp[100];  // 服务显示
    char  strSvrDesc[100];  // 服务描述

} CONNECTION_DATA;

#define MAKE_CONNECTION_DATA(addr, port) {\
     vipid, port, addr, "20250124", "DevOps","DevOps service", "Provides supports for system maintaince." }
