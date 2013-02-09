//=============================================================================================================
#include <dummyframework/menu/CButton.h>

namespace DummyFramework
{
	CButton::CButton()
		: CLabel()
	{
		ActiveColor = 0xffffff00;
	}
	//=============================================================================================================
	void CButton::SetState(unsigned int newstate)
	{
		switch( newstate )
		{
		case Idle:
			currentcolor = ActiveColor;
			break;

		case Inactive:
		case Hidden:
			currentcolor = Color;
			break;

		default:
			break;
		}

		CLabel::SetState(newstate);
	}
	//=============================================================================================================
	void CButton::Draw()
	{
		currentcolor.a = Color.a;

		if( state != Hidden )
			spritefont->Set(GroupID, textid, text, Position, Alignment, (DWORD)currentcolor);
	}
	//=============================================================================================================
}
