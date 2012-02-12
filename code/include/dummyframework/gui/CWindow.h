//=============================================================================================================
#ifndef _CWINDOW_H_
#define _CWINDOW_H_

#include "../template/defines.h"
#include "../template/signal.hpp"

#include <windows.h>
#include <commctrl.h>
#include <string>
#include <map>

#define _DUMMY_INPUT_EVENTS \
    case WM_KEYDOWN: \
	case WM_KEYUP: \
	case WM_LBUTTONDOWN: \
	case WM_LBUTTONUP: \
	case WM_RBUTTONDOWN: \
	case WM_RBUTTONUP: \

namespace DummyFramework
{
	enum ewindowaction
	{
		wa_defproc = 0
	};

	class CWindow
	{
		static WORD numids;

	protected:
		typedef std::map<HWND, CWindow*> windowmap;
		static windowmap wndmap;

		inline static WORD GenerateID() {
			++numids;
			return numids + 999;
		}

		WORD id;
		HWND hwnd;
		HWND parent;

	public:
		static bool CheckError();
		static CWindow* GetWindowByHandle(HWND hwnd);

		bool DelegateInput;
		bool DrawFocusRect;

		point2<int> Location;
		point2<int> Size;
		std::string Name;
		std::string Text;

		CWindow();
		virtual ~CWindow() {}

		virtual bool Initialize(HWND parent) = 0;
		virtual LRESULT ProcessCommands(WPARAM wparam, LPARAM lparam) = 0;

		inline HWND GetHandle() {
			return hwnd;
		}

		inline HWND GetParent() {
			return parent;
		}

		inline WORD ID() const {
			return id;
		}

		inline bool Contains(short x, short y) {
			return (x >= Location.x && y >= Location.y) && ((x <= Location.x + Size.x) && (y <= Location.y + Size.y));
		}

		inline void SetText(const std::string& text) {
			Text = text;
			SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)Text.c_str());
		}

		virtual void Enable(bool enable) const;
	};

	inline void CWindow::Enable(bool enable) const {
		EnableWindow(hwnd, enable);
	}
}

#endif
//=============================================================================================================
