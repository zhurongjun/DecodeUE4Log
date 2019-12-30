#pragma once
#include "LogElement.h"
#include <functional>
#include <vector>
using std::vector;
using std::function;

class CUELogFile;
class CUELog
{
public:
	CUELog(CUELogFile * file);
	~CUELog() {}

	const CUELogTime & GetLogTime() const { return m_LogTime; }
	LogTypeCode GetLogType() const { return m_LogTypeCode; }
	ELogLevel GetLevel() const { return m_Level; }
	string GetLogContent() const { return m_LogContent.GetString(); }
	CUELogFile * GetLogFile() const { return m_LogFile; }

	void SetLogFile(CUELogFile * file) { m_LogFile = file; }
	string GetLogTypeName() { return m_LogTypeName.GetString(); }

	bool TryDecodeLog(const char * Begin, const char * End, const char * & IndexAfterDecode);

private:
	CUELogTime		m_LogTime;
	LogTypeCode		m_LogTypeCode;
	ELogLevel		m_Level;
	
	CMemcutStr		m_LogTypeName;
	CMemcutStr		m_LogContent;

	CUELogFile *	m_LogFile;
};

namespace UELogDecodeHelper
{
	const char * FindLogHeaderEnd(const char * Begin, const char * End);
	void UpdateMaskFlyWeight(int & MaskCheckValue, CMaskFlyWeight * maskFlyWeight, const CUELogTime & time);
}

class CUELogFile
{
public:
	CUELogFile();
	~CUELogFile();

	bool ContainLogType(LogTypeCode code) const { return m_LogTypeDictionry.Contain(code); }
	bool ContainLogType(string name) const { return m_LogTypeDictionry.Contain(name); }
	string GetLogTypeName(LogTypeCode code) const { return m_LogTypeDictionry.GetTypeName(code); }
	LogTypeCode GetLogTypeCode(string name) { return m_LogTypeDictionry.GetTypeCode(name); }

	const CMaskFlyWeight * GetTimeMaskFlyWeight() const { return &m_MaskFlyWeight; }

	bool LoadLogFile(const char * Path);
	void Clear();

	int GetLogCount() { return m_AllLogs.size(); }

private:
	CLogTypeDictionry	m_LogTypeDictionry;
	char *				m_FileContent;
	CMaskFlyWeight		m_MaskFlyWeight;

	CMemcutStr			m_LogHeader;
	vector<CUELog *>	m_AllLogs;
};
