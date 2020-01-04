#pragma once
#include "LogElement.h"

class CUELogFile;
class CUELog;
class CUELogFilter
{
public:
	CUELogFilter();
	CUELogFilter(CUELogFile * File);
	~CUELogFilter();

	void SetFile(CUELogFile * File);

	void OpenLogLevelFilter(ELogLevel Level){ m_LogLevelMask |= Level; }
	void CloseLogLevelFilter(ELogLevel Level) { m_LogLevelMask -= Level; }
	void OpenAllLogLevelFilter() { m_LogLevelMask = 0xFFFFFFFF; }
	void CloseAllLogLevelFilter() { m_LogLevelMask = 0; }

	void OpenLogTypeFilter(LogTypeCode Code) { if(Code <= m_LogTypeMask.size() && Code > 0) m_LogTypeMask[Code - 1] = true; }
	void CloseLogTypeFilter(LogTypeCode Code) { if (Code <= m_LogTypeMask.size() && Code > 0) m_LogTypeMask[Code - 1] = false; }
	void OpenAllLogTypeFilter() { for (auto it = m_LogTypeMask.begin(); it != m_LogTypeMask.end(); it++) (*it) = true; }
	void CloseAllLogTypeFilter() { for (auto it = m_LogTypeMask.begin(); it != m_LogTypeMask.end(); it++) (*it) = false; }

	void Filtrate();
	const vector<CUELog *> & GetFiltResult() const { return m_FiltResult; }
	vector<CUELog *> & GetFiltResult() { return m_FiltResult; }

private:
	CUELogFile *			m_LogFile;
	vector<CUELog *>		m_FiltResult;
	
	unsigned int	m_LogLevelMask;
	vector<bool>	m_LogTypeMask;
};

class CUELogSorter
{
public:
	CUELogSorter(vector<CUELog *> & Logs) : m_Logs(Logs) {}

	vector<CUELog *> & GetResult() { return m_Logs; }

	void SortByTime_Asce();
	void SortByTime_Desc();
	void SortByLogLevel_Asce();
	void SortByLogLevel_Desc();
	void SortByLogType_Asce();
	void SortByLogType_Desc();

private:
	vector<CUELog *> & m_Logs;
};