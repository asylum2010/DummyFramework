//=============================================================================================================
#include <dummyframework/network/udpserver.h>
#include <iostream>

namespace DummyFramework
{
	CTimer udpserver::clock;

	udpserver::udpserver()
	{
		SendRate = 10;

		abletosend = false;
		connection = 0;
		currentclient = 0;

		clients.reserve(20);
	}
	//=============================================================================================================
	udpserver::udpserver(udpconnection& conn)
	{
		udpserver();
		attach(conn);
	}
	//=============================================================================================================
	udpserver::~udpserver()
	{
	}
	//=============================================================================================================
	bool udpserver::attach(udpconnection& conn)
	{
		connection = &conn;

		if( clock.IsStopped() )
			clock.Start();

		last = clock.Time();
		return true;
	}
	//=============================================================================================================
	bool udpserver::accept(udpchannel& out)
	{
		if( pending.size() > 0 )
		{
			size_t size = clients.size();

			if( size < clients.capacity() )
			{
				clients.push_back(slot());

				slot& client = clients[size];
				client.address = pending.front();

				out = udpchannel(*this, size);
				
				// kiküldünk egy csomagot, jelezve, hogy fogadtuk
				// (mert egyébként lehet, hogy sokáig nem is küldünk semmit)
				// a seq mind1, a másik oldal ignorolja
				// ha elveszett: elöbb-utobb csak küldünk valamit
				udppacket pack;

				pack.connection_id = size;
				pack.sequence_no = 0;
				pack.ack = 0;

				connection->sendpacket(client.address, pack << "accept");

				pending.pop();
				return true;
			}
		}

		return false;
	}
	//=============================================================================================================
	bool udpserver::addpacket(unsigned long client_id, const udppacket& packet)
	{
		if( clients.size() > client_id )
		{
			if( clients[client_id].outgoing.size() < 10 )
			{
				clients[client_id].outgoing.push(packet);
				return true;
			}
		}

		return false;
	}
	//=============================================================================================================
	bool udpserver::getpacket(unsigned long client_id, udppacket& packet)
	{
		if( client_id < clients.size() )
		{
			slot& client = clients[client_id];

			if( client.incoming.size() > 0 )
			{
				packet = *client.incoming.begin();
				client.incoming.erase(client.incoming.begin());

				// ennél korábbit már nem fogadunk
				// ezt nem igy kellene, hanem idö alapján
				client.firstseq = packet.sequence_no;
				return true;
			}
		}

		return false;
	}
	//=============================================================================================================
	void udpserver::run()
	{
		// TODO: ha a protocol_id egyezik
		// TODO: ha valamelyiktöl sokáig nem jön akkor kidobni (pl. a wow 5p után logout)

		// elöször küldünk (annyit küld amennyit a ráta megenged)
		// adaptálni kell a fogadók sebességéhez
		float packetsize = (float)_PACKET_SIZE_;
		float rate = SendRate / (float)clients.size();
		float time = packetsize / (rate * 128.0f);

		abletosend = false;

		// egyelöre fix ráta
		if( currentclient != clients.size() )
		{
			double current = clock.Time();

			if( last + time < current )
			{
				abletosend = true;
				slot& client = clients[currentclient];

				// ha alacsony a küldési ráta akkor ez nem hatékony
				// ha magas akkor viszont a vesztett idö --> 0
				if( client.outgoing.size() > 0 )
				{
					last = current;
					udppacket& packet = client.outgoing.front();

					// beállitjuk a sorszámot
					packet.connection_id = currentclient;
					packet.sequence_no = client.local;

					++client.local;

					// ackoljuk a elsö csomagot (érkezési sorrendben => az rtt reális)
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

				currentclient = (currentclient + 1) % clients.size();
			}
		}
		// nem biztos, hogy kell (amikor kilép valaki akkor mivan)
		else
			currentclient = 0;

		// aztán fogadunk
		ipv4address sender;
		udppacket packet;

		if( connection->recvpacket(packet, sender) )
		{
			// megnézzük hogy uj kapcsolat-e
			if( packet.connection_id == 0xffff )
			{
				// TODO: packetet is?
				pending.push(sender);
			}
			else
			{
				if( packet.connection_id < clients.size() )
				{
					slot& client = clients[packet.connection_id];

					if( packet < client.firstseq )
					{
						networkdebug("Packet obsolete");
					}
					else if( client.incoming.size() < _INCOMING_SIZE )
					{
						// TODO: ack alapján rtt számolás
						client.incoming.insert(packet);

						// ha sokáig nem küldünk, akkor kihagyunk párat...
						// az ackot csak rtt számolásra használja
						if( client.needsack.size() == 20 )
							client.needsack.pop();

						client.needsack.push(packet.sequence_no);
					}
					else
						networkdebug("Incoming queue too large");
				}
				else
				{
					// refuse
					connection->sendpacket(sender, packet << "go to hell");
				}
			}
		}
	}
	//=============================================================================================================
}
