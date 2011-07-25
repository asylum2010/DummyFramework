//=============================================================================================================
#include <dummyframework/gui/CRadioButton.h>

namespace DummyFramework
{
	WNDPROC CRadioButton::originalproc = 0;

	LRESULT CRadioButton::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		windowmap::iterator it = wndmap.find(hwnd);

		if( it != wndmap.end() )
		{
			switch( msg )
			{
			_DUMMY_INPUT_EVENTS
				if( it->second->DelegateInput )
					SendMessage(it->second->GetParent(), msg, wparam, lparam);
				break;

			case WM_SETFOCUS:
				if( !it->second->DrawFocusRect )
					return 0;
				break;

			default:
				break;
			}
		}

		return CallWindowProc(originalproc, hwnd, msg, wparam, lparam);
	}
	//=============================================================================================================
	CRadioButton::CRadioButton()
		: CWindow()
	{
		Name = "radiobutton";
		Text = "radiobutton";
	}
	//=============================================================================================================
	CRadioButton::~CRadioButton()
	{
	}
	//=============================================================================================================
	bool CRadioButton::Initialize(HWND parent)
	{
		id = GenerateID();

		hwnd = CreateWindowExA(
			0,
			"BUTTON",
			Text.c_str(),
			WS_VISIBLE|WS_CHILD|BS_AUTORADIOBUTTON,
			Location.x,
			Location.y,
			Size.x,
			Size.y,
			parent,
			(HMENU)id,
			(HINSTANCE)GetWindowLong(parent, GWL_HINSTANCE), 
			NULL);
		
		derror(false, "CRadioButton::Initialize(): Could not create window", hwnd);
        
		if( !originalproc )
		{
			originalproc = (WNDPROC)GetWindowLong(hwnd, GWL_WNDPROC);
			derror(false, "CRadioButton::Initialize(): Could not get window procedure", originalproc);
		}

        long ret = SetWindowLong(hwnd, GWL_WNDPROC, (LONG)&CRadioButton::WndProc);
        derror(false, "CRadioButton::Initialize(): Could not set window procedure", ret);

		HFONT font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
		SendMessage(hwnd, WM_SETFONT, (WPARAM)font, MAKELPARAM(TRUE, 0));

		return CWindow::Initialize(parent);
	}
	//=============================================================================================================
	LRESULT CRadioButton::ProcessCommands(WPARAM wparam, LPARAM lparam)
	{
		return 0;
	}
	//=============================================================================================================
}
