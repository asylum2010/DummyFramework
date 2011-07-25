//=============================================================================================================
#ifndef _CAPPLICATION_H_
#define _CAPPLICATION_H_

#include "../template/signal.hpp"
#include "../template/defines.h"

#define DIRECTINPUT_VERSION 0x0800

#include <windows.h>
#include <string>
#include <dinput.h>

#ifdef AddForm
#   undef AddForm
#endif

namespace DummyFramework
{
	class CWindow;

    enum emousebutton
    {
        MB_Left = 1,
        MB_Right = 2
    };

    enum ewindowstate
    {
        WS_Normal,
        WS_Minimized,
        WS_Maximized
    };

    struct skeyboardstate
    {
        unsigned char key;
        unsigned char keytable[256];
        unsigned short virtualkey;
    };

    struct smousestate
    {
        short x;
        short y;
        short dx;
        short dy;
        unsigned char button;
    };

	struct scommand
	{
		WPARAM wparam;
		LPARAM lparam;
	};

	/**
	 * \brief Win32 based application class
	 */
    class CApplication
    {
    protected:
		LPDIRECTINPUT8          dinput;
        LPDIRECTINPUTDEVICE8    mouse;
        DIMOUSESTATE            mstate;
		DIMOUSESTATE            mpstate;

        WNDCLASSEXA    wc;
		RECT           workarea;
        HWND           hwnd;
        bool           running;
        bool           active;
		bool           windowed;
        unsigned char  state;

        static CApplication* dummyinstance;
        static LRESULT WINAPI WndProc(HWND hWnd, unsigned int msg, WPARAM wParam, LPARAM lParam);

		void Adjust(tagRECT& out, long& width, long& height, DWORD style, DWORD exstyle = 0, bool menu = false);

    public:
        static skeyboardstate KeyBoardState;
        static smousestate MouseState;
		static scommand Command;

		std::string ExecutablePath;
        std::string Title;
		bool UseDirectInput;

        CApplication();
        ~CApplication();

        bool Initialize(long& width, long& height, bool fullscreen, bool menu = false);
        bool Run();
        
        void Resize(long& width, long& height, bool fullscreen, bool menu = false);
        void Show();
		void Hide();
        
        inline HWND GetWindowHandle() { return hwnd; }
        inline bool Active() const { return active; }

        inline void Close() {
            SendMessage(hwnd, WM_CLOSE, 0, 0);
            running = false;
        }

    events:
        signal0 activated;
        signal0 deactivated;
		signal0 paintsuspended;
		signal0 paintcontinued;
		signal0 closed;

        signal1<const skeyboardstate&> keydown;
        signal1<const skeyboardstate&> keyup;
        signal1<const smousestate&>    mousedown;
        signal1<const smousestate&>    mouseup;
        signal1<const smousestate&>    mousemove;
		signal1<const scommand&>       command;
		signal1<const scommand&>       notify;
    };
}

#endif
//=============================================================================================================


