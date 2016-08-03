// LogonDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IncrementSystem.h"
#include "LogonDlg.h"
#include "ViewVerDlg.h"
#include "BONCSocket.h"
#include "afxdialogex.h"

#include <afxhtml.h>
#include <afxinet.h>



// CLogonDlg dialog

IMPLEMENT_DYNAMIC(CLogonDlg, CDialogEx)

CLogonDlg::CLogonDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLogonDlg::IDD, pParent)
	, m_strUser(_T(""))
	, m_strPassword(_T(""))
	, m_strCode(_T(""))
	//IDC_RADIO_INTERNET|IDC_RADIO_INTERNAL|IDC_RADIO_OFFICE
	, m_nNetwork(2)//此处修改初始化登陆网络类型，具体详见630行case选择，INI_SECTION_OFFICE|INI_SECTION_INTERNAL|INI_SECTION_INTERNET;
	/*, m_bCopy(FALSE)*/
{
	m_strURL = _T("");
	m_strResetPwdURL = _T("");
	m_strLastUser = _T("");
	m_strLastPassword = _T("");
	m_bHasTimer = FALSE;
	m_bBeginLogon = FALSE;
	m_bCheckTelnet = FALSE;
	m_bCanValidate = FALSE;
	m_bAuthentication = TRUE;
	m_bRetry = FALSE;
}

CLogonDlg::~CLogonDlg()
{
	
}

void CLogonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXPLORER, m_ctlExplorer);
	DDX_Radio(pDX, IDC_RADIO_OFFICE, m_nNetwork);
	DDX_Text(pDX, IDC_USER, m_strUser);
	DDX_Text(pDX, IDC_PASSWORD, m_strPassword);
	DDX_Text(pDX, IDC_CODE, m_strCode);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_RESET_PWD, m_btnReset);
	DDX_Control(pDX, IDC_RESEND_CODE, m_btnResend);
	DDX_Control(pDX, IDC_TELNET, m_btnTelnet);
	DDX_Control(pDX, IDC_STATIC_STATUS, m_seStatus);
	/*DDX_Check(pDX, IDC_CHECK_COPY, m_bCopy);*/
}


BEGIN_MESSAGE_MAP(CLogonDlg, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_POPUP_VIEW, &CLogonDlg::OnPopupView)
	ON_COMMAND(ID_POPUP_CLOSE, &CLogonDlg::OnPopupClose)
	ON_BN_CLICKED(IDOK, &CLogonDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RESET_PWD, &CLogonDlg::OnBnClickedResetPwd)
	ON_BN_CLICKED(IDCANCEL, &CLogonDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_RESEND_CODE, &CLogonDlg::OnBnClickedResendCode)
	ON_BN_CLICKED(IDC_TELNET, &CLogonDlg::OnBnClickedTelnet)
	/*ON_BN_CLICKED(IDC_CHECK_COPY, &CLogonDlg::OnBnClickedCheckCopy)*/
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CLogonDlg, CDialogEx)
	ON_EVENT(CLogonDlg, IDC_EXPLORER, 259, CLogonDlg::DocumentCompleteExplorer, VTS_DISPATCH VTS_PVARIANT)
END_EVENTSINK_MAP()

// CLogonDlg message handlers

BOOL CLogonDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	HINSTANCE hInst = AfxGetInstanceHandle();
	HBITMAP hbmpLogon = NULL;

	m_ctlExplorer.SetWindowPos (NULL, 0, 0, 0, 0,SWP_NOACTIVATE | SWP_NOZORDER);

	// 载入图片
	hbmpLogon = (HBITMAP)::LoadImage(hInst,_T("Logon.bmp"), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION|LR_LOADFROMFILE);
	if(hbmpLogon != NULL) m_bmpLogon.Attach(hbmpLogon);

	m_btnOK.SetBitmap(IDB_LOGIN_NORMAL,IDB_LOGIN_SELECTED);
	m_btnCancel.SetBitmap(IDB_EXIT_NORMAL,IDB_EXIT_SELECTED);
	m_btnReset.SetBitmap(IDB_RESET_NORMAL,IDB_RESET_SELECTED);
	m_btnResend.SetBitmap(IDB_RESEND_NORMAL,IDB_RESEND_SELECTED);
	m_btnTelnet.SetBitmap(IDB_TELNET_NORMAL,IDB_TELNET_SELECTED);
	
	::SetTimer(m_hWnd,TIME_IDENTIFIER_AUTH,1000,NULL);

	m_bAuthentication = false;

	ReadParam(m_nNetwork);

	return TRUE;  // return TRUE unless you set the focus to a control

	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CLogonDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	CDialogEx::OnEraseBkgnd(pDC);

	CRect	rect;

	GetClientRect(&rect);
	ShowBitmap(pDC,m_bmpLogon,rect);

	return TRUE;
}

HBRUSH CLogonDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: Change any attributes of the DC here
	// 设为透明
	UINT uID = pWnd->GetDlgCtrlID();

	if (uID == IDC_RADIO_OFFICE || uID == IDC_RADIO_INTERNAL || uID == IDC_RADIO_INTERNET || uID == IDC_CHECK_COPY)
	{
		CRect rect;
		CDC*  dc = GetDC();

		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);

		pDC->SetBkMode(TRANSPARENT);
		pDC->BitBlt(0,0,rect.Width(),rect.Height(),dc,rect.left,rect.top,SRCCOPY);	//把父窗口背景图片先画到按钮上

		ReleaseDC(dc);

		return (HBRUSH)GetStockObject(NULL_BRUSH);
	}	
	else if (uID == IDC_STATIC_USER || uID == IDC_STATIC_PASSWORD || uID == IDC_STATIC_CODE)
	{
		pDC->SetBkMode(TRANSPARENT);

		return (HBRUSH)GetStockObject(NULL_BRUSH);
	}

	return hbr;
}

void CLogonDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == TIME_IDENTIFIER_AUTH)
	{
		if (m_bCanValidate)
		{
			if (m_nRemainSecond > 0)
			{
				CString str = _T("");

				str.Format(_T("剩余%d秒"),m_nRemainSecond--);
				::SetWindowText(::GetDlgItem(m_hWnd,IDC_RESEND_SECOND),str);
			}
			else
			{
				::ShowWindow(::GetDlgItem(m_hWnd,IDC_RESEND_CODE),SW_SHOW);
				::ShowWindow(::GetDlgItem(m_hWnd,IDC_RESEND_SECOND),SW_HIDE);

				::SetWindowText(::GetDlgItem(m_hWnd,IDC_RESEND_SECOND),_T(""));
			}
		}

	}
	else if (nIDEvent == TIME_IDENTIFIER_LOGON)
	{
		if (m_bBeginLogon && !m_bCheckTelnet)
		{
			m_bBeginLogon = FALSE;
			m_ctlExplorer.Stop();
			m_seStatus.SetTextColor(RGB(255,0,0));
			m_seStatus.SetWindowText(_T("登录超时！请检查网络是否正常或者系统配置是否正确。"));
			MessageBox(_T("登录超时！请检查网络是否正常或者系统配置是否正确。"),theApp.m_strTitle, MB_OK + MB_ICONEXCLAMATION);
		}
	}



	CDialogEx::OnTimer(nIDEvent);
}

BOOL CLogonDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE; 
	//if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE; 

	return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL CLogonDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: Add your message handler code here and/or call default
	if (m_bBeginLogon)
	{
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
        return TRUE;
	}

	return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
}

void CLogonDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// 实现无标题栏对话框的拖动
	//PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y)); 

	//CDialogEx::OnLButtonDown(nFlags, point);
	CRect rtTitle;
	CPoint ScreenPoint = point;

	int x = point.x;
	int y = point.y;

	//获取标题栏大小
	rtTitle.left = GetSystemMetrics(SM_CXFRAME);
	rtTitle.top = GetSystemMetrics(SM_CYFRAME);
	rtTitle.right = rtTitle.left + GetSystemMetrics(SM_CYSIZE);
	rtTitle.bottom = rtTitle.top + GetSystemMetrics(SM_CYSIZE);

	if (rtTitle.PtInRect(point))
	{
		// 模拟系统菜单
		CMenu  menu;
		CMenu* popup= NULL;
		CPoint point;
		CBitmap bitmap1;
		CBitmap bitmap2;

		// 加载位图
		bitmap1.LoadBitmap(IDB_BITMAP1);
		bitmap2.LoadBitmap(IDB_BITMAP2);

		// 加载ID为IDR_POPUP_SYSTEM的菜单
		menu.LoadMenu(IDR_POPUP_SYSTEM);

		// 获取弹出菜单的第一层子菜单的指针
		popup= menu.GetSubMenu(0);

		ClientToScreen(&ScreenPoint); 

		// 获取鼠标的坐标
		//GetCursorPos(&point);

		//为菜单的第一和第五项设置图标
		popup->SetMenuItemBitmaps(0,MF_BYPOSITION,&bitmap1,&bitmap1);
		popup->SetMenuItemBitmaps(4,MF_BYPOSITION,&bitmap2,&bitmap2);

		// 显示弹出菜单
		popup->TrackPopupMenu(TPM_LEFTALIGN,ScreenPoint.x,ScreenPoint.y,this); 

		menu.DestroyMenu(); 
	}
	else
	{
		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y)); 
	}
}

void CLogonDlg::OnPopupView()
{
	// TODO: Add your command handler code here
	CViewVerDlg dlg;

	dlg.DoModal();
}

void CLogonDlg::OnPopupClose()
{
	// TODO: Add your command handler code here
	OnBnClickedCancel();
}

void CLogonDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData();

	m_bCheckTelnet = FALSE;
	if (!ReadParam(m_nNetwork)) return;

	m_strUser.Trim();
	if (m_strUser.IsEmpty())
	{
		MessageBox(_T("登录工号不能为空！请输入您的登录工号。"),theApp.m_strTitle, MB_OK + MB_ICONEXCLAMATION);
		return;
	}

	if (!m_bHasTimer)
	{
		::SetTimer(m_hWnd,TIME_IDENTIFIER_LOGON,theApp.m_dwTimeout * 1000,NULL);
	}

	m_bBeginLogon = TRUE;
	m_bHasTimer = TRUE;

	m_seStatus.SetTextColor(RGB(0,0,255));
	m_seStatus.SetWindowText(_T("正在登录精细化助销平台服务 ..."));

	ConverSpecialChar(m_strUser);
	ConverSpecialChar(m_strPassword);

	if (m_strLastUser.Compare(m_strUser) || m_strLastPassword.Compare(m_strPassword))
	{
		m_bCanValidate = FALSE;
	}

	m_strLastUser = m_strUser;
	m_strLastPassword = m_strPassword;

	if (m_bCanValidate)
	{
		if (m_bRetry)
		{
			m_bRetry = FALSE;
			m_strURL.Format(_T("%susername=%s&password=%s&mac=%s&reSend=1"),theApp.m_strLogonURL,m_strUser,m_strPassword,theApp.m_strMAC);
		}
		else
		{
			m_strURL.Format(_T("%susername=%s&smsnum=%s"),theApp.m_strAuthURL,m_strUser,m_strCode);
		}
	}
	else
	{
		m_strURL.Format(_T("%susername=%s&password=%s&mac=%s&reSend=0"),theApp.m_strLogonURL,m_strUser,m_strPassword,theApp.m_strMAC);
	}

	m_ctlExplorer.Navigate2(&CComVariant(m_strURL),NULL,NULL,NULL,NULL);

}

void CLogonDlg::OnBnClickedResetPwd()
{
	// TODO: Add your control notification handler code here
	CString strURL = _T("");
	SHELLEXECUTEINFO sei = {0};

	UpdateData();

	if (!ReadParam(m_nNetwork)) return;

	m_bBeginLogon = FALSE;
	strURL.Format(_T("%sloginId=%s"),theApp.m_strResetPasswordURL,m_strUser);

	sei.cbSize = sizeof (SHELLEXECUTEINFO);
	sei.lpVerb = _T("Open");
	sei.lpFile = strURL;
	sei.nShow = SW_SHOW;
	sei.hInstApp = AfxGetInstanceHandle();

	ShellExecuteEx(&sei);
}

void CLogonDlg::OnBnClickedResendCode()
{
	// TODO: Add your control notification handler code here
	m_bRetry = TRUE;
	OnBnClickedOk();
}

void CLogonDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	if (MessageBox(_T("您确定要退出登录吗？"),theApp.m_strTitle, MB_YESNO + MB_ICONQUESTION) == IDNO) return;

	CDialogEx::OnCancel();
}

void CLogonDlg::OnBnClickedTelnet()
{
	// TODO: Add your control notification handler code here	
	m_bCheckTelnet = TRUE;
	AfxBeginThread(TelnetThread,this);
}

void CLogonDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	GetCursorPos(&m_pntMouse);
	
	CDialogEx::OnMouseMove(nFlags, point);
}


BOOL CLogonDlg::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	::KillTimer(m_hWnd,TIME_IDENTIFIER_LOGON);
	::KillTimer(m_hWnd,TIME_IDENTIFIER_AUTH);

	if (m_bmpLogon.m_hObject != NULL)	
	{
		m_bmpLogon.Detach();
		m_bmpLogon.DeleteObject();
	}

	return CDialogEx::DestroyWindow();
}

void CLogonDlg::ShowBitmap(CDC* pDC, CBitmap &bmp, CRect &rect)
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


void CLogonDlg::DocumentCompleteExplorer(LPDISPATCH pDisp, VARIANT* URL)
{
	// TODO: Add your message handler code here
	HRESULT hr = S_FALSE;
	CComPtr<IDispatch> pDocDisp(m_ctlExplorer.get_Document());   
	CComPtr<IHTMLDocument2> spDocument;
	CComPtr<IHTMLElement> pElem;
	CComPtr<IHTMLElement> pElem2;
	CString strBody = _T("");
	CString strResponse = _T("");
	CString strURL = _T("");
	CString strMsg = _T("");
	BSTR bstrBody; 
	
	ASSERT((IDispatch*)pDocDisp);   

	if (URL->vt == VT_BSTR)
	{
		strURL = URL->bstrVal;
		strURL = strURL.Left(strURL.Find(_T("?")) + 1);
	}

	m_bBeginLogon = FALSE;
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

	strResponse = GetResponse(strBody);
	if (strResponse.IsEmpty())
	{
		strMsg = _T("无法登录认证服务器！");
		m_seStatus.SetTextColor(RGB(255,0,0));
		m_seStatus.SetWindowText(strMsg);
		MessageBox(strMsg,theApp.m_strTitle, MB_OK + MB_ICONEXCLAMATION);
		return;
	}

	if (strURL.CompareNoCase(theApp.m_strLogonURL) == 0)
	{
		::ShowWindow(::GetDlgItem(m_hWnd,IDC_STATIC_CODE),SW_HIDE);
		::ShowWindow(::GetDlgItem(m_hWnd,IDC_CODE),SW_HIDE);
		::ShowWindow(::GetDlgItem(m_hWnd,IDC_RESEND_CODE),SW_HIDE);
		::ShowWindow(::GetDlgItem(m_hWnd,IDC_RESEND_SECOND),SW_HIDE);

		if (strResponse.Compare(_T("0")) == 0)
		{
			strMsg = _T("登录失败！请检查您输入的【登录工号】和【登录口令】是否正确。");
			m_seStatus.SetTextColor(RGB(255,0,0));
			m_seStatus.SetWindowText(strMsg);
			MessageBox(strMsg,theApp.m_strTitle, MB_OK + MB_ICONEXCLAMATION);
			return;
		}
		//else if (strResponse.Compare(_T("1")) == 0)
		//{
		//	strMsg.Format(_T("不在登录时间范围内，请在%s登录本系统。"),theApp.m_strWorkingTime);
		//	m_seStatus.SetTextColor(RGB(255,0,0));
		//	m_seStatus.SetWindowText(strMsg);
		//	MessageBox(strMsg,theApp.m_strTitle, MB_OK + MB_ICONEXCLAMATION);
		//	return;
		//}

		if (!m_bAuthentication)
		{
			theApp.m_strSessionId = strResponse;
			CDialogEx::OnOK();
			return;
		}

		if (strResponse.Compare(_T("2")) == 0)
		{
			strMsg = _T("您尚未注册手机号！");
			m_seStatus.SetTextColor(RGB(255,0,0));
			m_seStatus.SetWindowText(strMsg);
			MessageBox(strMsg,theApp.m_strTitle, MB_OK + MB_ICONEXCLAMATION);
			return;
		}

		m_bCanValidate = TRUE;
		m_nRemainSecond = CODE_EXP_TIME;

		::EnableWindow(::GetDlgItem(m_hWnd,IDC_USER),FALSE);
		::EnableWindow(::GetDlgItem(m_hWnd,IDC_PASSWORD),FALSE);
		::EnableWindow(::GetDlgItem(m_hWnd,IDC_RESET_PWD),FALSE);

		::ShowWindow(::GetDlgItem(m_hWnd,IDC_STATIC_CODE),SW_SHOW);
		::ShowWindow(::GetDlgItem(m_hWnd,IDC_CODE),SW_SHOW);
		::ShowWindow(::GetDlgItem(m_hWnd,IDC_RESEND_SECOND),SW_SHOW);
	}
	else if (strURL.CompareNoCase(theApp.m_strAuthURL) == 0)
	{
		if (strResponse.Compare(_T("2")) == 0)
		{
			strMsg = _T("您输入的验证码不正确！请重新输入。");
			m_seStatus.SetTextColor(RGB(255,0,0));
			m_seStatus.SetWindowText(strMsg);
			MessageBox(strMsg,theApp.m_strTitle, MB_OK + MB_ICONEXCLAMATION);
			return;
		}
		else if (strResponse.Compare(_T("9")) == 0)
		{
			strMsg = _T("您输入的验证码已经过期！");
			m_seStatus.SetTextColor(RGB(255,0,0));
			m_seStatus.SetWindowText(strMsg);
			MessageBox(strMsg,theApp.m_strTitle, MB_OK + MB_ICONEXCLAMATION);

			::EnableWindow(::GetDlgItem(m_hWnd,IDC_USER),TRUE);
			::EnableWindow(::GetDlgItem(m_hWnd,IDC_PASSWORD),TRUE);
			::EnableWindow(::GetDlgItem(m_hWnd,IDC_RESET_PWD),TRUE);

			return;
		}

		theApp.m_strSessionId = strResponse;
		CDialogEx::OnOK();
	}
}

CString	CLogonDlg::GetResponse(CString strBody)
{
	CString strResponse = _T("");
	CString strBegin = _T("<body>");
	CString strEnd = _T("</body>");
	CString strTemp = strBody;
	int		nStart = 0;
	int		nPos = -1;

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

	return strResponse;
}

BOOL CLogonDlg::ReadParam(int nNetwork)
{
	CString strSection = _T("");

	switch (nNetwork)
	{
	case 0:
		strSection = INI_SECTION_OFFICE;
		break;
	case 1:
		strSection = INI_SECTION_INTERNAL;
		break;
	case 2:
		strSection = INI_SECTION_INTERNET;
		break;
	default:
		ASSERT(FALSE);
	}

	GetPrivateProfileString(strSection,INI_KEY_LOGON,NULL,theApp.m_strLogonURL.GetBuffer(MAX_PATH),MAX_PATH,INI_APP_FILE);
	theApp.m_strLogonURL.ReleaseBuffer();

	GetPrivateProfileString(strSection,INI_KEY_AUTHENTICATION,NULL,theApp.m_strAuthURL.GetBuffer(MAX_PATH),MAX_PATH,INI_APP_FILE);
	theApp.m_strAuthURL.ReleaseBuffer();

	GetPrivateProfileString(strSection,INI_KEY_MAIN1,NULL,theApp.m_strMainURL1.GetBuffer(MAX_PATH),MAX_PATH,INI_APP_FILE);
	theApp.m_strMainURL1.ReleaseBuffer();

	GetPrivateProfileString(strSection,INI_KEY_PHONE_TYPE,NULL,theApp.m_strPhoneTypeURL.GetBuffer(MAX_PATH),MAX_PATH,INI_APP_FILE);
	theApp.m_strPhoneTypeURL.ReleaseBuffer();

	GetPrivateProfileString(strSection,INI_KEY_PHONE,NULL,theApp.m_strPhoneURL.GetBuffer(MAX_PATH),MAX_PATH,INI_APP_FILE);
	theApp.m_strPhoneURL.ReleaseBuffer();

	GetPrivateProfileString(strSection,INI_KEY_POPUP,NULL,theApp.m_strPopupURL.GetBuffer(MAX_PATH),MAX_PATH,INI_APP_FILE);
	theApp.m_strPopupURL.ReleaseBuffer();

	GetPrivateProfileString(strSection,INI_KEY_HEARTBEAT,NULL,theApp.m_strHeartbeatURL.GetBuffer(MAX_PATH),MAX_PATH,INI_APP_FILE);
	theApp.m_strHeartbeatURL.ReleaseBuffer();

	GetPrivateProfileString(strSection,INI_KEY_RESET_PWD,NULL,theApp.m_strResetPasswordURL.GetBuffer(MAX_PATH),MAX_PATH,INI_APP_FILE);
	theApp.m_strResetPasswordURL.ReleaseBuffer();

	GetPrivateProfileString(strSection,INI_KEY_LOGOFF,NULL,theApp.m_strLogoffURL.GetBuffer(MAX_PATH),MAX_PATH,INI_APP_FILE);
	theApp.m_strLogoffURL.ReleaseBuffer();

	if (theApp.m_strLogonURL.IsEmpty() || theApp.m_strResetPasswordURL.IsEmpty() || theApp.m_strLogoffURL.IsEmpty() || theApp.m_strMainURL1.IsEmpty() || theApp.m_strPhoneTypeURL.IsEmpty() || theApp.m_strPhoneURL.IsEmpty() || theApp.m_strHeartbeatURL.IsEmpty())
	{
		MessageBox(_T("精细化助销平台无法启动！\n请检查系统配置是否正确，或直接与应用程序供应商进行联系。"),theApp.m_strTitle,MB_OK + MB_ICONSTOP);
		return FALSE;
	}

	if (m_bAuthentication)
	{
		if (theApp.m_strAuthURL.IsEmpty())
		{
			MessageBox(_T("精细化助销平台无法启动！\n验证码使用的服务地址尚未配置，请与应用程序供应商进行联系。"),theApp.m_strTitle,MB_OK + MB_ICONSTOP);
			return FALSE;
		}
	}

	theApp.MakeSureQuestionMarkExist(theApp.m_strLogonURL);
	theApp.MakeSureQuestionMarkExist(theApp.m_strAuthURL);
	theApp.MakeSureQuestionMarkExist(theApp.m_strResetPasswordURL);
	theApp.MakeSureQuestionMarkExist(theApp.m_strLogoffURL);
	theApp.MakeSureQuestionMarkExist(theApp.m_strMainURL1);
	theApp.MakeSureQuestionMarkExist(theApp.m_strPhoneTypeURL);
	theApp.MakeSureQuestionMarkExist(theApp.m_strPhoneURL);

	return TRUE;
}

void CLogonDlg::ConverSpecialChar(CString &str)
{
	str.Replace(_T("%"), _T("%25"));
	str.Replace(_T("+"), _T("%2B"));
	str.Replace(_T(" "), _T("%20"));
	str.Replace(_T("/"), _T("%2F"));
	str.Replace(_T("?"), _T("%3F"));
	str.Replace(_T("#"), _T("%23"));
	str.Replace(_T("&"), _T("%26"));
	str.Replace(_T("="), _T("%3D"));
}


UINT TelnetThread(LPVOID lpParam)
{
	CLogonDlg*  pMe = static_cast<CLogonDlg*>(lpParam);
	CBONCSocket		sock;
	CString			strServer = _T("");	
	CString			strObject = _T("");
	INTERNET_PORT	wPort = 0;
	DWORD			dwType = 0;
	BOOL			bResult = FALSE;
	
	if (pMe == NULL)  return 1;

	pMe->m_bBeginLogon = TRUE;

	pMe->m_seStatus.SetTextColor(RGB(0,0,255));
	pMe->m_seStatus.SetWindowText(_T("正在检查精细化助销平台服务是否可用 ..."));

	AfxParseURL(theApp.m_strLogonURL, dwType, strServer, strObject, wPort);

	if (sock.Create())
	{
		bResult = sock.Connect(strServer,wPort);
	}

	sock.Close();
	pMe->m_bBeginLogon = FALSE;
	SetCursorPos(pMe->m_pntMouse.x,pMe->m_pntMouse.y);

	if (bResult)
	{
		pMe->m_seStatus.SetWindowText(_T("精细化助销平台服务端口可用！"));
	}
	else
	{
		pMe->m_seStatus.SetTextColor(RGB(255,0,0));
		pMe->m_seStatus.SetWindowText(_T("精细化助销平台服务不可用！"));
	}
	
	return 0;
}


