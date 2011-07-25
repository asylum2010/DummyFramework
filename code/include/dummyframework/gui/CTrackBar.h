//=============================================================================================================
#ifndef _CTRACKBAR_H_
#define _CTRACKBAR_H_

#include "CWindow.h"

namespace DummyFramework
{
	enum etrackbaraction
	{
		tba_moved = 2
	};

	class CTrackBar : public CWindow
	{
	protected:
		static WNDPROC originalproc;
		static LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

		unsigned short position;

	public:
		unsigned int TickFreq;

		CTrackBar();
		~CTrackBar();

		bool Initialize(HWND parent);
		LRESULT ProcessCommands(WPARAM wparam, LPARAM lparam);

		inline void SetRange(unsigned short min, unsigned short max) {
			SendMessage(hwnd, TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(min, max));
		}

		inline void SetPos(unsigned short pos) {
			SendMessage(hwnd, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)pos);
			position = pos;
		}

		inline unsigned short GetPos() const {
			return position;
		}
	};
}

#endif
//=============================================================================================================
