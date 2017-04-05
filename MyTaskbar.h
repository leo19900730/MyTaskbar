
// MyTaskbar.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "DLL\DynamicLogger.h"


// CMyTaskbarApp:
// 有关此类的实现，请参阅 MyTaskbar.cpp
//



class CMyTaskbarApp : public CWinApp
{
public:
	CMyTaskbarApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()

	/////////////////////////////////GDIPlus/////////////////////////////////////
	 ULONG_PTR m_gdiplusToken;  //用于在关闭时，传递给GdiplusShutdown
	 /////////////////////////////////GDIPlus/////////////////////////////////////

	 virtual int ExitInstance();
};

extern CMyTaskbarApp theApp;
extern CDynamicLogger Logger;