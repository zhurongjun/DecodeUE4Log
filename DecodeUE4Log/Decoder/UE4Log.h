#pragma once
#include "LogElement.h"
#include <functional>
#include <vector>
using std::vector;
using std::function;

class CUELog;
namespace UELogHelper
{
	// 查找Log头的结尾
	const char * FindLogHeaderEnd(const char * Begin, const char * End);
	// 比较函数
	bool CompareLogByTime_Min(CUELog * LogA, CUELog * LogB);
	bool CompareLogByTime_Max(CUELog * LogA, CUELog * LogB);
}

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
	vector<CUELog *> & GetAllLogs() { return m_AllLogs; }
	CMemcutStr GetLogHeader() { return m_LogHeader; }

	bool LoadLogFile(const char * Path);
	void Clear();

	int GetLogCount() { return m_AllLogs.size(); }

private:
	char *				m_FileContent;		// 存储这个Log内容的Buffer

	CLogTypeDictionry	m_LogTypeDictionry;	// Log类型字典，存储了所有的类型名，并建立了LogTypeCode->name的快速映射
	CMaskFlyWeight		m_MaskFlyWeight;	// 时间遮罩，为了提高按时间排序的效率

	CMemcutStr			m_LogHeader;		// 从文件头到第一行Log的Log头，UE使用这个头来记录正式启动之前的Log
	vector<CUELog *>	m_AllLogs;			// 所有的Log信息
};
