#include <string.h>
#include "LogElement.h"

CUELogTime::CUELogTime(CMaskFlyWeight * FlyWeight /* = nullptr */) :
	m_MaskFlyWeight(FlyWeight)
{
}

bool CMemcutStr::CopyStr(char * outBuf, int Len) const
{
	if (!outBuf) return false;
	if (!(m_Begin && m_End)) return false;
	if (Len < Length() + 1) return false;

	memcpy(outBuf, m_Begin, Length());
	outBuf[Length()] = '\0';
	return true;
}

string CMemcutStr::ToString() const
{
	if (!(m_Begin && m_End)) return string();

	int len = Length();
	string ret(len, ' ');
	for (int i = 0; i < len; ++i)
	{
		ret[i] = m_Begin[i];
	}
	return ret;
}

bool CUELogTime::operator>(const CUELogTime & Other) const
{
	switch (m_MaskFlyWeight ? m_MaskFlyWeight->GetMask() : ECompareMask::MaskYear)
	{
	case ECompareMask::MaskYear:
		if (m_Year < Other.m_Year) return false;
		if (m_Year > Other.m_Year) return true;
	case ECompareMask::MaskMouth:
		if (m_Mouth < Other.m_Mouth) return false;
		if (m_Mouth > Other.m_Mouth) return true;
	case ECompareMask::MaskDay:
		if (m_Day < Other.m_Day) return false;
		if (m_Day > Other.m_Day) return true;
	case ECompareMask::MaskHour:
		if (m_Hour < Other.m_Hour) return false;
		if (m_Hour > Other.m_Hour) return true;
	case ECompareMask::MaskMinute:
		if (m_Minute < Other.m_Minute) return false;
		if (m_Minute > Other.m_Minute) return true;
	case ECompareMask::MaskSecond:
		if (m_Second < Other.m_Second) return false;
		if (m_Second > Other.m_Second) return true;
	case ECompareMask::MaskMSecond:
		if (m_MSecond < Other.m_MSecond) return false;
		if (m_MSecond > Other.m_MSecond) return true;
	}
	return false;
}

bool CUELogTime::operator<(const CUELogTime & Other) const
{
	switch (m_MaskFlyWeight ? m_MaskFlyWeight->GetMask() : ECompareMask::MaskYear)
	{
	case ECompareMask::MaskYear:
		if (m_Year < Other.m_Year) return true;
		if (m_Year > Other.m_Year) return false;
	case ECompareMask::MaskMouth:
		if (m_Mouth < Other.m_Mouth) return true;
		if (m_Mouth > Other.m_Mouth) return false;
	case ECompareMask::MaskDay:
		if (m_Day < Other.m_Day) return true;
		if (m_Day > Other.m_Day) return false;
	case ECompareMask::MaskHour:
		if (m_Hour < Other.m_Hour) return true;
		if (m_Hour > Other.m_Hour) return false;
	case ECompareMask::MaskMinute:
		if (m_Minute < Other.m_Minute) return true;
		if (m_Minute > Other.m_Minute) return false;
	case ECompareMask::MaskSecond:
		if (m_Second < Other.m_Second) return true;
		if (m_Second > Other.m_Second) return false;
	case ECompareMask::MaskMSecond:
		if (m_MSecond < Other.m_MSecond) return true;
		if (m_MSecond > Other.m_MSecond) return false;
	}
	return false;
}

bool CUELogTime::TryDecodeTime(const char * Str)
{
	if (!Str) return false;
	if (!StrIsTime(Str, TIMESTR_LENGTH)) return false;
	m_TimeStr.SetStr(Str, Str + TIMESTR_LENGTH);

	ClearTimeInfo();
	m_Year += (Str[0] - '0') * 1000;
	m_Year += (Str[1] - '0') * 100;
	m_Year += (Str[2] - '0') * 10;
	m_Year += (Str[3] - '0');
	Str += 5;

	m_Mouth += (Str[0] - '0') * 10;
	m_Mouth += (Str[1] - '0');
	Str += 3;

	m_Day += (Str[0] - '0') * 10;
	m_Day += (Str[1] - '0');
	Str += 3;

	m_Hour += (Str[0] - '0') * 10;
	m_Hour += (Str[1] - '0');
	Str += 3;

	m_Minute += (Str[0] - '0') * 10;
	m_Minute += (Str[1] - '0');
	Str += 3;

	m_Second += (Str[0] - '0') * 10;
	m_Second += (Str[1] - '0');
	Str += 3;

	m_MSecond += (Str[0] - '0') * 100;
	m_MSecond += (Str[1] - '0') * 10;
	m_MSecond += (Str[2] - '0');

	return true;
}

bool CUELogTime::StrIsTime(const char * Str, unsigned int Len)
{
	if (Len != TIMESTR_LENGTH) return false;
	return	Str[4] == '-' &&
		Str[7] == '-' &&
		Str[10] == ' ' &&
		Str[13] == ':' &&
		Str[16] == ':' &&
		Str[19] == '.';
}

bool CLogTypeDictionry::Contain(LogTypeCode code) const
{
	auto it = m_Dictionry.find(code);
	return it != m_Dictionry.end();
}

bool CLogTypeDictionry::Contain(string name) const
{
	for (auto it : m_Dictionry)
	{
		if (it.second == name)
		{
			return true;
		}
	}
	return false;
}

LogTypeCode CLogTypeDictionry::GetTypeCode(string name)
{
	for (auto it : m_Dictionry)
	{
		if (it.second == name)
		{
			return it.first;
		}
	}
	if (m_Locked) return -1;
	
	++m_TypeCount;
	m_Dictionry[m_TypeCount] = name;
	return m_TypeCount;
}

vector<string> CLogTypeDictionry::GetLogTypeNames()
{
	vector<string> AllNames(m_Dictionry.size());
	for (auto it : m_Dictionry)
	{
		AllNames[it.first - 1] = it.second;
	}
	return AllNames;
}

void CMaskFlyWeight::UpDateFlyWeight(const CUELogTime * Time)
{
	if (!m_LastLog) { m_LastLog = Time; return; }
	if (m_Mask == MaskYear || !Time->IsValid()) return;

	switch (m_Mask)
	{
	case ECompareMask::MaskMSecond:
		if(m_LastLog->GetSecond() != Time->GetSecond()) m_Mask = MaskSecond;
	case ECompareMask::MaskSecond:
		if (m_LastLog->GetMinute() != Time->GetMinute()) m_Mask = MaskMinute;
	case ECompareMask::MaskMinute:
		if (m_LastLog->GetHour() != Time->GetHour()) m_Mask = MaskHour;
	case ECompareMask::MaskHour:
		if (m_LastLog->GetDay() != Time->GetDay()) m_Mask = MaskDay;
	case ECompareMask::MaskDay:
		if (m_LastLog->GetMouth() != Time->GetMouth()) m_Mask = MaskMouth;
	case ECompareMask::MaskMouth:
		if (m_LastLog->GetYear() != Time->GetYear ()) m_Mask = MaskYear;
	}

	// update last log
	m_LastLog = Time;
}
