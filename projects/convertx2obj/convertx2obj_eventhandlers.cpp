//=============================================================================================================
#include "convertx2obj.h"

void ConvertX2OBJ::application_keyup(const DummyFramework::skeyboardstate& kstate)
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
