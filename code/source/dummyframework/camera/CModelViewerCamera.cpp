//=============================================================================================================
#include <dummyframework/camera/CModelViewerCamera.h>
#include <dummyframework/base/CApplication.h>
#include <dummyframework/base/CHelper.h>

namespace DummyFramework
{
    CModelViewerCamera::CModelViewerCamera()
    {
        RotationSpeed = 0.004f;
        ZoomSpeed = 0.2f;

        rot.x = rot.y = 0;
        dest_rot.x = dest_rot.y = 0;

		state = 0;
        zoom = distance;
    }
    //=============================================================================================================
    void CModelViewerCamera::Update()
    {
        float speed = (state & CS_ZoomIn ? ZoomSpeed : (state & CS_ZoomOut ? -ZoomSpeed : 0));

        zoom.extend(speed);
        zoom.clamp(zmin, zmax);
    }
    //=============================================================================================================
    void CModelViewerCamera::UpdateTransforms(float alpha)
    {
        zoom.smooth(alpha);
        distance = zoom.value;
    }
    //=============================================================================================================
    void CModelViewerCamera::onmousemove(const smousestate& mstate)
    {
        dest_rot.x = dest_rot.y = 0;

        if( state & CS_Rotating )
        {
            dest_rot.x = (float)mstate.dx * RotationSpeed;
            dest_rot.y = (float)mstate.dy * RotationSpeed;
        }

        D3DXVec2Lerp(&rot, &rot, &dest_rot, 0.65f);

        OrbitUp(rot.y);
        OrbitRight(-rot.x);
    }
    //=============================================================================================================
    void CModelViewerCamera::onmousedown(const smousestate& mstate)
    {
        if( mstate.button & Button )
            state |= CS_Rotating;
    }
    //=============================================================================================================
    void CModelViewerCamera::onmouseup(const smousestate& mstate)
    {
        if( mstate.button & Button )
		{
			rot.x = rot.y = 0;
			state &= (~CS_Rotating);
		}
    }
    //=============================================================================================================
    void CModelViewerCamera::onkeydown(const skeyboardstate& kbstate)
    {
        if( kbstate.keytable[0x45] & 0x80 )
            state |= CS_ZoomOut;

        if( kbstate.keytable[0x51] & 0x80 )
            state |= CS_ZoomIn;
    }
    //=============================================================================================================
    void CModelViewerCamera::onkeyup(const skeyboardstate& kbstate)
    {
        if( kbstate.keytable[0x45] == 0 )
            state &= (~CS_ZoomOut);

        if( kbstate.keytable[0x51] == 0 )
            state &= (~CS_ZoomIn);
    }
    //=============================================================================================================
}
