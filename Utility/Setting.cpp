#include "../StdAfx.h"
#include "Setting.h"

#include "Utility.h"

////////////////////////////////////////
CString CSetting::m_sAppPath=L"";

CString CSetting::m_sPrintScreenAppPath=L"\\res\\SupportFiles\\搜狗屏幕截图\\screencapture.exe";


CString CSetting::m_sWallPaperEnable=L"启动屏保";

CString CSetting::m_sFileListPath=L"\\res\\SupportFiles\\FileList.txt";

int CSetting::m_iWallPaperSwitchTime=6000;
CString CSetting::m_sHolidayPath=L"\\res\\SupportFiles\\Days.txt";

CString CSetting::m_sStartPhotoPath=L"\\res\\WallPaper\\StartPhoto.jpg";
//CString CSetting::m_sStartWallPaperPath=L"\\res\\WallPaper\\StartPhoto.bmp";
CString CSetting::m_sPhotoFolder=L"\\res\\WallPaper\\Photo";
CString CSetting::m_sWallPaperFolder=L"\\res\\WallPaper\\CurrentWallPaper";

///////////////////////////////////////////
CString CSetting::m_sWallPaperSwtichTime=L"WallPaperSwtichTime";
CString CSetting::m_sAppName=L"MyTaskbar";

////////////////////////////////////////

CSetting::CSetting(void)
{
}


CSetting::~CSetting(void)
{
}


CString& CSetting::GetAppPath()
{
	if(m_sAppPath.IsEmpty())
	{
		return CUtility::GetInstance()->GetAppParentPath(m_sAppPath);
	}
	else
	{
		return m_sAppPath;

	}

}
////////////////////////////////////////////////////////////

CString& CSetting::GetPrintScreenAppPath(CString &sPrintScreenAppPath)
{
	sPrintScreenAppPath=GetAppPath()+m_sPrintScreenAppPath;
	return sPrintScreenAppPath;
}


DWORD CSetting::GetWallPaperEnable()
{
	DWORD ret;
	GetRegValue(m_sWallPaperEnable,ret,(DWORD)1);
	return (DWORD)ret;
}

BOOL CSetting::SetWallPaperEnable(DWORD val)
{
	 //m_iWallPaperSwitchTime=time;  
	 if(SetRegValue(m_sWallPaperEnable,(DWORD)val))
	 {
		 return TRUE;
	 }

	 return FALSE;
}



CString& CSetting::GetFileListPath(CString &sFileListPath)
{
	sFileListPath=GetAppPath()+m_sFileListPath;
	return sFileListPath;
}



CString& CSetting::GetHolidayPath(CString &sHolidayPath)
{
	sHolidayPath=GetAppPath()+m_sHolidayPath;
	return sHolidayPath;
}

CString& CSetting::GetSatrtPhotoFolder(CString &sSatrtPhoto)
{
	sSatrtPhoto=GetAppPath()+m_sStartPhotoPath;
	return  sSatrtPhoto;
}

CString& CSetting::GetPhotoFolder(CString &sPhotoFolder)
{
	sPhotoFolder=GetAppPath()+m_sPhotoFolder;
	return sPhotoFolder;

}

CString& CSetting::GetWallPaperFolder(CString &sWallPaperFolder)
{
	sWallPaperFolder=GetAppPath()+m_sWallPaperFolder;
	return sWallPaperFolder;
}

int  CSetting::GetWallPaperSwitchTime()
{
	DWORD ret;
	GetRegValue(m_sWallPaperSwtichTime,ret,(DWORD)m_iWallPaperSwitchTime);
	return (DWORD)ret;
}

 void CSetting::SetWallPaperSwitchTime(int time)
 {
	 m_iWallPaperSwitchTime=time;  
	 SetRegValue(m_sWallPaperSwtichTime,(DWORD)time);
 }


/////////////////////////注册表/////////////////////////////////
 void CSetting::InitRegedit()
 {

	 //添加自定启动项
	 HKEY hCurrUserRunKey;

	 CString appPath;
	 CUtility::GetInstance()->GetModulePath(appPath);
	 if(!GetCurrentUserRunKey(hCurrUserRunKey)) return ;
	 SetRegValue(hCurrUserRunKey,m_sAppName,appPath);
	 RegCloseKey(hCurrUserRunKey);

 }

bool CSetting::GetMainKey(HKEY &hKey)
{
	LPCTSTR sKeyName=L"Software\\MyTaskbak";
	//访问注册表，hKEY则保存此函数所打开的键的句柄
	long ret0=::RegOpenKeyEx(HKEY_CURRENT_USER,sKeyName,0,KEY_ALL_ACCESS,&hKey);
	if(ret0!=ERROR_SUCCESS)//如果无法打开hKEY,则中止程序的执行
	{
		//AfxMessageBox("错误：无法打开有关的hKEY");
		DWORD dwDisposition;
		if (RegCreateKeyEx(HKEY_CURRENT_USER, sKeyName, 0, REG_NONE,
			REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
			&hKey, &dwDisposition) == ERROR_SUCCESS)
		{
			return true;

		}
		else
		{
			return false;
		}

	}

	return true;

}


bool  CSetting::GetCurrentUserRunKey(HKEY &hKey)
{
	LPCTSTR sKeyName=L"Software\\Microsoft\\Windows\\CurrentVersion\\Run";
	long ret0=::RegOpenKeyEx(HKEY_CURRENT_USER,sKeyName,0,KEY_ALL_ACCESS,&hKey);
	if(ret0==ERROR_SUCCESS)//如果无法打开hKEY,则中止程序的执行
	{
		return true;

	}
	else
	{
		return false;
	}

	return true;

}


bool CSetting::GetRegValue(LPCTSTR  sValName,DWORD &retVal,const DWORD defVal)
{
	bool ret=false;
	LONG lResult;
	HKEY hMainKey; 
	DWORD dwType =REG_DWORD;
	DWORD dwSize=1024; 
	
	if(!GetMainKey(hMainKey)) return false;

	lResult=RegQueryValueEx(hMainKey,sValName, NULL, &dwType,(LPBYTE)&retVal, &dwSize);
	if(!lResult== ERROR_SUCCESS)
	{
		lResult=RegSetValueEx(hMainKey, sValName, NULL, REG_DWORD,(LPBYTE)&defVal, sizeof(defVal));		
		auto err=GetLastError();
		if(lResult==ERROR_SUCCESS)
		{
			retVal=defVal;
			ret=true;
		}
	}
	 RegCloseKey(hMainKey);
	 return ret;

}


bool CSetting::SetRegValue(LPCTSTR  sValName,const DWORD Val)
{
	bool ret=true;
	LONG lResult;
	HKEY hMainKey; 
	DWORD dwType =REG_DWORD;
	DWORD dwSize=1024; 
		
	if(!GetMainKey(hMainKey)) return false;		
	
	lResult=RegSetValueEx(hMainKey, sValName, NULL, REG_DWORD,(LPBYTE)&Val, sizeof(Val));

	if(!lResult==ERROR_SUCCESS) 
	{
		ret=false;
	}

	 RegCloseKey(hMainKey);
	 return ret;

}

//////////////////////////字符///////////////////////////////////
bool CSetting::GetRegValue(const HKEY &hMainKey,LPCTSTR  sValName,LPCTSTR retVal,LPCTSTR defVal)
{
	bool ret=true;
	LONG lResult;
	DWORD dwType =REG_SZ;
	DWORD dwSize=1024; 
	
	lResult=RegQueryValueEx(hMainKey,sValName, NULL, &dwType,(LPBYTE)retVal, &dwSize);
	if(!lResult== ERROR_SUCCESS)
	{
		SetRegValue(hMainKey,sValName,defVal);
		if(lResult==ERROR_SUCCESS)
		{
			retVal=defVal;
			//wcscpy(retVal,defVal);
			ret=true;
		}
	}

	 return ret;

}

bool CSetting::SetRegValue(const HKEY &hMainKey,LPCTSTR  sValName,const LPCTSTR Val)
{
	bool ret=false;
	LONG lResult;
					
	lResult=RegSetValueEx(hMainKey, sValName, NULL, REG_SZ,(LPBYTE)Val,  (wcslen(Val)+1)*sizeof(LPCTSTR));		
	if(!lResult==ERROR_SUCCESS)
	{
		ret=false;
	}

	 return ret;

}