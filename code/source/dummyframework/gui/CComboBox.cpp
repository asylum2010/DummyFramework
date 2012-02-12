//=============================================================================================================
#include <dummyframework/gui/CComboBox.h>
#include <Vssym32.h>

// http://msdn.microsoft.com/en-us/library/windows/desktop/hh404152%28v=vs.85%29.aspx
// http://msdn.microsoft.com/en-us/library/windows/desktop/dd373487%28v=vs.85%29.aspx
// http://msdn.microsoft.com/en-us/library/windows/desktop/bb773210%28v=vs.85%29.aspx

// http://www.codeguru.com/cpp/controls/buttonctrl/advancedbuttons/article.php/c5161
// http://www.windows-api.com/microsoft/Win32-UI/29257426/dropdown-list-with-cbsdropdownlist--cbsownerdrawvariable-in-vista.aspx

namespace DummyFramework
{
	WNDPROC CComboBox::originalproc = 0;

	LRESULT CComboBox::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		windowmap::iterator it = wndmap.find(hwnd);

		if( it != wndmap.end() )
		{
			CComboBox* combo = (CComboBox*)it->second;

			switch( msg )
			{
			/*
			case WM_PAINT:
				{
					PAINTSTRUCT ps;
					RECT rc;
					HRESULT hr;

					//HTHEME button = OpenThemeData(hwnd, L"BUTTON");

					HDC hdc = BeginPaint(hwnd, &ps);
					{
						GetWindowRect(hwnd, &rc);

						HDC ndc = 0;

						BeginBufferedPaint(hdc, &rc, , , &ndc);

						hr = DrawThemeBackground(combo->theme, hdc, CP_READONLY, 0, &rc, 0);
						//hr = DrawThemeBackground(button, hdc, BP_PUSHBUTTON, PBS_NORMAL, &rc, 0);
					}
					EndPaint(hwnd, &ps);
					//CloseThemeData(button);
				}
				return 0;
			*/

			case WM_DESTROY:
				if( combo->theme )
				{
					CloseThemeData(combo->theme);
					combo->theme = NULL;
				}

				break;
			
			default:
				break;
			}
		}

		return CallWindowProc(originalproc, hwnd, msg, wparam, lparam);
	}

	CComboBox::CComboBox()
		: CWindow()
	{
		Name = "combobox";
		DropDownStyle = dds_dropdown;

		theme = NULL;
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
			WS_VISIBLE|WS_CHILD|DropDownStyle|CBS_OWNERDRAWFIXED|CBS_HASSTRINGS,
			Location.x,
			Location.y,
			Size.x,
			Size.y,
			parent,
			(HMENU)id,
			(HINSTANCE)GetWindowLong(parent, GWL_HINSTANCE), 
			NULL);

		if( IsAppThemed() )
			theme = OpenThemeData(hwnd, L"COMBOBOXSTYLE;COMBOBOX");

		if( !originalproc )
		{
			originalproc = (WNDPROC)GetWindowLong(hwnd, GWL_WNDPROC);
			derror(false, "CComboBox::Initialize(): Could not get window procedure", originalproc);
		}

        long ret = SetWindowLong(hwnd, GWL_WNDPROC, (LONG)&CComboBox::WndProc);
        derror(false, "CComboBox::Initialize(): Could not set window procedure", ret);

		HFONT font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
		SendMessage(hwnd, WM_SETFONT, (WPARAM)font, MAKELPARAM(TRUE, 0));

		return CWindow::Initialize(parent);
	}
	//=============================================================================================================
	void CComboBox::RenderItem(LPDRAWITEMSTRUCT ds)
	{
		TCHAR itemtext[256];
		TEXTMETRIC tm;
		COLORREF fg, bg;
		int x, y, i = 0;
		int textlen;
		
		// measure item
		GetTextMetrics(ds->hDC, &tm);

		y = (ds->rcItem.bottom + ds->rcItem.top - tm.tmHeight) / 2;
		x = LOWORD(GetDialogBaseUnits()) / 4;

		// get the item's text
		SendMessage(ds->hwndItem, CB_GETLBTEXT, ds->itemID, (LPARAM)itemtext);
		textlen = strlen(itemtext);

		if( itemtext[0] == '\\' )
		{
			fg = SetTextColor(
				ds->hDC, GetSysColor(COLOR_GRAYTEXT));

			i = 1;
			--textlen;
		}
		else
		{
			bg = SetBkColor(
				ds->hDC, GetSysColor((ds->itemState & ODS_SELECTED) ? COLOR_HIGHLIGHT : COLOR_WINDOW));

			fg = SetTextColor(
				ds->hDC, GetSysColor((ds->itemState & ODS_SELECTED) ? COLOR_HIGHLIGHTTEXT : COLOR_WINDOWTEXT));
		}

		ExtTextOut(ds->hDC, 2 * x, y, ETO_CLIPPED|ETO_OPAQUE, &ds->rcItem,
			&itemtext[i], (UINT)textlen, NULL);

		// restore colors
		SetTextColor(ds->hDC, fg);

		if( i != 1 )
			SetBkColor(ds->hDC, bg);

		// draw focus rect
		if( ds->itemState & ODS_FOCUS && i != 1 )
			::DrawFocusRect(ds->hDC, &ds->rcItem);
	}
	//=============================================================================================================
	bool CComboBox::Render(LPDRAWITEMSTRUCT ds)
	{
		if( ds->itemID == -1 )
			return false;

		if( theme )
		{
			/*
			if( ds->itemAction == ODA_DRAWENTIRE )
			{
				HRESULT hr;

				//if( ds->itemState == 
				//printf("hello\n");

				if( ds->itemState == ODS_COMBOBOXEDIT ) {
					hr = DrawThemeBackground(theme, ds->hDC, CP_DROPDOWNBUTTON, CBXS_NORMAL, &ds->rcItem, 0);
				}
			}
			*/

			/*
			if( ds->itemState == ODS_COMBOBOXEDIT )
			{
				// render the combo box itself
				RECT rc;
				HRESULT hr;

				GetWindowRect(hwnd, &rc);
				hr = DrawThemeBackground(theme, ds->hDC, CP_DROPDOWNBUTTON, CBXS_NORMAL, &rc, 0);

				return false;
			}
			else
			*/
				RenderItem(ds);
		}
		else
			RenderItem(ds);

		return true;
	}
	//=============================================================================================================
	LRESULT CComboBox::ProcessCommands(WPARAM wparam, LPARAM lparam)
	{
		return 0;
	}
	//=============================================================================================================
}
