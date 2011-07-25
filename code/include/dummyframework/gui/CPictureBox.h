//=============================================================================================================
#ifndef _CPICTUREBOX_H_
#define _CPICTUREBOX_H_

#include "CWindow.h"

namespace DummyFramework
{
	class CPictureBox : public CWindow
	{
	protected:
		static WNDPROC originalproc;
		static LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	public:
		CPictureBox();
		~CPictureBox();

		bool Initialize(HWND parent);
		LRESULT ProcessCommands(WPARAM wparam, LPARAM lparam);
	};
}

#endif
//=============================================================================================================
