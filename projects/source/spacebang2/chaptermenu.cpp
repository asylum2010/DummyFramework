//=============================================================================================================
#include "chaptermenu.h"

ChapterMenu::ChapterMenu()
	: CForm()
{
	Background		= NULL;
	Thumbs			= NULL;
	CurrentLevel	= 0;
	selectedindex	= 0;

	alpha.direction	= -1;
	alpha.set(MENU_TRANSITION, 0, 1.0f);
	alpha = 0;

	AddControl(back);
	AddControl(title);
}
//=============================================================================================================
void ChapterMenu::Fill(gamedesc& desc)
{
	Thumbs = desc.texture;
	thumbs.resize(desc.levels.size());

	for( size_t i = 0; i < desc.levels.size(); ++i )
	{
		thumbs[i].SetText(desc.levels[i].title);

		memcpy(thumbs[i].Texcoords, desc.levels[i].texcoords, 4 * sizeof(float));
		AddControl(thumbs[i]);
	}
}
//=============================================================================================================
void ChapterMenu::Draw()
{
	D3DXMATRIX transform(Background->Size.x, 0, 0, 0, 0, Background->Size.y, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	D3DXCOLOR color(0xffffffff);

	alpha.smooth((float)game->Sync.Alpha());
	color.a = alpha.value;

	transform._41 = (GameVariables::ScreenWidth - Background->Size.x) * 0.5f - 0.5f;
	transform._42 = (GameVariables::ScreenHeight - Background->Size.y) * 0.5f - 0.5f;

	game->StateManager->SetTexture(0, Background->Texture);
	game->DrawImage(transform, Background->Texcoords, color);

	back.Color.a = title.Color.a = alpha.value;

	for( size_t i = 0; i < thumbs.size(); ++i )
		thumbs[i].Color.a = alpha.value;

	game->StateManager->SetTexture(0, Thumbs);
	CForm::Draw();

	spritefont->Draw(GroupID);
}
//=============================================================================================================
bool ChapterMenu::Initialize(DummyFramework::CGame9& mygame, DummyFramework::CSpriteFont9& font)
{
	onresetdevice();

	back.SetText("Back to main menu");
	title.SetText("Load chapter");

	return CForm::Initialize(mygame, font);
}
//=============================================================================================================
void ChapterMenu::SetState(unsigned int newstate)
{
	switch( newstate )
	{
	case TransitionIn:
		alpha = 0;
		ticks = 0;
		alpha.direction = 1;

		back.SetState(Inactive);
		title.SetState(Idle);

		for( size_t i = 0; i < thumbs.size(); ++i )
			thumbs[i].SetState(Inactive);

		break;

	case TransitionOut:
		alpha = alpha.count() - 1;
		ticks = 0;
		alpha.direction = -1;

		back.SetState(Inactive);

		for( size_t i = 0; i < thumbs.size(); ++i )
			thumbs[i].SetState(Inactive);
		break;

	default:
		break;
	}

	CControl::SetState(newstate);
}
//=============================================================================================================
void ChapterMenu::Update()
{
	alpha.step();

	switch( state )
	{
	case TransitionIn:
		if( ticks >= alpha.count() )
		{
			SetState(Idle);
			SelectedIndexChanged(0);
		}
		else
			++ticks;
		break;

	case TransitionOut:
		if( ticks >= alpha.count() )
		{
			selected(action);
			focuslost(*this);
		}
		else
			++ticks;
		break;

	default:
		break;
	}

	CForm::Update();
}
//=============================================================================================================
void ChapterMenu::SelectedIndexChanged(size_t prev)
{
	if( prev < thumbs.size() )
		thumbs[prev].SetState(Inactive);
	else
		back.SetState(Inactive);

	if( selectedindex < thumbs.size() )
	{
		thumbs[selectedindex].SetState(Idle);
		action = LoadGame;
		CurrentLevel = selectedindex;
	}
	else
	{
		back.SetState(Idle);
		action = MainMenu;
	}
}
//=============================================================================================================
void ChapterMenu::onfocusgained()
{
	CForm::onfocusgained();
	selectedindex = CurrentLevel;
	SetState(TransitionIn);
}
//=============================================================================================================
void ChapterMenu::onfocuslost()
{
	CForm::onfocuslost();
	SetState(Hidden);

	back.SetState(Hidden);
	title.SetState(Hidden);

	for( size_t i = 0; i < thumbs.size(); ++i )
		thumbs[i].SetState(Hidden);
}
//=============================================================================================================
void ChapterMenu::onresetdevice()
{
	title.Position.x = GameVariables::ScreenWidth * 0.5f;
	title.Position.y = GameVariables::ScreenHeight * 0.08f * 1.6f;

	back.Position.x = title.Position.x;
	back.Position.y = GameVariables::ScreenHeight - title.Position.y * 1.2f;

	D3DXVECTOR2 size(
		GameVariables::CorrelateH(1130),
		GameVariables::CorrelateH(554));

	D3DXVECTOR2 thumbsize(
		GameVariables::CorrelateH(260),
		GameVariables::CorrelateH(195));

	D3DXVECTOR2 spacing(
		GameVariables::CorrelateH(30),
		GameVariables::CorrelateH(94));

	for( size_t i = 0; i < thumbs.size(); ++i )
	{
		thumbs[i].Position.x =
			(GameVariables::ScreenWidth - size.x) * 0.5f +
			(i % 4) * (thumbsize.x + spacing.x) + thumbsize.x * 0.5f;

		thumbs[i].Position.y =
			(GameVariables::ScreenHeight - size.y) * 0.5f +
			(i / 4) * (thumbsize.y + spacing.y) + thumbsize.y * 0.5f;

		thumbs[i].Size = thumbsize;
	}

	CForm::onresetdevice();
}
//=============================================================================================================
void ChapterMenu::onkeyup(const DummyFramework::skeyboardstate& kstate)
{
	if( state == TransitionIn || state == TransitionOut )
		return;

	switch( kstate.key )
	{
	case VK_RETURN:
	case VK_SPACE:
		SetState(TransitionOut);
		break;

	case VK_DOWN:
	case 0x53:
		{
			size_t prev = selectedindex;

			if( selectedindex == thumbs.size() )
				selectedindex = 0;
			else
			{
				selectedindex = selectedindex + 4;

				if( selectedindex >= thumbs.size() )
					selectedindex = thumbs.size();
			}

			SelectedIndexChanged(prev);
		}
		break;

	case VK_UP:
	case 0x57:
		{
			size_t prev = selectedindex;

			if( selectedindex == thumbs.size() )
				--selectedindex;
			else
			{
				if( selectedindex < 4 )
					selectedindex = thumbs.size();
				else
					selectedindex = selectedindex - 4;
			}

			SelectedIndexChanged(prev);
		}
		break;

	case VK_LEFT:
	case 0x41:
		{
			size_t prev = selectedindex;

			if( selectedindex == 0 )
				selectedindex = thumbs.size() - 1;
			else if( selectedindex == thumbs.size() )
				selectedindex = thumbs.size() - 1;
			else
				--selectedindex;

			SelectedIndexChanged(prev);
		}
		break;

	case VK_RIGHT:
	case 0x44:
		{
			size_t prev = selectedindex;

			if( selectedindex == thumbs.size() - 1 )
				selectedindex = 0;
			else if( selectedindex == thumbs.size() )
				selectedindex = 0;
			else
				++selectedindex;

			SelectedIndexChanged(prev);
		}
		break;

	default:
		break;
	}
}
//=============================================================================================================
void ChapterMenu::onmouseup(const DummyFramework::smousestate& mstate)
{
	if( state == TransitionIn || state == TransitionOut )
		return;

	if( selectedindex < thumbs.size() )
	{
		if( thumbs[selectedindex].MouseOver(mstate.x, mstate.y) )
			SetState(TransitionOut);
	}
	else if( back.MouseOver(mstate.x, mstate.y) )
		SetState(TransitionOut);
}
//=============================================================================================================
void ChapterMenu::onmousemove(const DummyFramework::smousestate& mstate)
{
	if( state == TransitionOut )
		return;

	size_t current = thumbs.size() + 1;

	// intentionally don't stop (consider low framerate)
	for( size_t i = 0; i < thumbs.size(); ++i )
	{
		if( thumbs[i].MouseOver(mstate.x, mstate.y) )
			current = i;
	}

	if( current > thumbs.size() )
	{
		if( back.MouseOver(mstate.x, mstate.y) )
			current = thumbs.size();
	}

	if( current < thumbs.size() + 1 && selectedindex != current )
	{
		std::swap(selectedindex, current);
		SelectedIndexChanged(current);
	}
}
//=============================================================================================================
