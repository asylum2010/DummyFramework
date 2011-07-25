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
	connection.connect(hostip.c_str(), 8080);

	// ((packetsize * 8) / 1024) / UpdateInterval;
	client.SendRate = 20.0f; // kbps
	client.attach(connection);

	bool connected = false;
	std::string str;
	size_t myid = 0;

	while( 1 )
	{
		if( connected )
		{
			channel >> str;

			if( str.length() > 0 )
			{
				//std::cout << "* Message from server: " <<
				//	str << "\n";

				switch( str[0] )
				{
				case 'a':
					// a szerver fogadott minket
					myid = (size_t)atoi(str.substr(1).c_str());

					std::cout << "* <<accept>>: myid == " << myid << "\n";
					break;

				case 's': {
					// updateljük az i. állapotát
					// ha nincs még olyan akkor létrehozza
					std::vector<std::string> v;
					DummyFramework::CHelper::Split(v, '|', str, true);

					size_t id = (size_t)atoi(v[0].substr(1).c_str());

					if( id == 0 )
						std::cout << "* <<state>>: Invalid id\n";
					else
					{
						if( id >= airplanes.size() )
							airplanes.resize(id + 1);

						airplanes[id].UpdateFromPack(v[1], Sync.Timer().Time());
					}

					} break;

				default:
					break;
				}
			}
			
			if( cansend )
			{
				// küldjük az állapotunkat
				AirPlane& avatar = airplanes[0];

				str = "s" + DummyFramework::CHelper::DiscreteToString<size_t>(myid) + "|";
				avatar.MakeStatePack(str);

				channel << str;
				cansend = false;
			}
		}
		else if( connected = client.join(channel) )
			std::cout << "* Connected...\n";

		client.run();
	}
}
//=============================================================================================================
void NetworkTest::run_server()
{
	std::cout << "* Listening...\n";
	connection.connect(0, 0, 8080);

	server.attach(connection);
	server.SendRate = 50.0f; // kbps

	DummyFramework::udpchannel channels[10];
	size_t count = 0;
	std::string str;

	while( 1 )
	{
		if( server.accept(channels[count]) )
		{
			std::cout << "* " << channels[count].getaddress().tostring() <<
				" connected...\n";

			airplanes.push_back(AirPlane());
			AirPlane& avatar = *airplanes.begin();

			// kiküldjük neki az id-jét
			str = "a" + DummyFramework::CHelper::DiscreteToString<size_t>(count + 1);
			channels[count] << str;

			// és elküldjük a már kapcsolodott kliensek állapotát
			for( size_t i = 0; i < count + 1; ++i )
			{
				str = "s" + DummyFramework::CHelper::DiscreteToString<size_t>(i + 1) + "|";

				airplanes[i].MakeStatePack(str);
				channels[count] << str;
			}

			// a többi kliensnek lejjebb küldödik
			++count;
		}

		for( size_t i = 0; i < count; ++i )
		{
			channels[i] >> str;

			if( str.length() > 0 )
			{
				//std::cout << "* Message from " <<
				//	channels[i].getaddress().tostring() << ": " << str << "\n";

				switch( str[0] )
				{
				case 's': {
					// udapeteljük az i-edik állapotát
					std::vector<std::string> v;
					DummyFramework::CHelper::Split(v, '|', str, true);

					size_t id = (size_t)atoi(v[0].substr(1).c_str());

					if( id == 0 || id >= airplanes.size() || (id != i + 1) )
						std::cout << "* <<state>>: Invalid id\n";
					else
					{
						airplanes[id].UpdateFromPack(v[1], Sync.Timer().Time());
					}

					// mindenkinek továbbküldjük
					for( size_t j = 0; j < count; ++j )
					{
						if( j != i )
							channels[j] << str;
					}
					
					} break;

				default:
					break;
				}
			}
		}

		if( cansend )
		{
			// küldjük mindenkinek a saját állapotot
			str = "s1|";

			AirPlane& avatar = airplanes[0];
			avatar.MakeStatePack(str);

			for( size_t i = 0; i < count; ++i )
				channels[i] << str;

			cansend = false;
		}

		server.run();
	}
}
//=============================================================================================================
