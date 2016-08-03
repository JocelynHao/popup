
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类
#include <afxdhtml.h>        // HTML Dialogs


#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持

#define TIME_IDENTIFIER_LOGON		8001	// 登录定时器
#define TIME_IDENTIFIER_AUTH		8002	// 验证码定时器
#define TIME_IDENTIFIER_HEARTBEAT	8003	// 心跳定时器
#define TIME_IDENTIFIER_CLEAR		8004	// 清理日志定时器
#define TIME_IDENTIFIER_CLOSE_DLG	8005	// 关闭助销弹窗程序定时器



#define CODE_EXP_TIME				100									// 单位秒

// 升级模块
#define	EXE_UPDATE_FILE				_T(".\\Update.exe")
#define	INI_UPDATE_FILE				_T(".\\Update.ini")

#define INI_SECTION_SHELL			_T("Update")
#define INI_KEY_STATUS				_T("Status")

// 助销模块
#define	INI_APP_FILE				_T(".\\IncrementSystem.ini")
#define	INI_APP_FILE1				_T(".\\SelfDefine.ini")
#define	INI_SECTION_OFFICE			_T("OfficeNetwork")
#define	INI_SECTION_CHECKED		    _T("Checked")
#define	INI_SECTION_INTERNAL		_T("InternalNetwork")
#define	INI_SECTION_INTERNET		_T("InternetNetwork")
#define	INI_SECTION_SYSTEM1			_T("IfChecked")
#define	INI_SECTION_SYSTEM			_T("System")
#define	INI_SECTION_VERSION			_T("Version")
#define INI_SECTION_CHECK           _T("Checked")
#define	INI_KEY_WORKING_TIME		_T("WorkingTime")
#define	INI_KEY_TITLE				_T("Title")
#define	INI_KEY_LOGON				_T("Logon")
#define	INI_KEY_AUTHENTICATION		_T("Authentication")
#define	INI_KEY_MAIN1				_T("Http1")
#define	INI_KEY_MAIN2				_T("Http2")
#define	INI_KEY_MAIN3 				_T("Http3")
#define	INI_KEY_PHONE_TYPE			_T("QueryPhoneType")
#define	INI_KEY_PHONE				_T("QueryPhone")
#define	INI_KEY_POPUP				_T("QueryPopup")
#define	INI_KEY_HEARTBEAT			_T("Heartbeat")
#define	INI_KEY_RESET_PWD			_T("ResetPassword")
#define	INI_KEY_LOGOFF				_T("Logoff")
#define	INI_KEY_WIDTH				_T("Width")
#define	INI_KEY_HEIGHT				_T("Height")
#define	INI_KEY_TIMEOUT				_T("Timeout")
#define	INI_KEY_POLLING_INTERVAL	_T("PollingInterval")
#define	INI_KEY_QUERY_ELAPSE		_T("QueryElapse") 
#define	INI_KEY_BEEP				_T("Beep")
#define	INI_KEY_SHOW_CLOSE			_T("ShowClose")
#define	INI_KEY_SINGLE_APP			_T("SingleApp")
#define	INI_KEY_SAVE_LOG			_T("SaveLog")
#define	INI_KEY_SAVE_LOG			_T("SaveLog")
#define	INI_KEY_RESERVE_DAYS		_T("ReserveDays")

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


