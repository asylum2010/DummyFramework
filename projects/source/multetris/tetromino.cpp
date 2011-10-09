//=============================================================================================================
#include "tetromino.h"

Tetromino::Tetromino()
{
    X = Y = 0;
    Color = 0xffffffff;
    orientation = 0;

    shape[0] = "0000000000000000";
    shape[1] = "0000000000000000";
    shape[2] = "0000000000000000";
    shape[3] = "0000000000000000";
}
//=============================================================================================================
Tetromino& Tetromino::operator =(const Tetromino& other)
{
	if( &other != this )
	{
		X = other.X;
		Y = other.Y;
		Color = other.Color;
		orientation = other.orientation;

		shape[0] = other.shape[0];
		shape[1] = other.shape[1];
		shape[2] = other.shape[2];
		shape[3] = other.shape[3];
	}

	return *this;
}
//=============================================================================================================
