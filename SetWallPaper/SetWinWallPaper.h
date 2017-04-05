#pragma once

#include "../stdafx.h"
#include <vector>
#include "DrawWallPaper.h"

using std::vector;




class SetWinWallPaper
{
public:
	SetWinWallPaper(void);
	~SetWinWallPaper(void);

	int ThreadSetStartWallPaper();
	int ThreadSetWallPaper();


	static UINT WINAPI NewThreadFun1(LPVOID lpParam);

	static bool SetStartWallpaper();
	static bool CreateWallpapers();
	static bool InitWallPapers();
	static bool SetMyWallpaper();
	static bool SetMyWallpaper(CString &strPicFile, DWORD dwStyle);

	static CString GetFormatToday(bool bAndTime=false);
	static void ResetWallPaper();
	static void StopWallPaper();
	static void ExitWallPaper();
	
	

	/////////////////////////////////////////////////////////////////////////////////
	static void SetWallPaperSwitchTime(int iWallPaperSwitchTime){m_iWallPaperSwitchTime=iWallPaperSwitchTime;};

private:
	static CDrawWallPaper *m_pDrawWallPaper;

	static bool m_bWallPaperCreated;    //壁纸是否已建立
	static bool m_bWallPaperCreateing;
	static int m_iWallPaperCount;
	static int m_iWallPaperIndex;

	static vector<CString> m_vecWallPaper;
	static vector<int> m_vecRndNum;

	static CString m_sToday;   //保存首次运行时的日期

	static int m_iWallPaperSwitchTime;
	static bool m_WallPaperThreadRun;

	

};


