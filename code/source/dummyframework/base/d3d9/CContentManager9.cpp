//=============================================================================================================
#include <dummyframework/base/d3d9/CObject9.h>
#include <dummyframework/base/d3d9/CContentManager9.h>
#include <dummyframework/base/CHelper.h>
#include <dummyframework/template/defines.h>

#include <iostream>
#include <fstream>
#include <sstream>

// parameters from Quadron
#define _QM_STREAMS_	4
#define _QM_BBSIZE_		24

namespace DummyFramework
{
	#define F sizeof(float)
	#define B sizeof(unsigned char)

	static const WORD ComponentSizes[18] =
	{
		1, 2, 3, 4, 4, 4, // float1, float2, float3, float4, d3dcolor, ubyte4
		2, 4, 4, 2, 4, 2,
		4, 3, 3, 2, 4, 0
	};

	static const WORD ComponentStrides[18] =
	{
		F, F, F, F, B, B, // float1, float2, float3, float4, d3dcolor, ubyte4
		0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0
	};

	void __stdcall CContentManager9::ColorFill(D3DXVECTOR4* pOut, const D3DXVECTOR2* pTexCoord, const D3DXVECTOR2* pTexelSize, LPVOID pData)
	{
		D3DXCOLOR color(*((DWORD*)pData));
		*pOut = D3DXVECTOR4(color.r, color.g, color.b, color.a);
	}

	CContentManager9::CContentManager9()
	{
		statemanager = NULL;
		device = NULL;
	}
	//=============================================================================================================
	CContentManager9::~CContentManager9()
	{
		safe_release(statemanager);

		Clear();
	}
	//=============================================================================================================
	bool CContentManager9::Initialize(LPDIRECT3DDEVICE9 dev)
	{
		statemanager = CStateManager9::Create(dev);
		device = dev;

		return true;
	}
	//=============================================================================================================
	LPDIRECT3DTEXTURE9 CContentManager9::LoadTextureAtlas(textureatlas& out, const std::string& file, float (*correlate)(float))
	{
		std::ifstream infile(file.c_str());
		dnassert(NULL, "CContentManager9::LoadTextureAtlas(): Could not open file", infile.fail());

		std::string line, path, texname;
		std::stringstream ss;
		float w, h;

		std::getline(infile, texname);
		CHelper::GetPath(path, file);

		LPDIRECT3DTEXTURE9 texture = LoadTexture(path + texname);
		dassert(false, "CContentManager9::LoadTextureAtlas(): Could not load texture", texture);

		while( !infile.eof() )
		{
			std::getline(infile, line);

			if( line.length() < 10 )
				continue;

			ss.clear();
			ss.str(line);

			ss >> texname;

			CSprite9& sprite = out[texname];
			sprite.Texture = texture;

			ss >> sprite.Texcoords[0] >> sprite.Texcoords[1] >> sprite.Texcoords[2] >> sprite.Texcoords[3];
			ss >> w >> h;

			sprite.Texcoords[2] += sprite.Texcoords[0];
			sprite.Texcoords[3] += sprite.Texcoords[1];

			sprite.OriginalSize.x = w;
			sprite.OriginalSize.y = h;

			if( correlate )
			{
				sprite.Size.y = correlate(h);
				sprite.Size.x = correlate(w);
			}
			else
			{
				sprite.Size.y = h;
				sprite.Size.x = w;
			}
		}

		infile.close();
		return texture;
	}
	//=============================================================================================================
	LPDIRECT3DTEXTURE9 CContentManager9::LoadTexture(const std::string& file, D3DXIMAGE_INFO* info)
	{
		HRESULT hr;
		std::string name;

		CHelper::GetFile(name, file);
		texturemap::iterator it = textures.find(name);

		if( info )
		{
			hr = D3DXGetImageInfoFromFile(file.c_str(), info);
			dnassert(NULL, "CContentManager9::LoadTexture(): Could not get image info", FAILED(hr));
		}

		if( it != textures.end() )
		{
			std::cout << "Reference '" << name << "'\n";
			return (LPDIRECT3DTEXTURE9)it->second;
		}

		std::cout << "Creating '" << name << "'\n";
		LPDIRECT3DTEXTURE9 tex = NULL;

		hr = D3DXCreateTextureFromFileEx(
			device, file.c_str(), D3DX_DEFAULT, D3DX_DEFAULT, 1, 0,
			D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
			0, NULL, NULL, &tex);

		dnassert(NULL, "CContentManager9::LoadTexture(): Could not create texture", FAILED(hr));

		textures[name] = tex;
		return tex;
	}
	//=============================================================================================================
	LPDIRECT3DCUBETEXTURE9 CContentManager9::LoadCubeTexture(const std::string& file)
	{
		std::string name;
		CHelper::GetFile(name, file);
		texturemap::iterator it = textures.find(name);

		if( it != textures.end() )
		{
			std::cout << "Reference '" << name << "'\n";
			return (LPDIRECT3DCUBETEXTURE9)it->second;
		}

		std::cout << "Creating '" << name << "'\n";
		LPDIRECT3DCUBETEXTURE9 tex = NULL;

		HRESULT hr = D3DXCreateCubeTextureFromFile(device, file.c_str(), &tex);
		dnassert(NULL, "CContentManager9::LoadCubeTexture(): Could not create texture", FAILED(hr));

		textures[name] = tex;
		return tex;
	}
	//=============================================================================================================
	LPD3DXEFFECT CContentManager9::LoadMaterial(const std::string& file)
	{
		std::string name;
		CHelper::GetFile(name, file);
		materialmap::iterator it = materials.find(name);

		if( it != materials.end() )
		{
			std::cout << "Reference '" << name << "'\n";
			return it->second;
		}

		std::cout << "Creating '" << name << "'\n";

		LPD3DXEFFECT mat = NULL;
		LPD3DXBUFFER errors = NULL;

		DWORD flags = 0;

#ifdef _DEBUG
		flags |= D3DXSHADER_DEBUG;
#endif

		HRESULT hr = D3DXCreateEffectFromFileA(device, file.c_str(), NULL, NULL, flags, NULL, &mat, &errors);

		if( errors )
		{
			std::cout << (const char*)errors->GetBufferPointer() << "\n";
			errors->Release();
		}

		dnassert(NULL, "CContentManager9::LoadMaterial(): Could not create material", FAILED(hr));
		materials[name] = mat;

		// stateblockra nemmegy
		if( statemanager )
			hr = mat->SetStateManager(statemanager);
		else
			hr = E_FAIL;

		dnerror(mat, "CContentManager9::LoadMaterial(): Could not set state manager", FAILED(hr));
		return mat;
	}
	//=============================================================================================================
	LPD3DXMESH CContentManager9::LoadMesh(const std::string& file)
	{
		std::string name;
		CHelper::GetFile(name, file);
		meshmap::iterator it = meshes.find(name);

		if( it != meshes.end() )
		{
			std::cout << "Reference '" << name << "'\n";
			return it->second;
		}

		LPD3DXMESH mesh = NULL;
		std::cout << "Creating '" << name << "'\n";

		HRESULT hr = D3DXLoadMeshFromX(file.c_str(), D3DXMESH_MANAGED,
			device, NULL, NULL, NULL, NULL, &mesh);

		dnassert(NULL, "CContentManager9::LoadMesh(): Could not create mesh", FAILED(hr));
		meshes[name] = mesh;

		return mesh;
	}
	//=============================================================================================================
	bool CContentManager9::LoadSprite(CSprite9& out, const std::string& file, float (*correlate)(float))
	{
		std::ifstream infile(file.c_str());
		dnassert(false, "CContentManager9::LoadSprite(): Could not open file", infile.fail());

		float u, v, s, t, w, h;
		std::string texname, path;

		infile >> texname >> u >> v >> s >> t >> w >> h;
		infile.close();

		out.OriginalSize.x = w;
		out.OriginalSize.y = h;

		if( correlate )
		{
			out.Size.y = correlate(h);
			out.Size.x = correlate(w);
		}
		else
		{
			out.Size.y = h;
			out.Size.x = w;
		}

		CHelper::GetPath(path, file);

		out.Texture = LoadTexture(path + texname);
		dassert(false, "CContentManager9::LoadSprite(): Could not load texture", out.Texture);

		out.Texcoords[0] = u;
		out.Texcoords[1] = v;
		out.Texcoords[2] = u + s;
		out.Texcoords[3] = v + t;

		return true;
	}
	//=============================================================================================================
	bool CContentManager9::CreateObject(CObject9& out, const std::string& file)
	{
		std::string name;
		CHelper::GetFile(name, file);
		meshmap::iterator it = meshes.find(name);

		out.Mesh = NULL;

		if( it != meshes.end() )
		{
			std::cout << "Reference '" << name << "'\n";
			out.Mesh = it->second;
		}

		std::string ext;
		CHelper::GetExtension(ext, file);

		std::cout << "Creating '" << name << "'\n";

		if( ext == "x" )
		{
			if( !LoadX(out, file) )
				return false;
		}
		else if( ext == "qm" )
		{
			if( !LoadQM(out, file) )
				return false;
		}

		meshes[name] = out.Mesh;
		return true;
	}
	//=============================================================================================================
	LPD3DXMESH CContentManager9::CreateBox(float width, float height, float depth)
	{
		LPD3DXMESH mesh = NULL;

		D3DVERTEXELEMENT9 declaration[] = 
		{
			{ 0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
			{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0 },
			{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
			D3DDECL_END()
		};

		float whalf = width / 2;
		float hhalf = height / 2;
		float dhalf = depth / 2;

		float vertices[192] =
		{
			// front
				whalf, -hhalf, -dhalf,  0, 0, -1,  1, 1,
			-whalf, -hhalf, -dhalf,  0, 0, -1,  0, 1,
			-whalf,  hhalf, -dhalf,  0, 0, -1,  0, 0,
				whalf,  hhalf, -dhalf,  0, 0, -1,  1, 0,

			// bottom
				whalf, -hhalf,  dhalf,  0, -1, 0,  1, 1, 
			-whalf, -hhalf,  dhalf,  0, -1, 0,  0, 1,
			-whalf, -hhalf, -dhalf,  0, -1, 0,  0, 0,
				whalf, -hhalf, -dhalf,  0, -1, 0,  1, 0,

			// top
				whalf, hhalf, -dhalf,  0, 1, 0,  1, 1,
			-whalf, hhalf, -dhalf,  0, 1, 0,  0, 1,
			-whalf, hhalf,  dhalf,  0, 1, 0,  0, 0,
				whalf, hhalf,  dhalf,  0, 1, 0,  1, 0,

			-whalf, -hhalf, dhalf, 0, 0, 1, 1, 1,
			whalf, -hhalf, dhalf, 0, 0, 1, 0, 1,
			whalf, hhalf, dhalf, 0, 0, 1, 0, 0,
			-whalf, hhalf, dhalf, 0, 0, 1, 1, 0,

			whalf, -hhalf, dhalf, 1, 0, 0, 1, 1,
			whalf, -hhalf, -dhalf, 1, 0, 0, 0, 1,
			whalf, hhalf, -dhalf, 1, 0, 0, 0, 0,
			whalf, hhalf, dhalf, 1, 0, 0, 1, 0,

			-whalf, -hhalf, -dhalf, -1, 0, 0, 1, 1,
			-whalf, -hhalf, dhalf, -1, 0, 0, 0, 1,
			-whalf, hhalf, dhalf, -1, 0, 0, 0, 0,
			-whalf, hhalf, -dhalf, -1, 0, 0, 1, 0
		};

		unsigned short indices[36] =
		{
			0, 1, 2, 2, 3, 0, 
			4, 5, 6, 6, 7, 4,
			8, 9, 10, 10, 11, 8,
			12, 13, 14, 14, 15, 12,
			16, 17, 18, 18, 19, 16,
			20, 21, 22, 22, 23, 20
		};

		unsigned int id = GenerateUniqueMeshID();
		std::cout << "Creating 'mesh;id:" << id << "'\n";

		HRESULT hr = D3DXCreateMesh(12, 24, D3DXMESH_MANAGED, declaration, device, &mesh);
		dnassert(NULL, "CContentManager9::CreateBox(): Could not create mesh", FAILED(hr));

		void* vdata = NULL;
		hr = mesh->LockVertexBuffer(0, &vdata);
		dnassert(NULL, "CContentManager9::CreateBox(): Could not lock vertex buffer", FAILED(hr));

		memcpy(vdata, vertices, sizeof(float) * 192);
		mesh->UnlockVertexBuffer();

		void* idata = NULL;
		hr = mesh->LockIndexBuffer(0, &idata);
		dnassert(NULL, "CContentManager9::CreateBox(): Could not lock index buffer", FAILED(hr));

		memcpy(idata, indices, sizeof(unsigned short) * 36);
		mesh->UnlockIndexBuffer();

		createdmeshes.insert(mesh, id);
		return mesh;
	}
	//=============================================================================================================
	LPD3DXMESH CContentManager9::CreatePlane(float width, float height)
	{
		LPD3DXMESH mesh = NULL;

		D3DVERTEXELEMENT9 declaration[] = 
		{
			{ 0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
			{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0 },
			{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
			D3DDECL_END()
		};

		float whalf = width / 2;
		float hhalf = height / 2;

		float vertices[32] =
		{
			-whalf, 0,  hhalf, 0, 1, 0, 0, 0,
				whalf, 0,  hhalf, 0, 1, 0, 1, 0,
				whalf, 0, -hhalf, 0, 1, 0, 1, 1,
			-whalf, 0, -hhalf, 0, 1, 0, 0, 1
		};

		unsigned short indices[6] =
		{
			0, 1, 2,
			0, 2, 3
		};

		unsigned int id = GenerateUniqueMeshID();
		std::cout << "Creating 'mesh;id:" << id << "'\n";

		HRESULT hr = D3DXCreateMesh(2, 4, D3DXMESH_MANAGED, declaration, device, &mesh);
		dnassert(NULL, "CContentManager9::CreatePlane(): Could not create mesh", FAILED(hr));

		void* vdata = NULL;
		hr = mesh->LockVertexBuffer(0, &vdata);
		dnassert(NULL, "CContentManager9::CreatePlane(): Could not lock vertex buffer", FAILED(hr));

		memcpy(vdata, vertices, sizeof(float) * 32);
		mesh->UnlockVertexBuffer();

		void* idata = NULL;
		hr = mesh->LockIndexBuffer(0, &idata);
		dnassert(NULL, "CContentManager9::CreatePlane(): Could not lock index buffer", FAILED(hr));

		memcpy(idata, indices, sizeof(unsigned short) * 6);
		mesh->UnlockIndexBuffer();

		createdmeshes.insert(mesh, id);
		return mesh;
	}
	//=============================================================================================================
	LPD3DXMESH CContentManager9::CreateMesh(unsigned int faces, unsigned int vertices, const D3DVERTEXELEMENT9* decl)
	{
		LPD3DXMESH mesh = NULL;

		unsigned int id = GenerateUniqueMeshID();
		std::cout << "Creating 'mesh;id:" << id << "'\n";

		HRESULT hr = D3DXCreateMesh(faces, vertices, D3DXMESH_MANAGED, decl, device, &mesh);
		dnassert(NULL, "CContentManager9::CreateMesh(): Could not create mesh", FAILED(hr));

		createdmeshes.insert(mesh, id);
		return mesh;
	}
	//=============================================================================================================
	LPDIRECT3DTEXTURE9 CContentManager9::CreateTexture(unsigned int width, unsigned int height, D3DFORMAT format)
	{
		LPDIRECT3DTEXTURE9 tex = NULL;
		unsigned int id = GenerateUniqueTextureID();

		std::cout << "Creating 'texture;id:" << id << "'\n";

		HRESULT hr = device->CreateTexture(width, height, 1, 0,
			format, D3DPOOL_MANAGED, &tex, NULL);

		dnassert(NULL, "CContentManager9::CreateTexture(): Could not create texture", FAILED(hr));
		createdtextures.insert(tex, id);

		return tex;
	}
	//=============================================================================================================
	LPDIRECT3DTEXTURE9 CContentManager9::CreateRenderTarget(unsigned int width, unsigned int height, D3DFORMAT format)
	{
		LPDIRECT3DTEXTURE9 tex = NULL;
		unsigned int id = GenerateUniqueTextureID();

		std::cout << "Creating 'texture;id:" << id << "'\n";

		HRESULT hr = device->CreateTexture(width, height, 1, D3DUSAGE_RENDERTARGET,
			format, D3DPOOL_DEFAULT, &tex, NULL);

		dnassert(NULL, "CContentManager9::CreateRenderTarget(): Could not create texture", FAILED(hr));
		createdtextures.insert(tex, id);

		return tex;
	}
	//=============================================================================================================
	LPDIRECT3DCUBETEXTURE9 CContentManager9::CreateCubeRenderTarget(unsigned int size, D3DFORMAT format)
	{
		LPDIRECT3DCUBETEXTURE9 tex = NULL;
		unsigned int id = GenerateUniqueTextureID();

		std::cout << "Creating 'texture;id:" << id << "'\n";

		HRESULT hr = D3DXCreateCubeTexture(device, size, 1, D3DUSAGE_RENDERTARGET,
			format, D3DPOOL_DEFAULT, &tex);

		dnassert(NULL, "CContentManager9::CreateCubeRenderTarget(): Could not create texture", FAILED(hr));
		createdtextures.insert(tex, id);

		return tex;
	}
	//=============================================================================================================
	bool CContentManager9::GenerateTangentFrame(LPD3DXMESH* mesh)
	{
		LPD3DXMESH newmesh = NULL;
		LPD3DXMESH newmesh2 = NULL;

		D3DVERTEXELEMENT9 declaration[] = 
		{
			{ 0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
			{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
			{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
			{ 0, 32, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },
			{ 0, 44, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0 },
			D3DDECL_END()
		};

		std::string tmp("");
		unsigned int tmpid;
		unsigned char needupdate = 0;
		HRESULT hr;

		// az eltárolt pointert updatelni kell
		for( meshmap::iterator it = meshes.begin(); it != meshes.end(); ++it )
		{
			if( it->second == (*mesh) )
			{
				tmp = it->first;
				needupdate = 1;
			}
		}

		// de nem tudjuk, hogy ez most betöltött vagy generált
		if( needupdate == 0 )
		{
			if( createdmeshes.getvalue(tmpid, (*mesh)) )
			{
				needupdate = 2;
				createdmeshes.removebykey((*mesh));
			}
		}

		hr = (*mesh)->CloneMesh(D3DXMESH_MANAGED, declaration, device, &newmesh);
		dnassert(false, "CContentManager9::GenerateTangentFrame(): Could not clone mesh", FAILED(hr));

		hr = D3DXComputeTangentFrameEx(newmesh, D3DDECLUSAGE_TEXCOORD, 0,
			D3DDECLUSAGE_TANGENT, 0, D3DDECLUSAGE_BINORMAL, 0, D3DDECLUSAGE_NORMAL, 0,
			0, NULL, 0.01f, 0.25f, 0.01f, &newmesh2, NULL);

		if( FAILED(hr) )
		{
			newmesh->Release();
			dnassert(false, "CContentManager9::GenerateTangentFrame(): Could not compute tangent frame", true);
		}

		if( needupdate == 1 )
		{
			std::cout << "Updated '" << tmp << "'\n";
			meshes[tmp] = newmesh2;
		}
		else if( needupdate == 2 )
		{
			std::cout << "Updated 'mesh;id:" << tmpid << "'\n";
			createdmeshes.insert(newmesh2, tmpid);
		}

		safe_release(newmesh);
		safe_release((*mesh));

		(*mesh) = newmesh2;
		return true;
	}
	//=============================================================================================================
	bool CContentManager9::GenerateNormals(LPD3DXMESH* mesh)
	{
		LPD3DXMESH newmesh = NULL;
		DWORD fvf = (*mesh)->GetFVF();

		if( fvf & D3DFVF_NORMAL )
			return true;

		std::string tmp("");
		unsigned int tmpid;
		unsigned char needupdate = 0;
		HRESULT hr;

		// az eltárolt pointert updatelni kell
		for( meshmap::iterator it = meshes.begin(); it != meshes.end(); ++it )
		{
			if( it->second == (*mesh) )
			{
				tmp = it->first;
				needupdate = 1;
			}
		}

		// de nem tudjuk, hogy ez most betöltött vagy generált
		if( needupdate == 0 )
		{
			if( createdmeshes.getvalue(tmpid, (*mesh)) )
			{
				needupdate = 2;
				createdmeshes.removebykey((*mesh));
			}
		}

		hr = (*mesh)->CloneMeshFVF(D3DXMESH_MANAGED, fvf|D3DFVF_NORMAL, device, &newmesh);
		dnassert(false, "CContentManager9::GenerateNormals(): Could not clone mesh", FAILED(hr));

		DWORD* adjacency = new DWORD[newmesh->GetNumFaces() * 3];
		hr = newmesh->GenerateAdjacency(0.01f, adjacency);

		if( FAILED(hr) )
		{
			newmesh->Release();
			dnassert(false, "CContentManager9::GenerateNormals(): Could not generate adjacency", true);
		}

		hr = D3DXComputeNormals(newmesh, adjacency);

		if( FAILED(hr) )
		{
			newmesh->Release();
			dnassert(false, "CContentManager9::GenerateNormals(): Could not compute normals", true);
		}

		if( needupdate == 1 )
		{
			std::cout << "Updated '" << tmp << "'\n";
			meshes[tmp] = newmesh;
		}
		else if( needupdate == 2 )
		{
			std::cout << "Updated 'mesh;id:" << tmpid << "'\n";
			createdmeshes.insert(newmesh, tmpid);
		}

		delete[] adjacency;
		safe_release((*mesh));

		(*mesh) = newmesh;
		return true;
	}
	//=============================================================================================================
	bool CContentManager9::FillTexture(LPDIRECT3DTEXTURE9 texture, unsigned int color)
	{
		if( !texture )
			return false;

		return SUCCEEDED(D3DXFillTexture(texture, ColorFill, &color));
	}
	//=============================================================================================================
	void CContentManager9::Clear()
	{
		for( texturemap::iterator it = textures.begin(); it != textures.end(); ++it )
		{
			if( it->second )
			{
				std::cout << "Releasing '" << it->first << "'\n";
				it->second->Release();
			}
		}

		for( meshmap::iterator it = meshes.begin(); it != meshes.end(); ++it )
		{
			if( it->second )
			{
				std::cout << "Releasing '" << it->first << "'\n";
				it->second->Release();
			}
		}

		for( materialmap::iterator it = materials.begin(); it != materials.end(); ++it )
		{
			if( it->second )
			{
				std::cout << "Releasing '" << it->first << "'\n";
				it->second->Release();
			}
		}

		for( meshbmap::key_iterator it = createdmeshes.kbegin(); it != createdmeshes.kend(); ++it )
		{
			if( it->first )
			{
				std::cout << "Releasing 'mesh;id:" << it->second << "'\n";
				it->first->Release();
			}
		}

		for( texturebmap::key_iterator it = createdtextures.kbegin(); it != createdtextures.kend(); ++it )
		{
			if( it->first )
			{
				std::cout << "Releasing 'texture;id:" << it->second << "'\n";
				it->first->Release();
			}
		}

		textures.clear();
		createdtextures.clear();
		meshes.clear();
		createdmeshes.clear();
		materials.clear();
	}
	//=============================================================================================================
	unsigned int CContentManager9::GenerateUniqueMeshID()
	{
		unsigned int size = (unsigned int)createdmeshes.size() + 1;

		for( unsigned int i = 1; i < size + 1; ++i )
		{
			if( createdmeshes.valuecount(i) == 0 )
				return i;
		}

		return size;
	}
	//=============================================================================================================
	unsigned int CContentManager9::GenerateUniqueTextureID()
	{
		unsigned int size = (unsigned int)createdtextures.size() + 1;

		for( unsigned int i = 1; i < size; ++i )
		{
			// do not allow it to choose from IDs saved on device loss
			if( (createdtextures.valuecount(i) == 0) &&
				(descriptors.find(CTextureDesc(i)) == descriptors.end()) )
			{
				return i;
			}
		}

		return size;
	}
	//=============================================================================================================
	void CContentManager9::ReadString(FILE* f, char* buff)
	{
		size_t ind = 0;
		char ch = fgetc(f);

		while( ch != '\n' )
		{
			buff[ind] = ch;
			ch = fgetc(f);
			++ind;
		}

		buff[ind] = '\0';
	}
	//=============================================================================================================
	bool CContentManager9::LoadQM(CObject9& out, const std::string& file)
	{
		dassert(false, "CContentManager9::LoadQM(): device == NULL", device);

		FILE* infile = NULL;
		fopen_s(&infile, file.c_str(), "rb");

		dassert(false, "CContentManager9::LoadQM(): Could not open file", infile);

		std::string path;
		CHelper::GetPath(path, file);

		D3DVERTEXELEMENT9*	elem = NULL;
		LPD3DXMESH			mesh = NULL;
		LPDIRECT3DTEXTURE9	texture = NULL;

		HRESULT				hr;
		DWORD				numvertices, subsets, unused, numindices, numfaces, indexstride;
		WORD				stride = 0;
		std::string			str;
		char				buff[1024];

		// header
		fread(&unused, 4, 1, infile);
		fread(&numindices, 4, 1, infile);
		fread(&numfaces, 4, 1, infile);
		fread(&indexstride, 4, 1, infile);
		fread(&subsets, 4, 1, infile);
		fread(&numvertices, 4, 1, infile);

		// ennek az engineben szerepe van
		for( unsigned char i = 1; i < _QM_STREAMS_; ++i )
			fread(&unused, 4, 1, infile);

		// vertex declaration
		fread(&unused, 4, 1, infile);

		elem = new D3DVERTEXELEMENT9[unused + 1];
		memset(elem, 0, (unused + 1) * sizeof(D3DVERTEXELEMENT9));

		for( DWORD i = 0; i < unused; ++i )
		{
			fread(&elem[i].Stream, 2, 1, infile);
			fread(&elem[i].Usage, 1, 1, infile);
			fread(&elem[i].Type, 1, 1, infile);
			fread(&elem[i].UsageIndex, 1, 1, infile);

			elem[i].Method = D3DDECLMETHOD_DEFAULT;
			elem[i].Offset = stride;

			stride += ComponentSizes[elem[i].Type] * ComponentStrides[elem[i].Type];
		}

		elem[unused].Stream = 0xff;
		elem[unused].Offset = 0;
		elem[unused].Type = D3DDECLTYPE_UNUSED;
		elem[unused].Method = 0;
		elem[unused].Usage = 0;
		elem[unused].UsageIndex = 0;

		DWORD flags = D3DXMESH_MANAGED;

		if( indexstride == 4 )
			flags |= D3DXMESH_32BIT;

		hr = D3DXCreateMesh(numfaces, numvertices, flags, elem, device, &mesh);
		dnassert(false, "CContentManager9::LoadQM(): Could not create mesh", FAILED(hr));

		// a vertex és index adatokat egyetlen olvasással elintézzük
		void* vdata = NULL;
		hr = mesh->LockVertexBuffer(0, &vdata);
		dnassert(false, "CContentManager9::LoadQM(): Could not lock vertex buffer", FAILED(hr));

		fread(vdata, stride, numvertices, infile);
		mesh->UnlockVertexBuffer();

		void* idata = NULL;
		hr = mesh->LockIndexBuffer(0, &idata);
		dnassert(false, "CContentManager9::LoadQM(): Could not lock index buffer", FAILED(hr));

		fread(idata, indexstride, numindices, infile);
		mesh->UnlockIndexBuffer();
		out.Resize(subsets);

		D3DXATTRIBUTERANGE* attributetable = new D3DXATTRIBUTERANGE[subsets];
		char bb[_QM_BBSIZE_];

		for( size_t i = 0; i < subsets; ++i )
		{
			attributetable[i].AttribId = (DWORD)i;

			fread((void*)&attributetable[i].FaceStart, 4, 1, infile);
			fread((void*)&attributetable[i].VertexStart, 4, 1, infile);
			fread((void*)&unused, 4, 1, infile);
			fread((void*)&attributetable[i].VertexCount, 4, 1, infile);
			fread((void*)&attributetable[i].FaceCount, 4, 1, infile);
			fread((void*)bb, _QM_BBSIZE_, 1, infile);

			attributetable[i].FaceStart /= 3;

			ReadString(infile, buff);
			std::cout << "* QM: Added subset '" << buff << "'\n";

			for( unsigned char j = 0; j < 8; ++j )
			{
				ReadString(infile, buff);
				str = std::string(buff);

				if( str.length() > 2 ) 
					texture = LoadTexture((path + str).c_str());
				else
					texture = NULL;

				out.Textures(j, i) = texture;
			}
		}

		fclose(infile);

		// az id3dxmesh kicsit máshogy közeliti meg a dolgot...elég buta..
		DWORD* attr = NULL;
		DWORD count;

		hr = mesh->LockAttributeBuffer(0, &attr);
		dnassert(false, "CContentManager9::LoadQM(): Could not lock attribute buffer", FAILED(hr));

		for( DWORD i = 0; i < subsets; ++i )
		{
			count = attributetable[i].FaceStart + attributetable[i].FaceCount;

			for( DWORD j = attributetable[i].FaceStart; j < count; ++j )
				*(attr + j) = attributetable[i].AttribId;
		}

		mesh->UnlockAttributeBuffer();

		// ugyhogy hadd optiamizálja ö
		DWORD* adjacency = new DWORD[numindices];

		hr = mesh->GenerateAdjacency(1e-6f, adjacency);
		dnassert(false, "CContentManager9::LoadQM(): Could not generate adjacency", FAILED(hr));

		hr = mesh->OptimizeInplace(D3DXMESHOPT_ATTRSORT|D3DXMESHOPT_VERTEXCACHE, adjacency, NULL, NULL, NULL);
		dnassert(false, "CContentManager9::LoadQM(): Could not optimize mesh", FAILED(hr));

		// ez 100% memory leak ha valami hiba történt
		delete[] elem;
		delete[] attributetable;
		delete[] adjacency;

		if( !out.Mesh )
			out.Mesh = mesh;

		return true;
	}
	//=============================================================================================================
	bool CContentManager9::LoadX(CObject9& out, const std::string& file)
	{
		dassert(false, "CContentManager9::LoadX(): device == NULL", device);

		DWORD nummaterials = 0;
		LPD3DXBUFFER materialbuff = NULL;
		LPD3DXMESH mesh = NULL;

		if( SUCCEEDED(D3DXLoadMeshFromXA(file.c_str(), D3DXMESH_MANAGED, device, NULL,
			&materialbuff, NULL, &nummaterials, &mesh)))
		{
			if( materialbuff )
			{
				out.Resize(nummaterials);
				D3DXMATERIAL* mats = (D3DXMATERIAL*)materialbuff->GetBufferPointer();

				std::string path;
				CHelper::GetPath(path, file);

				for( DWORD i = 0; i < nummaterials; ++i )
				{
					if( mats[i].pTextureFilename && strlen(mats[i].pTextureFilename) > 2 )
					{
						out.Textures(0, i) = LoadTexture(path + mats[i].pTextureFilename);
					}
				}

				materialbuff->Release();
			}
		}
		else
			dnassert(false, "CContentManager9::LoadX(): Could not create mesh", true);

		if( !out.Mesh )
			out.Mesh = mesh;

		return true;
	}
	//=============================================================================================================
	void CContentManager9::UnloadTexture(LPDIRECT3DBASETEXTURE9 texture)
	{
		unsigned int id;

		if( createdtextures.getvalue(id, texture) )
		{
			std::cout << "Unloaded 'texture;id:" << id << "'\n";

			createdtextures.removebykey(texture);
			texture->Release();
		}
		else
		{
			for( texturemap::iterator it = textures.begin(); it != textures.end(); ++it )
			{
				if( it->second == texture )
				{
					std::cout << "Unloaded '" << it->first << "'\n";

					texture->Release();
					textures.erase(it);
					break;
				}
			}
		}
	}
	//=============================================================================================================
	void CContentManager9::onlostdevice()
	{
		CTextureDesc desc(0);
		texturebmap::key_iterator it = createdtextures.kbegin();
		texturebmap::key_iterator next;

		while( it != createdtextures.kend() )
		{
			next = it;
			++next;

			if( it->first->GetType() == D3DRTYPE_TEXTURE )
			{
				LPDIRECT3DTEXTURE9 tex = (LPDIRECT3DTEXTURE9)it->first;
				tex->GetLevelDesc(0, &desc.Desc);

				if( desc.Desc.Pool == D3DPOOL_DEFAULT )
				{
					desc.Type = Texture;
					desc.LevelCount = tex->GetLevelCount();
					createdtextures.getvalue(desc.ID, it->first);

					std::cout << "Lost 'texture;id:" << it->second << "'\n";

					descriptors.insert(desc);
					createdtextures.removebykey(it->first);
					tex->Release();
				}
			}
			else if( it->first->GetType() == D3DRTYPE_CUBETEXTURE )
			{
				LPDIRECT3DCUBETEXTURE9 tex = (LPDIRECT3DCUBETEXTURE9)it->first;
				tex->GetLevelDesc(0, &desc.Desc);

				if( desc.Desc.Pool == D3DPOOL_DEFAULT )
				{
					desc.Type = CubeTexture;
					desc.LevelCount = tex->GetLevelCount();
					createdtextures.getvalue(desc.ID, it->first);

					std::cout << "Lost 'texture;id:" << it->second << "'\n";

					descriptors.insert(desc);
					createdtextures.removebykey(it->first);
					tex->Release();
				}
			}

			it = next;
		}

		for( materialmap::iterator it = materials.begin(); it != materials.end(); ++it )
		{
			if( it->second )
			{
				std::cout << "Lost '" << it->first << "'\n";
				it->second->OnLostDevice();
			}
		}
	}
	//=============================================================================================================
	void CContentManager9::onresetdevice()
	{
		for( desclist::iterator it = descriptors.begin(); it != descriptors.end(); ++it )
		{
			if( it->Type == Texture )
			{
				LPDIRECT3DTEXTURE9 tex = NULL;

				if( FAILED(device->CreateTexture(it->Desc.Width, it->Desc.Height, it->LevelCount, it->Desc.Usage,
					it->Desc.Format, it->Desc.Pool, &tex, NULL)) )
				{
					std::cout << "Could not reset 'texture;id:" << it->ID << "'\n";
				}
				else
				{
					std::cout << "Reset 'texture;id:" << it->ID << "'\n";
					createdtextures.insert(tex, it->ID);
				}
			}
			else if( it->Type == CubeTexture )
			{
				LPDIRECT3DCUBETEXTURE9 tex = NULL;

				if( FAILED(D3DXCreateCubeTexture(device, it->Desc.Width, it->LevelCount, it->Desc.Usage,
					it->Desc.Format, it->Desc.Pool, &tex)) )
				{
					std::cout << "Could not reset 'texture;id:" << it->ID << "'\n";
				}
				else
				{
					std::cout << "Reset 'texture;id:" << it->ID << "'\n";
					createdtextures.insert(tex, it->ID);
				}
			}
		}

		for( materialmap::iterator it = materials.begin(); it != materials.end(); ++it )
		{
			if( it->second )
			{
				std::cout << "Reset '" << it->first << "'\n";
				it->second->OnResetDevice();
			}
		}
	}
	//=============================================================================================================
}
