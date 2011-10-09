//=============================================================================================================
#include "networktest.h"

void NetworkTest::application_keyup(const DummyFramework::skeyboardstate& kstate)
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
void NetworkTest::application_closed()
{
	worker.Stop();
	service->Disconnect();
}
//=============================================================================================================
