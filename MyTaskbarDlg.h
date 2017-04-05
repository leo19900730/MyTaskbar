
// MyTaskbarDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "ListCtrlEx.h"


#include <vector>
#include "mfcbuttonex.h"
#include "afxbutton.h"

#include "SetWinWallPaper.h"

using namespace std;
using namespace Gdiplus;

#define WM_SWITCH_SHOW_MSG WM_USER + 310 //自定义消息，用于和主程序通信


//////////////////////////////////////////////////////////////


// CMyTaskbarDlg 对话框
class CMyTaskbarDlg : public CDialogEx
{
	// 构造
public:
	CMyTaskbarDlg(CWnd* pParent = NULL);	// 标准构造函数

	// 对话框数据
	enum { IDD = IDD_MYTASKBAR_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	afx_msg LRESULT OnSwitchWindowShow(WPARAM wParam,LPARAM lParam);


	// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();

	afx_msg void OnBnClickedClose();
	afx_msg void OnBnClickedMin();
	afx_msg void OnBnClickedMenu();
	afx_msg void OnMenuSet();

private:
	bool m_bMainWindowShow;
	INT  m_taskBarHeight;   //任务栏高度
		
	CListCtrlEx m_ListCtrl;

	MFCButtonex m_btnClose;
	MFCButtonex m_btnMin;
	MFCButtonex m_btnMenu;

	SetWinWallPaper m_setWinWallPaper;
	
	
public:
	
	void SwitchWindowShow();
	void InitButton();
	//void ResetWallPaper(int iWallPaperSwitchTime);
	BOOL ImageFromIDResource(UINT resurceID,LPCTSTR imgType,Bitmap * &pImg);


	static UINT WINAPI SubThreadFun(LPVOID lpParam);
	void ReleaseResource();  //释放资源

	

///////////////////////////////////////////////////////
//////////////////////钩子相关////////////////////////
	static BOOL m_MouseHookRun;
	static BOOL m_KeyboardHookRun;
	
	void StartHook();
	void StopHook();
///////////////////////////////
	//static CMyTaskbarDlg * m_pMyTaskbarDlg;
	static CWinThread * m_pWinThread;
	static WeatherInfo * m_weatherInfo;
	static HINSTANCE m_hDllWeatherInfo;
	static bool m_bInitWeatherInfoSuccess;
	bool InitWeatherInfo();
	static UINT WINAPI InitWeatherInfoFun(LPVOID lpParam);

};

	