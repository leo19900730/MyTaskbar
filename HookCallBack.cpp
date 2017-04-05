#include "StdAfx.h"
#include "HookCallBack.h"
#include "PathHelper.h"
#include "Utility.h"
#include "Setting.h"

#define WM_SWITCH_SHOW_MSG WM_USER + 310 //自定义消息，用于和主程序通信

 HWND CHookCallBack::m_Hwnd=NULL;
 


CHookCallBack::CHookCallBack(void)
{
}


CHookCallBack::~CHookCallBack(void)
{
}


///////////////////////////////////////////////////////
//////////////////////钩子相关////////////////////////

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
		msg.Format(L"文件<%s>不存在",dllPath);
		AfxMessageBox(msg);
		return;
	}

	/////////////////////////////////////鼠标钩子////////////////////////////////////////////
	if(bMouseHook)
	{
		typedef BOOL (CALLBACK *HookFun)(HWND hWnd); 
		HookFun LowMouseHook;

		//获取钩子中的启动函数
		LowMouseHook=(HookFun)::GetProcAddress(m_hInstDll,"StartLowMouseHook");
		if(LowMouseHook==NULL)
		{
			AfxMessageBox(_T("func LowMouseHook not found!"));
			return;
		}

		//启动钩子
		if (LowMouseHook(hWnd))
		{
			TRACE("Hook LowMouseHook Ok\n");

		}
		else
		{
			TRACE("Hook LowMouseHook Error\n");	
		}
	}

	////////////////////////////////键盘钩子/////////////////////////////////////
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
// 全局键盘钩子回调函数  
// 参数: action 标识键盘消息(按下,弹起), keyStruct 包含按键信息  
// 返回 true 表示继续传递按键消息 ,返回 false 表示结束按键消息传递  
BOOL CALLBACK CHookCallBack::KeyboardProc(WPARAM action, KBDLLHOOKSTRUCT* pKeyStruct)  
{  
	CString sPrintScreenPath;
	CSetting::GetPrintScreenAppPath(sPrintScreenPath);
	WCHAR swPath[MAX_PATH];
	CHAR sPath[MAX_PATH];
	wcscpy(swPath,sPrintScreenPath);

	CUtility::GetInstance()->WcharToChar(swPath,sPath);

	// 判断按键动作  
	switch (action)  
	{  
	case WM_KEYDOWN:  
		 if (WM_SYSKEYDOWN)  //如果按键为按下状态
		 {
			    //if (Key_Info->vkCode == VK_LWIN || Key_Info->vkCode == VK_RWIN) //屏敝 WIN(左) 键
			 //if (pKeyStruct->vkCode == VK_LWIN )
			 //{
				// TRACE("Win");
				// return false;
			 //}
			 if(pKeyStruct->vkCode == VK_LWIN && GetKeyState(VK_CONTROL) & 0x8000) //屏敝 CTRL + WIN(左) 组合键
			 //if(pKeyStruct->vkCode == VK_LWIN) //屏敝 CTRL + WIN(左) 组合键
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
				msg.Format(L"路径：<%s>无效",swPath);
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


////此函数未使用
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
