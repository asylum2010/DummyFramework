//=============================================================================================================
#include <dummyframework/menu/CControl.h>
#include <d3dx9.h>

namespace DummyFramework
{
	CControl::CControl()
	{
		hasfocus		= false;
		initialized		= false;
		contentloaded	= false;
		game			= NULL;
		state			= Hidden;

		GroupID			= 0xffffffff;
		Alignment		= DT_CENTER|DT_VCENTER;
	}
	//=============================================================================================================
	bool CControl::Initialize(CGame9& mygame, CSpriteFont9& font)
	{
		game = &mygame;
		spritefont = &font;
		initialized = true;

		return true;
	}
	//=============================================================================================================
	void CControl::onfocuslost()
	{
		hasfocus = false;
	}
	//=============================================================================================================
	void CControl::onfocusgained()
	{
		hasfocus = true;
		focusgained(*this);
	}
	//=============================================================================================================
}
