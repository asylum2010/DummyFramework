//=============================================================================================================
#include <dummyframework/base/d3d9/CObject9.h>
#include <dummyframework/base/d3d9/CGame9.h>
#include <dummyframework/base/d3d9/CStateManager9.h>

namespace DummyFramework
{
	CObject9::CObject9()
	{
		device = NULL;
		manager = NULL;

		Mesh = NULL;
		Unique = true;

		subsets.push_back(CSubset9());
	}
	//=============================================================================================================
	void CObject9::Initialize(CGame9& mygame)
	{
		device = mygame.Graphics;
		manager = mygame.StateManager;
	}
	//=============================================================================================================
	void CObject9::ApplyMaterial(size_t id)
	{
		LPD3DXEFFECT effect = subsets[id].Material;

		if( effect )
		{
			effect->Begin(NULL, _DUMMY_EFFECT_DONT_SAVE_);
			effect->BeginPass(0);
		}
	}
	//=============================================================================================================
	void CObject9::UnapplyMaterial(size_t id)
	{
		LPD3DXEFFECT effect = subsets[id].Material;

		if( effect )
		{
			effect->EndPass();
			effect->End();
		}
	}
	//=============================================================================================================
	void CObject9::Draw()
	{
		if( !Mesh )
			return;

		if( Unique )
		{
			if( subsets[0].Material )
			{
				subsets[0].Material->Begin(NULL, _DUMMY_EFFECT_DONT_SAVE_);
				subsets[0].Material->BeginPass(0);

				for( size_t i = 0; i < subsets.size(); ++i )
				{
					const CSubset9& subset = subsets[i];

					for( unsigned char j = 0; j < 8; ++j )
						manager->SetTexture(j, subset.Textures[j]);

					Mesh->DrawSubset((DWORD)i);
				}

				subsets[0].Material->EndPass();
				subsets[0].Material->End();
			}
			else
			{
				for( size_t i = 0; i < subsets.size(); ++i )
				{
					const CSubset9& subset = subsets[i];

					for( unsigned char j = 0; j < 8; ++j )
						manager->SetTexture(j, subset.Textures[j]);

					Mesh->DrawSubset((DWORD)i);
				}
			}
		}
		else
		{
			for( size_t i = 0; i < subsets.size(); ++i )
			{
				const CSubset9& subset = subsets[i];

				for( unsigned char j = 0; j < 8; ++j )
					manager->SetTexture(j, subset.Textures[j]);

				if( subset.Material )
				{
					subset.Material->Begin(NULL, _DUMMY_EFFECT_DONT_SAVE_);
					subset.Material->BeginPass(0);

					Mesh->DrawSubset((DWORD)i);

					subset.Material->EndPass();
					subset.Material->End();
				}
				else
					Mesh->DrawSubset((DWORD)i);
			}
		}
	}
	//=============================================================================================================
	void CObject9::Draw(size_t index)
	{
		if( !Mesh )
			return;

		const CSubset9& subset = subsets[index];

		for( unsigned char j = 0; j < 8; ++j )
			manager->SetTexture(j, subset.Textures[j]);

		if( subset.Material )
		{
			subset.Material->Begin(NULL, _DUMMY_EFFECT_DONT_SAVE_);
			subset.Material->BeginPass(0);

			Mesh->DrawSubset((DWORD)index);

			subset.Material->EndPass();
			subset.Material->End();
		}
		else
			Mesh->DrawSubset((DWORD)index);
	}
	//=============================================================================================================
	void CObject9::DrawOptimized()
	{
		if( !Mesh )
			return;

		for( size_t i = 0; i < subsets.size(); ++i )
		{
			const CSubset9& subset = subsets[i];

			for( unsigned char j = 0; j < 8; ++j )
				manager->SetTexture(j, subset.Textures[j]);

			Mesh->DrawSubset((DWORD)i);
		}
	}
	//=============================================================================================================
}
