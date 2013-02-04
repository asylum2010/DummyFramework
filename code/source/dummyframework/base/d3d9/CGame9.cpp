//=============================================================================================================
#include <dummyframework/base/d3d9/CGame9.h>
#include <dummyframework/base/d3d9/CEffectRenderer9.h>
#include <dummyframework/base/CHelper.h>
#include <sstream>

namespace DummyFramework
{
	CGame9::CGame9()
	{
		Direct3D		= NULL;
		Graphics		= NULL;
		FullScreen		= false;
		VSync			= false;
		ZBuffer			= true;
		Stencil			= true;
		MultiThreaded	= false;
		RenderDeactive	= false;
		StateManager	= NULL;

		screenquad		= NULL;
		resetcounter	= 0;
		fixedtimestep	= true;
		devicelost		= false;

		DisplayMode.Format		= D3DFMT_UNKNOWN;
		DisplayMode.RefreshRate	= D3DPRESENT_RATE_DEFAULT;
		DisplayMode.Width		= 1024;
		DisplayMode.Height		= 768;

		Application.activated.connect(this, &CGame9::onactivate);
		Application.deactivated.connect(this, &CGame9::ondeactivate);

		lost.connect(this, &CGame9::onlostdevice);
		reset.connect(this, &CGame9::onresetdevice);

		memset(&PresentationParameters, 0, sizeof(D3DPRESENT_PARAMETERS));
		memset(&DeviceCaps, 0, sizeof(D3DCAPS9));
	}
	//=============================================================================================================
	CGame9::~CGame9()
	{
		for( rendererlist::iterator it = posteffects.begin(); it != posteffects.end(); ++it )
			safe_delete(*it);

		for( rendererlist::iterator it = effects.begin(); it != effects.end(); ++it )
			safe_delete(*it);

		posteffects.clear();
		effects.clear();

		Content.Clear();
		ShutDown();
	}
	//=============================================================================================================
	bool CGame9::Initialize()
	{
		if( Graphics )
			return true;

		if( !Direct3D )
		{
			Direct3D = Direct3DCreate9(D3D_SDK_VERSION);
			dassert(false, "CGame9::Initialize(): Could not create direct3d", Direct3D);
		}
		
		D3DDISPLAYMODE	mode;
		HWND			hwnd = NULL;
		HRESULT			hr;
		bool			success = false;

		hr = Direct3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mode);
		dassert(false, "CGame9::Initialize(): Could not get adapter display mode", SUCCEEDED(hr));

		hr = Direct3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps);
		dassert(false, "CGame9::Initialize(): Could not get device caps", SUCCEEDED(hr));

		// check stencil availability
		if( !(DeviceCaps.StencilCaps & D3DSTENCILCAPS_DECRSAT) ||
			!(DeviceCaps.StencilCaps & D3DSTENCILCAPS_INCRSAT) ||
			!(DeviceCaps.StencilCaps & D3DSTENCILCAPS_KEEP) )
		{
			dsad("Stencil buffer not supported");
			Stencil = false;
		}

		D3DFORMAT disp = mode.Format;
		success = false;

		// requested format
		if( DisplayMode.Format != D3DFMT_UNKNOWN )
		{
			PresentationParameters.BackBufferFormat = DisplayMode.Format;
			success = FindBestDepthStencil(disp, DisplayMode.Format, PresentationParameters.AutoDepthStencilFormat);
		}

		// look for a new one
		if( !success )
		{
			success = FindBestFormats(disp, PresentationParameters.BackBufferFormat, PresentationParameters.AutoDepthStencilFormat);

			if( !success )
			{
				dsad("CGame9::Initialize(): Could not find suitable depth format");
				ZBuffer = Stencil = false;
			}
		}

		// now init
		int width = (int)DisplayMode.Width;
		int height = (int)DisplayMode.Height;

		success = Application.Initialize(width, height, FullScreen);
		dassert(false, "CGame9::Initialize(): Could not initialize application", success);

		if( !PresentationParameters.hDeviceWindow )
		{
			hwnd = Application.GetWindowHandle();
			PresentationParameters.hDeviceWindow = hwnd;
			PresentationParameters.Windowed = !FullScreen;
		}
		else
		{
			hwnd = PresentationParameters.hDeviceWindow;
			PresentationParameters.Windowed = true;
		}
		
		std::cout << "Resolution:            " << DisplayMode.Width << "x" << DisplayMode.Height << " (" << width << "x" << height << ")\n";
		std::cout << "Display format:        " << CHelper::FormatToString(disp) << "\n";
		std::cout << "Backbuffer format:     " << CHelper::FormatToString(PresentationParameters.BackBufferFormat) << "\n";
		std::cout << "Depth/stencil format:  " << CHelper::FormatToString(PresentationParameters.AutoDepthStencilFormat) << "\n";
		std::cout << "\n";

		PresentationParameters.BackBufferHeight				= (FullScreen ? height : 0);
		PresentationParameters.BackBufferWidth				= (FullScreen ? width : 0);
		PresentationParameters.FullScreen_RefreshRateInHz	= (FullScreen ? DisplayMode.RefreshRate : 0);
		PresentationParameters.BackBufferCount				= 1;
		PresentationParameters.EnableAutoDepthStencil		= ZBuffer;
		PresentationParameters.PresentationInterval			= (VSync ? D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE); 
		PresentationParameters.MultiSampleType				= D3DMULTISAMPLE_NONE;
		PresentationParameters.MultiSampleQuality			= 0;
		PresentationParameters.SwapEffect					= D3DSWAPEFFECT_DISCARD;
		
		DWORD flags = (MultiThreaded ? D3DCREATE_MULTITHREADED : 0);

		if( DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
		{
			if( DeviceCaps.VertexShaderVersion < D3DVS_VERSION(1, 1) )
			{
				dsad("Vertex shading not supported");

				hr = Direct3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,
						flags|D3DCREATE_MIXED_VERTEXPROCESSING, &PresentationParameters, &Graphics);
			}
			else
			{
				hr = Direct3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,
						flags|D3DCREATE_HARDWARE_VERTEXPROCESSING, &PresentationParameters, &Graphics);
			}
		}
		else
		{
			dsad("Hardware vertex processing not supported");

			hr = Direct3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,
					flags|D3DCREATE_SOFTWARE_VERTEXPROCESSING, &PresentationParameters, &Graphics);
		}

		dnassert(false, "CGame9::Initialize(): Could not create device", FAILED(hr));

		if( DeviceCaps.NumSimultaneousRTs <= 1 )
			dsad("Multiple render targets not supported");

		if( DeviceCaps.MaxVertexIndex <= 0x0000ffff )
			dsad("32-bit index buffers not supported");

		if( PresentationParameters.BackBufferWidth == 0 || PresentationParameters.BackBufferHeight == 0 )
		{
			RECT rect;
			GetClientRect(hwnd, &rect);

			PresentationParameters.BackBufferWidth = rect.right - rect.left;
			PresentationParameters.BackBufferHeight = rect.bottom - rect.top;
		}

		screenquad = new CScreenQuad9(Graphics);
		success = Content.Initialize(Graphics);
		dassert(false, "CGame9::Initialize(): Could not initialize content", success);

		for( rendererlist::iterator it = effects.begin(); it != effects.end(); ++it )
		{
			if( !(*it)->Initialize(*this) )
				(*it)->Enabled = false;
		}

		for( rendererlist::iterator it = posteffects.begin(); it != posteffects.end(); ++it )
		{
			if( !(*it)->Initialize(*this) )
				(*it)->Enabled = false;
		}

		if( fixedtimestep )
			Sync.synchronize.connect(this, &CGame9::Update);

		StateManager = Content.GetStateManager();
		return true;
	}
	//=============================================================================================================
	bool CGame9::LoadContent()
	{
		for( rendererlist::iterator it = effects.begin(); it != effects.end(); ++it )
		{
			if( !(*it)->LoadContent() )
				(*it)->Enabled = false;
		}

		for( rendererlist::iterator it = posteffects.begin(); it != posteffects.end(); ++it )
		{
			if( !(*it)->LoadContent() )
				(*it)->Enabled = false;
		}

		return true;
	}
	//=============================================================================================================
	void CGame9::UnloadContent()
	{
		for( rendererlist::iterator it = effects.begin(); it != effects.end(); ++it )
		{
			if( *it )
				(*it)->UnloadContent();
		}

		for( rendererlist::iterator it = posteffects.begin(); it != posteffects.end(); ++it )
		{
			if( *it )
				(*it)->UnloadContent();
		}

		Content.Clear();
	}
	//=============================================================================================================
	void CGame9::ResetRenderStates()
	{
		for( rendererlist::iterator it = effects.begin(); it != effects.end(); ++it )
		{
			if( *it )
				(*it)->ResetRenderStates();
		}

		for( rendererlist::iterator it = posteffects.begin(); it != posteffects.end(); ++it )
		{
			if( *it )
				(*it)->ResetRenderStates();
		}
	}
	//=============================================================================================================
	void CGame9::Update()
	{
		for( rendererlist::iterator it = effects.begin(); it != effects.end(); ++it )
			(*it)->Update();

		for( rendererlist::iterator it = posteffects.begin(); it != posteffects.end(); ++it )
			(*it)->Update();
	}
	//=============================================================================================================
	void CGame9::Draw()
	{
		for( DWORD i = 0; i < 8; ++i )
			StateManager->SetTexture(i, NULL);

		StateManager->SetRenderState(D3DRS_ZENABLE, FALSE);

		for( rendererlist::iterator it = posteffects.begin(); it != posteffects.end(); ++it )
		{
			if( (*it)->Enabled )
				(*it)->Draw();
		}

		StateManager->SetRenderState(D3DRS_ZENABLE, TRUE);
	}
	//=============================================================================================================
	bool CGame9::Validate()
	{
		HRESULT hr = Graphics->TestCooperativeLevel();

		if( hr == D3DERR_DEVICELOST )
		{
			dsad("Device lost");
			dsad("Release all of your unmanaged resources");

			Lose();
		}
		else if( hr == D3DERR_DEVICENOTRESET )
		{
			if( Reset() )
			{
				dhappy("Device reset");
				dhappy("Query unmanaged resources from the content manager");
				dhappy("Reset render states");

				resetcounter = 0;
			}
			else
			{
				++resetcounter;

				if( resetcounter > 5 )
				{
					dsad("Number of tries exceeded the limit");
					Application.Close();
				}
			}
		}
		else if( hr == D3DERR_DRIVERINTERNALERROR )
		{
			MessageBoxA(NULL, "Driver internal error. The application will now quit.\n"
				"Contact manufacturer for more information.", "Fatal Error", MB_OK);
			exit(1);
		}

		return SUCCEEDED(hr);
	}
	//=============================================================================================================
	void CGame9::Run()
	{
		bool success = Initialize();

		if( !success )
		{
			dsad("CGame9::Run(): Initialization failed");
			goto _unload;
		}

		success = LoadContent();

		if( !success )
		{
			dsad("CGame9::Run(): Content creation failed");
			goto _unload;
		}

		ResetRenderStates();

		Sync.Timer().Start();
		Application.Show();
		
		valid = false;

		while( Application.Run() )
		{
			if( Application.Active() || RenderDeactive )
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

	_unload:
		UnloadContent();
	}
	//=============================================================================================================
	bool CGame9::Lose()
	{
		devicelost = true;
		screenquad->onlostdevice();
		lost();

		return true;
	}
	//=============================================================================================================
	bool CGame9::Reset()
	{
		bool stopped = Sync.Timer().IsStopped();
		Sync.Timer().Stop();
		
		D3DPRESENT_PARAMETERS old;
		D3DDISPLAYMODE mode;
		HRESULT hr;

		memcpy(&old, &PresentationParameters, sizeof(D3DPRESENT_PARAMETERS));

		if( old.Windowed && FullScreen )
			Application.Hide();

		if( !devicelost )
			Lose();

		hr = Direct3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mode);
		dassert(false, "CGame9::Reset(): Could not get adapter display mode", SUCCEEDED(hr));

		D3DFORMAT disp = mode.Format;
		bool success = false;

		// a formátum már ismert
		if( DisplayMode.Format != D3DFMT_UNKNOWN )
		{
			PresentationParameters.BackBufferFormat = DisplayMode.Format;
			success = FindBestDepthStencil(disp, DisplayMode.Format, PresentationParameters.AutoDepthStencilFormat);
		}

		// ujat kell keresni
		if( !success )
		{
			success = FindBestFormats(disp, PresentationParameters.BackBufferFormat, PresentationParameters.AutoDepthStencilFormat);

			if( !success )
			{
				dsad("CGame9::Reset(): Could not find suitable depth format");
				ZBuffer = Stencil = false;
			}
		}

		int width = (int)DisplayMode.Width;
		int height = (int)DisplayMode.Height;

		Application.Resize(width, height, FullScreen);
        
		PresentationParameters.EnableAutoDepthStencil		= ZBuffer;
		PresentationParameters.BackBufferHeight				= (FullScreen ? height : 0);
		PresentationParameters.BackBufferWidth				= (FullScreen ? width : 0);
		PresentationParameters.FullScreen_RefreshRateInHz	= (FullScreen ? DisplayMode.RefreshRate : 0);
		PresentationParameters.Windowed						= !FullScreen;
		PresentationParameters.PresentationInterval			= (VSync ? D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE); 
		PresentationParameters.SwapEffect					= D3DSWAPEFFECT_DISCARD;

		hr = Graphics->Reset(&PresentationParameters);

		if( FAILED(hr) )
		{
			dsad("CGame9::Reset(): Reverting to previous");

			hr = Graphics->Reset(&old);
			dnassert(false, "CGame9::Reset(): Device reset failed", FAILED(hr));

			memcpy(&PresentationParameters, &old, sizeof(D3DPRESENT_PARAMETERS));

			// TODO: inkonzisztens lesz (pl. fullscreen, width, height, stb.)
		}

		if( PresentationParameters.BackBufferWidth == 0 || PresentationParameters.BackBufferHeight == 0 )
		{
			RECT rect;
			GetClientRect(Application.GetWindowHandle(), &rect);

			PresentationParameters.BackBufferWidth = rect.right - rect.left;
			PresentationParameters.BackBufferHeight = rect.bottom - rect.top;
		}

		// renderstateket NEKED kell visszaállitani!
		devicelost = false;
		screenquad->onresetdevice();
		reset();

		ResetRenderStates();
		dhappy("Device reset succeeded");

		if( old.Windowed && FullScreen )
			Application.Show();

		if( !stopped )
			Sync.Timer().Start();

		return true;
	}
	//=============================================================================================================
	void CGame9::ShutDown()
	{
		if( Graphics )
		{
			if( screenquad )
			{
				delete screenquad;
				screenquad = NULL;
			}

			ULONG rc = Graphics->Release();

			// ha ez végrehajtódik, akkor valamit nem releaseltél
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

		safe_release(Direct3D);
	}
	//=============================================================================================================
	void CGame9::EnumDisplayModes(resolutionset& out)
	{
		derror(, "CGame9::EnumDisplayModes(): NULL == Direct3D", Direct3D);

		D3DDISPLAYMODE mode;
		HRESULT hr;

		std::string str;
		std::stringstream ss;

		if( DisplayMode.Format == D3DFMT_UNKNOWN )
		{
			hr = Direct3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mode);
			dassert(, "CGame9::EnumDisplayModes(): Could not get adapter display mode", SUCCEEDED(hr));

			DisplayMode.Format = mode.Format;
		}

		DWORD cnt = Direct3D->GetAdapterModeCount(D3DADAPTER_DEFAULT, DisplayMode.Format);
		derror(, "CGame9::EnumDisplayModes(): No display modes for given format", cnt);

		out.clear();

		for( DWORD i = 0; i < cnt; ++i )
		{
			Direct3D->EnumAdapterModes(D3DADAPTER_DEFAULT, DisplayMode.Format, i, &mode);

			ss.clear();
			ss.str("");

			ss << mode.Width << "x" << mode.Height;
			out.insert(ss.str());
		}
	}
	//=============================================================================================================
	bool CGame9::FindBestDepthStencil(D3DFORMAT disp, D3DFORMAT back, D3DFORMAT& depth)
	{
		D3DFORMAT depths[] =
		{
			D3DFMT_D32,
			D3DFMT_D24S8,
			D3DFMT_D24X8,
			D3DFMT_D24X4S4,
			D3DFMT_D32,
			D3DFMT_D16,
			D3DFMT_UNKNOWN,

			D3DFMT_D16,
			D3DFMT_D15S1,
			D3DFMT_D16,
			D3DFMT_UNKNOWN
		};
			
		DWORD i = (Stencil ? 1 : 0);

		for( ; depths[i] != D3DFMT_UNKNOWN; ++i )
		{
			depth = (ZBuffer ? depths[i] : D3DFMT_UNKNOWN);

			if( CheckDepthFormat(disp, back, depth) )
				break;
			else
				depth = D3DFMT_UNKNOWN;
		}

		return (depth != D3DFMT_UNKNOWN);
	}
	//=============================================================================================================
	bool CGame9::FindBestFormats(D3DFORMAT disp, D3DFORMAT& back, D3DFORMAT& depth)
	{
		DWORD startnostencil32 = 0;
		DWORD startnostencil16 = 11;
		DWORD startstencil32 = 2;
		DWORD startstencil16 = 14;

		D3DFORMAT formats[][2] =
		{
			{ D3DFMT_A8R8G8B8, D3DFMT_D32 },
			{ D3DFMT_X8R8G8B8, D3DFMT_D32 },

			{ D3DFMT_A8R8G8B8, D3DFMT_D24S8 },
			{ D3DFMT_A8R8G8B8, D3DFMT_D24X8 },
			{ D3DFMT_A8R8G8B8, D3DFMT_D24X4S4 },

			{ D3DFMT_X8R8G8B8, D3DFMT_D24S8 },
			{ D3DFMT_X8R8G8B8, D3DFMT_D24X8 },
			{ D3DFMT_X8R8G8B8, D3DFMT_D24X4S4 },

			{ D3DFMT_A8R8G8B8, D3DFMT_D16 },
			{ D3DFMT_X8R8G8B8, D3DFMT_D16 },
			{ D3DFMT_UNKNOWN,  D3DFMT_UNKNOWN },

			{ D3DFMT_A1R5G5B5, D3DFMT_D16 },
			{ D3DFMT_X1R5G5B5, D3DFMT_D16 },
			{ D3DFMT_R5G6B5,   D3DFMT_D16 },

			{ D3DFMT_A1R5G5B5, D3DFMT_D15S1 },
			{ D3DFMT_X1R5G5B5, D3DFMT_D15S1 },
			{ D3DFMT_R5G6B5,   D3DFMT_D15S1 },
			{ D3DFMT_UNKNOWN,  D3DFMT_UNKNOWN }
		};
		
		if( FullScreen )
		{
			DWORD i = (Stencil ? startstencil32 : startnostencil32);

			for( ; formats[i][0] != D3DFMT_UNKNOWN; ++i )
			{
				back = formats[i][0];
				depth = (ZBuffer ? formats[i][1] : D3DFMT_UNKNOWN);

				if( CheckDepthFormat(disp, back, depth) )
					break;
				else 
					back = depth = D3DFMT_UNKNOWN;
			}

			if( back == D3DFMT_UNKNOWN )
			{
				back = disp;
				FindBestDepthStencil(disp, back, depth);
			}
		}
		else
		{
			switch (disp)
			{
			case D3DFMT_R8G8B8:
			case D3DFMT_A8R8G8B8:
			case D3DFMT_X8R8G8B8:
				back = D3DFMT_X8R8G8B8;
				break;
				
			default:
				back = disp;
				break;
			}

			FindBestDepthStencil(disp, back, depth);
		}

		return !(ZBuffer && (depth == D3DFMT_UNKNOWN));
	}
	//=============================================================================================================
	bool CGame9::FindBestMultiSample(D3DFORMAT backbuffer, bool fullscreen, D3DMULTISAMPLE_TYPE& type)
	{
		D3DMULTISAMPLE_TYPE sampletypes[] =
		{
			D3DMULTISAMPLE_16_SAMPLES,
			D3DMULTISAMPLE_8_SAMPLES,
			D3DMULTISAMPLE_6_SAMPLES,
			D3DMULTISAMPLE_4_SAMPLES,
			D3DMULTISAMPLE_2_SAMPLES,
			D3DMULTISAMPLE_NONMASKABLE,
			D3DMULTISAMPLE_NONE
		};

		for( DWORD i = 0; sampletypes[i] != D3DMULTISAMPLE_NONE; ++i )
		{
			type = sampletypes[i];

			if( SUCCEEDED(Direct3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, backbuffer, !fullscreen, type, NULL)) )
				break;

			type = D3DMULTISAMPLE_NONE;
		}

		return (type != D3DMULTISAMPLE_NONE);
	}
	//=============================================================================================================
	bool CGame9::CheckDepthFormat(D3DFORMAT disp, D3DFORMAT back, D3DFORMAT depth)
	{
		if( FAILED(Direct3D->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, disp,
			D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, depth)) )
		{
			return false;
		}

		return SUCCEEDED(Direct3D->CheckDepthStencilMatch(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, disp, back, depth));
	}
	//=============================================================================================================
	void CGame9::onactivate()
	{
		if( !RenderDeactive )
			Sync.Timer().Start();
	}
	//=============================================================================================================
	void CGame9::ondeactivate()
	{
		if( !RenderDeactive )
			Sync.Timer().Stop();
	}
	//=============================================================================================================
	void CGame9::onlostdevice()
	{
		for( rendererlist::iterator it = effects.begin(); it != effects.end(); ++it )
			(*it)->onlostdevice();

		for( rendererlist::iterator it = posteffects.begin(); it != posteffects.end(); ++it )
			(*it)->onlostdevice();

		Content.onlostdevice();
		valid = false;
	}
	//=============================================================================================================
	void CGame9::onresetdevice()
	{
		StateManager->Reset();
		Content.onresetdevice();

		for( rendererlist::iterator it = effects.begin(); it != effects.end(); ++it )
			(*it)->onresetdevice();

		for( rendererlist::iterator it = posteffects.begin(); it != posteffects.end(); ++it )
			(*it)->onresetdevice();
		
		ResetRenderStates();
	}
	//=============================================================================================================
}
