//=============================================================================================================
#include <dummyframework/gui/CWindow.h>
#include <dummyframework/base/CHelper.h>
#include <dummyframework/template/defines.h>

namespace DummyFramework
{
	WORD CWindow::numids = 0;
	CWindow::windowmap CWindow::wndmap;

	CWindow::CWindow()
		: hwnd(0), parent(0)
	{
		id = 0xffff;

		Name = "control";
		Location.x = Location.y = 0;
		Size.x = Size.y = 0;

		DelegateInput = false;
		DrawFocusRect = true;
	}
	//=============================================================================================================
	bool CWindow::Initialize(HWND parent)
	{
		this->parent = parent;

		if( hwnd )
			wndmap[hwnd] = this;

		return true;
	}
	//=============================================================================================================
	bool CWindow::CheckError()
	{
		DWORD err = GetLastError();
		char buff[1024];

		memset(buff, 0, 1024);

		DWORD cnt = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, err, 0, buff, 1024, 0);
		buff[cnt] = '\0';

		std::string str1, str2(buff, cnt - 2);
		
		if( err > 0 )
		{
			dnerror(false, str2 << " (" << err << ")", true);
		}
		else
		{
			dhappy("CWindow::CheckError(): No error occured");
		}

		return true;
	}
	//=============================================================================================================
	CWindow* CWindow::GetWindowByHandle(HWND hwnd)
	{
		windowmap::iterator it = wndmap.find(hwnd);

		if( it == wndmap.end() )
			return NULL;

		return it->second;
	}
	//=============================================================================================================
}