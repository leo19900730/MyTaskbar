#pragma once


// CRenameDlg �Ի���

class CRenameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRenameDlg)

public:
	CRenameDlg(CString* Title,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRenameDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()


	
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();

	CString* m_sTitle;
	CString m_txtTitle;
};
