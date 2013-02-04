//=============================================================================================================
#ifndef _AIRCRAFT_H_
#define _AIRCRAFT_H_

#include <dummyframework/base/d3d9/CGame9.h>
#include <dummyframework/camera/CModelViewerCamera.h>

#include "airplane.h"

class Aircraft : public DummyFramework::CGame9
{
public:
	Aircraft();
	~Aircraft();

protected:
	bool Initialize();
	bool LoadContent();
	bool LoadSettings();

	void ResetRenderStates();
	void Update();
	void Draw();

private:
	DummyFramework::CModelViewerCamera camera;
	DummyFramework::CObject9 ground;

	AirPlane plane;
	D3DXMATRIX view, proj;

_DUMMY_EVENTHANDLERS:
	void application_keyup(const DummyFramework::skeyboardstate& kstate);
	void application_keydown(const DummyFramework::skeyboardstate& kstate);
};

#endif
//=============================================================================================================
