//=============================================================================================================
#include <dummyframework/camera/CArcballCamera.h>
#include <dummyframework/base/CHelper.h>
#include <cmath>

namespace DummyFramework
{
	CArcballCamera::CArcballCamera()
	{
		pmax = D3DX_PI / 2;
		pmin = -D3DX_PI / 2;

		zmin = 0.1f;
		zmax = 5.0f;

		euler = D3DXVECTOR3(0, 0, 0);
	}
	//=============================================================================================================
	void CArcballCamera::OrbitRight(float angle)
	{
		euler.x = fmodf(euler.x + angle, D3DX_PI * 2);
	}
	//=============================================================================================================
	void CArcballCamera::OrbitUp(float angle)
	{
		euler.y = fmodf(euler.y + angle, D3DX_PI * 2);
	}
	//=============================================================================================================
	void CArcballCamera::SetOrientation(const D3DXVECTOR3& angles)
	{
		euler.x = fmodf(angles.x, D3DX_PI * 2);
		euler.y = fmodf(angles.y, D3DX_PI * 2);
		euler.z = fmodf(angles.z, D3DX_PI * 2);
	}
	//=============================================================================================================
	void CArcballCamera::SetOrientation(float x, float y, float z)
	{
		euler.x = fmodf(x, D3DX_PI * 2);
		euler.y = fmodf(y, D3DX_PI * 2);
		euler.z = fmodf(z, D3DX_PI * 2);
	}
	//=============================================================================================================
	D3DXVECTOR3& CArcballCamera::GetEyePosition(D3DXVECTOR3& out)
	{
		euler.y = CHelper::Clamp<float>(euler.y, pmin, pmax);

		D3DXQuaternionRotationYawPitchRoll(&orientation, -euler.x, euler.y, euler.z);
		D3DXQuaternionNormalize(&orientation, &orientation);

		Forward(forward);
		out = position - (distance * forward);

		return out;
	}
	//=============================================================================================================
	D3DXMATRIX& CArcballCamera::GetViewMatrix(D3DXMATRIX& out)
	{
		euler.y = CHelper::Clamp<float>(euler.y, pmin, pmax);

		D3DXQuaternionRotationYawPitchRoll(&orientation, -euler.x, euler.y, euler.z);
		D3DXQuaternionNormalize(&orientation, &orientation);

		Up(up);
		Forward(forward);

		eye = position - (forward * distance);
		D3DXMatrixLookAtLH(&out, &eye, &position, &up);

		return out;
	}
	//=============================================================================================================
}
