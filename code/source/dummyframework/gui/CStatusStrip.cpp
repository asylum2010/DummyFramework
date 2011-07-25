//=============================================================================================================
#include <dummyframework/gui/CStatusStrip.h>

namespace DummyFramework
{
	CStatusStrip::CStatusStrip()
		: CWindow()
	{
		numdividers = 0;
		Name = "statusstrip";
	}
	//=============================================================================================================
	CStatusStrip::~CStatusStrip()
	{
	}
	//=============================================================================================================
	bool CStatusStrip::Initialize(HWND parent)
	{
		hwnd = CreateWindowEx(
			0, 
			STATUSCLASSNAME,
			Text.c_str(),
			WS_CHILD|WS_VISIBLE,
			0,
			0,
			0,
			0,
			parent,
			0,
			(HINSTANCE)GetWindowLong(parent, GWL_HINSTANCE),
			NULL);

		if( hwnd )
		{
			RECT rect;
			GetClientRect(hwnd, &rect);

			dividers[numdividers] = rect.right;
			SendMessage(hwnd, SB_SETPARTS, (WPARAM)(numdividers + 1), (LPARAM)dividers);
		}

		return (hwnd != NULL);
	}
	//=============================================================================================================
	LRESULT CStatusStrip::ProcessCommands(WPARAM wparam, LPARAM lparam)
	{
		return 0;
	}
	//=============================================================================================================
}
