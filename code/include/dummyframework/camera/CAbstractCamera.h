//=============================================================================================================
#ifndef _CABSTRACTCAMERA_H_
#define _CABSTRACTCAMERA_H_

#include "../template/defines.h"

#if _DUMMY_DX == 9
#	include <d3dx9.h>
#elif _DUMMY_DX == 10
#	include <d3dx10.h>
#endif

namespace DummyFramework
{
	class CAbstractCamera
	{
	protected:
		D3DXVECTOR3		position;
		D3DXQUATERNION	orientation;
		float			distance;
		float			clipnear, clipfar;
		float			fov, aspect;

	public:
		unsigned char Button;

		CAbstractCamera();
		virtual ~CAbstractCamera() {}

		inline D3DXMATRIX& GetProjectionMatrix(D3DXMATRIX& out) {
			D3DXMatrixPerspectiveFovLH(&out, fov, aspect, clipnear, clipfar);
			return out;
		}

		D3DXVECTOR3& GetPosition(D3DXVECTOR3& out);
		D3DXVECTOR3& Forward(D3DXVECTOR3& out);
		D3DXVECTOR3& Right(D3DXVECTOR3& out);
		D3DXVECTOR3& Up(D3DXVECTOR3& out);

		virtual D3DXMATRIX& GetViewMatrix(D3DXMATRIX& out) = 0;
		virtual D3DXVECTOR3& GetEyePosition(D3DXVECTOR3& out) = 0;

		virtual void SetPosition(const D3DXVECTOR3& pos);
		virtual void SetPosition(float x, float y, float z);
		virtual void SetOrientation(const D3DXVECTOR3& angles) = 0;
		virtual void SetOrientation(float x, float y, float z) = 0;

		inline void SetAspect(float value) {
			aspect = value;
		}

		inline void SetClip(float cnear, float cfar) {
			clipnear = cnear;
			clipfar = cfar;
		}

		inline void SetDistance(float value) {
			distance = value;
		}

		inline void SetFov(float value) {
			fov = value;
		}

		inline float GetDistance() const {
			return distance;
		}

		inline D3DXVECTOR2& GetClip(D3DXVECTOR2& out) const {
			out.x = clipnear;
			out.y = clipfar;
			return out;
		}

		inline D3DXQUATERNION& GetOrientation(D3DXQUATERNION& out) const {
			out = orientation;
			return out;
		}
	};

	inline D3DXVECTOR3& CAbstractCamera::GetPosition(D3DXVECTOR3& out) {
		out = position;
		return out;
	}

	inline void CAbstractCamera::SetPosition(const D3DXVECTOR3& pos) {
		position = pos;
	}

	inline void CAbstractCamera::SetPosition(float x, float y, float z) {
		position.x = x;
		position.y = y;
		position.z = z;
	}
}

#endif
//=============================================================================================================
