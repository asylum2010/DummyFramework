//*************************************************************************************************************
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "ComCtl32.lib")
#pragma comment(lib, "UxTheme.lib")

#include "winapitest.h"

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
		WinapiTest game;
		game.Run();
	}

	_CrtDumpMemoryLeaks();

	system("pause");
	return 0;
}
//*************************************************************************************************************

