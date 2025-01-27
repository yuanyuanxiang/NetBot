#include "Command.h"
#include "common.h"

#ifndef TRACE
#include <stdio.h>
#define TRACE printf
#endif

//开启TCP保活机制
BOOL TurnonKeepAlive(SOCKET s, UINT nKeepAliveSec)
{
    if (nKeepAliveSec < 1)
        return TRUE;

    BOOL bSetKeepAlive = TRUE;
    if (::setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, (const char*)&bSetKeepAlive, sizeof(BOOL)) != 0)
        return FALSE;

    DWORD dwBytes;
    struct tcp_keepalive Settings = { 0 };
    struct tcp_keepalive Retvals = { 0 };
    Settings.onoff = 1;
    Settings.keepaliveinterval = 15000; //回应超时间隔，如果出现超时，Windows将重新发送检测包，直到5次全部失败。
    Settings.keepalivetime = nKeepAliveSec * 1000; //开始首次KeepAlive探测前的TCP空闭时间

    if (::WSAIoctl(s, SIO_KEEPALIVE_VALS, &Settings, sizeof(Settings), &Retvals, sizeof(Retvals), &dwBytes, NULL, NULL) != 0) {
        return FALSE;
    }
    return TRUE;
}

//发送数据
BOOL SendData(SOCKET s, const char *data, int len)
{
    const char* pData = data;
    int iHasSend = 0;
    int iLeftSend = len;
    int iRet = 0;

    if (len <= 0)
        return TRUE;

    while (1) {
        iRet = send(s, pData, iLeftSend, 0);
        if (iRet == 0) {
            Mprintf("Socket 连接关闭.\n");
            return FALSE;
        }
        if (iRet == SOCKET_ERROR) {
			int err = WSAGetLastError();
			if (err == WSAETIMEDOUT) {
                Mprintf("Socket 发送超时.\n");
			} else {
                Mprintf("Socket 发送失败: %d.\n", err);
			}
            return FALSE;
        }

        iHasSend += iRet;
        pData += iRet;
        iLeftSend -= iRet;
        if (iHasSend >= len)
            break;
    }

    return TRUE;
}

//接收数据
BOOL RecvData(SOCKET s, char *data, int len)
{
    char * pData = data;
    int iHasRecv = 0;
    int iLeftRecv = len;
    int iRet = 0;

    if (len <= 0)
        return TRUE;

    while (1) {
        iRet = recv(s, pData, iLeftRecv, 0);
		if (iRet == 0) {
			Mprintf("Socket 连接关闭.\n");
			return FALSE;
		}
		if (iRet == SOCKET_ERROR) {
			int err = WSAGetLastError();
			if (err == WSAETIMEDOUT) {
				Mprintf("Socket 接收超时.\n");
			}
			else {
				Mprintf("Socket 接收失败: %d.\n", err);
			}
			return FALSE;
		}

        iHasRecv += iRet;
        pData += iRet;
        iLeftRecv -= iRet;
        if (iHasRecv >= len)
            break;
    }

    return TRUE;
}

BOOL SendMsg(SOCKET s, SafeBuffer pBuf, LPMsgHead lpMsgHead)
{
    //发送消息头
    if (!SendData(s, (char*)lpMsgHead, sizeof(MsgHead)))
        return FALSE;

    //查看数据长度
    if (lpMsgHead->dwSize <= 0)
        return TRUE;

    //发送数据
    if (!SendData(s, pBuf.SafeCheck(lpMsgHead->dwSize), lpMsgHead->dwSize))
        return FALSE;

    return TRUE;
}

BOOL RecvMsg(SOCKET s, SafeBuffer pBuf, LPMsgHead lpMsgHead)
{
    //接收消息头
    if (!RecvData(s, (char*)lpMsgHead, sizeof(MsgHead)) || !lpMsgHead->IsValid())
        return FALSE;

    if (lpMsgHead->dwSize <= 0)
        return TRUE;

    if (!RecvData(s, pBuf.SafeCheck(lpMsgHead->dwSize), lpMsgHead->dwSize))
        return FALSE;

    return TRUE;
}
