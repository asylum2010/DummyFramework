//=============================================================================================================
#include <dummyframework/base/d3d9/CScreenQuad9.h>
#include <dummyframework/template/defines.h>

namespace DummyFramework
{
	LPDIRECT3DVERTEXDECLARATION9 CScreenQuad9::Declaration = NULL;

	CScreenQuad9::CScreenQuad9(LPDIRECT3DDEVICE9 gfx)
	{
		device = gfx;

		positions[0] = D3DXVECTOR4(0, 0, 0, 1);
		positions[1] = D3DXVECTOR4(1, 0, 0, 1);
		positions[2] = D3DXVECTOR4(1, 1, 0, 1);
		positions[3] = D3DXVECTOR4(0, 1, 0, 1);

		vertices[0].u = 0.0f; vertices[1].u = 1.0f;
		vertices[2].u = 1.0f; vertices[3].u = 0.0f;
		vertices[0].v = 0.0f; vertices[1].v = 0.0f;
		vertices[2].v = 1.0f; vertices[3].v = 1.0f;

		indices[0] = 3;  indices[3] = 3;
		indices[1] = 0;  indices[4] = 1;
		indices[2] = 1;  indices[5] = 2;

		onresetdevice();
	}

	CScreenQuad9::~CScreenQuad9()
	{
		safe_release(Declaration);
	}

	void CScreenQuad9::Draw(const D3DXMATRIX& transform, DWORD color)
	{
		D3DXVec4Transform(&vertices[0].position, &positions[0], &transform);
		D3DXVec4Transform(&vertices[1].position, &positions[1], &transform);
		D3DXVec4Transform(&vertices[2].position, &positions[2], &transform);
		D3DXVec4Transform(&vertices[3].position, &positions[3], &transform);

		vertices[0].color = vertices[1].color = color;
		vertices[2].color = vertices[3].color = color;

		vertices[0].u = 0.0f; vertices[1].u = 1.0f;
		vertices[2].u = 1.0f; vertices[3].u = 0.0f;
		vertices[0].v = 0.0f; vertices[1].v = 0.0f;
		vertices[2].v = 1.0f; vertices[3].v = 1.0f;

		device->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 4, 2, indices,
			D3DFMT_INDEX16, vertices, sizeof(sscreenvertex));
	}

	void CScreenQuad9::Draw(const D3DXMATRIX& transform, float texcoords[4], DWORD color)
	{
		D3DXVec4Transform(&vertices[0].position, &positions[0], &transform);
		D3DXVec4Transform(&vertices[1].position, &positions[1], &transform);
		D3DXVec4Transform(&vertices[2].position, &positions[2], &transform);
		D3DXVec4Transform(&vertices[3].position, &positions[3], &transform);

		vertices[0].color = vertices[1].color = color;
		vertices[2].color = vertices[3].color = color;

		vertices[0].u = vertices[3].u = texcoords[0];
		vertices[0].v = vertices[1].v = texcoords[1];
		vertices[1].u = vertices[2].u = texcoords[2];
		vertices[2].v = vertices[3].v = texcoords[3];

		device->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 4, 2, indices,
			D3DFMT_INDEX16, vertices, sizeof(sscreenvertex));
	}

	void CScreenQuad9::onlostdevice()
	{
		// ??? kell? nemkell? MIIIIIIIII
		safe_release(Declaration);
	}

	void CScreenQuad9::onresetdevice()
	{
		D3DVERTEXELEMENT9 decl[] =
		{
			{ 0,  0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITIONT, 0 },
			{ 0, 16, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
			{ 0, 20, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
			D3DDECL_END()
		};

		if( !Declaration )
		{
			HRESULT hr = device->CreateVertexDeclaration(decl, &Declaration);
			dnassert(, "CScreenQuad9::onresetdevice(): Could not create vertex declaration", FAILED(hr));
		}
	}

	CDynamicQuadBuffer9::CDynamicQuadBuffer9()
	{
		vertexbuffer = NULL;
		indexbuffer = NULL;
		device = NULL;
		vdata = NULL;
		canbedynamic = false;
	}
	//=============================================================================================================
	CDynamicQuadBuffer9::~CDynamicQuadBuffer9()
	{
		safe_release(vertexbuffer);
		safe_release(indexbuffer);
	}
	//=============================================================================================================
	bool CDynamicQuadBuffer9::Initialize(LPDIRECT3DDEVICE9 gfx, unsigned int quadnum)
	{
		device = gfx;
		quads = quadnum;

		D3DCAPS9 caps;
		HRESULT hr;

		hr = device->GetDeviceCaps(&caps);
		dnassert(false, "CDynamicQuadBuffer9: Could not get device caps", FAILED(hr));

		if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT &&
			caps.VertexShaderVersion >= D3DVS_VERSION(1, 1) )
		{
			canbedynamic = true;
		}

		DWORD usage = D3DUSAGE_SOFTWAREPROCESSING|D3DUSAGE_WRITEONLY;
		D3DPOOL pool = D3DPOOL_SYSTEMMEM;

		if( canbedynamic )
		{
			usage = D3DUSAGE_WRITEONLY;
			pool = D3DPOOL_MANAGED;
		}

		if( quads * 4 > 0xffff )
			hr = device->CreateIndexBuffer(quads * 6 * sizeof(unsigned int), 0, D3DFMT_INDEX32, D3DPOOL_MANAGED, &indexbuffer, NULL);
		else
			hr = device->CreateIndexBuffer(quads * 6 * sizeof(unsigned short), usage, D3DFMT_INDEX16, pool, &indexbuffer, NULL);

		dnassert(false, "CDynamicQuadBuffer9: Could not create index buffer", FAILED(hr));

		unsigned short* idata = NULL;
		unsigned short* indices;

		hr = indexbuffer->Lock(0, 0, (void**)&idata, 0);
		dnassert(false, "CDynamicQuadBuffer9: Could not lock index buffer", FAILED(hr));

		for( unsigned short i = 0; i < quads; ++i )
		{
			indices = idata + i * 6;

			indices[0] = indices[3] = i * 4 + 3;
			indices[2] = indices[4] = i * 4 + 1;  
			indices[1] = i * 4 + 0;
			indices[5] = i * 4 + 2;
		}

		indexbuffer->Unlock();
		onresetdevice();

		return (vertexbuffer != NULL);
	}
	//=============================================================================================================
	void CDynamicQuadBuffer9::Lock()
	{
		vertexbuffer->Lock(0, 0, (void**)&vdata, (canbedynamic ? D3DLOCK_DISCARD : 0));
	}
	//=============================================================================================================
	void CDynamicQuadBuffer9::Lock(UINT start, DWORD flags)
	{
		vertexbuffer->Lock(start * sizeof(sscreenvertex) * 4, 0, (void**)&vdata, flags);
	}
	//=============================================================================================================
	void CDynamicQuadBuffer9::Unlock()
	{
		vertexbuffer->Unlock();
		vdata = NULL;
	}
	//=============================================================================================================
	void CDynamicQuadBuffer9::Draw(size_t count)
	{
		if( count != 0 && count <= quads )
		{
			device->SetStreamSource(0, vertexbuffer, 0, sizeof(sscreenvertex));
			device->SetIndices(indexbuffer);
			device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, (UINT)count * 4, 0, (UINT)count * 2);
		}
	}
	//=============================================================================================================
	void CDynamicQuadBuffer9::WriteQuad(size_t index, const D3DXVECTOR2& position, const D3DXVECTOR2& size,
		float texcoords[4], unsigned int color, bool flipuv)
	{
		dnerror(, "CDynamicQuadBuffer9::WriteQuad(): Buffer overflow", index >= quads);
		sscreenvertex* vertices = vdata + index * 4;

		vertices[0].position.x = vertices[3].position.x = position.x - 0.5f;
		vertices[0].position.y = vertices[1].position.y = position.y - 0.5f;
		vertices[1].position.x = vertices[2].position.x = position.x + size.x - 0.5f;
		vertices[2].position.y = vertices[3].position.y = position.y + size.y - 0.5f;

		vertices[0].position.z = vertices[1].position.z = vertices[2].position.z = vertices[3].position.z = 0;
		vertices[0].position.w = vertices[1].position.w = vertices[2].position.w = vertices[3].position.w = 1;
		vertices[0].color = vertices[1].color = vertices[2].color = vertices[3].color = color;

		if( flipuv )
		{
			vertices[0].u = vertices[1].u = texcoords[0];
			vertices[0].v = vertices[3].v = texcoords[1];
			vertices[1].v = vertices[2].v = texcoords[2];
			vertices[2].u = vertices[3].u = texcoords[3];
		}
		else
		{
			vertices[0].u = vertices[3].u = texcoords[0];
			vertices[0].v = vertices[1].v = texcoords[1];
			vertices[1].u = vertices[2].u = texcoords[2];
			vertices[2].v = vertices[3].v = texcoords[3];
		}
	}
	//=============================================================================================================
	void CDynamicQuadBuffer9::WriteQuad(size_t index, D3DXVECTOR2 positions[4], float texcoords[4],
		unsigned int color, bool flipuv)
	{
		dnerror(, "CDynamicQuadBuffer9::WriteQuad(): Buffer overflow", index >= quads);
		sscreenvertex* vertices = vdata + index * 4;

		vertices[0].position.x = positions[0].x - 0.5f;
		vertices[1].position.x = positions[1].x - 0.5f;
		vertices[2].position.x = positions[2].x - 0.5f;
		vertices[3].position.x = positions[3].x - 0.5f;

		vertices[0].position.y = positions[0].y - 0.5f;
		vertices[1].position.y = positions[1].y - 0.5f;
		vertices[2].position.y = positions[2].y - 0.5f;
		vertices[3].position.y = positions[3].y - 0.5f;

		vertices[0].position.z = vertices[1].position.z = vertices[2].position.z = vertices[3].position.z = 0;
		vertices[0].position.w = vertices[1].position.w = vertices[2].position.w = vertices[3].position.w = 1;
		vertices[0].color = vertices[1].color = vertices[2].color = vertices[3].color = color;

		if( flipuv )
		{
			vertices[0].u = vertices[1].u = texcoords[0];
			vertices[0].v = vertices[3].v = texcoords[1];
			vertices[1].v = vertices[2].v = texcoords[2];
			vertices[2].u = vertices[3].u = texcoords[3];
		}
		else
		{
			vertices[0].u = vertices[3].u = texcoords[0];
			vertices[0].v = vertices[1].v = texcoords[1];
			vertices[1].u = vertices[2].u = texcoords[2];
			vertices[2].v = vertices[3].v = texcoords[3];
		}
	}
	//=============================================================================================================
	void CDynamicQuadBuffer9::onlostdevice()
	{
		safe_release(vertexbuffer);
	}
	//=============================================================================================================
	void CDynamicQuadBuffer9::onresetdevice()
	{
		DWORD usage = D3DUSAGE_SOFTWAREPROCESSING;
		D3DPOOL pool = D3DPOOL_SYSTEMMEM;

		if( canbedynamic )
		{
			usage = D3DUSAGE_DYNAMIC|D3DUSAGE_WRITEONLY;
			pool = D3DPOOL_DEFAULT;
		}

		HRESULT hr = device->CreateVertexBuffer(quads * 4 * sizeof(sscreenvertex), usage, sscreenvertex::fvf, pool, &vertexbuffer, NULL);
		dnassert(, "CDynamicQuadBuffer9: Could not create vertex buffer", FAILED(hr));
	}
	//=============================================================================================================
}
