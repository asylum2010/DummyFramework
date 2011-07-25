//=============================================================================================================
#ifndef _CCOMBOBOX_H_
#define _CCOMBOBOX_H_

#include "CWindow.h"

namespace DummyFramework
{
	enum edropdownstyle
	{
		dds_simple = CBS_SIMPLE,
		dds_dropdown = CBS_DROPDOWN,
		dds_dropdownlist = CBS_DROPDOWNLIST
	};

	class CComboBox : public CWindow
	{
	public:
		edropdownstyle DropDownStyle;

		CComboBox();
		~CComboBox();

		bool Initialize(HWND parent);
		LRESULT ProcessCommands(WPARAM wparam, LPARAM lparam);

		inline void AddString(const std::string& str) {
			SendMessage(hwnd, CB_ADDSTRING, 0L, (LPARAM)str.c_str());
		}

		inline void SetCurrentItem(int index) {
			SendMessage(hwnd, CB_SETCURSEL, (WPARAM)index, 0L);
		}
	};
}

#endif
//=============================================================================================================
