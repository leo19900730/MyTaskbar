

#pragma once

#include<vector>
#include <string>
using namespace std;;




struct HolidayInfo
{
	CString sName;
	CString stype;
	CString sLunarDay;
	CString sDay;
};



////////////////////////////////////////////////////////

class CCalendar
{
public:
	CCalendar(WORD iYear, WORD iMonth, WORD iDay);	
	~CCalendar(void);

private:
	WORD m_iYear, m_iMonth, m_iDay;
	static  WORD START_YEAR;
	static   WORD END_YEAR;
	//static   CString m_sHolidayPath;

public:
	//���㹫��iYear��iMonth��iDay�ն�Ӧ����������,���ض�Ӧ���������� 0-24
	//1901��1��1��---2050��12��31��

	//��ȡũ����
	CString GetLunarYear();
	//��ȡũ������
	CString GetLunarMonthAndDay();

	CString GetLunarMonthAndDay(WORD iYear,WORD iMonth,WORD iDay);
	CString GetWeek();
	CString GetHoliday();
	CString GetMonthAndDay();
	CString GetMonthAndDay(WORD iMonth,WORD iDay);
	
	//��ȡ����
	CString GetGetLunarHolDay(WORD iYear, WORD iMonth, WORD iDay);  

	
private:

	WORD GetLunarDate(WORD iYear, WORD iMonth, WORD iDay,
		WORD &iLunarYear, WORD &iLunarMonth, WORD &iLunarDay);

	WORD GetYear(){return m_iYear;}
	WORD GetMonth(){return m_iMonth;}
	WORD GetDay(){return m_iDay;}
	void GetDate(WORD &iYear, WORD &iMonth, WORD &iDay);
	BOOL SetDate(WORD iYear, WORD iMonth, WORD iDay);


	//�ж�iYear�ǲ�������
	BOOL IsLeapYear(WORD iYear)
	{return !(iYear%4)&&(iYear%100) || !(iYear%400);}

	//����iYear,iMonth,iDay��Ӧ�����ڼ� 1��1��1�� --- 65535��12��31��
	WORD WeekDay(WORD iYear, WORD iMonth, WORD iDay);

	//����iYear��iMonth�µ����� 1��1�� --- 65535��12��
	WORD MonthDays(WORD iYear, WORD iMonth);

	//��������iLunarYer������iLunarMonth�µ����������iLunarMonthΪ���£�
	//����Ϊ�ڶ���iLunarMonth�µ��������������Ϊ0 
	// 1901��1��---2050��12��
	LONG LunarMonthDays(WORD iLunarYear, WORD iLunarMonth);

	//��������iLunarYear���������
	// 1901��1��---2050��12��
	WORD LunarYearDays(WORD iLunarYear);

	//��������iLunarYear��������·ݣ���û�з���0
	// 1901��1��---2050��12��
	WORD GetLeapMonth(WORD iLunarYear);

	//��iYear���ʽ������ɼ��귨��ʾ���ַ���
	void FormatLunarYear(WORD  iYear, CHAR *pBuffer);

	//��iMonth��ʽ���������ַ���
	void FormatMonth(WORD iMonth, CHAR *pBuffer, BOOL bLunar = TRUE);

	//��iDay��ʽ���������ַ���
	void FormatLunarDay(WORD  iDay, CHAR *pBuffer);

	//���㹫���������ڼ���������  1��1��1�� --- 65535��12��31��
	LONG CalcDateDiff(WORD iEndYear, WORD iEndMonth, WORD iEndDay,
		WORD iStartYear = START_YEAR, 
		WORD iStartMonth =1, WORD iStartDay =1);



	void   l_InitData();

	//�����1901��1��1�չ�iSpanDays������������
	void   l_CalcLunarDate(WORD &iYear, WORD &iMonth ,WORD &iDay, LONG iSpanDays);

	//���㹫��iYear��iMonth��iDay�ն�Ӧ�Ľ��� 0-24��0���ǽ���
	WORD   l_GetLunarHolDay(WORD iYear, WORD iMonth, WORD iDay);

	////////////////////////////////////////////new///////////////////////////////////////////////////////

	//�ж��Ƿ���Ч���
	BOOL isEffectiveYear(WORD year);

	
	//����������Ϣ
	BOOL ReadHolidayInfo(vector<CString> &vecHolidayList,vector<HolidayInfo> &vecInfo);

	//��ָ�����ڻ�ȡ����
	CString GetHolidayForDate(int iAddDay,vector<HolidayInfo> &vecInfoD);


	//������������ڣ����������ڵ�������
	void GetAddDate(int iAddDay,WORD &iNewYear, WORD &iNewMonth, WORD &iNewDay);

};

