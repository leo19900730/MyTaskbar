#include "../StdAfx.h"
#include "Utility.h"
#include <locale.h>

////////////////////////////////////////////
#include "shlwapi.h"
#pragma comment(lib,"Shlwapi.lib")
///////////////////////////////////////////////////

//CUtility* CUtility::m_cutility=NULL;


CUtility::CUtility(void)
{
}


CUtility::~CUtility(void)
{
}

//typedef CString (*pFun1)(CString & sPhotoPath,vector<CString> & vecWallPaper);
 CUtility* CUtility::GetInstance()
 {
	 static CUtility cutility;
	 return &cutility;
 }

	

///////////////////////////////////////////////////////////////	
/////////////////////////随机数字///////////////////////////////
bool CUtility::GetRandom(int min,int max,vector<int> &vecNum)
{
	//vector<int> vec;
	int iRandNum;
	int count=max-min+1;

	srand((unsigned)time(NULL));      //随机种子
	

	for(int i=0; i<count; i++)          //产生随机数  
	{  
		iRandNum=rand()%(max-min+1)+min;
		vecNum.push_back(iRandNum);
	}  
	return TRUE;
}

bool CUtility::GetRandomNotRepeat(int min,int max,vector<int> &vecNum)
{
		
	int count=max-min+1;

	vector<int> vec1;
	vector<int>::iterator it;
	int i, iRandNum, iNum;

	//填充指定范围的数据
	for(i=min; i<(max+1); i++)          //产生随机数  
	{  
		vec1.push_back(i);
	}  

	srand((unsigned)time(NULL));      //随机种子
	

	int iRmin=0;
	int iRmax=count-1;

	for(i=0; i<count; i++)          //产生随机数  
	{  
		iRandNum=rand()%(iRmax-iRmin+1)+iRmin;   //此随机值范围为0~vec1.size
		it=vec1.begin()+iRandNum;				//获取随机迭代指针			
		iNum=*it;                               //随机值

		vecNum.push_back(iNum);
		vec1.erase(it);
		iRmax--;
	}  


	return true;
}
///////////////////////////////////////////////////////////////	


/////////////////////////////////////////////////////////////
/////////////////////// Path ////////////////////////////////

BOOL CUtility::PathIsExist(CString sFilePath)
{
	if (PathFileExists(sFilePath)) 
	{
		return true;
	}

	return false;
}

//BOOL CUtility::PathIsExist(LPCSTR sFilePath)
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

CString CUtility::GetParentDirectory(const CString &FilePath)
{
	int Where;
	Where=FilePath.ReverseFind('\\');  //反向查找

	if(Where==-1)
	{
		Where=FilePath.ReverseFind('/');
	}

	CString sParentDirectory=FilePath.Left(Where);
	
	return sParentDirectory;
}

CString CUtility::GetPathName(CString FilePath)
{
	int Where;
	Where=FilePath.ReverseFind('\\');  //反向查找

	//可能是相对路径
	if(Where==-1)
	{
		Where=FilePath.ReverseFind('/');
	}

	CString FileTitle=FilePath.Right(FilePath.GetLength()-1-Where);

	if(GetFileAttributes(FilePath)!=FILE_ATTRIBUTE_DIRECTORY)
	{
		Where=FileTitle.ReverseFind('.');
		FileTitle=FileTitle.Left(Where);
	}

		
	
	return FileTitle;
}


CString CUtility::GetExtendName(CString FilePath)
{
	int Where;
	Where=FilePath.ReverseFind('.');
	CString ExtendName=FilePath.Right(FilePath.GetLength()-Where-1);

	return ExtendName;
}

////////////////////////////////////////////////////

/////////////////////转换//////////////////////////////
BOOL CUtility::AscToUni(const LPCSTR lpAsc,LPWSTR lpUni)
{
	//LPCSTR *p="中华人民共和国";
	int Len=::MultiByteToWideChar (CP_ACP, 0, lpAsc, -1, NULL, 0);
	//LPWSTR *buf=new LPWSTR [Len];
	MultiByteToWideChar(CP_ACP,0,lpAsc,-1,lpUni,Len);

	return true;
}

int CUtility::GetAscLenght(const LPCSTR lpAsc)
{
	int len=::MultiByteToWideChar (CP_ACP, 0, lpAsc, -1, NULL, 0);
	return len;
}

int CUtility::WcharToChar(const LPCWSTR wp, LPSTR m_char)  
{     
    int len= WideCharToMultiByte(CP_ACP,0,wp,wcslen(wp),NULL,0,NULL,NULL);  
    //m_char=new char[len+1];  
    WideCharToMultiByte(CP_ACP,0,wp,wcslen(wp),m_char,len,NULL,NULL);  
    m_char[len]='\0';
	len=GetAscLenght(m_char);

    return len;  
}  


void  CUtility::StringSplit(const CString inStr,vector<CString> & outVec,char splitchar)
{
	int len=inStr.GetLength()-1;
	int iPos1=0;
	int iPos2=inStr.Find(splitchar,iPos1);

	if(!iPos2>iPos1) return;

	while(iPos2!=-1 && iPos2<=len )
	{
		CString str=inStr.Mid(iPos1,iPos2-iPos1);
		iPos1=iPos2+1;
		iPos2=inStr.Find(splitchar,iPos1);
		outVec.push_back(str);
	}

	
	if(iPos2<len) 
	{
		CString str=inStr.Right(len+1-iPos1);
		if(str!=L"")outVec.push_back(str);
	}

}


//wchar_t* CharToWchar(const char* c)  
//{  
//   wchar_t *m_wchar;
//    int len = MultiByteToWideChar(CP_ACP,0,c,strlen(c),NULL,0);  
//    m_wchar=new wchar_t[len+1];  
//    MultiByteToWideChar(CP_ACP,0,c,strlen(c),m_wchar,len);  
//    m_wchar[len]='\0';  
//    return m_wchar;  
//} 
////////////////////////////////////////////////////


////////////////////////File///////////////////////////

void CUtility::GetFiles(const CString &sDirPath,vector<CString> &vecFiles,BOOL bSubDir)
{
	CFileFind FileFinder;  
	CString strFileName;
	BOOL ret = FileFinder.FindFile(sDirPath); 

	while (ret)  
	{  
		
		if (ret != 0)
		{
			ret = FileFinder.FindNextFile();

			//是否递归子目录
			if (FileFinder.IsDirectory() && !FileFinder.IsDots()&& bSubDir)
			{
				CString path = FileFinder.GetFilePath();
				GetFiles(path, vecFiles,bSubDir);
			}
			else if (!FileFinder.IsDirectory() && !FileFinder.IsDots())
			{
				//CString name = FileFinder.GetFileName();
				CString path = FileFinder.GetFilePath();
				vecFiles.push_back(path);
			}

		} 
	}
	FileFinder.Close();

}



void CUtility::DeleteDirectory(const CString &sDirPath,BOOL bSubDir)
{
	CFileFind FileFinder;  
	CString strSubDir,strPath,strFileName;
	BOOL ret = FileFinder.FindFile(sDirPath); 

	while (ret)  
	{  		
		if (ret != 0)
		{	ret = FileFinder.FindNextFile();

			//是否递归子目录
			if (FileFinder.IsDirectory() && !FileFinder.IsDots()&& bSubDir)
			{
				strSubDir = FileFinder.GetFilePath();
				DeleteDirectory(strSubDir, bSubDir);
			}
			else if (!FileFinder.IsDirectory() && !FileFinder.IsDots())
			{
				//strFileName = FileFinder.GetFileName();
				strPath = FileFinder.GetFilePath();
				if(CUtility::PathIsExist(strPath))
				{
					DeleteFile(strPath);
				}
			}

		} 
	}
	FileFinder.Close();


}


BOOL CUtility::ReadTextLine(vector<CString> &vecList,const CString sTxtFile)
{
	//设置中文环境
	char *old_locale=_strdup(setlocale(LC_CTYPE,NULL));
	setlocale(LC_CTYPE,"chs");


	CStdioFile file;
	CString szTemp,szA[1024],szB[1024];
	file.Open(sTxtFile,CFile::modeRead);

	for(int i=0; file.ReadString(szTemp); i++ )
	{
		szTemp.Delete(0, szTemp.Find(L"\t")+1);
		szA[i] = szTemp.Mid(0, szTemp.Find(L"\t")).Trim();
		szB[i] = szTemp.Mid(szTemp.Find(L"\t")+1).Trim();

		if(!szTemp.IsEmpty())
		{
			vecList.push_back(szTemp);
		}
		
	}
	file.Close();

	//还原语言环境
	setlocale(LC_CTYPE,old_locale);
	free(old_locale);

	return true;
}

BOOL CUtility::WriteText(vector<CString> &vecList,const CString sTxtFile)
{
	//设置中文环境
	char *old_locale=_strdup(setlocale(LC_CTYPE,NULL));
	setlocale(LC_CTYPE,"chs");

	//char* pszFileName="C:\\myfile.txt"
	CStdioFile csFile;
	CFileException fileException;

	if(csFile.Open(sTxtFile,CFile::typeText|CFile::modeCreate|CFile::modeReadWrite),&fileException)

	{
		vector<CString>::iterator it;
		for(it=vecList.begin();it<vecList.end();it++)
		{
			csFile.WriteString(*it);
			if(it!=(vecList.end()-1)) csFile.WriteString(L"\n"); //最后一行不换行
		}
	}
	else
	{
		TRACE("Can't open file %s,error=%u\n",sTxtFile,fileException.m_cause);
		return false;
	}

	csFile.Close();

	//还原语言环境
	setlocale(LC_CTYPE,old_locale);
	free(old_locale);

	return true;
}


/////////////////////////////////System//////////////////////////////////////////////////////

CString& CUtility::GetModulePath(CString &sAppPath)
{
	HMODULE module = GetModuleHandle(0); 
	WCHAR pFileName[MAX_PATH]; 
	GetModuleFileName(module, pFileName, MAX_PATH); 

	sAppPath=pFileName;
	return sAppPath;
}


 CString& CUtility::GetAppParentPath(CString &sAppPath)
{
	HMODULE module = GetModuleHandle(0); 
	WCHAR pFileName[MAX_PATH]; 
	GetModuleFileName(module, pFileName, MAX_PATH); 

	CString sModulePath=pFileName;
	int where=sModulePath.ReverseFind(L'\\');

	sAppPath=sModulePath.Left(where);
	return sAppPath;
}


//解析快捷方式：
HRESULT CUtility::GetPathForLink( LPCWSTR lpszLinkFile, LPWSTR lpszPath,HWND hwnd)
{
	HRESULT hres;
	IShellLink* psl;
	TCHAR szGotPath[MAX_PATH];
	//TCHAR szDescription[MAX_PATH];
	WIN32_FIND_DATA wfd;

	*lpszPath=0;//assume failure

	CoInitialize(NULL); 
	//Get a pointer to the  IShellLink   interface.  
	hres=CoCreateInstance(CLSID_ShellLink,NULL,CLSCTX_INPROC_SERVER, IID_IShellLink,(void**)&psl);
	if(SUCCEEDED(hres)){
		IPersistFile *ppf;

		//GetapointertotheIPersistFileinterface.
		hres=psl->QueryInterface(IID_IPersistFile, (void**)&ppf);
		if(SUCCEEDED(hres))
		{	
			hres=ppf->Load(lpszLinkFile,STGM_READ);
			if(SUCCEEDED(hres))
			{
				//Resolvethelink.
				hres=psl->Resolve(hwnd,SLR_ANY_MATCH);
				if(SUCCEEDED(hres))
				{
					//Getthepathtothelinktarget.
					hres=psl->GetPath(szGotPath,MAX_PATH,(WIN32_FIND_DATA*)&wfd,SLGP_SHORTPATH);
			
					TCHAR strLongPath[MAX_PATH] ={0};  
					GetLongPathName(szGotPath, strLongPath, sizeof(strLongPath)/sizeof(TCHAR));
					lstrcpy(lpszPath,strLongPath);

					//Getthedescriptionofthetarget.
					//hres=psl->GetDescription(szDescription,MAX_PATH);
					//lstrcpy(lpszPath,szGotPath);
				}
			}
			ppf->Release();
		}
		psl->Release();
	}
	return hres;
}