//=============================================================================================================
#ifndef _TRANSFORMABLE_H_
#define _TRANSFORMABLE_H_

#include "../template/defines.h"

#if _DUMMY_DX == 9
#	include <d3dx9.h>
#elif _DUMMY_DX == 10
#	include <d3dx10.h>
#endif

namespace DummyFramework
{
	/**
	 * \brief Represents a transformable entity
	 */
	class CTransformable
	{
	protected:
		D3DXVECTOR3			scaling;
		D3DXVECTOR3			translation;
		D3DXVECTOR3			scalingcenter;
		D3DXVECTOR3			rotationcenter;
		D3DXVECTOR3			bmmin;
		D3DXVECTOR3			bbmax;
		D3DXQUATERNION		rotation;
		D3DXMATRIX			transform;
		D3DXMATRIX			invtransform;
		bool				needupdate;

	public:
		D3DXMATRIX UserTransform;

		CTransformable();
		virtual ~CTransformable() {};

		D3DXMATRIX& GetTransform();
		D3DXMATRIX& GetInvTransform();

		inline const D3DXQUATERNION& GetRotation() const {
			return rotation;
		}

		inline const D3DXVECTOR3 GetTranslation() const {
			return translation;
		}

		inline void Rotate(const D3DXQUATERNION& rotate) {
			rotation = rotate;
			needupdate = true;
		}

		inline void RotationCenter(const D3DXVECTOR3& rc) {
			rotationcenter = rc;
			needupdate = true;
		}

		inline void Scale(float x, float y, float z) {
			scaling.x = x;
			scaling.y = y;
			scaling.z = z;
			needupdate = true;
		}

		inline void Scale(const D3DXVECTOR3& scale) {
			scaling = scale;
			needupdate = true;
		}

		inline void Translate(float x, float y, float z) {
			translation.x = x;
			translation.y = y;
			translation.z = z;
			needupdate = true;
		}

		inline void Translate(const D3DXVECTOR3& translate) {
			translation = translate;
			needupdate = true;
		}
	};
}

#endif
//=============================================================================================================
