//=============================================================================================================
#include "multitable.h"

MultiTable::MultiTable()
{
	Cell = Glow = 0;

	X = Y = 0;
	Width = 19;
	Height = 25;
	HangOut = 4;

	drawtable = new tetriscell*[Width];
	tetrominos.resize(7);

	tetrominos[0] = new TetroS();
	tetrominos[1] = new TetroZ();
	tetrominos[2] = new TetroL();
	tetrominos[3] = new TetroI();
	tetrominos[4] = new TetroO();
	tetrominos[5] = new TetroJ();
	tetrominos[6] = new TetroT();

	for( int i = 0; i < Width; ++i )
		drawtable[i] = new tetriscell[Height];
}
//=============================================================================================================
MultiTable::~MultiTable()
{
	for( int i = 0; i < 7; ++i )
		safe_delete(tetrominos[i]);

	for( int i = 0; i < Width; ++i )
		safe_delete(drawtable[i]);

	safe_delete(drawtable);
}
//=============================================================================================================
bool MultiTable::Collide(const Tetromino& tetro)
{
	int m, n;
	const std::string& shape = tetro.GetOrientedShape();

	for( int i = 0; i < 4; ++i )
	{
		for( int j = 0; j < 4; ++j )
		{
			m = tetro.X + i;
			n = tetro.Y + j;

			if( m >= 0 && m < Width && n < Height )
			{
				if( drawtable[m][n].mask && (shape[i + 4 * j] == '1') )
					return true;
			}
		}
	}

	return false;
}
//=============================================================================================================
void MultiTable::Clear()
{
	state = Clearing;
	flashticks = 0;

	MarkToDelete(0, Height - 1);
}
//=============================================================================================================
void MultiTable::DrawTetro(const Tetromino& tetro)
{
	const std::string& shape = tetro.GetOrientedShape();

	int sx = max(tetro.X, 1);
	int ex = min(tetro.X + 4, Width - 1);
	int ey = min(tetro.Y + 4, Height);
	int m, n, ind;

	for( m = sx; m < ex; ++m )
	{
		for( n = tetro.Y; n < ey; ++n )
		{
			ind = (m - tetro.X) + 4 * (n - tetro.Y);

			if( shape[ind] == '1' )
			{
				++drawtable[m][n].visible;
				drawtable[m][n].color = ((drawtable[m][n].visible > 1) ? 0xffff9bf5 : tetro.Color);
			}
		}
	}
}
//=============================================================================================================
void MultiTable::EraseTetro(const Tetromino& tetro)
{
	const std::string& shape = tetro.GetOrientedShape();

	int sx = max(tetro.X, 1);
	int ex = min(tetro.X + 4, Width - 1);
	int ey = min(tetro.Y + 4, Height);
	int m, n, ind;

	for( m = sx; m < ex; ++m )
	{
		for( n = tetro.Y; n < ey; ++n )
		{
			ind = (m - tetro.X) + 4 * (n - tetro.Y);

			if( shape[ind] == '1' )
			{
				drawtable[m][n].visible = max(drawtable[m][n].visible - 1, 0);

				if( tetro.Color == current[0].Color )
					drawtable[m][n].color = current[1].Color;
				else
					drawtable[m][n].color = current[0].Color;
			}
		}
	}
}
//=============================================================================================================
void MultiTable::Glue(const Tetromino& tetro)
{
	const std::string& shape = tetro.GetOrientedShape();
	int m, n;

	for( int i = 0; i < 4; ++i )
	{
		for( int j = 0; j < 4; ++j )
		{
			m = tetro.X + i;
			n = tetro.Y + j;

			if( m >= 0 && m < Width && n < Height )
			{
				if( shape[i + 4 * j] == '1' )
				{
					++drawtable[m][n].visible;

					drawtable[m][n].mask = true;
					drawtable[m][n].color =	((drawtable[m][n].visible > 1) ? 0xffff9bf5 : tetro.Color);
				}
			}
		}
	}
}
//=============================================================================================================
void MultiTable::MarkToDelete(int start, int end)
{
	todelete.clear();

	for( int i = start; i < end; ++i )
	{
		for( int j = 1; j < Width - 1; ++j )
		{
			if( drawtable[j][i].mask )
			{
				todelete.insert(i);
				break;
			}
		}
	}
}
//=============================================================================================================
void MultiTable::FindToDelete(int start, int end)
{
	todelete.clear();
	bool filled;

	for( int i = start; i < end; ++i )
	{
		if( i >= Height - 1 )
			continue;

		filled = true;

		for( int j = 1; j < Width - 1; ++j )
		{
			if( !drawtable[j][i].mask )
			{
				filled = false;
				break;
			}
		}

		if( filled )
			todelete.insert(i);
	}
}
//=============================================================================================================
void MultiTable::Move(char player, char direction)
{
	if( state != Idle )
		return;

	Tetromino& t = current[player];

	EraseTetro(t);
	t.X += direction;

	if( Collide(t) )
		t.X -= direction;

	DrawTetro(t);
}
//=============================================================================================================
void MultiTable::Fill(unsigned int colors[])
{
	int ind;

	for( int i = 0; i < Width; ++i )
	{
		for( int j = 0; j < Height; ++j )
		{
			ind = i * Height + j;
			
			if( colors[ind] != 0 )
			{
				drawtable[i][j].mask = true;
				drawtable[i][j].visible = true;
				drawtable[i][j].color = colors[ind];
			}
		}
	}
}
//=============================================================================================================
void MultiTable::Reset()
{
	for( int i = 0; i < Width; ++i )
	{
		for( int j = 0; j < Height; ++j )
		{
			drawtable[i][j].mask = (i == 0 || (i == Width - 1) || (j == Height - 1));

			if( j >= HangOut && drawtable[i][j].mask )
			{
				drawtable[i][j].visible = 1;
				drawtable[i][j].color = 0xffffdab9;
			}
			else
				drawtable[i][j].visible = 0;
		}
	}

	Fast1 = false;
	Fast2 = false;
	Score = 0;

	flash = false;
	state = Dead;
	ticks = 0;

	Next1 = *tetrominos[rand() % tetrominos.size()];
	Next1.Rotate(rand());

	Next2 = *tetrominos[rand() % tetrominos.size()];
	Next2.Rotate(rand());

	current[0].X = -1;
	current[1].X = -1;
}
//=============================================================================================================
void MultiTable::Rotate(char player)
{
	if( state != Idle )
		return;

	Tetromino& t = current[player];

	EraseTetro(t);
	t.Rotate90();

	if( Collide(t) )
		t.Rotate270();

	DrawTetro(t);
}
//=============================================================================================================
void MultiTable::Step(char player)
{
	Tetromino& n = (player == 0 ? Next1 : Next2);
	Tetromino& t = current[player];

	t = n;
	t.Y = 0;

	if( player == 0 )
		t.X = Width / 3 - 2;
	else
		t.X = 2 * Width / 3 - 2;
		
	n = *tetrominos[rand() % tetrominos.size()];
	n.Rotate(rand());

	// ha az elsö vagy második lépésnél ütközik, akkor vége
	if( Collide(t) )
	{
		state = Dead;
		gameover();
	}
	else
	{
		t.Y = 1;

		if( Collide(t) )
		{
			t.Y = 0;
			Glue(t);

			state = Dead;
			gameover();
		}
		else
		{
			t.Y = 0;
			DrawTetro(t);
		}
	}
}
//=============================================================================================================
void MultiTable::CheckCollision()
{
	Tetromino& t1 = current[0];
	Tetromino& t2 = current[1];

	bool b1 = false, b2 = false;
	bool f1 = true, f2 = true;
	bool d1 = true, d2 = true;
	int s = -1;

	if( !Fast1 && (ticks != 1) )
		f1 = false;
	else
		EraseTetro(t1);

	if( !Fast2 && (ticks != 1) )
		f2 = false;
	else
		EraseTetro(t2);

	// elsö kör
	if( f1 )
	{
		++t1.Y;

		if( b1 = Collide(t1) )
		{
			--t1.Y;
			Glue(t1);
			t1.X = -1;

			s = t1.Y - 1;
		}
	}

	if( f2 )
	{
		++t2.Y;

		if( b2 = Collide(t2) )
		{
			--t2.Y;
			Glue(t2);
			t2.X = -1;

			s = t2.Y - 1;
		}
	}

	if( b1 && b2 )
		s = min(t1.Y, t2.Y);

	// második kör
	if( f1 )
	{
		// egyik nem ütközött, de a másik igen -> lehet hogy fedik egymást
		if( !b1 && b2 )
		{
			if( Collide(t1) )
			{
				--t1.Y;
				Glue(t1);
				t1.X = -1;

				s = min(t1.Y, t2.Y);
				d1 = false;
			}
		}
	}

	if( f2 )
	{
		// egyik nem ütközött, de a másik igen -> lehet hogy fedik egymást
		if( !b2 && b1 )
		{
			if( Collide(t2) )
			{
				--t2.Y;
				Glue(t2);
				t2.X = -1;

				s = min(t1.Y, t2.Y);
				d2 = false;
			}
		}
	}

	// ha volt ütközés
	if( s != -1 )
	{
		FindToDelete(s, Height);

		// ha törölni kell
		if( todelete.size() > 0 )
		{
			flash = false;
			state = Collapsing;

			flashticks = 0;
			ticks = 0;

			if( f1 && d1 && t1.X != -1 )
				DrawTetro(t1);

			if( f2 && d2 && t2.X != -1 )
				DrawTetro(t2);
		}
		else
		{
			if( t1.X == -1 )
				Step(0);
			else if( f1 && d1 )
				DrawTetro(t1);

			if( t2.X == -1 )
				Step(1);
			else if( f2 && d2 )
				DrawTetro(t2);
		}
	}
	else
	{
		if( f1 && d1 )
			DrawTetro(t1);

		if( f2 && d2 )
			DrawTetro(t2);
	}
}
//=============================================================================================================
void MultiTable::Update()
{
	ticks = (ticks + 1) % 4;

	switch( state )
	{
	case Idle:
		CheckCollision();
		break;

	case Clearing:
	case Collapsing:
		if( flashticks >= 24 )
		{
			// le kell törölni, mert különben a Collapse() törli
			if( current[0].X != -1 )
				EraseTetro(current[0]);

			if( current[1].X != -1 )
				EraseTetro(current[1]);

			size_t score = Collapse();

			if( state == Collapsing )
				Score += score;

			state = Idle;
			flash = false;

			// ha beragadt akkor lépünk
			if( current[0].X == -1 )
				Step(0);
			// ha nem, akkor ujra kell rajzolni, mert ez nem ütközött
			else
				DrawTetro(current[0]);

			// ha beragadt akkor lépünk
			if( current[1].X == -1 )
				Step(1);
			// ha nem, akkor ujra kell rajzolni, mert ez nem ütközött
			else
				DrawTetro(current[1]);
		}
		else
		{
			if( flashticks % 3 == 0 )
				flash = !flash;

			++flashticks;
		}

		break;

	default:
		break;
	}
}
//=============================================================================================================
size_t MultiTable::Collapse()
{
	size_t ret = todelete.size();
	int i, j, k;
	bool empty;

	while( todelete.size() > 0 )
	{
		i = *(todelete.rbegin());
		todelete.erase(i);

		j = i - 1;

		while( todelete.count(j) > 0 )
			--j;

		empty = true;

		for( k = 1; k < Width - 1; ++k )
		{
			if( drawtable[k][i].mask = drawtable[k][j].mask )
			{
				empty = false;
				drawtable[k][i].color = drawtable[k][j].color;
				drawtable[k][i].visible = drawtable[k][j].visible;
			}
			else
				drawtable[k][i].visible = 0;
		}

		if( !empty )
			todelete.insert(j);
	}

	return ret;
}
//=============================================================================================================
size_t MultiTable::Write(size_t start, DummyFramework::CDynamicQuadBuffer9& quads, float alpha)
{
	size_t written = 0;
	int i, j;

	D3DXVECTOR2 pos;
	D3DXCOLOR color;
	D3DXCOLOR white(0xffffffff);

	for( i = 0; i < Width; ++i )
	{
		for( j = 0; j < Height; ++j )
		{
			if( drawtable[i][j].visible )
			{
				pos.x = X + i * Cell->Size.x;
				pos.y = Y + j * Cell->Size.y;

				color = drawtable[i][j].color;

				if( flash && todelete.count(j) > 0 && drawtable[i][j].mask )
				{
					if( i > 0 && i < Width - 1 )
						color = 0xffffffff;
				}

				white.a = color.a = alpha;

				quads.WriteQuad(start + written, pos, Cell->Size, Cell->Texcoords, color);
				quads.WriteQuad(start + written + 1, pos, Glow->Size, Glow->Texcoords, white);

				written += 2;
			}
		}
	}

	return written;
}
//=============================================================================================================
