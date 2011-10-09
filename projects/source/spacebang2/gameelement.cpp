//=============================================================================================================
#include "gameelement.h"
#include <dummyframework/base/CHelper.h>

DummyFramework::CGameLogicSynchronizer GameVariables::Sync;
std::string GameVariables::Quality("high");
bool GameVariables::EnableShaders = true;
bool GameVariables::EnablePowerups = true;

float GameVariables::RefWidth = 1920.0f;
float GameVariables::RefHeight = 1080.0f;
float GameVariables::ScreenWidth = 0;
float GameVariables::ScreenHeight = 0;
float GameVariables::PlayFieldHeight = 0;

unsigned short GameVariables::EnemiesKilled;
unsigned short GameVariables::Deaths;
unsigned short GameVariables::PowerupsAcquired;
unsigned long GameVariables::Score;
unsigned long GameVariables::TotalScore;

std::string& GameVariables::Qualify(std::string& out, const std::string& str)
{
    std::string path, name, ext;

    DummyFramework::CHelper::GetPath(path, str);
    DummyFramework::CHelper::GetName(name, str);
    DummyFramework::CHelper::GetExtension(ext, str);
    
    if( ext.length() == 0 )
        out = path + Quality + "/" + name + "_" + Quality;
    else
        out = path + Quality + "/" + name + "_" + Quality + "." + ext;

    return out;
}
//=============================================================================================================
