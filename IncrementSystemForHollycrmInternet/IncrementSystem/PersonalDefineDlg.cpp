// PersonalDefineDlg.cpp : implementation file
//
//maliang：PersonalDefineDlg.cpp :实现自定义复选框功能的类
#include "stdafx.h"
#include "IncrementSystem.h"
#include "PersonalDefineDlg.h"
#include "afxdialogex.h"
#include "BONCSocket.h"
#include "IncrementSystemDlg.h"

// CPersonalDefineDlg dialog

IMPLEMENT_DYNAMIC(CPersonalDefineDlg, CDialogEx)



	//maliang：初始化设置对话框
	BOOL CPersonalDefineDlg::OnInitDialog()
{   
	CDialog::OnInitDialog();

	/*maliang：读取是否前置的ini文件，并为checkbox赋初值，如果取不到值或没有ini文件，取值为0*/
	int is_shown = GetPrivateProfileInt(_T("displayConfig"), _T("is_shown"),0, _T("./display.ini"));
	CButton* pBtn = (CButton*)GetDlgItem(IDC_POPUP_SETTING);
	pBtn->SetCheck(is_shown);

	/*maliang：读取是否支持复制弹窗的ini文件，并为对应的checkbox赋初值*/
	int is_certain = GetPrivateProfileInt(_T("ifCopyConfig"), _T("is_certain"),0, _T("./checkcopy.ini"));
	CButton* pBtn1 = (CButton*)GetDlgItem(IDC_CHECK_COPY);
	pBtn1->SetCheck(is_certain);

	//——>20160714 郝爽：读取是否支持开启按键号码捕捉的ini文件，并为对应的checkbox赋初值
	int is_NoCapture = GetPrivateProfileInt(_T("NoCaptureConfig"), _T("is_NoCapture"),0, _T("./NoCapture.ini"));
	CButton* pBtn2 = (CButton*)GetDlgItem(IDC_CHECK_CAPTURE);
	pBtn2->SetCheck(is_NoCapture);
	//——>20160714 郝爽 读取是否支持开启按键号码捕捉的ini文件修改结束

	//——>20160725 郝爽：读取是否支持开启设定时间内不进行号码查询的ini文件，并为对应的checkbox赋初值
	int is_TimeSetting = GetPrivateProfileInt(_T("TimeSettingConfig"), _T("is_TimeSetting"),0, _T("./TimeSetting.ini"));
	CButton* pBtn3 = (CButton*)GetDlgItem(IDC_CHECK_TIMESETTING);
	pBtn3->SetCheck(is_TimeSetting);
	//——>20160725 郝爽 读取是否支持开启设定时间内不进行号码查询的ini文件修改结束

	//——>20160726 郝爽 滑动条初始化
	m_slider_timesetting.SetRange(1,60);//设置滑动范围
	m_slider_timesetting.SetTicFreq(1);//每1个单位画一刻度
	m_slider_timesetting.SetLineSize(1);
	m_slider_timesetting.SetPos(GetPrivateProfileInt(_T("SliderTimePosConfig"), _T("is_SliderTimePos"),1, _T("./SliderTimePos.ini")));//设置默认当前位置
	//——>20160726 郝爽 滑动条初始化结束

	//——>20160728 郝爽：设置滑动条等控件是否显示
	if(is_TimeSetting == 1)
	{
		GetDlgItem(IDC_SLIDER_TIMESETTING)-> EnableWindow(TRUE);//显示滑动条
		GetDlgItem(IDC_EDIT_TIMESETTINGSHOW)-> EnableWindow(TRUE);//显示编辑框
		GetDlgItem(IDC_STATIC_TIMESHOW)-> EnableWindow(TRUE);//显示“分钟”
		GetDlgItem(IDC_BUTTON_CONFIRM_TIME)-> EnableWindow(TRUE);//显示确定按钮
	}
	else
	{
		GetDlgItem(IDC_SLIDER_TIMESETTING)-> EnableWindow(FALSE);//不显示滑动条
		GetDlgItem(IDC_EDIT_TIMESETTINGSHOW)-> EnableWindow(FALSE);//不显示编辑框
		GetDlgItem(IDC_STATIC_TIMESHOW)-> EnableWindow(FALSE);//不显示“分钟”
		GetDlgItem(IDC_BUTTON_CONFIRM_TIME)-> EnableWindow(FALSE);//不显示确定按钮
	}
	//——>20160728 郝爽：设置滑动条等控件是否显示修改结束

	//——>20160728 郝爽：静态文本框显示“分钟”初始化
	SetDlgItemText(IDC_STATIC_TIMESHOW,_T("分钟"));//20160727 郝爽 设置ID为IDC_STATIC_TIMESHOW的文本框的内容,显示“分钟”
	//——>20160728 郝爽：静态文本框显示“分钟”初始化修改结束

	return true;
}


CPersonalDefineDlg::CPersonalDefineDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPersonalDefineDlg::IDD, pParent)	
	, m_edit_timesettingshow(0)                 //20160714 郝爽 自动生成
{
	
}

CPersonalDefineDlg::~CPersonalDefineDlg()
{
}

void CPersonalDefineDlg::DoDataExchange(CDataExchange* pDX)
{
	//郝爽 用于数据交换 下列语句自动生成
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_SLIDER_TIMESETTING, m_slider_timesetting);
	DDX_Control(pDX, IDC_STATIC_TIMESHOW, m_static_timeshow);
	DDX_Text(pDX, IDC_EDIT_TIMESETTINGSHOW, m_edit_timesettingshow);
	DDV_MinMaxFloat(pDX, m_edit_timesettingshow, 0, 30);
}


BEGIN_MESSAGE_MAP(CPersonalDefineDlg, CDialogEx)

	ON_BN_CLICKED(IDC_POPUP_SETTING, &CPersonalDefineDlg::OnBnClickedPopupSetting)
	ON_BN_CLICKED(IDC_CHECK_COPY, &CPersonalDefineDlg::OnBnClickedCheckCopy)
	ON_BN_CLICKED(IDC_CHECK_CAPTURE, &CPersonalDefineDlg::OnBnClickedCheckCapture)//20160714 郝爽 自动生成
	ON_BN_CLICKED(IDC_CHECK_TIMESETTING, &CPersonalDefineDlg::OnBnClickedCheckTimesetting)//20160725 郝爽
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_TIMESETTING, &CPersonalDefineDlg::OnNMCustomdrawSliderTimesetting)//20160726 郝爽	
	ON_EN_CHANGE(IDC_EDIT_TIMESETTINGSHOW, &CPersonalDefineDlg::OnEnChangeEditTimesettingshow)//20160727 郝爽
	ON_BN_CLICKED(IDC_BUTTON_CONFIRM_TIME, &CPersonalDefineDlg::OnBnClickedButtonConfirmTime)//20160727 郝爽
END_MESSAGE_MAP()


// CPersonalDefineDlg message handlers




//maliang:是否支持弹窗前置的复选框的相应函数
void CPersonalDefineDlg::OnBnClickedPopupSetting()
{  
	//-->20160711 郝爽 优化弹窗前置不能实时生效问题 part4

	//郝爽：在80与93行添加SetWindowPos语句，句柄使用m_hWnd，当前置选项被选中为1时，将弹窗设为前置，为0时不前置
	//郝爽：断点调试时，发现选中与不选中都实时执行了相应的SetWindowPos语句，但窗口由前置状态改变为不前置状态可立即生效
	//郝爽：而由不前置状态改变为前置状态时，语句能够执行，但不能立即生效，需将窗口最小化后重新载入，则可以实现前置

	// TODO: Add your control notification handler code here
	//maliang：判断如果选中复选框：
	if(((CButton*)GetDlgItem(IDC_POPUP_SETTING))->GetCheck()==TRUE)
	{   //maliang：写入相对路径下的displasy.ini文件,令is_shown=1
		////给是否支持弹窗前置的变量赋值
		//theApp.m_bCancelTop=1;
		WritePrivateProfileString(_T("displayConfig"), _T("is_shown"), _T("1"), _T("./display.ini"));

		//20160711 郝爽
		::SetWindowPos(m_hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);//20160711 郝爽
	}   
	//maliang：判断如果取消选中复选框：
	if(((CButton*)GetDlgItem(IDC_POPUP_SETTING))->GetCheck()==FALSE)
	{
		////给是否支持弹窗前置的变量赋值
		//theApp.m_bCancelTop=0;
		//maliang：写入相对路径下的displasy.ini文件，令is_shown=0
		WritePrivateProfileString(_T("displayConfig"), _T("is_shown"), _T("0"), _T("./display.ini"));
		//20160711 郝爽
		::SetWindowPos(m_hWnd,HWND_NOTOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);//20160711 郝爽
		//-->郝爽：弹窗前置part4实时生效问题修改结束，实时未实现
	}
}


//maliang:是否支持复制弹窗的复选框的相应函数	
void CPersonalDefineDlg::OnBnClickedCheckCopy()
{
	// TODO: Add your control notification handler code here
	//maliang：判断如果选中复选框：
	if(((CButton*)GetDlgItem(IDC_CHECK_COPY))->GetCheck()==TRUE)
	{   //maliang：将是否支持复制弹窗的变量设为TRUE
		theApp.m_bSupportCopy=TRUE;
		//maliang：写入相对路径下的checkcopy.ini文件，令is_certain=1
		WritePrivateProfileString(_T("ifCopyConfig"), _T("is_certain"), _T("1"), _T("./checkcopy.ini"));
	}
	//maliang：判断如果取消选中复选框：
	if(((CButton*)GetDlgItem(IDC_CHECK_COPY))->GetCheck()==FALSE)
	{  //maliang：将是否支持复制弹窗的变量设为FALSE
		theApp.m_bSupportCopy=FALSE;
		//maliang：写入相对路径下的checkcopy.ini文件，令is_certain=0
		WritePrivateProfileString(_T("ifCopyConfig"), _T("is_certain"), _T("0"), _T("./checkcopy.ini"));
	}
}

//——>20160714 郝爽 是否开启按键号码捕捉的相应函数
void CPersonalDefineDlg::OnBnClickedCheckCapture()
{
	// TODO: Add your control notification handler code here
	//20160713 郝爽 判断如果选中复选框
	if(((CButton*)GetDlgItem(IDC_CHECK_CAPTURE))->GetCheck()==TRUE)
	{
		//20160713 郝爽：写入相对路径下的NoCapture.ini文件，令is_capture=1
		theApp.m_bSupportNoCapture=TRUE;
		WritePrivateProfileString(_T("NoCaptureConfig"), _T("is_NoCapture"), _T("1"), _T("./NoCapture.ini"));
	}
	//20160713 郝爽 判断如果取消选中复选框
	if(((CButton*)GetDlgItem(IDC_CHECK_CAPTURE))->GetCheck()==FALSE)
	{
		//20160713 郝爽：写入相对路径下的NoCapture.ini文件，令is_capture=0
		theApp.m_bSupportNoCapture=FALSE;
		WritePrivateProfileString(_T("NoCaptureConfig"), _T("is_NoCapture"), _T("0"), _T("./NoCapture.ini"));
	}
}
//——>20160714 郝爽 是否开启按键号码捕捉的相应函数修改结束


//——>20160725 郝爽 是否开启设定时间内不执行查询号码功能
void CPersonalDefineDlg::OnBnClickedCheckTimesetting()
{
	// TODO: Add your control notification handler code here
	//20160725 郝爽 判断如果选中复选框
	if(((CButton*)GetDlgItem(IDC_CHECK_TIMESETTING))->GetCheck()==TRUE)
	{
		//20160725 郝爽：写入相对路径下的TimeSetting.ini文件，令is_TimeSetting=1
		theApp.m_bSupportTimeSetting=TRUE;
		WritePrivateProfileString(_T("TimeSettingConfig"), _T("is_TimeSetting"), _T("1"), _T("./TimeSetting.ini"));
		WritePrivateProfileString(_T("NumTimeConfig"), _T("NumTime"),_T(""), _T("./NumTime.ini"));//郝爽 创建配置文件NumTime
		GetDlgItem(IDC_SLIDER_TIMESETTING)-> EnableWindow(TRUE);//显示滑动条
		GetDlgItem(IDC_EDIT_TIMESETTINGSHOW)-> EnableWindow(TRUE);//显示编辑框
		GetDlgItem(IDC_STATIC_TIMESHOW)-> EnableWindow(TRUE);//显示“分钟”
		GetDlgItem(IDC_BUTTON_CONFIRM_TIME)-> EnableWindow(TRUE);//显示确定按钮
	}
	//20160725 郝爽 判断如果取消选中复选框
	if(((CButton*)GetDlgItem(IDC_CHECK_TIMESETTING))->GetCheck()==FALSE)
	{
		//20160725 郝爽：写入相对路径下的TimeSetting.ini文件，令is_TimeSetting=0
		theApp.m_bSupportTimeSetting=FALSE;
		WritePrivateProfileString(_T("TimeSettingConfig"), _T("is_TimeSetting"), _T("0"), _T("./TimeSetting.ini"));
		WritePrivateProfileString(_T("NumTimeConfig"), _T("NumTime"),_T(""), _T("./NumTime.ini"));//郝爽 清空配置文件NumTime
		GetDlgItem(IDC_SLIDER_TIMESETTING)-> EnableWindow(FALSE);//不显示滑动条
		GetDlgItem(IDC_EDIT_TIMESETTINGSHOW)-> EnableWindow(FALSE);//不显示编辑框
		GetDlgItem(IDC_STATIC_TIMESHOW)-> EnableWindow(FALSE);//不显示“分钟”
		GetDlgItem(IDC_BUTTON_CONFIRM_TIME)-> EnableWindow(FALSE);//不显示确定按钮
	}
}
//——>20160725 郝爽 是否开启设定时间内不执行查询号码功能修改结束

//——>20160726 郝爽 滑动条响应函数
void CPersonalDefineDlg::OnNMCustomdrawSliderTimesetting(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	theApp.SliderPos = m_slider_timesetting.GetPos(); //获得滑块的当前位置
	CString str;
	str.Format(_T("%d"),theApp.SliderPos);
	WritePrivateProfileString(_T("SliderTimePosConfig"), _T("is_SliderTimePos"), str, _T("./SliderTimePos.ini"));
	CString strfloat;
	double time = theApp.SliderPos/2.0;
	strfloat.Format(_T("%.1f"),time);
	SetDlgItemText(IDC_EDIT_TIMESETTINGSHOW,strfloat);

}
//——>20160726 郝爽 滑动条响应函数修改结束

//——>20160726 郝爽 编辑框函数修改 (应该没用)
void CPersonalDefineDlg::OnEnChangeEditTimesettingshow()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
//——>20160726 郝爽 编辑框函数修改结束

//——>20160728 郝爽 按钮相应函数修改
void CPersonalDefineDlg::OnBnClickedButtonConfirmTime()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_slider_timesetting.SetPos((m_edit_timesettingshow)*2);
	CString str;
	CString str0;
	double time = m_slider_timesetting.GetPos()/2.0;
	str.Format(_T("%.1f"),time);
	str0.Format(_T("%d"),m_slider_timesetting.GetPos());
	SetDlgItemText(IDC_EDIT_TIMESETTINGSHOW,str);
	WritePrivateProfileString(_T("SliderTimePosConfig"), _T("is_SliderTimePos"), str0, _T("./SliderTimePos.ini"));
	//UpdateData(TRUE);
}
//——>20160728 郝爽 按钮相应函数修改结束
