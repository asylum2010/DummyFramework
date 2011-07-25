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
	void CButton::SetState(unsigned long newstate)
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
            spritefont->Set(GroupID, textid, Text, Position, Alignment, (DWORD)currentcolor);
    }
	//=============================================================================================================
}
