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

	/**
	 * \brief Texture description
	 */
	class CTextureDesc
	{
	public:
		unsigned int	ID;
		unsigned char	Type;
		DWORD			LevelCount;
		D3DSURFACE_DESC	Desc;

		CTextureDesc(unsigned int id)
			: ID(id), LevelCount(0) {}

		inline bool operator <(const CTextureDesc& other) const {
			return ID < other.ID;
		}
	};
	
	/**
	 * \brief Basic content manager
	 *
	 * The content manager keeps track of resources, and
	 * handles them on device loss/reset. However it
	 * does not make any reference counting on them, so
	 * if you no longer use a resource that will be released
	 * only when the program terminates.
	 *
	 * Resources in the default pool are handled specially
	 * (f. e. render targets). You can query the recreated
	 * resource through the <kbd>GetTextureByID()</kbd>
	 * method, when the device is reset.
	 *
	 * Note that the manager handles resources only created
	 * through its methods, thus if you get a texture's
	 * surface level for example, you have to release that
	 * on device loss/termination.
	 */
	class CContentManager9 : public has_slots
	{
	protected:
		// a bonyolitások elkerülése végett ezek managedek
		typedef std::map<std::string, LPDIRECT3DBASETEXTURE9> texturemap;
		typedef std::map<std::string, LPD3DXMESH> meshmap;
		typedef std::map<std::string, LPD3DXEFFECT> materialmap;

		// render targets are allocated in D3DPOOL_DEFAULT and released
		// automatically on device loss
		typedef bijectivemap<LPDIRECT3DBASETEXTURE9, unsigned int> texturebmap;
		typedef bijectivemap<LPD3DXMESH, unsigned int> meshbmap;
		typedef std::set<CTextureDesc> desclist;

		CStateManager9*		statemanager;
		LPDIRECT3DDEVICE9	device;
		texturemap			textures;
		meshmap				meshes;
		texturebmap			createdtextures;
		meshbmap			createdmeshes;
		materialmap			materials;
		desclist			descriptors;

		unsigned int GenerateUniqueMeshID();
		unsigned int GenerateUniqueTextureID();

		static void CALLBACK ColorFill(D3DXVECTOR4* pOut, const D3DXVECTOR2* pTexCoord, const D3DXVECTOR2* pTexelSize, LPVOID pData);

		void ReadString(FILE* f, char* buff);
		bool LoadQM(CObject9& out, const std::string& file);
		bool LoadX(CObject9& out, const std::string& file);

	public:
		typedef std::map<std::string, CSprite9> textureatlas;

		CContentManager9();
		virtual ~CContentManager9();

		virtual bool LoadSprite(CSprite9& out, const std::string& file, float (*correlate)(float) = NULL);
		virtual bool CreateObject(CObject9& out, const std::string& file);
		virtual bool GenerateTangentFrame(LPD3DXMESH* mesh);
		virtual bool GenerateNormals(LPD3DXMESH* mesh);
		virtual bool Initialize(LPDIRECT3DDEVICE9 dev);
		virtual bool FillTexture(LPDIRECT3DTEXTURE9 texture, unsigned int color);

		virtual void UnloadTexture(LPDIRECT3DBASETEXTURE9 texture);
		virtual void Clear();

		virtual LPDIRECT3DTEXTURE9		LoadTextureAtlas(textureatlas& out, const std::string& file, float (*correlate)(float) = NULL);
		virtual LPDIRECT3DTEXTURE9		LoadTexture(const std::string& file, D3DXIMAGE_INFO* info = NULL);
		virtual LPDIRECT3DCUBETEXTURE9	LoadCubeTexture(const std::string& file);
		virtual LPD3DXEFFECT			LoadMaterial(const std::string& file);
		virtual LPD3DXMESH				LoadMesh(const std::string& file);

		virtual LPD3DXMESH				CreateBox(float width, float height, float depth);
		virtual LPD3DXMESH				CreatePlane(float width, float height);
		virtual LPD3DXMESH				CreateMesh(unsigned int faces, unsigned int vertices, const D3DVERTEXELEMENT9* decl);
		virtual LPDIRECT3DTEXTURE9		CreateTexture(unsigned int width, unsigned int height, D3DFORMAT format);
		virtual LPDIRECT3DTEXTURE9		CreateRenderTarget(unsigned int width, unsigned int height, D3DFORMAT format);
		virtual LPDIRECT3DCUBETEXTURE9	CreateCubeRenderTarget(unsigned int size, D3DFORMAT format);

		inline unsigned int GetTextureID(LPDIRECT3DBASETEXTURE9 tex) {
			unsigned int ret = 0;
			createdtextures.getvalue(ret, tex);
			return ret;
		}

		inline LPDIRECT3DBASETEXTURE9 GetTextureByID(unsigned int id) {
			LPDIRECT3DBASETEXTURE9 tex = NULL;
			createdtextures.getkey(tex, id);
			return tex;
		}

		inline CStateManager9* GetStateManager() {
			return statemanager;
		}

	_DUMMY_EVENTHANDLERS:
		virtual void onlostdevice();
		virtual void onresetdevice();
	};
}

#endif
//=============================================================================================================
