//=============================================================================================================
#ifndef _CGAME9_H_
#define _CGAME9_H_

#include "../CApplication.h"
#include "../CGameLogicSynchronizer.h"

#include "CContentManager9.h"
#include "CScreenQuad9.h"

#include <d3dx9.h>
#include <list>

#define _DUMMY_EFFECT_DONT_SAVE_ D3DXFX_DONOTSAVESTATE|D3DXFX_DONOTSAVESAMPLERSTATE|D3DXFX_DONOTSAVESHADERSTATE

namespace DummyFramework
{
    class CEffectRenderer9;

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
        
        rendererlist            effects;
        rendererlist            posteffects;
        bool                    fixedtimestep;
        bool                    devicelost;
        unsigned long           resetcounter;

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

        // ezekhez reset szükséges (vagy az Initialize() elött beállitod)
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
