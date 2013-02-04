//=============================================================================================================
#ifndef _NETWORKTEST_H_
#define _NETWORKTEST_H_

#define _DUMMY_DX 9
#include <dummyframework/base/d3d9/CGame9.h>
#include <dummyframework/thread/CThread.h>
#include <dummyframework/camera/CModelViewerCamera.h>

#include "../aircraft/airplane.h"
#include "networkserviceprovider.h"

class NetworkTest : public DummyFramework::CGame9, public DummyFramework::CCallable
{
public:
	NetworkTest();
	~NetworkTest();

protected:
	bool Initialize();
	bool LoadContent();
	bool LoadSettings();

	void ResetRenderStates();
	void Update();
	void Draw();

private:
	typedef std::vector<AirPlane> planelist;

	DummyFramework::CThread worker;
	DummyFramework::CObject9 ground;
	DummyFramework::CModelViewerCamera camera;

	D3DXMATRIX view, proj;
	bool isserver;
	bool cansend;
	planelist airplanes;
	std::string hostip;

	NetworkServiceProvider* service;

	void operator ()();
	void run_client();
	void run_server();

_DUMMY_EVENTHANDLERS:
	void application_keyup(const DummyFramework::skeyboardstate& kstate);
	void application_closed();
};

#endif
//===============================================================================