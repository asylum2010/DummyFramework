//=============================================================================================================
#ifndef _CGROUPBOX_H_
#define _CGROUPBOX_H_

#include "CWindow.h"

namespace DummyFramework
{
	class CGroupBox : public CWindow
	{
	public:
		CGroupBox();
		~CGroupBox();

		bool Initialize(HWND parent);
		LRESULT ProcessCommands(WPARAM wparam, LPARAM lparam);
	};
}

#endif
//=============================================================================================================
