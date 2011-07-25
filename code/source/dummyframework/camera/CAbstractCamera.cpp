//=============================================================================================================
#include <dummyframework/camera/CAbstractCamera.h>
#include <dummyframework/base/CApplication.h>

namespace DummyFramework
{
    CAbstractCamera::CAbstractCamera()
    {
		Button = MB_Left;

        D3DXQuaternionIdentity(&orientation);

        distance    = 1.0f;
        clipnear    = 1.0f;
        clipfar     = 100.0f;
        aspect      = 4.0f / 3.0f;
        fov         = D3DX_PI / 3;
    }
    //=============================================================================================================
    D3DXVECTOR3& CAbstractCamera::Forward(D3DXVECTOR3& out)
    {
        out.x =  2.0f * (orientation.z * orientation.x + orientation.w * orientation.y);
        out.y = -2.0f * (orientation.w * orientation.x - orientation.z * orientation.y);
        out.z = (orientation.z * orientation.z + orientation.w * orientation.w) -
                  (orientation.y * orientation.y + orientation.x * orientation.x);

        D3DXVec3Normalize(&out, &out);
        return out;
    }
    //=============================================================================================================
    D3DXVECTOR3& CAbstractCamera::Right(D3DXVECTOR3& out)
    {
        out.x = (orientation.x * orientation.x + orientation.w * orientation.w) -
                  (orientation.z * orientation.z + orientation.y * orientation.y);
        out.y =  2.0f * (orientation.x * orientation.y + orientation.z * orientation.w);
        out.z = -2.0f * (orientation.y * orientation.w - orientation.z * orientation.x);

        D3DXVec3Normalize(&out, &out);
        return out;
    }
    //=============================================================================================================
    D3DXVECTOR3& CAbstractCamera::Up(D3DXVECTOR3& out)
    {
        out.x = -2.0f * (orientation.z * orientation.w - orientation.x * orientation.y);
        out.y = (orientation.y * orientation.y + orientation.w * orientation.w) -
                  (orientation.x * orientation.x + orientation.z * orientation.z);
        out.z = 2.0f * (orientation.y * orientation.z + orientation.x * orientation.w);

        D3DXVec3Normalize(&out, &out);
        return out;
    }
    //=============================================================================================================
}
