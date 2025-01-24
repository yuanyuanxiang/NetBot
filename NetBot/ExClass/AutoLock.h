#if !defined(AFX_AUTOLOCK_H)
#define AFX_AUTOLOCK_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

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

#endif
