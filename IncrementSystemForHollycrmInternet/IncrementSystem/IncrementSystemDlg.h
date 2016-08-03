
// IncrementSystemDlg.h : ͷ�ļ�
//

#pragma once
#include "explorer.h"
#include "afxwin.h"

#include "SystemTray.h"

#define	WM_ICON_NOTIFY			WM_APP + 10

// CIncrementSystemDlg �Ի���
class CIncrementSystemDlg : public CDHtmlDialog
{
// ����
public:
	CIncrementSystemDlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CIncrementSystemDlg();

// �Ի�������
	enum { IDD = IDD_INCREMENTSYSTEM_DIALOG, IDH = IDR_HTML_INCREMENTSYSTEM_DIALOG };
	CExplorer m_ctlExplorer;

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	HRESULT OnHideWindow(IHTMLElement *pElement);

public:
	BOOL	NeedQuery(CString strPhoneNo, CString &strValidPhoneNo, BOOL bInput = TRUE);
	void	ExecuteQuery(CString &strPhoneNo, BOOL bInput = TRUE);
	BOOL	SaveLog(LPCTSTR lpszLogFile, LPCTSTR lpszSourceName, int nLine, LPCTSTR lpszText);
	void	DumpModuleVersion(LPCTSTR lpszModuleName);

protected:
	BOOL	IsDocumentComplete(LPDISPATCH pDisp);
	void	HandleQueryRequest(LPDISPATCH pDisp, LPCTSTR szUrl);
	void	ReadPhoneType(LPDISPATCH pDisp);
	void	ReadPopupFlag(LPDISPATCH pDisp);
	CString	GetResponse(CString strBody);
	BOOL	EnablePrivilege(LPCTSTR lpName, BOOL fEnable);
	void	ShowBitmap(CDC* pDC, CBitmap &bmp, CRect &rect);

public:
	CString			m_strType;
	CString			m_strQueryPhoneTypeURL;
	CString			m_strQueryPhoneURL;
	CString			m_strQueryPopupURL;
	CString			m_strPhoneNo;
	CString			m_strPopupFlag;
	CStringArray	m_saHistory;
	CDWordArray		m_dwaHistory;
	BOOL			m_bLogined;
	BOOL			m_bQuery;
	BOOL			m_bManual;
	BOOL			m_bReadClipboard;
	BOOL			m_bVisit;
	

// ʵ��
protected:
	CBitmap		m_bmpTitle;	
	HHOOK		m_hkKeyboard;
	CSystemTray	m_TrayIcon;
	BOOL		m_bOffWork;
	HWND		m_hWndNext;
		

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPopupClose();
	afx_msg void OnPopupShow();
	afx_msg void OnPopupHide();
	afx_msg void OnPopupView();
	afx_msg void OnPopupExit();
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnDrawClipboard();
	virtual void OnBeforeNavigate(LPDISPATCH pDisp, LPCTSTR szUrl);
	virtual void OnDocumentComplete(LPDISPATCH pDisp, LPCTSTR szUrl);
	afx_msg LRESULT OnUserIvrNotice(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	void DocumentCompleteExplorer(LPDISPATCH pDisp, VARIANT* URL);	
	afx_msg void OnPopupSetting();
	
};
