//=============================================================================================================
#ifndef _CSTATIC_H_
#define _CSTATIC_H_

#include "CWindow.h"

namespace DummyFramework
{
	enum ealign
	{
		al_simple = SS_SIMPLE,
		al_left = SS_LEFT,
		al_right = SS_RIGHT
	};

	class CStatic : public CWindow
	{
	public:
		ealign Align;

		CStatic();
		~CStatic();

		bool Initialize(HWND parent);
		LRESULT ProcessCommands(WPARAM wparam, LPARAM lparam);
	};
}

#endif
//=============================================================================================================
