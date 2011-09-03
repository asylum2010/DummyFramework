//=============================================================================================================
#include "networktest.h"
#include <dummyframework/base/CHelper.h>

void NetworkTest::operator ()()
{
	if( isserver )
		run_server();
	else
		run_client();
}
//=============================================================================================================
void NetworkTest::run_client()
{
	std::cout << "* Connecting...\n";
	service->Connect(hostip.c_str(), 8080);

	entityDTO dto;
	size_t cnt;

	while( true )
	{
		if( service->IsConnected() )
		{
			cnt = service->GetNumConnections();

			if( cnt >= airplanes.size() )
				airplanes.resize(cnt + 1);

			// update other players' states
			for( size_t i = 0; i < cnt; ++i )
			{
				if( service->ReceiveState(dto, i) )
				{
					airplanes[i + 1].UpdateState(
						dto.input, dto.position, dto.orientation, Sync.Timer().Time());
				}
			}

			// send own state
			if( cansend )
			{
				AirPlane& avatar = airplanes[0];
				avatar.MakeState(dto.input, dto.position, dto.orientation);

				service->SendState(dto);
				cansend = false;
			}
		}

		service->UpdateClient(true);
	}
}
//=============================================================================================================
void NetworkTest::run_server()
{
	std::cout << "* Listening...\n";
	service->Connect("", 8080);

	entityDTO dto;
	size_t cnt;

	while( true )
	{
		cnt = service->GetNumConnections();

		// new player
		if( cnt > airplanes.size() - 1 )
			airplanes.resize(cnt + 1);

		// update other players' states
		for( size_t i = 0; i < cnt; ++i )
		{
			if( service->ReceiveState(dto, i) )
			{
				airplanes[i + 1].UpdateState(
					dto.input, dto.position, dto.orientation, Sync.Timer().Time());
			}
		}

		// send player states to everyone
		for( size_t i = 0; i < airplanes.size(); ++i )
		{
			airplanes[i].MakeState(dto.input, dto.position, dto.orientation);

			for( size_t j = 0; j < cnt; ++j )
			{
				if( j + 1 != i )
					service->SendState(dto, j);
			}
		}
			
		service->UpdateServer(true);
	}
}
//=============================================================================================================
