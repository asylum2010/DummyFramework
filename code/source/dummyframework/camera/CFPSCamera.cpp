//=============================================================================================================
#include <dummyframework/camera/CFPSCamera.h>
#include <dummyframework/base/CApplication.h>
#include <dummyframework/base/CHelper.h>

namespace DummyFramework
{
	CFPSCamera::CFPSCamera()
	{
		RotationSpeed = 0.004f;
		MoveSpeed = 1;
		WeaponCooldown = 10;

		dest_rot = rot = D3DXVECTOR2(0, 0);
		euler = D3DXVECTOR3(0, 0, 0);
		
		state = 0;
		weaponcd = 0;
		pmax = D3DX_PI * 0.5f;
        pmin = D3DX_PI * -0.5f;

		move = D3DXVECTOR3(0, 0, 0);

		weapon.set(D3DXVECTOR2(-0.5f, 1), D3DXVECTOR2(0.5f, 1));
		weapon = weapon.count() / 2;
		weapon.direction = -1;

		pushback.set(1, 1);
		pushback = pushback.count() - 1;
		pushback.direction = 1;
	}
	//=============================================================================================================
	void CFPSCamera::Update()
	{
		D3DXVECTOR3 ex(0, 0, 0);

		up = D3DXVECTOR3(0, 1, 0);
		Right(right);

		D3DXVec3Cross(&forward, &right, &up);
		
		if( state & CS_Forward )
			ex += forward;
		else if( state & CS_Backward )
			ex -= forward;

		if( state & CS_Left )
			ex -= right;
		else if( state & CS_Right )
			ex += right;

		if( state & CS_Forward || state & CS_Backward || state & CS_Left || state & CS_Right )
		{
			weapon.step();

			if( weapon.current() == 0 )
				weapon.direction = 1;
			else if( weapon.current() == weapon.count() - 1 )
				weapon.direction = -1;
		}
		else
		{
			if( weapon.current() == weapon.count() / 2 )
			{
				weapon.direction = -1;
				weapon = weapon.count() / 2;
			}
			else
			{
				weapon.previous() = weapon.current();
				weapon.current() = weapon.count() / 2;

				weapon.direction *= -1;
			}
		}

		if( state & CS_Shooting && weaponcd == 0 )
		{
			weaponcd = WeaponCooldown;
			pushback = 0;

			shoot();
		}

		if( weaponcd > 0 )
			--weaponcd;

		pushback.step();
		
		D3DXVec3Normalize(&ex, &ex);
		ex *= MoveSpeed;

		move.extend(ex);
	}
	//=============================================================================================================
	void CFPSCamera::UpdateTransforms(float alpha)
	{
		move.smooth(alpha);
		weapon.smooth(alpha);
		pushback.smooth(alpha);

		CAbstractCamera::SetPosition(move.value);
	}
	//=============================================================================================================
	void CFPSCamera::SetOrientation(const D3DXVECTOR3& angles)
	{
		euler.x = fmodf(angles.x, D3DX_PI * 2.0f);
        euler.y = fmodf(angles.y, D3DX_PI * 2.0f);
        euler.z = fmodf(angles.z, D3DX_PI * 2.0f);
	}
	//=============================================================================================================
    void CFPSCamera::SetOrientation(float x, float y, float z)
	{
		euler.x = fmodf(x, D3DX_PI * 2.0f);
        euler.y = fmodf(y, D3DX_PI * 2.0f);
        euler.z = fmodf(z, D3DX_PI * 2.0f);
	}
	//=============================================================================================================
	void CFPSCamera::XRotate(float angle)
	{
		euler.y = fmodf(euler.y + angle, D3DX_PI * 2.0f);
	}
	//=============================================================================================================
    void CFPSCamera::YRotate(float angle)
	{
		euler.x = fmodf(euler.x + angle, D3DX_PI * 2.0f);
	}
	//=============================================================================================================
	D3DXMATRIX& CFPSCamera::GetViewMatrix(D3DXMATRIX& out)
	{
		euler.y = CHelper::Clamp(euler.y, pmin, pmax);

		D3DXQuaternionRotationYawPitchRoll(&orientation, euler.x, euler.y, euler.z);
		D3DXQuaternionNormalize(&orientation, &orientation);

        Up(up);
		Forward(forward);

		D3DXVECTOR3 eye;
		GetEyePosition(eye);

		D3DXVec3Cross(&right, &up, &forward);
                
        out._11 = right.x;  out._12 = up.x;  out._13 = forward.x;  out._14 = 0.0f;
        out._21 = right.y;  out._22 = up.y;  out._23 = forward.y;  out._24 = 0.0f;
        out._31 = right.z;  out._32 = up.z;  out._33 = forward.z;  out._34 = 0.0f;

        out._41 = -D3DXVec3Dot(&right, &eye);
        out._42 = -D3DXVec3Dot(&up, &eye);
        out._43 = -D3DXVec3Dot(&forward, &eye);
        out._44 = 1.0f;

        return out;
	}
	//=============================================================================================================
    void CFPSCamera::onmousemove(const smousestate& mstate)
    {
        dest_rot.x = (float)mstate.dx * RotationSpeed;
        dest_rot.y = (float)mstate.dy * RotationSpeed;

        D3DXVec2Lerp(&rot, &rot, &dest_rot, 0.65f);

        XRotate(rot.y);
		YRotate(rot.x);
    }
	//=============================================================================================================
	void CFPSCamera::onmousedown(const smousestate& mstate)
	{
		if( mstate.button & Button )
            state |= CS_Shooting;
	}
	//=============================================================================================================
    void CFPSCamera::onmouseup(const smousestate& mstate)
	{
		if( mstate.button & Button )
			state &= (~CS_Shooting);
	}
	//=============================================================================================================
    void CFPSCamera::onkeydown(const skeyboardstate& kbstate)
    {
		if( kbstate.keytable[0x57] & 0x80 )
			state |= CS_Forward;

		if( kbstate.keytable[0x53] & 0x80 )
			state |= CS_Backward;

		if( kbstate.keytable[0x44] & 0x80 )
			state |= CS_Right;

		if( kbstate.keytable[0x41] & 0x80 )
			state |= CS_Left;
    }
    //=============================================================================================================
    void CFPSCamera::onkeyup(const skeyboardstate& kbstate)
    {
        if( kbstate.keytable[0x57] == 0 )
			state &= (~CS_Forward);

		if( kbstate.keytable[0x53] == 0 )
			state &= (~CS_Backward);

		if( kbstate.keytable[0x44] == 0 )
			state &= (~CS_Right);

		if( kbstate.keytable[0x41] == 0 )
			state &= (~CS_Left);
    }
	//=============================================================================================================
}

