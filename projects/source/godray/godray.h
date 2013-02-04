//=============================================================================================================
#ifndef _GODRAY_H_
#define _GODRAY_H_

#define _DUMMY_DX 9
#include <dummyframework/base/d3d9/CGame9.h>
#include <dummyframework/base/d3d9/CObject9.h>
#include <dummyframework/camera/CModelViewerCamera.h>
#include <dummyframework/renderers/d3d9/CWaterRenderer9.h>
#include <dummyframework/renderers/d3d9/CBloomRenderer9.h>
#include <dummyframework/renderers/d3d9/CGodRayRenderer9.h>
#include <dummyframework/renderers/d3d9/CScreenFader9.h>
#include <vector>

class Godray : public DummyFramework::CGame9
{
public:
	Godray();
	~Godray();

protected:
	bool Initialize();
	bool LoadContent();
	bool LoadSettings();

	void Update();
	void Draw();

	void DrawGodray();
	void DrawGodrayMRT();
	void DrawGodrayBloom();
	void DrawGodrayBloomMRT();
	void DrawScene();

private:
	typedef std::vector<DummyFramework::CObject9> objectlist;

	DummyFramework::CScreenFader9*        screenfader;
	DummyFramework::CBloomRenderer9*      bloomrenderer;
	DummyFramework::CWaterRenderer9*      reflectionrenderer;
	DummyFramework::CGodRayRenderer9*     godrayrenderer;
	DummyFramework::CModelViewerCamera    camera;

	D3DXMATRIX view, proj, viewproj;
	D3DXVECTOR3 eye;
	objectlist objects;

_DUMMY_EVENTHANDLERS:
	void application_keyup(const DummyFramework::skeyboardstate& kstate);
	void screenfader_faded(DummyFramework::CScreenFader9::efademode mode);
	void reflectionrenderer_renderreflection();
};

#endif
//=============================================================================================================
 
