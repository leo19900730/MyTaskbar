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

	////��ȡ�ļ���
	//static CString GetPathName(CString sFilePath);
	////��ȡ��չ��
	//static CString GetExtendName(CString FilePath);


	//�ж��ļ���Ŀ¼�Ƿ����
	//static BOOL PathIsExist(CString sFilePath);
	//static BOOL PathIsExist(LPCSTR sFilePath);

	//static BOOL AscToUni(const LPCSTR lpAsc,LPWSTR lpUni);
	//static int GetAscLenght(const LPCSTR lpAsc);

	//���ж�ȡTxt�ļ�
	/*static BOOL CPathHelper::ReadTextLine(vector<CString> &vec,CString sTxtFile);*/


};

