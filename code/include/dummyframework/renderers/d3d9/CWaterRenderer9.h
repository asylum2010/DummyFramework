//=============================================================================================================
#ifndef _CWATERRENDERER9_H_
#define _CWATERRENDERER9_H_

#include "../../base/d3d9/CEffectRenderer9.h"
#include <d3dx9.h>

namespace DummyFramework
{
	class CWaterRenderer9 : public CEffectRenderer9
	{
	private:
		LPDIRECT3DSURFACE9 oldsurface;
		LPDIRECT3DSURFACE9 reflectsurface;
		LPDIRECT3DSURFACE9 refractsurface;

		D3DXMATRIX		view, projection;
		D3DXVECTOR3		eye, look, up;
		D3DXPLANE		plane;
		unsigned int	frequency;
		unsigned int	id1;
		unsigned int	id2;

	public:
		LPDIRECT3DTEXTURE9 ReflectionTexture;
		LPDIRECT3DTEXTURE9 RefractionTexture;

		D3DXMATRIX		View;
		D3DXMATRIX		Projection;
		D3DXVECTOR3		Eye;
		unsigned int	Frequency;
		bool			Clip;

		CWaterRenderer9();
		~CWaterRenderer9() {}

		bool LoadContent();

		void UnloadContent();
		void Draw();

		inline void SetViewMatrix(const D3DXMATRIX& view) {
			this->view = view;
		}

		inline void SetProjectionMatrix(const D3DXMATRIX& proj) {
			this->projection = proj;
		}

		inline void SetEyePosition(const D3DXVECTOR3& eye) {
			this->eye = eye;
		}

		inline void SetClipPlane(const D3DXPLANE& plane) {
			this->plane = plane;
		}

		inline LPDIRECT3DSURFACE9 GetRefractionSurface() {
			return refractsurface;
		}

	_DUMMY_EVENTS:
		signal0 renderreflection;

	_DUMMY_EVENTHANDLERS:
		void onlostdevice();
		void onresetdevice();
	};
}

#endif
//=============================================================================================================
 
 
