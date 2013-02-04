//=============================================================================================================
#include <dummyframework/menu/CLabel.h>

namespace DummyFramework
{
	CLabel::CLabel()
		: CControl()
	{
		Text = "Label";
		Color = 0xffffffff;
		Position.x = Position.y = 0;
	}
	//=============================================================================================================
	bool CLabel::Initialize(CGame9& mygame, CSpriteFont9& font)
	{
		return CControl::Initialize(mygame, font);
	}
	//=============================================================================================================
	bool CLabel::LoadContent()
	{
		textid = spritefont->AddText(GroupID, Text);
		spritefont->SetTextVisibility(GroupID, textid, false);

		return CControl::LoadContent();
	}
	//=============================================================================================================
	void CLabel::SetState(unsigned int newstate)
	{
		switch( newstate )
		{
		case Hidden:
			spritefont->SetTextVisibility(GroupID, textid, false);
			break;

		case TransitionIn:
		case TransitionOut:
		case Idle:
		case Inactive:
			spritefont->SetTextVisibility(GroupID, textid, true);
			break;

		default:
			break;
		}

		CControl::SetState(newstate);
	}
	//=============================================================================================================
	void CLabel::Draw()
	{
		if( state != Hidden )
			spritefont->Set(GroupID, textid, Text, Position, Alignment, (DWORD)Color);
	}
	//=============================================================================================================
}
