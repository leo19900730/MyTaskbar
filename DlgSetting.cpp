// DlgSetting.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyTaskbar.h"
#include "DlgSetting.h"
#include "afxdialogex.h"
#include "Setting.h"


// CDlgSetting �Ի���

IMPLEMENT_DYNAMIC(CDlgSetting, CDialogEx)

CDlgSetting::CDlgSetting(CWnd* pParent/*,SettingInfo & setInfo*/)
	: CDialogEx(CDlgSetting::IDD, pParent)
{

	//m_WallPaperSwitchTime = setInfo.WallPaperSwitchTime;
	//m_setInfo=&setInfo;
}

CDlgSetting::~CDlgSetting()
{
}

void CDlgSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_WP_SWITCH, m_WallPaperSwitchTime);
	DDV_MinMaxInt(pDX, m_WallPaperSwitchTime, 2000, 1000000);
	DDX_Control(pDX, IDC_ENABLE_WALLPAPER, m_chkEnableWallPaper);
}


BEGIN_MESSAGE_MAP(CDlgSetting, CDialogEx)
	ON_BN_CLICKED(IDC_OPEN_APPDIR, &CDlgSetting::OnBnClickedOpenAppdir)
	ON_BN_CLICKED(IDC_OPEN_LINKFILE, &CDlgSetting::OnBnClickedOpenLinkfile)
	ON_BN_CLICKED(IDOK, &CDlgSetting::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgSetting ��Ϣ�������


BOOL CDlgSetting::CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, LPVOID lpParam)
{
	// TODO: �ڴ����ר�ô����/����û���
	CenterWindow();

	return CDialogEx::CreateEx(dwExStyle, lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, lpParam);
}


BOOL CDlgSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//CenterWindow();
	int scrWidth = GetSystemMetrics(SM_CXSCREEN);  
	int scrHeight = GetSystemMetrics(SM_CYSCREEN);  

	CRect rec;
	GetWindowRect(rec);

	int left=(scrWidth-rec.Width())/2;
	int top=(scrHeight-rec.Height())/2;

	MoveWindow(left,top,rec.Width(),rec.Height());


	if(CSetting::GetWallPaperEnable()==1)
	{
		m_chkEnableWallPaper.SetCheck(true);
	}
	else
	{
		m_chkEnableWallPaper.SetCheck(false);
	}

	//m_WallPaperSwitchTime = m_setInfo->WallPaperSwitchTime;
	m_WallPaperSwitchTime =CSetting::GetWallPaperSwitchTime();

	UpdateData(FALSE);


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgSetting::OnBnClickedOpenAppdir()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString sPath=CSetting::GetAppPath();
	ShellExecute(NULL, _T("open"),sPath, NULL, NULL, SW_MAXIMIZE);
}


void CDlgSetting::OnBnClickedOpenLinkfile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString sPath;
	CSetting::GetFileListPath(sPath);
	ShellExecute(NULL, _T("open"),sPath, NULL, NULL, SW_MAXIMIZE);
}


void CDlgSetting::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	CSetting::SetWallPaperSwitchTime(m_WallPaperSwitchTime);

	if(m_chkEnableWallPaper.GetCheck())
	{
		CSetting::SetWallPaperEnable(1);
	}
	else
	{
		CSetting::SetWallPaperEnable(0);
	}

	//m_setInfo->WallPaperSwitchTime=m_WallPaperSwitchTime;


	CDialogEx::OnOK();
}
