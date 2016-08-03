// PersonalDefineDlg.cpp : implementation file
//
//maliang��PersonalDefineDlg.cpp :ʵ���Զ��帴ѡ���ܵ���
#include "stdafx.h"
#include "IncrementSystem.h"
#include "PersonalDefineDlg.h"
#include "afxdialogex.h"
#include "BONCSocket.h"
#include "IncrementSystemDlg.h"

// CPersonalDefineDlg dialog

IMPLEMENT_DYNAMIC(CPersonalDefineDlg, CDialogEx)



	//maliang����ʼ�����öԻ���
	BOOL CPersonalDefineDlg::OnInitDialog()
{   
	CDialog::OnInitDialog();

	/*maliang����ȡ�Ƿ�ǰ�õ�ini�ļ�����Ϊcheckbox����ֵ�����ȡ����ֵ��û��ini�ļ���ȡֵΪ0*/
	int is_shown = GetPrivateProfileInt(_T("displayConfig"), _T("is_shown"),0, _T("./display.ini"));
	CButton* pBtn = (CButton*)GetDlgItem(IDC_POPUP_SETTING);
	pBtn->SetCheck(is_shown);

	/*maliang����ȡ�Ƿ�֧�ָ��Ƶ�����ini�ļ�����Ϊ��Ӧ��checkbox����ֵ*/
	int is_certain = GetPrivateProfileInt(_T("ifCopyConfig"), _T("is_certain"),0, _T("./checkcopy.ini"));
	CButton* pBtn1 = (CButton*)GetDlgItem(IDC_CHECK_COPY);
	pBtn1->SetCheck(is_certain);

	//����>20160714 ��ˬ����ȡ�Ƿ�֧�ֿ����������벶׽��ini�ļ�����Ϊ��Ӧ��checkbox����ֵ
	int is_NoCapture = GetPrivateProfileInt(_T("NoCaptureConfig"), _T("is_NoCapture"),0, _T("./NoCapture.ini"));
	CButton* pBtn2 = (CButton*)GetDlgItem(IDC_CHECK_CAPTURE);
	pBtn2->SetCheck(is_NoCapture);
	//����>20160714 ��ˬ ��ȡ�Ƿ�֧�ֿ����������벶׽��ini�ļ��޸Ľ���

	//����>20160725 ��ˬ����ȡ�Ƿ�֧�ֿ����趨ʱ���ڲ����к����ѯ��ini�ļ�����Ϊ��Ӧ��checkbox����ֵ
	int is_TimeSetting = GetPrivateProfileInt(_T("TimeSettingConfig"), _T("is_TimeSetting"),0, _T("./TimeSetting.ini"));
	CButton* pBtn3 = (CButton*)GetDlgItem(IDC_CHECK_TIMESETTING);
	pBtn3->SetCheck(is_TimeSetting);
	//����>20160725 ��ˬ ��ȡ�Ƿ�֧�ֿ����趨ʱ���ڲ����к����ѯ��ini�ļ��޸Ľ���

	//����>20160726 ��ˬ ��������ʼ��
	m_slider_timesetting.SetRange(1,60);//���û�����Χ
	m_slider_timesetting.SetTicFreq(1);//ÿ1����λ��һ�̶�
	m_slider_timesetting.SetLineSize(1);
	m_slider_timesetting.SetPos(GetPrivateProfileInt(_T("SliderTimePosConfig"), _T("is_SliderTimePos"),1, _T("./SliderTimePos.ini")));//����Ĭ�ϵ�ǰλ��
	//����>20160726 ��ˬ ��������ʼ������

	//����>20160728 ��ˬ�����û������ȿؼ��Ƿ���ʾ
	if(is_TimeSetting == 1)
	{
		GetDlgItem(IDC_SLIDER_TIMESETTING)-> EnableWindow(TRUE);//��ʾ������
		GetDlgItem(IDC_EDIT_TIMESETTINGSHOW)-> EnableWindow(TRUE);//��ʾ�༭��
		GetDlgItem(IDC_STATIC_TIMESHOW)-> EnableWindow(TRUE);//��ʾ�����ӡ�
		GetDlgItem(IDC_BUTTON_CONFIRM_TIME)-> EnableWindow(TRUE);//��ʾȷ����ť
	}
	else
	{
		GetDlgItem(IDC_SLIDER_TIMESETTING)-> EnableWindow(FALSE);//����ʾ������
		GetDlgItem(IDC_EDIT_TIMESETTINGSHOW)-> EnableWindow(FALSE);//����ʾ�༭��
		GetDlgItem(IDC_STATIC_TIMESHOW)-> EnableWindow(FALSE);//����ʾ�����ӡ�
		GetDlgItem(IDC_BUTTON_CONFIRM_TIME)-> EnableWindow(FALSE);//����ʾȷ����ť
	}
	//����>20160728 ��ˬ�����û������ȿؼ��Ƿ���ʾ�޸Ľ���

	//����>20160728 ��ˬ����̬�ı�����ʾ�����ӡ���ʼ��
	SetDlgItemText(IDC_STATIC_TIMESHOW,_T("����"));//20160727 ��ˬ ����IDΪIDC_STATIC_TIMESHOW���ı��������,��ʾ�����ӡ�
	//����>20160728 ��ˬ����̬�ı�����ʾ�����ӡ���ʼ���޸Ľ���

	return true;
}


CPersonalDefineDlg::CPersonalDefineDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPersonalDefineDlg::IDD, pParent)	
	, m_edit_timesettingshow(0)                 //20160714 ��ˬ �Զ�����
{
	
}

CPersonalDefineDlg::~CPersonalDefineDlg()
{
}

void CPersonalDefineDlg::DoDataExchange(CDataExchange* pDX)
{
	//��ˬ �������ݽ��� ��������Զ�����
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_SLIDER_TIMESETTING, m_slider_timesetting);
	DDX_Control(pDX, IDC_STATIC_TIMESHOW, m_static_timeshow);
	DDX_Text(pDX, IDC_EDIT_TIMESETTINGSHOW, m_edit_timesettingshow);
	DDV_MinMaxFloat(pDX, m_edit_timesettingshow, 0, 30);
}


BEGIN_MESSAGE_MAP(CPersonalDefineDlg, CDialogEx)

	ON_BN_CLICKED(IDC_POPUP_SETTING, &CPersonalDefineDlg::OnBnClickedPopupSetting)
	ON_BN_CLICKED(IDC_CHECK_COPY, &CPersonalDefineDlg::OnBnClickedCheckCopy)
	ON_BN_CLICKED(IDC_CHECK_CAPTURE, &CPersonalDefineDlg::OnBnClickedCheckCapture)//20160714 ��ˬ �Զ�����
	ON_BN_CLICKED(IDC_CHECK_TIMESETTING, &CPersonalDefineDlg::OnBnClickedCheckTimesetting)//20160725 ��ˬ
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_TIMESETTING, &CPersonalDefineDlg::OnNMCustomdrawSliderTimesetting)//20160726 ��ˬ	
	ON_EN_CHANGE(IDC_EDIT_TIMESETTINGSHOW, &CPersonalDefineDlg::OnEnChangeEditTimesettingshow)//20160727 ��ˬ
	ON_BN_CLICKED(IDC_BUTTON_CONFIRM_TIME, &CPersonalDefineDlg::OnBnClickedButtonConfirmTime)//20160727 ��ˬ
END_MESSAGE_MAP()


// CPersonalDefineDlg message handlers




//maliang:�Ƿ�֧�ֵ���ǰ�õĸ�ѡ�����Ӧ����
void CPersonalDefineDlg::OnBnClickedPopupSetting()
{  
	//-->20160711 ��ˬ �Ż�����ǰ�ò���ʵʱ��Ч���� part4

	//��ˬ����80��93�����SetWindowPos��䣬���ʹ��m_hWnd����ǰ��ѡ�ѡ��Ϊ1ʱ����������Ϊǰ�ã�Ϊ0ʱ��ǰ��
	//��ˬ���ϵ����ʱ������ѡ���벻ѡ�ж�ʵʱִ������Ӧ��SetWindowPos��䣬��������ǰ��״̬�ı�Ϊ��ǰ��״̬��������Ч
	//��ˬ�����ɲ�ǰ��״̬�ı�Ϊǰ��״̬ʱ������ܹ�ִ�У�������������Ч���轫������С�����������룬�����ʵ��ǰ��

	// TODO: Add your control notification handler code here
	//maliang���ж����ѡ�и�ѡ��
	if(((CButton*)GetDlgItem(IDC_POPUP_SETTING))->GetCheck()==TRUE)
	{   //maliang��д�����·���µ�displasy.ini�ļ�,��is_shown=1
		////���Ƿ�֧�ֵ���ǰ�õı�����ֵ
		//theApp.m_bCancelTop=1;
		WritePrivateProfileString(_T("displayConfig"), _T("is_shown"), _T("1"), _T("./display.ini"));

		//20160711 ��ˬ
		::SetWindowPos(m_hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);//20160711 ��ˬ
	}   
	//maliang���ж����ȡ��ѡ�и�ѡ��
	if(((CButton*)GetDlgItem(IDC_POPUP_SETTING))->GetCheck()==FALSE)
	{
		////���Ƿ�֧�ֵ���ǰ�õı�����ֵ
		//theApp.m_bCancelTop=0;
		//maliang��д�����·���µ�displasy.ini�ļ�����is_shown=0
		WritePrivateProfileString(_T("displayConfig"), _T("is_shown"), _T("0"), _T("./display.ini"));
		//20160711 ��ˬ
		::SetWindowPos(m_hWnd,HWND_NOTOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);//20160711 ��ˬ
		//-->��ˬ������ǰ��part4ʵʱ��Ч�����޸Ľ�����ʵʱδʵ��
	}
}


//maliang:�Ƿ�֧�ָ��Ƶ����ĸ�ѡ�����Ӧ����	
void CPersonalDefineDlg::OnBnClickedCheckCopy()
{
	// TODO: Add your control notification handler code here
	//maliang���ж����ѡ�и�ѡ��
	if(((CButton*)GetDlgItem(IDC_CHECK_COPY))->GetCheck()==TRUE)
	{   //maliang�����Ƿ�֧�ָ��Ƶ����ı�����ΪTRUE
		theApp.m_bSupportCopy=TRUE;
		//maliang��д�����·���µ�checkcopy.ini�ļ�����is_certain=1
		WritePrivateProfileString(_T("ifCopyConfig"), _T("is_certain"), _T("1"), _T("./checkcopy.ini"));
	}
	//maliang���ж����ȡ��ѡ�и�ѡ��
	if(((CButton*)GetDlgItem(IDC_CHECK_COPY))->GetCheck()==FALSE)
	{  //maliang�����Ƿ�֧�ָ��Ƶ����ı�����ΪFALSE
		theApp.m_bSupportCopy=FALSE;
		//maliang��д�����·���µ�checkcopy.ini�ļ�����is_certain=0
		WritePrivateProfileString(_T("ifCopyConfig"), _T("is_certain"), _T("0"), _T("./checkcopy.ini"));
	}
}

//����>20160714 ��ˬ �Ƿ����������벶׽����Ӧ����
void CPersonalDefineDlg::OnBnClickedCheckCapture()
{
	// TODO: Add your control notification handler code here
	//20160713 ��ˬ �ж����ѡ�и�ѡ��
	if(((CButton*)GetDlgItem(IDC_CHECK_CAPTURE))->GetCheck()==TRUE)
	{
		//20160713 ��ˬ��д�����·���µ�NoCapture.ini�ļ�����is_capture=1
		theApp.m_bSupportNoCapture=TRUE;
		WritePrivateProfileString(_T("NoCaptureConfig"), _T("is_NoCapture"), _T("1"), _T("./NoCapture.ini"));
	}
	//20160713 ��ˬ �ж����ȡ��ѡ�и�ѡ��
	if(((CButton*)GetDlgItem(IDC_CHECK_CAPTURE))->GetCheck()==FALSE)
	{
		//20160713 ��ˬ��д�����·���µ�NoCapture.ini�ļ�����is_capture=0
		theApp.m_bSupportNoCapture=FALSE;
		WritePrivateProfileString(_T("NoCaptureConfig"), _T("is_NoCapture"), _T("0"), _T("./NoCapture.ini"));
	}
}
//����>20160714 ��ˬ �Ƿ����������벶׽����Ӧ�����޸Ľ���


//����>20160725 ��ˬ �Ƿ����趨ʱ���ڲ�ִ�в�ѯ���빦��
void CPersonalDefineDlg::OnBnClickedCheckTimesetting()
{
	// TODO: Add your control notification handler code here
	//20160725 ��ˬ �ж����ѡ�и�ѡ��
	if(((CButton*)GetDlgItem(IDC_CHECK_TIMESETTING))->GetCheck()==TRUE)
	{
		//20160725 ��ˬ��д�����·���µ�TimeSetting.ini�ļ�����is_TimeSetting=1
		theApp.m_bSupportTimeSetting=TRUE;
		WritePrivateProfileString(_T("TimeSettingConfig"), _T("is_TimeSetting"), _T("1"), _T("./TimeSetting.ini"));
		WritePrivateProfileString(_T("NumTimeConfig"), _T("NumTime"),_T(""), _T("./NumTime.ini"));//��ˬ ���������ļ�NumTime
		GetDlgItem(IDC_SLIDER_TIMESETTING)-> EnableWindow(TRUE);//��ʾ������
		GetDlgItem(IDC_EDIT_TIMESETTINGSHOW)-> EnableWindow(TRUE);//��ʾ�༭��
		GetDlgItem(IDC_STATIC_TIMESHOW)-> EnableWindow(TRUE);//��ʾ�����ӡ�
		GetDlgItem(IDC_BUTTON_CONFIRM_TIME)-> EnableWindow(TRUE);//��ʾȷ����ť
	}
	//20160725 ��ˬ �ж����ȡ��ѡ�и�ѡ��
	if(((CButton*)GetDlgItem(IDC_CHECK_TIMESETTING))->GetCheck()==FALSE)
	{
		//20160725 ��ˬ��д�����·���µ�TimeSetting.ini�ļ�����is_TimeSetting=0
		theApp.m_bSupportTimeSetting=FALSE;
		WritePrivateProfileString(_T("TimeSettingConfig"), _T("is_TimeSetting"), _T("0"), _T("./TimeSetting.ini"));
		WritePrivateProfileString(_T("NumTimeConfig"), _T("NumTime"),_T(""), _T("./NumTime.ini"));//��ˬ ��������ļ�NumTime
		GetDlgItem(IDC_SLIDER_TIMESETTING)-> EnableWindow(FALSE);//����ʾ������
		GetDlgItem(IDC_EDIT_TIMESETTINGSHOW)-> EnableWindow(FALSE);//����ʾ�༭��
		GetDlgItem(IDC_STATIC_TIMESHOW)-> EnableWindow(FALSE);//����ʾ�����ӡ�
		GetDlgItem(IDC_BUTTON_CONFIRM_TIME)-> EnableWindow(FALSE);//����ʾȷ����ť
	}
}
//����>20160725 ��ˬ �Ƿ����趨ʱ���ڲ�ִ�в�ѯ���빦���޸Ľ���

//����>20160726 ��ˬ ��������Ӧ����
void CPersonalDefineDlg::OnNMCustomdrawSliderTimesetting(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	theApp.SliderPos = m_slider_timesetting.GetPos(); //��û���ĵ�ǰλ��
	CString str;
	str.Format(_T("%d"),theApp.SliderPos);
	WritePrivateProfileString(_T("SliderTimePosConfig"), _T("is_SliderTimePos"), str, _T("./SliderTimePos.ini"));
	CString strfloat;
	double time = theApp.SliderPos/2.0;
	strfloat.Format(_T("%.1f"),time);
	SetDlgItemText(IDC_EDIT_TIMESETTINGSHOW,strfloat);

}
//����>20160726 ��ˬ ��������Ӧ�����޸Ľ���

//����>20160726 ��ˬ �༭�����޸� (Ӧ��û��)
void CPersonalDefineDlg::OnEnChangeEditTimesettingshow()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
//����>20160726 ��ˬ �༭�����޸Ľ���

//����>20160728 ��ˬ ��ť��Ӧ�����޸�
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
//����>20160728 ��ˬ ��ť��Ӧ�����޸Ľ���
