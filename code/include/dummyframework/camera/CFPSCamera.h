//=============================================================================================================
#ifndef _CFPSCAMERA_H_
#define _CFPSCAMERA_H_

#include "CAbstractCamera.h"
#include "../template/signal.hpp"
#include "../template/defines.h"

namespace DummyFramework
{
    struct smousestate;
    struct skeyboardstate;

    class CFPSCamera : public CAbstractCamera, public has_slots
    {
        enum ecamerastate
        {
            CS_Idle = 0,
            CS_Rotating = 1,
            CS_Forward = 2,
			CS_Backward = 4,
			CS_Left = 8,
			CS_Right = 16,
			CS_Jumping = 32,
			CS_Crouching = 64,
			CS_Shooting = 128
        };

		struct weaponanim
		{
			inline static D3DXVECTOR2 y(float x) {
				D3DXVECTOR2 out(x - 0.5f, cos(D3DX_PI * x + D3DX_PI * 0.5f) + 1);
				return out;
			}
		};

		struct pushbackanim
		{
			inline static float y(float x) {
				return ((x < 0.1f) ? 1.0f : (1.0f - (x - 1) * (x - 1)));
			}
		};
        
    private:
		syncedanimator<D3DXVECTOR2, 5, weaponanim> weapon;
		syncedanimator<float, 5, pushbackanim> pushback;
		syncedlinear<D3DXVECTOR3> move;

		D3DXVECTOR3 euler;
		D3DXVECTOR3 up, forward, right;
        D3DXVECTOR2 rot, dest_rot;

        unsigned long state;
		unsigned int weaponcd;
		float pmin, pmax;

    public:
        float RotationSpeed;
		float MoveSpeed;
		unsigned int WeaponCooldown;

        CFPSCamera();

        void Update();
        void UpdateTransforms(float alpha);
		void SetOrientation(const D3DXVECTOR3& angles);
        void SetOrientation(float x, float y, float z);
		void SetPosition(const D3DXVECTOR3& pos);
        void SetPosition(float x, float y, float z);
		void XRotate(float angle);
        void YRotate(float angle);

		D3DXMATRIX& GetViewMatrix(D3DXMATRIX& out);
        D3DXVECTOR3& GetEyePosition(D3DXVECTOR3& out);

		inline const D3DXVECTOR2& GetWeaponOffset() const {
			return weapon.value;
		}

		inline const float GetWeaponPushback() const {
			return pushback.value;
		}

	events:
		DummyFramework::unisignal0 shoot;

    eventhandlers:
        void onmousemove(const smousestate& mstate);
        void onkeydown(const skeyboardstate& kbstate);
        void onkeyup(const skeyboardstate& kbstate);
		void onmousedown(const smousestate& mstate);
        void onmouseup(const smousestate& mstate);
    };

	inline void CFPSCamera::SetPosition(const D3DXVECTOR3& pos) {
		move = pos;
	}

	inline void CFPSCamera::SetPosition(float x, float y, float z) {
		move = D3DXVECTOR3(x, y, z);
	}

	inline D3DXVECTOR3& CFPSCamera::GetEyePosition(D3DXVECTOR3& out) {
		return (out = move.value);
	}
}

#endif
//=============================================================================================================
