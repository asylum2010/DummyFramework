//=============================================================================================================
#ifndef _UDPCLIENT_H_
#define _UDPCLIENT_H_

#include "udpconnection.h"
#include "udpchannel.h"
#include "../base/CTimer.h"

#include <set>
#include <queue>

namespace DummyFramework
{
	class udpclient : public udpstation
	{
	private:
		static CTimer clock;

		typedef std::set<udppacket> packetset;
		typedef std::queue<udppacket> packetlist;
		typedef std::queue<unsigned short> seqlist;

		struct client
		{
			unsigned short local;
			unsigned short firstseq;
			ipv4address address;

			packetset incoming;
			packetlist outgoing;
			seqlist needsack;

			client() {
				local = 0;
				firstseq = 0;
			}
		};
		
		client client;
		udpconnection* connection;

		bool abletosend;
		double last;
		double lasttimeout;
		unsigned short connection_id;

	public:
		float SendRate;
		float TimeOut;

		udpclient();
		udpclient(udpconnection& conn);
		~udpclient();

		bool attach(udpconnection& conn);
		bool join(udpchannel& out);
		bool addpacket(unsigned long client_id, const udppacket& packet);
		bool getpacket(unsigned long client_id, udppacket& packet);
		void run();

		inline bool connected() const {
			return connection_id != 0xffff;
		}

		inline bool cansend() const {
			return abletosend;
		}

		inline void getaddress(unsigned long client_id, ipv4address& addr) {
			addr = client.address;
		}
	};
}

#endif
//=============================================================================================================


 
