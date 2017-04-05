#pragma once
class CCalculateTime
{
public:
	CCalculateTime(void);
	~CCalculateTime(void);

	void StartCalculateTime();
	double GetCountTime();

private:
	__int64 m_Frequency;
	__int64 m_StartTime;
	__int64 m_CurTime;
	double m_dTime;


};

