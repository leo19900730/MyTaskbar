#pragma once

//#include <atlcom.h>
//#include <atlbase.h>
//using namespace ATL;

class CSetting
{
public:
	CSetting(void);
	~CSetting(void);

private:
	static CString m_sAppPath;

	//抓屏软件路径
	static CString m_sPrintScreenAppPath;

	////屏保开关
	static CString m_sWallPaperEnable;

	//链接文件清单
	static CString m_sFileListPath;

	static CString m_sHolidayPath;

	static CString m_sStartPhotoPath;
	//static CString m_sStartWallPaperPath;
	static CString m_sPhotoFolder;
	static CString m_sWallPaperFolder;

	//屏保切换时间
	static int m_iWallPaperSwitchTime;

	//////////////////////////////////
	static CString m_sAppName;
	static CString m_sWallPaperSwtichTime;


public:
	static  CString& GetAppPath();
	static  CString& GetPrintScreenAppPath(CString &sPrintScreenAppPath);

	static  DWORD GetWallPaperEnable();
	static  BOOL SetWallPaperEnable(DWORD val);

	static  CString& GetFileListPath(CString &sFileListPath);
	static  CString& GetHolidayPath(CString &sHolidayPath);

	static  CString& GetSatrtPhotoFolder(CString &sSatrtPhoto);
	static  CString& GetPhotoFolder(CString &sPhotoFolder);
	static  CString& GetWallPaperFolder(CString &sWallPaperFolder);

	static int GetWallPaperSwitchTime();
	static void SetWallPaperSwitchTime(int time);

	//////////////////////////////////////////////////////////
	static void InitRegedit();
	static bool GetMainKey(HKEY &hKey);
	static bool GetCurrentUserRunKey(HKEY &hKey);
	static bool GetRegValue(LPCTSTR  sKeyName,DWORD &resVal,const DWORD defVal);
	static bool SetRegValue(LPCTSTR  sValName,const DWORD Val);
	static bool GetRegValue(const HKEY &hMainKey,LPCTSTR  sValName,LPCTSTR retVal,LPCTSTR defVal);
	static bool SetRegValue(const HKEY &hMainKey,LPCTSTR  sValName,const LPCTSTR Val);

};

