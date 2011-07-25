//=============================================================================================================
#ifndef _SINGLETABLE_H_
#define _SINGLETABLE_H_

#include <dummyframework/base/d3d9/CScreenQuad9.h>
#include <dummyframework/base/d3d9/CSprite9.h>
#include <dummyframework/template/defines.h>
#include <dummyframework/template/signal.hpp>
#include <vector>

#include "tetromino.h"

class SingleTable
{
	struct tetriscell
    {
        unsigned long color;
		bool visible;
		bool mask;
    };

private:
	typedef std::vector<Tetromino*> tetrolist;
	typedef std::set<int> rowset;

	Tetromino      current;
	tetrolist      tetrominos;
	rowset         todelete;
	tetriscell**   drawtable;
	unsigned long  state;
	int            flashticks;
	int            ticks;
	bool           flash;

	void EraseTetro(const Tetromino& tetro);
	void DrawTetro(const Tetromino& tetro);
	void Glue(const Tetromino& tetro);
	void MarkToDelete(int start, int end);
	void FindToDelete(int start, int end);
	void Step();

public:
	enum tablestate
	{
		Idle,
		Clearing,
		Collapsing,
		Dead
	};

	DummyFramework::CSprite9* Cell;
	DummyFramework::CSprite9* Glow;

	Tetromino Next;
	size_t Score;
	bool Fast;
    

	int Width, Height, HangOut;
	float X, Y;
	
    SingleTable();
	~SingleTable();

	bool Collide(const Tetromino& tetro);

	void Clear();
	void Fill(unsigned long colors[]);
	void Half();
	void Move(char direction);
	void Reset();
	void Rotate();
	void Update();

	size_t Collapse();
	size_t Write(size_t start, DummyFramework::CDynamicQuadBuffer9& quads, float alpha);

	inline unsigned long GetState() const {
		return state;
	}
	
events:
	DummyFramework::signal0 gameover;
};

#endif
//=============================================================================================================
