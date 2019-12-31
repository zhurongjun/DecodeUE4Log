#include "UE4Log.h"
#include <stdio.h>
#include <io.h>

CUELog::CUELog(CUELogFile * file) : 
	m_LogTypeCode(0),
	m_Level(ELogLevel::Unknow), 
	m_LogFile(file) 
{ 
	if (file) 
		m_LogTime.SetMaskFlyWeight(file->GetTimeMaskFlyWeight()); 
}

bool CUELog::TryDecodeLog(const char * Begin, const char * End, const char *& IndexAfterDecode)
{
	IndexAfterDecode = nullptr;

	if (!m_LogFile) return false;

	// try decode time
	if (!m_LogTime.TryDecodeTime(Begin + 1)) return false;
	
	// move pointer to LogTypeName
	Begin += TIMESTR_LENGTH + 4;
	while (*Begin != ']')
	{
		++Begin;
		if (Begin >= End) return false;
	}
	++Begin;

	// decode LogTypeName
	short NameLength = 0;
	while (*Begin != ':')
	{
		++NameLength;
		++Begin;
		if (Begin >= End) return false;
	}
	m_LogTypeName.SetStr(Begin - NameLength, Begin);
	m_LogTypeCode = m_LogFile->GetLogTypeCode(m_LogTypeName.GetString());
	Begin += 2;	// move to log content

	// decode log level and move pointer to content
	if (Begin[0] == 'W' && Begin[1] == 'a' && Begin[2] == 'r' && Begin[3] == 'n' && Begin[4] == 'i' && Begin[5] == 'n' && Begin[6] == 'g')
	{
		m_Level = ELogLevel::Warning;
		Begin += 9;
	}
	else if (Begin[0] == 'E' && Begin[1] == 'r' && Begin[2] == 'r' && Begin[3] == 'o' && Begin[4] == 'r')
	{
		m_Level = ELogLevel::Error;
		Begin += 7;
	}
	else
	{
		m_Level = ELogLevel::Message;
	}

	// find next log
	const char * LastBegin = Begin;
	while (*Begin != '[' || !CUELogTime::StrIsTime(Begin + 1, TIMESTR_LENGTH))
	{
		++Begin;

		// current log is the last log
		if (Begin >= End)
		{
			break;
		}
	}

	// current index is the begin of a log
	m_LogContent.SetStr(LastBegin, Begin);
	IndexAfterDecode = Begin;
	return true;
}

CUELogFile::CUELogFile():
	m_FileContent(nullptr)
{
}

CUELogFile::~CUELogFile()
{
	Clear();
}

bool CUELogFile::LoadLogFile(const char * Path)
{
	Clear();
	if (!Path) return false;

	// verify file exist
	int accessRet = _access(Path, 0);
	if (accessRet == ENOENT || accessRet == EINVAL) return false;

	// get file size
	struct stat buf;
	stat(Path, &buf);

	// create buf and read file
	m_FileContent = new char[buf.st_size];
	FILE * fp = nullptr;
	fopen_s(&fp, Path, "rb+");

	int size = fread(m_FileContent, 1, buf.st_size, fp);
	fclose(fp);

	// find header
	const char * BufBegin = m_FileContent;
	const char * BufEnd = m_FileContent + size;
	const char * CurIndex = BufBegin;
	CurIndex = UELogHelper::FindLogHeaderEnd(BufBegin, BufEnd);
	m_LogHeader.SetStr(BufBegin, CurIndex);

	// loop decode log
	CUELog * TempLog = nullptr;
	const char * NextIndex = nullptr;
	while (CurIndex < BufEnd)
	{
		TempLog = new CUELog(this);
		
		bool IsDecodeSucess = TempLog->TryDecodeLog(CurIndex, BufEnd, NextIndex);

		m_MaskFlyWeight.UpDateFlyWeight(&TempLog->GetLogTime());
		CurIndex = NextIndex;

		m_AllLogs.push_back(TempLog);

		// decode failed
		if (!IsDecodeSucess)
		{
			Clear();
			return false;
		}
	}

	return true;
}

void CUELogFile::Clear()
{
	for (CUELog* Log : m_AllLogs)
	{
		delete Log;
	}
	m_AllLogs.clear();
	if (m_FileContent) delete[] m_FileContent;
	m_FileContent = nullptr;
	m_LogTypeDictionry.Clear();
}

const char * UELogHelper::FindLogHeaderEnd(const char * Begin, const char * End)
{
	if (!(Begin && End)) return nullptr;
	const char * CurIndex = Begin;

	// find first Log Index
	while (*CurIndex != '[' || !CUELogTime::StrIsTime(CurIndex + 1, TIMESTR_LENGTH))
	{
		++CurIndex;
	}
	return CurIndex;
}

bool UELogHelper::CompareLogByTime_Max(CUELog * LogA, CUELog * LogB)
{
	if (!LogA) return true;
	if (!LogB) return false;
	return LogA->GetLogTime() > LogB->GetLogTime();
}

bool UELogHelper::CompareLogByTime_Min(CUELog * LogA, CUELog * LogB)
{
	if (!LogA) return false;
	if (!LogB) return true;
	return LogA->GetLogTime() < LogB->GetLogTime();
}
