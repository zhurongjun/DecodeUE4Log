#include "Decoder/UE4Log.h"
#include <stdio.h>
#include <string>

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

	system("Pause");
	return 0;
}