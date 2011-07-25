//=============================================================================================================
#include <dummyframework/base/d3d9/CEffectRenderer9.h>
#include <dummyframework/base/d3d9/CGame9.h>
#include <dummyframework/template/defines.h>

namespace DummyFramework
{
    CEffectRenderer9::CEffectRenderer9()
    {
        game = NULL;
        manager = NULL;
		canbeapplied = false;

        Enabled = true;
        MediaDir = "./";
    }
    //=============================================================================================================
    CEffectRenderer9::~CEffectRenderer9()
    {
    }
    //=============================================================================================================
    bool CEffectRenderer9::Initialize(CGame9& mygame)
    {
        game = &mygame;
		manager = mygame.Content.GetStateManager();

        return true;
    }
    //=============================================================================================================
	bool CEffectRenderer9::ResetRenderStates()
	{
		return true;
	}
    //=============================================================================================================
    bool CEffectRenderer9::LoadContent()
	{
		canbeapplied = true;
		return true;
	}
    //=============================================================================================================
	void CEffectRenderer9::UnloadContent()
	{
	}
    //=============================================================================================================
    void CEffectRenderer9::onlostdevice()
    {
    }
    //=============================================================================================================
    void CEffectRenderer9::onresetdevice()
    {
    }
    //=============================================================================================================
}
