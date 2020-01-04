#include "LogFilter.h"
#include "UE4Log.h"
#include <algorithm>


CUELogFilter::CUELogFilter() : 
	m_LogLevelMask(0xFFFFFFFF)
{
}

CUELogFilter::CUELogFilter(CUELogFile * File) :
	m_LogLevelMask(0xFFFFFFFF)
{
	SetFile(File);
}

CUELogFilter::~CUELogFilter()
{
}

void CUELogFilter::SetFile(CUELogFile * File)
{
	if (!File) return;
	m_LogFile = File;
	const vector<CUELog *> Logs = File->GetAllLogs();

	m_LogTypeMask.resize(File->GetLogTypeCount());
	for (auto it = m_LogTypeMask.begin(); it != m_LogTypeMask.end(); it++)
	{
		*it = true;
	}
	m_FiltResult.clear();
}

void CUELogFilter::Filtrate()
{
	if (!m_LogFile) return;
	const vector<CUELog *> Logs = m_LogFile->GetAllLogs();

	m_FiltResult.clear();

	for (auto val : Logs)
	{
		if (!(m_LogLevelMask & val->GetLogLevel())) continue;
		if (!m_LogTypeMask[val->GetLogType() - 1]) continue;
		m_FiltResult.push_back(val);
	}
}

void CUELogSorter::SortByTime_Asce()
{
	std::sort(m_Logs.begin(), m_Logs.end(), &UELogHelper::CompareLogByTime_Asce);
}

void CUELogSorter::SortByTime_Desc()
{
	std::sort(m_Logs.begin(), m_Logs.end(), &UELogHelper::CompareLogByTime_Desc);
}

void CUELogSorter::SortByLogType_Asce()
{
	std::sort(m_Logs.begin(), m_Logs.end(), &UELogHelper::CompareLogByType_Asce);
}

void CUELogSorter::SortByLogType_Desc()
{
	std::sort(m_Logs.begin(), m_Logs.end(), &UELogHelper::CompareLogByType_Desc);
}

void CUELogSorter::SortByLogLevel_Asce()
{
	std::sort(m_Logs.begin(), m_Logs.end(), &UELogHelper::CompareLogByLevel_Asce);
}

void CUELogSorter::SortByLogLevel_Desc()
{
	std::sort(m_Logs.begin(), m_Logs.end(), &UELogHelper::CompareLogByLevel_Desc);
}
