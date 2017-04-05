#include "../StdAfx.h"
#include <locale.h>
//#include <fstream>



#include "PathHelper.h"



CPathHelper::CPathHelper(void)
{
}


CPathHelper::~CPathHelper(void)
{
}

////////////////////////////////////////////////////////

//CString CPathHelper::GetPathName(CString FilePath)
//{
//	int Where;
//	Where=FilePath.ReverseFind('\\');  //反向查找
//
//	if(Where==-1)
//	{
//		Where=FilePath.ReverseFind('/');
//	}
//
//	CString FileTitle=FilePath.Right(FilePath.GetLength()-1-Where);
//	
//	return FileTitle;
//}
//
//
//CString CPathHelper::GetExtendName(CString FilePath)
//{
//	int Where;
//	Where=FilePath.ReverseFind('.');
//	CString ExtendName=FilePath.Right(FilePath.GetLength()-Where-1);
//
//	return ExtendName;
//}








//BOOL CPathHelper::PathIsExist(LPCSTR sFilePath)
//{
//	TCHAR sPath[MAX_PATH];
//
//	AscToUni(sFilePath,sPath);
//
//	if (PathFileExists(sPath)) 
//	{
//		return true;
//	}
//
//	return false;
//}
//////////////////////////////////////////////////////


///////////////////////转换//////////////////////////////
//BOOL CPathHelper::AscToUni(const LPCSTR lpAsc,LPWSTR lpUni)
//{
//	//LPCSTR *p="中华人民共和国";
//	int Len=::MultiByteToWideChar (CP_ACP, 0, lpAsc, -1, NULL, 0);
//	//LPWSTR *buf=new LPWSTR [Len];
//	MultiByteToWideChar(CP_ACP,0,lpAsc,-1,lpUni,Len);
//
//	return true;
//}
//
//int CPathHelper::GetAscLenght(const LPCSTR lpAsc)
//{
//	int len=::MultiByteToWideChar (CP_ACP, 0, lpAsc, -1, NULL, 0);
//	return len;
//}



//BOOL AscToUni(const LPCSTR lpAsc,LPCWSTR lpUni)
//{
//	//LPCSTR *p="中华人民共和国";
//	int Len=::MultiByteToWideChar (CP_ACP, 0, lpAsc, -1, NULL, 0);
//	//LPWSTR *buf=new LPWSTR [Len];
//	MultiByteToWideChar(CP_ACP,0,lpAsc,-1,lpUni,Len)
//
//	return true;
//
//}


//////////////////////////////////////////////////


//BOOL CPathHelper::ReadTextLine(vector<CString> &vecList,CString sTxtFile)
//{
//	//设置中文环境
//	char *old_locale=_strdup(setlocale(LC_CTYPE,NULL));
//	setlocale(LC_CTYPE,"chs");
//
//
//	CStdioFile file;
//	CString szTemp,szA[1024],szB[1024];
//	file.Open(sTxtFile,CFile::modeRead);
//
//	for(int i=0; file.ReadString(szTemp); i++ )
//	{
//		szTemp.Delete(0, szTemp.Find(L"\t")+1);
//		szA[i] = szTemp.Mid(0, szTemp.Find(L"\t")).Trim();
//		szB[i] = szTemp.Mid(szTemp.Find(L"\t")+1).Trim();
//
//		vecList.push_back(szTemp);
//	}
//	file.Close();
//
//	//还原语言环境
//	setlocale(LC_CTYPE,old_locale);
//	free(old_locale);
//
//	return true;
//}

