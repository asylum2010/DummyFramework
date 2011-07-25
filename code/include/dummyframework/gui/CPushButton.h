//=============================================================================================================
#ifndef _CPUSHBUTTON_H_
#define _CPUSHBUTTON_H_

#include "CWindow.h"

namespace DummyFramework
{
	class CPushButton : public CWindow
	{
	protected:
		static WNDPROC originalproc;
		static LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	public:
		CPushButton();
		~CPushButton();

		bool Initialize(HWND parent);
		LRESULT ProcessCommands(WPARAM wparam, LPARAM lparam);
	};
}

#endif
//=============================================================================================================
