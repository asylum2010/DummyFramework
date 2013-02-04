//=============================================================================================================
#include <dummyframework/base/d3d9/CStateManager9.h>
#include <iostream>

#define _ZERORS_	300

namespace DummyFramework
{
	CStateManager9* CStateManager9::Create(LPDIRECT3DDEVICE9 device)
	{
		CStateManager9* mgr = NULL;

		if( device )
		{
			mgr = new CStateManager9();
			mgr->gfx = device;
		}

		return mgr;
	}
	//=============================================================================================================
	CStateManager9::CStateManager9()
	{
		refcount = 1;

		gfx = NULL;
		lastps = NULL;
		lastvs = NULL;

		Reset();
	}
	//=============================================================================================================
	void CStateManager9::Reset()
	{
		memset(&renderstates, _ZERORS_, sizeof(renderstates));
		memset(&texturestates, _ZERORS_, sizeof(texturestates));
		memset(&samplerstates, _ZERORS_, sizeof(samplerstates));
		memset(textures, 0, sizeof(textures));
	}
	//=============================================================================================================
	ULONG CStateManager9::AddRef()
	{
		return (ULONG)InterlockedIncrement(&refcount);
	}
	//=============================================================================================================
	ULONG CStateManager9::Release()
	{
		if( 0L == InterlockedDecrement(&refcount) )
		{
			delete this;
			return 0L;
		}

		return refcount;
	}
	//=============================================================================================================
	HRESULT CStateManager9::QueryInterface(const IID& iid, LPVOID* ppv)
	{
		if( !ppv )
			return E_POINTER;

		// TODO: ?

		if( iid == IID_IUnknown || iid == IID_ID3DXEffectStateManager )
		{
			*ppv = (LPVOID)this;
			AddRef();

			return S_OK;
		}
		else
			*ppv = NULL;

		return E_NOINTERFACE;
	}
	//=============================================================================================================
	HRESULT CStateManager9::SetPixelShader(LPDIRECT3DPIXELSHADER9 ps)
	{
		if( lastps != ps )
		{
			lastps = ps;
			gfx->SetPixelShader(ps);
		}

		return S_OK;
	}
	//=============================================================================================================
	HRESULT CStateManager9::SetRenderState(D3DRENDERSTATETYPE state, DWORD value)
	{
		if( renderstates[state] != value )
		{
			renderstates[state] = value;
			gfx->SetRenderState(state, value);
		}

		return S_OK;
	}
	//=============================================================================================================
	HRESULT CStateManager9::SetSamplerState(DWORD sampler, D3DSAMPLERSTATETYPE type, DWORD value)
	{
		if( sampler > 256 )
			gfx->SetSamplerState(sampler, type, value);
		else if( samplerstates[sampler][type] != value )
		{
			samplerstates[sampler][type] = value;
			gfx->SetSamplerState(sampler, type, value);
		}

		return S_OK;
	}
	//=============================================================================================================
	HRESULT CStateManager9::SetTexture(DWORD stage, LPDIRECT3DBASETEXTURE9 texture)
	{
		if( textures[stage] != texture )
		{
			textures[stage] = texture;
			gfx->SetTexture(stage, texture);
		}

		return S_OK;
	}
	//=============================================================================================================
	HRESULT CStateManager9::SetTextureStageState(DWORD stage, D3DTEXTURESTAGESTATETYPE type, DWORD value)
	{
		if( texturestates[stage][type] != value )
		{
			texturestates[stage][type] = value;
			gfx->SetTextureStageState(stage, type, value);
		}

		return S_OK;
	}
	//=============================================================================================================
	HRESULT CStateManager9::SetVertexShader(LPDIRECT3DVERTEXSHADER9 vs)
	{
		if( lastvs != vs )
		{
			lastvs = vs;
			gfx->SetVertexShader(vs);
		}

		return S_OK;
	}
	//=============================================================================================================
}
