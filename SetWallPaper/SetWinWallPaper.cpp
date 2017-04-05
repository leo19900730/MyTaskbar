#include "../StdAfx.h"
#include "SetWinWallPaper.h"

#include <time.h>
#include "Utility.h"
#include "Setting.h"
#include "CalculateTime.h"
#include "../MyTaskbar.h"

///////////////////////////

#define WALL_PAPER_LOOP   


/////////////////////////////////////////
//DWORD WINAPI NewThreadFun(LPVOID lpParam);  

CEvent g_EnableWallPaperEvent;

///////////////////////////////////////////////////////

CDrawWallPaper *SetWinWallPaper::m_pDrawWallPaper=NULL;

bool SetWinWallPaper::m_bWallPaperCreated=false;
bool SetWinWallPaper::m_bWallPaperCreateing=false;

int SetWinWallPaper::m_iWallPaperCount=0;
int SetWinWallPaper::m_iWallPaperIndex=0;
vector<CString> SetWinWallPaper::m_vecWallPaper;
vector<int> SetWinWallPaper::m_vecRndNum;

CString SetWinWallPaper::m_sToday=L"";
int SetWinWallPaper::m_iWallPaperSwitchTime=2000;
bool SetWinWallPaper::m_WallPaperThreadRun=true;
///////////////////////////////////////////////////////




//////////////////////////////////////////////////////

SetWinWallPaper::SetWinWallPaper(void)
{
	if(m_pDrawWallPaper==NULL)
	{
		int ScreenWidth= GetSystemMetrics(SM_CXSCREEN);
		int ScreenHeight=GetSystemMetrics(SM_CYSCREEN);
		m_pDrawWallPaper=new CDrawWallPaper(ScreenWidth,ScreenHeight);	
	}

	g_EnableWallPaperEvent.SetEvent();   //设置事件为发信状态,取消线程等待
}


SetWinWallPaper::~SetWinWallPaper(void)
{
	delete m_pDrawWallPaper;
}

////////////////////子线程//////////////////////////
//此处除了建立第一张壁纸并显示之外，
//还负责建立所以其他壁纸，并初始化壁纸文件的路径
int SetWinWallPaper::ThreadSetStartWallPaper()  
{  
	AfxBeginThread((AFX_THREADPROC)NewThreadFun1, this); 
	return 0;  
}  

  

//设置壁纸 
UINT WINAPI SetWinWallPaper::NewThreadFun1(LPVOID lpParam)  
{  
	SetWinWallPaper * ptrData = (SetWinWallPaper *)lpParam;  
	if(ptrData->SetStartWallpaper())
	{
		Logger->Info(L"设置开始壁纸成功");	
	}
	else
	{
		Logger->Info(L"设置开始壁纸失败");	
	}
	

#ifdef WALL_PAPER_LOOP

	SetWinWallPaper * _this = (SetWinWallPaper *)lpParam;  
	m_iWallPaperSwitchTime=CSetting::GetWallPaperSwitchTime();

	//循环显示壁纸
	while(m_WallPaperThreadRun)
	{

		//如果g_EnableWallPaperEvent无信号，则无限等待
		WaitForSingleObject(g_EnableWallPaperEvent,INFINITE);  		

		_this->SetMyWallpaper();

		CString stime=GetFormatToday(true);
		Logger->Info(L"--壁纸时间--：%s",stime);	

		Sleep(m_iWallPaperSwitchTime);
		g_EnableWallPaperEvent.SetEvent();
	}

#endif

	return 0;  
} 

void SetWinWallPaper::ResetWallPaper()
{
	//m_WallPaperThreadRun=true;
	g_EnableWallPaperEvent.SetEvent();
}

void SetWinWallPaper::StopWallPaper()
{
	//m_WallPaperThreadRun=false;
	g_EnableWallPaperEvent.Lock();
}

void SetWinWallPaper::ExitWallPaper()
{
	g_EnableWallPaperEvent.SetEvent();
	m_WallPaperThreadRun=false;
}
/////////////////////////////////////////////

bool SetWinWallPaper::SetStartWallpaper()
{
	CString sWallPaper;
	m_sToday=GetFormatToday();
	Logger->Info(L"壁纸日期：%s",m_sToday);	

	if(m_pDrawWallPaper->CreateStartWallPaper(sWallPaper))
	{
		SetMyWallpaper(sWallPaper,WPSTYLE_STRETCH);
		if(CreateWallpapers())
		{
			Logger->Info(L"建立壁纸成功!");	
		}
		else
		{
			Logger->Info(L"建立壁纸失败!");	
		}

	}
		
	return true;
}

bool SetWinWallPaper::CreateWallpapers()
{
	
	//建立壁纸 
	if(!m_pDrawWallPaper->CreateWallPapers())
	{
		return false;
	}
	else
	{
		InitWallPapers();
		m_bWallPaperCreated=true;
	}
	return true;	
}


//初始化要显示的壁纸
bool SetWinWallPaper::InitWallPapers()
{

	//获取壁纸目录下的图片
	CString sTmp,sWallPaperDir;
	CSetting::GetWallPaperFolder(sTmp);
	sWallPaperDir=sTmp+L"\\*.bmp";
	CUtility::GetInstance()->GetFiles(sWallPaperDir,m_vecWallPaper);

	if(m_vecWallPaper.empty()) return false;

	//建立随机数
	m_iWallPaperCount=m_vecWallPaper.size();
	CUtility::GetInstance()->GetRandomNotRepeat(0,m_iWallPaperCount-1,m_vecRndNum);
	m_iWallPaperIndex=0;

	return true;
}



bool SetWinWallPaper::SetMyWallpaper()
{	

	//如果是跨日期，则重新建立壁纸
	CString sToday=GetFormatToday();
	if(m_sToday!=sToday)
	{
		m_sToday=sToday;
		m_bWallPaperCreated=false;
		CreateWallpapers();
	}

	if(!m_bWallPaperCreated) return false;

	CString sWallPaperPath=m_vecWallPaper.at(m_vecRndNum.at(m_iWallPaperIndex));
	if(!CUtility::GetInstance()->PathIsExist(sWallPaperPath)) return false;

	Logger->Info(L"壁纸路径：%s，壁纸index：%d",sWallPaperPath,m_vecRndNum.at(m_iWallPaperIndex));
		
	//CCalculateTime ct;
	//ct.StartCalculateTime();

	//设置
	SetMyWallpaper(sWallPaperPath,WPSTYLE_STRETCH);

	++m_iWallPaperIndex;
	if(m_iWallPaperIndex>m_iWallPaperCount-1)
	{
		m_iWallPaperIndex=0;
	}

	//TRACE("设置屏保时间：%lf\n",ct.GetCountTime());
	return true;

}





//函数一：

//strPicFile是图像文件名,支持BMP JPEG GIF等格式
//dwStyle是墙纸的样式
//WPSTYLE_CENTER 居中 0
//WPSTYLE_TILE 平铺 1
//WPSTYLE_STRETCH 拉伸 2
//WPSTYLE_MAX 3
//返回值是TRUE时墙纸设置成功，返回FALSE时失败
bool SetWinWallPaper::SetMyWallpaper(CString &strPicFile, DWORD dwStyle)
{
	CCalculateTime ct1;
	ct1.StartCalculateTime();

	HRESULT hr;
	IActiveDesktop* pIAD;

	//Applications must initialize the COM library before they can call COM library functions
	CoInitialize(NULL); 

	//创建接口的实例
	hr = CoCreateInstance (CLSID_ActiveDesktop,  NULL, CLSCTX_INPROC_SERVER,      
		IID_IActiveDesktop, (void**) &pIAD );
	if(!SUCCEEDED(hr)) return FALSE;
	
	strPicFile.ReleaseBuffer();

	//设置墙纸
	hr = pIAD->SetWallpaper(strPicFile, 0);
	if(!SUCCEEDED(hr)) return FALSE;

	//设置墙纸的样式
	WALLPAPEROPT wpo;
	wpo.dwSize = sizeof(wpo);
	wpo.dwStyle = dwStyle;
	hr = pIAD->SetWallpaperOptions(&wpo, 0);
	if(!SUCCEEDED(hr)) return FALSE;

	//应用墙纸的设置
	hr = pIAD->ApplyChanges(AD_APPLY_ALL);
	if(!SUCCEEDED(hr)) return FALSE;
		
	//释放接口的实例
	pIAD->Release();
	CoUninitialize();

	TRACE("屏保设置时间：%lf\n",ct1.GetCountTime());
	return TRUE;

	
}
//获取当前日期
 CString SetWinWallPaper::GetFormatToday(bool bAndTime)
 {
	  SYSTEMTIME st;//定义系统时间结构体的对象
	  GetLocalTime(&st);//调用GetLocalTime获得当前时间，并保存在结构体st中

	  CString sToday;
	  if(bAndTime)
	  {
		sToday.Format(L"%d-%02d-%02d-%02d-%02d-%02d",st.wYear ,st.wMonth,st.wDay,
						st.wHour,st.wMinute,st.wSecond );
	  }
	  else
	  {
		sToday.Format(L"%d-%02d-%02d",st.wYear ,st.wMonth,st.wDay);
	  }

	  return sToday;

 }


////////////////////////////////////////////////////////////////////////////////

