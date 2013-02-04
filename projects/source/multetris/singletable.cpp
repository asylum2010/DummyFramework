//=============================================================================================================
#include "singletable.h"

SingleTable::SingleTable()
{
	Cell = Glow = 0;

	X = Y = 0;
	Width = 12;
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
SingleTable::~SingleTable()
{
	for( int i = 0; i < 7; ++i )
		safe_delete(tetrominos[i]);

	for( int i = 0; i < Width; ++i )
		safe_delete(drawtable[i]);

	safe_delete(drawtable);
}
//=============================================================================================================
bool SingleTable::Collide(const Tetromino& tetro)
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
void SingleTable::Clear()
{
	state = Clearing;
	flashticks = 0;

	MarkToDelete(0, Height - 1);
}
//=============================================================================================================
void SingleTable::DrawTetro(const Tetromino& tetro)
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
				drawtable[m][n].visible = true;
				drawtable[m][n].color = tetro.Color;
			}
		}
	}
}
//=============================================================================================================
void SingleTable::EraseTetro(const Tetromino& tetro)
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
				drawtable[m][n].visible = false;
		}
	}
}
//=============================================================================================================
void SingleTable::Glue(const Tetromino& tetro)
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
					drawtable[m][n].color =	tetro.Color;
					drawtable[m][n].mask = true;
					drawtable[m][n].visible = true;
				}
			}
		}
	}
}
//=============================================================================================================
void SingleTable::Half()
{
	int h = 0;

	while( h < Height - 1 )
	{
		for( int j = 1; j < Width - 1; ++j )
		{
			if( drawtable[j][h].mask )
				goto _found;
		}

		++h;
	}

_found:
	if( h < Height / 2 )
	{
		state = Clearing;
		flashticks = 0;

		MarkToDelete(Height / 2, Height - 1);
	}
}
//=============================================================================================================
void SingleTable::MarkToDelete(int start, int end)
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
void SingleTable::FindToDelete(int start, int end)
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
void SingleTable::Move(char direction)
{
	if( state != Idle )
		return;

	EraseTetro(current);
	current.X += direction;

	if( Collide(current) )
		current.X -= direction;

	DrawTetro(current);
}
//=============================================================================================================
void SingleTable::Fill(unsigned int colors[])
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
void SingleTable::Reset()
{
	for( int i = 0; i < Width; ++i )
	{
		for( int j = 0; j < Height; ++j )
		{
			drawtable[i][j].mask = (i == 0 || (i == Width - 1) || (j == Height - 1));

			if( j >= HangOut && drawtable[i][j].mask )
			{
				drawtable[i][j].visible = true;
				drawtable[i][j].color = 0xffffdab9;
			}
			else
				drawtable[i][j].visible = false;
		}
	}

	Fast = false;
	flash = false;
	Score = 0;

	state = Dead;
	ticks = 0;

	Next = *tetrominos[rand() % tetrominos.size()];
	Next.Rotate(rand());

	current.X = -1;
}
//=============================================================================================================
void SingleTable::Rotate()
{
	if( state != Idle )
		return;

	EraseTetro(current);
	current.Rotate90();

	if( Collide(current) )
		current.Rotate270();

	DrawTetro(current);
}
//=============================================================================================================
void SingleTable::Step()
{
	current = Next;

	current.X = Width / 2 - 2;
	current.Y = 0;

	Next = *tetrominos[rand() % tetrominos.size()];
	Next.Rotate(rand());

	// ha az elsö vagy második lépésnél ütközik, akkor vége
	if( Collide(current) )
	{
		state = Dead;
		gameover();
	}
	else
	{
		current.Y = 1;

		if( Collide(current) )
		{
			current.Y = 0;
			Glue(current);

			state = Dead;
			gameover();
		}
		else
		{
			current.Y = 0;
			DrawTetro(current);
		}
	}
}
//=============================================================================================================
void SingleTable::Update()
{
	ticks = (ticks + 1) % 4;

	switch( state )
	{
	case Idle:
		if( !Fast && (ticks != 1) )
			return;
		
		EraseTetro(current);
		++current.Y;

		if( Collide(current) )
		{
			--current.Y;

			Glue(current);
			FindToDelete(current.Y, Height);
			current.X = -1;

			// ha törölni kell
			if( todelete.size() > 0 )
			{
				flash = false;
				state = Collapsing;

				flashticks = 0;
				ticks = 0;
			}
			else
				Step();
		}
		else
			DrawTetro(current);

		break;

	case Clearing:
	case Collapsing:
		if( flashticks >= 24 )
		{
			size_t score = Collapse();

			if( state == Collapsing )
				Score += score;

			state = Idle;
			flash = false;

			// ha beragadt akkor lépünk
			if( current.X == -1 )
				Step();
			// ha nem, akkor ujra kell rajzolni, mert felezés volt
			else
				DrawTetro(current);
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
size_t SingleTable::Collapse()
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
size_t SingleTable::Write(size_t start, DummyFramework::CDynamicQuadBuffer9& quads, float alpha)
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
