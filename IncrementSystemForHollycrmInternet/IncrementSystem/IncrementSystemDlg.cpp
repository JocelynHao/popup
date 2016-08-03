
// IncrementSystemDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
//#include <afxtempl.h>//20160727 ��ˬ ��̬����
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

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDHtmlDialog
{
public:
	CAboutDlg();

	// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	// ʵ��
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


// CIncrementSystemDlg �Ի���




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

// CIncrementSystemDlg ��Ϣ�������

BOOL CIncrementSystemDlg::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	CRect rect;
	CString strURL = _T("");

	SaveLog(theApp.m_pszAppName,_T(__FILE__),__LINE__,_T("***************************************************************************"));
	SaveLog(theApp.m_pszAppName,_T(__FILE__),__LINE__,_T("General edition"));
	DumpModuleVersion(CString(theApp.m_pszAppName) + _T(".exe"));
	DumpModuleVersion(_T("Update.exe"));
	DumpModuleVersion(_T("TelEscrow.dll"));
	SaveLog(theApp.m_pszAppName,_T(__FILE__),__LINE__,_T("***************************************************************************"));
	m_ctlExplorer.SetWindowPos (NULL, 0, 0, 0, 0,SWP_NOACTIVATE | SWP_NOZORDER);

	/*��ȡ�Ƿ��Ƶ����������ļ����ָ�Ϊ��һ������*/
	int is_certain = GetPrivateProfileInt(_T("ifCopyConfig"), _T("is_certain"),0, _T("./checkcopy.ini"));
	if(is_certain==0)
	{   //maliang����֧�ָ��Ƶ���
		theApp.m_bSupportCopy=FALSE;
	}
	if(is_certain==1)
	{   //maliang��֧�ָ��Ƶ���
		theApp.m_bSupportCopy=TRUE;
	}

	/*20160711 ��ˬ������ʵʱǰ�ù����Ż�����4���֣��޸ĺ�ʵʱ����δʵ��
	�ֱ���IncrementSystemDlg.cpp�ĵ�167��466��894�Լ�PersonalDefineDlg.cpp��65��
	��ˬ���ϵ����ʱ������ѡ���벻ѡ�����������ʵʱִ������Ӧ��SetWindowPos��䣬��������ǰ��״̬�ı�Ϊ��ǰ��״̬��������Ч
	��ˬ�����ɲ�ǰ��״̬�ı�Ϊǰ��״̬ʱ������ܹ�ִ�У�������������Ч���轫������С�����������룬�����ʵ��ǰ��
	*/

	//����>20160711 ��ˬ ���õ����ö� part1 ��ʼ������
	SetWindowPos (NULL,0, 0,theApp.m_nWidth,theApp.m_nHeight,SWP_NOACTIVATE | SWP_NOZORDER);//SetWindowPos������ʼ��
	//2016.3.15,maliang:��ȡdisplay.ini�ļ������ȡֵΪ��0����ȡ��ǰ�ã�ȡ��1����������ǰ��ʾ
	int is_shown = GetPrivateProfileInt(_T("displayConfig"), _T("is_shown"),0, _T("./display.ini"));
	/*2016.3.16��maliang����ȫ�ֱ���m_bCancelTop��ֵ*/
	theApp.m_bSupportTop=is_shown;
	if(is_shown==0)
	{ 
		::SetWindowPos(m_hWnd,HWND_NOTOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);//20160711 ��ˬ ��һ������ʹ���¾��еľ��Ҳ��
		//::SetWindowPos(this->GetSafeHwnd(),HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
	}
	else
	{   
		::SetWindowPos(m_hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);//20160711 ��ˬ
	}	
	//����>��ˬ������ǰ��part1�޸Ľ�����ʵʱδʵ��

	//����>20160713 ��ˬ����ȡ�Ƿ񰴼����벶׽�������ļ����ָ�Ϊ��һ������
	int is_NoCapture = GetPrivateProfileInt(_T("NoCaptureConfig"), _T("is_NoCapture"),0, _T("./NoCapture.ini"));
	if(is_NoCapture == 1)
	{   //��ˬ��֧�ּ��̺��벶׽
		theApp.m_bSupportNoCapture = TRUE;
	}
	if(is_NoCapture == 0)
	{   //��ˬ����֧�ּ��̺��벶׽
		theApp.m_bSupportNoCapture = FALSE;
	}
	//����>20160713 ��ˬ ��ȡ�Ƿ񰴼����벶׽�������ļ��޸Ľ���

	//����>20160725 ��ˬ����ȡ�Ƿ����趨ʱ���ڲ���ѯ���빦�ܣ��ָ�Ϊ��һ������
	int is_TimeSetting = GetPrivateProfileInt(_T("TimeSettingConfig"), _T("is_TimeSetting"),0, _T("./TimeSetting.ini"));
	if(is_TimeSetting == 1)
	{   //��ˬ��֧�ֿ����趨ʱ���ڲ���ѯ���빦��
		theApp.m_bSupportTimeSetting = TRUE;
		WritePrivateProfileString(_T("NumTimeConfig"), _T("NumTime"),_T(""), _T("./NumTime.ini"));//��ˬ ���������ļ�NumTime
		theApp.SliderPos = GetPrivateProfileInt(_T("SliderTimePosConfig"), _T("is_SliderTimePos"),1, _T("./SliderTimePos.ini"));//ȡ���������趨��ʱ��
	}
	if(is_TimeSetting == 0)
	{   //��ˬ����֧�ֿ����趨ʱ���ڲ���ѯ���빦��
		theApp.m_bSupportTimeSetting = FALSE;
	}
	//����>20160725 ��ˬ ��ȡ�Ƿ����趨ʱ���ڲ���ѯ���빦���޸Ľ���

	HINSTANCE hInst = AfxGetInstanceHandle();
	HBITMAP hbmpTitle = NULL;
	HICON   hIcon = AfxGetApp()->LoadIcon(IDI_ICON4);

	// ����ͼƬ
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
	::SetTimer(m_hWnd,TIME_IDENTIFIER_CLEAR,24 * 60 * 60 * 1000,NULL);	// ���ϵͳ�������У���ÿ������һ����־�ļ���
	/**
	���� 2016 06 22 ������ʱ�رչ��ܣ�ʡ��Ҫ��ÿ��ص�¼����������һ���24��������
	ÿ��0�㣬׼ʱ�رյ����ͻ��ˡ�
	**/
	SYSTEMTIME st;
	GetLocalTime(&st);
	st.wHour;// - Сʱ
	st.wMinute;// - ��
	st.wSecond;// - ��
	//::SetTimer(m_hWnd,TIME_IDENTIFIER_CLOSE_DLG,10*1000,NULL);
	::SetTimer(m_hWnd,TIME_IDENTIFIER_CLOSE_DLG,24*60*60*1000-(st.wHour*60*60*1000+st.wMinute*60*1000+st.wSecond*1000),NULL);
	//------>>�޸Ľ���

	EnablePrivilege(SE_DEBUG_NAME,TRUE); //���������Ȩ�� 
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
		_T("��ϸ������ƽ̨\n��������Ҽ�������Ӧ�ó���Ŀ�ݲ˵���"),  // tooltip
		hIcon,
		IDR_POPUP_MENU,                  // ID of tray icon
		FALSE,
		_T("˫������ϵͳ����\n����Ҽ��ر�ϵͳ"), // balloon tip
		_T("ϵͳ��ʾ"),               // balloon title
		NIIF_INFO,                    // balloon icon
		20 ))                            // balloon timeout
	{
		return -1;
	}

	m_TrayIcon.SetMenuDefaultItem(0, TRUE);

	SetWindowText(theApp.m_strTitle);

	m_hWndNext = SetClipboardViewer(); //�����ǵĳ�����ӵ�������۲���

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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


// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CIncrementSystemDlg::OnPaint()
{
	CDHtmlDialog::OnPaint();
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��

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
		// IDC_STATIC��Ϊ͸��
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
	//��Ӧ�Ͳο����� ���� 20160622�޸�����
	if(nIDEvent == TIME_IDENTIFIER_CLOSE_DLG)
	{
		CString strURL = _T("");
		strURL.Format(_T("%sloginId=%s"),theApp.m_strLogoffURL,theApp.m_strUser);

		m_ctlExplorer.Navigate2(&CComVariant(strURL),NULL,NULL,NULL,NULL);

		m_TrayIcon.RemoveIcon();	

		ChangeClipboardChain(m_hWndNext);

		CDHtmlDialog::OnCancel();
	}
	//------>>�޸����

	CDHtmlDialog::OnTimer(nIDEvent);
}

void CIncrementSystemDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// ��Ҫͨ���жϵ���λ�����ж��ǵ�����ݲ˵������ƶ����塣
	// �������д�������Ϣ��ȡ������Ҫ�º������������ֿ�������
	// PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y)); 

	CRect rect;
	CRect rtSystem;
	CRect rtMinimize;
	CRect rtClose;
	CPoint ScreenPoint = point;
	int	  size = GetSystemMetrics(SM_CYSIZE);
	int   space = 2;

	//��ȡ��������С
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
		// ģ��ϵͳ�˵�
		CMenu  menu;
		CMenu* popup= NULL;
		CPoint point;
		CBitmap bitmap1;
		CBitmap bitmap2;
		CBitmap bitmap3;
		// ����λͼ
		bitmap1.LoadBitmap(IDB_BITMAP1);
		bitmap2.LoadBitmap(IDB_BITMAP2);
		bitmap3.LoadBitmap(IDB_BITMAP3);
		// ����IDΪIDR_POPUP_SYSTEM�Ĳ˵�
		menu.LoadMenu(IDR_POPUP_SYSTEM);

		// ��ȡ�����˵��ĵ�һ���Ӳ˵���ָ��
		popup= menu.GetSubMenu(0);

		ClientToScreen(&ScreenPoint); 

		//maliang:Ϊ�˵��ĵ�һ�����ĺ͵���������ͼ��
		popup->SetMenuItemBitmaps(0,MF_BYPOSITION,&bitmap1,&bitmap1);
		popup->SetMenuItemBitmaps(3,MF_BYPOSITION,&bitmap3,&bitmap3);
		popup->SetMenuItemBitmaps(4,MF_BYPOSITION,&bitmap2,&bitmap2);

		// ��ʾ�����˵�
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
	//ע��֮ǰ����
	/*m_bManual = TRUE;
	m_TrayIcon.StopAnimation();
	ShowWindow(SW_NORMAL);
	::SetWindowPos(this->GetSafeHwnd(),NULL,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE); */
	/*2016.3.16��maliang:���ͼ�꣬�ж�ȫ�ֱ���m_bSupportTop����Ϊ1��ǰ�ã�Ϊ0��ǰ��*/

	//����>20160711 ��ˬ ����ǰ�� part2 
	//��ˬ��ʵʱ��is_shown��ֵ��ֵ��ȫ�ֱ���theApp.m_bSupportTop��Ϊ1��ǰ�ã�Ϊ0��ǰ��
	theApp.m_bSupportTop=GetPrivateProfileInt(_T("displayConfig"), _T("is_shown"),0, _T("./display.ini"));//��ˬ

	if(theApp.m_bSupportTop==0)
	{
		m_bManual = TRUE;
		m_TrayIcon.StopAnimation();
		ShowWindow(SW_NORMAL);
		::SetWindowPos(this->GetSafeHwnd(),HWND_NOTOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);//20160711 ��ˬ ����������滻Ϊm_hWnd
	}
	else 
	{
		m_bManual = TRUE;
		m_TrayIcon.StopAnimation();
		ShowWindow(SW_NORMAL);
		::SetWindowPos(this->GetSafeHwnd(),HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE); //20160711 ��ˬ
	}
}
//����>��ˬ������ǰ��part2�޸Ľ�����ʵʱδʵ��

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
		if (MessageBox(_T("��ȷ��Ҫ�˳���ϸ������ƽ̨��"),theApp.m_strTitle, MB_YESNO + MB_ICONQUESTION) == IDNO) return;
	}

	strURL.Format(_T("%sloginId=%s"),theApp.m_strLogoffURL,theApp.m_strUser);
	m_ctlExplorer.Navigate2(&CComVariant(strURL),NULL,NULL,NULL,NULL);

	m_TrayIcon.RemoveIcon();	

	ChangeClipboardChain(m_hWndNext);

	CDHtmlDialog::OnCancel();

	ExitProcess(0);	//20160729 ��ˬ �ر������������
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

	// �򿪼�����
	if ( OpenClipboard() )
	{
		char*	buffer = NULL;
		HANDLE	hData = NULL;
		CString strFromClipboard = _T("");
		CString strValidPhoneNo = _T("");

		hData = GetClipboardData(CF_TEXT); //��ȡ�������ڴ���׵�ַ
		buffer = (char*)GlobalLock(hData); //�����������ڴ�飬ָ��ָ����ڴ������
		strFromClipboard = buffer;

		GlobalUnlock(hData); //�������
		CloseClipboard(); //�رռ�����

		if (!m_bReadClipboard) //m_bReadClipboard��ֵΪfalse
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
			////2016/3/23:maliang:������ѯ������m_bSupportCopy��=FALSE���رռ�����
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
// ����:
//		���������Ȩ��
// �������:
//		lpName��		[in]��Ȩ����;
//		fEnable��	[in]��Ȩ�޿���.
// ����ֵ:
//		TRUE���ɹ���
//		FALSE��ʧ��.
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
�й���ͨ��
�š����Σ�130  131 132 155 156 176 185 186
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
		ch = strValidPhoneNo.GetAt(i); //GetAtȡ�ַ����е�i���ַ�
		if ((ch < '0') || (ch > '9')) 
		{
			SaveLog(theApp.m_pszAppName,_T(__FILE__),__LINE__,_T("The acquired data is not a valid phone number."));
			return FALSE;
		}
	}

	//Cstring����Compare�������ȽϽ����ͬ�򷵻�0
	if (strPrefix.Compare(_T("130")) && strPrefix.Compare(_T("131")) 
		&& strPrefix.Compare(_T("132")) && strPrefix.Compare(_T("155")) 
		&& strPrefix.Compare(_T("156")) && strPrefix.Compare(_T("185")) 
		&& strPrefix.Compare(_T("186")) && strPrefix.Compare(_T("176"))) 
	{
		SaveLog(theApp.m_pszAppName,_T(__FILE__),__LINE__,_T("The acquired data is not a valid china unicom phone number."));
		return FALSE;
	}

	//����>20160726 ��ˬ �ж��Ƿ����趨ʱ���ڲ����к����ѯ�����޸�
	if (theApp.m_bSupportTimeSetting == TRUE)
	{
		//��ˬ ��ȡ��ǰϵͳʱ��
		SYSTEMTIME st;
		GetLocalTime(&st);
		st.wHour;// - Сʱ
		st.wMinute;// - ��
		st.wSecond;// - ��

		CString StrCurrentTime;//��ǰʱ��
		StrCurrentTime.Format(_T("%.2d%.2d%.2d"),st.wHour,st.wMinute,st.wSecond);
		
		//��ˬ ��ȡ�����ļ��е���ʷ������ʱ�䡱
		CString StrHistoryNumTime;
		GetPrivateProfileString(_T("NumTimeConfig"), _T("NumTime"),_T(""),StrHistoryNumTime.GetBuffer(500*1024),500*1024, _T("./NumTime.ini"));
		StrHistoryNumTime.ReleaseBuffer();

		int Count = 0;
		int NumTimeFindStart = 0;
		int NumTimeFindPosition = 0;
		int IntCurrentTime = 0;
		int IntHistoryTime = 0;
		BOOL is_Num = FALSE;//ΪTRUEʱ��ʾ����ͬ����
		CStringArray AllHistoryNumTime;
		CString strHistoryTime = _T("");
		
		//���������ļ��еĺ���
		while(NumTimeFindPosition = StrHistoryNumTime.Find(';',NumTimeFindStart))
		{
			if(NumTimeFindPosition < 1)
			{
				break;
			}
			CString TempSingleNumTime = StrHistoryNumTime.Mid(NumTimeFindStart,NumTimeFindPosition-NumTimeFindStart);
			AllHistoryNumTime.Add(TempSingleNumTime);//��������ġ�����ʱ�䡱�����ַ�������
			
			NumTimeFindStart = NumTimeFindPosition + 1;
			NumTimeFindPosition = 0;	
			Count++;
		}
		//�ж������ļ����Ƿ�����ͬ����
		for(int i=0;i<=(Count-1);i++)
		{
			if(!strValidPhoneNo.Compare(AllHistoryNumTime.GetAt(i).Left(11)))
			{
				strHistoryTime = AllHistoryNumTime.GetAt(i).Right(6);
				is_Num = TRUE;
			}
		}
		//����ͬ����
		if(is_Num == TRUE)
		{
			IntCurrentTime = st.wHour*60*60+st.wMinute*60+st.wSecond;
			IntHistoryTime = _ttoi(strHistoryTime.Mid(0,2))*60*60+_ttoi(strHistoryTime.Mid(2,2))*60+_ttoi(strHistoryTime.Mid(4,2));
			if((IntCurrentTime-IntHistoryTime) <= ((theApp.SliderPos/2.0)*60))
			{
				return FALSE;//ͬһ�������ʱ��С�ڵ����趨ʱ�䣬�������������в�ѯ
			}
			else
			{
				//ͬһ�������ʱ������趨ʱ�䣬�滻ԭ����������Ӧ��ʱ�䣬��ִ�в�ѯ
				StrHistoryNumTime.Replace(strHistoryTime,StrCurrentTime);
				WritePrivateProfileString(_T("NumTimeConfig"), _T("NumTime"),StrHistoryNumTime, _T("./NumTime.ini"));
			}
		}
		else  //����ͬ����
		{
			//������+ʱ��д�������ļ�����ִ�в�ѯ
			StrHistoryNumTime = StrHistoryNumTime + strValidPhoneNo + StrCurrentTime + _T(";");
			WritePrivateProfileString(_T("NumTimeConfig"), _T("NumTime"),StrHistoryNumTime, _T("./NumTime.ini"));
		}
	}
	//����>20160726 ��ˬ �ж��Ƿ����趨ʱ���ڲ����к����ѯ�����޸Ľ���*/

	if (!bInput) return TRUE;

	dwTime = GetTickCount(); //GetTickCount����������󾭹��ĺ�����
	nSize = m_saHistory.GetCount(); //GetCount��ȡ������Ԫ�ظ���

	if (m_dwaHistory.GetCount() != nSize) 
	{
		SaveLog(theApp.m_pszAppName,_T(__FILE__),__LINE__,_T("The number of two array elements is not equal. Resetting ..."));
		m_saHistory.RemoveAll();//�����������
		m_dwaHistory.RemoveAll();

		m_saHistory.Add(strValidPhoneNo);//m_saHistory��CstringArray�ͣ�Add��ʾ������ĩβ���Ԫ��
		m_dwaHistory.Add(dwTime);
		return TRUE;
	}

	// ɾ���Ѿ���ʱ�ĺ��롣
	// �������ĺ��볬ʱ����ǰ��ĺ���һ����ʱ��
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

	nSize = m_saHistory.GetCount();		// �������¼��㳤��
	for (int i = 0; i < nSize; i ++)	// ʣ�µĶ�����ʱ
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

		//20160714 ��ˬ �ж�ȫ�ֱ���theApp.m_bSupportNoCaptureΪTRUEʱ��ִ���±߰����жϲ�ѯ����䣬��������
		if(theApp.m_bSupportNoCapture == FALSE)return 0;
		//20160714 ��ˬ �޸Ľ���

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

	if (pDisp == pWBDisp) // ҳ��������
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
		//maliang/3/24������������Ϊ��1��
		if (m_strPopupFlag == _T("1"))
			//maliang��ע��֮ǰ���룺
			//**start
			/*ShowWindow(SW_NORMAL);		
			::SetWindowPos(this->GetSafeHwnd(),HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE); */ //**end
			//maliang������´���**start
			//2016.3.16��maliang���ж�ȫ�ֱ���m_bSupportTop��ֵ����Ϊ1��ǰ�ã���Ϊ1��ȡ��ǰ��
		{   //maliang����֧�ֵ���ǰ��

			//����>20160711 ��ˬ ����ǰ�� part3
			//��ˬ��ʵʱ��is_shown��ֵ��ֵ��ȫ�ֱ���theApp.m_bSupportTop��Ϊ1��ǰ�ã�Ϊ0��ǰ��
			theApp.m_bSupportTop = GetPrivateProfileInt(_T("displayConfig"), _T("is_shown"),0, _T("./display.ini"));

			if(theApp.m_bSupportTop==0)
			{
				ShowWindow(SW_NORMAL);
				//::SwitchToThisWindow(this->GetSafeHwnd(),true);//20160729 ���� ǰ����ʾ
				::SetForegroundWindow(this->GetSafeHwnd());//20160729 ��ˬ ǰ����ʾ

				::SetWindowPos(this->GetSafeHwnd(),HWND_NOTOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE); //20160711 ��ˬ

				//maliang����ȡ�Ƿ�֧�ָ��Ƶ����������ļ��������ȡֵΪ��0��������رռ����壬Ϊ��1����ָ�������
				int is_certain = GetPrivateProfileInt(_T("ifCopyConfig"), _T("is_certain"),0, _T("./checkcopy.ini"));
				if(is_certain==0)
				{
					//maliang����֧�ָ��Ƶ���
					theApp.m_bSupportCopy=FALSE;
				}
				else
				{
					//maliang��֧�ָ��Ƶ���(�򿪼�����)
					theApp.m_bSupportCopy=TRUE;
				}	
			}
			//maliang��֧�ֵ���ǰ��
			if(theApp.m_bSupportTop==1)
			{
				ShowWindow(SW_NORMAL);
				::SetWindowPos(this->GetSafeHwnd(),HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE); //20160711 ��ˬ
				//��ˬ������ǰ��part4�޸���ɣ�ʵʱδʵ��

				//maliang/3/24����ȡ�Ƿ�֧�ָ��Ƶ����������ļ��������ȡֵΪ��0��������رռ����壬Ϊ��1����ָ�������
				int is_certain = GetPrivateProfileInt(_T("ifCopyConfig"), _T("is_certain"),0, _T("./checkcopy.ini"));
				if(is_certain==0)
				{
					//maliang����֧�ָ��Ƶ���
					theApp.m_bSupportCopy=FALSE;
				}
				else
				{ 
					//maliang��֧�ָ��Ƶ���(�򿪼�����)
					theApp.m_bSupportCopy=TRUE;
				}
			}
		}

		//maliang/3/24:����������Ϊ��0����
		else
		{
			//maliang����ȡ�Ƿ�֧�ָ��Ƶ����������ļ��������ȡֵΪ��0��������رռ����壬Ϊ��1����ָ�������
			int is_certain = GetPrivateProfileInt(_T("ifCopyConfig"), _T("is_certain"),0, _T("./checkcopy.ini"));
			if(is_certain==0)
			{
				//maliang����֧�ָ��Ƶ���
				theApp.m_bSupportCopy=FALSE;
			}
			else
			{
				//maliang��֧�ָ��Ƶ���(�򿪼�����)
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

	// ����һ������DC.
	dc.CreateCompatibleDC(pDC);

	// ���ȱ���ԭ�����豸����.
	pOldBitmap = dc.SelectObject(&bmp);

	// ȡ��Bitmap�������Ϣ
	bmp.GetBitmap(&bmpBitmap);
	bmpWidth = bmpBitmap.bmWidth;
	bmpHeight = bmpBitmap.bmHeight;

	// ��֤ͼƬ����.
	pDC->SetStretchBltMode(STRETCH_HALFTONE);

	// ��ʾͼƬ
	pDC->StretchBlt(rect.left,rect.top,rect.Width(),rect.Height(),&dc,0,0,bmpWidth,bmpHeight,SRCCOPY);

	// �ָ�ԭ�����豸����.
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
			m_bOffWork = TRUE;	// ���ڹ���ʱ����
			OnPopupExit();
		}
		else
		{
			m_bLogined = TRUE;
		}
	}
}
//maliang:����˵����Զ������õ���Ӧ����
void CIncrementSystemDlg::OnPopupSetting()
{
	//maliang:��ʾһ��ģ̬�Ի���
	CPersonalDefineDlg pd ;
	pd.DoModal();

}