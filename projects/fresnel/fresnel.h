//=============================================================================================================
#ifndef _FRESNEL_H_
#define _FRESNEL_H_

#define _DUMMY_DX 9
#include <dummyframework/base/d3d9/CGame9.h>
#include <dummyframework/base/d3d9/CObject9.h>
#include <dummyframework/camera/CModelViewerCamera.h>
#include <dummyframework/renderers/d3d9/CWaterRenderer9.h>
#include <dummyframework/renderers/d3d9/CEnvironmentRenderer9.h>
#include <dummyframework/renderers/d3d9/CBloomRenderer9.h>
#include <dummyframework/renderers/d3d9/CScreenFader9.h>
#include <vector>

class Fresnel : public DummyFramework::CGame9
{
public:
	Fresnel();
	~Fresnel();

protected:
	bool Initialize();
	bool LoadContent();
	bool LoadSettings();

	void Update();
	void Draw();

private:
	typedef std::vector<DummyFramework::CObject9> objectlist;

	DummyFramework::CScreenFader9*         screenfader;
	DummyFramework::CBloomRenderer9*       bloomrenderer;
	DummyFramework::CWaterRenderer9*       reflectionrenderer;
	DummyFramework::CEnvironmentRenderer9* environmentrenderer;
	DummyFramework::CModelViewerCamera     camera;

	D3DXMATRIX view, proj, viewproj;
	D3DXVECTOR3 eye;
	objectlist objects;

eventhandlers:
	void application_keyup(const DummyFramework::skeyboardstate& kstate);
	void screenfader_faded(DummyFramework::CScreenFader9::efademode mode);
	void environmentrenderer_render();
	void reflectionrenderer_renderreflection();
};

#endif
//=============================================================================================================
 
