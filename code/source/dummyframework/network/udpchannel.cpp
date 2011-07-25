//=============================================================================================================
#include <dummyframework/network/udpchannel.h>
#include <dummyframework/network/udpconnection.h>

namespace DummyFramework
{
	udpchannel::udpchannel()
	{
		parent = NULL;
	}
	//=============================================================================================================
	udpchannel::udpchannel(udpstation& station, unsigned short id)
	{
		parent = &station;
		client_id = id;
	}
	//=============================================================================================================
	udpchannel::~udpchannel()
	{
	}
	//=============================================================================================================
	udpchannel& udpchannel::operator =(const udpchannel& other)
	{
		parent = other.parent;
		client_id = other.client_id;

		return *this;
	}
	//=============================================================================================================
	udpchannel& udpchannel::operator <<(const std::string& str)
	{
		// TODO: csomagokra bontás

		if( parent )
		{
			udppacket packet;
			packet << str;

			parent->addpacket(client_id, packet);
		}

		return *this;
	}
	//=============================================================================================================
	udpchannel& udpchannel::operator >>(std::string& str)
	{
		// TODO: összerakás

		if( parent )
		{
			udppacket packet;
			str = "";

			if( parent->getpacket(client_id, packet) )
				packet >> str;
		}

		return *this;
	}
	//=============================================================================================================
	ipv4address udpchannel::getaddress() const
	{
		ipv4address addr;

		parent->getaddress(client_id, addr);
		return addr;
	}
	//=============================================================================================================
}