//=============================================================================================================
#ifndef _CARCBALLCAMERA_H_
#define _CARCBALLCAMERA_H_

#include "CAbstractCamera.h"

namespace DummyFramework
{
    class CArcballCamera : public CAbstractCamera
    {
    private:
        D3DXVECTOR3 up;
        D3DXVECTOR3 forward;
        D3DXVECTOR3 eye;

    protected:
        D3DXVECTOR3 euler;
        float pmax, pmin;
        float zmax, zmin;

    public:
        CArcballCamera();

        void OrbitRight(float angle);
        void OrbitUp(float angle);
        void SetOrientation(const D3DXVECTOR3& angles);
        void SetOrientation(float x, float y, float z);

        virtual D3DXVECTOR3& GetEyePosition(D3DXVECTOR3& out);
        virtual D3DXMATRIX& GetViewMatrix(D3DXMATRIX& out);

        inline void ClampPitch(float low, float high) {
			pmin = low;
			pmax = high;
		}

        inline void ClampZoom(float low, float high) {
			zmin = low;
			zmax = high;
		}
    };
}

#endif
//=============================================================================================================
