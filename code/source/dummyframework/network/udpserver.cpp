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
				
				// kik�ld�nk egy csomagot, jelezve, hogy fogadtuk
				// (mert egy�bk�nt lehet, hogy sok�ig nem is k�ld�nk semmit)
				// a seq mind1, a m�sik oldal ignorolja
				// ha elveszett: el�bb-utobb csak k�ld�nk valamit
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

				// enn�l kor�bbit m�r nem fogadunk
				// ezt nem igy kellene, hanem id� alapj�n
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
		// TODO: ha valamelyikt�l sok�ig nem j�n akkor kidobni (pl. a wow 5p ut�n logout)

		// el�sz�r k�ld�nk (annyit k�ld amennyit a r�ta megenged)
		// adapt�lni kell a fogad�k sebess�g�hez
		float packetsize = (float)_PACKET_SIZE_;
		float rate = SendRate / (float)clients.size();
		float time = packetsize / (rate * 128.0f);

		abletosend = false;

		// egyel�re fix r�ta
		if( currentclient != clients.size() )
		{
			double current = clock.Time();

			if( last + time < current )
			{
				abletosend = true;
				slot& client = clients[currentclient];

				// ha alacsony a k�ld�si r�ta akkor ez nem hat�kony
				// ha magas akkor viszont a vesztett id� --> 0
				if( client.outgoing.size() > 0 )
				{
					last = current;
					udppacket& packet = client.outgoing.front();

					// be�llitjuk a sorsz�mot
					packet.connection_id = currentclient;
					packet.sequence_no = client.local;

					++client.local;

					// ackoljuk a els� csomagot (�rkez�si sorrendben => az rtt re�lis)
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
		// nem biztos, hogy kell (amikor kil�p valaki akkor mivan)
		else
			currentclient = 0;

		// azt�n fogadunk
		ipv4address sender;
		udppacket packet;

		if( connection->recvpacket(packet, sender) )
		{
			// megn�zz�k hogy uj kapcsolat-e
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
						// TODO: ack alapj�n rtt sz�mol�s
						client.incoming.insert(packet);

						// ha sok�ig nem k�ld�nk, akkor kihagyunk p�rat...
						// az ackot csak rtt sz�mol�sra haszn�lja
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
