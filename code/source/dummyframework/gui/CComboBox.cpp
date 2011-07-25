//=============================================================================================================
#include <dummyframework/gui/CComboBox.h>

namespace DummyFramework
{
	CComboBox::CComboBox()
		: CWindow()
	{
		Name = "combobox";
		DropDownStyle = dds_dropdown;
	}
	//=============================================================================================================
	CComboBox::~CComboBox()
	{
	}
	//=============================================================================================================
	bool CComboBox::Initialize(HWND parent)
	{
		id = GenerateID();

		hwnd = CreateWindowA(
			"COMBOBOX",
			Name.c_str(),
			WS_VISIBLE|WS_CHILD|DropDownStyle,
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
	LRESULT CComboBox::ProcessCommands(WPARAM wparam, LPARAM lparam)
	{
		return 0;
	}
	//=============================================================================================================
}
