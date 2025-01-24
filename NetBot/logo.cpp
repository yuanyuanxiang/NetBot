// logo.cpp : implementation file
//

#include "stdafx.h"
#include "NetBot.h"
#include "logo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// logo dialog


logo::logo(CWnd* pParent /*=NULL*/)
    : CDialog(logo::IDD, pParent)
{
    //{{AFX_DATA_INIT(logo)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}


void logo::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(logo)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(logo, CDialog)
    //{{AFX_MSG_MAP(logo)
    // NOTE: the ClassWizard will add message map macros here
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// logo message handlers
