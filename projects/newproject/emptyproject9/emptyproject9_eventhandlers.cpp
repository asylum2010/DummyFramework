//=============================================================================================================
#include "EmptyProject9.h"

void EmptyProject9::application_keyup(const DummyFramework::skeyboardstate& kstate)
{
	switch( kstate.key )
	{
	case VK_ESCAPE:
		Application.Close();
		break;

	default:
		break;
	}
}
//=============================================================================================================
