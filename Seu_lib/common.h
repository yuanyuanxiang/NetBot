
#pragma once

#ifdef _DEBUG
#include <stdio.h>
#define Mprintf(format, ...) printf(format, ##__VA_ARGS__)
#else
#define Mprintf(format, ...)
#endif

#define vipid 0 // ���ڸ���һ��ע����Ϣ���м���

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
