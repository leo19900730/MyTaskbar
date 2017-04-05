#pragma once

#include "shlwapi.h"
#include <vector>

#pragma comment(lib,"Shlwapi.lib")

using namespace std;


class CPathHelper
{
public:
	CPathHelper(void);
	~CPathHelper(void);

	////获取文件名
	//static CString GetPathName(CString sFilePath);
	////获取扩展名
	//static CString GetExtendName(CString FilePath);


	//判断文件或目录是否存在
	//static BOOL PathIsExist(CString sFilePath);
	//static BOOL PathIsExist(LPCSTR sFilePath);

	//static BOOL AscToUni(const LPCSTR lpAsc,LPWSTR lpUni);
	//static int GetAscLenght(const LPCSTR lpAsc);

	//按行读取Txt文件
	/*static BOOL CPathHelper::ReadTextLine(vector<CString> &vec,CString sTxtFile);*/


};

