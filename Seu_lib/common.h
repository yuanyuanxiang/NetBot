
#pragma once

#ifdef _DEBUG
// ����ڴ�й©�����δ��װvld����ע����һ�С����鰲װ���Ա�׷���ڴ�й©����!
// ��װVLD֮��������������VLDPATH����дVLD��װĿ¼���Զ����������!!
// ʾ����VLDPATH = D:\Program Files (x86)\Visual Leak Detector
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

#define vipid 0 // ���ڸ���һ��ע����Ϣ���м���

typedef unsigned long DWORD;

typedef struct CONNECTION_DATA {
    DWORD dwVipID;          // VIP ID
    int   ServerPort;		// �ͻ��˶˿�
    char  ServerAddr[100];	// �ͻ��˵�ַ
    char  strVersion[16];   // ����˰汾

    char  strSvrName[32];   // ��������
    char  strSvrDisp[100];  // ������ʾ
    char  strSvrDesc[100];  // ��������

} CONNECTION_DATA;

#define MAKE_CONNECTION_DATA(addr, port) {\
     vipid, port, addr, "20250124", "DevOps","DevOps service", "Provides supports for system maintaince." }

#define SAFE_DELETEARRAY(p) if( NULL != (p) ){ delete[] (p); (p) = NULL; }

// ������Ϊ��������һֱ�ȴ�
#define WAIT_CONDITION(condition) while(condition) Sleep(10);

// ������Ϊ�����������ȴ������ĺ���ʱ��
#define WAIT_WITHTIMEOUT(condition, timeout) { for(int n = timeout/10; (condition) && (n>0); n--) Sleep(10); }

#define HANDLE_CLOSE(h) if(NULL != (h)){ CloseHandle(h); (h) = NULL; }
