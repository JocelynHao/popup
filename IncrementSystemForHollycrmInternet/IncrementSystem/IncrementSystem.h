
// IncrementSystem.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CIncrementSystemApp:
// 有关此类的实现，请参阅 IncrementSystem.cpp
//

class CIncrementSystemApp : public CWinApp
{
public:
	CIncrementSystemApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()

public:
	CString GetNetworkCardMac(void);

public:
	//BOOL	ReadSysParam1(void);
	BOOL	ReadSysParam(void);
	void	MakeSureQuestionMarkExist(CString &strURL);
	BOOL	ProcessIsRunning(CString strExeName);
	int		EnumFiles(LPCTSTR lpszPath, CString sSpec, CStringArray &arrFiles, BOOL bIncludeChildPath = FALSE);
	BOOL	ClearOverdueLog(void);
	BOOL	ClearOverdueFile(LPCTSTR lpszPath, CTime tBaseDate);
	CString GetFileDate(LPCTSTR lpszFile);
	CString GetModuleVersionInfo(LPCTSTR pszModule, LPCTSTR pszItem);
	CString GetFileLastWriteTime(LPCTSTR pszFile);

// 属性
public:
	CString m_strWorkingTime;
	CString m_strUser;
	CString m_strTitle;
	CString m_strLogonURL;
	CString m_strAuthURL;
	CString m_strMainURL1;
	CString m_strPhoneTypeURL;
	CString m_strPhoneURL;
	CString m_strPopupURL;
	CString m_strHeartbeatURL;
	CString m_strResetPasswordURL;
	CString m_strLogoffURL;
	CString m_strSessionId;
	CString m_strLogPath;
	CString m_strMAC;
	CString m_strVerConfigFile;
	int		m_nWidth;
	int		m_nHeight;
	DWORD	m_dwTimeout;
	DWORD	m_dwPollingInterval;
	DWORD	m_dwQueryElapse;
	BOOL	m_bBeep;
	BOOL	m_bShowClose;
	BOOL	m_bSingleApp;
	BOOL	m_bSaveLog;
	BOOL	m_bSupportCopy;
	BOOL	m_bSupportNoCapture;
	BOOL    m_bSupportTimeSetting;
	//BOOL    m_bSettingTimeQuery;
	int		m_nReserveDays;
	int     m_bSupportTop;
	int     SliderPos;
};

extern CIncrementSystemApp theApp;