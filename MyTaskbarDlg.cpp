
// MyTaskbarDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyTaskbar.h"
#include "MyTaskbarDlg.h"
#include "afxdialogex.h"

#include "PathHelper.h"
#include "HookCallBack.h"

#include "DlgSetting.h"

#include "Utility.h"
#include "Setting.h"

//#include <iostream>  
//#include <string>  

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WALL_PAPER_SWITCH   //��ֽ���� 

//////////////////////////////////////////////////

// CMyTaskbarDlg �Ի���


enum TimerNumEnum
{
	TID_HideMainWindow=0,
	//TID_SetWallPaper=1		    //��������
};



 BOOL CMyTaskbarDlg::m_MouseHookRun=FALSE;
 BOOL CMyTaskbarDlg::m_KeyboardHookRun=TRUE;
 //CMyTaskbarDlg * m_pMyTaskbarDlg=NULL;


/////////////////////////////////////////////////
CMyTaskbarDlg::CMyTaskbarDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyTaskbarDlg::IDD, pParent)
{
	//m_pMyTaskbarDlg=this;

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bMainWindowShow=true;
	m_taskBarHeight = 30;
	
}



void CMyTaskbarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
	DDX_Control(pDX, IDC_CLOSE, m_btnClose);
	DDX_Control(pDX, IDC_MENU, m_btnMenu);
	DDX_Control(pDX, IDC_MIN, m_btnMin);
}

BEGIN_MESSAGE_MAP(CMyTaskbarDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CLOSE, &CMyTaskbarDlg::OnBnClickedClose)
	ON_BN_CLICKED(IDC_MENU, &CMyTaskbarDlg::OnBnClickedMenu)
	ON_COMMAND(ID_MENU_SET, &CMyTaskbarDlg::OnMenuSet)
	ON_MESSAGE(WM_SWITCH_SHOW_MSG,OnSwitchWindowShow)
	ON_BN_CLICKED(IDC_MIN, &CMyTaskbarDlg::OnBnClickedMin)
END_MESSAGE_MAP()


// CMyTaskbarDlg ��Ϣ�������



// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMyTaskbarDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMyTaskbarDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



int CMyTaskbarDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������

	//�޸ķ��ʹ����������������ʾ
	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);
	//ȥ���رհ���(����뻭3�������Ļ�)
	//ModifyStyle(WS_SYSMENU,NULL);

	SwitchWindowShow();

	//AnimateWindow(2000, AW_BLEND);
	


	return 0;
}

BOOL CMyTaskbarDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	////////////////////////////////////////////////////////
	InitButton();

	m_ListCtrl.InitListCtrl();
	CRect rec;
	GetClientRect(rec);
	m_ListCtrl.MoveWindow(0,10 ,rec.Width()-200 ,rec.Height()-10);


	SetTimer(TID_HideMainWindow,1000,NULL);  // ��ʾһ��ʱ������ش���

	StartHook();
	//���߳����һЩ��ʼ��
	AfxBeginThread((AFX_THREADPROC)SubThreadFun, this); 

	InitWeatherInfo();

	Logger->Info(L"���ڳ�ʼ�����");
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMyTaskbarDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	switch(nIDEvent)
	{
	case TID_HideMainWindow:
		SwitchWindowShow();

		if(CSetting::GetWallPaperEnable()==1)
		{

#ifdef WALL_PAPER_SWITCH
			m_setWinWallPaper.ThreadSetStartWallPaper(); // ��ֽ
#endif
		}
		KillTimer(TID_HideMainWindow);
		break;
	default:break;
	}

	
	CDialogEx::OnTimer(nIDEvent);
}

UINT WINAPI CMyTaskbarDlg::SubThreadFun(LPVOID lpParam) 
{
	CMyTaskbarDlg * _this = (CMyTaskbarDlg *)lpParam; 	
	CSetting::InitRegedit();

	return 0;
}


///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

void CMyTaskbarDlg::SwitchWindowShow()
{

	CRect recWnd;
	GetWindowRect(recWnd);
	if(recWnd.top==0 && recWnd.left==0)
	{
		m_bMainWindowShow=true;
	}
	else if(recWnd.top==0)
	{
		m_bMainWindowShow=false;
	}
	else
	{
		m_bMainWindowShow=true;
	}


	int iScreenWidth=GetSystemMetrics(SM_CXSCREEN);
	int iWndWidth=iScreenWidth-200;
	int iWndHeight=80;
	int iWndX=(iScreenWidth-iWndWidth)/2;
	int iWndY=0;
	
	

	if(m_bMainWindowShow)
	{
		iWndY=0;		
		//m_bMainWindowShow=false;
	}
	else
	{
		iWndY=-iWndHeight-2;
		//m_bMainWindowShow=true;
	}

	SetWindowPos(&wndTopMost,iWndX,iWndY,iWndWidth,iWndHeight,SWP_SHOWWINDOW);
	//MoveWindow(iWndX,iWndY,iWndWidth,iWndHeight);	

}


void CMyTaskbarDlg::InitButton()
{
	CRect rcMainWnd;
	GetClientRect(&rcMainWnd);

	
	int iLeft=0,iTop=5,iWidth=16,iHeight=20;	

	///////////////////////////////////////
	//m_btnClose.ShowImage(L"./res/UI/Close_1.png",L"./res/UI/Close_2.png");
	HBITMAP hBmp, hBmpHot;
	Bitmap *pBmp, *pBmpHot;

	ImageFromIDResource(IDB_CLOSE1,L"PNG",pBmp);
	ImageFromIDResource(IDB_CLOSE2,L"PNG",pBmpHot);
	pBmp->GetHBITMAP(Color(0,0,0),&hBmp);
	pBmpHot->GetHBITMAP(Color(0,0,0),&hBmpHot);

	m_btnClose.ShowImage(hBmp,hBmpHot);

	iLeft=rcMainWnd.right-iWidth-10;
	m_btnClose.MoveWindow(iLeft,iTop ,iWidth ,iHeight);

	////////////////////////////////////////
	ImageFromIDResource(IDB_MIN1,L"PNG",pBmp);
	ImageFromIDResource(IDB_MIN2,L"PNG",pBmpHot);
	pBmp->GetHBITMAP(Color(0,0,0),&hBmp);
	pBmpHot->GetHBITMAP(Color(0,0,0),&hBmpHot);

	m_btnMin.ShowImage(hBmp,hBmpHot);

	iLeft=iLeft-iWidth-10;
	m_btnMin.MoveWindow(iLeft,iTop ,iWidth ,iHeight);

	///////////////////////////////////////////////////
	  
	//m_btnMenu.ShowImage(L"./res/UI/Menu_1.png",L"./res/UI/Menu_2.png");
	ImageFromIDResource(IDB_MENU1,L"PNG",pBmp);
	ImageFromIDResource(IDB_MENU2,L"PNG",pBmpHot);
	pBmp->GetHBITMAP(Color(0,0,0),&hBmp);
	pBmpHot->GetHBITMAP(Color(0,0,0),&hBmpHot);
	m_btnMenu.ShowImage(hBmp,hBmpHot);

	iLeft=iLeft-iWidth-10;
	m_btnMenu.MoveWindow(iLeft,iTop ,iWidth ,iHeight);


}




void CMyTaskbarDlg::ReleaseResource()
{

	StopHook();
	SetWinWallPaper::ExitWallPaper();

	if(m_hDllWeatherInfo!=NULL) FreeLibrary(m_hDllWeatherInfo);
}

void CMyTaskbarDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	ReleaseResource();
		

	Logger->Info(L"�����˳�");
	Logger->UnInit();
}



///////////////////////////////////////////////////////
//////////////////////�������////////////////////////

//Hook Mouse
void CMyTaskbarDlg::StartHook()
{
	CHookCallBack::StartHook(m_hWnd,m_MouseHookRun,m_KeyboardHookRun);
}

//Unhook Mouse
void CMyTaskbarDlg::StopHook()
{
	CHookCallBack::StopHook(m_MouseHookRun,m_KeyboardHookRun);
}




//////////////////////////
// ȫ�ּ��̹��ӻص�����  
// ����: action ��ʶ������Ϣ(����,����), keyStruct ����������Ϣ  
BOOL CALLBACK KeyboardProc(WPARAM action, KBDLLHOOKSTRUCT* pKeyStruct)  
{  
	return CHookCallBack::KeyboardProc(action,pKeyStruct);
}  

////////////////////////////////////////
//��ť�¼�
void CMyTaskbarDlg::OnBnClickedMin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SwitchWindowShow();

}

void CMyTaskbarDlg::OnBnClickedClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}



void CMyTaskbarDlg::OnBnClickedMenu()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
    CRect rect;  
    m_btnMenu.GetWindowRect(&rect);  
    CPoint pt;  
    GetCursorPos(&pt);//�������Ļ����  

    if (rect.PtInRect(pt))  
    {  
        CMenu m_Menu;  
        m_Menu.LoadMenu(IDR_MENU1);//�༭�õĲ˵���Դ  
        CMenu *m_SubMenu = m_Menu.GetSubMenu(0);  
  
		//��ʾ�˵�
        m_SubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);  
    }  
}


void CMyTaskbarDlg::OnMenuSet()
{
	// TODO: �ڴ���������������
	//SettingInfo info;
	//info.WallPaperSwitchTime=CSetting::GetWallPaperSwitchTime();

	CDlgSetting dlg(this);
	dlg.DoModal();


	if(CSetting::GetWallPaperEnable()==1)
	{
		SetWinWallPaper::ResetWallPaper();
	}
	else
	{
		SetWinWallPaper::StopWallPaper();
	}

	//��������ʱ��
	//CSetting::SetWallPaperSwitchTime(info.WallPaperSwitchTime);
	//ResetWallPaper(info.WallPaperSwitchTime);
	int WallPaperSwitchTime=CSetting::GetWallPaperSwitchTime();
	SetWinWallPaper::SetWallPaperSwitchTime(WallPaperSwitchTime);

}


BOOL CMyTaskbarDlg::ImageFromIDResource(UINT resurceID,LPCTSTR imgType,Bitmap * &pImg)
{
	HINSTANCE hInst = AfxGetResourceHandle();
	HRSRC hRsrc = ::FindResource (hInst,MAKEINTRESOURCE(resurceID),imgType); // type
	if (hRsrc)
    {
	    // load resource into memory
	    DWORD len = SizeofResource(hInst, hRsrc);
	    BYTE* lpRsrc = (BYTE*)LoadResource(hInst, hRsrc);
	    if (lpRsrc)
        {
            // Allocate global memory on which to create stream
	        HGLOBAL m_hMem = GlobalAlloc(GMEM_FIXED, len);
	        BYTE* pmem = (BYTE*)GlobalLock(m_hMem);
	        memcpy(pmem,lpRsrc,len);
	        IStream* pstm;
	        CreateStreamOnHGlobal(m_hMem,FALSE,&pstm);
	
	        // load from stream
	        pImg=Gdiplus::Bitmap::FromStream(pstm);

	        // free/release stuff
	        GlobalUnlock(m_hMem);
	        pstm->Release();
	        FreeResource(lpRsrc);
            return TRUE;
        }
    }
    return FALSE;
}

LRESULT CMyTaskbarDlg::OnSwitchWindowShow(WPARAM wParam,LPARAM lParam)
{
	SwitchWindowShow();
	return 1;
}


//////////////////��ȡ������Ϣ///////////////////////
typedef WeatherInfo* (*GetWeatherInfoFunc)();

CWinThread * CMyTaskbarDlg::m_pWinThread=NULL;
WeatherInfo* CMyTaskbarDlg::m_weatherInfo=NULL;
HINSTANCE CMyTaskbarDlg::m_hDllWeatherInfo=NULL;
bool CMyTaskbarDlg::m_bInitWeatherInfoSuccess=false;

bool CMyTaskbarDlg::InitWeatherInfo()
{
	m_pWinThread=AfxBeginThread((AFX_THREADPROC)InitWeatherInfoFun, this); 
	
	return true;  
}


UINT WINAPI CMyTaskbarDlg::InitWeatherInfoFun(LPVOID lpParam)
{
	m_hDllWeatherInfo=NULL;
	m_hDllWeatherInfo=LoadLibrary(L"Weather.dll");
	if(m_hDllWeatherInfo==NULL) return 1;

	GetWeatherInfoFunc GetWeatherInfo=(GetWeatherInfoFunc)GetProcAddress(m_hDllWeatherInfo,"GetWeatherInfo");  
	if(!GetWeatherInfo) return 1;

	m_weatherInfo=GetWeatherInfo();
	//////

	//GetWeatherInfoFunc ReleaseResource=(GetWeatherInfoFunc)GetProcAddress(m_hDllWeatherInfo,"ReleaseResource");
	
	m_bInitWeatherInfoSuccess=true;

	//ReleaseResource();
	return 0;  
}