//=============================================================================================================
#include "emptyproject10.h"

void EmptyProject10::application_keyup(const DummyFramework::skeyboardstate& kstate)
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
