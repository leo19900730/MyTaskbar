#pragma once
#include "afxwin.h"


// CDlgSetting �Ի���

//struct SettingInfo
//{
//	int WallPaperSwitchTime;
//
//};



class CDlgSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSetting)

public:
	CDlgSetting(CWnd* pParent/*,SettingInfo & setInfo*/ );   // ��׼���캯��
	virtual ~CDlgSetting();

// �Ի�������
	enum { IDD = IDD_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, LPVOID lpParam = NULL);
	virtual BOOL OnInitDialog();

	////////////////////////////////
	int m_WallPaperSwitchTime;
	//SettingInfo * m_setInfo;

	afx_msg void OnBnClickedOpenAppdir();
	afx_msg void OnBnClickedOpenLinkfile();
	afx_msg void OnBnClickedOk();

private:


public:
	CButton m_chkEnableWallPaper;
};


