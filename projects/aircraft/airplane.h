//=============================================================================================================
#ifndef _AIRPLANE_H_
#define _AIRPLANE_H_

#include <dummyframework/base/d3d9/CGame9.h>
#include <dummyframework/base/d3d9/CObject9.h>
#include <dummyframework/template/signal.hpp>
#include <deque>

class AirPlane : public DummyFramework::CObject9, public DummyFramework::has_slots
{
	enum planestate
	{
		Idle = 0,
		RollLeft = 1,
		RollRight = 2,
		PitchUp = 4,
		PitchDown = 8,
		YawLeft = 16,
		YawRight = 32,
		Move = 64
	};

private:
	DummyFramework::syncedlinear<D3DXVECTOR3> position;
	DummyFramework::syncedlinear<D3DXQUATERNION> rotation;

	unsigned long state;
	double lastinput;

public:
	AirPlane();
	~AirPlane() {}

	void MakeStatePack(std::string& out);
	void UpdateFromPack(const std::string& str, double time);
	void Update(double time);
	void UpdateTransforms(float alpha);

	inline D3DXVECTOR3& GetPosition() {
		return position.value;
	}

	inline bool Interacting() const {
		return (state != Idle);
	}

eventhandlers:
	void onkeyup(const DummyFramework::skeyboardstate& kstate);
	void onkeydown(const DummyFramework::skeyboardstate& kstate);
};

#endif
//=============================================================================================================
