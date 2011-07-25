//=============================================================================================================
#include "aircraft.h"

void Aircraft::application_keyup(const DummyFramework::skeyboardstate& kstate)
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
