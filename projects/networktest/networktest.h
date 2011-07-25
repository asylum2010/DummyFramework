//=============================================================================================================
#ifndef _NETWORKTEST_H_
#define _NETWORKTEST_H_

#define _DUMMY_DX 9
#include <dummyframework/network/udpclient.h>
#include <dummyframework/network/udpserver.h>

#include <dummyframework/base/d3d9/CGame9.h>
#include <dummyframework/thread/CThread.h>
#include <dummyframework/camera/CModelViewerCamera.h>

#include "../aircraft/airplane.h"

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

	DummyFramework::udpconnection connection;
	DummyFramework::udpclient client;
	DummyFramework::udpserver server;
	DummyFramework::udpchannel channel;

	D3DXMATRIX view, proj;
	bool isserver;
	bool cansend;

	planelist airplanes;
	std::string hostip;

	void operator ()();
	void run_client();
	void run_server();

eventhandlers:
	void application_keyup(const DummyFramework::skeyboardstate& kstate);

	void application_closed() {
		worker.Stop();
		connection.disconnect();
	}
};

#endif
//===============================================================================