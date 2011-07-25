//=============================================================================================================
#ifndef _CSTATUSSTRIP_H_
#define _CSTATUSSTRIP_H_

#include "CWindow.h"

namespace DummyFramework
{
	class CStatusStrip : public CWindow
	{
	protected:
		int dividers[256];
		int numdividers;

	public:
		CStatusStrip();
		~CStatusStrip();

		bool Initialize(HWND parent);
		LRESULT ProcessCommands(WPARAM wparam, LPARAM lparam);

		inline void AddDivider(int pos) {
			dividers[numdividers] = pos;
			++numdividers;
		}
	};
}

#endif
//=============================================================================================================
