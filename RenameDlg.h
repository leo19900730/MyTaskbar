#pragma once


// CRenameDlg 对话框

class CRenameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRenameDlg)

public:
	CRenameDlg(CString* Title,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRenameDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()


	
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();

	CString* m_sTitle;
	CString m_txtTitle;
};
