//=============================================================================================================
#include <dummyframework/gui/CTrackBar.h>

namespace DummyFramework
{
	WNDPROC CTrackBar::originalproc = 0;

	LRESULT CTrackBar::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
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
	CTrackBar::CTrackBar()
		: CWindow()
	{
		position = 0;
		Name = "trackbar";
		TickFreq = 1;
	}
	//=============================================================================================================
	CTrackBar::~CTrackBar()
	{
	}
	//=============================================================================================================
	bool CTrackBar::Initialize(HWND parent)
	{
		id = GenerateID();

		hwnd = CreateWindowExA( 
			0,
			TRACKBAR_CLASS,
			Name.c_str(),
			WS_CHILD|WS_VISIBLE|TBS_AUTOTICKS,
			Location.x,
			Location.y,
			Size.x,
			Size.y,
			parent,
			(HMENU)id,
			(HINSTANCE)GetWindowLong(parent, GWL_HINSTANCE),
			NULL);

		derror(false, "CTrackBar::Initialize(): Could not create window", hwnd);

		if( !originalproc )
		{
			originalproc = (WNDPROC)GetWindowLong(hwnd, GWL_WNDPROC);
			derror(false, "CTrackBar::Initialize(): Could not get window procedure", originalproc);
		}

		int ret = SetWindowLong(hwnd, GWL_WNDPROC, (LONG)&CTrackBar::WndProc);
		derror(false, "CTrackBar::Initialize(): Could not set window procedure", ret);

		SetRange(0, 10);

		SendMessage(hwnd, TBM_SETTICFREQ, TickFreq, 0);
		SendMessage(hwnd, TBM_SETPAGESIZE, 0, (LPARAM)1);
		SendMessage(hwnd, TBM_SETLINESIZE, 0, (LPARAM)1);
		
		return CWindow::Initialize(parent);
	}
	//=============================================================================================================
	LRESULT CTrackBar::ProcessCommands(WPARAM wparam, LPARAM lparam)
	{
		LRESULT ret = 0;
		WORD msg = LOWORD(wparam);

		switch( msg )
		{
		case TB_LINEUP:
		case TB_LINEDOWN:
		case TB_PAGEUP:
		case TB_PAGEDOWN:
			position = (unsigned short)SendMessageA(hwnd, TBM_GETPOS, 0, 0);
			ret = tba_moved;
			break;

		case TB_THUMBTRACK:
			position = HIWORD(wparam);
			ret = tba_moved;
			break;

		default:
			break;
		}

		return ret;
	}
	//=============================================================================================================
}
