//=============================================================================================================
#include <dummyframework/network/udpconnection.h>

namespace DummyFramework
{
	ipv4address::ipv4address()
	{
		memset(&addr, 0, sizeof(sockaddr_in));
		addr.sin_family = AF_INET;
	}
	//=============================================================================================================
	ipv4address::ipv4address(const char* ip, unsigned short port)
	{
		memset(&addr, 0, sizeof(sockaddr_in));

		if( ip )
			addr.sin_addr.s_addr = inet_addr(ip);
		else
			addr.sin_addr.s_addr = htonl(INADDR_ANY);

		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
	}
	//*************************************************************************************************************
	udpconnection::udpconnection()
	{
	#ifdef _WIN32
		bad = (0 != WSAStartup(MAKEWORD(2, 2), &wsa));
	#endif

		sock = -1;
		Blocking = false;
	}
	//=============================================================================================================
	udpconnection::udpconnection(const char* ip, unsigned short remoteport, unsigned short localport)
	{
		udpconnection();
		connect(ip, remoteport, localport);
	}
	//=============================================================================================================
	udpconnection::~udpconnection()
	{
		if( sock != -1 )
			disconnect();
	}
	//=============================================================================================================
	void udpconnection::connect(const char* ip, unsigned short remoteport, unsigned short localport)
	{
		addr = ipv4address(ip, remoteport);
		ipv4address tmp(0, localport);

		bad = (-1 == (sock = socket(AF_INET, SOCK_DGRAM, 0)));
		bad = (-1 == bind(sock, tmp.address(), sizeof(sockaddr_in)));

		if( !bad && !Blocking )
		{
	#ifdef _WIN32
			DWORD nb = 1;
			bad = (0 != ioctlsocket(sock, FIONBIO, &nb));
	#else
			int nb = 1;
			bad = (-1 == fcntl(sock, F_SETFL, O_NONBLOCK, nb));
	#endif
		}
	}
	//=============================================================================================================
	void udpconnection::disconnect()
	{
		#ifdef _WIN32
			closesocket(sock);
			WSACleanup();
		#else
			close(sock);
		#endif

		sock = -1;
	}
	//=============================================================================================================
	bool udpconnection::sendpacket(const ipv4address& to, const udppacket& packet)
	{
		if( !bad )
		{
			return (sizeof(udppacket) == sendto(sock, (const char*)&packet,
				sizeof(udppacket), 0, to.address(), sizeof(sockaddr)));
		}

		return false;
	}
	//=============================================================================================================
	bool udpconnection::sendpacket(const udppacket& packet)
	{
		if( !bad )
		{
			return (sizeof(udppacket) == sendto(sock, (const char*)&packet,
				sizeof(udppacket), 0, addr.address(), sizeof(sockaddr)));
		}

		return false;
	}
	//=============================================================================================================
	bool udpconnection::recvpacket(udppacket& packet, ipv4address& sender)
	{
		if( !bad )
		{
			int len = sizeof(sockaddr);
			int readbytes = recvfrom(sock, (char*)&packet,
				sizeof(udppacket), 0, sender.address(), &len);
			
			return (readbytes > 0);
		}

		return false;
	}
	//=============================================================================================================
}
