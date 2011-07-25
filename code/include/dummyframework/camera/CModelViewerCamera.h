//=============================================================================================================
#ifndef _CMODELVIEWERCAMERA_H_
#define _CMODELVIEWERCAMERA_H_

#include "CArcballCamera.h"
#include "../template/signal.hpp"
#include "../template/defines.h"

namespace DummyFramework
{
    struct smousestate;
    struct skeyboardstate;

    class CModelViewerCamera : public CArcballCamera, public has_slots
    {
        enum ecamerastate
        {
            CS_Idle = 0,
            CS_Rotating = 1,
            CS_ZoomIn = 2,
            CS_ZoomOut = 4
        };
        
    private:
        syncedlinear<float> zoom;

        D3DXVECTOR2 rot;
        D3DXVECTOR2 dest_rot;
        unsigned long state;

    public:
        float RotationSpeed;
        float ZoomSpeed;

        CModelViewerCamera();

        void Update();
        void UpdateTransforms(float alpha);
        
		inline void SetDistance(float value) {
			zoom = value;
		}

    eventhandlers:
        void onmousedown(const smousestate& mstate);
        void onmouseup(const smousestate& mstate);
        void onmousemove(const smousestate& mstate);
        void onkeydown(const skeyboardstate& kbstate);
        void onkeyup(const skeyboardstate& kbstate);
    };
}

#endif
//=============================================================================================================
