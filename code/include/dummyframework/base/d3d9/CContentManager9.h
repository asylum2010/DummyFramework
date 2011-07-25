//=============================================================================================================
#ifndef _CCONTENTMANAGER9_H_
#define _CCONTENTMANAGER9_H_

#include "../../template/bijectivemap.hpp"
#include "../../template/signal.hpp"

#include "CSprite9.h"
#include "CStateManager9.h"

#include <set>
#include <string>
#include <d3dx9.h>

namespace DummyFramework
{
	enum etexturetype
    {
        Texture = 0,
        CubeTexture = 1
    };

    class CObject9;

    class CTextureDesc
    {
    public:
        unsigned long    ID;
        unsigned char    Type;
        DWORD            LevelCount;
        D3DSURFACE_DESC  Desc;

        CTextureDesc(unsigned long id)
            : ID(id), LevelCount(0) {}

        inline bool operator <(const CTextureDesc& other) const {
            return ID < other.ID;
        }
    };
	
    class CContentManager9 : public has_slots
    {
    protected:
        // a bonyolitások elkerülése végett ezek managedek
        typedef std::map<std::string, LPDIRECT3DBASETEXTURE9> texturemap;
        typedef std::map<std::string, LPD3DXMESH> meshmap;
        typedef std::map<std::string, LPD3DXEFFECT> materialmap;

        // a rendertargetek a default poolban jönnek létre (és device lostkor felszabadulnak)
        // ez egy megoldás...a szebb megoldás ha saját eröforrás osztályokat irsz
        typedef bijectivemap<LPDIRECT3DBASETEXTURE9, unsigned long> texturebmap;
        typedef bijectivemap<LPD3DXMESH, unsigned long> meshbmap;
        typedef std::set<CTextureDesc> desclist;

		CStateManager9*    statemanager;
        LPDIRECT3DDEVICE9  device;
        texturemap         textures;
        meshmap            meshes;
        texturebmap        createdtextures;
        meshbmap           createdmeshes;
        materialmap        materials;
        desclist           descriptors;

        unsigned long GenerateUniqueMeshID();
        unsigned long GenerateUniqueTextureID();

        static void CALLBACK ColorFill(D3DXVECTOR4* pOut, const D3DXVECTOR2* pTexCoord, const D3DXVECTOR2* pTexelSize, LPVOID pData);

        void ReadString(FILE* f, char* buff);
        bool LoadQM(CObject9& out, const std::string& file);
        bool LoadX(CObject9& out, const std::string& file);

    public:
        typedef std::map<std::string, CSprite9> textureatlas;

        CContentManager9();
        virtual ~CContentManager9();

        virtual LPDIRECT3DTEXTURE9 LoadTextureAtlas(textureatlas& out, const std::string& file, float (*correlate)(float) = NULL);
        virtual LPDIRECT3DTEXTURE9 LoadTexture(const std::string& file, D3DXIMAGE_INFO* info = NULL);
        virtual LPDIRECT3DCUBETEXTURE9 LoadCubeTexture(const std::string& file);
        virtual LPD3DXEFFECT LoadMaterial(const std::string& file);
        virtual LPD3DXMESH LoadMesh(const std::string& file);

        virtual LPD3DXMESH CreateBox(float width, float height, float depth);
        virtual LPD3DXMESH CreatePlane(float width, float height);
		virtual LPD3DXMESH CreateMesh(unsigned int faces, unsigned int vertices, const D3DVERTEXELEMENT9* decl);
        virtual LPDIRECT3DTEXTURE9 CreateTexture(unsigned int width, unsigned int height, D3DFORMAT format);
        virtual LPDIRECT3DTEXTURE9 CreateRenderTarget(unsigned int width, unsigned int height, D3DFORMAT format);
        virtual LPDIRECT3DCUBETEXTURE9 CreateCubeRenderTarget(unsigned int size, D3DFORMAT format);

        virtual bool LoadSprite(CSprite9& out, const std::string& file, float (*correlate)(float) = NULL);
        virtual bool CreateObject(CObject9& out, const std::string& file);
        virtual bool GenerateTangentFrame(LPD3DXMESH* mesh);
        virtual bool GenerateNormals(LPD3DXMESH* mesh);
        virtual bool Initialize(LPDIRECT3DDEVICE9 dev);
        virtual bool FillTexture(LPDIRECT3DTEXTURE9 texture, unsigned long color);

        // TODO: többi resourcera is; esetleg névvel és id-vel is
        virtual void UnloadTexture(LPDIRECT3DBASETEXTURE9 texture);
        virtual void Clear();

        inline unsigned long GetTextureID(LPDIRECT3DBASETEXTURE9 tex) {
            unsigned long ret = 0;
            createdtextures.getvalue(ret, tex);
            return ret;
        }

        inline LPDIRECT3DBASETEXTURE9 GetTextureByID(unsigned long id) {
            LPDIRECT3DBASETEXTURE9 tex = NULL;
            createdtextures.getkey(tex, id);
            return tex;
        }

		inline CStateManager9* GetStateManager() {
			return statemanager;
		}

	eventhandlers:
		virtual void onlostdevice();
        virtual void onresetdevice();
    };
}

#endif
//=============================================================================================================
