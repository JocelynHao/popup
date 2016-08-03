// ViewVerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IncrementSystem.h"
#include "ViewVerDlg.h"
#include "afxdialogex.h"


// CViewVerDlg dialog

IMPLEMENT_DYNAMIC(CViewVerDlg, CDialogEx)

CViewVerDlg::CViewVerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CViewVerDlg::IDD, pParent)
{

}

CViewVerDlg::~CViewVerDlg()
{
}

void CViewVerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_VER, m_lvVer);
}


BEGIN_MESSAGE_MAP(CViewVerDlg, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_VER, &CViewVerDlg::OnLvnItemchangedListVer)
END_MESSAGE_MAP()


// CViewVerDlg message handlers


BOOL CViewVerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	LoadModuleVer();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CViewVerDlg::LoadModuleVer(void)
{
	LV_COLUMN	lvColumn = {0};
	LV_ITEM		lvItem = {0};
	CString		strFileVer = _T("");
	int			nRow = 0;
	CRect		rect;

	m_lvVer.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_lvVer.GetWindowRect(&rect);

	// 列头
	lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvColumn.fmt = LVCFMT_LEFT;

	lvColumn.iSubItem = 0;
	lvColumn.pszText = _T("模块");
	lvColumn.cx = 3 * rect.Width() / 5;
	m_lvVer.InsertColumn(0,&lvColumn);

	lvColumn.iSubItem = 1;
	lvColumn.pszText = _T("版本");
	lvColumn.cx = 2 * rect.Width() / 5;
	m_lvVer.InsertColumn(1,&lvColumn);

	// 行
	lvItem.mask = LVIF_TEXT;   

	lvItem.iItem = nRow;
	lvItem.iSubItem = 0;
	lvItem.pszText = _T("主模块");

	strFileVer = ReadLocalFileVer(_T("IncrementSystem.exe"));

	m_lvVer.InsertItem(&lvItem);
	m_lvVer.SetItemText(nRow,1,strFileVer);
	nRow++;

	lvItem.iItem = nRow;
	lvItem.iSubItem = 0;
	lvItem.pszText = _T("升级模块");

	strFileVer = ReadLocalFileVer(_T("Update.exe"));

	m_lvVer.InsertItem(&lvItem);
	m_lvVer.SetItemText(nRow,1,strFileVer);
	nRow++;

	lvItem.iItem = nRow;
	lvItem.iSubItem = 0;
	lvItem.pszText = _T("配置信息");

	strFileVer = ReadLocalFileVer(_T("IncrementSystem.ini"));

	m_lvVer.InsertItem(&lvItem);
	m_lvVer.SetItemText(nRow,1,strFileVer);
	nRow++;

	lvItem.iItem = nRow;
	lvItem.iSubItem = 0;
	lvItem.pszText = _T("登录图片");

	strFileVer = ReadLocalFileVer(_T("Logon.bmp"));

	m_lvVer.InsertItem(&lvItem);
	m_lvVer.SetItemText(nRow,1,strFileVer);
	nRow++;

	lvItem.iItem = nRow;
	lvItem.iSubItem = 0;
	lvItem.pszText = _T("有关闭按钮的主界面图片");

	strFileVer = ReadLocalFileVer(_T("Titlewithclose.bmp"));

	m_lvVer.InsertItem(&lvItem);
	m_lvVer.SetItemText(nRow,1,strFileVer);
	nRow++;

	lvItem.iItem = nRow;
	lvItem.iSubItem = 0;
	lvItem.pszText = _T("没有关闭按钮的主界面图片");

	strFileVer = ReadLocalFileVer(_T("TitleWithoutClose.bmp"));

	m_lvVer.InsertItem(&lvItem);
	m_lvVer.SetItemText(nRow,1,strFileVer);
	nRow++;

	lvItem.iItem = nRow;
	lvItem.iSubItem = 0;
	lvItem.pszText = _T("自述文件");

	strFileVer = ReadLocalFileVer(_T("Readme.txt"));

	m_lvVer.InsertItem(&lvItem);
	m_lvVer.SetItemText(nRow,1,strFileVer);
	
	// 固定列宽,不能手动改变控件宽度 
	CHeaderCtrl*  pHeaderCtrl  =  (CHeaderCtrl*)m_lvVer.GetHeaderCtrl(); 
	pHeaderCtrl->EnableWindow(FALSE)  ; 
}

CString CViewVerDlg::ReadLocalFileVer(LPCTSTR lpszFileName)
{
	CString strFileVer = _T("");

	GetPrivateProfileString(INI_SECTION_VERSION,lpszFileName,NULL,strFileVer.GetBuffer(MAX_PATH + 1),MAX_PATH,theApp.m_strVerConfigFile);
	strFileVer.ReleaseBuffer();

	return strFileVer;
}

void CViewVerDlg::OnLvnItemchangedListVer(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}
