//=============================================================================================================
#ifndef _CGAME9_H_
#define _CGAME9_H_

#include "../CApplication.h"
#include "../CGameLogicSynchronizer.h"

#include "CContentManager9.h"
#include "CScreenQuad9.h"

#include <d3dx9.h>
#include <list>

#define _DUMMY_EFFECT_DONT_SAVE_   D3DXFX_DONOTSAVESTATE|D3DXFX_DONOTSAVESAMPLERSTATE|D3DXFX_DONOTSAVESHADERSTATE

namespace DummyFramework
{
    class CEffectRenderer9;

	/**
	 * \brief Base class for DummyFramework applications
	 *
	 * The <kbd>CGame9</kbd> class is responsible for Direct3D initialization,
	 * device loss/reset handling and fixed timestep updating of the
	 * application's logic.
	 
	 * Note that the <kbd>Update()</kbd> method is called
	 * in discrete time steps (every 1/10 th second by default), that
	 * can be modified through the <kbd>Sync.UpdateInterval</kbd> member.
	 * Fixed timestep updating can be turned off by setting the
	 * <kbd>fixedtimestep</kbd> member to false.
	 *
	 * Another important thing is <i>state management</i>, that was
	 * recently introduced into the framework. This requires that if
	 * something can be called on the <kbd>StateManager</kbd> object
	 * then that has to be called on it. This implies that state blocks
	 * should be used with care (that means you have to guarantee that
	 * the framework stays consistent).
	 */
    class CGame9 : public has_slots
    {
    private:
        CScreenQuad9* screenquad;
        bool valid;

        bool Lose();
        bool Validate();
        void ShutDown();

        bool CheckDepthFormat(D3DFORMAT disp, D3DFORMAT back, D3DFORMAT depth);
        bool FindBestMultiSample(D3DFORMAT backbuffer, bool fullscreen, D3DMULTISAMPLE_TYPE& type);
		bool FindBestDepthStencil(D3DFORMAT disp, D3DFORMAT back, D3DFORMAT& depth);
        bool FindBestFormats(D3DFORMAT disp, D3DFORMAT& back, D3DFORMAT& depth);

    protected:
        typedef std::list<CEffectRenderer9*> rendererlist;
        
        rendererlist   effects;
        rendererlist   posteffects;
        bool           fixedtimestep;
        bool           devicelost;
        unsigned long  resetcounter;

        virtual bool Initialize();
        virtual bool LoadContent();

        virtual void UnloadContent();
        virtual void Update();
        virtual void Draw();
        virtual void ResetRenderStates();

		inline bool CanUseMultipleRTs() const {
			return (DeviceCaps.NumSimultaneousRTs > 1);
		}

    public:
        D3DPRESENT_PARAMETERS   PresentationParameters;
		D3DCAPS9                DeviceCaps;
        LPDIRECT3D9             Direct3D;
        LPDIRECT3DDEVICE9       Graphics;
		CContentManager9        Content;
        CApplication            Application;
        CGameLogicSynchronizer  Sync;
		CStateManager9*         StateManager;

        // this require device reset (or you set them before calling Initialize())
        D3DDISPLAYMODE          DisplayMode;
        bool                    FullScreen;
        bool                    VSync;
        bool                    ZBuffer;
        bool                    Stencil;
		bool                    MultiThreaded;
		bool                    RenderDeactive;

        CGame9();
        virtual ~CGame9();

        virtual void Run();

        bool Reset();
        void EnumDisplayModes(resolutionset& out);

        inline bool IsDeviceLost() const {
            return devicelost;
        }

        inline void DrawImage(const D3DXMATRIX& transform, unsigned long color = 0xffffffff) {
            screenquad->Draw(transform, color);
        }

        inline void DrawImage(const D3DXMATRIX& transform, float texcoords[4], unsigned long color) {
            screenquad->Draw(transform, texcoords, color);
        }

    events:
        signal0 lost;
        signal0 reset;

    eventhandlers:
        virtual void onactivate();
        virtual void ondeactivate();
        virtual void onlostdevice();
        virtual void onresetdevice();
    };
}

#endif
//=============================================================================================================
