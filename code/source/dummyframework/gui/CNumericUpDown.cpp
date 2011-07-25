//=============================================================================================================
#include <dummyframework/gui/CNumericUpDown.h>

namespace DummyFramework
{
	CNumericUpDown::CNumericUpDown()
		: CWindow()
	{
		buddy = 0;
		buddyid = 0;
		value = 0;

		Name = "numericupdown";
		Text = "numericupdown";

		MinValue = 0;
		MaxValue = 10;
		Increment = 1;
		Precision = 0;
	}
	//=============================================================================================================
	CNumericUpDown::~CNumericUpDown()
	{
	}
	//=============================================================================================================
	bool CNumericUpDown::Initialize(HWND parent)
	{
		id = GenerateID();
		buddyid = GenerateID();

		Text.resize(11);

		if( Precision == 0 )
		{
			sprintf_s(&Text[0], 11, "%d", (int)value);
		}
		else
		{
			char format[] = "%.1f";
			format[2] = Precision + 0x30;

			sprintf_s(&Text[0], 11, format, value);
		}

		hwnd = CreateWindowExA(
			0,
			UPDOWN_CLASS,
			0,
			WS_VISIBLE|WS_CHILD|UDS_ALIGNRIGHT,
			Location.x,
			Location.y,
			14,
			16,
			parent,
			(HMENU)id,
			(HINSTANCE)GetWindowLong(parent, GWL_HINSTANCE), 
			NULL);

		buddy = CreateWindowExA(
			WS_EX_CLIENTEDGE,
			"EDIT",
			Text.c_str(),
			WS_VISIBLE|WS_CHILD|ES_NUMBER,
			Location.x,
			Location.y,
			Size.x,
			Size.y,
			parent,
			(HMENU)buddyid,
			(HINSTANCE)GetWindowLong(parent, GWL_HINSTANCE), 
			NULL);
		
		HFONT font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
		SendMessage(buddy, WM_SETFONT, (WPARAM)font, MAKELPARAM(TRUE, 0));
		SendMessage(hwnd, UDM_SETBUDDY, (WPARAM)buddy, 0);
		SendMessage(hwnd, UDM_SETRANGE, 0, MAKELPARAM(10, 0));

		return (hwnd != NULL);
	}
	//=============================================================================================================
	void CNumericUpDown::SetValue(double v)
	{
		value = v;

		if( Precision == 0 )
		{
			sprintf_s(&Text[0], 11, "%d", (int)value);
		}
		else
		{
			char format[] = "%.1f";
			format[2] = Precision + 0x30;

			sprintf_s(&Text[0], 11, format, value);
		}

		SendMessage(buddy, WM_SETTEXT, 0, (LPARAM)Text.c_str());
	}
	//=============================================================================================================
	LRESULT CNumericUpDown::ProcessCommands(WPARAM wparam, LPARAM lparam)
	{
		LRESULT ret = 0;
		NMHDR* hdr = (NMHDR*)lparam;
	
		if( hdr->code == UDN_DELTAPOS )
		{
			NMUPDOWN* ud = (NMUPDOWN*)lparam;

			value += ud->iDelta * Increment;
			value = min(max(value, MinValue), MaxValue);

			SetValue(value);
			ret = uda_valuechanged;
		}

		return ret;
	}
	//=============================================================================================================
}
