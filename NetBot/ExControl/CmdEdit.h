#if !defined(AFX_CMDEDIT_H__00571AE7_F546_4F95_A466_8EAB2078C801__INCLUDED_)
#define AFX_CMDEDIT_H__00571AE7_F546_4F95_A466_8EAB2078C801__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CmdEdit.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CCmdEdit window

class CCmdEdit : public CEdit
{
// Construction
public:
    CCmdEdit();

// Attributes
public:
    COLORREF m_clrFore;//前景色
    COLORREF m_clrBack;//背景色
    CBrush m_brush;//背景刷子
// Operations
public:
    void ClearEdit();
    void AddText(LPCTSTR szText);

public:
    CFont m_font;
    CRect m_rc;
    virtual ~CCmdEdit();

protected:

    afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);

    DECLARE_MESSAGE_MAP()
};

#endif
