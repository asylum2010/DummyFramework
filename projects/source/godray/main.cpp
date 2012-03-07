//*************************************************************************************************************
#include "godray.h"

#pragma comment(lib, "../../../extern/dxguid.lib")
#pragma comment(lib, "../../../extern/dinput8.lib")

#ifdef _DEBUG
#	pragma comment(lib, "../../../code/lib/Debug/DummyFramework.lib")
#   pragma comment(lib, "../../../extern/d3d9.lib")
#   pragma comment(lib, "../../../extern/d3dx9.lib") // d3dx9d.lib
 
#   define _CRTDBG_MAP_ALLOC
#   include <stdlib.h>
#   include <crtdbg.h>
#else
#   pragma comment(lib, "../../../code/lib/Release/DummyFramework.lib")
#   pragma comment(lib, "../../../extern/d3d9.lib")
#   pragma comment(lib, "../../../extern/d3dx9.lib")
#endif

int main()
{
	{
		Godray game;
		game.Run();
	}

	_CrtDumpMemoryLeaks();

	system("pause");
	return 0;
}
//*************************************************************************************************************

