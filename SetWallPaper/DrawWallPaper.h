#pragma once

#include"../stdafx.h"
#include <vector>



using std::vector;
using namespace Gdiplus;

//////////////////////////////////


////////////////////////////

class CDrawWallPaper
{
public:
	CDrawWallPaper(int CanvasWidth,int CanvasHeight);
	~CDrawWallPaper(void);

	 bool CreateStartWallPaper(CString &sWallPaper);
	 bool CreateWallPapers();
	 CString CreateImage(const CString & sPhotoPath,CString &sWallPaperSaveDir);
	 void DrawWindow(HWND hWnd,Graphics &g);

	 void Draw(Graphics &g,CString sPhotoPath);
	 void DwawBackground(Graphics &g,Rect &rec);
	 void DrawPhoto( Graphics &g, const CString &sImgPath);
	 void DrawCalender(Graphics &g);
	 void DrawWeatherInfo(Graphics &g);
	 void DrawShadow( Graphics &g, Rect rec);
	 void DrawRoundPath(GraphicsPath &gp,Rect &rec, int iR);
	 

	//��ȡͼƬ��ʽ����
	 int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);  
	 //��ȡ���ֳߴ�
	 SizeF GetTextBounds(const Gdiplus::Font& font,const StringFormat& strFormat,const CString& szText);



public:


	//static CString GetWallPaperDir(){return m_sWallPaperDir;}

private:
	//static CString m_sPhotoFolder;
	//static CString m_sWallPaperFolder;

	 CString m_sPhotoDir;
	 CString m_sWallPaperDir;

	  int  m_iCanvasWidth;
	  int  m_iCanvasHeight;

   //��Ƭ��������Ļ���˵ļ�϶
     int  m_iDpTopMargin;
   //��Ƭ��������Ļ�׶˵ļ�϶
     int  m_iDpBottomMargin;
   //��Ƭ��������Ļ�Ҷ˵ļ�϶
     int  m_iDpLeftMargin;


   //��Ƭ����������
     int  m_iDpWidth;
   //��Ƭ��������߶�
     int  m_iDpHeight;
   //��Ƭ��������϶
     int  m_iPhotoVsCalenderMargin;


   //�ճ�����Ƭ�����ļ�϶
     int  m_iDpAndScheduleMargin;
   //�ճ̿��
     int  m_iScheduleWidth;
   //�ճ�����߶�
     int  m_iScheduleFontHeight;


   //��Ƭ��Ĭ�Ͽ�Ⱥ͸߶ȣ��߶�ͨ�������� //Ĭ��Ϊ240*338
   int  m_iPhotoHeight; 
   int  m_iPhotoWidth;

   REAL  m_dPhotoRate;  //��Ƭ�����

   //��Ƭ����λ��
     int  m_iStartPosX;
     int  m_iStartPosY;

   //��Ƭ�ڱ߿��
     int  m_PhotoBound;
   //��Ƭ�ڱ���ɫ
     Color m_PhotoBoundColor;

   //�����ߴ�
     int  m_iCalendarWidth;
     int  m_iCalendarHeight;
     int  m_iDateHeight;


   //----------  ����ͼƬ  ---------------
    // REAL  m_iDayPhotoLeftMargin  = 400
    // REAL  m_iDayPhotoHeight  = 0
    // REAL  m_iDayPhotoWidth  = m_iCanvasWidth - m_iDayPhotoLeftMargin * 2

    //

    // Const DAYS_IMAGE_EXNAME As String = ".Jpg"
    // Const BIRTHDAY_IMAGE As String = "����.Jpg"


	 //����
	 int m_iWeatherWidth;
	 int m_iWeatherHeight;

};

