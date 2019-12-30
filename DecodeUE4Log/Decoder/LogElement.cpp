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

string CMemcutStr::GetString() const
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

bool CUELogTime::TryDecodeTime(const char * Str)
{
	if (!Str) return false;
	if (!StrIsTime(Str, TIMESTR_LENGTH)) return false;

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

	m_MSecond += (Str[1] - '0') * 100;
	m_MSecond += (Str[2] - '0') * 10;
	m_MSecond += (Str[3] - '0');

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
	++m_TypeCount;
	m_Dictionry[m_TypeCount] = name;
}
