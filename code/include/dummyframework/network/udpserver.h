//=============================================================================================================
#ifndef _UDPSERVER_H_
#define _UDPSERVER_H_

#include "udpconnection.h"
#include "udpchannel.h"
#include "../base/CTimer.h"

#include <set>
#include <vector>
#include <queue>

namespace DummyFramework
{
	class udpserver : public udpstation
	{
	private:
		static CTimer clock;

		typedef std::set<udppacket> packetset;
		typedef std::set<ipv4address> addresscache;
		typedef std::queue<udppacket> packetlist;
		typedef std::queue<unsigned short> seqlist;

		struct slot
		{
			bool free;
			unsigned short local;
			unsigned short firstseq;
			ipv4address address;

			packetset incoming;
			packetlist outgoing;
			seqlist needsack;

			slot() {
				free = true;
				local = 0;
				firstseq = 0;
			}
		};
		
		typedef std::vector<slot> clientlist;
		typedef std::queue<ipv4address> pendinglist;

		udpconnection* connection;
		clientlist clients;
		pendinglist pending;
		addresscache cache;
		
		bool abletosend;
		double last;
		size_t currentclient;

	public:
		float SendRate;

		udpserver();
		udpserver(udpconnection& conn);
		~udpserver();

		bool attach(udpconnection& conn);
		bool accept(udpchannel& out);
		bool addpacket(unsigned long client_id, const udppacket& packet);
		bool getpacket(unsigned long client_id, udppacket& packet);
		void run();

		inline void getaddress(unsigned long client_id, ipv4address& addr) {
			addr = clients[client_id].address;
		}

		inline bool cansend() const {
			return abletosend;
		}
	};
}

#endif
//=============================================================================================================


