//=============================================================================================================
#ifndef _GAMEELEMENT_H_
#define _GAMEELEMENT_H_

#include <dummyframework/template/defines.h>
#include <dummyframework/base/d3d9/CGame9.h>

#define MENU_TRANSITION 4

typedef DummyFramework::point2<unsigned int> point2;
typedef DummyFramework::syncedlinear<D3DXVECTOR2> syncedvector;
typedef DummyFramework::syncedlinear<float> syncedfloat;
typedef DummyFramework::CDynamicQuadBuffer9 quadbuffer;
typedef DummyFramework::CContentManager9::textureatlas textureatlas;

/**
 * \brief Base class for game entities
 */
class GameElement
{
protected:
	DummyFramework::CGame9* game;

public:
	GameElement()
		: game(0)
	{
	}

	virtual ~GameElement()
	{
	}

	//! Initialization
	virtual void Initialize(DummyFramework::CGame9& mygame) {
		game = &mygame;
	}

	//! Rendering
	virtual size_t Write(size_t start, quadbuffer& quad) {
		return 0;
	}
};

/**
 * \brief Global game variables
 */
class GameVariables
{
private:
	GameVariables() {}

public:
	static DummyFramework::CGameLogicSynchronizer Sync;
	static std::string Quality;
	static bool EnableShaders;
	static bool EnablePowerups;

	static float RefWidth;			/*!< Width reference for game graphics */
	static float RefHeight;			/*!< Height reference for game graphics */
	static float ScreenWidth;
	static float ScreenHeight;
	static float PlayFieldHeight;	/*!< Screen height without animation strips */

	static unsigned short EnemiesKilled;
	static unsigned short Deaths;
	static unsigned short PowerupsAcquired;
	static unsigned int Score;
	static unsigned int TotalScore;

	//! Scale quantity to screen width
	inline static float CorrelateW(float value) {
		return (value * ScreenWidth) / RefWidth;
	}

	//! Scale quantity to screen height
	inline static float CorrelateH(float value) {
		return (value * ScreenHeight) / RefHeight;
	}

	//! Returns the interpolation value
	inline static float GetAlpha() {
		return (float)Sync.Alpha();
	}

	//! Returns the time for the text to be visible
	inline static size_t ReadLength(const std::string& str) {
		return (size_t)(((float)str.length() * 0.7f) + 10);
	}

	//! Qualifies a file name with the graphics quality
	static std::string& Qualify(std::string& out, const std::string& str);
};

#endif
//=============================================================================================================

