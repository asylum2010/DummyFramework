//*************************************************************************************************************
#include "emptyproject9.h"

#pragma comment(lib, "../../../extern/lib/dxguid.lib")
#pragma comment(lib, "../../../extern/lib/dinput8.lib")

#ifdef _DEBUG
#	pragma comment(lib, "../../../code/lib/Debug/DummyFramework.lib")
#	pragma comment(lib, "../../../extern/lib/dx9/d3d9.lib")
#	pragma comment(lib, "../../../extern/lib/dx9/d3dx9.lib") // d3dx9d.lib

#	define _CRTDBG_MAP_ALLOC
#	include <stdlib.h>
#	include <crtdbg.h>
#else
#	pragma comment(lib, "../../../code/lib/Release/DummyFramework.lib")
#	pragma comment(lib, "../../../extern/lib/dx9/d3d9.lib")
#	pragma comment(lib, "../../../extern/lib/dx9/d3dx9.lib")
#endif

int main()
{
	{
		EmptyProject9 game;
		game.Run();
	}

	_CrtDumpMemoryLeaks();

	system("pause");
	return 0;
}
//*************************************************************************************************************

