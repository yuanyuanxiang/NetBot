#include "Command.h"
#include "common.h"

#ifndef TRACE
#include <stdio.h>
#define TRACE printf
#endif

//����TCP�������
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
    Settings.keepaliveinterval = 15000; //��Ӧ��ʱ�����������ֳ�ʱ��Windows�����·��ͼ�����ֱ��5��ȫ��ʧ�ܡ�
    Settings.keepalivetime = nKeepAliveSec * 1000; //��ʼ�״�KeepAlive̽��ǰ��TCP�ձ�ʱ��

    if (::WSAIoctl(s, SIO_KEEPALIVE_VALS, &Settings, sizeof(Settings), &Retvals, sizeof(Retvals), &dwBytes, NULL, NULL) != 0) {
        return FALSE;
    }
    return TRUE;
}

//��������
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
            Mprintf("Socket ���ӹر�.\n");
            return FALSE;
        }
        if (iRet == SOCKET_ERROR) {
			int err = WSAGetLastError();
			if (err == WSAETIMEDOUT) {
                Mprintf("Socket ���ͳ�ʱ.\n");
			} else {
                Mprintf("Socket ����ʧ��: %d.\n", err);
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

//��������
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
			Mprintf("Socket ���ӹر�.\n");
			return FALSE;
		}
		if (iRet == SOCKET_ERROR) {
			int err = WSAGetLastError();
			if (err == WSAETIMEDOUT) {
				Mprintf("Socket ���ճ�ʱ.\n");
			}
			else {
				Mprintf("Socket ����ʧ��: %d.\n", err);
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
    //������Ϣͷ
    if (!SendData(s, (char*)lpMsgHead, sizeof(MsgHead)))
        return FALSE;

    //�鿴���ݳ���
    if (lpMsgHead->dwSize <= 0)
        return TRUE;

    //��������
    if (!SendData(s, pBuf.SafeCheck(lpMsgHead->dwSize), lpMsgHead->dwSize))
        return FALSE;

    return TRUE;
}

BOOL RecvMsg(SOCKET s, SafeBuffer pBuf, LPMsgHead lpMsgHead)
{
    //������Ϣͷ
    if (!RecvData(s, (char*)lpMsgHead, sizeof(MsgHead)) || !lpMsgHead->IsValid())
        return FALSE;

    if (lpMsgHead->dwSize <= 0)
        return TRUE;

    if (!RecvData(s, pBuf.SafeCheck(lpMsgHead->dwSize), lpMsgHead->dwSize))
        return FALSE;

    return TRUE;
}
