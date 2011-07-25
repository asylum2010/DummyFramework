//=============================================================================================================
#ifndef _CCAUSTICSRENDERER9_H_
#define _CCAUSTICSRENDERER9_H_

#include "../../base/d3d9/CEffectRenderer9.h"
#include <d3dx9.h>
#include <vector>

namespace DummyFramework
{
    class CCausticsRenderer9 : public CEffectRenderer9
    {
		struct distancemap
		{
			unsigned long id1;
			unsigned long id2;

			LPDIRECT3DCUBETEXTURE9 colors;
			LPDIRECT3DCUBETEXTURE9 distances;
			LPDIRECT3DSURFACE9 surfaces[12];

			distancemap() {
				distances = colors = NULL;
				memset(surfaces, 0, sizeof(surfaces));
			}
		};

    private:
		typedef std::vector<distancemap> distancemaplist;

		LPDIRECT3DSURFACE9 photonsurface;
		LPDIRECT3DSURFACE9 shadowsurface;
		LPDIRECT3DVERTEXDECLARATION9 declaration;
		LPDIRECT3DSURFACE9 causticsurfaces[6];
		LPDIRECT3DVERTEXBUFFER9 vertices;
		LPDIRECT3DINDEXBUFFER9 indices;
		LPD3DXEFFECT effect;

		distancemaplist distancemaps;
		unsigned long id1, id2, id3;
		
		D3DXVECTOR3 look;
        D3DXVECTOR3 viewdir[6];
        D3DXVECTOR3 up[3];

		void DrawColors();
		void DrawDistances();

    public:
		LPDIRECT3DTEXTURE9 PhotonHits;
		LPDIRECT3DTEXTURE9 ShadowMap;
		LPDIRECT3DCUBETEXTURE9 Caustics;

		D3DXMATRIX View;
        D3DXMATRIX Projection;
        D3DXVECTOR3 Position;

		unsigned int DistanceMapCount;
		unsigned int DistanceMapSize;
		unsigned int PhotonHitImageSize;

        CCausticsRenderer9();
        ~CCausticsRenderer9();

        bool Initialize(CGame9& mygame);
        bool LoadContent();

        void UnloadContent();
		void Draw();
		void DrawColorDistance();
		void PhotonTrace();

		inline LPDIRECT3DCUBETEXTURE9 GetDistanceMap(size_t index) {
			return distancemaps[index].distances;
		}

		inline LPDIRECT3DCUBETEXTURE9 GetColorMap(size_t index) {
			return distancemaps[index].colors;
		}

		inline LPD3DXEFFECT GetEffect() {
			return effect;
		}

	events:
		signal0 photontrace;
		signal0 rendershadowmap;
		signal0 rendershadow;

		signal1<size_t> rendercolor;
		signal1<size_t> renderdistance;

	eventhandlers:
		void onlostdevice();
        void onresetdevice();
    };
}

#endif
//=============================================================================================================
 
