#pragma once
#include "LogElement.h"
#include <functional>
#include <vector>
using std::vector;
using std::function;

class CUELog;
namespace UELogHelper
{
	// ����Logͷ�Ľ�β
	const char * FindLogHeaderEnd(const char * Begin, const char * End);
	// �ȽϺ���
	bool CompareLogByTime_Asce(CUELog * LogA, CUELog * LogB);
	bool CompareLogByTime_Desc(CUELog * LogA, CUELog * LogB);
	bool CompareLogByLevel_Asce(CUELog * LogA, CUELog * LogB);
	bool CompareLogByLevel_Desc(CUELog * LogA, CUELog * LogB);
	bool CompareLogByType_Asce(CUELog * LogA, CUELog * LogB);
	bool CompareLogByType_Desc(CUELog * LogA, CUELog * LogB);
}

class CUELogFile;
class CUELog
{
public:
	CUELog(CUELogFile * file);
	~CUELog() {}

	const CUELogTime & GetLogTime() const { return m_LogTime; }
	LogTypeCode GetLogType() const { return m_LogTypeCode; }
	ELogLevel GetLogLevel() const { return m_Level; }
	CUELogFile * GetLogFile() const { return m_LogFile; }

	void SetLogFile(CUELogFile * file) { m_LogFile = file; }

	CMemcutStr GetLogContent() const { return m_LogContent; }
	CMemcutStr GetLogTypeName() const { return m_LogTypeName; }

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
	vector<string> GetLogTypeNames() { return m_LogTypeDictionry.GetLogTypeNames(); }
	int GetLogTypeCount() { return m_LogTypeDictionry.GetLogTypeCount(); }

	const CMaskFlyWeight * GetTimeMaskFlyWeight() const { return &m_MaskFlyWeight; }
	const vector<CUELog *> & GetAllLogs() const { return m_AllLogs; }
	CMemcutStr GetLogHeader() { return m_LogHeader; }

	bool LoadLogFile(const char * Path);
	void Clear();

	int GetLogCount() { return m_AllLogs.size(); }

	const char * GetBuffer()const { return m_FileContent; }

private:
	char *				m_FileContent;		// �洢���Log���ݵ�Buffer

	CLogTypeDictionry	m_LogTypeDictionry;	// Log�����ֵ䣬�洢�����е�����������������LogTypeCode->name�Ŀ���ӳ��
	CMaskFlyWeight		m_MaskFlyWeight;	// ʱ�����֣�Ϊ����߰�ʱ�������Ч��

	CMemcutStr			m_LogHeader;		// ���ļ�ͷ����һ��Log��Logͷ��UEʹ�����ͷ����¼��ʽ����֮ǰ��Log
	vector<CUELog *>	m_AllLogs;			// ���е�Log��Ϣ
};
