//=============================================================================================================
#ifndef _CAUSTICS_H_
#define _CAUSTICS_H_

#define _DUMMY_DX 9
#include <dummyframework/base/d3d9/CGame9.h>
#include <dummyframework/base/d3d9/CObject9.h>
#include <dummyframework/camera/CModelViewerCamera.h>
#include <dummyframework/renderers/d3d9/CCausticsRenderer9.h>
#include <dummyframework/renderers/d3d9/CBloomRenderer9.h>
#include <dummyframework/renderers/d3d9/CScreenFader9.h>
#include <vector>

// TODO: ezért kell a cubemapon rekonstruálni... (h ne terüljön szét)

class Caustics : public DummyFramework::CGame9
{
public:
	Caustics();
	~Caustics();

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
	DummyFramework::CCausticsRenderer9*    causticsrenderer;
	DummyFramework::CModelViewerCamera     camera;
	DummyFramework::CModelViewerCamera     light;
	
	D3DXMATRIX view, proj, viewproj;
	D3DXMATRIX lightvp, lightv;
	D3DXVECTOR3 eye, lightpos;
	
	objectlist objects;

eventhandlers:
	void application_keyup(const DummyFramework::skeyboardstate& kstate);
	void screenfader_faded(DummyFramework::CScreenFader9::efademode mode);

	void causticsrenderer_rendercolor(size_t index);
	void causticsrenderer_renderdistance(size_t index);
	void causticsrenderer_photontrace();
	void causticsrenderer_rendershadow();
};

#endif
//===============================================================================