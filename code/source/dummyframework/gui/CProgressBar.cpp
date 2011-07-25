//=============================================================================================================
#include <dummyframework/gui/CProgressBar.h>

namespace DummyFramework
{
	CProgressBar::CProgressBar()
		: CWindow()
	{
		Name = "progressbar";
	}
	//=============================================================================================================
	CProgressBar::~CProgressBar()
	{
	}
	//=============================================================================================================
	bool CProgressBar::Initialize(HWND parent)
	{
		hwnd = CreateWindowEx(
			0, 
			PROGRESS_CLASS,
			Name.c_str(),
			WS_CHILD|WS_VISIBLE,
			Location.x,
			Location.y,
			Size.x,
			Size.y,
			parent,
			0,
			(HINSTANCE)GetWindowLong(parent, GWL_HINSTANCE),
			NULL);

		return (hwnd != NULL);
	}
	//=============================================================================================================
	LRESULT CProgressBar::ProcessCommands(WPARAM wparam, LPARAM lparam)
	{
		return 0;
	}
	//=============================================================================================================
}
