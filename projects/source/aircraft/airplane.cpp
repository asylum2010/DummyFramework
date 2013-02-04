//=============================================================================================================
#include "airplane.h"

#include <dummyframework/base/CHelper.h>
#include <sstream>

AirPlane::AirPlane()
{
	state = 0;
	lastinput = -1;

	D3DXQUATERNION q;
	D3DXQuaternionIdentity(&q);

	rotation = q;
	position = D3DXVECTOR3(0, 0, 0);
}
//=============================================================================================================
void AirPlane::MakeState(unsigned int& input, D3DXVECTOR3& pos, D3DXQUATERNION& q)
{
	input = state;

	pos = position.current();
	q = rotation.current();
}
//=============================================================================================================
void AirPlane::UpdateState(unsigned int input, const D3DXVECTOR3& pos, const D3DXQUATERNION& q, double time)
{
	D3DXVECTOR3 pd;
	D3DXQUATERNION qd;

	state = input;

	// szinkronizácio
	pd = pos - position.current();

	//if( D3DXVec3Length(&pd) > 6 )
		position = pos;
	
	qd = q - rotation.current();

	//if( D3DXQuaternionLength(&qd) > 1.5f )
		rotation = q;

	lastinput = time;
}
//=============================================================================================================
void AirPlane::Update(double time)
{
	// ez lesz a lagg effekt
	if( lastinput > 0 && (time - lastinput > 0.1f) )
		state = Idle;

	float pitch = 0, roll = 0, yaw = 0, move = 0;

	if( state & RollLeft )
		roll = -0.2f;
	else if( state & RollRight )
		roll = 0.2f;
	else if( state & PitchUp )
		pitch = -0.2f;
	else if( state & PitchDown )
		pitch = 0.2f;
	else if( state & YawLeft )
		yaw = -0.2f;
	else if( state & YawRight )
		yaw = 0.2f;

	if( state & AirPlane::Move )
		move = -2;
	
	D3DXVECTOR3& p = position.current();
	D3DXQUATERNION& q = rotation.current();

	D3DXVECTOR3 x, y, z;
	D3DXQUATERNION t, u, v, w;
	
	x.x = q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z;
    x.y = 2.0f * (q.z * q.w + q.x * q.y);
    x.z = 2.0f * (q.x * q.z - q.y * q.w);

	y.x = 2.0f * (q.y * q.x - q.z * q.w);
    y.y = q.w * q.w + q.y * q.y - q.z * q.z - q.x * q.x;
    y.z = 2.0f * (q.x * q.w + q.y * q.z);

	z.x = 2.0f * (q.y * q.w + q.z * q.x);
    z.y = 2.0f * (q.z * q.y - q.x * q.w);
    z.z = q.w * q.w + q.z * q.z - q.x * q.x - q.y * q.y;

	D3DXQuaternionRotationAxis(&u, &z, roll);
	D3DXQuaternionRotationAxis(&v, &x, pitch);
	D3DXQuaternionRotationAxis(&t, &y, yaw);

	D3DXQuaternionMultiply(&w, &u, &v);
	D3DXQuaternionMultiply(&w, &w, &t);

	rotation.previous() = q;
	D3DXQuaternionMultiply(&q, &q, &w);

	D3DXVec3Normalize(&z, &z);
	position.previous() = p;

	p += z * move;
}
//=============================================================================================================
void AirPlane::UpdateTransforms(float alpha)
{
	rotation.smooth(alpha);
	Rotate(rotation.value);

	position.smooth(alpha);
	Translate(position.value);
}
//=============================================================================================================
void AirPlane::onkeyup(const DummyFramework::skeyboardstate& kstate)
{
	switch( kstate.key )
	{
	case 0x41:
		state &= (~AirPlane::RollLeft);
		break;

	case 0x44:
		state &= (~AirPlane::RollRight);
		break;

	case 0x53:
		state &= (~AirPlane::PitchDown);
		break;

	case 0x57:
		state &= (~AirPlane::PitchUp);
		break;

	case 0x51:
		state &= (~AirPlane::YawLeft);
		break;

	case 0x45:
		state &= (~AirPlane::YawRight);
		break;

	case VK_CONTROL:
		state &= (~Move);
		break;

	default:
		break;
	}
}
//=============================================================================================================
void AirPlane::onkeydown(const DummyFramework::skeyboardstate& kstate)
{
	switch( kstate.key )
	{
	case 0x41:
		state |= AirPlane::RollLeft;
		break;

	case 0x44:
		state |= AirPlane::RollRight;
		break;

	case 0x53:
		state |= AirPlane::PitchDown;
		break;

	case 0x57:
		state |= AirPlane::PitchUp;
		break;

	case 0x51:
		state |= AirPlane::YawLeft;
		break;

	case 0x45:
		state |= AirPlane::YawRight;
		break;

	case VK_CONTROL:
		state |= Move;
		break;

	default:
		break;
	}
}
//=============================================================================================================
