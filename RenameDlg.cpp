// RenameDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyTaskbar.h"
#include "RenameDlg.h"
#include "afxdialogex.h"


// CRenameDlg 对话框

IMPLEMENT_DYNAMIC(CRenameDlg, CDialogEx)

CRenameDlg::CRenameDlg(CString* Title,CWnd* pParent /*=NULL*/)
	: CDialogEx(CRenameDlg::IDD, pParent),
	m_sTitle(Title), 
	m_txtTitle(_T(""))
{
	
}

CRenameDlg::~CRenameDlg()
{
}

void CRenameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TITLE, m_txtTitle);
}


BEGIN_MESSAGE_MAP(CRenameDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CRenameDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CRenameDlg 消息处理程序


BOOL CRenameDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  在此添加额外的初始化
	m_txtTitle=*m_sTitle;
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CRenameDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	*m_sTitle=m_txtTitle;
	CDialogEx::OnOK();
}
