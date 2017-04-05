#include "../StdAfx.h"
#include "DrawWallPaper.h"

#include "Calendar.h"
#include "CalculateTime.h"
#include "Utility.h"
#include "Setting.h"

#include "../MyTaskbarDlg.h"
//#define TEST



////////////////////////////////////////////////////////////
/*
'+----------------------------------------------+
'|                    +-----+ +-------+ +-----+ |    
'|                    |     | |       | |photo| |
'|                    |wea  | |       | |     | |
'|                    |     | | 日程  | +-----+ |
'|                    +-----+ |       | +-----+ |      
'|                            |       | |日历 | |
'|                            |       | |     | |
'|                            +-------+ +-----+ |
'|                                              |
'+----------------------------------------------+

*/



/////////////////////////////////////////



//构造函数
CDrawWallPaper::CDrawWallPaper(int CanvasWidth,int CanvasHeight)
{
	//m_iScreenWidth= (REAL)GetSystemMetrics(SM_CXFULLSCREEN);
	//m_iScreenHeight= (REAL)GetSystemMetrics(SM_CYFULLSCREEN);

	m_iCanvasWidth= CanvasWidth;
	m_iCanvasHeight=CanvasHeight;


	//相片日历在屏幕顶端的间隙
	m_iDpTopMargin  = 20;
	//相片日历在屏幕底端的间隙
	m_iDpBottomMargin  = 50;
	//相片日历在屏幕右端的间隙
	m_iDpLeftMargin  = 15;


	//相片日历整体宽度
	m_iDpWidth  = 0;
	//相片日历整体高度
	m_iDpWidth=m_iCanvasHeight-m_iDpTopMargin-m_iDpBottomMargin;
	//m_iPhotoHeight

	//照片与日历间隙
	m_iPhotoVsCalenderMargin  = 10;
	//相片的默认宽度和高度，高度通过计算获得 //默认为240*338
	m_iPhotoHeight=(m_iDpWidth-m_iPhotoVsCalenderMargin)/2;
	m_iPhotoWidth  = 0;
	m_dPhotoRate=1;

	//相片插入位置
	m_iStartPosX  = 0;
	m_iStartPosY  = m_iDpTopMargin;
	//相片遮边宽度
	m_PhotoBound  = 5;
	//相片遮边颜色
	m_PhotoBoundColor=Color(250, 248, 250, 205);

	//日程与相片日历的间隙
	m_iDpAndScheduleMargin  = 10;
	//日程宽度
	m_iScheduleWidth  = 320;
	//日程字体高度
	m_iScheduleFontHeight  = 15;


	//日历尺寸
	m_iCalendarWidth  = 0;
	m_iCalendarHeight  = 0;
	m_iDateHeight  = 20;


	m_iStartPosX = m_iCanvasWidth - m_iDpWidth - m_iDpLeftMargin;
	m_iStartPosY = m_iDpTopMargin;

	//----------  节日图片  ---------------
	// int  m_iDayPhotoLeftMargin  = 400
	// int  m_iDayPhotoHeight  = 0
	// int  m_iDayPhotoWidth  = m_iCanvasWidth - m_iDayPhotoLeftMargin * 2

	//

	// Const DAYS_IMAGE_EXNAME As String = ".Jpg"
	// Const BIRTHDAY_IMAGE As String = "生日.Jpg"

	//HMODULE module = GetModuleHandle(0); 
	//WCHAR pFileName[MAX_PATH]; 
	//GetModuleFileName(module, pFileName, MAX_PATH); 
	//CString sModulePath=pFileName;
	//int where=sModulePath.ReverseFind(L'\\');
	//sModulePath=sModulePath.Left(where);

	//CString sModulePath;
	//CUtility::GetInstance()->GetAppParentPath(sModulePath);
			
	//m_sPhotoDir=sModulePath+m_sPhotoFolder;
	//m_sWallPaperDir=sModulePath+m_sWallPaperFolder;

	//天气
	 m_iWeatherWidth=180;
	 m_iWeatherHeight=150;

	CSetting::GetPhotoFolder(m_sPhotoDir);
	CSetting::GetWallPaperFolder(m_sWallPaperDir);

}


CDrawWallPaper::~CDrawWallPaper(void)
{
}

bool CDrawWallPaper::CreateStartWallPaper(CString &sWallPaper)
{
	CString sMsg;
	CString sStartPhotoPath;
	CSetting::GetSatrtPhotoFolder(sStartPhotoPath);
	
	if (!PathFileExists(sStartPhotoPath)) 
	{
		sMsg.Format(L"文件<%s>不存在！\n",sStartPhotoPath);
		::AfxMessageBox(sMsg);
		return false;
	}

	CString sParentDirectory=CUtility::GetInstance()->GetParentDirectory(sStartPhotoPath);
	sWallPaper=CreateImage(sStartPhotoPath,sParentDirectory);
		
	return true;
}


bool CDrawWallPaper::CreateWallPapers()
{
	
	CString sMsg;
	
	if (!PathFileExists(m_sPhotoDir)) 
	{
		sMsg.Format(L"目录<%s>不存在！\n",m_sPhotoDir);
		::AfxMessageBox(sMsg);
		return false;
	}
	

	if (!PathFileExists(m_sWallPaperDir)) 
	{
		sMsg.Format(L"目录<%s>不存在！",m_sWallPaperDir);
		::AfxMessageBox(sMsg);
		return false;
	}
		

	//获取照片
	CString path=m_sPhotoDir +L"\\*.jpg";
	vector<CString> vecPhotoFiles;
	CUtility::GetInstance()->GetFiles(path,vecPhotoFiles);
	if(vecPhotoFiles.empty())
	{
		sMsg.Format(L"目录<%s>没有照片！",m_sPhotoDir);
		::AfxMessageBox(sMsg);
		return false;
	}

	//删除原有文件
	CString sDelDir=m_sWallPaperDir+"\\*.*";
	CUtility::GetInstance()->DeleteDirectory(sDelDir);

	//建立壁纸，返回壁纸路径
	/////////////////////////
	//CCalculateTime ct1;
	//ct1.StartCalculateTime();
	///////////////////////////
	vector<CString>::iterator it;
	/*CString saveDir=L"";*/
	for(it=vecPhotoFiles.begin();it<vecPhotoFiles.end();it++)
	{
		//CCalculateTime ct2;
		//ct2.StartCalculateTime();		
		CreateImage(*it,m_sWallPaperDir);

		//TRACE("建立一张图片使用时间：%lf\n",ct2.GetCountTime());
	}
	//TRACE("所有图片使用时间：%lf\n",ct1.GetCountTime());
	



	return true;
}


CString CDrawWallPaper::CreateImage(const CString & sPhotoPath,CString &sWallPaperSaveDir)
{
	
	CString sPhotoName=CUtility::GetInstance()->GetPathName(sPhotoPath);
	CString path=sWallPaperSaveDir +L"\\"+ sPhotoName +L".bmp";
	

	//建立画布
	Bitmap map(m_iCanvasWidth,m_iCanvasHeight);
	Graphics g((Image*)&map);
	g.SetSmoothingMode(SmoothingModeAntiAlias);
	g.SetPageUnit(UnitPixel);        ;

	Draw(g,sPhotoPath);
	
	//获取图片编码
	CLSID pngClsid;
	GetEncoderClsid(L"image/bmp", &pngClsid);
	//GetEncoderClsid(L"image/png", &pngClsid);
	auto result=map.Save(path, &pngClsid,NULL);
	
	return path;

}


void CDrawWallPaper::DrawWindow(HWND hWnd,Graphics &g)
{
	/////////////////////////////////////////////
	//int cx = GetSystemMetrics(SM_CXFULLSCREEN);
	//   int cy = GetSystemMetrics(SM_CYFULLSCREEN);


	//CDC* pDC = GetDC();
	//   Graphics g(pDC->m_hDC);



	//绘制灰底  
	/*   SolidBrush brush(Color(111,111,111));  
	g.FillRectangle(&brush, rt.left, rt.top, rt.right, rt.bottom);  */

	//Rect rec(rt.left, rt.top, rt.right, rt.bottom);



	////设置画图时的滤波模式为消除锯齿现象  
	//   g.SetSmoothingMode(SmoothingModeAntiAlias);  
	// 
	//   //创建一个红色的画笔  
	//   //Pen *pen = new Pen(pens,1);  
	//Pen pen(Color(188,88,88),3); 

	//Bitmap map(rec.Width,rec.Height);
	//Graphics *gImg= Graphics::FromImage(&map);

	Draw(g,L"./res/001.jpg");

}

void CDrawWallPaper::Draw(Graphics &g,CString sPhotoPath)
{
	Rect rec(0, 0, m_iCanvasWidth, m_iCanvasHeight);
	DwawBackground(g,rec);

	//DrawPhoto(g,L"./res/bg.jpg");
	DrawPhoto(g,sPhotoPath);
	DrawCalender(g);
	DrawWeatherInfo(g);
}

void CDrawWallPaper::DwawBackground(Graphics &g,Rect &rec)
{
	Color col1(6, 39, 220);
	Color col2(6, 10, 86);

	//SolidBrush brush(col1); 
	//g.FillRectangle(&brush, rec);

	LinearGradientBrush lgb(rec ,col1,col2,45);
	g.FillRectangle(&lgb, rec);

	//Image *img=Image::FromFile(L"./res/bg.jpg");
	//   g.DrawImage(img, rec);


}


void CDrawWallPaper::DrawPhoto( Graphics &g, const CString &sImgPath)
{
	Image *img=Image::FromFile(sImgPath);
	REAL iPhotoWidth=(REAL)(img->GetWidth());
	REAL iPostoHeight=(REAL)(img->GetHeight());
	m_dPhotoRate = iPhotoWidth / iPostoHeight;
	m_iPhotoWidth =(int)(m_iPhotoHeight * m_dPhotoRate);
	m_iDpWidth=m_iPhotoWidth;
	m_iCalendarWidth = m_iPhotoWidth;

	//起始X坐标
	m_iStartPosX=m_iCanvasWidth-m_iPhotoWidth-m_iDpLeftMargin;


	Rect recPhoto(m_iStartPosX, m_iStartPosY, m_iPhotoWidth, m_iPhotoHeight);

	//绘制阴影
	DrawShadow(g, recPhoto);


	//绘制相片
	g.DrawImage(img, recPhoto);


	//绘制边框
	Rect rec2 = recPhoto;
	Size sz(-m_PhotoBound, -m_PhotoBound);
	rec2.Inflate(sz);

	Region reg1(recPhoto);
	Region reg2(rec2);
	reg1.Exclude(&reg2);
	SolidBrush sb(m_PhotoBoundColor);
	g.FillRegion(&sb, &reg1);

}


//'绘制日历
void CDrawWallPaper::DrawCalender(Graphics &g)
{
	int dDateHeight=130;
	int strMargin=5;    //文字内容边距
	int dStrWidth=25;	//填充文字的矩形宽度
	int dStrHeight=dDateHeight;

	int iCalendarX = m_iStartPosX;
	int iCalendarY = m_iStartPosY + m_iPhotoHeight + m_iPhotoVsCalenderMargin;
	m_iCalendarHeight=m_iPhotoHeight;
	Rect recDate(iCalendarX, iCalendarY, m_iCalendarWidth, m_iCalendarHeight);

	//绘制阴影
	DrawShadow(g, recDate);

	//'绘制填色矩形作为日历背景
	Color col1(253, 101, 0);
	Color col2(180, 47, 9);
	LinearGradientBrush lb(recDate, col1, col2, 45, true);
	g.FillRectangle(&lb, recDate);


	//设置字体
	FontFamily fontFamily(L"黑体");			
	Gdiplus::Font font1(&fontFamily, (REAL)m_iDateHeight, FontStyleBold, UnitPixel);
	//SolidBrush myBrush(Color::White);
	Color cloBak(220, 220, 220);
	SolidBrush myBrush(cloBak);
	//设置竖格式
	StringFormat drawFormat;
	drawFormat.SetFormatFlags(StringFormatFlagsDirectionVertical);  //竖排
	drawFormat.SetAlignment(StringAlignmentCenter);     //居中
	drawFormat.SetLineAlignment(StringAlignmentCenter);

	//'----------绘制日期------------    

	CTime tm( CTime::GetCurrentTime());
	CCalendar calendar(tm.GetYear(),tm.GetMonth(),tm.GetDay());

	//绘制新历年
	CString sDrawStr;
	sDrawStr.Format(L"%d年%d月",tm.GetYear(),tm.GetMonth()); 
	int iPosX= iCalendarX + strMargin, iPosY = iCalendarY;
	RectF *recStr=&RectF((REAL)iPosX,(REAL)iPosY,(REAL)dStrWidth,(REAL)dStrHeight);		
	g.DrawString(sDrawStr, sDrawStr.GetLength(), &font1,*recStr,&drawFormat,&myBrush);

#ifdef TEST
	g.DrawRectangle(&Pen(Color(255,255,255),1),*recStr);
#endif

	//'绘制农历年月

	sDrawStr =calendar.GetLunarYear();  //获取农历年
	iPosX = recDate.GetRight()-dStrWidth- strMargin;
	recStr=&RectF((REAL)iPosX,(REAL)iPosY,(REAL)dStrWidth,(REAL)dStrHeight);
	g.DrawString(sDrawStr, sDrawStr.GetLength(), &font1,*recStr,&drawFormat,&myBrush);

#ifdef TEST
	g.DrawRectangle(&Pen(Color(255,255,255),1),*recStr);
#endif

	//'绘制日期
	sDrawStr =calendar.GetLunarMonthAndDay() ; //显示当前日期    
	iPosX = iPosX-dStrWidth;
	recStr=&RectF((REAL)iPosX,(REAL)iPosY,(REAL)dStrWidth,(REAL)dStrHeight);
	g.DrawString(sDrawStr, sDrawStr.GetLength(), &font1,*recStr,&drawFormat,&myBrush);

#ifdef TEST
	g.DrawRectangle(&Pen(Color(255,255,255),1),*recStr);
#endif

	//'绘制日期
	StringFormat drawFormat2;
	drawFormat2.SetAlignment(StringAlignmentCenter);     //居中
	drawFormat2.SetLineAlignment(StringAlignmentCenter);  


	//显示当前日期
	sDrawStr.Format(L"%d",tm.GetDay());		
	//Dim iFontHeight As Integer = m_iCalendarHeight / 5;
	Gdiplus::Font font2(&fontFamily, (REAL)70, FontStyleBold, UnitPixel);
	//msr = _graphics.MeasureString(sDrawStr, font);
	dStrWidth=120;
	dStrHeight=90;
	iPosX = iCalendarX + (m_iCalendarWidth - dStrWidth) / 2 ;
	iPosY = iCalendarY;
	recStr=&RectF((REAL)iPosX,(REAL)iPosY,(REAL)dStrWidth,(REAL)dStrHeight);
	g.DrawString(sDrawStr, sDrawStr.GetLength(), &font2,*recStr,&drawFormat2,&myBrush);

#ifdef TEST
	g.DrawRectangle(&Pen(Color(255,255,255),1),*recStr);
#endif

	//'绘制星期
	sDrawStr =calendar.GetWeek();		
	dStrWidth=80;
	dStrHeight=25;
	iPosX = iCalendarX + (m_iCalendarWidth - dStrWidth) / 2;
	iPosY = iCalendarY+dDateHeight-strMargin-dStrHeight;
	recStr=&RectF((REAL)iPosX,(REAL)iPosY,(REAL)dStrWidth,(REAL)dStrHeight);
	g.DrawString(sDrawStr, sDrawStr.GetLength(), &font1,*recStr,&drawFormat2,&myBrush);

#ifdef TEST
	g.DrawRectangle(&Pen(Color(255,255,255),1),*recStr);
#endif

	//'绘制节日底色
	int iDayY =iCalendarY+dDateHeight;
	Color col3(30, 255, 255, 255);

	//'计算节日底色高度，
	int iHeight=m_iCalendarHeight - dDateHeight;

	int iMargin=2;
	recStr=&RectF((REAL)(recDate.X + iMargin), (REAL)iDayY, (REAL)(recDate.Width - iMargin * 2), (REAL)(iHeight - iMargin * 2));
	g.FillRectangle(&SolidBrush(col3), *recStr);


	//'绘制节日
	sDrawStr = calendar.GetHoliday();
	if (!sDrawStr.IsEmpty()) 
	{	
		Gdiplus::Font font3(&fontFamily, (REAL)m_iDateHeight-5, FontStyleBold, UnitPixel);
		recStr->Inflate(-3,-3);
		g.DrawString(sDrawStr, sDrawStr.GetLength(), &font3,*recStr,&drawFormat2,&myBrush);
	}

#ifdef TEST
	g.DrawRectangle(&Pen(Color(255,255,255),1),*recStr);
#endif



}

void CDrawWallPaper::DrawWeatherInfo(Graphics &g)
{
	if(CMyTaskbarDlg::m_weatherInfo==NULL) return;


	if(!CMyTaskbarDlg::m_bInitWeatherInfoSuccess)
	{
		WaitForSingleObject(CMyTaskbarDlg::m_pWinThread->m_hThread,10000);
	}

	CString sDrawStr;
	Color col(30, 255, 255, 255);
	Color cloBak(220, 220, 220);
	FontFamily fontFamily(L"黑体");			
	Gdiplus::Font font1(&fontFamily, (REAL)m_iDateHeight, FontStyleBold, UnitPixel);		
	SolidBrush myBrush(cloBak);

		//绘制天气
	int iWeatherX=m_iStartPosX-m_iWeatherWidth-m_iDpLeftMargin;
	int iWeatherY=m_iStartPosY;
	RectF *recStr=&RectF((REAL)iWeatherX, iWeatherY, (REAL)m_iWeatherWidth, (REAL)m_iWeatherHeight);
	g.FillRectangle(&SolidBrush(col), *recStr);

	sDrawStr.Format(L"%s\n\n%s\n%s\n%s\n%s",
		CMyTaskbarDlg::m_weatherInfo->City,
		CMyTaskbarDlg::m_weatherInfo->Weather,
		CMyTaskbarDlg::m_weatherInfo->Temperature,
		CMyTaskbarDlg::m_weatherInfo->WindPower,
		CMyTaskbarDlg::m_weatherInfo->Visibility);

	
		//'绘制日期
	StringFormat drawFormat3;
	drawFormat3.SetAlignment(StringAlignmentNear);     //居中
	drawFormat3.SetLineAlignment(StringAlignmentCenter);  

	Gdiplus::Font font4(&fontFamily, (REAL)m_iDateHeight-3, FontStyleRegular, UnitPixel);
	recStr->Inflate(-3,-3);
	g.DrawString(sDrawStr, sDrawStr.GetLength(), &font4,*recStr,&drawFormat3,&myBrush);
}


///////////////////////////////////////////

void  CDrawWallPaper::DrawShadow( Graphics &g, Rect rec)
{
	Rect recShadow= rec;
	recShadow.Inflate(6,6);

	GraphicsPath gp;
	DrawRoundPath(gp,recShadow, 6);

	//SolidBrush blackBrush(Color(255, 0, 0, 0));
	//g.FillPath(&blackBrush,&gp);

	//gp.AddRectangle(rec);

	PathGradientBrush pgb(&gp);
	Color colCenter(255, 0, 0, 0);
	pgb.SetCenterColor(colCenter);	


	REAL dShadowX=0.9F;
	REAL dShadowY=.85F/*dShadowX/m_dPhotoRate*/;
	pgb.SetFocusScales(dShadowX, dShadowY);
	//pgb.SetFocusScales(0.9F, 0.7F);


	Color col2(0, 0, 0, 0);
	Color arrCol[]={col2};
	INT count=1;
	pgb.SetSurroundColors(arrCol,&count);
	g.FillPath(&pgb, &gp);

	//gp.Dispose();
	//pgb.Dispose();
}


//绘制圆角路径
void CDrawWallPaper::DrawRoundPath(GraphicsPath &gp,Rect &rec, int iR)
{

	//上直线
	Point p1(rec.X + iR, rec.Y);
	Point p2(rec.GetRight() - iR, rec.Y);
	gp.AddLine(p1, p2);

	////右上圆角
	Rect recTmp1(rec.GetRight() - iR * 2, rec.Y, iR * 2, iR * 2);
	gp.AddArc(recTmp1, 270, 90);

	//右直线
	Point p3(rec.GetRight(), rec.Y + iR);
	Point p4(rec.GetRight(), rec.GetBottom() - iR);
	gp.AddLine(p3, p4);

	//右下圆角
	Rect recTmp2(rec.GetRight() - iR * 2, rec.GetBottom() - iR * 2, iR * 2, iR * 2);
	gp.AddArc(recTmp2, 360, 90);

	//下直线
	Point p5(rec.X + iR, rec.GetBottom());
	Point p6(rec.GetRight() - iR, rec.GetBottom());
	gp.AddLine(p5, p6);

	//左下圆角
	Rect recTmp3(rec.X, rec.GetBottom() - iR * 2, iR * 2, iR * 2);
	gp.AddArc(recTmp3, 90, 90);

	//左直线
	Point p7 (rec.X, rec.Y + iR);
	Point p8(rec.X, rec.GetBottom() - iR);
	gp.AddLine(p7, p8);

	//左上圆角
	Rect recTmp4(rec.X, rec.Y, iR * 2, iR * 2);
	gp.AddArc(recTmp4, 180, 90);

}





///////////////////////////////////////////////////////////////////

/*
@name    GetEncoderClsid 
@brief   取图片类型GLSID 
@param[in]        const WCHAR * format 
@param[in]        CLSID * pClsid 
@return           int 
*/
//GetEncoderClsid(L"image/png", &encoderClsid); //png
//GetEncoderClsid(L"image/bmp", &encoderClsid);
//GetEncoderClsid(L"image/gif", &encoderClsid);
//GetEncoderClsid(L"image/jpeg", &encoderClsid);
//GetEncoderClsid(L"image/tiff", &encoderClsid);
int CDrawWallPaper::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)  
{  
	UINT  num = 0;  
	UINT  size = 0;  
	ImageCodecInfo* pImageCodecInfo = NULL;  
	GetImageEncodersSize(&num, &size);  
	if(size == 0)  
		return -1;  

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));  
	if(pImageCodecInfo == NULL)  
		return -1;  

	GetImageEncoders(num, size, pImageCodecInfo);  
	for(UINT j = 0; j < num; ++j)  
	{  
		if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )  
		{  
			*pClsid = pImageCodecInfo[j].Clsid;  
			free(pImageCodecInfo);  
			return j;  
		}      
	}  
	free(pImageCodecInfo);  
	return -1;  
} 

SizeF CDrawWallPaper::GetTextBounds(const  Gdiplus::Font& font,const StringFormat& strFormat,const CString& szText)
{
	GraphicsPath path;
	FontFamily fontfamily;
	font.GetFamily(&fontfamily);
	path.AddString(szText,-1,&fontfamily,font.GetStyle(),font.GetSize(),PointF(0,0),&strFormat);
	RectF rcBound;
	path.GetBounds(&rcBound);
	return SizeF(rcBound.Width,rcBound.Height);
}



