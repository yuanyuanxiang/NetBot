#if !defined(AFX_AUTOLOCK_H)
#define AFX_AUTOLOCK_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Seu_lib/common.h"

class CAutoLock
{
public:
    CAutoLock(CRITICAL_SECTION& cs)
        : m_rCs(cs)
    {
        EnterCriticalSection(&m_rCs);
    }

    ~CAutoLock()
    {
        LeaveCriticalSection(&m_rCs);
    }

private:
    CRITICAL_SECTION& m_rCs;
};

// �ڽ����߳�ʱ����һ�����󲢴����������߳��˳�ʱ�ر��߳̾��
class CAutoRelease
{
public:
	CAutoRelease(HANDLE& threadHandle)
		: m_handle(threadHandle)
	{
        Mprintf(">>> Enter thread[%d]: %p\n", GetThreadId(m_handle), m_handle);
	}

	~CAutoRelease()
	{
        Mprintf(">>> Leave thread[%d]: %p\n", GetThreadId(m_handle), m_handle);
		HANDLE_CLOSE(m_handle);
	}

private:
	HANDLE& m_handle;
};

#endif
