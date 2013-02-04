//=============================================================================================================
#ifndef _UDPCONNECTION_H_
#define _UDPCONNECTION_H_

#ifdef _WIN32
#	include <winsock2.h>
#else
#	include <sys/socket.h>
#	include <sys/types.h>
#	include <unistd.h>
#	include <netdb.h>
#	include <fcntl.h>
#	define SOCKET int
#endif

#include <string>

// TODO:
// - rtt count
// - flow control

#define _PACKET_SIZE_	256
#define _PACKET_DATA_	(_PACKET_SIZE_ - 12)
#define _INCOMING_SIZE	20

#define networkdebug(x) { std::cout << "* NETWORK: " << x << ".\n"; }

namespace DummyFramework
{
	class udpchannel;

	class ipv4address
	{
	protected:
		sockaddr_in addr;

	public:
		ipv4address();
		ipv4address(const char* ip, unsigned short port = 0);

		inline const sockaddr* address() const {
			return (sockaddr*)&addr;
		}

		inline sockaddr* address() {
			return (sockaddr*)&addr;
		}

		inline std::string tostring() {
			return std::string(inet_ntoa(addr.sin_addr));
		}
	};

	class udppacket
	{
	public:
		unsigned int  protocol_id;
		unsigned short connection_id;
		unsigned short sequence_no;
		unsigned short ack;
		unsigned short ack_bitfield;
		char data[_PACKET_DATA_];

		inline bool operator <(const udppacket& other) const {
			return
				(sequence_no < other.sequence_no) &&
				(other.sequence_no - sequence_no < 300) ||
				(sequence_no > other.sequence_no) &&
				(sequence_no - other.sequence_no >= 300);
		}

		inline bool operator <(unsigned short seq) const {
			return
				(sequence_no < seq) &&
				(seq - sequence_no < 300) ||
				(sequence_no > seq) &&
				(sequence_no - seq >= 300);
		}
		
		inline udppacket& operator <<(const std::string& str) {
			const size_t& tmp = min(str.length(), _PACKET_DATA_ - 1);
			memcpy(data, &str[0], tmp);

			data[tmp] = 0;
			return *this;
		}
		
		inline udppacket& operator >>(std::string& str) {
			str = data;
			return *this;
		}
	};

	class udpconnection
	{
	private:
		SOCKET sock;
		WSADATA wsa;
		ipv4address addr;
		bool bad;

	public:
		bool Blocking;

		udpconnection();
		udpconnection(const char* ip, unsigned short remoteport, unsigned short localport = 0);
		~udpconnection();

		void connect(const char* ip, unsigned short remoteport, unsigned short localport = 0);
		void disconnect();
		
		bool sendpacket(const ipv4address& to, const udppacket& packet);
		bool sendpacket(const udppacket& packet);
		bool recvpacket(udppacket& packet, ipv4address& sender);

		inline bool fail() const { return bad; }
		inline const ipv4address& getaddr() const { return addr; }
	};
}

#endif
//=============================================================================================================
