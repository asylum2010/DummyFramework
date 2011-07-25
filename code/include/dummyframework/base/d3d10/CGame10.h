//=============================================================================================================
#ifndef _CGAME9_H_
#define _CGAME9_H_

#include "../CApplication.h"
#include "../CGameLogicSynchronizer.h"

#include <d3dx10.h>
#include <list>

namespace DummyFramework
{
    class CEffectRenderer10;

    class CGame10 : public has_slots
    {
    private:
        bool valid;

        bool Lose();
        bool Validate();
        void ShutDown();

    protected:
        typedef std::list<CEffectRenderer10*> rendererlist;
        
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

    public:
        DXGI_SWAP_CHAIN_DESC     SwapChainDesc;
		DXGI_MODE_DESC           DisplayMode;

        ID3D10Device*            Graphics;
		ID3D10RenderTargetView*  RenderTargetView;
		ID3D10DepthStencilView*  DepthStencilView;
		ID3D10Texture2D*         DepthStencil;
		IDXGISwapChain*          SwapChain;

        CApplication             Application;
        CGameLogicSynchronizer   Sync;
        bool                     FullScreen;
        bool                     VSync;
        bool                     ZBuffer;
        bool                     Stencil;

        CGame10();
        virtual ~CGame10();

        virtual void Run();
       
        inline bool IsDeviceLost() const {
            return devicelost;
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
