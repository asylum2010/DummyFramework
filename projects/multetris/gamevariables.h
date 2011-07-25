//=============================================================================================================
#ifndef _GAMEVARIABLES_H_
#define _GAMEVARIABLES_H_

#define MENU_TRANSITION 4

/**
 * \brief Global game variables
 */
class GameVariables
{
private:
    GameVariables() {}

public:
    static float RefWidth;          /*!< Width reference for game graphics */
    static float RefHeight;         /*!< Height reference for game graphics */
    static float ScreenWidth;
    static float ScreenHeight;
	
	//! Scale quantity to screen width
    inline static float CorrelateW(float value) {
		return (value * ScreenWidth) / RefWidth;
	}

	//! Scale quantity to screen height
    inline static float CorrelateH(float value) {
		return (value * ScreenHeight) / RefHeight;
	}
};

#endif
//=============================================================================================================

