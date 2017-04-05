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
	//�������һ�鲻�ظ��ĺ���
	bool GetRandomNotRepeat(int min,int max,vector<int> &vecNum);
	//�������һ�麯��
	bool GetRandom(int min,int max,vector<int> &vecNum);

	///////////////////////////////IO//////////////////////////////////////	
	//�ж��ļ���Ŀ¼�Ƿ����
	BOOL PathIsExist(CString sFilePath);
	//BOOL PathIsExist(LPCSTR sFilePath);

	//���ֽ��ַ���ת���ֽ�
	BOOL AscToUni(const LPCSTR lpAsc,LPWSTR lpUni);
	//���ֽ�ת���ֽ��ַ���
	int WcharToChar(const LPCWSTR wp, LPSTR m_char);
	//��ȡ���ֽ��ַ�������
	int GetAscLenght(const LPCSTR lpAsc);
	//�ָ��ַ���
	void  StringSplit(const CString inStr,vector<CString> & outVec ,char splitchar='|');

	
	//��ȡ·���ĸ�Ŀ¼
	CString GetParentDirectory(const CString &FilePath);

	//��ȡĿ¼�µ������ļ���
	//sDirPath ·��
	//
	void GetFiles(const CString &sDirPath,vector<CString> &vecFiles,BOOL bSubDir=FALSE);

	//ɾ��Ŀ¼�������ļ�
	void DeleteDirectory(const CString &sDirPath,BOOL bSubDir=FALSE);


	//���ж�ȡ�ı��ļ�
	BOOL ReadTextLine(vector<CString> &vecList,const CString sTxtFile);

	BOOL WriteText(vector<CString> &vecList,const CString sTxtFile);

	//��ȡ�ļ���
	CString GetPathName(CString sFilePath);
	//��ȡ��չ��
	CString GetExtendName(CString FilePath);


	/////////////////////////////////////System//////////////////////////////////////
	//��ȡ����·��
	CString& GetModulePath(CString &sAppPath);
	//��ȡ����Ŀ¼
	CString& GetAppParentPath(CString &sAppPath);
	//��ȡ��ݷ�ʽָ��ĳ���·��
	HRESULT GetPathForLink(LPCWSTR lpszLinkFile, LPWSTR lpszPath,HWND hwnd=NULL);



};

