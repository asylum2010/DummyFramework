//=============================================================================================================
#include "versusgame.h"

#include <dummyframework/base/CHelper.h>
#include <dummyframework/base/d3d9/CGame9.h>
#include <sstream>

unsigned int examplecolors1[336] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff8a2be2, 0xff8a2be2, 0xff8a2be2, 0, 0xff8a2be2, 0xff8a2be2, 0xff8a2be2, 0xffff0000, 0xff8a2be2, 0xff8a2be2, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff8a2be2, 0xff00ff00, 0xff8a2be2, 0xff8a2be2, 0xff8a2be2, 0xffff0000, 0xffff0000, 0xff8a2be2, 0xffffa500, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff00ff00, 0xff00ff00, 0xffffff00, 0xffffff00, 0xff00ffff, 0xff00ffff, 0xff00ffff, 0xffffa500, 0xffffa500, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff00ff00, 0, 0xffffff00, 0xffffff00, 0xff00ff00, 0xff0000ff, 0xff0000ff, 0xffffff00, 0xffffff00, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff00ff00, 0, 0xffffff00, 0xffffff00, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0, 0xffffa500, 0xffffff00, 0xffffa500, 0xffffa500, 0xff00ffff, 0xff00ffff, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xffffa500, 0xffffff00, 0, 0, 0xff00ffff, 0xff00ffff, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xffffa500, 0xff0000ff, 0xffffa500, 0xffffa500, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xffffa500, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff8a2be2, 0xff8a2be2, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xffffa500, 0xffffa500, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xffff0000, 0xff8a2be2, 0xff8a2be2, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

unsigned int examplecolors2[336] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff8a2be2, 0xff8a2be2, 0xff8a2be2, 0xff8a2be2, 0xff8a2be2, 0xff8a2be2, 0xffff0000, 0xff8a2be2, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff8a2be2, 0xff8a2be2, 0xff8a2be2, 0xffff0000, 0xff8a2be2, 0xffff0000, 0, 0xff8a2be2, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff8a2be2, 0xffff0000, 0, 0xffff0000, 0xff00ffff, 0xff00ffff, 0xff00ffff, 0xff8a2be2, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xffff0000, 0xffff0000, 0xff00ff00, 0xffff0000, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff8a2be2, 0xff8a2be2, 0xffff0000, 0, 0xff00ff00, 0xff00ff00, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xffff0000, 0xffffa500, 0xffffa500, 0xffffa500, 0xff00ffff, 0xff00ffff, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xffff0000, 0, 0xffffa500, 0xffffa500, 0xffffa500, 0xff00ffff, 0xff00ffff, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff00ffff, 0xff00ffff, 0xff00ffff, 0xffffff00, 0xffffff00, 0, 0xff00ffff, 0xff00ffff, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xffffff00, 0xffffa500, 0xffffff00, 0xffffff00, 0xff8a2be2, 0xff00ffff, 0xff00ffff, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xffffff00, 0xffffa500, 0, 0xff8a2be2, 0xff8a2be2, 0, 0xff0000ff, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

VersusGame::VersusGame()
	: CForm()
{
	Atlas = NULL;
	Cell = Glow = NULL;
	SwapDelay = 309;
	
	blink.set(3, 0, 1);
	blink = blink.count() - 1;
	blink.direction = 1;

	swap.set(10, 0, 1);
	swap = 0;
	swap.direction = -1;

	action = PauseMenu;
	swapped = false;
	ticks = 0;

	for( size_t i = 0; i < 9; ++i )
		AddControl(labels[i]);

	players[0].table.gameover.connect(this, &VersusGame::ongame1over);
	players[1].table.gameover.connect(this, &VersusGame::ongame2over);
	
	Reset(true);
}
//=============================================================================================================
VersusGame::~VersusGame()
{
}
//=============================================================================================================
bool VersusGame::Initialize(DummyFramework::CGame9& mygame, DummyFramework::CSpriteFont9& font)
{
	ResetGUI();

	labels[0].Text = "Game time: 0";
	labels[1].Text = "Next swap: 0";
	labels[2].Text = "Player A";
	labels[3].Text = "Score: 0";
	labels[4].Text = "Player B";
	labels[5].Text = "Score: 0";
	labels[6].Text = "Game over";
	labels[7].Text = "Game over";
	labels[8].Text = "Player A won!\nPress any key to continue";

	CForm::Initialize(mygame, font);
	quads.Initialize(mygame.Graphics, 1500);
	
	return true;
}
//=============================================================================================================
bool VersusGame::LoadContent()
{
	CForm::LoadContent();

	players[0].table.Cell = players[1].table.Cell = Cell;
	players[0].table.Glow = players[1].table.Glow = Glow;

	for( size_t i = 0; i < 6; ++i )
		labels[i].SetState(Idle);

	return true;
}
//=============================================================================================================
void VersusGame::Draw()
{
	swap.smooth((float)game->Sync.Alpha());
	blink.smooth((float)game->Sync.Alpha());

	SingleTable& t1 = players[0].table;
	SingleTable& t2 = players[1].table;

	char i = (swapped ? 1 : 0);
	char j = 1 - i;

	size_t count = 0;
	float w = GameVariables::ScreenWidth - GameVariables::CorrelateH(276);

	w = floor(w / Cell->Size.x) * Cell->Size.x;
	w = floor(w * 0.5f + 0.5f);

	float low = w - 11 * Cell->Size.x;
	float high = w + Cell->Size.x;

	t1.X = low + swap.value * (high - low);
	t2.X = low + (1.0f - swap.value) * (high - low);
	t1.Y = t2.Y = 0;

	labels[6].Position.x = low + 6 * Cell->Size.x;
	labels[7].Position.x = high + 6 * Cell->Size.x;
	labels[8].Position.x = high;

	labels[6].Position.y = GameVariables::ScreenHeight * 0.5f;
	labels[7].Position.y = GameVariables::ScreenHeight * 0.5f;
	labels[8].Position.y = GameVariables::ScreenHeight * 0.3f;

	float s = GameVariables::CorrelateH(276);
	float r = GameVariables::ScreenWidth - s;
	float h1 = GameVariables::CorrelateH(210);
	float h2 = GameVariables::CorrelateH(420);

	quads.Lock();
	{
		count += t1.Write(count, quads, blink.value);
		count += t2.Write(count, quads, blink.value);

		count += DrawTetro(count, players[i].table.Next, D3DXVECTOR2(r + s * 0.3116f, h1));
		count += DrawTetro(count, players[j].table.Next, D3DXVECTOR2(r + s * 0.3116f, h2));
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

	mins = (size_t)((double)swaptime * game->Sync.UpdateInterval);
	secs = mins % 60;
	mins = mins / 60;

	ss.str("");
	ss << "Next swap: " << (mins < 10 ? "0" : "") <<
		mins << ":" << (secs < 10 ? "0" : "") << secs;

	labels[1].Text = ss.str();

	ss.str("");
	ss << "Score: " << players[i].table.Score;
	labels[3].Text = ss.str();

	ss.str("");
	ss << "Score: " << players[j].table.Score;
	labels[5].Text = ss.str();

	labels[0].Color.a = labels[1].Color.a = labels[2].Color.a =
		labels[3].Color.a = labels[4].Color.a = labels[5].Color.a =
		labels[6].Color.a = labels[7].Color.a = labels[8].Color.a = blink.value;

	CForm::Draw();
	spritefont->Draw(GroupID);
}
//=============================================================================================================
size_t VersusGame::DrawTetro(size_t start, const Tetromino& tetro, const D3DXVECTOR2& pos)
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
void VersusGame::Reset(bool clear)
{
	juststarted = true;
	gametime = 3009;
	swaptime = SwapDelay;

	players[0].movedir = 0;
	players[0].rotated = false;
	players[0].firststep = false;

	players[1].movedir = 0;
	players[1].rotated = false;
	players[1].firststep = false;

	players[0].table.Reset();
	players[1].table.Reset();

	blink = 0;

	if( clear )
	{
		players[0].table.Fill(examplecolors1);
		players[1].table.Fill(examplecolors2);
	}

	if( contentloaded )
	{
		labels[6].SetState(Hidden);
		labels[7].SetState(Hidden);
		labels[8].SetState(Hidden);
	}
}
//=============================================================================================================
void VersusGame::ResetGUI()
{
	labels[0].Alignment = DT_LEFT|DT_TOP;
	labels[1].Alignment = DT_LEFT|DT_TOP;
	labels[3].Alignment = DT_LEFT|DT_TOP;
	labels[5].Alignment = DT_LEFT|DT_TOP;

	labels[2].Alignment = DT_CENTER|DT_TOP;
	labels[4].Alignment = DT_CENTER|DT_TOP;
	labels[6].Alignment = DT_CENTER|DT_TOP;
	labels[7].Alignment = DT_CENTER|DT_TOP;
	labels[8].Alignment = DT_CENTER|DT_TOP;

	float pw = GameVariables::CorrelateH(276);
	float off = pw * 0.07f;

	labels[0].Position.x = GameVariables::ScreenWidth - pw + off;
	labels[0].Position.y = GameVariables::CorrelateH(20);

	labels[1].Position.x = labels[0].Position.x;
	labels[1].Position.y = GameVariables::CorrelateH(60);

	labels[2].Position.x = GameVariables::ScreenWidth - pw * 0.5f;
	labels[2].Position.y = GameVariables::CorrelateH(126);

	labels[3].Position.x = labels[0].Position.x;
	labels[3].Position.y = GameVariables::CorrelateH(166);

	labels[4].Position.x = GameVariables::ScreenWidth - pw * 0.5f;
	labels[4].Position.y = GameVariables::CorrelateH(337);

	labels[5].Position.x = labels[0].Position.x;
	labels[5].Position.y = GameVariables::CorrelateH(377);
}
//=============================================================================================================
void VersusGame::SetState(unsigned int newstate)
{
	switch( newstate )
	{
	case Idle:
		if( juststarted )
		{
			juststarted = false;

			players[0].table.Clear();
			players[1].table.Clear();
		}
		break;

	case TransitionIn:
		labels[6].SetState(Hidden);
		labels[7].SetState(Hidden);
		labels[8].SetState(Hidden);
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
void VersusGame::Update()
{
	swap.step();
	blink.step();

	switch( state )
	{
	case Idle:
		// ha folyamatosan le van nyomva a gomb akkor mozogjon
		if( !players[0].firststep && players[0].movedir != 0 )
			players[0].movedir = 0;

		if( !players[1].firststep && players[1].movedir != 0 )
			players[1].movedir = 0;

		players[0].table.Update();
		players[1].table.Update();

		--gametime;

		if( gametime <= 0 )
		{
			ongameover();
		}
		else if( swaptime <= 0 )
		{
			if( players[0].table.GetState() == SingleTable::Idle &&
				players[1].table.GetState() == SingleTable::Idle )
			{
				state = Swapping;
				swapticks = swap.count();

				if( swapped )
				{
					swap = swap.count() - 1;
					swap.direction = -1;
				}
				else
				{
					swap = 0;
					swap.direction = 1;
				}
			}
		}
		else
			--swaptime;

		break;

	case TransitionIn:
		SetState(Idle);
		focusgained(*this);
		break;

	case TransitionOut:
		focuslost(*this);
		break;
	
	case BlinkOut:
		if( blink.previous == 0 )
			Reset(true);
		break;

	case Swapping:
		if( swapticks == 0 )
		{
			myswap<unsigned int>(players[0].bindings.b, players[1].bindings.b);
			myswap<size_t>(players[0].table.Score, players[1].table.Score);

			players[0].table.Fast = false;
			players[1].table.Fast = false;

			swapped = !swapped;
			swaptime = SwapDelay;
			state = Idle;

			players[0].table.Half();
			players[1].table.Half();
		}
		else
			--swapticks;

		break;

	case Dead:
		if( ticks > 30 )
		{
			if( players[0].table.Score > players[1].table.Score )
			{
				if( swapped )
					labels[8].Text = "Player B won!\nPress any key to continue";
				else
					labels[8].Text = "Player A won!\nPress any key to continue";
			}
			else if( players[0].table.Score < players[1].table.Score )
			{
				if( swapped )
					labels[8].Text = "Player A won!\nPress any key to continue";
				else
					labels[8].Text = "Player B won!\nPress any key to continue";
			}
			else
				labels[8].Text = "Draw!\nPress any key to continue";

			labels[8].SetState(Idle);
		}

		++ticks;
		break;

	default:
		break;
	}
	
	CForm::Update();
}
//=============================================================================================================
void VersusGame::onfocusgained()
{
	CForm::onfocusgained();
	SetState(TransitionIn);
}
//=============================================================================================================
void VersusGame::onfocuslost()
{
	CForm::onfocuslost();
	SetState(Inactive);
}
//=============================================================================================================
void VersusGame::onkeyup(const DummyFramework::skeyboardstate& kstate)
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

	// swap közben ezt nem lehet mert
	// ugyanazon a timeren van, mint a menü
	// (megakadt lemez)
	if( kstate.key == VK_ESCAPE )
	{
		action = PauseMenu;
		SetState(TransitionOut);
	}
	
	if( kstate.keytable[players[0].bindings.a[Down]] == 0 )
		players[0].table.Fast = false;

	if( kstate.keytable[players[1].bindings.a[Down]] == 0 )
		players[1].table.Fast = false;

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
void VersusGame::onkeydown(const DummyFramework::skeyboardstate& kstate)
{
	if( state != Idle )
		return;

	if( kstate.keytable[players[0].bindings.a[Down]] & 0x80 )
		players[0].table.Fast = true;
	else if( kstate.keytable[players[0].bindings.a[Rotate]] & 0x80 && !players[0].rotated )
	{
		players[0].rotated = true;
		players[0].table.Rotate();
	}

	if( kstate.keytable[players[1].bindings.a[Down]] & 0x80 )
		players[1].table.Fast = true;
	else if( kstate.keytable[players[1].bindings.a[Rotate]] & 0x80 && !players[1].rotated )
	{
		players[1].rotated = true;
		players[1].table.Rotate();
	}

	if( kstate.keytable[players[0].bindings.a[Left]] & 0x80 )
	{
		if( players[0].firststep = (players[0].movedir == 0) )
			players[0].table.Move(-1);

		players[0].movedir = -1;
	}
	else if( kstate.keytable[players[0].bindings.a[Right]] & 0x80 )
	{
		if( players[0].firststep = (players[0].movedir == 0) )
			players[0].table.Move(1);

		players[0].movedir = 1;
	}

	if( kstate.keytable[players[1].bindings.a[Left]] & 0x80 )
	{
		if( players[1].firststep = (players[1].movedir == 0) )
			players[1].table.Move(-1);

		players[1].movedir = -1;
	}
	else if( kstate.keytable[players[1].bindings.a[Right]] & 0x80 )
	{
		if( players[1].firststep = (players[1].movedir == 0) )
			players[1].table.Move(1);

		players[1].movedir = 1;
	}
}
//=============================================================================================================
void VersusGame::onlostdevice()
{
	quads.onlostdevice();
}
//=============================================================================================================
void VersusGame::onresetdevice()
{
	quads.onresetdevice();

	ResetGUI();
	CForm::onresetdevice();
}
//=============================================================================================================
void VersusGame::ongameover()
{
	labels[6].SetState(Idle);
	labels[7].SetState(Idle);
	
	SetState(Dead);
}
//=============================================================================================================
void VersusGame::ongame1over()
{
	if( swapped )
		labels[7].SetState(Idle);
	else
		labels[6].SetState(Idle);

	if( players[1].table.GetState() == SingleTable::Dead )
		ongameover();
}
//=============================================================================================================
void VersusGame::ongame2over()
{
	if( swapped )
		labels[6].SetState(Idle);
	else
		labels[7].SetState(Idle);

	if( players[0].table.GetState() == SingleTable::Dead )
		ongameover();
}
//=============================================================================================================
