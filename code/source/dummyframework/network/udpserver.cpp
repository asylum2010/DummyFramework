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
				
				// send a packet to acknowledge
				// SEQ is irrelevant, the other side ignores it
				// we will send something sooner or later
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
	bool udpserver::addpacket(unsigned int client_id, const udppacket& packet)
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
	bool udpserver::getpacket(unsigned int client_id, udppacket& packet)
	{
		if( client_id < clients.size() )
		{
			slot& client = clients[client_id];

			if( client.incoming.size() > 0 )
			{
				packet = *client.incoming.begin();
				client.incoming.erase(client.incoming.begin());

				// do not receive older than this
				// TODO: use time instead
				client.firstseq = packet.sequence_no;
				return true;
			}
		}

		return false;
	}
	//=============================================================================================================
	void udpserver::run()
	{
		// TODO: when protocol_id-s match
		// TODO: throw out inactive clients

		// send first (based on rate)
		// adapt to receivers
		float packetsize = (float)_PACKET_SIZE_;
		float rate = SendRate / (float)clients.size();
		float time = packetsize / (rate * 128.0f);

		abletosend = false;

		// fix rate for now
		if( currentclient != clients.size() )
		{
			double current = clock.Time();

			if( last + time < current )
			{
				abletosend = true;
				slot& client = clients[currentclient];

				// when send rate is low, this is ineffective
				// when send rate is high, then lost time converges to zero
				if( client.outgoing.size() > 0 )
				{
					last = current;
					udppacket& packet = client.outgoing.front();

					// set SEQ
					packet.connection_id = currentclient;
					packet.sequence_no = client.local;

					++client.local;

					// ACK first packet (based on arrival => RTT makes sense)
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
		// what if someone quits?
		else
			currentclient = 0;

		// receive
		ipv4address sender;
		udppacket packet;

		if( connection->recvpacket(packet, sender) )
		{
			// check if its a new connection
			if( packet.connection_id == 0xffff )
			{
				// TODO: packet?
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
						// TODO: calculate RTT based on ACK
						client.incoming.insert(packet);

						// if we did not send a int time ago, skip some
						// (ACK is used only for RTT calculation)
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
