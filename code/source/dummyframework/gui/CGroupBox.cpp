//=============================================================================================================
#include <dummyframework/gui/CGroupBox.h>

namespace DummyFramework
{
	CGroupBox::CGroupBox()
		: CWindow()
	{
		Name = "groupbox";
		Text = "groupbox";
	}
	//=============================================================================================================
	CGroupBox::~CGroupBox()
	{
	}
	//=============================================================================================================
	bool CGroupBox::Initialize(HWND parent)
	{
		id = GenerateID();

		hwnd = CreateWindowExA(
			WS_EX_TRANSPARENT,
			"BUTTON",
			Text.c_str(),
			WS_VISIBLE|WS_CHILD|BS_GROUPBOX,
			Location.x,
			Location.y,
			Size.x,
			Size.y,
			parent,
			(HMENU)id,
			(HINSTANCE)GetWindowLong(parent, GWL_HINSTANCE), 
			NULL);
		
		HFONT font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
		SendMessage(hwnd, WM_SETFONT, (WPARAM)font, MAKELPARAM(TRUE, 0));

		return (hwnd != NULL);
	}
	//=============================================================================================================
	LRESULT CGroupBox::ProcessCommands(WPARAM wparam, LPARAM lparam)
	{
		return 0;
	}
	//=============================================================================================================
}
