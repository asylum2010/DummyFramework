//=============================================================================================================
#include "caustics.h"

void Caustics::application_keyup(const DummyFramework::skeyboardstate& kstate)
{
	switch( kstate.key )
	{
	case VK_ESCAPE:
		screenfader->Reset(DummyFramework::CScreenFader9::Out);
		break;

	default:
		break;
	}
}
//=============================================================================================================
void Caustics::screenfader_faded(DummyFramework::CScreenFader9::efademode mode)
{
	if( mode == DummyFramework::CScreenFader9::Out )
		Application.Close();
}
//=============================================================================================================
void Caustics::causticsrenderer_rendercolor(size_t index)
{
	Graphics->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);

	if( index == 1 )
	{
		D3DXMatrixMultiply(&viewproj, &causticsrenderer->View, &causticsrenderer->Projection);

		objects[0].Material()->SetMatrix("matViewProj", &viewproj);
		objects[0].Material()->SetMatrix("matWorld", &objects[0].GetTransform());
		objects[0].Material()->SetMatrix("matWorldInv", &objects[0].GetInvTransform());
		objects[0].Material()->SetVector("eyePos", (D3DXVECTOR4*)&causticsrenderer->Position);

		objects[0].Draw();
	}
	else if( index == 2 )
	{
		D3DXMatrixMultiply(&viewproj, &causticsrenderer->View, &causticsrenderer->Projection);

		objects[1].Translate(causticsrenderer->Position);
		objects[1].Material()->SetMatrix("matWorld", &objects[1].GetTransform());
		objects[1].Material()->SetMatrix("matViewProj", &viewproj);
		objects[1].Material()->SetVector("eyePos", (D3DXVECTOR4*)&causticsrenderer->Position);

		objects[2].Material()->SetTechnique("diffuse");
		objects[2].Material()->SetMatrix("matViewProj", &viewproj);

		objects[1].Draw();
		objects[2].Draw();
	}
}
//=============================================================================================================
void Caustics::causticsrenderer_renderdistance(size_t index)
{
	LPD3DXEFFECT effect = causticsrenderer->GetEffect();
	Graphics->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);

	if( index == 0 )
	{
		StateManager->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

		effect->SetMatrix("matWorld", &objects[0].GetTransform());
		effect->SetMatrix("matWorldInv", &objects[0].GetInvTransform());
		effect->CommitChanges();

		objects[0].DrawOptimized();
		StateManager->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}
	else if( index == 1 )
	{
		effect->SetMatrix("matWorld", &objects[0].GetTransform());
		effect->SetMatrix("matWorldInv", &objects[0].GetInvTransform());
		effect->CommitChanges();

		objects[0].DrawOptimized();
	}
	else if( index == 2 )
	{
		objects[1].Translate(causticsrenderer->Position);

		effect->SetMatrix("matWorld", &objects[1].GetTransform());
		effect->SetMatrix("matWorldInv", &objects[1].GetInvTransform());
		effect->CommitChanges();

		objects[1].DrawOptimized();

		effect->SetMatrix("matWorld", &objects[2].GetTransform());
		effect->SetMatrix("matWorldInv", &objects[2].GetInvTransform());
		effect->CommitChanges();

		objects[2].DrawOptimized();
	}
}
//=============================================================================================================
void Caustics::causticsrenderer_photontrace()
{
	light.GetViewMatrix(lightv);
	light.GetProjectionMatrix(proj);
	light.GetEyePosition(lightpos);

	D3DXMatrixMultiply(&lightvp, &lightv, &proj);
	LPD3DXEFFECT effect = causticsrenderer->GetEffect();

	effect->SetMatrix("matWorld", &objects[0].GetTransform());
	effect->SetMatrix("matWorldInv", &objects[0].GetInvTransform());
	effect->SetMatrix("lightVP", &lightvp);
	effect->SetVector("lightPos", (D3DXVECTOR4*)&lightpos);
	effect->CommitChanges();

	Graphics->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
	objects[0].DrawOptimized();
}
//=============================================================================================================
void Caustics::causticsrenderer_rendershadow()
{
	LPD3DXEFFECT effect = causticsrenderer->GetEffect();

	effect->SetMatrix("matWorld", &objects[2].GetTransform());
	effect->SetMatrix("lightVP", &lightvp);
	effect->SetVector("lightPos", (D3DXVECTOR4*)&lightpos);
	effect->CommitChanges();

	objects[2].DrawOptimized();
}
//=============================================================================================================
