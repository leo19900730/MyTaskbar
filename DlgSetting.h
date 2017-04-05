#pragma once
#include "afxwin.h"


// CDlgSetting 对话框

//struct SettingInfo
//{
//	int WallPaperSwitchTime;
//
//};



class CDlgSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSetting)

public:
	CDlgSetting(CWnd* pParent/*,SettingInfo & setInfo*/ );   // 标准构造函数
	virtual ~CDlgSetting();

// 对话框数据
	enum { IDD = IDD_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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


