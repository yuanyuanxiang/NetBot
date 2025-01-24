#if !defined(AFX_DDOSDLG_H__5A153243_4289_4193_8C5B_5D68DA4128D5__INCLUDED_)
#define AFX_DDOSDLG_H__5A153243_4289_4193_8C5B_5D68DA4128D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DDosDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDDosDlg dialog

class CDDosDlg : public CDialog
{
// Construction
public:
    CDDosDlg(CWnd* pParent = NULL);   // standard constructor

    enum { IDD = IDD_DDOS_DLG };
    CListBox	m_LogList;
    CString	m_IP1;
    CString	m_IP2;
    CString	m_IP3;
    CString	m_IP4;
    int		m_Port1;
    int		m_Port2;
    int		m_Port3;
    int		m_Port4;
    int		m_Type1;
    int		m_Type2;
    int		m_Type3;
    int		m_Type4;
    int		m_Num1;
    int		m_Num2;
    int		m_Num3;
    int		m_Num4;
    int		m_Thread1;
    int		m_Thread2;
    int		m_Thread3;
    int		m_Thread4;
    CString	m_IP;
    int		m_Type;
    int		m_Port;
    int		m_Thread;
    BOOL	m_bUseSelect;
    BOOL	m_bAutoAttack;
    int		m_Num;
    CString	m_CCUrl;
    UINT	m_CCThread;
    UINT	m_CCPort;
    UINT	m_CCNum;
    UINT	m_CCParamX;
    UINT	m_CCParamY;
    BOOL	m_bCCUseSel;

    virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
    virtual BOOL OnInitDialog();
    afx_msg void OnBtnAttack1();
    afx_msg void OnBtnStop1();
    afx_msg void OnBtnAttack2();
    afx_msg void OnBtnStop2();
    afx_msg void OnBtnAttack3();
    afx_msg void OnBtnStop3();
    afx_msg void OnBtnAttack4();
    afx_msg void OnBtnStop4();
    afx_msg void OnBtnAttack();
    afx_msg void OnBtnStop();
    afx_msg void OnBtnCcAttack();
    afx_msg void OnBtnCcStop();
    DECLARE_MESSAGE_MAP()
};

#endif
