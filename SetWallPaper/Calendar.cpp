#include "../StdAfx.h"
#include "Calendar.h"
#include "../Utility/Utility.h"

#include "Setting.h"

extern WORD gLunarMonthDay[];
extern BYTE gLunarMonth[];
extern BYTE gLunarHolDay[];

WORD CCalendar::START_YEAR =1901;
WORD CCalendar::END_YEAR   =2050;


//////////////////////////////////////////////////////////////////

CCalendar::CCalendar(WORD iYear, WORD iMonth, WORD iDay):
                     m_iYear(iYear),m_iMonth(iMonth),m_iDay(iDay)
{

}


CCalendar::~CCalendar(void)
{
}

///////////////////////////////////////////////////////////////////////////////////


CString CCalendar::GetLunarYear()
{
	CString sLunarYear;

	const BYTE STR_LEN=24;

	if(isEffectiveYear(m_iYear))
	{
		WORD iLunarYear,iLunarMonth,iLunarDay;  //ũ��������
		auto days=GetLunarDate(m_iYear,m_iMonth,m_iDay,iLunarYear,iLunarMonth,iLunarDay);

		//��ȡũ����
		CHAR sYear[STR_LEN];
		FormatLunarYear(iLunarYear,sYear);
		sLunarYear=sYear;
	}


	return sLunarYear;

}


CString CCalendar::GetLunarMonthAndDay()
{
	return GetLunarMonthAndDay(m_iYear,m_iMonth,m_iDay);	
}


CString CCalendar::GetLunarMonthAndDay(WORD iYear,WORD iMonth,WORD iDay)
{
	CString sLunarMD;
	const BYTE STR_LEN=24;

	if(isEffectiveYear(m_iYear))
	{
		WORD iLunarYear,iLunarMonth,iLunarDay;  //ũ��������
		auto days=GetLunarDate(iYear,iMonth,iDay,iLunarYear,iLunarMonth,iLunarDay);
		
		//��ȡũ����
		CHAR sMonth[STR_LEN];
		FormatMonth(iLunarMonth,sMonth);

		//��ȡũ����
		CHAR sDay[STR_LEN];
		FormatLunarDay(iLunarDay,sDay);

		CString m,d;
		m=sMonth;
		m.Trim();
		//�ж��Ƿ�����
		if(GetLeapMonth(iLunarYear)==iLunarMonth)
		{
			m=L"��" + m;
		}
		d=sDay;
		d.Trim();
		sLunarMD=m+d;
	}


	return sLunarMD;
}

CString CCalendar::GetWeek()
{
	WORD iWeek=WeekDay(m_iYear,m_iMonth,m_iDay);
	switch(iWeek)
	{
	case 0:
		return L"������";
	case 1:
		return L"����һ";
	case 2:
		return L"���ڶ�";
	case 3:
		return L"������";
	case 4:
		return L"������";
	case 5:
		return L"������";
	case 6:
		return L"������";	
	default:
		break;
	}

	return L"";
}
	

CString CCalendar::GetMonthAndDay()
{
	return GetMonthAndDay(m_iMonth,m_iDay);	
}


CString CCalendar::GetMonthAndDay(WORD iMonth,WORD iDay)
{
	CString md;
	md.Format(L"%02d-%02d",iMonth,iDay);

	return md;
}


WORD CCalendar::GetLunarDate(WORD iYear, WORD iMonth, WORD iDay,
	WORD &iLunarYear, WORD &iLunarMonth, WORD &iLunarDay)
{
	l_CalcLunarDate(iLunarYear, iLunarMonth, iLunarDay, 
		CalcDateDiff(iYear, iMonth, iDay));

	return l_GetLunarHolDay(iYear, iMonth, iDay);
}


//�����⣬���ز���
CString CCalendar::GetGetLunarHolDay(WORD iYear, WORD iMonth, WORD iDay)
{
	WORD iHolDay=l_GetLunarHolDay(iYear, iMonth, iDay);
	CString sLunarHolDay=L"";

	if (iHolDay==0) return sLunarHolDay;
	 char *HolText[] ={"С��", "��", "����", "��ˮ",
		              "����", "����", "����", "����",
					  "����", "С��", "â��", "����",
                      "С��", "����", "����", "����",
					  "��¶", "���", "��¶", "˪��",
					  "����", "Сѩ", "��ѩ", "����"};

	sLunarHolDay=HolText[iHolDay-1];
	return sLunarHolDay;
}

/////////////////////////////////////////////////////////////
CString CCalendar::GetHoliday()
{
	CString sHolidayPath;
	CSetting::GetHolidayPath(sHolidayPath);

	if(!CUtility::GetInstance()->PathIsExist(sHolidayPath)) 
	{
		CString sMsg;
		sMsg.Format(L"Ŀ¼<%s>�����ڣ�",sHolidayPath);
		AfxMessageBox(sMsg);
		return L"";
	}


	vector<CString> vecHolidayList;
	CUtility::GetInstance()->ReadTextLine(vecHolidayList,sHolidayPath);
	
	vector<HolidayInfo> vecHolidayInfo;
	//����������Ϣ
	ReadHolidayInfo(vecHolidayList,vecHolidayInfo);


	CString sHoliday,sTmp;

	//��ȡ����Ľ���
	sTmp=GetHolidayForDate(0,vecHolidayInfo);
	if(!sTmp.IsEmpty())
	{
		sTmp=L"������:\n" +sTmp+ L"\n";
		sHoliday.Append(sTmp);
	}


	//��ȡ����Ľ���
	sTmp.Empty();
	sTmp=GetHolidayForDate(1,vecHolidayInfo);
	if(!sTmp.IsEmpty())
	{
		sTmp=L"������:\n" +sTmp+L"\n";
		sHoliday.Append(sTmp);
	}

	//��ȡ2���Ľ���
	sTmp.Empty();	
	sTmp=GetHolidayForDate(2,vecHolidayInfo);
	if(!sTmp.IsEmpty())
	{
		sTmp=L"������:\n" +sTmp+L"\n";
		sHoliday.Append(sTmp);
	}

	//��ȡ7���Ľ���
	sTmp.Empty();
	sTmp=GetHolidayForDate(7,vecHolidayInfo);
	if(!sTmp.IsEmpty())
	{
		sTmp=L"һ�ܺ���:\n" +sTmp+L"\n";
		sHoliday.Append(sTmp);
	}
	
	return sHoliday;
	
}



BOOL CCalendar::ReadHolidayInfo(vector<CString> &vecHolidayList,vector<HolidayInfo> &vecInfo)
{
	vector<CString>::iterator it;
	CString str1,str2;
	int pos1=0,pos2=0;
	BOOL bFirst=TRUE;

	for(it=vecHolidayList.begin();it!=vecHolidayList.end();it++)
	{
		if(bFirst) 
		{
			bFirst=FALSE;
			continue;  //������һ��
		}

		str1=*it;
		HolidayInfo hi;

		pos1=str1.Find(L"|",0);
		hi.sName=str1.Mid(0,pos1);

		pos2=str1.Find(L"|",pos1+1);
		hi.stype=str1.Mid(pos1+1,pos2-pos1-1);

		pos1=str1.Find(L"|",pos2+1);
		hi.sLunarDay=str1.Mid(pos2+1,pos1-pos2-1);

		pos2=str1.Find(L"|",pos1+1);
		hi.sDay=str1.Mid(pos1+1,pos2-pos1-1);

		vecInfo.push_back(hi);		
	}


	return TRUE;

}


CString CCalendar::GetHolidayForDate(int iAddDay,vector<HolidayInfo> &vecInfo)
{
	CString sHoliday,stmp;	
	CString sMD,sLunarMD;
	WORD iYear,iMonth,iDay;

	//iAddDayΪ0ʱ��ʾ����
	GetAddDate(iAddDay,iYear,iMonth,iDay);
	sMD=GetMonthAndDay(iMonth,iDay);
	sLunarMD=GetLunarMonthAndDay(iYear,iMonth,iDay);
	
	stmp.Empty();
	stmp=GetGetLunarHolDay(iYear,iMonth,iDay);  //��ȡũ������
	if(stmp!=L"")
	{
		sHoliday.Append(stmp);
		sHoliday.Append(L"\n");
	}


	vector<HolidayInfo>::iterator hit;
	for(hit=vecInfo.begin();hit!=vecInfo.end();hit++)
	{
		stmp.Empty();
		
		if(hit->sLunarDay==sLunarMD)
		{
			if(hit->stype!=L"NULL") 
			{			
				stmp.Format(L"%s %s\n",hit->sName,hit->stype);			

			}
			else
			{
				stmp.Format(L"%s\n",hit->sName);
			}
			sHoliday.Append(stmp);
			
		}
		else if(hit->sDay ==sMD)
		{
			if(hit->stype!=L"NULL") 
			{				
				stmp.Format(L"%s %s\n",hit->sName,hit->stype);		
			}
			else
			{
				stmp.Format(L"%s\n",hit->sName);				
			}
			sHoliday.Append(stmp);		
		}

	}
	
	return sHoliday;
}



////////////////////////////////////////////////////////////////////////////////////

void CCalendar::l_InitData()
{
	SYSTEMTIME systime;
	::GetSystemTime(&systime);

	m_iYear  = systime.wYear;
	m_iMonth = systime.wMonth;
	m_iDay   = systime.wDay;

}




LONG CCalendar::CalcDateDiff(WORD iEndYear, WORD iEndMonth, WORD iEndDay,
	WORD  iStartYear, WORD iStartMonth, WORD iStartDay)
{
	WORD monthday[]={0, 31, 59 ,90, 120, 151, 181, 212, 243, 273, 304, 334}; 

	//�����������1��1��֮����������
	LONG iDiffDays =(iEndYear - iStartYear)*365;
	iDiffDays += (iEndYear-1)/4 - (iStartYear-1)/4;
	iDiffDays -= ((iEndYear-1)/100 - (iStartYear-1)/100);
	iDiffDays += (iEndYear-1)/400 - (iStartYear-1)/400;

	//����iEndYear��1��1�յ�iEndMonth��iEndDay��֮�������
	iDiffDays += monthday[iEndMonth-1] +
		(IsLeapYear(iEndYear)&&iEndMonth>2? 1: 0);
	iDiffDays += iEndDay;

	//��ȥiStartYear��1��1�յ�iStartMonth��iStartDay��֮�������
	iDiffDays -= (monthday[iStartMonth-1] + 
		(IsLeapYear(iStartYear)&&iStartMonth>2 ? 1: 0));
	iDiffDays -= iStartDay;	
	return iDiffDays;
}

void  CCalendar::l_CalcLunarDate(WORD &iYear, WORD &iMonth ,WORD &iDay, LONG iSpanDays)
{
	//����1901��2��19��Ϊ����1901�����³�һ
	//����1901��1��1�յ�2��19�չ���49��
	if(iSpanDays <49)
	{
		iYear  = START_YEAR-1;
		if(iSpanDays <19)
		{ 
			iMonth = 11;  
			iDay   = 11+WORD(iSpanDays);
		}
		else
		{
			iMonth = 12;
			iDay   =  WORD(iSpanDays) -18;
		}
		return ;
	}
	//���������1901�����³�һ����
	iSpanDays -=49;
	iYear  = START_YEAR;
	iMonth = 1;
	iDay   = 1;
	//������
	LONG tmp = LunarYearDays(iYear); 
	while(iSpanDays >= tmp)
	{
		iSpanDays -= tmp;
		tmp = LunarYearDays(++iYear);
	}
	//������
	tmp = LOWORD(LunarMonthDays(iYear, iMonth));
	while(iSpanDays >= tmp)
	{
		iSpanDays -= tmp;
		if(iMonth == GetLeapMonth(iYear))
		{
			tmp  = HIWORD(LunarMonthDays(iYear, iMonth));
			if(iSpanDays < tmp)	
				break;
			iSpanDays -= tmp;
		}
		tmp = LOWORD(LunarMonthDays(iYear, ++iMonth));
	}
	//������
	iDay += WORD(iSpanDays);
}



//���ݽ������ݴ洢��ʽ,��������iYear��iMonth��iDay�ն�Ӧ�Ľ���,
WORD  CCalendar::l_GetLunarHolDay(WORD iYear, WORD iMonth, WORD iDay)
{
	BYTE &flag = gLunarHolDay[(iYear - START_YEAR)*12+iMonth -1];
	WORD day;
	if(iDay <15)
		day= 15 - ((flag>>4)&0x0f);
	else
		day = ((flag)&0x0f)+15;
	if(iDay == day)
		return (iMonth-1) *2 + (iDay>15? 1: 0) +1; 
	else
		return 0;
}

void CCalendar::GetDate(WORD &iYear, WORD &iMonth, WORD &iDay) 
{
	iYear  = m_iYear;
	iMonth = m_iMonth;
	iDay   = m_iDay;
}

BOOL CCalendar::SetDate(WORD iYear, WORD iMonth, WORD iDay)
{
	if(iYear < START_YEAR || iYear > END_YEAR || iMonth <1 || iMonth >12)
		return FALSE;

	if(iDay <1 || iDay > MonthDays(iYear, iMonth))
		return FALSE;

	m_iYear   = iYear;
	m_iMonth  = iMonth;
	m_iDay    = iDay;

	return TRUE;
} 

WORD CCalendar::WeekDay(WORD iYear, WORD iMonth, WORD iDay)
{
	//����Ԫ��monthday[i]��ʾ��i������ǰ������������7������
	WORD monthday[]={0,3,3,6,1,4,6,2,5,0,3,5};
	WORD iDays = (iYear-1)%7 + (iYear-1)/4 - (iYear-1)/100 +(iYear-1)/400;
	iDays += (monthday[iMonth-1] +iDay) ;
	//���iYear������
	if(IsLeapYear(iYear) && iMonth>2)
		iDays++;
	//����:0,1,2,3,4,5,6���ա�һ�����������ġ��塢��
	return iDays%7;
}

WORD CCalendar::MonthDays(WORD iYear, WORD iMonth)
{
	switch(iMonth)
	{
	case 1:case 3:case 5:case 7:case 8:case 10:case 12:
		return 31;
		break;
	case 4:case 6:case 9:case 11:
		return 30;
		break;
	case 2:
		//���������
		if(IsLeapYear(iYear))
			return 29;
		else
			return 28;
		break;
	}
	return 0;
}

WORD CCalendar::GetLeapMonth(WORD iLunarYear)
{
	BYTE &flag = gLunarMonth[(iLunarYear - START_YEAR)/2];
	return  (iLunarYear - START_YEAR)%2 ? flag&0x0f : flag>>4;
}

LONG CCalendar::LunarMonthDays(WORD iLunarYear, WORD iLunarMonth)
{
	if(iLunarYear < START_YEAR) 
		return 30L;

	WORD height =0 ,low =29;
	int iBit = 16 - iLunarMonth;

	if(iLunarMonth > GetLeapMonth(iLunarYear) && GetLeapMonth(iLunarYear))
		iBit --;

	if(gLunarMonthDay[iLunarYear - START_YEAR] & (1<<iBit))
		low ++;

	if(iLunarMonth == GetLeapMonth(iLunarYear))
		if(gLunarMonthDay[iLunarYear - START_YEAR] & (1<< (iBit -1)))
			height =30;
		else 
			height =29;

	return MAKELONG(low, height);
}

WORD CCalendar::LunarYearDays(WORD iLunarYear)
{
	/*
	WORD days=348 ; //12*29
	int  month = 12 ;
	//���iYear�������£���Ϊ13����
	if(gLanarMonth[iYear - START_YEAR]) 
	month ++;
	//���ĳ������ʮ����days++
	while(month >=0 && (gLanarMonthDay[iYear - START_YEAR] & (1 << (16 - month))))
	{   
	days ++; 
	month --;
	}
	return days;
	*/
	WORD days =0;
	for(WORD  i=1; i<=12; i++)
	{ 
		LONG  tmp = LunarMonthDays(iLunarYear ,i); 
		days += HIWORD(tmp);
		days += LOWORD(tmp);
	}
	return days;
}

void CCalendar::FormatLunarYear(WORD  iYear, CHAR *pBuffer)
{	
	CHAR szText1[]="���ұ����켺�����ɹ�";
	CHAR szText2[]="�ӳ���î������δ�����纥";
	CHAR szText3[]="��ţ������������Ｆ����";

	memcpy(pBuffer,  szText1+((iYear-4)%10)*2,2);
	memcpy(pBuffer+2,szText2+((iYear-4)%12)*2,2);
	pBuffer[4]=' ';
	memcpy(pBuffer+5,szText3+((iYear-4)%12)*2,2);
	strcpy(pBuffer+7,"��");
}

void CCalendar::FormatMonth(WORD iMonth, CHAR *pBuffer, BOOL bLunar)
{
	if(!bLunar && iMonth==1)
	{
		strcpy(pBuffer, "��һ��");
		return;
	}

	CHAR szText[]="�������������߰˾�ʮ";
	if(iMonth<=10)
	{
		memcpy(pBuffer, "��", 2);
		memcpy(pBuffer+2, szText + (iMonth -1)*2, 2);
		strcpy(pBuffer+4 , "��");
		return;
	}
	if (iMonth == 11)
	{
		strcpy(pBuffer, "ʮһ");
	}
	else
	{
		strcpy(pBuffer, "ʮ��");
	}
	strcpy(pBuffer+4 , "��");


}
void CCalendar::FormatLunarDay(WORD iDay, CHAR *pBuffer)
{
	CHAR szText1[]="��ʮإ��";
	CHAR szText2[]="һ�����������߰˾�ʮ";
	if(iDay != 20 && iDay !=30)
	{
		memcpy(pBuffer, szText1 + (iDay-1)/10*2 ,2);
		memcpy(pBuffer+2, szText2 + ((iDay-1)%10)*2 ,2);
		pBuffer[4]='\0';
	}
	else
	{
		memcpy(pBuffer, szText1 + iDay/10*2, 2);
		strcpy(pBuffer+2, szText2 +18);
	}
}


BOOL CCalendar::isEffectiveYear(WORD year)
{
	if(year>START_YEAR && START_YEAR<END_YEAR)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}




void CCalendar::GetAddDate(int iAddDay,WORD &iNewYear, WORD &iNewMonth, WORD &iNewDay)
{
	CTime tmToday(m_iYear,m_iMonth,m_iDay,0,0,0); //��������� 
	CTime tmNewDay; //ǰ��������� 
	CTimeSpan tmspanTwoDay(iAddDay,0,0,0); //�����ʱ����� 
	tmNewDay=tmToday+tmspanTwoDay; 

	iNewYear=tmNewDay.GetYear();
	iNewMonth=tmNewDay.GetMonth();
	iNewDay=tmNewDay.GetDay();

}







//////////////////////////////////////////////////////////////////////////////
/******************************************************************************
����Ϊ�����������������,Ϊ��ʡ�洢�ռ�,���Բ�������Ƚϱ�̬�Ĵ洢����.

*******************************************************************************/
//����gLunarDay��������1901�굽2100��ÿ���е���������Ϣ��
//����ÿ��ֻ����29��30�죬һ����12����13����������λ��ʾ����ӦλΪ1��30�죬����Ϊ29��
WORD gLunarMonthDay[]=
{
	//��������ֻ��1901.1.1 --2050.12.31
	//   1      2       3       4       5       6       7       8       9       10
	0X4ae0, 0Xa570, 0X5268, 0Xd260, 0Xd950, 0X6aa8, 0X56a0, 0X9ad0, 0X4ae8, 0X4ae0,   //1901-1910
	0Xa4d8, 0Xa4d0, 0Xd250, 0Xd548, 0Xb550, 0X56a0, 0X96d0, 0X95b0, 0X49b8, 0X49b0,   //1920
	0Xa4b0, 0Xb258, 0X6a50, 0X6d40, 0Xada8, 0X2b60, 0X9570, 0X4978, 0X4970, 0X64b0,   //1930
	0Xd4a0, 0Xea50, 0X6d48, 0X5ad0, 0X2b60, 0X9370, 0X92e0, 0Xc968, 0Xc950, 0Xd4a0,   //1940
	0Xda50, 0Xb550, 0X56a0, 0Xaad8, 0X25d0, 0X92d0, 0Xc958, 0Xa950, 0Xb4a8, 0X6ca0,   //1950
	0Xb550, 0X55a8, 0X4da0, 0Xa5b0, 0X52b8, 0X52b0, 0Xa950, 0Xe950, 0X6aa0, 0Xad50,   //1960
	0Xab50, 0X4b60, 0Xa570, 0Xa570, 0X5260, 0Xe930, 0Xd950, 0X5aa8, 0X56a0, 0X96d0,   //1970
	0X4ae8, 0X4ad0, 0Xa4d0, 0Xd268, 0Xd250, 0Xd528, 0Xb540, 0Xb6a0, 0X96d0, 0X95b0,   //1980
	0X49b0, 0Xa4b8, 0Xa4b0, 0Xb258, 0X6a50, 0X6d40, 0Xada0, 0Xab60, 0X9370, 0X4978,   //1990
	0X4970, 0X64b0, 0X6a50, 0Xea50, 0X6b28, 0X5ac0, 0Xab60, 0X9368, 0X92e0, 0Xc960,   //2000
	0Xd4a8, 0Xd4a0, 0Xda50, 0X5aa8, 0X56a0, 0Xaad8, 0X25d0, 0X92d0, 0Xc958, 0Xa950,   //2010
	0Xb4a0, 0Xb550, 0Xb550, 0X55a8, 0X4ba0, 0Xa5b0, 0X52b8, 0X52b0, 0Xa930, 0X74a8,   //2020
	0X6aa0, 0Xad50, 0X4da8, 0X4b60, 0X9570, 0Xa4e0, 0Xd260, 0Xe930, 0Xd530, 0X5aa0,   //2030
	0X6b50, 0X96d0, 0X4ae8, 0X4ad0, 0Xa4d0, 0Xd258, 0Xd250, 0Xd520, 0Xdaa0, 0Xb5a0,   //2040
	0X56d0, 0X4ad8, 0X49b0, 0Xa4b8, 0Xa4b0, 0Xaa50, 0Xb528, 0X6d20, 0Xada0, 0X55b0,   //2050

};

//����gLanarMonth�������1901�굽2050�����µ��·ݣ���û����Ϊ0��ÿ�ֽڴ�����
BYTE  gLunarMonth[]=
{
	0X00, 0X50, 0X04, 0X00, 0X20,   //1910
	0X60, 0X05, 0X00, 0X20, 0X70,   //1920
	0X05, 0X00, 0X40, 0X02, 0X06,   //1930
	0X00, 0X50, 0X03, 0X07, 0X00,   //1940
	0X60, 0X04, 0X00, 0X20, 0X70,   //1950
	0X05, 0X00, 0X30, 0X80, 0X06,   //1960
	0X00, 0X40, 0X03, 0X07, 0X00,   //1970
	0X50, 0X04, 0X08, 0X00, 0X60,   //1980
	0X04, 0X0a, 0X00, 0X60, 0X05,   //1990
	0X00, 0X30, 0X80, 0X05, 0X00,   //2000
	0X40, 0X02, 0X07, 0X00, 0X50,   //2010
	0X04, 0X09, 0X00, 0X60, 0X04,   //2020
	0X00, 0X20, 0X60, 0X05, 0X00,   //2030
	0X30, 0Xb0, 0X06, 0X00, 0X50,   //2040
	0X02, 0X07, 0X00, 0X50, 0X03    //2050
};

//����gLanarHoliDay���ÿ��Ķ�ʮ�Ľ�����Ӧ����������
//ÿ��Ķ�ʮ�Ľ�����Ӧ���������ڼ����̶���ƽ���ֲ���ʮ��������
//   1��          2��         3��         4��         5��         6��   
//С�� ��   ����  ��ˮ   ���� ����   ���� ����   ���� С��   â�� ����
//   7��          8��         9��         10��       11��        12��  
//С�� ����   ����  ����   ��¶ ���   ��¶ ˪��   ���� Сѩ   ��ѩ ����

/*********************************************************************************
�������κ�ȷ������,����ֻ�ô��,Ҫ��ʡ�ռ�,����....
�������ִ淨ʵ����̫��̬��,��ͽ����ſ���
**********************************************************************************/
//���ݸ�ʽ˵��:
//��1901��Ľ���Ϊ
//  1��     2��     3��   4��    5��   6��   7��    8��   9��    10��  11��     12��
// 6, 21, 4, 19,  6, 21, 5, 21, 6,22, 6,22, 8, 23, 8, 24, 8, 24, 8, 24, 8, 23, 8, 22
// 9, 6,  11,4,   9, 6,  10,6,  9,7,  9,7,  7, 8,  7, 9,  7,  9, 7,  9, 7,  8, 7, 15
//�����һ������Ϊÿ�½�����Ӧ����,15��ȥÿ�µ�һ������,ÿ�µڶ���������ȥ15�õڶ���
// ����ÿ������������Ӧ���ݶ�С��16,ÿ����һ���ֽڴ��,��λ��ŵ�һ����������,��λ���
//�ڶ�������������,�ɵ��±�

BYTE gLunarHolDay[]=
{
	0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X79, 0X69, 0X78, 0X77,   //1901
	0X96, 0XA4, 0X96, 0X96, 0X97, 0X87, 0X79, 0X79, 0X79, 0X69, 0X78, 0X78,   //1902
	0X96, 0XA5, 0X87, 0X96, 0X87, 0X87, 0X79, 0X69, 0X69, 0X69, 0X78, 0X78,   //1903
	0X86, 0XA5, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X79, 0X78, 0X87,   //1904
	0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X79, 0X69, 0X78, 0X77,   //1905
	0X96, 0XA4, 0X96, 0X96, 0X97, 0X97, 0X79, 0X79, 0X79, 0X69, 0X78, 0X78,   //1906
	0X96, 0XA5, 0X87, 0X96, 0X87, 0X87, 0X79, 0X69, 0X69, 0X69, 0X78, 0X78,   //1907
	0X86, 0XA5, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X69, 0X78, 0X87,   //1908
	0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X79, 0X69, 0X78, 0X77,   //1909
	0X96, 0XA4, 0X96, 0X96, 0X97, 0X97, 0X79, 0X79, 0X79, 0X69, 0X78, 0X78,   //1910
	0X96, 0XA5, 0X87, 0X96, 0X87, 0X87, 0X79, 0X69, 0X69, 0X69, 0X78, 0X78,   //1911
	0X86, 0XA5, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X69, 0X78, 0X87,   //1912
	0X95, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X79, 0X69, 0X78, 0X77,   //1913
	0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X79, 0X79, 0X79, 0X69, 0X78, 0X78,   //1914
	0X96, 0XA5, 0X97, 0X96, 0X97, 0X87, 0X79, 0X79, 0X69, 0X69, 0X78, 0X78,   //1915
	0X96, 0XA5, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X79, 0X77, 0X87,   //1916
	0X95, 0XB4, 0X96, 0XA6, 0X96, 0X97, 0X78, 0X79, 0X78, 0X69, 0X78, 0X87,   //1917
	0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X79, 0X79, 0X79, 0X69, 0X78, 0X77,   //1918
	0X96, 0XA5, 0X97, 0X96, 0X97, 0X87, 0X79, 0X79, 0X69, 0X69, 0X78, 0X78,   //1919
	0X96, 0XA5, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X79, 0X77, 0X87,   //1920
	0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X78, 0X79, 0X78, 0X69, 0X78, 0X87,   //1921
	0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X79, 0X79, 0X79, 0X69, 0X78, 0X77,   //1922
	0X96, 0XA4, 0X96, 0X96, 0X97, 0X87, 0X79, 0X79, 0X69, 0X69, 0X78, 0X78,   //1923
	0X96, 0XA5, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X79, 0X77, 0X87,   //1924
	0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X78, 0X79, 0X78, 0X69, 0X78, 0X87,   //1925
	0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X79, 0X69, 0X78, 0X77,   //1926
	0X96, 0XA4, 0X96, 0X96, 0X97, 0X87, 0X79, 0X79, 0X79, 0X69, 0X78, 0X78,   //1927
	0X96, 0XA5, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X78, 0X87, 0X87,   //1928
	0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X79, 0X77, 0X87,   //1929
	0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X79, 0X69, 0X78, 0X77,   //1930
	0X96, 0XA4, 0X96, 0X96, 0X97, 0X87, 0X79, 0X79, 0X79, 0X69, 0X78, 0X78,   //1931
	0X96, 0XA5, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X78, 0X87, 0X87,   //1932
	0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X69, 0X78, 0X87,   //1933
	0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X79, 0X69, 0X78, 0X77,   //1934
	0X96, 0XA4, 0X96, 0X96, 0X97, 0X97, 0X79, 0X79, 0X79, 0X69, 0X78, 0X78,   //1935
	0X96, 0XA5, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X78, 0X87, 0X87,   //1936
	0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X69, 0X78, 0X87,   //1937
	0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X79, 0X69, 0X78, 0X77,   //1938
	0X96, 0XA4, 0X96, 0X96, 0X97, 0X97, 0X79, 0X79, 0X79, 0X69, 0X78, 0X78,   //1939
	0X96, 0XA5, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X78, 0X87, 0X87,   //1940
	0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X69, 0X78, 0X87,   //1941
	0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X79, 0X69, 0X78, 0X77,   //1942
	0X96, 0XA4, 0X96, 0X96, 0X97, 0X97, 0X79, 0X79, 0X79, 0X69, 0X78, 0X78,   //1943
	0X96, 0XA5, 0X96, 0XA5, 0XA6, 0X96, 0X88, 0X78, 0X78, 0X78, 0X87, 0X87,   //1944
	0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X79, 0X77, 0X87,   //1945
	0X95, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X78, 0X69, 0X78, 0X77,   //1946
	0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X79, 0X79, 0X79, 0X69, 0X78, 0X78,   //1947
	0X96, 0XA5, 0XA6, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X78, 0X78, 0X87, 0X87,   //1948
	0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X79, 0X78, 0X79, 0X77, 0X87,   //1949
	0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X78, 0X79, 0X78, 0X69, 0X78, 0X77,   //1950
	0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X79, 0X79, 0X79, 0X69, 0X78, 0X78,   //1951
	0X96, 0XA5, 0XA6, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X78, 0X78, 0X87, 0X87,   //1952
	0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X79, 0X77, 0X87,   //1953
	0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X78, 0X79, 0X78, 0X68, 0X78, 0X87,   //1954
	0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X79, 0X69, 0X78, 0X77,   //1955
	0X96, 0XA5, 0XA5, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X78, 0X78, 0X87, 0X87,   //1956
	0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X79, 0X77, 0X87,   //1957
	0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X69, 0X78, 0X87,   //1958
	0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X79, 0X69, 0X78, 0X77,   //1959
	0X96, 0XA4, 0XA5, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   //1960
	0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X78, 0X87, 0X87,   //1961
	0X96, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X69, 0X78, 0X87,   //1962
	0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X79, 0X69, 0X78, 0X77,   //1963
	0X96, 0XA4, 0XA5, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   //1964
	0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X78, 0X87, 0X87,   //1965
	0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X69, 0X78, 0X87,   //1966
	0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X79, 0X69, 0X78, 0X77,   //1967
	0X96, 0XA4, 0XA5, 0XA5, 0XA6, 0XA6, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   //1968
	0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X78, 0X87, 0X87,   //1969
	0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X69, 0X78, 0X87,   //1970
	0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X79, 0X69, 0X78, 0X77,   //1971
	0X96, 0XA4, 0XA5, 0XA5, 0XA6, 0XA6, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   //1972
	0XA5, 0XB5, 0X96, 0XA5, 0XA6, 0X96, 0X88, 0X78, 0X78, 0X78, 0X87, 0X87,   //1973
	0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X69, 0X78, 0X87,   //1974
	0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X78, 0X69, 0X78, 0X77,   //1975
	0X96, 0XA4, 0XA5, 0XB5, 0XA6, 0XA6, 0X88, 0X89, 0X88, 0X78, 0X87, 0X87,   //1976
	0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X88, 0X78, 0X78, 0X87, 0X87,   //1977
	0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X79, 0X78, 0X87,   //1978
	0X96, 0XB4, 0X96, 0XA6, 0X96, 0X97, 0X78, 0X79, 0X78, 0X69, 0X78, 0X77,   //1979
	0X96, 0XA4, 0XA5, 0XB5, 0XA6, 0XA6, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   //1980
	0XA5, 0XB4, 0X96, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X78, 0X78, 0X77, 0X87,   //1981
	0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X79, 0X77, 0X87,   //1982
	0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X78, 0X79, 0X78, 0X69, 0X78, 0X77,   //1983
	0X96, 0XB4, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X88, 0X78, 0X87, 0X87,   //1984
	0XA5, 0XB4, 0XA6, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X78, 0X78, 0X87, 0X87,   //1985
	0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X79, 0X77, 0X87,   //1986
	0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X79, 0X78, 0X69, 0X78, 0X87,   //1987
	0X96, 0XB4, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X88, 0X78, 0X87, 0X86,   //1988
	0XA5, 0XB4, 0XA5, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   //1989
	0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X79, 0X77, 0X87,   //1990
	0X95, 0XB4, 0X96, 0XA5, 0X86, 0X97, 0X88, 0X78, 0X78, 0X69, 0X78, 0X87,   //1991
	0X96, 0XB4, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X88, 0X78, 0X87, 0X86,   //1992
	0XA5, 0XB3, 0XA5, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   //1993
	0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X78, 0X87, 0X87,   //1994
	0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X76, 0X78, 0X69, 0X78, 0X87,   //1995
	0X96, 0XB4, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X88, 0X78, 0X87, 0X86,   //1996
	0XA5, 0XB3, 0XA5, 0XA5, 0XA6, 0XA6, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   //1997
	0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X78, 0X87, 0X87,   //1998
	0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X69, 0X78, 0X87,   //1999
	0X96, 0XB4, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X88, 0X78, 0X87, 0X86,   //2000
	0XA5, 0XB3, 0XA5, 0XA5, 0XA6, 0XA6, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   //2001
	0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X78, 0X87, 0X87,   //2002
	0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X69, 0X78, 0X87,   //2003
	0X96, 0XB4, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X88, 0X78, 0X87, 0X86,   //2004
	0XA5, 0XB3, 0XA5, 0XA5, 0XA6, 0XA6, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   //2005
	0XA5, 0XB4, 0X96, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X78, 0X78, 0X87, 0X87,   //2006
	0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X69, 0X78, 0X87,   //2007
	0X96, 0XB4, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X87, 0X78, 0X87, 0X86,   //2008
	0XA5, 0XB3, 0XA5, 0XB5, 0XA6, 0XA6, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   //2009
	0XA5, 0XB4, 0X96, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X78, 0X78, 0X87, 0X87,   //2010
	0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X79, 0X78, 0X87,   //2011
	0X96, 0XB4, 0XA5, 0XB5, 0XA5, 0XA6, 0X87, 0X88, 0X87, 0X78, 0X87, 0X86,   //2012
	0XA5, 0XB3, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X88, 0X78, 0X87, 0X87,   //2013
	0XA5, 0XB4, 0X96, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X78, 0X78, 0X87, 0X87,   //2014
	0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X79, 0X77, 0X87,   //2015
	0X95, 0XB4, 0XA5, 0XB4, 0XA5, 0XA6, 0X87, 0X88, 0X87, 0X78, 0X87, 0X86,   //2016
	0XA5, 0XC3, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X88, 0X78, 0X87, 0X87,   //2017
	0XA5, 0XB4, 0XA6, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X78, 0X78, 0X87, 0X87,   //2018
	0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X79, 0X77, 0X87,   //2019
	0X95, 0XB4, 0XA5, 0XB4, 0XA5, 0XA6, 0X97, 0X87, 0X87, 0X78, 0X87, 0X86,   //2020
	0XA5, 0XC3, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X88, 0X78, 0X87, 0X86,   //2021
	0XA5, 0XB4, 0XA5, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   //2022
	0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X79, 0X77, 0X87,   //2023
	0X95, 0XB4, 0XA5, 0XB4, 0XA5, 0XA6, 0X97, 0X87, 0X87, 0X78, 0X87, 0X96,   //2024
	0XA5, 0XC3, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X88, 0X78, 0X87, 0X86,   //2025
	0XA5, 0XB3, 0XA5, 0XA5, 0XA6, 0XA6, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   //2026
	0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X78, 0X87, 0X87,   //2027
	0X95, 0XB4, 0XA5, 0XB4, 0XA5, 0XA6, 0X97, 0X87, 0X87, 0X78, 0X87, 0X96,   //2028
	0XA5, 0XC3, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X88, 0X78, 0X87, 0X86,   //2029
	0XA5, 0XB3, 0XA5, 0XA5, 0XA6, 0XA6, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   //2030
	0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X78, 0X87, 0X87,   //2031
	0X95, 0XB4, 0XA5, 0XB4, 0XA5, 0XA6, 0X97, 0X87, 0X87, 0X78, 0X87, 0X96,   //2032
	0XA5, 0XC3, 0XA5, 0XB5, 0XA6, 0XA6, 0X88, 0X88, 0X88, 0X78, 0X87, 0X86,   //2033
	0XA5, 0XB3, 0XA5, 0XA5, 0XA6, 0XA6, 0X88, 0X78, 0X88, 0X78, 0X87, 0X87,   //2034
	0XA5, 0XB4, 0X96, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X78, 0X78, 0X87, 0X87,   //2035
	0X95, 0XB4, 0XA5, 0XB4, 0XA5, 0XA6, 0X97, 0X87, 0X87, 0X78, 0X87, 0X96,   //2036
	0XA5, 0XC3, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X88, 0X78, 0X87, 0X86,   //2037
	0XA5, 0XB3, 0XA5, 0XA5, 0XA6, 0XA6, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   //2038
	0XA5, 0XB4, 0X96, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X78, 0X78, 0X87, 0X87,   //2039
	0X95, 0XB4, 0XA5, 0XB4, 0XA5, 0XA6, 0X97, 0X87, 0X87, 0X78, 0X87, 0X96,   //2040
	0XA5, 0XC3, 0XA5, 0XB5, 0XA5, 0XA6, 0X87, 0X88, 0X87, 0X78, 0X87, 0X86,   //2041
	0XA5, 0XB3, 0XA5, 0XB5, 0XA6, 0XA6, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   //2042
	0XA5, 0XB4, 0X96, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X78, 0X78, 0X87, 0X87,   //2043
	0X95, 0XB4, 0XA5, 0XB4, 0XA5, 0XA6, 0X97, 0X87, 0X87, 0X88, 0X87, 0X96,   //2044
	0XA5, 0XC3, 0XA5, 0XB4, 0XA5, 0XA6, 0X87, 0X88, 0X87, 0X78, 0X87, 0X86,   //2045
	0XA5, 0XB3, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X88, 0X78, 0X87, 0X87,   //2046
	0XA5, 0XB4, 0X96, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X78, 0X78, 0X87, 0X87,   //2047
	0X95, 0XB4, 0XA5, 0XB4, 0XA5, 0XA5, 0X97, 0X87, 0X87, 0X88, 0X86, 0X96,   //2048
	0XA4, 0XC3, 0XA5, 0XA5, 0XA5, 0XA6, 0X97, 0X87, 0X87, 0X78, 0X87, 0X86,   //2049
	0XA5, 0XC3, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X78, 0X78, 0X87, 0X87    //2050

};


