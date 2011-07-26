//=============================================================================================================
#ifndef _CSTATEMANAGER9_H_
#define _CSTATEMANAGER9_H_

#include <d3dx9.h>

namespace DummyFramework
{
	/**
	 * \brief State manager for effects
	 *
	 * The state manager object helps to filter out
	 * redundant renderstate and texture state switches.
	 * However this requires that you use it for these kind
	 * of calls.
	 *
	 * If you use <kbd>D3DXCreateEffectXX()</kbd>, then you have to
	 * set the state manager for it manually, and you have to
	 * use the <kbd>_DUMMY_EFFECT_DONT_SAVE_</kbd> flag when calling
	 * <kbd>ID3DXEffect::Begin()</kbd>.
	 */
	class CStateManager9 : public ID3DXEffectStateManager
	{
	protected:
		LPDIRECT3DDEVICE9 gfx;
		LONG refcount;
		DWORD renderstates[210];
		DWORD texturestates[8][33];
		DWORD samplerstates[8][14];

		LPDIRECT3DPIXELSHADER9 lastps;
		LPDIRECT3DVERTEXSHADER9 lastvs;
		LPDIRECT3DBASETEXTURE9 textures[8];

		CStateManager9();
		virtual ~CStateManager9() {}

	public:
		static CStateManager9* Create(LPDIRECT3DDEVICE9 device);

		void Reset();

		ULONG COM_DECLSPEC_NOTHROW STDMETHODCALLTYPE AddRef();
		ULONG COM_DECLSPEC_NOTHROW STDMETHODCALLTYPE Release();
		HRESULT COM_DECLSPEC_NOTHROW STDMETHODCALLTYPE QueryInterface(const IID& iid, LPVOID* ppv);

		HRESULT COM_DECLSPEC_NOTHROW STDMETHODCALLTYPE LightEnable(DWORD index, BOOL enable) {
			gfx->LightEnable(index, enable);
			return S_OK;
		}

		HRESULT COM_DECLSPEC_NOTHROW STDMETHODCALLTYPE SetFVF(DWORD fvf) {
			gfx->SetFVF(fvf);
			return S_OK;
		}

		HRESULT COM_DECLSPEC_NOTHROW STDMETHODCALLTYPE SetLight(DWORD index, const D3DLIGHT9* light) {
			gfx->SetLight(index, light);
			return S_OK;
		}

		HRESULT COM_DECLSPEC_NOTHROW STDMETHODCALLTYPE SetMaterial(const D3DMATERIAL9* material) {
			gfx->SetMaterial(material);
			return S_OK;
		}

		HRESULT COM_DECLSPEC_NOTHROW STDMETHODCALLTYPE SetNPatchMode(float segments) {
			gfx->SetNPatchMode(segments);
			return S_OK;
		}

		HRESULT COM_DECLSPEC_NOTHROW STDMETHODCALLTYPE SetPixelShader(LPDIRECT3DPIXELSHADER9 ps);

		HRESULT COM_DECLSPEC_NOTHROW STDMETHODCALLTYPE SetPixelShaderConstantB(UINT reg, const BOOL* data, UINT regcount) {
			gfx->SetPixelShaderConstantB(reg, data, regcount);
			return S_OK;
		}

		HRESULT COM_DECLSPEC_NOTHROW STDMETHODCALLTYPE SetPixelShaderConstantF(UINT reg, const FLOAT* data, UINT regcount) {
			gfx->SetPixelShaderConstantF(reg, data, regcount);
			return S_OK;
		}

		HRESULT COM_DECLSPEC_NOTHROW STDMETHODCALLTYPE SetPixelShaderConstantI(UINT reg, const INT* data, UINT regcount) {
			gfx->SetPixelShaderConstantI(reg, data, regcount);
			return S_OK;
		}

		HRESULT COM_DECLSPEC_NOTHROW STDMETHODCALLTYPE SetRenderState(D3DRENDERSTATETYPE state, DWORD value);
		HRESULT COM_DECLSPEC_NOTHROW STDMETHODCALLTYPE SetSamplerState(DWORD sampler, D3DSAMPLERSTATETYPE type, DWORD value);
		HRESULT COM_DECLSPEC_NOTHROW STDMETHODCALLTYPE SetTexture(DWORD stage, LPDIRECT3DBASETEXTURE9 texture);
		HRESULT COM_DECLSPEC_NOTHROW STDMETHODCALLTYPE SetTextureStageState(DWORD stage, D3DTEXTURESTAGESTATETYPE type, DWORD value);

		HRESULT COM_DECLSPEC_NOTHROW STDMETHODCALLTYPE SetTransform(D3DTRANSFORMSTATETYPE state, const D3DMATRIX* matrix) {
			gfx->SetTransform(state, matrix);
			return S_OK;
		}

		HRESULT COM_DECLSPEC_NOTHROW STDMETHODCALLTYPE SetVertexShader(LPDIRECT3DVERTEXSHADER9 vs);

		HRESULT COM_DECLSPEC_NOTHROW STDMETHODCALLTYPE SetVertexShaderConstantB(UINT reg, const BOOL* data, UINT regcount) {
			gfx->SetVertexShaderConstantB(reg, data, regcount);
			return S_OK;
		}

		HRESULT COM_DECLSPEC_NOTHROW STDMETHODCALLTYPE SetVertexShaderConstantF(UINT reg, const FLOAT* data, UINT regcount) {
			gfx->SetVertexShaderConstantF(reg, data, regcount);
			return S_OK;
		}

		HRESULT COM_DECLSPEC_NOTHROW STDMETHODCALLTYPE SetVertexShaderConstantI(UINT reg, const INT* data, UINT regcount) {
			gfx->SetVertexShaderConstantI(reg, data, regcount);
			return S_OK;
		}
	};
}

#endif
//=============================================================================================================
