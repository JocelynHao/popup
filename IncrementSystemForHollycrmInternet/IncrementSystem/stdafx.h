
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ


#include <afxdisp.h>        // MFC �Զ�����
#include <afxdhtml.h>        // HTML Dialogs


#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��

#define TIME_IDENTIFIER_LOGON		8001	// ��¼��ʱ��
#define TIME_IDENTIFIER_AUTH		8002	// ��֤�붨ʱ��
#define TIME_IDENTIFIER_HEARTBEAT	8003	// ������ʱ��
#define TIME_IDENTIFIER_CLEAR		8004	// ������־��ʱ��
#define TIME_IDENTIFIER_CLOSE_DLG	8005	// �ر�������������ʱ��



#define CODE_EXP_TIME				100									// ��λ��

// ����ģ��
#define	EXE_UPDATE_FILE				_T(".\\Update.exe")
#define	INI_UPDATE_FILE				_T(".\\Update.ini")

#define INI_SECTION_SHELL			_T("Update")
#define INI_KEY_STATUS				_T("Status")

// ����ģ��
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


