//=============================================================================================================
#include <dummyframework/base/d3d10/CGame10.h>
#include <dummyframework/base/CHelper.h>
#include <sstream>

namespace DummyFramework
{
    CGame10::CGame10()
    {
        Graphics          = NULL;
		SwapChain         = NULL;
		RenderTargetView  = NULL;
		DepthStencilView  = NULL;
		DepthStencil      = NULL;
        FullScreen        = false;
        VSync             = false;
        ZBuffer           = true;
        Stencil           = false;

        resetcounter      = 0;
        fixedtimestep     = true;
        devicelost        = false;

        DisplayMode.Format      = DXGI_FORMAT_R8G8B8A8_UNORM; // DXGI_FORMAT_UNKNOWN
		DisplayMode.Width       = 1024;
        DisplayMode.Height      = 768;

        DisplayMode.RefreshRate.Numerator = 60;
        DisplayMode.RefreshRate.Denominator = 1;

        Application.activated.connect(this, &CGame10::onactivate);
        Application.deactivated.connect(this, &CGame10::ondeactivate);

        lost.connect(this, &CGame10::onlostdevice);
        reset.connect(this, &CGame10::onresetdevice);

        memset(&SwapChainDesc, 0, sizeof(DXGI_SWAP_CHAIN_DESC));
    }
    //=============================================================================================================
    CGame10::~CGame10()
    {
        // ...
        posteffects.clear();
        effects.clear();

		ShutDown();
    }
    //=============================================================================================================
    bool CGame10::Initialize()
    {
        if( Graphics )
            return true;
				
		HRESULT hr;
		HWND hwnd;
		bool success = false;
		ID3D10Texture2D* backbuffer;
		D3D10_VIEWPORT vp;

		long width = (long)DisplayMode.Width;
		long height = (long)DisplayMode.Height;

        success = Application.Initialize(width, height, FullScreen);
        dassert(false, "CGame10::Initialize(): Could not initialize application", success);

		hwnd = Application.GetWindowHandle();

		SwapChainDesc.BufferDesc          = DisplayMode;
		SwapChainDesc.BufferDesc.Width    = width;
		SwapChainDesc.BufferDesc.Height   = height;
		SwapChainDesc.BufferCount         = 1;
		SwapChainDesc.BufferUsage         = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		SwapChainDesc.OutputWindow        = hwnd;
		SwapChainDesc.SampleDesc.Count    = 1;
		SwapChainDesc.SampleDesc.Quality  = 0;
		SwapChainDesc.Windowed            = !FullScreen;
		
		UINT flags = 0;

#ifdef _DEBUG
		flags |= D3D10_CREATE_DEVICE_DEBUG;
#endif

		//D3D10_DRIVER_TYPE_REFERENCE

		hr = D3D10CreateDeviceAndSwapChain(NULL, D3D10_DRIVER_TYPE_HARDWARE, NULL, flags,
			D3D10_SDK_VERSION, &SwapChainDesc, &SwapChain, &Graphics);

        dnassert(false, "CGame10::Initialize(): Could not create device", FAILED(hr));

		hr = SwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), (void**)&backbuffer);
		dnassert(false, "CGame10::Initialize(): Could not get backbuffer", FAILED(hr));

		hr = Graphics->CreateRenderTargetView(backbuffer, NULL, &RenderTargetView);

		backbuffer->Release();
		dnassert(false, "CGame10::Initialize(): Could not create render target view", FAILED(hr));

		// zbuffer
		if( ZBuffer )
		{
			D3D10_TEXTURE2D_DESC depthdesc;
			memset(&depthdesc, 0, sizeof(D3D10_TEXTURE2D_DESC));

			depthdesc.Width               = width;
			depthdesc.Height              = height;
			depthdesc.MipLevels           = 1;
			depthdesc.ArraySize           = 1;
			depthdesc.Format              = DXGI_FORMAT_D32_FLOAT;
			depthdesc.SampleDesc.Count    = 1;
			depthdesc.SampleDesc.Quality  = 0;
			depthdesc.Usage               = D3D10_USAGE_DEFAULT;
			depthdesc.BindFlags           = D3D10_BIND_DEPTH_STENCIL;

			hr = Graphics->CreateTexture2D(&depthdesc, NULL, &DepthStencil);
			dnassert(false, "CGame10::Initialize(): Could not create depth buffer", FAILED(hr));

			D3D10_DEPTH_STENCIL_VIEW_DESC dsv;
			memset(&dsv, 0, sizeof(D3D10_DEPTH_STENCIL_VIEW_DESC));
			
			dsv.Format = depthdesc.Format;
			dsv.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
			dsv.Texture2D.MipSlice = 0;

			hr = Graphics->CreateDepthStencilView(DepthStencil, &dsv, &DepthStencilView);
			dnassert(false, "CGame10::Initialize(): Could not create depth stencil view", FAILED(hr));
		}

		Graphics->OMSetRenderTargets(1, &RenderTargetView, DepthStencilView);

		vp.Width = width;
		vp.Height = height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;

		Graphics->RSSetViewports(1, &vp);

		// ...

        if( fixedtimestep )
            Sync.synchronize.connect(this, &CGame10::Update);

        return true;
    }
    //=============================================================================================================
    bool CGame10::LoadContent()
    {
		// ...
        return true;
    }
    //=============================================================================================================
    void CGame10::UnloadContent()
    {
		// ...
    }
	//=============================================================================================================
	void CGame10::ResetRenderStates()
	{
	}
    //=============================================================================================================
    void CGame10::Update()
    {
		// ...
    }
    //=============================================================================================================
    void CGame10::Draw()
    {
		// ...
    }
    //=============================================================================================================
    bool CGame10::Validate()
    {
        return true;
    }
    //=============================================================================================================
    void CGame10::Run()
    {
        bool success = Initialize();
        dassert(, "CGame10::Run(): Initialization failed", success);

        success = LoadContent();
        dassert(, "CGame10::Run(): Content creation failed", success);

		dsad("Most of the features are not yet implemented");
		ResetRenderStates();

		Sync.Timer().Start();
        Application.Show();
        
        valid = false;

        while( Application.Run() )
        {
            if( Application.Active() )
            {
                if( !valid )
                {
                    valid = Validate();
                }
                else
                {
                    if( fixedtimestep )
                        Sync.Update();
                    else
                        Update();

                    Draw();
                }
            }
            else
            {
                valid = false;
                Sleep(100);
            }
        }

        UnloadContent();
    }
    //=============================================================================================================
    bool CGame10::Lose()
    {
        devicelost = true;
        lost();

		// ...

        return true;
    }
    //=============================================================================================================
    void CGame10::ShutDown()
    {
		if( Graphics )
			Graphics->ClearState();

		safe_release(DepthStencil);
		safe_release(DepthStencilView);
		safe_release(RenderTargetView);
		safe_release(SwapChain);

		if( Graphics )
		{
			ULONG rc = Graphics->Release();

			if( rc > 0 )
            {
                std::stringstream ss;
                ss << "d3ddevice refcount == " << rc;
                dnassert(, ss.str(), true);
            }
            else
            {
                dhappy("D3D device released successfully");
                Graphics = NULL;
            }
		}
    }
    //=============================================================================================================
    void CGame10::onactivate()
    {
        Sync.Timer().Start();
    }
    //=============================================================================================================
    void CGame10::ondeactivate()
    {
        Sync.Timer().Stop();
    }
    //=============================================================================================================
    void CGame10::onlostdevice()
    {
		// ...
        valid = false;
    }
    //=============================================================================================================
    void CGame10::onresetdevice()
    {
		// ...
        ResetRenderStates();
    }
    //=============================================================================================================
}

