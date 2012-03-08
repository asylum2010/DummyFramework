//=============================================================================================================
#include "convertx2obj.h"
#include <fstream>
#include <algorithm>

ConvertX2OBJ::ConvertX2OBJ()
{
	Application.keyup.connect(this, &ConvertX2OBJ::application_keyup);
	Application.keydown.connect(&camera, &DummyFramework::CModelViewerCamera::onkeydown);
	Application.keyup.connect(&camera, &DummyFramework::CModelViewerCamera::onkeyup);
	Application.mousedown.connect(&camera, &DummyFramework::CModelViewerCamera::onmousedown);
	Application.mouseup.connect(&camera, &DummyFramework::CModelViewerCamera::onmouseup);
	Application.mousemove.connect(&camera, &DummyFramework::CModelViewerCamera::onmousemove);
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

	camera.ZoomSpeed = 0.05f;
	camera.SetDistance(1.4f);
	camera.SetClip(0.1f, 100);
	camera.SetAspect((float)DisplayMode.Width / (float)DisplayMode.Height);
	camera.SetPosition(0, 0, 0);
	camera.SetFov(D3DX_PI / 2);
	camera.OrbitRight(-1.2f * D3DX_PI);
	camera.OrbitUp(0.45f);
	camera.ClampPitch(-0.55f, D3DX_PI / 2);

	Graphics->SetRenderState(D3DRS_LIGHTING, false);
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

	object.Initialize(*this);

	bool success = Content.CreateObject(object, "meshes/knot.x");
	dassert(false, "ConvertX2OBJ::LoadContent(): Could not load object", success);
	
	//object.Mesh = Content.LoadMesh("meshes/airplane.X");
	//dassert(false, "ConvertX2OBJ::LoadContent(): Could not load mesh", object.Mesh);
	
	LPD3DXMESH mesh = object.Mesh;

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

	std::cout << "\nNumber of entries: " << entries << "\n";
	std::cout << "Position offset: " << posoff << ", " << possize << "\nTexcoord offset: " << texoff <<
		", " << texsize << "\nNormal offset: " << normoff << ", " << normsize << "\n";

	std::cout << "\nNumber of vertices: " << mesh->GetNumVertices() << "\n";
	std::cout << "Vertex stride: " << mesh->GetNumBytesPerVertex() << "\n";
	std::cout << "Number of faces: " << mesh->GetNumFaces() << "\n\n";

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

	std::cout << "\nNumber of subsets: " << numsubsets << "\n";

	for( DWORD i = 0; i < numsubsets; ++i )
	{
		std::cout << "   subset " << i << ": " << subsetstarts[i] << " - " << subsetcounts[i] << "\n";
	}

	// for each subset write out data
	float x, y, z, w;
	DWORD count;

	CreateDirectory("meshes/output", NULL);

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
		DWORD index;

		for( DWORD i = 0; i < numsubsets; ++i )
		{
			of << "# subset " << i << " to come\n";
			
			// positions
			count = 0;

			for( DWORD k = 0; k < mesh->GetNumVertices(); ++k )
			{
				index = k;
							
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
				of << "f " << index;

				if( texsize > 0 )
					of << "/" << index;
				else if( normsize > 0 )
					of << "/";

				if( normsize > 0 )
					of << "/" << index;

				index = *(((WORD*)idata) + subsetstarts[i] + j + 1) + 1;
				of << " " << index;

				if( texsize > 0 )
					of << "/" << index;
				else if( normsize > 0 )
					of << "/";

				if( normsize > 0 )
					of << "/" << index;

				index = *(((WORD*)idata) + subsetstarts[i] + j + 2) + 1;
				of << " " << index;

				if( texsize > 0 )
					of << "/" << index;
				else if( normsize > 0 )
					of << "/";

				if( normsize > 0 )
					of << "/" << index;

				of << "\n";

				++count;
			}

			of << "# " << count << " faces\n\n";
			of << "g\n\n\n";
		}
	}

	D3DXMATRIX tmp;
	D3DXVECTOR3 center;
	float radius;

	D3DXComputeBoundingSphere((D3DXVECTOR3*)(vdata + posoff), mesh->GetNumVertices(), stride, &center, &radius);
	std::cout << "\nBounding sphere radius: " << radius << "\n";

	mesh->UnlockAttributeBuffer();
	mesh->UnlockIndexBuffer();
	mesh->UnlockVertexBuffer();

	of.close();

	radius = 0.7f / radius;

	D3DXMatrixScaling(&world, radius, radius, radius);
	D3DXVec3TransformCoord(&center, &center, &world);

	D3DXMatrixTranslation(&tmp, -center.x, -center.y, -center.z);
	D3DXMatrixMultiply(&world, &world, &tmp);

	return CGame9::LoadContent();
}
//=============================================================================================================
void ConvertX2OBJ::Update()
{
	camera.Update();
	CGame9::Update();
}
//=============================================================================================================
void ConvertX2OBJ::Draw()
{
	camera.UpdateTransforms((float)Sync.Alpha());
	camera.GetViewMatrix(view);
	camera.GetProjectionMatrix(proj);

	Graphics->SetTransform(D3DTS_WORLD, &world);
	Graphics->SetTransform(D3DTS_VIEW, &view);
	Graphics->SetTransform(D3DTS_PROJECTION, &proj);

	Graphics->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0xff6694ed, 1.0f, 0);

	Graphics->BeginScene();
	{
		object.Draw();
		CGame9::Draw();
	}
	Graphics->EndScene();
	
	Graphics->Present(NULL, NULL, NULL, NULL);
}
//=============================================================================================================
