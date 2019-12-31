#include "Decoder/UE4Log.h"
#include <stdio.h>
#include <string>
#include <algorithm>

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
	std::sort(Log.GetAllLogs().begin(), Log.GetAllLogs().end(), &UELogHelper::CompareLogByTime_Min);

	for (int i = 0; i < 100; i++)
	{
		printf("%s\n", Log.GetAllLogs()[i]->GetLogTime().GetTimeStr().GetString().c_str());
	}

	system("Pause");
	return 0;
}