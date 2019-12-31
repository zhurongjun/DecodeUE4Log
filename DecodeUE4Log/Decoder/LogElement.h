#pragma once
#include <map>
#include <string>
using std::map;
using std::string;
using std::shared_ptr;
using std::weak_ptr;
using std::make_shared;

typedef unsigned short USHORT;
typedef unsigned char  UCHAR;
typedef unsigned int   LogTypeCode;

const unsigned int TIMESTR_LENGTH = 23;

enum ELogLevel
{
	Unknow,
	Message,
	Warning,
	Error
};

// 在一个字符数组中划分一部分字符组成的字符串
class CMemcutStr final
{
public:
	CMemcutStr() :
		m_Begin(nullptr) ,
		m_End(nullptr)
	{
	}
	CMemcutStr(char * Begin, char * End) :
		m_Begin(Begin),
		m_End(End)
	{
	}

	void SetStr(const char * Begin, const char * End) { m_Begin = Begin; m_End = End; }
	void SetStr(const char * Begin, int Len) { m_Begin = Begin; m_End = m_Begin + Len; }

	int Length() const { return (m_Begin && m_End) ? m_End - m_Begin : 0; }
	bool CopyStr(char * outBuf, int Len) const;
	string GetString() const;

private:
	const char * m_Begin;
	const char * m_End;
};

// Log类型字典，用来快速查找Log类型
class CLogTypeDictionry final
{
public:
	bool Contain(LogTypeCode code) const;
	bool Contain(string name) const;

	string GetTypeName(LogTypeCode code) const { auto it = m_Dictionry.find(code); return it == m_Dictionry.end() ? string() : it->second; }
	LogTypeCode GetTypeCode(string name);

	int GetTypeCount() { return m_TypeCount; }
	void Clear() { m_TypeCount = 0; m_Dictionry.clear(); }

private:
	map<LogTypeCode, string>	m_Dictionry;
	int		m_TypeCount;
};

// 比较遮罩，遮罩级别之上的Log时间不会参与比较，用于提高性能
enum ECompareMask
{
	MaskYear,
	MaskMouth,
	MaskDay,
	MaskHour,
	MaskMinute,
	MaskSecond,
	MaskMSecond
};

// 遮罩享元
class CUELogTime;
class CMaskFlyWeight
{
public:
	CMaskFlyWeight() :m_Mask(ECompareMask::MaskMSecond),m_LastLog(nullptr) {}
	ECompareMask GetMask() const { return m_Mask; }
	void UpDateFlyWeight(const CUELogTime * Time);
	ECompareMask SetMask(ECompareMask Mask) { m_Mask = Mask; }
private:
	ECompareMask m_Mask;
	const CUELogTime * m_LastLog;
};

class CUELogTime final
{
public:
	CUELogTime(CMaskFlyWeight * FlyWeight = nullptr);

	USHORT GetYear() const { return m_Year; }
	UCHAR GetMouth() const { return m_Mouth; }
	UCHAR GetDay() const { return m_Day; }
	UCHAR GetHour() const { return m_Hour; }
	UCHAR GetMinute() const { return m_Minute; }
	UCHAR GetSecond() const { return m_Second; }
	USHORT GetMSecond() const { return m_MSecond; }
	const CMaskFlyWeight * GetMaskFlyWeight() const { return m_MaskFlyWeight; }
	void SetMaskFlyWeight(const CMaskFlyWeight * maskFlyWeight) { m_MaskFlyWeight = maskFlyWeight; }
	CMemcutStr GetTimeStr() const { return m_TimeStr; }

	bool YearIsValid() const { return true; }
	bool MouthIsValid() const { return m_Mouth > 0 && m_Mouth <= 12; }
	bool DayIsValid() const { return true; }
	bool HourIsValid() const { return m_Hour <= 24; }
	bool MinuteIsValid() const { return m_Minute <= 60; }
	bool SecondIsValid() const { return m_Second <= 60; }
	bool MSecondIsValid() const { return m_MSecond <= 1000; }
	bool IsValid() const
	{
		return	YearIsValid() &&
			MouthIsValid() &&
			DayIsValid() &&
			HourIsValid() &&
			MinuteIsValid() &&
			SecondIsValid() &&
			MSecondIsValid();
	}

	bool operator>(const CUELogTime & Other) const;
	bool operator<(const CUELogTime & Other) const;
	bool operator==(const CUELogTime & Other) const
	{
		return	m_Year == Other.m_Year &&
			m_Mouth == Other.m_Mouth &&
			m_Day == Other.m_Day &&
			m_Hour == Other.m_Hour &&
			m_Minute == Other.m_Minute &&
			m_Second == Other.m_Second &&
			m_MSecond == Other.m_MSecond;
	}
	bool operator >= (const CUELogTime & Other) const
	{
		return *this == Other || *this > Other;
	}
	bool operator <= (const CUELogTime & Other) const
	{
		return *this == Other || *this < Other;
	}

	void ClearTimeInfo() { m_Year = m_MSecond = 0; m_Mouth = m_Day = m_Hour = m_Minute = m_Second = 0; }
	// string like 2019-12-06 07:06:26.325
	bool TryDecodeTime(const char * Str);
	// is string like 2019-12-06 07:06:26.325
	static bool StrIsTime(const char * Str, unsigned int Len);

private:
	USHORT		m_Year;
	UCHAR		m_Mouth;
	UCHAR		m_Day;
	UCHAR		m_Hour;
	UCHAR		m_Minute;
	UCHAR		m_Second;
	USHORT		m_MSecond;
	CMemcutStr  m_TimeStr;

	const CMaskFlyWeight *	m_MaskFlyWeight;
};