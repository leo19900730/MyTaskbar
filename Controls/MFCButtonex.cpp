// MFCButtonex.cpp : ʵ���ļ�
//

#include "../stdafx.h"
//#include "GDIPlusButton.h"
#include "MFCButtonex.h"
#include "../MyTaskbar.h"



// MFCButtonex

IMPLEMENT_DYNAMIC(MFCButtonex, CMFCButton)

//MFCButtonex::MFCButtonex()
//{
//
//}

MFCButtonex::MFCButtonex()
	//:m_imgNormal(NULL),m_imgHover(NULL)
{
	

}

MFCButtonex::~MFCButtonex()
{
	//delete m_imgNormal;
	//delete m_imgHover;
}


BEGIN_MESSAGE_MAP(MFCButtonex, CMFCButton)
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()



// MFCButtonex ��Ϣ�������




void MFCButtonex::OnSetFocus(CWnd* pOldWnd)
{
	//CMFCButton::OnSetFocus(pOldWnd);
	//����ȡ����ť��ȡ����ʱ�����߿�
	// TODO: �ڴ˴������Ϣ����������
}


void MFCButtonex::ShowImage(HBITMAP hBmp,HBITMAP hBmpHot)
{
	//HBITMAP hBmp=NULL, hBmpHot=NULL;  

	//m_imgNormal=new Bitmap(sImgNormal);
	//m_imgHover=new Bitmap(sImgNormal);

	//HBITMAP hBmp = ::LoadBitmap(NULL, MAKEINTRESOURCE(iImgNormal));
	//HBITMAP hBmpHot = ::LoadBitmap(NULL, MAKEINTRESOURCE(iImgHover));

	//m_imgNormal->GetHBITMAP(NULL, &hBmp);  
	//m_imgHover->GetHBITMAP(NULL, &hBmpHot);  
	//htisSetDlgItemText(IDC_CLOSE,L"");

	if(hBmp==NULL) Logger->Info(L"hBmp��ʼ��ʧ�ܣ�");
	if(hBmpHot==NULL) Logger->Info(L"hBmpHot��ʼ��ʧ�ܣ�");

	this->SetWindowText(L"");
	this->m_bTransparent = TRUE;
	this->SetImage(hBmp, TRUE, hBmpHot); 	
}