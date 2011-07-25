//=============================================================================================================
#ifndef _CPROGRESSBAR_H_
#define _CPROGRESSBAR_H_

#include "CWindow.h"

namespace DummyFramework
{
	class CProgressBar : public CWindow
	{
	public:
		CProgressBar();
		~CProgressBar();

		bool Initialize(HWND parent);
		LRESULT ProcessCommands(WPARAM wparam, LPARAM lparam);
	};
}

#endif
//=============================================================================================================
