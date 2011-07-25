//=============================================================================================================
#include <dummyframework/renderers/d3d9/CMotionBlurRenderer9.h>

namespace DummyFramework
{
	CMotionBlurRenderer9::CMotionBlurRenderer9()
	{
	}
	//=============================================================================================================
	bool CMotionBlurRenderer9::Initialize(CGame9& mygame)
	{
		CEffectRenderer9::Initialize(mygame);
		return true;
	}
	//=============================================================================================================
	bool CMotionBlurRenderer9::LoadContent()
	{
		return CEffectRenderer9::LoadContent();
	}
	//=============================================================================================================
	void CMotionBlurRenderer9::UnloadContent()
	{
		CEffectRenderer9::UnloadContent();
	}
	//=============================================================================================================
	void CMotionBlurRenderer9::Draw()
	{
	}
	//=============================================================================================================
	void CMotionBlurRenderer9::onlostdevice()
	{
	}
	//=============================================================================================================
    void CMotionBlurRenderer9::onresetdevice()
	{
	}
	//=============================================================================================================
}
