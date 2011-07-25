//=============================================================================================================
#include "relief.h"

void Relief::application_keyup(const DummyFramework::skeyboardstate& kstate)
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
void Relief::screenfader_faded(DummyFramework::CScreenFader9::efademode mode)
{
	if( mode == DummyFramework::CScreenFader9::Out )
		Application.Close();
}
//=============================================================================================================
