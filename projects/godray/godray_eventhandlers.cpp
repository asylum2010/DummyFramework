//=============================================================================================================
#include "godray.h"

void Godray::application_keyup(const DummyFramework::skeyboardstate& kstate)
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
void Godray::screenfader_faded(DummyFramework::CScreenFader9::efademode mode)
{
	if( mode == DummyFramework::CScreenFader9::Out )
		Application.Close();
}
//=============================================================================================================
void Godray::reflectionrenderer_renderreflection()
{
	D3DXMatrixMultiply(&viewproj, &reflectionrenderer->View, &reflectionrenderer->Projection);

	objects[1].Translate(reflectionrenderer->Eye);
	objects[1].Material()->SetMatrix("matWorld", &objects[1].GetTransform());
	objects[1].Material()->SetMatrix("matViewProj", &viewproj);
	objects[1].Material()->SetVector("eyePos", (D3DXVECTOR4*)&reflectionrenderer->Eye);

	objects[0].Material(0)->SetVector("eyePos", (D3DXVECTOR4*)&reflectionrenderer->Eye);
	objects[0].Material(0)->SetMatrix("matViewProj", &viewproj);

	Graphics->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0xff000000, 1.0f, 0);

	objects[1].Draw();
	objects[0].Draw();
}
//=============================================================================================================
