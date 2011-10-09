//=============================================================================================================
#include "godray.h"

void Godray::DrawGodray()
{
	objects[0].Material(0)->SetTechnique("blinnphong");
	
	Graphics->BeginScene();
	{
		reflectionrenderer->SetViewMatrix(view);
		reflectionrenderer->SetProjectionMatrix(proj);
		reflectionrenderer->SetEyePosition(eye);
		reflectionrenderer->Draw();

		D3DXMatrixMultiply(&viewproj, &view, &proj);
		
		objects[2].Textures(1) = reflectionrenderer->ReflectionTexture;
		objects[2].Material()->SetMatrix("matViewProj", &viewproj);

		objects[0].Material(0)->SetVector("eyePos", (D3DXVECTOR4*)&eye);
		objects[0].Material(0)->SetMatrix("matViewProj", &viewproj);

		objects[1].Translate(eye);
		objects[1].Material()->SetMatrix("matWorld", &objects[1].GetTransform());
		objects[1].Material()->SetMatrix("matViewProj", &viewproj);
		objects[1].Material()->SetVector("eyePos", (D3DXVECTOR4*)&eye);

		Graphics->GetRenderTarget(0, &godrayrenderer->BackBuffer);
		Graphics->SetRenderTarget(0, godrayrenderer->Color);
		Graphics->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0xff6c6c6c, 1.0f, 0);

		objects[1].Draw();
		objects[2].Draw();
		objects[0].Draw();
		
		objects[0].Material(0)->SetTechnique("alpha");
		
		Graphics->SetRenderTarget(0, godrayrenderer->Occluders);
		Graphics->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0xff6c6c6c, 1.0f, 0);

		objects[0].Draw();
		
		Graphics->SetVertexDeclaration(DummyFramework::CScreenQuad9::Declaration);
		CGame9::Draw();
	}
	Graphics->EndScene();

	safe_release(godrayrenderer->BackBuffer);
}
//=============================================================================================================
void Godray::DrawGodrayMRT()
{
	objects[0].Material(0)->SetTechnique("blinnphong_mrt");
	
	Graphics->BeginScene();
	{
		reflectionrenderer->SetViewMatrix(view);
		reflectionrenderer->SetProjectionMatrix(proj);
		reflectionrenderer->SetEyePosition(eye);
		reflectionrenderer->Draw();

		D3DXMatrixMultiply(&viewproj, &view, &proj);
		
		objects[2].Textures(1) = reflectionrenderer->ReflectionTexture;
		objects[2].Material()->SetMatrix("matViewProj", &viewproj);

		objects[0].Material(0)->SetVector("eyePos", (D3DXVECTOR4*)&eye);
		objects[0].Material(0)->SetMatrix("matViewProj", &viewproj);

		objects[1].Translate(eye);
		objects[1].Material()->SetMatrix("matWorld", &objects[1].GetTransform());
		objects[1].Material()->SetMatrix("matViewProj", &viewproj);
		objects[1].Material()->SetVector("eyePos", (D3DXVECTOR4*)&eye);

		Graphics->GetRenderTarget(0, &godrayrenderer->BackBuffer);
		Graphics->SetRenderTarget(0, godrayrenderer->Color);
		Graphics->SetRenderTarget(1, godrayrenderer->Occluders);

		Graphics->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0xff6c6c6c, 1.0f, 0);

		objects[1].Draw();
		objects[2].Draw();
		objects[0].Draw();
		
		Graphics->SetRenderTarget(1, NULL);
		Graphics->SetVertexDeclaration(DummyFramework::CScreenQuad9::Declaration);

		CGame9::Draw();
	}
	Graphics->EndScene();

	safe_release(godrayrenderer->BackBuffer);
}
//=============================================================================================================
void Godray::DrawGodrayBloom()
{
	objects[0].Material(0)->SetTechnique("blinnphong");
	
	Graphics->BeginScene();
	{
		reflectionrenderer->SetViewMatrix(view);
		reflectionrenderer->SetProjectionMatrix(proj);
		reflectionrenderer->SetEyePosition(eye);
		reflectionrenderer->Draw();

		D3DXMatrixMultiply(&viewproj, &view, &proj);
		
		objects[2].Textures(1) = reflectionrenderer->ReflectionTexture;
		objects[2].Material()->SetMatrix("matViewProj", &viewproj);

		objects[0].Material(0)->SetVector("eyePos", (D3DXVECTOR4*)&eye);
		objects[0].Material(0)->SetMatrix("matViewProj", &viewproj);

		objects[1].Translate(eye);
		objects[1].Material()->SetMatrix("matWorld", &objects[1].GetTransform());
		objects[1].Material()->SetMatrix("matViewProj", &viewproj);
		objects[1].Material()->SetVector("eyePos", (D3DXVECTOR4*)&eye);

		bloomrenderer->BackBuffer = godrayrenderer->Color;

		Graphics->GetRenderTarget(0, &godrayrenderer->BackBuffer);
		Graphics->SetRenderTarget(0, bloomrenderer->LDR);
		Graphics->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0xff6c6c6c, 1.0f, 0);

		objects[1].Draw();
		objects[2].Draw();
		objects[0].Draw();
		
		objects[0].Material(0)->SetTechnique("alpha");
		
		Graphics->SetRenderTarget(0, godrayrenderer->Occluders);
		Graphics->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0xff6c6c6c, 1.0f, 0);

		objects[0].Draw();
		
		Graphics->SetVertexDeclaration(DummyFramework::CScreenQuad9::Declaration);
		CGame9::Draw();
	}
	Graphics->EndScene();

	safe_release(godrayrenderer->BackBuffer);
}
//=============================================================================================================
void Godray::DrawGodrayBloomMRT()
{
	objects[0].Material(0)->SetTechnique("blinnphong_mrt");
	
	Graphics->BeginScene();
	{
		reflectionrenderer->SetViewMatrix(view);
		reflectionrenderer->SetProjectionMatrix(proj);
		reflectionrenderer->SetEyePosition(eye);
		reflectionrenderer->Draw();

		D3DXMatrixMultiply(&viewproj, &view, &proj);
		
		objects[2].Textures(1) = reflectionrenderer->ReflectionTexture;
		objects[2].Material()->SetMatrix("matViewProj", &viewproj);

		objects[0].Material(0)->SetVector("eyePos", (D3DXVECTOR4*)&eye);
		objects[0].Material(0)->SetMatrix("matViewProj", &viewproj);

		objects[1].Translate(eye);
		objects[1].Material()->SetMatrix("matWorld", &objects[1].GetTransform());
		objects[1].Material()->SetMatrix("matViewProj", &viewproj);
		objects[1].Material()->SetVector("eyePos", (D3DXVECTOR4*)&eye);

		bloomrenderer->BackBuffer = godrayrenderer->Color;

		Graphics->GetRenderTarget(0, &godrayrenderer->BackBuffer);
		Graphics->SetRenderTarget(0, bloomrenderer->LDR);
		Graphics->SetRenderTarget(1, godrayrenderer->Occluders);

		Graphics->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0xff6694ed, 1.0f, 0);

		objects[1].Draw();
		objects[2].Draw();
		objects[0].Draw();
		
		Graphics->SetRenderTarget(1, NULL);
		Graphics->SetVertexDeclaration(DummyFramework::CScreenQuad9::Declaration);

		CGame9::Draw();
	}
	Graphics->EndScene();

	safe_release(godrayrenderer->BackBuffer);
}
//=============================================================================================================
void Godray::DrawScene()
{
	objects[0].Material(0)->SetTechnique("blinnphong_mrt");
	
	Graphics->BeginScene();
	{
		reflectionrenderer->SetViewMatrix(view);
		reflectionrenderer->SetProjectionMatrix(proj);
		reflectionrenderer->SetEyePosition(eye);
		reflectionrenderer->Draw();

		D3DXMatrixMultiply(&viewproj, &view, &proj);
		
		objects[2].Textures(1) = reflectionrenderer->ReflectionTexture;
		objects[2].Material()->SetMatrix("matViewProj", &viewproj);

		objects[0].Material(0)->SetVector("eyePos", (D3DXVECTOR4*)&eye);
		objects[0].Material(0)->SetMatrix("matViewProj", &viewproj);

		objects[1].Translate(eye);
		objects[1].Material()->SetMatrix("matWorld", &objects[1].GetTransform());
		objects[1].Material()->SetMatrix("matViewProj", &viewproj);
		objects[1].Material()->SetVector("eyePos", (D3DXVECTOR4*)&eye);
		
		Graphics->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0xff6694ed, 1.0f, 0);

		objects[1].Draw();
		objects[2].Draw();
		objects[0].Draw();
	}
	Graphics->EndScene();

	safe_release(godrayrenderer->BackBuffer);
}
//=============================================================================================================
