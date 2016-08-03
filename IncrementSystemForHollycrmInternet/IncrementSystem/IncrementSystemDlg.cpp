
// IncrementSystemDlg.cpp : 实现文件
//

#include "stdafx.h"
//#include <afxtempl.h>//20160727 郝爽 动态数组
#include "IncrementSystem.h"
#include "IncrementSystemDlg.h"
#include "ViewVerDlg.h"
#include "PersonalDefineDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


const int nBorderSize = 4;
const int nStaticHeight = 35;
const int nBitmapHeight = 70;

LRESULT CALLBACK LowLevelKeyboardProc (int nCode,WPARAM wParam,LPARAM lParam);

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDHtmlDialog
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDHtmlDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDHtmlDialog)
END_MESSAGE_MAP()


// CIncrementSystemDlg 对话框




CIncrementSystemDlg::CIncrementSystemDlg(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(CIncrementSystemDlg::IDD, CIncrementSystemDlg::IDH, pParent)
{
	m_strType = _T("");
	m_strQueryPhoneTypeURL = _T("");
	m_strQueryPhoneURL = _T("");
	m_strQueryPopupURL = _T("");
	m_strPhoneNo = _T("");
	m_strPopupFlag = _T("0");
	m_bLogined = FALSE;	
	m_bQuery = FALSE;
	m_bManual = TRUE;
	m_bOffWork = FALSE;
	m_bReadClipboard = FALSE;
	m_bVisit = FALSE;
	m_hWndNext = NULL;
}

CIncrementSystemDlg::~CIncrementSystemDlg()
{
	TRACE(_T("Destruction!\n"));
	SaveLog(theApp.m_pszAppName,_T(__FILE__),__LINE__,_T("Exiting ..."));
}

void CIncrementSystemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXPLORER, m_ctlExplorer);
}

BEGIN_MESSAGE_MAP(CIncrementSystemDlg, CDHtmlDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_POPUP_CLOSE, &CIncrementSystemDlg::OnPopupClose)
	ON_COMMAND(ID_POPUP_SHOW, &CIncrementSystemDlg::OnPopupShow)
	ON_COMMAND(ID_POPUP_HIDE, &CIncrementSystemDlg::OnPopupHide)
	ON_COMMAND(ID_POPUP_VIEW, &CIncrementSystemDlg::OnPopupView)
	ON_COMMAND(ID_POPUP_EXIT, &CIncrementSystemDlg::OnPopupExit)
	ON_COMMAND(ID_POPUP_SETTING, &CIncrementSystemDlg::OnPopupSetting)
	ON_WM_COPYDATA()
	ON_WM_DRAWCLIPBOARD()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(CIncrementSystemDlg)
	DHTML_EVENT_ONCLICK(_T("HideWindow"), OnHideWindow)
END_DHTML_EVENT_MAP()

// CIncrementSystemDlg 消息处理程序

BOOL CIncrementSystemDlg::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动

	// TODO: 在此添加额外的初始化代码
	CRect rect;
	CString strURL = _T("");

	SaveLog(theApp.m_pszAppName,_T(__FILE__),__LINE__,_T("***************************************************************************"));
	SaveLog(theApp.m_pszAppName,_T(__FILE__),__LINE__,_T("General edition"));
	DumpModuleVersion(CString(theApp.m_pszAppName) + _T(".exe"));
	DumpModuleVersion(_T("Update.exe"));
	DumpModuleVersion(_T("TelEscrow.dll"));
	SaveLog(theApp.m_pszAppName,_T(__FILE__),__LINE__,_T("***************************************************************************"));
	m_ctlExplorer.SetWindowPos (NULL, 0, 0, 0, 0,SWP_NOACTIVATE | SWP_NOZORDER);

	/*读取是否复制弹窗的配置文件，恢复为上一次设置*/
	int is_certain = GetPrivateProfileInt(_T("ifCopyConfig"), _T("is_certain"),0, _T("./checkcopy.ini"));
	if(is_certain==0)
	{   //maliang：不支持复制弹窗
		theApp.m_bSupportCopy=FALSE;
	}
	if(is_certain==1)
	{   //maliang：支持复制弹窗
		theApp.m_bSupportCopy=TRUE;
	}

	/*20160711 郝爽：弹窗实时前置功能优化，共4部分，修改后实时功能未实现
	分别在IncrementSystemDlg.cpp的第167，466，894以及PersonalDefineDlg.cpp的65行
	郝爽：断点调试时，发现选中与不选中两种情况都实时执行了相应的SetWindowPos语句，但窗口由前置状态改变为不前置状态可立即生效
	郝爽：而由不前置状态改变为前置状态时，语句能够执行，但不能立即生效，需将窗口最小化后重新载入，则可以实现前置
	*/

	//——>20160711 郝爽 设置弹窗置顶 part1 初始化部分
	SetWindowPos (NULL,0, 0,theApp.m_nWidth,theApp.m_nHeight,SWP_NOACTIVATE | SWP_NOZORDER);//SetWindowPos函数初始化
	//2016.3.15,maliang:读取display.ini文件，如果取值为“0”则取消前置，取“1”则设置最前显示
	int is_shown = GetPrivateProfileInt(_T("displayConfig"), _T("is_shown"),0, _T("./display.ini"));
	/*2016.3.16，maliang：给全局变量m_bCancelTop赋值*/
	theApp.m_bSupportTop=is_shown;
	if(is_shown==0)
	{ 
		::SetWindowPos(m_hWnd,HWND_NOTOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);//20160711 郝爽 第一个参数使用下句中的句柄也可
		//::SetWindowPos(this->GetSafeHwnd(),HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
	}
	else
	{   
		::SetWindowPos(m_hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);//20160711 郝爽
	}	
	//——>郝爽：弹窗前置part1修改结束，实时未实现

	//——>20160713 郝爽：读取是否按键号码捕捉的配置文件，恢复为上一次设置
	int is_NoCapture = GetPrivateProfileInt(_T("NoCaptureConfig"), _T("is_NoCapture"),0, _T("./NoCapture.ini"));
	if(is_NoCapture == 1)
	{   //郝爽：支持键盘号码捕捉
		theApp.m_bSupportNoCapture = TRUE;
	}
	if(is_NoCapture == 0)
	{   //郝爽：不支持键盘号码捕捉
		theApp.m_bSupportNoCapture = FALSE;
	}
	//——>20160713 郝爽 读取是否按键号码捕捉的配置文件修改结束

	//——>20160725 郝爽：读取是否开启设定时间内不查询号码功能，恢复为上一次设置
	int is_TimeSetting = GetPrivateProfileInt(_T("TimeSettingConfig"), _T("is_TimeSetting"),0, _T("./TimeSetting.ini"));
	if(is_TimeSetting == 1)
	{   //郝爽：支持开启设定时间内不查询号码功能
		theApp.m_bSupportTimeSetting = TRUE;
		WritePrivateProfileString(_T("NumTimeConfig"), _T("NumTime"),_T(""), _T("./NumTime.ini"));//郝爽 创建配置文件NumTime
		theApp.SliderPos = GetPrivateProfileInt(_T("SliderTimePosConfig"), _T("is_SliderTimePos"),1, _T("./SliderTimePos.ini"));//取滑动条所设定的时间
	}
	if(is_TimeSetting == 0)
	{   //郝爽：不支持开启设定时间内不查询号码功能
		theApp.m_bSupportTimeSetting = FALSE;
	}
	//——>20160725 郝爽 读取是否开启设定时间内不查询号码功能修改结束

	HINSTANCE hInst = AfxGetInstanceHandle();
	HBITMAP hbmpTitle = NULL;
	HICON   hIcon = AfxGetApp()->LoadIcon(IDI_ICON4);

	// 载入图片
	if (theApp.m_bShowClose)
	{	
		hbmpTitle = (HBITMAP)::LoadImage(hInst,_T("TitleWithClose.bmp"), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION|LR_LOADFROMFILE);
	}
	else
	{
		hbmpTitle = (HBITMAP)::LoadImage(hInst,_T("TitleWithoutClose.bmp"), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION|LR_LOADFROMFILE);
	}
	if(hbmpTitle != NULL) m_bmpTitle.Attach(hbmpTitle);

	::SetTimer(m_hWnd,TIME_IDENTIFIER_HEARTBEAT,theApp.m_dwPollingInterval * 1000,NULL);
	::SetTimer(m_hWnd,TIME_IDENTIFIER_CLEAR,24 * 60 * 60 * 1000,NULL);	// 如果系统长期运行，就每天清理一次日志文件。
	/**
	王略 2016 06 22 新增定时关闭功能，省分要求每天必登录，不在允许挂机过24点的情况。
	每天0点，准时关闭弹窗客户端。
	**/
	SYSTEMTIME st;
	GetLocalTime(&st);
	st.wHour;// - 小时
	st.wMinute;// - 分
	st.wSecond;// - 秒
	//::SetTimer(m_hWnd,TIME_IDENTIFIER_CLOSE_DLG,10*1000,NULL);
	::SetTimer(m_hWnd,TIME_IDENTIFIER_CLOSE_DLG,24*60*60*1000-(st.wHour*60*60*1000+st.wMinute*60*1000+st.wSecond*1000),NULL);
	//------>>修改结束

	EnablePrivilege(SE_DEBUG_NAME,TRUE); //提升程序的权限 
	m_hkKeyboard = SetWindowsHookEx(WH_KEYBOARD_LL,(HOOKPROC)LowLevelKeyboardProc,(HINSTANCE)theApp.m_hInstance,0);

	GetWindowRect(&rect);

	m_pBrowserApp->put_Top(nStaticHeight);
	m_pBrowserApp->put_Left(nBorderSize);
	m_pBrowserApp->put_Height(theApp.m_nHeight - nStaticHeight - nBorderSize);
	m_pBrowserApp->put_Width(theApp.m_nWidth - nBorderSize - nBorderSize); 

	m_bVisit = TRUE;
	strURL.Format(_T("%slonginUser=%s"),theApp.m_strMainURL1,theApp.m_strSessionId);
	Navigate(strURL);

	if (!m_TrayIcon.Create(
		NULL,                            // Let icon deal with its own messages
		WM_ICON_NOTIFY,                  // Icon notify message to use
		_T("精细化助销平台\n单击鼠标右键，弹出应用程序的快捷菜单。"),  // tooltip
		hIcon,
		IDR_POPUP_MENU,                  // ID of tray icon
		FALSE,
		_T("双击弹出系统界面\n鼠标右键关闭系统"), // balloon tip
		_T("系统提示"),               // balloon title
		NIIF_INFO,                    // balloon icon
		20 ))                            // balloon timeout
	{
		return -1;
	}

	m_TrayIcon.SetMenuDefaultItem(0, TRUE);

	SetWindowText(theApp.m_strTitle);

	m_hWndNext = SetClipboardViewer(); //将我们的程序添加到剪贴板观察链

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

BOOL CIncrementSystemDlg::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	::KillTimer(m_hWnd,TIME_IDENTIFIER_HEARTBEAT);
	::KillTimer(m_hWnd,TIME_IDENTIFIER_CLEAR);

	if (m_hkKeyboard)
	{
		UnhookWindowsHookEx(m_hkKeyboard);
		m_hkKeyboard = NULL;
	}

	return CDHtmlDialog::DestroyWindow();
}

void CIncrementSystemDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDHtmlDialog::OnSysCommand(nID, lParam);
	}
}


// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CIncrementSystemDlg::OnPaint()
{
	CDHtmlDialog::OnPaint();
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。

void CIncrementSystemDlg::OnSize(UINT nType, int cx, int cy)
{
	CDHtmlDialog::OnSize(nType, cx, cy);
}

BOOL CIncrementSystemDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	CDHtmlDialog::OnEraseBkgnd(pDC);

	CRect	rect;

	GetClientRect(&rect);
	ShowBitmap(pDC,m_bmpTitle,rect);

	return TRUE;
}

HBRUSH CIncrementSystemDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDHtmlDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: Change any attributes of the DC here
	if(nCtlColor == CTLCOLOR_STATIC)
	{
		// IDC_STATIC设为透明
		pDC-> SetBkMode(TRANSPARENT);

		return (HBRUSH)GetStockObject(NULL_BRUSH);
	}

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

BOOL CIncrementSystemDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE; 
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;

	return CDHtmlDialog::PreTranslateMessage(pMsg);
}

void CIncrementSystemDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default bonc123456
	if (nIDEvent == TIME_IDENTIFIER_HEARTBEAT)
	{
		CString strURL = _T("");

		strURL.Format(_T("%susername=%s&sessionId=%s&longinUser=%s"),theApp.m_strHeartbeatURL,theApp.m_strUser,theApp.m_strSessionId,theApp.m_strSessionId);
		m_ctlExplorer.Navigate2(&CComVariant(strURL),NULL,NULL,NULL,NULL);
	}
	else if (nIDEvent == TIME_IDENTIFIER_CLEAR)
	{
		SaveLog(theApp.m_pszAppName,_T(__FILE__),__LINE__,_T("Clearing the overdue logs ..."));
		theApp.ClearOverdueLog();
	}
	//对应和参考上面 王略 20160622修改内容
	if(nIDEvent == TIME_IDENTIFIER_CLOSE_DLG)
	{
		CString strURL = _T("");
		strURL.Format(_T("%sloginId=%s"),theApp.m_strLogoffURL,theApp.m_strUser);

		m_ctlExplorer.Navigate2(&CComVariant(strURL),NULL,NULL,NULL,NULL);

		m_TrayIcon.RemoveIcon();	

		ChangeClipboardChain(m_hWndNext);

		CDHtmlDialog::OnCancel();
	}
	//------>>修改完毕

	CDHtmlDialog::OnTimer(nIDEvent);
}

void CIncrementSystemDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// 需要通过判断单击位置来判断是弹出快捷菜单还是移动窗体。
	// 下面这行代码会把消息截取掉，需要下后续代码那样分开来处理。
	// PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y)); 

	CRect rect;
	CRect rtSystem;
	CRect rtMinimize;
	CRect rtClose;
	CPoint ScreenPoint = point;
	int	  size = GetSystemMetrics(SM_CYSIZE);
	int   space = 2;

	//获取标题栏大小
	rtSystem.left = GetSystemMetrics(SM_CXFRAME);
	rtSystem.top = GetSystemMetrics(SM_CYFRAME);
	rtSystem.right = rtSystem.left + size;
	rtSystem.bottom = rtSystem.top + size;

	GetClientRect(&rect);

	rtClose.left =  rect.Width() - size;
	rtClose.top = rtSystem.top;
	rtClose.right = rect.Width();
	rtClose.bottom = rtSystem.bottom;

	rtMinimize.left =  rtClose.left - size - space;
	rtMinimize.top = rtSystem.top;
	rtMinimize.right = rtClose.left - space;
	rtMinimize.bottom = rtSystem.bottom;


	if (rtSystem.PtInRect(point))
	{
		// 模拟系统菜单
		CMenu  menu;
		CMenu* popup= NULL;
		CPoint point;
		CBitmap bitmap1;
		CBitmap bitmap2;
		CBitmap bitmap3;
		// 加载位图
		bitmap1.LoadBitmap(IDB_BITMAP1);
		bitmap2.LoadBitmap(IDB_BITMAP2);
		bitmap3.LoadBitmap(IDB_BITMAP3);
		// 加载ID为IDR_POPUP_SYSTEM的菜单
		menu.LoadMenu(IDR_POPUP_SYSTEM);

		// 获取弹出菜单的第一层子菜单的指针
		popup= menu.GetSubMenu(0);

		ClientToScreen(&ScreenPoint); 

		//maliang:为菜单的第一，第四和第五项设置图标
		popup->SetMenuItemBitmaps(0,MF_BYPOSITION,&bitmap1,&bitmap1);
		popup->SetMenuItemBitmaps(3,MF_BYPOSITION,&bitmap3,&bitmap3);
		popup->SetMenuItemBitmaps(4,MF_BYPOSITION,&bitmap2,&bitmap2);

		// 显示弹出菜单
		popup->TrackPopupMenu(TPM_LEFTALIGN,ScreenPoint.x,ScreenPoint.y,this); 

		menu.DestroyMenu(); 
	}
	else if (rtClose.PtInRect(point))
	{
		m_bManual = FALSE;
		if (theApp.m_bShowClose) OnPopupExit();
	}
	else if (rtMinimize.PtInRect(point))
	{
		m_bManual = FALSE;
		ShowWindow(SW_HIDE);
	}
	else
	{
		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y)); 
	}

	CDHtmlDialog::OnLButtonDown(nFlags, point);
}

void CIncrementSystemDlg::OnPopupClose()
{
	// TODO: Add your command handler code here
	m_bManual = FALSE;
	ShowWindow(SW_HIDE);
}

void CIncrementSystemDlg::OnPopupShow()
{
	// TODO: Add your command handler code here
	//注释之前代码
	/*m_bManual = TRUE;
	m_TrayIcon.StopAnimation();
	ShowWindow(SW_NORMAL);
	::SetWindowPos(this->GetSafeHwnd(),NULL,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE); */
	/*2016.3.16，maliang:点击图标，判断全局变量m_bSupportTop，若为1则前置，为0不前置*/

	//——>20160711 郝爽 弹窗前置 part2 
	//郝爽：实时将is_shown的值赋值给全局变量theApp.m_bSupportTop，为1则前置，为0则不前置
	theApp.m_bSupportTop=GetPrivateProfileInt(_T("displayConfig"), _T("is_shown"),0, _T("./display.ini"));//郝爽

	if(theApp.m_bSupportTop==0)
	{
		m_bManual = TRUE;
		m_TrayIcon.StopAnimation();
		ShowWindow(SW_NORMAL);
		::SetWindowPos(this->GetSafeHwnd(),HWND_NOTOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);//20160711 郝爽 句柄参数可替换为m_hWnd
	}
	else 
	{
		m_bManual = TRUE;
		m_TrayIcon.StopAnimation();
		ShowWindow(SW_NORMAL);
		::SetWindowPos(this->GetSafeHwnd(),HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE); //20160711 郝爽
	}
}
//——>郝爽：弹窗前置part2修改结束，实时未实现

void CIncrementSystemDlg::OnPopupHide()
{
	// TODO: Add your command handler code here
	m_bManual = FALSE;
	ShowWindow(SW_HIDE);
}

void CIncrementSystemDlg::OnPopupView()
{
	// TODO: Add your command handler code here
	CViewVerDlg dlg;

	dlg.DoModal();
}

void CIncrementSystemDlg::OnPopupExit()
{
	// TODO: Add your command handler code here
	CString strURL = _T("");

	if (!m_bOffWork)
	{
		if (MessageBox(_T("您确定要退出精细化助销平台吗？"),theApp.m_strTitle, MB_YESNO + MB_ICONQUESTION) == IDNO) return;
	}

	strURL.Format(_T("%sloginId=%s"),theApp.m_strLogoffURL,theApp.m_strUser);
	m_ctlExplorer.Navigate2(&CComVariant(strURL),NULL,NULL,NULL,NULL);

	m_TrayIcon.RemoveIcon();	

	ChangeClipboardChain(m_hWndNext);

	CDHtmlDialog::OnCancel();

	ExitProcess(0);	//20160729 郝爽 关闭整个程序进程
}

BOOL CIncrementSystemDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	// TODO: Add your message handler code here and/or call default
	if (pCopyDataStruct)
	{
		CString strFromSend = _T("");
		CString strValidPhoneNo = _T("");
		char	szText[MAX_PATH] = {0};
		int		nSize = 0;

		nSize = (pCopyDataStruct->cbData < MAX_PATH ? pCopyDataStruct->cbData : MAX_PATH);
		memcpy(szText,pCopyDataStruct->lpData,nSize);

		szText[nSize] = 0;
		strFromSend = szText;

		SaveLog(theApp.m_pszAppName,_T(__FILE__),__LINE__,_T("The data from the other app is \"") + strFromSend + _T("\"."));

		if (NeedQuery(strFromSend,strValidPhoneNo,FALSE))
		{
			ExecuteQuery(strValidPhoneNo,FALSE);
		}
	}

	return CDHtmlDialog::OnCopyData(pWnd, pCopyDataStruct);
}

void CIncrementSystemDlg::OnDrawClipboard()
{
	SaveLog(theApp.m_pszAppName,_T(__FILE__),__LINE__,_T("This line is running CIncrementSystemDlg::OnDrawClipboard!"));
	CDHtmlDialog::OnDrawClipboard();
	// TODO: Add your message handler code here

	// 打开剪贴板
	if ( OpenClipboard() )
	{
		char*	buffer = NULL;
		HANDLE	hData = NULL;
		CString strFromClipboard = _T("");
		CString strValidPhoneNo = _T("");

		hData = GetClipboardData(CF_TEXT); //获取剪贴板内存块首地址
		buffer = (char*)GlobalLock(hData); //锁定剪贴板内存块，指针指向该内存块内容
		strFromClipboard = buffer;

		GlobalUnlock(hData); //解除锁定
		CloseClipboard(); //关闭剪贴板

		if (!m_bReadClipboard) //m_bReadClipboard初值为false
		{
			SaveLog(theApp.m_pszAppName,_T(__FILE__),__LINE__,_T("The data from clipboard will be ignored when the app is starting!"));
			m_bReadClipboard = TRUE;
			return;
		}

		if (theApp.m_bSupportCopy == FALSE) return; 

		SaveLog(theApp.m_pszAppName,_T(__FILE__),__LINE__,_T("The data from clipboard is \"") + strFromClipboard + _T("\"."));

		if (NeedQuery(strFromClipboard,strValidPhoneNo,FALSE))
		{
			ExecuteQuery(strValidPhoneNo,FALSE);
			////2016/3/23:maliang:发出查询请求后令“m_bSupportCopy”=FALSE来关闭剪贴板
			theApp.m_bSupportCopy=FALSE;
		}
	}
}

HRESULT CIncrementSystemDlg::OnHideWindow(IHTMLElement* /*pElement*/)
{
	ShowWindow(SW_HIDE);
	return S_OK;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// 功能:
//		提升程序的权限
// 输入参数:
//		lpName：		[in]，权限名;
//		fEnable：	[in]，权限开关.
// 返回值:
//		TRUE：成功；
//		FALSE：失败.
BOOL CIncrementSystemDlg::EnablePrivilege(LPCTSTR lpszName, BOOL fEnable)
{
	LUID				Luid = {0};
	HANDLE				hObject = NULL;
	TOKEN_PRIVILEGES	NewStatus = {0}; 

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES, &hObject))
		return FALSE;

	if (LookupPrivilegeValue(NULL, lpszName, &Luid))
	{
		NewStatus.Privileges[0].Luid = Luid;
		NewStatus.PrivilegeCount = 1;
		NewStatus.Privileges[0].Attributes = fEnable ? SE_PRIVILEGE_ENABLED : 0;

		AdjustTokenPrivileges(hObject, FALSE, &NewStatus, 0, 0, 0);

		CloseHandle(hObject);
		return TRUE;
	}

	return FALSE;
}

/*
中国联通：
号　　段：130  131 132 155 156 176 185 186
*/
BOOL CIncrementSystemDlg::NeedQuery(CString strPhoneNo, CString &strValidPhoneNo, BOOL bInput)
{
	CString strPrefix = _T("");
	CString strHistoryPhone = _T("");
	DWORD	dwHistoryTime = 0;
	DWORD	dwTime = 0;
	BOOL	bFound = FALSE;
	TCHAR   ch = 0;
	int		nSize = 0;

	SaveLog(theApp.m_pszAppName,_T(__FILE__),__LINE__,_T("Acquired data = ") + strPhoneNo);

	if (strPhoneNo.GetLength() < 11) return FALSE;

	strValidPhoneNo = strPhoneNo.Right(11);
	strPrefix = strValidPhoneNo.Left(3);

	SaveLog(theApp.m_pszAppName,_T(__FILE__),__LINE__,strValidPhoneNo);

	for (int i = 3; i < 11; i++)
	{
		ch = strValidPhoneNo.GetAt(i); //GetAt取字符串中第i个字符
		if ((ch < '0') || (ch > '9')) 
		{
			SaveLog(theApp.m_pszAppName,_T(__FILE__),__LINE__,_T("The acquired data is not a valid phone number."));
			return FALSE;
		}
	}

	//Cstring类中Compare函数，比较结果相同则返回0
	if (strPrefix.Compare(_T("130")) && strPrefix.Compare(_T("131")) 
		&& strPrefix.Compare(_T("132")) && strPrefix.Compare(_T("155")) 
		&& strPrefix.Compare(_T("156")) && strPrefix.Compare(_T("185")) 
		&& strPrefix.Compare(_T("186")) && strPrefix.Compare(_T("176"))) 
	{
		SaveLog(theApp.m_pszAppName,_T(__FILE__),__LINE__,_T("The acquired data is not a valid china unicom phone number."));
		return FALSE;
	}

	//——>20160726 郝爽 判断是否开启设定时间内不进行号码查询功能修改
	if (theApp.m_bSupportTimeSetting == TRUE)
	{
		//郝爽 读取当前系统时间
		SYSTEMTIME st;
		GetLocalTime(&st);
		st.wHour;// - 小时
		st.wMinute;// - 分
		st.wSecond;// - 秒

		CString StrCurrentTime;//当前时间
		StrCurrentTime.Format(_T("%.2d%.2d%.2d"),st.wHour,st.wMinute,st.wSecond);
		
		//郝爽 读取配置文件中的历史“号码时间”
		CString StrHistoryNumTime;
		GetPrivateProfileString(_T("NumTimeConfig"), _T("NumTime"),_T(""),StrHistoryNumTime.GetBuffer(500*1024),500*1024, _T("./NumTime.ini"));
		StrHistoryNumTime.ReleaseBuffer();

		int Count = 0;
		int NumTimeFindStart = 0;
		int NumTimeFindPosition = 0;
		int IntCurrentTime = 0;
		int IntHistoryTime = 0;
		BOOL is_Num = FALSE;//为TRUE时表示有相同号码
		CStringArray AllHistoryNumTime;
		CString strHistoryTime = _T("");
		
		//分离配置文件中的号码
		while(NumTimeFindPosition = StrHistoryNumTime.Find(';',NumTimeFindStart))
		{
			if(NumTimeFindPosition < 1)
			{
				break;
			}
			CString TempSingleNumTime = StrHistoryNumTime.Mid(NumTimeFindStart,NumTimeFindPosition-NumTimeFindStart);
			AllHistoryNumTime.Add(TempSingleNumTime);//将分离出的“号码时间”存入字符串数组
			
			NumTimeFindStart = NumTimeFindPosition + 1;
			NumTimeFindPosition = 0;	
			Count++;
		}
		//判断配置文件中是否有相同号码
		for(int i=0;i<=(Count-1);i++)
		{
			if(!strValidPhoneNo.Compare(AllHistoryNumTime.GetAt(i).Left(11)))
			{
				strHistoryTime = AllHistoryNumTime.GetAt(i).Right(6);
				is_Num = TRUE;
			}
		}
		//有相同号码
		if(is_Num == TRUE)
		{
			IntCurrentTime = st.wHour*60*60+st.wMinute*60+st.wSecond;
			IntHistoryTime = _ttoi(strHistoryTime.Mid(0,2))*60*60+_ttoi(strHistoryTime.Mid(2,2))*60+_ttoi(strHistoryTime.Mid(4,2));
			if((IntCurrentTime-IntHistoryTime) <= ((theApp.SliderPos/2.0)*60))
			{
				return FALSE;//同一号码出现时间小于等于设定时间，则跳出，不进行查询
			}
			else
			{
				//同一号码出现时间大于设定时间，替换原来号码所对应的时间，并执行查询
				StrHistoryNumTime.Replace(strHistoryTime,StrCurrentTime);
				WritePrivateProfileString(_T("NumTimeConfig"), _T("NumTime"),StrHistoryNumTime, _T("./NumTime.ini"));
			}
		}
		else  //无相同号码
		{
			//将号码+时间写入配置文件，并执行查询
			StrHistoryNumTime = StrHistoryNumTime + strValidPhoneNo + StrCurrentTime + _T(";");
			WritePrivateProfileString(_T("NumTimeConfig"), _T("NumTime"),StrHistoryNumTime, _T("./NumTime.ini"));
		}
	}
	//——>20160726 郝爽 判断是否开启设定时间内不进行号码查询功能修改结束*/

	if (!bInput) return TRUE;

	dwTime = GetTickCount(); //GetTickCount计算机启动后经过的毫秒数
	nSize = m_saHistory.GetCount(); //GetCount获取数组中元素个数

	if (m_dwaHistory.GetCount() != nSize) 
	{
		SaveLog(theApp.m_pszAppName,_T(__FILE__),__LINE__,_T("The number of two array elements is not equal. Resetting ..."));
		m_saHistory.RemoveAll();//清空整个数组
		m_dwaHistory.RemoveAll();

		m_saHistory.Add(strValidPhoneNo);//m_saHistory是CstringArray型，Add表示在数组末尾添加元素
		m_dwaHistory.Add(dwTime);
		return TRUE;
	}

	// 删掉已经超时的号码。
	// 如果后面的号码超时，则前面的号码一定超时。
	for (int i = nSize - 1; i >= 0; i --)
	{
		SaveLog(theApp.m_pszAppName,_T(__FILE__),__LINE__,_T("Removes all the overtime elements ..."));
		dwHistoryTime = m_dwaHistory.GetAt(i);
		if (dwTime - dwHistoryTime > theApp.m_dwQueryElapse * 1000) 
		{
			for (int m = i; m >= 0; m --)
			{
				m_saHistory.RemoveAt(m);
				m_dwaHistory.RemoveAt(m);
			}
			break;
		}
	}

	nSize = m_saHistory.GetCount();		// 必须重新计算长度
	for (int i = 0; i < nSize; i ++)	// 剩下的都不超时
	{
		strHistoryPhone = m_saHistory.GetAt(i);
		if (strHistoryPhone.CompareNoCase(strValidPhoneNo) == 0)
		{
			SaveLog(theApp.m_pszAppName,_T(__FILE__),__LINE__,_T("Replace the older element with a newer element  ..."));
			m_saHistory.RemoveAt(i);
			m_dwaHistory.RemoveAt(i);
			m_strType.Empty();
			m_strPhoneNo.Empty();
			bFound = TRUE;
			break;
		}
	}


	m_saHistory.Add(strValidPhoneNo);
	m_dwaHistory.Add(dwTime);

	return (!bFound);
}

void CIncrementSystemDlg::ExecuteQuery(CString &strPhoneNo, BOOL bInput)
{
	m_strQueryPhoneTypeURL.Format(_T("%stel=%s&longinUser=%s"),theApp.m_strPhoneTypeURL,strPhoneNo,theApp.m_strSessionId);
	m_strQueryPhoneURL.Format(_T("%stel=%s&longinUser=%s"),theApp.m_strPhoneURL,strPhoneNo,theApp.m_strSessionId);
	m_strQueryPopupURL.Format(_T("%stel=%s&longinUser=%s"),theApp.m_strPopupURL,strPhoneNo,theApp.m_strSessionId);

	m_bQuery = TRUE;
	m_strType.Empty();

	if (bInput) strPhoneNo.Empty();

	Navigate(m_strQueryPhoneTypeURL);
}


LRESULT CALLBACK LowLevelKeyboardProc (int nCode,WPARAM wParam,LPARAM lParam)
{

	if (wParam == WM_KEYUP)
	{
		KBDLLHOOKSTRUCT		key = {0};
		CString				strKey = _T("");
		CString				strValidPhoneNo = _T("");
		int					nKeySize = 0;
		CIncrementSystemDlg* pMe = NULL;

		pMe = (CIncrementSystemDlg*)theApp.m_pMainWnd;
		if (pMe == NULL) return 1;

		memcpy(&key,(void*)lParam,sizeof(KBDLLHOOKSTRUCT));

		strKey.Format(_T("0x%02x = %c"),key.vkCode,key.vkCode);
		//pMe->SaveLog(theApp.m_pszAppName,_T(__FILE__),__LINE__,strKey);

		//20160714 郝爽 判断全局变量theApp.m_bSupportNoCapture为TRUE时，执行下边按键判断查询的语句，否则跳过
		if(theApp.m_bSupportNoCapture == FALSE)return 0;
		//20160714 郝爽 修改结束

		if (key.vkCode >= 0x30 && key.vkCode <= 0x39)
		{
			strKey.Format(_T("%d"),key.vkCode - 0x30);
			pMe->m_strPhoneNo += strKey;
			TRACE(_T("The keystroke message was received.the key is %d.\n"),key.vkCode);

			if (pMe->NeedQuery(pMe->m_strPhoneNo,strValidPhoneNo))
			{
				pMe->m_strQueryPhoneTypeURL.Format(_T("%stel=%s&longinUser=%s"),theApp.m_strPhoneTypeURL,strValidPhoneNo,theApp.m_strSessionId);
				pMe->m_strQueryPhoneURL.Format(_T("%stel=%s&longinUser=%s"),theApp.m_strPhoneURL,strValidPhoneNo,theApp.m_strSessionId);
				pMe->m_strQueryPopupURL.Format(_T("%stel=%s&longinUser=%s"),theApp.m_strPopupURL,strValidPhoneNo,theApp.m_strSessionId);
				pMe->m_bQuery = TRUE;
				pMe->m_strType.Empty();
				pMe->m_strPhoneNo.Empty();
				pMe->Navigate(pMe->m_strQueryPhoneTypeURL);
			}
		}
		else if (key.vkCode >= 0x60 && key.vkCode <= 0x69)
		{
			strKey.Format(_T("%d"),key.vkCode - 0x60);
			pMe->m_strPhoneNo += strKey;
			TRACE(_T("The keystroke message was received.the key is %d.\n"),key.vkCode);

			if (pMe->NeedQuery(pMe->m_strPhoneNo,strValidPhoneNo))
			{
				pMe->m_strQueryPhoneTypeURL.Format(_T("%stel=%s&longinUser=%s"),theApp.m_strPhoneTypeURL,strValidPhoneNo,theApp.m_strSessionId);
				pMe->m_strQueryPhoneURL.Format(_T("%stel=%s&longinUser=%s"),theApp.m_strPhoneURL,strValidPhoneNo,theApp.m_strSessionId);
				pMe->m_strQueryPopupURL.Format(_T("%stel=%s&longinUser=%s"),theApp.m_strPopupURL,strValidPhoneNo,theApp.m_strSessionId);
				pMe->m_bQuery = TRUE;
				pMe->m_strType.Empty();
				pMe->m_strPhoneNo.Empty();
				pMe->Navigate(pMe->m_strQueryPhoneTypeURL);
			}
		}

		else if (key.vkCode == VK_BACK)
		{
			nKeySize = pMe->m_strPhoneNo.GetLength();
	
			
			if (nKeySize > 0)
			{
				pMe->m_strPhoneNo = pMe->m_strPhoneNo.Left(nKeySize - 1);
			}
		}
		else if (key.vkCode != VK_RETURN)
		{
			pMe->m_strType.Empty();
			pMe->m_strPhoneNo.Empty();
		}
	}


	return 0;

}

void CIncrementSystemDlg::OnBeforeNavigate(LPDISPATCH pDisp, LPCTSTR szUrl)
{
	// TODO: Add your specialized code here and/or call the base class
	if (!m_bVisit) return;
	SaveLog(theApp.m_pszAppName,_T(__FILE__),__LINE__,_T("URL Navigating = ") + CString(szUrl));

	CDHtmlDialog::OnBeforeNavigate(pDisp, szUrl);
}

void CIncrementSystemDlg::OnDocumentComplete(LPDISPATCH pDisp, LPCTSTR szUrl)
{
	CDHtmlDialog::OnDocumentComplete(pDisp, szUrl);

	// TODO: Add your specialized code here and/or call the base class
	if ((_tcslen(szUrl) < 1) || (szUrl == NULL)) return;

	SaveLog(theApp.m_pszAppName,_T(__FILE__),__LINE__,_T("URL Completed = ") + CString(szUrl));

	if (IsDocumentComplete(pDisp))
	{
		HandleQueryRequest(pDisp,szUrl);

	}
}


BOOL CIncrementSystemDlg::IsDocumentComplete(LPDISPATCH pDisp)
{
	HRESULT		hr = S_FALSE;
	CWnd*		pCtrl = NULL;
	IUnknown*	pUnk = NULL;
	LPDISPATCH	pWBDisp = NULL;

	pCtrl = GetDlgItem(AFX_IDC_BROWSER);
	if (pCtrl == NULL) return FALSE;

	pUnk = pCtrl->GetControlUnknown();
	if (pUnk == NULL) return FALSE;

	hr = pUnk->QueryInterface(IID_IDispatch, (void**)&pWBDisp);
	if (FAILED(hr)) return FALSE;

	if (pDisp == pWBDisp) // 页面加载完成
	{

		pWBDisp->Release();
		return TRUE;
	}

	pWBDisp->Release();
	return FALSE;
}

void CIncrementSystemDlg::HandleQueryRequest(LPDISPATCH pDisp, LPCTSTR szUrl)
{
	CString	strURL(szUrl);
	CString	strQueryPhoneTypeURL(m_strQueryPhoneTypeURL);
	CString	strQueryPhoneURL(m_strQueryPhoneURL);
	CString	strQueryPopupURL(m_strQueryPopupURL);

	if (strURL.IsEmpty() || m_strQueryPhoneTypeURL.IsEmpty() || m_strQueryPhoneURL.IsEmpty() || m_strQueryPopupURL.IsEmpty()) return;

	if (strURL.Find(strQueryPhoneTypeURL) > -1)
	{
		ReadPhoneType(pDisp);
	}
	else if (strURL.Find(strQueryPopupURL) > -1)
	{
		ReadPopupFlag(pDisp);
	}
	else if (strURL.Find(strQueryPhoneURL) > -1)
	{
		SaveLog(theApp.m_pszAppName,_T(__FILE__),__LINE__,_T("PopupFlag = ") + m_strPopupFlag);
		//maliang/3/24：如果弹出标记为“1”
		if (m_strPopupFlag == _T("1"))
			//maliang：注释之前代码：
			//**start
			/*ShowWindow(SW_NORMAL);		
			::SetWindowPos(this->GetSafeHwnd(),HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE); */ //**end
			//maliang：添加新代码**start
			//2016.3.16，maliang：判断全局变量m_bSupportTop的值，若为1则前置，不为1则取消前置
		{   //maliang：不支持弹窗前置

			//——>20160711 郝爽 弹窗前置 part3
			//郝爽：实时将is_shown的值赋值给全局变量theApp.m_bSupportTop，为1则前置，为0则不前置
			theApp.m_bSupportTop = GetPrivateProfileInt(_T("displayConfig"), _T("is_shown"),0, _T("./display.ini"));

			if(theApp.m_bSupportTop==0)
			{
				ShowWindow(SW_NORMAL);
				//::SwitchToThisWindow(this->GetSafeHwnd(),true);//20160729 王略 前端显示
				::SetForegroundWindow(this->GetSafeHwnd());//20160729 郝爽 前端显示

				::SetWindowPos(this->GetSafeHwnd(),HWND_NOTOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE); //20160711 郝爽

				//maliang：读取是否支持复制弹窗的配置文件，如果读取值为“0”则继续关闭剪贴板，为“1”则恢复剪贴板
				int is_certain = GetPrivateProfileInt(_T("ifCopyConfig"), _T("is_certain"),0, _T("./checkcopy.ini"));
				if(is_certain==0)
				{
					//maliang：不支持复制弹窗
					theApp.m_bSupportCopy=FALSE;
				}
				else
				{
					//maliang：支持复制弹窗(打开剪贴板)
					theApp.m_bSupportCopy=TRUE;
				}	
			}
			//maliang：支持弹窗前置
			if(theApp.m_bSupportTop==1)
			{
				ShowWindow(SW_NORMAL);
				::SetWindowPos(this->GetSafeHwnd(),HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE); //20160711 郝爽
				//郝爽：弹窗前置part4修改完成，实时未实现

				//maliang/3/24：读取是否支持复制弹窗的配置文件，如果读取值为“0”则继续关闭剪贴板，为“1”则恢复剪贴板
				int is_certain = GetPrivateProfileInt(_T("ifCopyConfig"), _T("is_certain"),0, _T("./checkcopy.ini"));
				if(is_certain==0)
				{
					//maliang：不支持复制弹窗
					theApp.m_bSupportCopy=FALSE;
				}
				else
				{ 
					//maliang：支持复制弹窗(打开剪贴板)
					theApp.m_bSupportCopy=TRUE;
				}
			}
		}

		//maliang/3/24:如果弹出标记为“0”：
		else
		{
			//maliang：读取是否支持复制弹窗的配置文件，如果读取值为“0”则继续关闭剪贴板，为“1”则恢复剪贴板
			int is_certain = GetPrivateProfileInt(_T("ifCopyConfig"), _T("is_certain"),0, _T("./checkcopy.ini"));
			if(is_certain==0)
			{
				//maliang：不支持复制弹窗
				theApp.m_bSupportCopy=FALSE;
			}
			else
			{
				//maliang：支持复制弹窗(打开剪贴板)
				theApp.m_bSupportCopy=TRUE;
			}
		}

	}
}

void CIncrementSystemDlg::ReadPhoneType(LPDISPATCH pDisp)
{
	CComPtr<IHTMLDocument2> spDocument;
	CComPtr<IHTMLElement>	pElem;
	CComPtr<IHTMLElement>	pElem2;
	HRESULT					hr = S_FALSE;
	CString					strBody = _T("");
	BSTR					bstrBody; 

	hr = GetDHtmlDocument(&spDocument);
	if(FAILED(hr)) 
	{
		TRACE(_T("Failed to call GetDHtmlDocument().\n"));
		SaveLog(theApp.m_pszAppName,_T(__FILE__),__LINE__,_T("Failed to call GetDHtmlDocument()."));
		return;
	}

	hr = spDocument->get_body(&pElem);
	if(FAILED(hr)) 
	{
		TRACE(_T("Failed to call get_body().\n"));
		SaveLog(theApp.m_pszAppName,_T(__FILE__),__LINE__,_T("Failed to call get_body()."));
		return;
	}

	hr = pElem->get_parentElement(&pElem2);
	if(FAILED(hr)) 
	{
		TRACE(_T("Failed to call get_parentElement().\n"));
		SaveLog(theApp.m_pszAppName,_T(__FILE__),__LINE__,_T("Failed to call get_parentElement()."));
		return;
	}

	hr = pElem2->get_outerHTML(&bstrBody);
	if(FAILED(hr)) 
	{
		TRACE(_T("Failed to call get_outerHTML().\n"));
		SaveLog(theApp.m_pszAppName,_T(__FILE__),__LINE__,_T("Failed to call get_outerHTML()."));
		return;
	}

	strBody = bstrBody;
	::SysFreeString(bstrBody);

	m_strType = GetResponse(strBody);

	TRACE(_T("Navigating to the specified URL: %s.\n"),m_strQueryPopupURL);

	Navigate(m_strQueryPopupURL);
}

void CIncrementSystemDlg::ReadPopupFlag(LPDISPATCH pDisp)
{
	CComPtr<IHTMLDocument2> spDocument;
	CComPtr<IHTMLElement>	pElem;
	CComPtr<IHTMLElement>	pElem2;
	HRESULT					hr = S_FALSE;
	CString					strBody = _T("");
	BSTR					bstrBody; 

	m_strPopupFlag = _T("0");

	hr = GetDHtmlDocument(&spDocument);
	if(FAILED(hr)) 
	{
		TRACE(_T("Failed to call GetDHtmlDocument().\n"));
		SaveLog(theApp.m_pszAppName,_T(__FILE__),__LINE__,_T("Failed to call GetDHtmlDocument()."));
		return;
	}

	hr = spDocument->get_body(&pElem);
	if(FAILED(hr)) 
	{
		TRACE(_T("Failed to call get_body().\n"));
		SaveLog(theApp.m_pszAppName,_T(__FILE__),__LINE__,_T("Failed to call get_body()."));
		return;
	}

	hr = pElem->get_parentElement(&pElem2);
	if(FAILED(hr)) 
	{
		TRACE(_T("Failed to call get_parentElement().\n"));
		SaveLog(theApp.m_pszAppName,_T(__FILE__),__LINE__,_T("Failed to call get_parentElement()."));
		return;
	}

	hr = pElem2->get_outerHTML(&bstrBody);
	if(FAILED(hr)) 
	{
		TRACE(_T("Failed to call get_outerHTML().\n"));
		SaveLog(theApp.m_pszAppName,_T(__FILE__),__LINE__,_T("Failed to call get_outerHTML()."));
		return;
	}

	strBody = bstrBody;
	::SysFreeString(bstrBody);

	m_strPopupFlag = GetResponse(strBody);
	Navigate(m_strQueryPhoneURL);
}

CString	CIncrementSystemDlg::GetResponse(CString strBody)
{
	CString strResponse = _T("");
	CString strBegin = _T("<body>");
	CString strEnd = _T("</body>");
	CString strTemp = strBody;
	int		nStart = 0;
	int		nPos = -1;

	SaveLog(theApp.m_pszAppName,_T(__FILE__),__LINE__,strBody);

	strTemp.MakeLower();
	nPos = strTemp.Find(strBegin);
	if (nPos > 0) 
	{
		nStart = nPos + strBegin.GetLength();
		nPos = strTemp.Find(strEnd,nStart);
		if (nPos > 0) 
		{
			strResponse = strBody.Mid(nStart,nPos - nStart);
		}
	}

	SaveLog(theApp.m_pszAppName,_T(__FILE__),__LINE__,_T("The response is \"") + strResponse + _T("\"."));

	return strResponse;
}

void CIncrementSystemDlg::ShowBitmap(CDC* pDC, CBitmap &bmp, CRect &rect)
{
	CDC			dc;
	int			bmpWidth = 0;
	int			bmpHeight = 0;
	BITMAP		bmpBitmap = {0};
	CBitmap*	pOldBitmap = NULL;

	if(!bmp.m_hObject) return;

	// 建立一个兼容DC.
	dc.CreateCompatibleDC(pDC);

	// 首先保存原来的设备环境.
	pOldBitmap = dc.SelectObject(&bmp);

	// 取得Bitmap对象的信息
	bmp.GetBitmap(&bmpBitmap);
	bmpWidth = bmpBitmap.bmWidth;
	bmpHeight = bmpBitmap.bmHeight;

	// 保证图片质量.
	pDC->SetStretchBltMode(STRETCH_HALFTONE);

	// 显示图片
	pDC->StretchBlt(rect.left,rect.top,rect.Width(),rect.Height(),&dc,0,0,bmpWidth,bmpHeight,SRCCOPY);

	// 恢复原来的设备环境.
	dc.SelectObject(pOldBitmap);
}

BOOL CIncrementSystemDlg::SaveLog(LPCTSTR lpszLogFile, LPCTSTR lpszSourceName, int nLine, LPCTSTR lpszText)
{
	int			nIndex = -1;
	FILE*		stream = NULL;
	errno_t		err = 0;
	CString		strLogPath = _T("");
	CString		strLogFile = _T("");
	CString		strFileNmae(lpszSourceName);
	SYSTEMTIME	time = {0};

	if (!theApp.m_bSaveLog) return FALSE;
	if (_taccess_s(theApp.m_strLogPath,0)) return FALSE;	// directory does not exist

	nIndex = strFileNmae.ReverseFind(_T('\\'));
	if (nIndex > -1)
	{
		strFileNmae = strFileNmae.Mid(nIndex + 1);
	}

	GetLocalTime(&time);
	strLogFile.Format(_T("%s\\%s%04d%02d%02d.txt"),theApp.m_strLogPath,lpszLogFile,time.wYear,time.wMonth,time.wDay);
	err  = _tfopen_s( &stream, strLogFile, _T("a+"));
	if(err != 0)
	{
		TRACE(_T("The file '%s' was not opened.\n"),strLogFile);
		return FALSE;
	}

	_ftprintf_s(stream,_T("[%02d:%02d:%02d:%03d %20s(%04d)] %s\n"),
		time.wHour,time.wMinute,time.wSecond,time.wMilliseconds,strFileNmae,nLine,lpszText);

	fclose( stream );

	return TRUE;
}

void CIncrementSystemDlg::DumpModuleVersion(LPCTSTR lpszModuleName)
{
	CString strFile = _T("");
	CString strVersion = _T("");
	CString strTime = _T("");
	CString strMsg = _T("");

	strFile.Format(_T(".\\%s"),lpszModuleName);
	strVersion = theApp.GetModuleVersionInfo(strFile,_T("FileVersion"));
	strTime = theApp.GetFileLastWriteTime(strFile);

	strMsg.Format(_T("[ %s version = %s;last write time = %s ]"),lpszModuleName,strVersion,strTime);

	SaveLog(theApp.m_pszAppName,_T(__FILE__),__LINE__,strMsg);
}

BEGIN_EVENTSINK_MAP(CIncrementSystemDlg, CDHtmlDialog)
	ON_EVENT(CIncrementSystemDlg, IDC_EXPLORER, 259, CIncrementSystemDlg::DocumentCompleteExplorer, VTS_DISPATCH VTS_PVARIANT)
END_EVENTSINK_MAP()

void CIncrementSystemDlg::DocumentCompleteExplorer(LPDISPATCH pDisp, VARIANT* URL)
{
	// TODO: Add your message handler code here
	HRESULT hr = S_FALSE;
	CComPtr<IDispatch> pDocDisp(m_ctlExplorer.get_Document());   
	CComPtr<IHTMLDocument2> spDocument;
	CComPtr<IHTMLElement> pElem;
	CComPtr<IHTMLElement> pElem2;
	CString strBody = _T("");
	CString strResponse = _T("");
	BSTR bstrBody; 

	ASSERT((IDispatch*)pDocDisp);   

	hr = pDocDisp->QueryInterface(IID_IHTMLDocument, (void**)&spDocument);
	if(FAILED(hr)) 
	{
		TRACE(_T("Failed to call QueryInterface().\n"));
		return;
	}

	hr = spDocument->get_body(&pElem);
	if(FAILED(hr)) 
	{
		TRACE(_T("Failed to call get_body().\n"));
		return;
	}

	hr = pElem->get_parentElement(&pElem2);
	if(FAILED(hr)) 
	{
		TRACE(_T("Failed to call get_parentElement().\n"));
		return;
	}

	hr = pElem2->get_outerHTML(&bstrBody);
	if(FAILED(hr)) 
	{
		TRACE(_T("Failed to call get_outerHTML().\n"));
		return;
	}

	strBody = bstrBody;
	::SysFreeString(bstrBody);	

	if (!m_bLogined)
	{
		strResponse = GetResponse(strBody);

		if (strResponse.CompareNoCase(_T("offwork")) == 0)
		{
			m_bOffWork = TRUE;	// 不在工作时间内
			OnPopupExit();
		}
		else
		{
			m_bLogined = TRUE;
		}
	}
}
//maliang:点击菜单中自定义设置的相应函数
void CIncrementSystemDlg::OnPopupSetting()
{
	//maliang:显示一个模态对话框
	CPersonalDefineDlg pd ;
	pd.DoModal();

}