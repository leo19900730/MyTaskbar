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

	g_EnableWallPaperEvent.SetEvent();   //�����¼�Ϊ����״̬,ȡ���̵߳ȴ�
}


SetWinWallPaper::~SetWinWallPaper(void)
{
	delete m_pDrawWallPaper;
}

////////////////////���߳�//////////////////////////
//�˴����˽�����һ�ű�ֽ����ʾ֮�⣬
//������������������ֽ������ʼ����ֽ�ļ���·��
int SetWinWallPaper::ThreadSetStartWallPaper()  
{  
	AfxBeginThread((AFX_THREADPROC)NewThreadFun1, this); 
	return 0;  
}  

  

//���ñ�ֽ 
UINT WINAPI SetWinWallPaper::NewThreadFun1(LPVOID lpParam)  
{  
	SetWinWallPaper * ptrData = (SetWinWallPaper *)lpParam;  
	if(ptrData->SetStartWallpaper())
	{
		Logger->Info(L"���ÿ�ʼ��ֽ�ɹ�");	
	}
	else
	{
		Logger->Info(L"���ÿ�ʼ��ֽʧ��");	
	}
	

#ifdef WALL_PAPER_LOOP

	SetWinWallPaper * _this = (SetWinWallPaper *)lpParam;  
	m_iWallPaperSwitchTime=CSetting::GetWallPaperSwitchTime();

	//ѭ����ʾ��ֽ
	while(m_WallPaperThreadRun)
	{

		//���g_EnableWallPaperEvent���źţ������޵ȴ�
		WaitForSingleObject(g_EnableWallPaperEvent,INFINITE);  		

		_this->SetMyWallpaper();

		CString stime=GetFormatToday(true);
		Logger->Info(L"--��ֽʱ��--��%s",stime);	

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
	Logger->Info(L"��ֽ���ڣ�%s",m_sToday);	

	if(m_pDrawWallPaper->CreateStartWallPaper(sWallPaper))
	{
		SetMyWallpaper(sWallPaper,WPSTYLE_STRETCH);
		if(CreateWallpapers())
		{
			Logger->Info(L"������ֽ�ɹ�!");	
		}
		else
		{
			Logger->Info(L"������ֽʧ��!");	
		}

	}
		
	return true;
}

bool SetWinWallPaper::CreateWallpapers()
{
	
	//������ֽ 
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


//��ʼ��Ҫ��ʾ�ı�ֽ
bool SetWinWallPaper::InitWallPapers()
{

	//��ȡ��ֽĿ¼�µ�ͼƬ
	CString sTmp,sWallPaperDir;
	CSetting::GetWallPaperFolder(sTmp);
	sWallPaperDir=sTmp+L"\\*.bmp";
	CUtility::GetInstance()->GetFiles(sWallPaperDir,m_vecWallPaper);

	if(m_vecWallPaper.empty()) return false;

	//���������
	m_iWallPaperCount=m_vecWallPaper.size();
	CUtility::GetInstance()->GetRandomNotRepeat(0,m_iWallPaperCount-1,m_vecRndNum);
	m_iWallPaperIndex=0;

	return true;
}



bool SetWinWallPaper::SetMyWallpaper()
{	

	//����ǿ����ڣ������½�����ֽ
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

	Logger->Info(L"��ֽ·����%s����ֽindex��%d",sWallPaperPath,m_vecRndNum.at(m_iWallPaperIndex));
		
	//CCalculateTime ct;
	//ct.StartCalculateTime();

	//����
	SetMyWallpaper(sWallPaperPath,WPSTYLE_STRETCH);

	++m_iWallPaperIndex;
	if(m_iWallPaperIndex>m_iWallPaperCount-1)
	{
		m_iWallPaperIndex=0;
	}

	//TRACE("��������ʱ�䣺%lf\n",ct.GetCountTime());
	return true;

}





//����һ��

//strPicFile��ͼ���ļ���,֧��BMP JPEG GIF�ȸ�ʽ
//dwStyle��ǽֽ����ʽ
//WPSTYLE_CENTER ���� 0
//WPSTYLE_TILE ƽ�� 1
//WPSTYLE_STRETCH ���� 2
//WPSTYLE_MAX 3
//����ֵ��TRUEʱǽֽ���óɹ�������FALSEʱʧ��
bool SetWinWallPaper::SetMyWallpaper(CString &strPicFile, DWORD dwStyle)
{
	CCalculateTime ct1;
	ct1.StartCalculateTime();

	HRESULT hr;
	IActiveDesktop* pIAD;

	//Applications must initialize the COM library before they can call COM library functions
	CoInitialize(NULL); 

	//�����ӿڵ�ʵ��
	hr = CoCreateInstance (CLSID_ActiveDesktop,  NULL, CLSCTX_INPROC_SERVER,      
		IID_IActiveDesktop, (void**) &pIAD );
	if(!SUCCEEDED(hr)) return FALSE;
	
	strPicFile.ReleaseBuffer();

	//����ǽֽ
	hr = pIAD->SetWallpaper(strPicFile, 0);
	if(!SUCCEEDED(hr)) return FALSE;

	//����ǽֽ����ʽ
	WALLPAPEROPT wpo;
	wpo.dwSize = sizeof(wpo);
	wpo.dwStyle = dwStyle;
	hr = pIAD->SetWallpaperOptions(&wpo, 0);
	if(!SUCCEEDED(hr)) return FALSE;

	//Ӧ��ǽֽ������
	hr = pIAD->ApplyChanges(AD_APPLY_ALL);
	if(!SUCCEEDED(hr)) return FALSE;
		
	//�ͷŽӿڵ�ʵ��
	pIAD->Release();
	CoUninitialize();

	TRACE("��������ʱ�䣺%lf\n",ct1.GetCountTime());
	return TRUE;

	
}
//��ȡ��ǰ����
 CString SetWinWallPaper::GetFormatToday(bool bAndTime)
 {
	  SYSTEMTIME st;//����ϵͳʱ��ṹ��Ķ���
	  GetLocalTime(&st);//����GetLocalTime��õ�ǰʱ�䣬�������ڽṹ��st��

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

