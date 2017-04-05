#pragma once
class CHookCallBack
{

private:
	static HWND m_Hwnd;
	
public:
	CHookCallBack(void);
	~CHookCallBack(void);

	static CString Dll_File;
	static HINSTANCE m_hInstDll;

	static void StartHook(HWND hWnd,BOOL bMouseHook,BOOL bKeyboardHook);
	static void StopHook(BOOL bMouseHook,BOOL bKeyboardHook);

	//static LRESULT MouseProc(WPARAM wParam,LPARAM lParam);
	static BOOL CALLBACK KeyboardProc(WPARAM action, KBDLLHOOKSTRUCT* pKeyStruct);
	//static StopHook();

};

