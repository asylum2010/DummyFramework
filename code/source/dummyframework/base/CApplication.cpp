//=============================================================================================================
#include <dummyframework/base/CApplication.h>
#include <dummyframework/base/CHelper.h>
#include <sstream>

namespace DummyFramework
{
	skeyboardstate  CApplication::KeyBoardState;
	smousestate     CApplication::MouseState;
	scommand        CApplication::Command;
	CApplication*   CApplication::dummyinstance = NULL;

	LRESULT WINAPI CApplication::WndProc(HWND hWnd, unsigned int msg, WPARAM wParam, LPARAM lParam)
	{
		dummyinstance->KeyBoardState.key = 0;
		dummyinstance->KeyBoardState.virtualkey = 0;
		dummyinstance->MouseState.button = 0;

		smessage Message;

		Message.wparam = wParam;
		Message.lparam = lParam;
		Message.msg = msg;
		Message.handled = false;

		dummyinstance->windowproc(Message);

		if( Message.handled )
			return TRUE;

		switch( msg )
		{
		case WM_MENUSELECT:
			if( dummyinstance->active )
			{
				dummyinstance->paintsuspended();
				dummyinstance->active = false;
			}
			else if( HIWORD(wParam) == 0xffff && lParam == NULL )
			{
				dummyinstance->paintcontinued();
				dummyinstance->active = true;
			}
			break;

		case WM_COMMAND:
			Command.wparam = wParam;
			Command.lparam = lParam;

			dummyinstance->command(Command);
			break;

		case WM_HSCROLL:
			Command.wparam = wParam;
			Command.lparam = lParam;

			dummyinstance->command(Command);
			break;

		case WM_VSCROLL:
			Command.wparam = wParam;
			Command.lparam = lParam;

			dummyinstance->command(Command);
			break;

		case WM_NOTIFY:
			Command.wparam = wParam;
			Command.lparam = lParam;

			dummyinstance->notify(Command);
			break;
		
		case WM_CLOSE:
			dummyinstance->closed();
			dummyinstance->activated.disconnectall();
			dummyinstance->deactivated.disconnectall();
			dummyinstance->paintsuspended.disconnectall();
			dummyinstance->paintcontinued.disconnectall();

			ShowWindow(hWnd, SW_HIDE);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

		case WM_ACTIVATE:
		case WM_ACTIVATEAPP:
			switch( wParam )
			{
			case WA_INACTIVE:
				if( dummyinstance->active )
				{
					dummyinstance->active = false;
					dummyinstance->deactivated();

					dsad("Application deactivated");
				}
				break;

			case WA_ACTIVE:
			case WA_CLICKACTIVE:
				if( !dummyinstance->active && dummyinstance->state != WS_Minimized )
				{
					memset(&KeyBoardState, 0, sizeof(skeyboardstate));
					MouseState.button = 0;

					dummyinstance->active = true;
					dummyinstance->activated();

					dhappy("Application activated");
				}
				break;

			default:
				break;
			}

			break;

		case WM_SYSCOMMAND:
			switch( wParam )
			{
			case SC_MINIMIZE:
				dummyinstance->state = WS_Minimized;
				break;

			case SC_MAXIMIZE:
				dummyinstance->state = WS_Maximized;
				break;

			case SC_RESTORE:
				dummyinstance->state = WS_Normal;
				break;

			default:
				break;
			}

		case WM_MOUSEMOVE:
			if( !dummyinstance->UseDirectInput )
			{
				short x = (short)(lParam & 0xffff);
				short y = (short)((lParam >> 16) & 0xffff);

				dummyinstance->MouseState.dx = x - dummyinstance->MouseState.x;
				dummyinstance->MouseState.dy = y - dummyinstance->MouseState.y;

				dummyinstance->MouseState.x = x;
				dummyinstance->MouseState.y = y;

				dummyinstance->mousemove(dummyinstance->MouseState);
			}
			break;

		case WM_LBUTTONDOWN:
			if( !dummyinstance->UseDirectInput )
			{
				dummyinstance->MouseState.button |= MB_Left;
				dummyinstance->KeyBoardState.virtualkey = (unsigned short)wParam;

				dummyinstance->MouseState.x = (short)(lParam & 0xffff);
				dummyinstance->MouseState.y = (short)((lParam >> 16) & 0xffff);

				dummyinstance->MouseState.dx = 0;
				dummyinstance->MouseState.dy = 0;

				dummyinstance->mousedown(dummyinstance->MouseState);
			}
			break;

		case WM_LBUTTONUP:
			if( !dummyinstance->UseDirectInput )
			{
				dummyinstance->MouseState.button |= MB_Left;
				dummyinstance->KeyBoardState.virtualkey = (unsigned short)wParam;

				dummyinstance->MouseState.x = (short)(lParam & 0xffff);
				dummyinstance->MouseState.y = (short)((lParam >> 16) & 0xffff);

				dummyinstance->MouseState.dx = 0;
				dummyinstance->MouseState.dy = 0;

				dummyinstance->mouseup(dummyinstance->MouseState);
			}
			break;

		case WM_RBUTTONDOWN:
			if( !dummyinstance->UseDirectInput )
			{
				dummyinstance->MouseState.button |= MB_Right;
				dummyinstance->KeyBoardState.virtualkey = (unsigned short)wParam;
				dummyinstance->mousedown(dummyinstance->MouseState);
			}
			break;

		case WM_RBUTTONUP:
			if( !dummyinstance->UseDirectInput )
			{
				dummyinstance->MouseState.button |= MB_Right;
				dummyinstance->KeyBoardState.virtualkey = (unsigned short)wParam;
				dummyinstance->mouseup(dummyinstance->MouseState);
			}
			break;

		case WM_KEYDOWN:
			dummyinstance->KeyBoardState.key = (unsigned char)wParam;
			dummyinstance->KeyBoardState.keytable[wParam] |= 0x80;
			dummyinstance->keydown(dummyinstance->KeyBoardState);
			break;

		case WM_KEYUP:
			dummyinstance->KeyBoardState.key = (unsigned char)wParam;
			dummyinstance->KeyBoardState.keytable[wParam] &= (~0x80);
			dummyinstance->keyup(dummyinstance->KeyBoardState);
			break;

		case WM_SYSKEYDOWN:
			return 0;

		default:
			break;
		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	CApplication::CApplication()
	{
		memset(&wc, 0, sizeof(WNDCLASSEXA));
		memset(&KeyBoardState, 0, sizeof(skeyboardstate));
		memset(&MouseState, 0, sizeof(smousestate));

		memset(&mstate, 0, sizeof(DIMOUSESTATE));
		memset(&mpstate, 0, sizeof(DIMOUSESTATE));

		hwnd = NULL;
		dinput = NULL;
		mouse = NULL;

		running = false;
		active = false;
		windowed = true;
		dummyinstance = this;

		Title = "Dummy Application";
		UseDirectInput = false;

		ExecutablePath.resize(256, ' ');
		GetCurrentDirectoryA(256, &ExecutablePath[0]);

		CHelper::Crop(ExecutablePath, ExecutablePath, ' ');
		CHelper::Crop(ExecutablePath, ExecutablePath, '\0');

		if( ExecutablePath[ExecutablePath.length() - 1] != '/' ||
			ExecutablePath[ExecutablePath.length() - 1] != '\\' )
		{
			ExecutablePath += "\\";
		}

		std::cout << 
			"DummyFramework v1.2\n"
			"(c) 2008-2012 Asylum\n\n";
	}
	//=============================================================================================================
	CApplication::~CApplication()
	{
		if( mouse )
		{
			mouse->Unacquire();
			mouse->Release();
			mouse = NULL;
		}

		if( dinput )
		{
			ULONG rc = dinput->Release();

			if( rc > 0 )
			{
				std::stringstream ss;
				ss << "dinput refcount == " << rc;
				dnassert(, ss.str(), true);
			}
			else
			{
				dhappy("DI device released successfully");
				dinput = NULL;
			}
		}

		UnregisterClass("DummyDXClass", NULL);
	}
	//=============================================================================================================
	void CApplication::Adjust(tagRECT& out, int& width, int& height, DWORD style, DWORD exstyle, bool menu)
	{
		int w = workarea.right - workarea.left;
		int h = workarea.bottom - workarea.top;

		out.left = (w - width) / 2;
		out.top = (h - height) / 2;
		out.right = (w + width) / 2;
		out.bottom = (h + height) / 2;

		AdjustWindowRectEx(&out, style, menu, 0);

		int windowwidth = out.right - out.left;
		int windowheight = out.bottom - out.top;

		int dw = windowwidth - width;
		int dh = windowheight - height;

		if( windowheight > h )
		{
			float ratio = (float)width / (float)height;
			float realw = (float)(h - dh) * ratio + 0.5f;

			windowheight = h;
			windowwidth = (int)floor(realw) + dw;
		}

		if( windowwidth > w )
		{
			float ratio = (float)height / (float)width;
			float realh = (float)(w - dw) * ratio + 0.5f;

			windowwidth = w;
			windowheight = (int)floor(realh) + dh;
		}

		out.left = workarea.left + (w - windowwidth) / 2;
		out.top = workarea.top + (h - windowheight) / 2;
		out.right = workarea.left + (w + windowwidth) / 2;
		out.bottom = workarea.top + (h + windowheight) / 2;

		width = windowwidth - dw;
		height = windowheight - dh;
	}
	//=============================================================================================================
	bool CApplication::Initialize(int& width, int& height, bool fullscreen, bool menu)
	{
		RECT rect = { 0, 0, width, height };
		DWORD style = WS_CLIPCHILDREN|WS_CLIPSIBLINGS;
		WCHAR szExePath[MAX_PATH];

		if( hwnd )
		{
			GetClientRect(hwnd, &rect);
			width = rect.right - rect.left;
			height = rect.bottom - rect.top;
			return true;
		}

		GetModuleFileNameW(NULL, szExePath, MAX_PATH);

		wc.cbSize			= sizeof(WNDCLASSEXA);
		wc.style			= CS_CLASSDC;
		wc.lpfnWndProc		= (WNDPROC)WndProc;
		wc.hInstance		= GetModuleHandle(NULL);
		wc.lpszClassName	= "DummyDXClass";
		wc.hbrBackground	= CreateSolidBrush(RGB(240, 240, 240));
		wc.hIcon			= ExtractIconW(wc.hInstance, szExePath, 0);

		if( !RegisterClassEx(&wc) )
			dassert(false, "CApplication::Initialize(): Could not register class", true);

		windowed = !fullscreen;
		SystemParametersInfo(SPI_GETWORKAREA, 0, &workarea, 0);

		if( windowed )
		{
			style |= (WS_SYSMENU|WS_BORDER|WS_CAPTION|WS_MINIMIZEBOX); // |WS_MAXIMIZEBOX
			Adjust(rect, width, height, style, menu);
		}
		else
			style |= (WS_POPUP|WS_EX_TOPMOST);

		hwnd = CreateWindowA("DummyDXClass", Title.c_str(), style,
			rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
			NULL, NULL, wc.hInstance, NULL);

		dassert(false, "CApplication::Initialize(): Could not create window", hwnd);

		HFONT font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
		HDC hdc = GetDC(hwnd);
		SelectObject(hdc, font);

		// directinput
		if( UseDirectInput )
		{
			HRESULT hr = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8A, (void**)&dinput, NULL);
			dnassert(false, "CApplication::Initialize(): Could not create dinput device", FAILED(hr));

			hr = dinput->CreateDevice(GUID_SysMouse, &mouse, NULL);
			dnassert(false, "CApplication::Initialize(): Could not create mouse device", FAILED(hr));

			hr = mouse->SetDataFormat(&c_dfDIMouse);
			dnassert(false, "CApplication::Initialize(): Could not set mouse data format", FAILED(hr));

			hr = mouse->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE|DISCL_BACKGROUND);
			dnassert(false, "CApplication::Initialize(): Could not set mouse coop level", FAILED(hr));

			mouse->Acquire();
		}

		POINT p;
		GetCursorPos(&p);
		ScreenToClient(hwnd, &p);

		MouseState.x = (short)p.x;
		MouseState.y = (short)p.y;

		running = true;
		active = true;

		return true;
	}
	//=============================================================================================================
	bool CApplication::Run()
	{
		if( !running )
			return false;

		MSG msg;
		ZeroMemory(&msg, sizeof(msg));

		MouseState.dx = MouseState.dy = 0;

		while( PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE) )
		{
			TranslateMessage(&msg);
			DispatchMessageA(&msg);

			if( msg.message == WM_QUIT )
				running = false;
		}

		if( UseDirectInput && mouse )
		{
			memcpy(&mpstate, &mstate, sizeof(DIMOUSESTATE));
			HRESULT hr = mouse->GetDeviceState(sizeof(DIMOUSESTATE), &mstate);

			if( FAILED(hr) )
			{
				hr = mouse->Acquire();

				if( hr == DIERR_OTHERAPPHASPRIO || hr == DIERR_NOTACQUIRED )
					return running;

				if( hr == DIERR_INPUTLOST )
					hr = mouse->Acquire();
			}
			else
			{
				MouseState.dx = (short)mstate.lX;
				MouseState.dy = (short)mstate.lY;

				MouseState.x += MouseState.dx;
				MouseState.y += MouseState.dy;

				if( mstate.rgbButtons[0] & 0x80 )
				{
					MouseState.button |= MB_Left;
					mousedown(MouseState);
				}
				else
				{
					MouseState.button |= MB_Left;

					if( mpstate.rgbButtons[0] & 0x80 )
						mouseup(MouseState);

					MouseState.button &= (~MB_Left);
				}

				if( mstate.rgbButtons[1] & 0x80 )
				{
					MouseState.button |= MB_Right;
					mousedown(MouseState);
				}
				else
				{
					MouseState.button |= MB_Right;

					if( mpstate.rgbButtons[1] & 0x80 )
						mouseup(MouseState);

					MouseState.button &= (~MB_Right);
				}

				mousemove(MouseState);
			}
		}

		return running;
	}
	//=============================================================================================================
	void CApplication::Resize(int& width, int& height, bool fullscreen, bool menu)
	{
		RECT rect;
		DWORD style = WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS;

		if( fullscreen )
			style |= WS_POPUP;
		else
			style |= WS_SYSMENU|WS_BORDER|WS_CAPTION|WS_MINIMIZEBOX; // |WS_MAXIMIZEBOX

		if( !fullscreen )
		{
			if( !windowed )
				ChangeDisplaySettings(NULL, 0);

			// w/fs -> w
			Adjust(rect, width, height, style, menu);

			SetWindowLong(hwnd, GWL_STYLE, style);
			SetWindowPos(hwnd, HWND_NOTOPMOST, rect.left, rect.top, rect.right - rect.left,
				rect.bottom - rect.top, SWP_FRAMECHANGED);
		}
		else
		{
			if( windowed )
			{
				// w -> fs
				SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, width, height, SWP_NOACTIVATE);
				SetWindowLong(hwnd, GWL_STYLE, style);
				SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER|SWP_FRAMECHANGED);
			}
			else
			{
				// fs -> fs
				SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, width, height, SWP_NOACTIVATE);
			}
		}

		windowed = !fullscreen;
	}
	//=============================================================================================================
	void CApplication::Show()
	{
		SetActiveWindow(hwnd);
		SetForegroundWindow(hwnd);
		ShowWindow(hwnd, SW_SHOWDEFAULT);
		UpdateWindow(hwnd);
	}
	//=============================================================================================================
	void CApplication::Hide()
	{
		ShowWindow(hwnd, SW_HIDE);
	}
	//=============================================================================================================
}
