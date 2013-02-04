//=============================================================================================================
#include <dummyframework/base/CTransformable.h>

namespace DummyFramework
{
	CTransformable::CTransformable()
	{
		needupdate		= false;
		scaling			= D3DXVECTOR3(1, 1, 1);
		translation		= D3DXVECTOR3(0, 0, 0);
		scalingcenter	= translation;
		rotationcenter	= translation;

		D3DXQuaternionIdentity(&rotation);
		D3DXMatrixIdentity(&transform);

		D3DXMatrixIdentity(&UserTransform);
	}
	//=============================================================================================================
	D3DXMATRIX& CTransformable::GetTransform()
	{
		if( needupdate )
		{
			D3DXMatrixTransformation(&transform, &scalingcenter, NULL, &scaling,
				&rotationcenter, &rotation, &translation);

			D3DXMatrixMultiply(&transform, &UserTransform, &transform);
			needupdate = false;
		}

		return transform;
	}
	//=============================================================================================================
	D3DXMATRIX& CTransformable::GetInvTransform()
	{
		GetTransform();

		D3DXMatrixInverse(&invtransform, NULL, &transform);
		return invtransform;
	}
	//=============================================================================================================
}
