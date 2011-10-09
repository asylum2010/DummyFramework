//=============================================================================================================
#ifndef _TETROMINO_H_
#define _TETROMINO_H_

#include <string>

class Tetromino
{
protected:
	std::string shape[4];
    size_t orientation;

public:
	int X, Y;
    unsigned long Color;

    Tetromino();
	virtual ~Tetromino() {}

	Tetromino& operator =(const Tetromino& other);

    inline void Rotate90() {
		orientation = (orientation + 1) % 4;
	}

    inline void Rotate270() {
		orientation = (orientation + 3) % 4;
	}

    inline void Rotate(int n) {
		orientation = n % 4;
	}

	inline const std::string& GetOrientedShape() const {
		return shape[orientation];
	}
};
//*************************************************************************************************************
class TetroS : public Tetromino
{
public:
    TetroS()
    {
        shape[0] = "0100011000100000";
        shape[1] = "0000001101100000";
        shape[2] = "0100011000100000";
        shape[3] = "0000001101100000";

        Color = 0xff00ff00;
    }
};
//*************************************************************************************************************
class TetroZ : public Tetromino
{
public:
    TetroZ()
    {
        shape[0] = "0010011001000000";
        shape[1] = "0000110001100000";
        shape[2] = "0010011001000000";
        shape[3] = "0000110001100000";

        Color = 0xffff0000;
    }
};
//*************************************************************************************************************
class TetroI : public Tetromino
{
public:
    TetroI()
    {
        shape[0] = "0000111100000000";
        shape[1] = "0100010001000100";
        shape[2] = "0000111100000000";
        shape[3] = "0100010001000100";

        Color = 0xff00ffff;
    }
};
//*************************************************************************************************************
class TetroL : public Tetromino
{
public:
	TetroL()
    {
        shape[0] = "0100010001100000";
        shape[1] = "0000011101000000";
        shape[2] = "0110001000100000";
        shape[3] = "0000001011100000";

        Color = 0xffffa500;
    }
};
//*************************************************************************************************************
class TetroJ : public Tetromino
{
public:
	TetroJ()
    {
        shape[0] = "0010001001100000";
        shape[1] = "0000010001110000";
        shape[2] = "0110010001000000";
        shape[3] = "0000011100010000";

        Color = 0xff0000ff;
    }
};
//*************************************************************************************************************
class TetroO : public Tetromino
{
public:
	TetroO()
    {
        shape[0] = "0000011001100000";
        shape[1] = "0000011001100000";
        shape[2] = "0000011001100000";
        shape[3] = "0000011001100000";

        Color = 0xffffff00;
    }
};
//*************************************************************************************************************
class TetroT : public Tetromino
{
public:
	TetroT()
    {
        shape[0] = "0000010011100000";
        shape[1] = "0000010001100100";
        shape[2] = "0000111001000000";
        shape[3] = "0000001001100010";

        Color = 0xff8a2be2;
    }
};
//*************************************************************************************************************
class TetroX : public Tetromino
{
public:
	TetroX()
    {
        shape[0] = "0000010011100100";
        shape[1] = "0000010011100100";
        shape[2] = "0000010011100100";
        shape[3] = "0000010011100100";

        Color = 0xffff9bf5;
    }
};

#endif
//=============================================================================================================
