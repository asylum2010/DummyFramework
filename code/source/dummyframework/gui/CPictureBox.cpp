//=============================================================================================================
#include <dummyframework/gui/CPictureBox.h>

namespace DummyFramework
{
	WNDPROC CPictureBox::originalproc = 0;

	LRESULT CPictureBox::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
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
			
			default:
				break;
			}
		}

		return CallWindowProc(originalproc, hwnd, msg, wparam, lparam);
	}
	//=============================================================================================================
	CPictureBox::CPictureBox()
		: CWindow()
	{
		Name = "picturebox";
	}
	//=============================================================================================================
	CPictureBox::~CPictureBox()
	{
	}
	//=============================================================================================================
	bool CPictureBox::Initialize(HWND parent)
	{
		if( hwnd )
			return true;

		hwnd = CreateWindowA(
			"STATIC",
			Name.c_str(),
			WS_VISIBLE|WS_CHILD|SS_BLACKRECT,
			Location.x,
			Location.y,
			Size.x,
			Size.y,
			parent,
			0,
			(HINSTANCE)GetWindowLong(parent, GWL_HINSTANCE), 
			NULL);

		derror(false, "CPictureBox::Initialize(): Could not create window", hwnd);

		if( !originalproc )
		{
			originalproc = (WNDPROC)GetWindowLong(hwnd, GWL_WNDPROC);
			derror(false, "CPictureBox::Initialize(): Could not get window procedure", originalproc);
		}

		int ret = SetWindowLong(hwnd, GWL_WNDPROC, (LONG)&CPictureBox::WndProc);
		derror(false, "CPictureBox::Initialize(): Could not set window procedure", ret);

		return CWindow::Initialize(parent);
	}
	//=============================================================================================================
	LRESULT CPictureBox::ProcessCommands(WPARAM wparam, LPARAM lparam)
	{
		return 0;
	}
	//=============================================================================================================
}
