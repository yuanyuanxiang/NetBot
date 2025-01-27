// CmdEdit.cpp : implementation file
//

#include "../stdafx.h"
#include "CmdEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCmdEdit

CCmdEdit::CCmdEdit()
{
    m_clrFore = RGB(255, 255, 255);//字白色
    m_clrBack = RGB(0, 0, 0);//背景黑色
    m_brush.CreateSolidBrush(m_clrBack);
}

CCmdEdit::~CCmdEdit()
{
}


BEGIN_MESSAGE_MAP(CCmdEdit, CEdit)
    ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCmdEdit message handlers

HBRUSH CCmdEdit::CtlColor(CDC* pDC, UINT nCtlColor)
{
    // TODO: Change any attributes of the DC here
    pDC->SetTextColor(m_clrFore);
    pDC->SetBkColor(m_clrBack);
    return (HBRUSH)m_brush.GetSafeHandle();
}

void CCmdEdit::ClearEdit()
{
    this->SetWindowText("");
}

void CCmdEdit::AddText(LPCTSTR szText)
{
    CString strContext;
    GetWindowText(strContext);

    strContext += szText;
    this->SetWindowText(strContext);

    LineScroll(GetLineCount());//滚动到位置
    SetFocus();
    SetSel(strContext.GetLength(), -1);//光标位置？
}