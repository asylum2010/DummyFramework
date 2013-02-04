//=============================================================================================================
#ifndef _CMOTIONBLURRENDERER9_H_
#define _CMOTIONBLURRENDERER9_H_

#include "../../base/d3d9/CEffectRenderer9.h"
#include <d3dx9.h>

namespace DummyFramework
{
	class CMotionBlurRenderer9 : public CEffectRenderer9
	{
	private:
		LPD3DXEFFECT effect;

	public:
		CMotionBlurRenderer9();
		~CMotionBlurRenderer9() {}

		bool Initialize(CGame9& mygame);
		bool LoadContent();

		void UnloadContent();
		void Draw();

		inline LPD3DXEFFECT GetEffect() {
			return effect;
		}

	_DUMMY_EVENTHANDLERS:
		void onlostdevice();
		void onresetdevice();
	};
}

#endif
//=============================================================================================================
