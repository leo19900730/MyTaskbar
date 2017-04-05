#include "../StdAfx.h"
#include "CalculateTime.h"


CCalculateTime::CCalculateTime(void)
{
	 m_Frequency=0;
	 m_StartTime=0;
	 m_CurTime=0;
	 m_dTime=0;
}


CCalculateTime::~CCalculateTime(void)
{
}


//////////////////////////////////////
void CCalculateTime::StartCalculateTime()
{	
	QueryPerformanceFrequency((LARGE_INTEGER *) &m_Frequency);
	QueryPerformanceCounter((LARGE_INTEGER *) &m_StartTime);
}


double CCalculateTime::GetCountTime()
{	
	QueryPerformanceCounter((LARGE_INTEGER *) &m_CurTime);	
	if(m_StartTime>0 && m_CurTime>0)
	{
		m_dTime=(double)(m_CurTime-m_StartTime)/(double)m_Frequency;	
	}

	return m_dTime;
}
