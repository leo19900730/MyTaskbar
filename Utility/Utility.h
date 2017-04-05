#pragma once

#include <vector>
using std::vector;


class CUtility
{
private:
	CUtility(void);
	~CUtility(void);

	//static CUtility* m_cutility;

public:
	static CUtility* GetInstance();

//private:	
	//生成随机一组不重复的函数
	bool GetRandomNotRepeat(int min,int max,vector<int> &vecNum);
	//生成随机一组函数
	bool GetRandom(int min,int max,vector<int> &vecNum);

	///////////////////////////////IO//////////////////////////////////////	
	//判断文件或目录是否存在
	BOOL PathIsExist(CString sFilePath);
	//BOOL PathIsExist(LPCSTR sFilePath);

	//单字节字符串转多字节
	BOOL AscToUni(const LPCSTR lpAsc,LPWSTR lpUni);
	//多字节转单字节字符串
	int WcharToChar(const LPCWSTR wp, LPSTR m_char);
	//获取单字节字符串长度
	int GetAscLenght(const LPCSTR lpAsc);
	//分割字符串
	void  StringSplit(const CString inStr,vector<CString> & outVec ,char splitchar='|');

	
	//获取路径的父目录
	CString GetParentDirectory(const CString &FilePath);

	//获取目录下的所有文件，
	//sDirPath 路径
	//
	void GetFiles(const CString &sDirPath,vector<CString> &vecFiles,BOOL bSubDir=FALSE);

	//删除目录下所有文件
	void DeleteDirectory(const CString &sDirPath,BOOL bSubDir=FALSE);


	//按行读取文本文件
	BOOL ReadTextLine(vector<CString> &vecList,const CString sTxtFile);

	BOOL WriteText(vector<CString> &vecList,const CString sTxtFile);

	//获取文件名
	CString GetPathName(CString sFilePath);
	//获取扩展名
	CString GetExtendName(CString FilePath);


	/////////////////////////////////////System//////////////////////////////////////
	//获取程序路径
	CString& GetModulePath(CString &sAppPath);
	//获取程序父目录
	CString& GetAppParentPath(CString &sAppPath);
	//获取快捷方式指向的程序路径
	HRESULT GetPathForLink(LPCWSTR lpszLinkFile, LPWSTR lpszPath,HWND hwnd=NULL);



};

