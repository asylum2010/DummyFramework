//=============================================================================================================
#include "coopgame.h"

#include <dummyframework/base/CHelper.h>
#include <dummyframework/base/d3d9/CGame9.h>
#include <sstream>

unsigned long examplecolors4[672] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xffffff00, 0xffffff00, 0xff00ffff, 0xff00ffff, 0xff0000ff, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xffffff00, 0xffffff00, 0xff00ffff, 0xff00ffff, 0xff00ff00, 0xff8a2be2, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff00ff00, 0xff00ff00, 0xffffa500, 0xffffa500, 0xff00ffff, 0xff00ffff, 0xff8a2be2, 0xff00ff00, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xffffa500, 0xff0000ff, 0xff0000ff, 0xff8a2be2, 0xff0000ff, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff8a2be2, 0xffffa500, 0xffffa500, 0xff0000ff, 0xffffff00, 0xffffff00, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff00ff00, 0xff00ff00, 0xff8a2be2, 0xff8a2be2, 0xff8a2be2, 0xffffa500, 0xff0000ff, 0xffffff00, 0xffffff00, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xffffff00, 0xffffff00, 0xffffa500, 0xffffa500, 0xff0000ff, 0xff0000ff, 0xffffa500, 0xffffff00, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xffffa500, 0, 0xffffff00, 0xffffff00, 0xffffa500, 0xffffa500, 0xff0000ff, 0xff0000ff, 0xff00ffff, 0xffffff00, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xffffa500, 0xffffa500, 0xffffa500, 0xffff0000, 0xffff0000, 0xffff0000, 0xff00ff00, 0xff00ff00, 0xffff0000, 0xffffff00, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xffffa500, 0xffffa500, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xffffff00, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xffffa500, 0, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0, 0xff00ff00, 0xff00ff00, 0xff8a2be2, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xffffa500, 0, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffff9bf5, 0xff00ff00, 0xff8a2be2, 0xff8a2be2, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xff00ff00, 0, 0xffff9bf5, 0xff00ff00, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xff00ff00, 0xff8a2be2, 0, 0xff00ff00, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xffffff00, 0xffffff00, 0xffffa500, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xff00ff00, 0xff8a2be2, 0xff0000ff, 0xff8a2be2, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xffffff00, 0xffffff00, 0xffffa500, 0xffffff00, 0xffffff00, 0xffffff00, 0xffffff00, 0xff00ff00, 0xff00ff00, 0xffffff00, 0xffff0000, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xffffa500, 0xffffa500, 0xff00ffff, 0xff00ffff, 0xff00ffff, 0xff00ffff, 0xff00ffff, 0xff00ffff, 0xffffff00, 0xffff0000, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
//=============================================================================================================
CoopGame::CoopGame()
	: CForm()
{
	Atlas = NULL;
	Cell = Glow = NULL;
	
	blink.set(0, 1);
	blink = blink.count() - 1;
	blink.direction = 1;
	
	action = PauseMenu;
	ticks = 0;

	for( size_t i = 0; i < 5; ++i )
        AddControl(labels[i]);

	table.gameover.connect(this, &CoopGame::ongameover);
	Reset(true);
}
//=============================================================================================================
CoopGame::~CoopGame()
{
}
//=============================================================================================================
bool CoopGame::Initialize(DummyFramework::CGame9& mygame, DummyFramework::CSpriteFont9& font)
{
	ResetGUI();

	labels[0].Text = "Game time: 0";
	labels[1].Text = "Players";
	labels[2].Text = "Score: 0";
	labels[3].Text = "Game over";
	labels[4].Text = "Press any key to continue";

	CForm::Initialize(mygame, font);
	quads.Initialize(mygame.Graphics, 1500);
	
	return true;
}
//=============================================================================================================
bool CoopGame::LoadContent()
{
	CForm::LoadContent();

	table.Cell = Cell;
	table.Glow = Glow;

	for( size_t i = 0; i < 3; ++i )
		labels[i].SetState(Idle);

	return true;
}
//=============================================================================================================
void CoopGame::Draw()
{
	blink.smooth((float)game->Sync.Alpha());

	size_t count = 0;
	float w = GameVariables::ScreenWidth - GameVariables::CorrelateH(276);

	w = floor(w / Cell->Size.x) * Cell->Size.x;

	table.X = w * 0.5f - (table.Width / 2) * Cell->Size.x;
	table.Y = 0;

	labels[3].Position.x = table.X + table.Width / 2 * Cell->Size.x;
	labels[4].Position.x = labels[3].Position.x;

	labels[3].Position.y = GameVariables::ScreenHeight * 0.5f;
	labels[4].Position.y = GameVariables::ScreenHeight * 0.3f;

	float s = GameVariables::CorrelateH(276);
	float r = GameVariables::ScreenWidth - s;
	float h1 = GameVariables::CorrelateH(210);
	float h2 = GameVariables::CorrelateH(420);

	quads.Lock();
    {
		count += table.Write(count, quads, blink.value);

		count += DrawTetro(count, table.Next1, D3DXVECTOR2(r + s * 0.3116f, h1));
		count += DrawTetro(count, table.Next2, D3DXVECTOR2(r + s * 0.3116f, h2));
	}
	quads.Unlock();

	game->StateManager->SetTexture(0, Atlas);
    quads.Draw(count);

	// szövegek frissitése
	size_t mins, secs;
	std::stringstream ss("");
	
	mins = (size_t)((double)gametime * game->Sync.UpdateInterval);
	secs = mins % 60;
	mins = mins / 60;

	ss.str("");
	ss << "Game time: " << (mins < 10 ? "0" : "") <<
		mins << ":" << (secs < 10 ? "0" : "") << secs;

	labels[0].Text = ss.str();
	
	ss.str("");
	ss << "Score: " << table.Score;
	labels[2].Text = ss.str();
	
	labels[0].Color.a = labels[1].Color.a = labels[2].Color.a =
		labels[3].Color.a = labels[4].Color.a = blink.value;

	CForm::Draw();
    spritefont->Draw(GroupID);
}
//=============================================================================================================
size_t CoopGame::DrawTetro(size_t start, const Tetromino& tetro, const D3DXVECTOR2& pos)
{
	const std::string& shape = tetro.GetOrientedShape();
	size_t count = 0;

	D3DXVECTOR2 p;
	D3DXCOLOR color = tetro.Color;
	D3DXCOLOR white(0xffffffff);

	color.a = white.a = blink.value;
	
    for( int i = 0; i < 4; ++i )
    {
        for( int j = 0; j < 4; ++j )
        {
			p.x = pos.x + Cell->Size.x * i;
			p.y = pos.y + Cell->Size.y * j;

            if( shape[i + 4 * j] == '1' )
            {
                quads.WriteQuad(start + count, p, Cell->Size, Cell->Texcoords, color);
				quads.WriteQuad(start + count + 1, p, Glow->Size, Glow->Texcoords, white);

				count += 2;
            }
        }
    }

	return count;
}
//=============================================================================================================
void CoopGame::Reset(bool clear)
{
	juststarted = true;
	gametime = 0;

	players[0].movedir = 0;
	players[0].rotated = false;
	players[0].firststep = false;

	players[1].movedir = 0;
	players[1].rotated = false;
	players[1].firststep = false;

	table.Reset();
	blink = 0;

	if( clear )
	{
		table.Fill(examplecolors4);
	}

	if( contentloaded )
	{
		labels[3].SetState(Hidden);
		labels[4].SetState(Hidden);
	}
}
//=============================================================================================================
void CoopGame::ResetGUI()
{
	labels[0].Alignment = DT_LEFT|DT_TOP;
	labels[2].Alignment = DT_LEFT|DT_TOP;

	labels[1].Alignment = DT_CENTER|DT_TOP;
	labels[3].Alignment = DT_CENTER|DT_TOP;
	labels[4].Alignment = DT_CENTER|DT_TOP;
	
	float pw = GameVariables::CorrelateH(276);
	float off = pw * 0.07f;

	labels[0].Position.x = GameVariables::ScreenWidth - pw + off;
	labels[0].Position.y = GameVariables::CorrelateH(20);

	labels[1].Position.x = GameVariables::ScreenWidth - pw * 0.5f;
	labels[1].Position.y = GameVariables::CorrelateH(126);

	labels[2].Position.x = labels[0].Position.x;
	labels[2].Position.y = GameVariables::CorrelateH(166);
}
//=============================================================================================================
void CoopGame::SetState(unsigned long newstate)
{
	switch( newstate )
	{
	case Idle:
		if( juststarted )
		{
			juststarted = false;

			table.Clear();
		}
		break;

	case TransitionIn:
		labels[3].SetState(Hidden);
		labels[4].SetState(Hidden);
		break;
	
	case BlinkIn:
		blink = 0;
		blink.direction = 1;
		break;

	case BlinkOut:
		blink = blink.count() - 1;
		blink.direction = -1;
		break;

	case Dead:
		ticks = 0;
		break;

	default:
		break;
	}

	CForm::SetState(newstate);
}
//=============================================================================================================
void CoopGame::Update()
{
	blink.step();

	switch( state )
	{
	case Idle:
		// ha folyamatosan le van nyomva a gomb akkor mozogjon
		if( !players[0].firststep && players[0].movedir != 0 )
			players[0].movedir = 0;

		if( !players[1].firststep && players[1].movedir != 0 )
			players[1].movedir = 0;

		table.Update();

		++gametime;
		break;

	case TransitionIn:
		SetState(Idle);
		focusgained(*this);
		break;

	case TransitionOut:
        focuslost(*this);
		break;
	
	case BlinkOut:
		if( blink.previous() == 0 )
			Reset(true);
		break;
	
	case Dead:
		if( ticks > 30 )
		{
			labels[4].SetState(Idle);
		}

		++ticks;
		break;

	default:
		break;
	}
	
	CForm::Update();
}
//=============================================================================================================
void CoopGame::onfocusgained()
{
	CForm::onfocusgained();
	SetState(TransitionIn);
}
//=============================================================================================================
void CoopGame::onfocuslost()
{
	CForm::onfocuslost();
	SetState(Inactive);
}
//=============================================================================================================
void CoopGame::onkeyup(const DummyFramework::skeyboardstate& kstate)
{
	if( state == Dead && ticks > 35 )
	{
		action = GameOver;

		focuslost(*this);
		SetState(BlinkOut);

		return;
	}

	if( state != Idle )
		return;
	
	if( kstate.key == VK_ESCAPE )
	{
		action = PauseMenu;
		SetState(TransitionOut);
	}
	
	if( kstate.keytable[players[0].bindings.a[Down]] == 0 )
		table.Fast1 = false;

	if( kstate.keytable[players[1].bindings.a[Down]] == 0 )
		table.Fast2 = false;

	if( kstate.keytable[players[0].bindings.a[Rotate]] == 0 )
		players[0].rotated = false;

	if( kstate.keytable[players[1].bindings.a[Rotate]] == 0 )
		players[1].rotated = false;
	
	if( kstate.keytable[players[0].bindings.a[Left]] == 0 ||
		kstate.keytable[players[0].bindings.a[Right]] == 0 )
	{
		players[0].movedir = 0;
		players[0].firststep = false;
	}

	if( kstate.keytable[players[1].bindings.a[Left]] == 0 ||
		kstate.keytable[players[1].bindings.a[Right]] == 0 )
	{
		players[1].movedir = 0;
		players[1].firststep = false;
	}
}
//=============================================================================================================
void CoopGame::onkeydown(const DummyFramework::skeyboardstate& kstate)
{
	if( state != Idle )
		return;

	if( kstate.keytable[players[0].bindings.a[Down]] & 0x80 )
		table.Fast1 = true;
	else if( kstate.keytable[players[0].bindings.a[Rotate]] & 0x80 && !players[0].rotated )
	{
		players[0].rotated = true;
		table.Rotate(0);
	}

	if( kstate.keytable[players[1].bindings.a[Down]] & 0x80 )
		table.Fast2 = true;
	else if( kstate.keytable[players[1].bindings.a[Rotate]] & 0x80 && !players[1].rotated )
	{
		players[1].rotated = true;
		table.Rotate(1);
	}

	if( kstate.keytable[players[0].bindings.a[Left]] & 0x80 )
	{
		if( players[0].firststep = (players[0].movedir == 0) )
			table.Move(0, -1);

		players[0].movedir = -1;
	}
	else if( kstate.keytable[players[0].bindings.a[Right]] & 0x80 )
	{
		if( players[0].firststep = (players[0].movedir == 0) )
			table.Move(0, 1);

		players[0].movedir = 1;
	}

	if( kstate.keytable[players[1].bindings.a[Left]] & 0x80 )
	{
		if( players[1].firststep = (players[1].movedir == 0) )
			table.Move(1, -1);

		players[1].movedir = -1;
	}
	else if( kstate.keytable[players[1].bindings.a[Right]] & 0x80 )
	{
		if( players[1].firststep = (players[1].movedir == 0) )
			table.Move(1, 1);

		players[1].movedir = 1;
	}
}
//=============================================================================================================
void CoopGame::onlostdevice()
{
	quads.onlostdevice();
}
//=============================================================================================================
void CoopGame::onresetdevice()
{
	quads.onresetdevice();

	ResetGUI();
	CForm::onresetdevice();
}
//=============================================================================================================
void CoopGame::ongameover()
{
	labels[3].SetState(Idle);
	SetState(Dead);
}
//=============================================================================================================
