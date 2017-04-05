
// MyTaskbar.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "MyTaskbar.h"
#include "MyTaskbarDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//#define WM_SWITCH_SHOW_MSG WM_USER + 310 //�Զ�����Ϣ�����ں�������ͨ��
///////////////////////////////////////////
// CMyTaskbarApp

BEGIN_MESSAGE_MAP(CMyTaskbarApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CMyTaskbarApp ����

CMyTaskbarApp::CMyTaskbarApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CMyTaskbarApp ����

CMyTaskbarApp theApp;
CDynamicLogger Logger;
HANDLE m_hMutex;


// CMyTaskbarApp ��ʼ��

BOOL CMyTaskbarApp::InitInstance()
{
	//ע�ᴰ����
	WNDCLASS wc;
	::GetClassInfo(AfxGetInstanceHandle(),L"#32770",&wc);
	//�˴���������Ҫ����rc�ļ�����ӣ�CLASS "CMyTaskbarDlg"
	wc.lpszClassName=L"CMyTaskbarDlg";
	bool flag=AfxRegisterClass(&wc);  //ע��

	
	//m_hMutex = ::CreateMutexW(NULL,FALSE,_T("MyTaskbar"));
	m_hMutex = ::CreateMutexW(NULL,FALSE,m_pszExeName);

	//�жϻ������Ƿ����
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		//AfxMessageBox(_T("Ӧ�ó����Ѿ�����!"));
	
		HWND hwnd=FindWindow(L"CMyTaskbarDlg",NULL);
		if(hwnd)
		{
			CMyTaskbarDlg *pDlg=(CMyTaskbarDlg*)CWnd::FromHandle(hwnd);
			if(pDlg) pDlg->SwitchWindowShow();   //��ʾ����
		}
		

		return false;
	}
	else
	{
		//AfxMessageBox(_T("Ӧ�ó����һ������"));
	}

	
	if(Logger==NULL)
	{
		AfxMessageBox(L"��־��������ʧ�ܣ�");
	}
	else
	{
		Logger->Init();
		Logger->Info(L"==========================================");
		Logger->Info(L"��������");

	}


	/////////////////////////////////////////////////////

	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	/////////////////////////////////GDIPlus03////////////////////////////////////
	//��ʼ��
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
	//////////////////////////////GDIPlus////////////////////////////////////////


	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CMyTaskbarDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�

	//CoInitialize(NULL); 

	return FALSE;
}



int CMyTaskbarApp::ExitInstance()
{
	// TODO: �ڴ����ר�ô����/����û���


	/////////////////////////////////GDIPlus04/////////////////////////////////////	
	Gdiplus::GdiplusShutdown(m_gdiplusToken);  //�ر�
	/////////////////////////////////GDIPlus/////////////////////////////////////

	//CoUninitialize();
	if(m_hMutex != NULL)
	{
		CloseHandle(m_hMutex);	//�رվ��
	}

	
	//LPCTSTR desc = ::ILogger_GetErrorDesc(Logger->GetLastError());
	//std::cerr << "ERROR: " << T2A((LPTSTR)desc) << std::endl;


	return CWinApp::ExitInstance();
}
