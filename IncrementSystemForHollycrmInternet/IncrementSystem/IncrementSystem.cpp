
// IncrementSystem.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "IncrementSystem.h"
#include "IncrementSystemDlg.h"

#include "LogonDlg.h"

#include <Nb30.h>
#pragma   comment(lib, "Netapi32.lib")

#include <Tlhelp32.h>
#pragma comment(lib, "Kernel32.lib")

#pragma comment(lib, "Version.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CIncrementSystemApp

BEGIN_MESSAGE_MAP(CIncrementSystemApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CIncrementSystemApp 构造
#include <Dbghelp.h>
CIncrementSystemApp::CIncrementSystemApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
	m_strWorkingTime = _T("");
	m_strTitle = _T("");
	m_strLogonURL = _T("");
	m_strAuthURL = _T("");
	m_strMainURL1 = _T("");
	m_strPhoneTypeURL = _T("");
	m_strPhoneURL = _T("");
	m_strPopupURL = _T("");
	m_strHeartbeatURL = _T("");
	m_strResetPasswordURL = _T("");
	m_strLogoffURL = _T("");
	m_strUser = _T("");
	m_strSessionId = _T("");
	m_strLogPath = _T("");
	m_strMAC = _T("");
	m_strVerConfigFile = _T("");
	m_nReserveDays = 0;
	m_bSupportCopy =FALSE;
	CFileFind finder;

}


// 唯一的一个 CIncrementSystemApp 对象

CIncrementSystemApp theApp;


// CIncrementSystemApp 初始化

BOOL CIncrementSystemApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("BONC"));
	
	// 升级Update模块
	CString		 strAppDataPath = _T("");
	CString		 strUpdateFile = _T("");

	SHGetSpecialFolderPath(NULL,strAppDataPath.GetBuffer(MAX_PATH),CSIDL_APPDATA,TRUE);
	strAppDataPath.ReleaseBuffer();

	if (strAppDataPath.Right(1) == _T("\\"))
	{
		strUpdateFile = strAppDataPath + _T("IncrementSystem\\Update\\Update.exe");
		m_strVerConfigFile = strAppDataPath + _T("IncrementSystem\\IncrementSystem.ver");
	}
	else
	{
		strUpdateFile = strAppDataPath + _T("\\IncrementSystem\\Update\\Update.exe");
		m_strVerConfigFile = strAppDataPath + _T("\\IncrementSystem\\IncrementSystem.ver");
	}

	if(PathFileExists(strUpdateFile))
	{
		MoveFileEx(strUpdateFile,_T(".\\Update.exe"),MOVEFILE_COPY_ALLOWED | MOVEFILE_REPLACE_EXISTING);
	}

	if(PathFileExists(EXE_UPDATE_FILE))
	{
		int		nStatus = theApp.GetProfileInt(INI_SECTION_SHELL, INI_KEY_STATUS,1);

		if (nStatus == 1)
		{
			SHELLEXECUTEINFO sei = {0};

			sei.cbSize = sizeof (SHELLEXECUTEINFO);
			sei.lpVerb = _T("Open");
			sei.lpFile = EXE_UPDATE_FILE;
			sei.nShow = SW_SHOW;
			sei.hInstApp = AfxGetInstanceHandle();

			if (ShellExecuteEx (&sei))
			{
				// 确保应用程序退出
				return FALSE;
			}
		}

		theApp.WriteProfileInt(INI_SECTION_SHELL, INI_KEY_STATUS,1);
	}

	if (!ReadSysParam()) 
	{
		MessageBox(NULL,_T("应用程序启动失败！精细化助销平台尚未正确配置。"),m_strTitle, MB_OK + MB_ICONEXCLAMATION);
		return FALSE;
	}


	if (m_bSingleApp)
	{
		HANDLE hMutex = ::CreateMutex(NULL, TRUE, m_pszAppName); 
		if ( ::GetLastError() == ERROR_ALREADY_EXISTS )  
		{
			MessageBox(NULL,_T("应用程序启动失败！已经有一个应用程序正在运行。"),m_strTitle, MB_OK + MB_ICONEXCLAMATION);
			return FALSE;
		}
	}
	
	if (m_bSaveLog) _tmkdir(m_strLogPath);

	ClearOverdueLog();

	CLogonDlg dlgLogon;

	if (dlgLogon.DoModal() != IDOK) return FALSE;

	m_strUser = dlgLogon.m_strUser;

	CIncrementSystemDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}


BOOL CIncrementSystemApp::ReadSysParam(void)
{
	TCHAR szFilePath[MAX_PATH + 1] = {0};
	DWORD dwSize = 0;

	GetPrivateProfileString(INI_SECTION_SYSTEM,INI_KEY_TITLE,_T("精细化助销平台"),m_strTitle.GetBuffer(MAX_PATH),MAX_PATH,INI_APP_FILE);
	m_strTitle.ReleaseBuffer();

	GetPrivateProfileString(INI_SECTION_SYSTEM,INI_KEY_WORKING_TIME,_T("8：00-18：30"),m_strWorkingTime.GetBuffer(MAX_PATH),MAX_PATH,INI_APP_FILE);
	m_strWorkingTime.ReleaseBuffer();
	m_bBeep = GetPrivateProfileInt(INI_SECTION_SYSTEM,INI_KEY_BEEP,1,INI_APP_FILE);
	m_bShowClose = GetPrivateProfileInt(INI_SECTION_SYSTEM,INI_KEY_SHOW_CLOSE,1,INI_APP_FILE);
	m_bSingleApp = GetPrivateProfileInt(INI_SECTION_SYSTEM,INI_KEY_SINGLE_APP,1,INI_APP_FILE);
	m_bSaveLog = GetPrivateProfileInt(INI_SECTION_SYSTEM,INI_KEY_SAVE_LOG,0,INI_APP_FILE);
	m_nReserveDays = GetPrivateProfileInt(INI_SECTION_SYSTEM,INI_KEY_RESERVE_DAYS,30,INI_APP_FILE);
	
	m_nWidth = GetPrivateProfileInt(INI_SECTION_SYSTEM,INI_KEY_WIDTH,500,INI_APP_FILE);
	if (m_nWidth < 1) m_nWidth = 500;

	m_nHeight = GetPrivateProfileInt(INI_SECTION_SYSTEM,INI_KEY_HEIGHT,350,INI_APP_FILE);
	if (m_nHeight < 1) m_nHeight = 350;

	m_dwTimeout = GetPrivateProfileInt(INI_SECTION_SYSTEM,INI_KEY_TIMEOUT,10,INI_APP_FILE);
	if (m_dwTimeout < 2) m_dwTimeout = 10;

	m_dwPollingInterval = GetPrivateProfileInt(INI_SECTION_SYSTEM,INI_KEY_POLLING_INTERVAL,30,INI_APP_FILE);
	if (m_dwPollingInterval < 2) m_dwPollingInterval = 30;

	m_dwQueryElapse = GetPrivateProfileInt(INI_SECTION_SYSTEM,INI_KEY_QUERY_ELAPSE,30,INI_APP_FILE);
	if (m_dwQueryElapse < 2) m_dwQueryElapse = 10;

	dwSize = GetModuleFileName(NULL, szFilePath, MAX_PATH);
	if (dwSize < 3) return FALSE;

	(_tcsrchr(szFilePath, _T('\\')))[1] = 0; //删除文件名，只获得路径

	m_strLogPath.Format(_T("%sLog"),szFilePath);

	_tmkdir(m_strLogPath);

	m_strMAC = GetNetworkCardMac();

	return TRUE;
}

void CIncrementSystemApp::MakeSureQuestionMarkExist(CString &strURL)
{
	if (strURL.Right(1) != _T("?")) strURL += _T("?");
}

BOOL CIncrementSystemApp::ProcessIsRunning(CString strExeName)
{
	PROCESSENTRY32	pe32 = {0};
	HANDLE			hSnapshot = INVALID_HANDLE_VALUE;
	BOOL			bIsRunning = FALSE;

	pe32.dwSize =sizeof(pe32);

	hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if (hSnapshot == INVALID_HANDLE_VALUE) return FALSE;

	bIsRunning = Process32First(hSnapshot,&pe32);
	while (bIsRunning)
	{
		TRACE(_T("%s\n"),pe32.szExeFile);
		if (strExeName.CompareNoCase(pe32.szExeFile) == 0)
		{
			break;
		}

		bIsRunning = ::Process32Next(hSnapshot,&pe32);
	}

	CloseHandle(hSnapshot);

	return bIsRunning;
}

int CIncrementSystemApp::EnumFiles(LPCTSTR lpszPath, CString sSpec, CStringArray &arrFiles, BOOL bIncludeChildPath)
{
	CFileFind finder;
	CString sPath(lpszPath);
	CString sFile = _T("");
	CString sFileSpec = _T("");

	if (sPath.Right(1) != _T("\\")) sPath += "\\";

	sFile.Format(_T("%s*.*"),sPath);

	// start working for files
	BOOL bWorking = finder.FindFile(sFile);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		// skip . and .. files; otherwise, we'd
		// recur infinitely!
		if (finder.IsDots()) continue;

		sFile.Format(_T("%s%s"),sPath,finder.GetFileName());
		// if it's a directory and include child path, recursively search it
		if (finder.IsDirectory())
		{
			if (bIncludeChildPath)
				EnumFiles(sFile,sSpec,arrFiles,bIncludeChildPath);
		}
		else
		{
			// 过滤掉不符合条件的文件
			sFileSpec.Format(_T("%s%s"),sPath,sSpec);
			if (!PathMatchSpec(sFile,sFileSpec)) continue;

			arrFiles.Add(sFile);
		}
	}

	finder.Close();

	return (int)arrFiles.GetSize();
}

BOOL CIncrementSystemApp::ClearOverdueLog(void)
{
	CTime   dtHisDate;
	CTime   dtNowDate = CTime::GetCurrentTime();
	CTimeSpan ts(m_nReserveDays,0,0,0);

	dtHisDate = dtNowDate - ts;

	return ClearOverdueFile(m_strLogPath,dtHisDate);
}

BOOL CIncrementSystemApp::ClearOverdueFile(LPCTSTR lpszPath, CTime tBaseDate)
{
	int			 nNum = 0;
	CString		 strSpec = _T("");
	CString		 strFile = _T("");
	CString		 strFileDate = _T("");
	CStringArray arrFiles;
	COleDateTime odtTime;
	SYSTEMTIME   sysTime = {0};

	strSpec.Format(_T("%s????????.txt"),m_pszAppName);
	nNum = EnumFiles(lpszPath,strSpec,arrFiles);

	for (int i = 0; i < nNum; i ++)
	{
		strFile = arrFiles.GetAt(i);
		strFileDate = GetFileDate(strFile);

		if (odtTime.ParseDateTime(strFileDate,VAR_DATEVALUEONLY))
		{
			odtTime.GetAsSystemTime(sysTime);
			if (CTime(sysTime) < tBaseDate)
			{
				// 过期，删除...
				DeleteFile(strFile);
			}
		}
	}

	return TRUE;
}

CString CIncrementSystemApp::GetFileDate(LPCTSTR lpszFile)
{
	CString strDate = _T("");
	CString strFileName = lpszFile;

	strFileName = strFileName.Right(12);
	strDate = strFileName.Left(4) + _T("-") + strFileName.Mid(4,2) + _T("-") + strFileName.Mid(6,2);

	return strDate;
}

/**********************************************************
* 功能:
*		获得模块的版本信息
* 输入参数:
*		pszModule:[in]模块;
*		pszItem:[in]版本的特定项,例如:OriginalFilename或FileDescription.
* 返回值:
*		模块的版本信息.
**********************************************************/
CString CIncrementSystemApp::GetModuleVersionInfo(LPCTSTR pszModule, LPCTSTR pszItem)
{
	struct LANGANDCODEPAGE 
	{
		WORD wLanguage;
		WORD wCodePage;
	} *lpTranslate = NULL;
	CString sVersionInfo = _T("");
	CString sForamt = _T("");
	DWORD	dwSize = 0;
	BYTE*	lpData = NULL;
	TCHAR*	lpInfo = NULL;
	TCHAR	szSubBlock[MAX_PATH] = {0};
	HRESULT hR = 0;

	dwSize = GetFileVersionInfoSize((LPTSTR)pszModule,NULL);
	if (dwSize == 0) return _T("");

	lpData = new BYTE[dwSize];
	if (lpData == NULL) return _T("");

	if (GetFileVersionInfo((LPTSTR)pszModule,NULL,dwSize,lpData))
	{
		// Read the list of languages and code pages.
		if (VerQueryValue(lpData,_T("\\VarFileInfo\\Translation"),(LPVOID*)&lpTranslate,(PUINT)&dwSize))
		{
			sForamt = _T("\\StringFileInfo\\%04x%04x\\%s");
			for(UINT i = 0;i < (dwSize/sizeof(struct LANGANDCODEPAGE));i ++)
			{
				hR = _stprintf_s(szSubBlock,MAX_PATH,sForamt,
					lpTranslate[i].wLanguage,lpTranslate[i].wCodePage,pszItem);
				if (SUCCEEDED(hR))
				{
					// Retrieve file description for language and code page "i". 
					if (VerQueryValue(lpData,szSubBlock,(LPVOID*)&lpInfo,(PUINT)&dwSize))
					{
						sVersionInfo = (LPCTSTR)lpInfo;
						break;	// Only retrieve the first language and code page
					}
				}
			}
		}
	}

	delete[] lpData;
	lpData = NULL;

	return sVersionInfo;
}

/**********************************************************
* 功能:
*		获得文件的最后写时间
* 输入参数:
*		pszFile:[in]文件.
* 返回值:
*		文件的最后写时间.
**********************************************************/
CString CIncrementSystemApp::GetFileLastWriteTime(LPCTSTR pszFile)
{
	HANDLE		hFile = NULL;
	CString		sFileTime = _T("");
	FILETIME	ftCreate = {0};
	FILETIME	ftAccess = {0};
	FILETIME	ftWrite = {0};
	SYSTEMTIME	stUTC = {0};
	SYSTEMTIME	stLocal = {0};

	if (PathFileExists(pszFile))
	{
		hFile = CreateFile(pszFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if(hFile != INVALID_HANDLE_VALUE)
		{
			if (GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite))
			{
				// Convert the last-write time to local time.
				FileTimeToSystemTime(&ftWrite, &stUTC);
				SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
				sFileTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"),
					stLocal.wYear,stLocal.wMonth,stLocal.wDay,
					stLocal.wHour,stLocal.wMinute,stLocal.wSecond);
			}

			CloseHandle(hFile);
		}
	}

	return sFileTime;
}

CString CIncrementSystemApp::GetNetworkCardMac(void)
{
	typedef struct tag_ASTAT
	{
		ADAPTER_STATUS adapt;
		NAME_BUFFER    NameBuff [30];
	} ASTAT, *PASTAT;

	NCB		ncb = {0};
	UCHAR	uRetCode = 0;
	ASTAT   Adapter = {0};
	LANA_ENUM	lana_enum = {0};
	CString strMAC = _T("");

	ncb.ncb_command = NCBENUM;
	ncb.ncb_buffer = (unsigned char *)&lana_enum;
	ncb.ncb_length = sizeof(lana_enum);

	// 向网卡发送NCBENUM命令，以获取当前机器的网卡信息，如有多少个网卡、每张网卡的编号等
	uRetCode = Netbios(&ncb);
	if (uRetCode == 0)
	{
		TRACE(_T("The Ethernet Count is %d.\n"), lana_enum.length);
	}

	memset(&ncb, 0, sizeof(ncb));
	ncb.ncb_command = NCBRESET;
	ncb.ncb_lana_num = lana_enum.lana[0];	// 指定网卡号

	// 发送NCBRESET命令，进行初始化
	uRetCode = Netbios(&ncb);

	memset(&ncb, 0, sizeof(ncb));
	ncb.ncb_command = NCBASTAT;
	ncb.ncb_lana_num = lana_enum.lana[0];   // 指定网卡号

	_tcscpy_s((TCHAR *)ncb.ncb_callname, NCBNAMSZ, _T("*              "));
	ncb.ncb_buffer = (unsigned char *) &Adapter;

	// 指定返回的信息存放的变量
	ncb.ncb_length = sizeof(Adapter);

	// 发送NCBASTAT命令以获取网卡的信息
	uRetCode = Netbios(&ncb);
	if (uRetCode == 0)
	{
		// 把网卡MAC地址格式化成常用的16进制形式
		strMAC.Format(_T("%02X%02X%02X%02X%02X%02X"),
			Adapter.adapt.adapter_address[0],
			Adapter.adapt.adapter_address[1],
			Adapter.adapt.adapter_address[2],
			Adapter.adapt.adapter_address[3],
			Adapter.adapt.adapter_address[4],
			Adapter.adapt.adapter_address[5]);

		TRACE(_T("The MAC is  %s.\n"),strMAC);
	}

	return strMAC;
}