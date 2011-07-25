//=============================================================================================================
#ifndef _CSCREENQUAD_H_
#define _CSCREENQUAD_H_

#include "../../template/signal.hpp"
#include <d3dx9.h>

namespace DummyFramework
{
	/**
	 * \brief 2D vertex
	 */
    struct sscreenvertex
    {
        D3DXVECTOR4 position;
        D3DCOLOR color;
        float u, v;

        static const DWORD fvf = D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1;
    };

	/**
	 * \brief Single quad for 2D rendering
	 */
    class CScreenQuad9
    {
    private:
        LPDIRECT3DDEVICE9 device;

        sscreenvertex vertices[4];
        D3DXVECTOR4 positions[4];
        unsigned short indices[6];

    public:
        static LPDIRECT3DVERTEXDECLARATION9 Declaration;

        CScreenQuad9(LPDIRECT3DDEVICE9 gfx);
        ~CScreenQuad9();

        void Draw(const D3DXMATRIX& transform, DWORD color = 0xffffffff);
        void Draw(const D3DXMATRIX& transform, float texcoords[4], DWORD color);

    eventhandlers:
        void onlostdevice();
        void onresetdevice();
    };

	/**
	 * \brief Dynamic quad buffer
	 */
    class CDynamicQuadBuffer9 : public has_slots
    {
    private:
        LPDIRECT3DDEVICE9 device;
        LPDIRECT3DVERTEXBUFFER9 vertexbuffer;
        LPDIRECT3DINDEXBUFFER9 indexbuffer;
        sscreenvertex* vdata;
        unsigned long quads;
		bool canbedynamic;

    public:
        static LPDIRECT3DVERTEXDECLARATION9 Declaration;

        CDynamicQuadBuffer9();
        ~CDynamicQuadBuffer9();

        bool Initialize(LPDIRECT3DDEVICE9 gfx, unsigned long quadnum);
        void Lock();
        void Lock(UINT start, DWORD flags);
        void Unlock();
        void Draw(size_t count);
        void WriteQuad(size_t index, const D3DXVECTOR2& position, const D3DXVECTOR2& size, float texcoords[4], unsigned long color = 0xffffffff, bool flipuv = false);
        void WriteQuad(size_t index, D3DXVECTOR2 positions[4], float texcoords[4], unsigned long color = 0xffffffff, bool flipuv = false);

        inline unsigned long GetQuads() const {
            return quads;
        }

    eventhandlers:
        void onlostdevice();
        void onresetdevice();
    };
}

#endif
//=============================================================================================================
 
