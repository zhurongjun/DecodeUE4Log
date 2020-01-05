#pragma comment(lib,"d3d11.lib")
#include <stdio.h>
#include <string>
#include <algorithm>
#include <tchar.h>
#include "Decoder/UE4Log.h"
#include "Decoder/LogFilter.h"


int main()
{
	CUELogFile Log;
	char Path[256];
	scanf_s("%s", Path, 256);
	if (Path[0] == '"')
	{
		Path[strlen(Path) - 1] = 0;
	}
	Log.LoadLogFile(Path);

	printf("¹²½âÎö%dÌõLog\n", Log.GetLogCount());

	vector<string> AllNames = Log.GetLogTypeNames();

	for (auto val : AllNames)
	{
		printf("%s\n", val.c_str());
	}
	
	CUELogFilter Filter(&Log);
	Filter.CloseAllLogLevelFilter();
	Filter.CloseAllLogTypeFilter();
	Filter.OpenLogLevelFilter(ELogLevel::Error);
	Filter.OpenLogTypeFilter(Log.GetLogTypeCode("LogResMgr"));
	Filter.Filtrate();
	
	decltype(auto) FiltResult = Filter.GetFiltResult();
	CUELogSorter sorter(FiltResult);
	sorter.SortByTime_Asce();
	for (auto val : FiltResult)
	{
		printf("%s  %s  %s\n", val->GetLogTime().GetTimeStr().ToString().c_str(), val->GetLogTypeName().ToString().c_str(), val->GetLogContent().ToString().c_str());
	}



	system("Pause");
	return 0;
}