

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
	//计算公历iYear年iMonth月iDay日对应的阴历日期,返回对应的阴历节气 0-24
	//1901年1月1日---2050年12月31日

	//获取农历年
	CString GetLunarYear();
	//获取农历月日
	CString GetLunarMonthAndDay();

	CString GetLunarMonthAndDay(WORD iYear,WORD iMonth,WORD iDay);
	CString GetWeek();
	CString GetHoliday();
	CString GetMonthAndDay();
	CString GetMonthAndDay(WORD iMonth,WORD iDay);
	
	//获取节气
	CString GetGetLunarHolDay(WORD iYear, WORD iMonth, WORD iDay);  

	
private:

	WORD GetLunarDate(WORD iYear, WORD iMonth, WORD iDay,
		WORD &iLunarYear, WORD &iLunarMonth, WORD &iLunarDay);

	WORD GetYear(){return m_iYear;}
	WORD GetMonth(){return m_iMonth;}
	WORD GetDay(){return m_iDay;}
	void GetDate(WORD &iYear, WORD &iMonth, WORD &iDay);
	BOOL SetDate(WORD iYear, WORD iMonth, WORD iDay);


	//判断iYear是不是闰年
	BOOL IsLeapYear(WORD iYear)
	{return !(iYear%4)&&(iYear%100) || !(iYear%400);}

	//计算iYear,iMonth,iDay对应是星期几 1年1月1日 --- 65535年12月31日
	WORD WeekDay(WORD iYear, WORD iMonth, WORD iDay);

	//返回iYear年iMonth月的天数 1年1月 --- 65535年12月
	WORD MonthDays(WORD iYear, WORD iMonth);

	//返回阴历iLunarYer年阴历iLunarMonth月的天数，如果iLunarMonth为闰月，
	//高字为第二个iLunarMonth月的天数，否则高字为0 
	// 1901年1月---2050年12月
	LONG LunarMonthDays(WORD iLunarYear, WORD iLunarMonth);

	//返回阴历iLunarYear年的总天数
	// 1901年1月---2050年12月
	WORD LunarYearDays(WORD iLunarYear);

	//返回阴历iLunarYear年的闰月月份，如没有返回0
	// 1901年1月---2050年12月
	WORD GetLeapMonth(WORD iLunarYear);

	//把iYear年格式化成天干记年法表示的字符串
	void FormatLunarYear(WORD  iYear, CHAR *pBuffer);

	//把iMonth格式化成中文字符串
	void FormatMonth(WORD iMonth, CHAR *pBuffer, BOOL bLunar = TRUE);

	//把iDay格式化成中文字符串
	void FormatLunarDay(WORD  iDay, CHAR *pBuffer);

	//计算公历两个日期间相差的天数  1年1月1日 --- 65535年12月31日
	LONG CalcDateDiff(WORD iEndYear, WORD iEndMonth, WORD iEndDay,
		WORD iStartYear = START_YEAR, 
		WORD iStartMonth =1, WORD iStartDay =1);



	void   l_InitData();

	//计算从1901年1月1日过iSpanDays天后的阴历日期
	void   l_CalcLunarDate(WORD &iYear, WORD &iMonth ,WORD &iDay, LONG iSpanDays);

	//计算公历iYear年iMonth月iDay日对应的节气 0-24，0表不是节气
	WORD   l_GetLunarHolDay(WORD iYear, WORD iMonth, WORD iDay);

	////////////////////////////////////////////new///////////////////////////////////////////////////////

	//判断是否有效年份
	BOOL isEffectiveYear(WORD year);

	
	//解析节日信息
	BOOL ReadHolidayInfo(vector<CString> &vecHolidayList,vector<HolidayInfo> &vecInfo);

	//按指定日期获取节日
	CString GetHolidayForDate(int iAddDay,vector<HolidayInfo> &vecInfoD);


	//计算相隔的日期，返回新日期的年月日
	void GetAddDate(int iAddDay,WORD &iNewYear, WORD &iNewMonth, WORD &iNewDay);

};

