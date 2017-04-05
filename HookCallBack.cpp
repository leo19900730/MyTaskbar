#include "StdAfx.h"
#include "HookCallBack.h"
#include "PathHelper.h"
#include "Utility.h"
#include "Setting.h"

#define WM_SWITCH_SHOW_MSG WM_USER + 310 //�Զ�����Ϣ�����ں�������ͨ��

 HWND CHookCallBack::m_Hwnd=NULL;
 


CHookCallBack::CHookCallBack(void)
{
}


CHookCallBack::~CHookCallBack(void)
{
}


///////////////////////////////////////////////////////
//////////////////////�������////////////////////////

HINSTANCE CHookCallBack::m_hInstDll=NULL;
CString CHookCallBack::Dll_File=_T("\\HookHelper.dll");

typedef BOOL (CALLBACK* LPFNKEYBOARDPROC)(WPARAM, KBDLLHOOKSTRUCT*);
static BOOL CALLBACK KeyboardProc(WPARAM action, KBDLLHOOKSTRUCT* pKeyStruct);

//Hook Mouse
void CHookCallBack::StartHook(HWND hWnd,BOOL bMouseHook,BOOL bKeyboardHook)
{
	
	m_Hwnd=hWnd;
	CString appPath;
	CUtility::GetInstance()->GetAppParentPath(appPath);
	CString dllPath=appPath+Dll_File;

	//m_hInstDll=LoadLibrary(_T("GlobalHook.dll"));   //MFC
	m_hInstDll=LoadLibrary(dllPath);		//Win32
	if(m_hInstDll==NULL)
	{
		CString msg;
		msg.Format(L"�ļ�<%s>������",dllPath);
		AfxMessageBox(msg);
		return;
	}

	/////////////////////////////////////��깳��////////////////////////////////////////////
	if(bMouseHook)
	{
		typedef BOOL (CALLBACK *HookFun)(HWND hWnd); 
		HookFun LowMouseHook;

		//��ȡ�����е���������
		LowMouseHook=(HookFun)::GetProcAddress(m_hInstDll,"StartLowMouseHook");
		if(LowMouseHook==NULL)
		{
			AfxMessageBox(_T("func LowMouseHook not found!"));
			return;
		}

		//��������
		if (LowMouseHook(hWnd))
		{
			TRACE("Hook LowMouseHook Ok\n");

		}
		else
		{
			TRACE("Hook LowMouseHook Error\n");	
		}
	}

	////////////////////////////////���̹���/////////////////////////////////////
	if(bKeyboardHook)
	{
		typedef BOOL (CALLBACK *HookFunEx)(HWND hWnd,LPFNKEYBOARDPROC lpKeybord); 
		HookFunEx LowKeyboardHook;
		LowKeyboardHook=(HookFunEx)::GetProcAddress(m_hInstDll,"StartLowKeyboardHook");
		if(LowKeyboardHook==NULL)
		{
			AfxMessageBox(_T("func LowKeyboardHook not found!"));
			return;
		}

		//////////////////////////////////
		if (LowKeyboardHook(hWnd,&KeyboardProc))
			//if (LowKeyboardHook(this->m_hWnd))
		{
			TRACE("Hook LowKeyboardHook Ok\n");

		}
		else
		{
			TRACE("Hook LowKeyboardHook Error\n");	
		}
	}
	///////////////////////////////////

}

//Unhook Mouse
void CHookCallBack::StopHook(BOOL bMouseHook,BOOL bKeyboardHook)
{
	typedef VOID (CALLBACK *HookFun)(); 
	HookFun LowMouseHook;
	HookFun LowKeyboardHook;

	CString appPath;
	CUtility::GetInstance()->GetAppParentPath(appPath);
	CString dllPath=appPath+Dll_File;


	if(bMouseHook)
	{
		m_hInstDll=LoadLibrary(dllPath);
		LowMouseHook=(HookFun)::GetProcAddress(m_hInstDll,"StopLowMouseHook");
		if(LowMouseHook==NULL)
		{
			TRACE("Stop LowMouseHook Error");
		}
		else
		{
			LowMouseHook();
			TRACE("Stop LowMouseHook Ok");	
		}
	}

	//////////////////////////////////////////////////////////////
	if(bKeyboardHook)
	{
		LowKeyboardHook=(HookFun)::GetProcAddress(m_hInstDll,"StopLowKeyboardHook");
		if(LowKeyboardHook==NULL)
		{
			TRACE("Stop LowKeyboardHook Error");
		}
		else
		{
			LowKeyboardHook();
			TRACE("Stop LowKeyboardHook Ok");	
		}
		

		if (m_hInstDll!=NULL)
		{
			::FreeLibrary(m_hInstDll);
		}
	}

}



//////////////////////////
// ȫ�ּ��̹��ӻص�����  
// ����: action ��ʶ������Ϣ(����,����), keyStruct ����������Ϣ  
// ���� true ��ʾ�������ݰ�����Ϣ ,���� false ��ʾ����������Ϣ����  
BOOL CALLBACK CHookCallBack::KeyboardProc(WPARAM action, KBDLLHOOKSTRUCT* pKeyStruct)  
{  
	CString sPrintScreenPath;
	CSetting::GetPrintScreenAppPath(sPrintScreenPath);
	WCHAR swPath[MAX_PATH];
	CHAR sPath[MAX_PATH];
	wcscpy(swPath,sPrintScreenPath);

	CUtility::GetInstance()->WcharToChar(swPath,sPath);

	// �жϰ�������  
	switch (action)  
	{  
	case WM_KEYDOWN:  
		 if (WM_SYSKEYDOWN)  //�������Ϊ����״̬
		 {
			    //if (Key_Info->vkCode == VK_LWIN || Key_Info->vkCode == VK_RWIN) //���� WIN(��) ��
			 //if (pKeyStruct->vkCode == VK_LWIN )
			 //{
				// TRACE("Win");
				// return false;
			 //}
			 if(pKeyStruct->vkCode == VK_LWIN && GetKeyState(VK_CONTROL) & 0x8000) //���� CTRL + WIN(��) ��ϼ�
			 //if(pKeyStruct->vkCode == VK_LWIN) //���� CTRL + WIN(��) ��ϼ�
			 {
				  TRACE("Ctrl+Win");
				  ::SendMessage(m_Hwnd, WM_SWITCH_SHOW_MSG,0,0);
				
				  return false;
			 }

		 }
		 
		break;  
	case WM_KEYUP:  

		if(pKeyStruct->vkCode==VK_SNAPSHOT)
		{
			CString sPath2(sPath);
			if (CUtility::GetInstance()->PathIsExist(sPath2))
			{
				WinExec(sPath,SW_SHOW);
				//ShellExecute(NULL,L"open",szAppPath,NULL,NULL, SW_SHOWNORMAL); 
			}
			else
			{
				CString msg;
				msg.Format(L"·����<%s>��Ч",swPath);
				AfxMessageBox(msg);
			}
			
		}
		break;  
	case WM_SYSKEYDOWN:  

		break;  
	case WM_SYSKEYUP:  

		break;  

	}  


	return true;  
}  


////�˺���δʹ��
//LRESULT CMyTaskbarDlg::OnKeyboardMsg(WPARAM wParam,LPARAM lParam)
//{
//	if(WM_KEYUP == wParam) //256 down ,257 up
//	{	
//
//		PKBDLLHOOKSTRUCT pKeyboardHookStruct = (PKBDLLHOOKSTRUCT) lParam;	
//		if(pKeyboardHookStruct->vkCode==VK_SNAPSHOT)
//		{
//			WCHAR str1[256]; 
//			int i=(int)pKeyboardHookStruct->vkCode;
//			wsprintf(str1,L"vkCode=%d\n",i);
//
//			TRACE(str1);
//		}
//
//
//	}
//
//	
//	return 1;
//}
