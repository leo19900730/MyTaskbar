// RenameDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyTaskbar.h"
#include "RenameDlg.h"
#include "afxdialogex.h"


// CRenameDlg �Ի���

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


// CRenameDlg ��Ϣ�������


BOOL CRenameDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_txtTitle=*m_sTitle;
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CRenameDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	*m_sTitle=m_txtTitle;
	CDialogEx::OnOK();
}
