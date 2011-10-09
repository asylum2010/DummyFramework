//=============================================================================================================
#include "water.h"

void Water::application_keyup(const DummyFramework::skeyboardstate& kstate)
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
void Water::screenfader_faded(DummyFramework::CScreenFader9::efademode mode)
{
	if( mode == DummyFramework::CScreenFader9::Out )
		Application.Close();
}
//=============================================================================================================
void Water::waterrenderer_renderreflection()
{
	D3DXMatrixMultiply(&viewproj, &waterrenderer->View, &waterrenderer->Projection);

	objects[1].Translate(waterrenderer->Eye);
	objects[1].Material()->SetMatrix("matWorld", &objects[1].GetTransform());
	objects[1].Material()->SetMatrix("matViewProj", &viewproj);
	objects[1].Material()->SetVector("eyePos", (D3DXVECTOR4*)&waterrenderer->Eye);

	objects[0].Material()->SetVector("eyePos", (D3DXVECTOR4*)&waterrenderer->Eye);
	objects[0].Material()->SetMatrix("matViewProj", &viewproj);
	
	Graphics->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0xff000000, 1.0f, 0);

	objects[1].Draw();
	objects[0].Draw();
}
//=============================================================================================================
