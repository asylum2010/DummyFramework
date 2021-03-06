//*************************************************************************************************************
#include "emptyproject10.h"

#pragma comment(lib, "../../../extern/lib/dxguid.lib")
#pragma comment(lib, "../../../extern/lib/dinput8.lib")

#ifdef _DEBUG
#	pragma comment(lib, "../../../code/lib/Debug/DummyFramework.lib")
#	pragma comment(lib, "../../../extern/lib/dx10/d3d10.lib")
#	pragma comment(lib, "../../../extern/lib/dx10/d3dx10.lib") // d3dx10d.lib

#	define _CRTDBG_MAP_ALLOC
#	include <stdlib.h>
#	include <crtdbg.h>
#else
#	pragma comment(lib, "../../../code/lib/Release/DummyFramework.lib")
#	pragma comment(lib, "../../../extern/lib/dx10/d3d10.lib")
#	pragma comment(lib, "../../../extern/lib/dx10/d3dx10.lib")
#endif

int main()
{
	{
		EmptyProject10 game;
		game.Run();
	}

	_CrtDumpMemoryLeaks();

	system("pause");
	return 0;
}
//*************************************************************************************************************

