
// MyTaskbar.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "DLL\DynamicLogger.h"


// CMyTaskbarApp:
// �йش����ʵ�֣������ MyTaskbar.cpp
//



class CMyTaskbarApp : public CWinApp
{
public:
	CMyTaskbarApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()

	/////////////////////////////////GDIPlus/////////////////////////////////////
	 ULONG_PTR m_gdiplusToken;  //�����ڹر�ʱ�����ݸ�GdiplusShutdown
	 /////////////////////////////////GDIPlus/////////////////////////////////////

	 virtual int ExitInstance();
};

extern CMyTaskbarApp theApp;
extern CDynamicLogger Logger;