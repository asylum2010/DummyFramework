//=============================================================================================================
#include <dummyframework/network/udpclient.h>
#include <iostream>

namespace DummyFramework
{
	CTimer udpclient::clock;

	udpclient::udpclient()
	{
		SendRate = 10;
		TimeOut = 5;

		abletosend = false;
		connection = 0;
	}
	//=============================================================================================================
	udpclient::udpclient(udpconnection& conn)
	{
		udpclient();
		attach(conn);
	}
	//=============================================================================================================
	udpclient::~udpclient()
	{
	}
	//=============================================================================================================
	bool udpclient::attach(udpconnection& conn)
	{
		connection = &conn;
		lasttimeout = -10;

		if( clock.IsStopped() )
			clock.Start();

		last = clock.Time();
		client.address = conn.getaddr();
		connection_id = 0xffff;

		return true;
	}
	//=============================================================================================================
	bool udpclient::join(udpchannel& out)
	{
		if( connection_id != 0xffff )
		{
			out = udpchannel(*this, 0);
			return true;
		}

		return false;
	}
	//=============================================================================================================
	bool udpclient::addpacket(unsigned long client_id, const udppacket& packet)
	{
		if( client.outgoing.size() < 10 )
		{
			client.outgoing.push(packet);
			return true;
		}

		return false;
	}
	//=============================================================================================================
	bool udpclient::getpacket(unsigned long client_id, udppacket& packet)
	{
		if( client.incoming.size() > 0 )
		{
			packet = *client.incoming.begin();
			client.incoming.erase(client.incoming.begin());

			// enn�l kor�bbit m�r nem fogadunk
			// ezt nem igy kellene, hanem id� alapj�n
			client.firstseq = packet.sequence_no;
			return true;
		}

		return false;
	}
	//=============================================================================================================
	void udpclient::run()
	{
		// TODO: ha a protocol_id egyezik

		float packetsize = (float)_PACKET_SIZE_;
		float time = packetsize / (SendRate * 128.0f);
		double current = clock.Time();
		
		abletosend = false;

		// timeout eset�n ujra prob�l kapcsolodni
		// a szerver ezt nemfogja let�rolni
		if( connection_id == 0xffff )
		{
			if( lasttimeout + TimeOut < current )
			{
				lasttimeout = current;
				udppacket packet;

				packet.connection_id = 0xffff;
				packet.sequence_no = 0;

				client.outgoing.push(packet << "wanna connect");
			}
		}
		
		// k�ld�nk
		if( last + time < current )
		{
			abletosend = true;
			last = current;
			
			if( client.outgoing.size() > 0 )
			{
				udppacket& packet = client.outgoing.front();

				packet.connection_id = connection_id;
				packet.sequence_no = client.local;

				++client.local;

				if( client.needsack.size() > 0 )
				{
					packet.ack = client.needsack.front();
					client.needsack.pop();
				}
				else
					packet.ack = 0;

				connection->sendpacket(client.address, packet);
				client.outgoing.pop();
			}
		}

		// fogadunk
		ipv4address sender;
		udppacket packet;

		if( connection->recvpacket(packet, sender) )
		{
			if( connection_id == 0xffff )
			{
				//if( 0xffff == (connection_id = packet.connection_id) )
				//	return;

				// az "accept" nem �rdekes...de ha elveszett?
				// valszeg ugyis k�ld vmi adatot a szerver el�bb-utobb
				connection_id = packet.connection_id;
				return;
			}
			
			if( packet.connection_id == connection_id )
			{
				if( packet < client.firstseq )
				{
					networkdebug("Packet obsolete");
				}
				else if( client.incoming.size() < _INCOMING_SIZE )
				{
					// TODO: ack alapj�n rtt sz�mol�s
					client.incoming.insert(packet);
					
					if( client.needsack.size() == 20 )
						client.needsack.pop();

					client.needsack.push(packet.sequence_no);
				}
				else
					networkdebug("Incoming queue too large");
			}
		}
	}
	//=============================================================================================================
}
