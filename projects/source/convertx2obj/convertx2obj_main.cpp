//=============================================================================================================
#include "convertx2obj.h"
#include <fstream>
#include <algorithm>

ConvertX2OBJ::ConvertX2OBJ()
{
	Application.keyup.connect(this, &ConvertX2OBJ::application_keyup);
}
//=============================================================================================================
ConvertX2OBJ::~ConvertX2OBJ()
{
}
//=============================================================================================================
bool ConvertX2OBJ::LoadSettings()
{
	std::ifstream infile("settings.ini");

	if( infile.fail() )
		return false;

	infile >> DisplayMode.Width >> DisplayMode.Height >> FullScreen >> VSync;
	infile.close();

	return true;
}
//=============================================================================================================
bool ConvertX2OBJ::Initialize()
{
	Application.Title = "ConvertX2OBJ (under development)";

	if( !LoadSettings() )
	{
		DisplayMode.Width = 800;
		DisplayMode.Height = 600;
	}

	bool success = CGame9::Initialize();
	dassert(false, "ConvertX2OBJ::Initialize(): Could not initialize game", success);

	// TODO: initialization logic
	return success;
}
//=============================================================================================================
bool ConvertX2OBJ::LoadContent()
{
#ifdef _DEBUG
	SetCurrentDirectory("../../../media/");
#else
	SetCurrentDirectory("../media/");
#endif

	LPD3DXMESH mesh = Content.LoadMesh("meshes/skullocc2.X");
	dassert(false, "ConvertX2OBJ::LoadContent(): Could not load mesh", mesh);

	// look for vertex attributes
	D3DVERTEXELEMENT9 decl[MAX_FVF_DECL_SIZE];
	char* vdata = NULL;
	char* idata = NULL;
	DWORD* adata = NULL;

	mesh->GetDeclaration(decl);

	WORD posoff, possize = 0;
	WORD texoff, texsize = 0;
	WORD normoff, normsize = 0;
	WORD entries = 0;

	WORD sizes[] =
	{
		1, // D3DDECLTYPE_FLOAT1
		2, // D3DDECLTYPE_FLOAT2
		3, // D3DDECLTYPE_FLOAT3
		4, // D3DDECLTYPE_FLOAT4
	};

	for( DWORD i = 0; i < MAX_FVF_DECL_SIZE; ++i )
	{
		if( decl[i].Stream == 0xff )
			break;

		if( decl[i].Usage == D3DDECLUSAGE_POSITION )
		{
			posoff = decl[i].Offset;

			if( decl[i].Type == D3DDECLTYPE_FLOAT3 )
				possize = 3;
			else
				possize = 4;
		}
		else if( decl[i].Usage == D3DDECLUSAGE_NORMAL )
		{
			normoff = decl[i].Offset;
			normsize = 3;
		}
		else if(
			decl[i].Usage == D3DDECLUSAGE_TEXCOORD &&
			decl[i].UsageIndex == 0 )
		{
			texoff = decl[i].Offset;
			
			texsize = sizes[decl[i].Type];
		}

		++entries;
	}

	std::cout << "\nnumber of entries: " << entries << "\n";
	std::cout << "position: " << posoff << ", " << possize << "\ntexcoord: " << texoff <<
		", " << texsize << "\nnormal: " << normoff << ", " << normsize << "\n";

	std::cout << "\nnumber of vertices: " << mesh->GetNumVertices() << "\n";
	std::cout << "vertex stride: " << mesh->GetNumBytesPerVertex() << "\n";
	std::cout << "number of faces: " << mesh->GetNumFaces() << "\n\n";

	// convert to obj
	mesh->LockVertexBuffer(0, (void**)&vdata);
	mesh->LockIndexBuffer(0, (void**)&idata);
	mesh->LockAttributeBuffer(0, &adata);

	// create a simple 'subset table'
	DWORD options = mesh->GetOptions();
	DWORD stride = mesh->GetNumBytesPerVertex();

	DWORD lastattr = adata[0];
	DWORD numsubsets = 0;
	DWORD subsetstarts[256];
	DWORD subsetcounts[256];

	if( options & D3DXMESH_32BIT )
		std::cout << "32 bit index buffer\n";
	else
		std::cout << "16 bit index buffer\n";

	subsetstarts[0] = 0;
	subsetcounts[0] = 0;

	for( DWORD i = 0; i < mesh->GetNumFaces(); ++i )
	{
		if( adata[i] != lastattr )
		{
			subsetcounts[numsubsets] *= 3;

			lastattr = adata[i];
			++numsubsets;

			subsetstarts[numsubsets] = i * 3;
			subsetcounts[numsubsets] = 0;
		}
		else
			++subsetcounts[numsubsets];
	}

	subsetcounts[numsubsets] *= 3;
	++numsubsets;

	std::cout << "\nnumber of subsets: " << numsubsets << "\n";

	for( DWORD i = 0; i < numsubsets; ++i )
	{
		std::cout << "   subset " << i << ": " << subsetstarts[i] << " - " << subsetcounts[i] << "\n";
	}

	// for each subset write out data
	float x, y, z, w;
	DWORD count;

	std::ofstream of("meshes/output/output.obj");
	of << "#\n# DummyFramework X to OBJ converter\n#\n\n";

	if( options & D3DXMESH_32BIT )
	{
		// TODO: same as 16 bit but with DWORD cache and index
	}
	else
	{
		typedef std::list<WORD> wordlist;

		wordlist cache;
		WORD index;

		for( DWORD i = 0; i < numsubsets; ++i )
		{
			of << "# subset " << i << " to come\n";

			/*
			// filter out duplicate vertices
			// probably this isn't the most efficient method
			cache.clear();

			for( DWORD j = 0; j < subsetcounts[i]; ++j )
			{
				index = *(((WORD*)idata) + subsetstarts[i] + j);

				if( cache.end() == std::find(cache.begin(), cache.end(), index) )
					cache.push_back(index);
			}
			*/

			// positions
			count = 0;

			for( DWORD k = 0; k < mesh->GetNumVertices(); ++k )
			{
				index = k;

			//for( wordlist::iterator it = cache.begin(); it != cache.end(); ++it )
			//{
				//index = *it;
				
				x = *((float*)(vdata + stride * index + posoff));
				y = *((float*)(vdata + stride * index + posoff + sizeof(float)));
				z = *((float*)(vdata + stride * index + posoff + 2 * sizeof(float)));

				if( possize == 4 )
				{
					w = *((float*)(vdata + stride * index + posoff + 3 * sizeof(float)));

					// homogeneous to euclidean
					x /= w;
					y /= w;
					z /= w;
				}

				of << "v " << x << " " << y << " " << z << "\n";
				++count;
			}

			of << "# " << count << " vertices\n\n";

			// texcoords
			count = 0;

			if( texsize > 0 )
			{
				count = 0;

				for( DWORD k = 0; k < mesh->GetNumVertices(); ++k )
				{
					index = k;

				//for( wordlist::iterator it = cache.begin(); it != cache.end(); ++it )
				//{
				//	index = *it;

					x = *((float*)(vdata + stride * index + texoff));

					if( texsize > 1 )
						y = *((float*)(vdata + stride * index + texoff + sizeof(float)));
					else
						y = 0;
					
					of << "vt " << x << " " << y << "\n";
					++count;
				}
			}

			of << "# " << count << " texcoords\n\n";

			// normals
			count = 0;

			if( normsize > 0 )
			{
				count = 0;

				for( DWORD k = 0; k < mesh->GetNumVertices(); ++k )
				{
					index = k;
				//for( wordlist::iterator it = cache.begin(); it != cache.end(); ++it )
				//{
				//	index = *it;

					x = *((float*)(vdata + stride * index + normoff));
					y = *((float*)(vdata + stride * index + normoff + sizeof(float)));
					z = *((float*)(vdata + stride * index + normoff + 2 * sizeof(float)));

					of << "vn " << x << " " << y << " " << z << "\n";
					++count;
				}
			}

			of << "# " << count << " normals\n\n";

			// faces
			count = 0;

			of << "g subset" << i << "\n";

			for( DWORD j = 0; j < subsetcounts[i]; j += 3 )
			{
				index = *(((WORD*)idata) + subsetstarts[i] + j) + 1;
				of << "f " << index << "/" << index << "/" << index << " ";

				index = *(((WORD*)idata) + subsetstarts[i] + j + 1) + 1;
				of << index << "/" << index << "/" << index << " ";

				index = *(((WORD*)idata) + subsetstarts[i] + j + 2) + 1;
				of << index << "/" << index << "/" << index << "\n";

				++count;
			}

			of << "# " << count << " faces\n\n";
			of << "g\n\n\n";
		}
	}

	mesh->UnlockAttributeBuffer();
	mesh->UnlockIndexBuffer();
	mesh->UnlockVertexBuffer();

	of.close();

	return CGame9::LoadContent();
}
//=============================================================================================================
void ConvertX2OBJ::Update()
{
	// TODO: fixed time step game logic
	CGame9::Update();
}
//=============================================================================================================
void ConvertX2OBJ::Draw()
{
	// TODO: interpolation

	Graphics->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0xff6694ed, 1.0f, 0);

	Graphics->BeginScene();
	{
		// TODO: draw
		CGame9::Draw();
	}
	Graphics->EndScene();
	
	Graphics->Present(NULL, NULL, NULL, NULL);
}
//=============================================================================================================