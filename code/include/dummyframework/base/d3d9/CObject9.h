//=============================================================================================================
#ifndef _COBJECT9_H_
#define _COBJECT9_H_

#include "../CTransformable.h"
#include <vector>

namespace DummyFramework
{
	class CStateManager9;
	class CGame9;

	/**
	 * \brief Represents a mesh part
	 */
    class CSubset9
    {
    public:
        LPD3DXEFFECT Material;
        LPDIRECT3DBASETEXTURE9 Textures[8];

        CSubset9()
        {
            memset(Textures, 0, 8 * sizeof(LPDIRECT3DBASETEXTURE9));
            Material = NULL;
        }
    };

	/**
	 * \brief Simple game object
	 *
	 * The CObject9 class encapsulates the common properties
	 * of a game object (meshparts, materials, textures).
	 */
	class CObject9 : public CTransformable
    {
    protected:
        typedef std::vector<CSubset9> subsetlist;

		LPDIRECT3DDEVICE9 device;
		CStateManager9* manager;
        subsetlist subsets;

    public:
        LPD3DXMESH Mesh;
        bool Unique;

        CObject9();
        ~CObject9() {}

		void Initialize(CGame9& mygame);
		void ApplyMaterial(size_t id = 0);
		void UnapplyMaterial(size_t id = 0);
        void Draw();
        void Draw(size_t index);
		void DrawOptimized();

        inline LPD3DXEFFECT& Material(size_t id = 0) {
            Unique = Unique && (id == 0);
            return subsets[id].Material;
        }

        inline LPDIRECT3DBASETEXTURE9& Textures(unsigned char stage, size_t id = 0) {
            return subsets[id].Textures[stage];
        }

        inline void Resize(size_t newsize) {
            subsets.resize(newsize);
        }
    };
}

#endif
//=============================================================================================================
 
