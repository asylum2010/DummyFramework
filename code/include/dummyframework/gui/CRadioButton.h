//=============================================================================================================
#ifndef _CRADIOBUTTON_H_
#define _CRADIOBUTTON_H_

#include "CWindow.h"

namespace DummyFramework
{
	class CRadioButton : public CWindow
	{
	protected:
		static WNDPROC originalproc;
		static LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	public:
		CRadioButton();
		~CRadioButton();

		bool Initialize(HWND parent);
		LRESULT ProcessCommands(WPARAM wparam, LPARAM lparam);

		inline bool GetChecked() {
			return (BST_CHECKED == SendMessage(hwnd, BM_GETCHECK, 0, 0));
		}

		inline void SetChecked(bool check) {
			SendMessage(hwnd, BM_SETCHECK, (check ? BST_CHECKED : BST_UNCHECKED), 0);
		}
	};
}

#endif
//=============================================================================================================
