//=============================================================================================================
#include <dummyframework/gui/CStatic.h>

namespace DummyFramework
{
	CStatic::CStatic()
		: CWindow()
	{
		Align = al_left;
		Name = "label";
	}
	//=============================================================================================================
	CStatic::~CStatic()
	{
	}
	//=============================================================================================================
	bool CStatic::Initialize(HWND parent)
	{
		SIZE s;
		HDC hdc = GetDC(parent);

		GetTextExtentPoint32A(hdc, Text.c_str(), Text.length(), &s);

		hwnd = CreateWindowA(
			"STATIC",
			Text.c_str(),
			WS_VISIBLE|WS_CHILD|Align,
			Location.x,
			Location.y,
			s.cx,
			s.cy,
			parent,
			0,
			(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), 
			NULL);
		
		Size.x = s.cx;
		Size.y = s.cy;

		HFONT font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
		SendMessage(hwnd, WM_SETFONT, (WPARAM)font, MAKELPARAM(TRUE, 0));

		return (hwnd != NULL);
	}
	//=============================================================================================================
	LRESULT CStatic::ProcessCommands(WPARAM wparam, LPARAM lparam)
	{
		return 0;
	}
	//=============================================================================================================
}
