
#pragma once

#ifdef _DEBUG
// 检测内存泄漏，如果未安装vld，需注释这一行。建议安装，以便追踪内存泄漏问题!
// 安装VLD之后，新增环境变量VLDPATH，填写VLD安装目录可自动解决该问题!!
// 示例：VLDPATH = D:\Program Files (x86)\Visual Leak Detector
#include <vld.h>

#ifdef USE_TRACE
#define Mprintf(format, ...) TRACE(format, ##__VA_ARGS__)
#else
#include <stdio.h>
#define Mprintf(format, ...) printf(format, ##__VA_ARGS__)
#endif
#else
#define Mprintf(format, ...)
#endif

#define vipid 0 // 用于给第一条注册消息进行加密

typedef unsigned long DWORD;

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

#define SAFE_DELETEARRAY(p) if( NULL != (p) ){ delete[] (p); (p) = NULL; }

// 在条件为真的情况下一直等待
#define WAIT_CONDITION(condition) while(condition) Sleep(10);

// 在条件为真的情况下最多等待给定的毫秒时间
#define WAIT_WITHTIMEOUT(condition, timeout) { for(int n = timeout/10; (condition) && (n>0); n--) Sleep(10); }

#define HANDLE_CLOSE(h) if(NULL != (h)){ CloseHandle(h); (h) = NULL; }
