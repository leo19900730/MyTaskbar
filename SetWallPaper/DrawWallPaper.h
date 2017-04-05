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
	 

	//获取图片格式编码
	 int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);  
	 //获取文字尺寸
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

   //相片日历在屏幕顶端的间隙
     int  m_iDpTopMargin;
   //相片日历在屏幕底端的间隙
     int  m_iDpBottomMargin;
   //相片日历在屏幕右端的间隙
     int  m_iDpLeftMargin;


   //相片日历整体宽度
     int  m_iDpWidth;
   //相片日历整体高度
     int  m_iDpHeight;
   //照片与日历间隙
     int  m_iPhotoVsCalenderMargin;


   //日程与相片日历的间隙
     int  m_iDpAndScheduleMargin;
   //日程宽度
     int  m_iScheduleWidth;
   //日程字体高度
     int  m_iScheduleFontHeight;


   //相片的默认宽度和高度，高度通过计算获得 //默认为240*338
   int  m_iPhotoHeight; 
   int  m_iPhotoWidth;

   REAL  m_dPhotoRate;  //照片长宽比

   //相片插入位置
     int  m_iStartPosX;
     int  m_iStartPosY;

   //相片遮边宽度
     int  m_PhotoBound;
   //相片遮边颜色
     Color m_PhotoBoundColor;

   //日历尺寸
     int  m_iCalendarWidth;
     int  m_iCalendarHeight;
     int  m_iDateHeight;


   //----------  节日图片  ---------------
    // REAL  m_iDayPhotoLeftMargin  = 400
    // REAL  m_iDayPhotoHeight  = 0
    // REAL  m_iDayPhotoWidth  = m_iCanvasWidth - m_iDayPhotoLeftMargin * 2

    //

    // Const DAYS_IMAGE_EXNAME As String = ".Jpg"
    // Const BIRTHDAY_IMAGE As String = "生日.Jpg"


	 //天气
	 int m_iWeatherWidth;
	 int m_iWeatherHeight;

};

