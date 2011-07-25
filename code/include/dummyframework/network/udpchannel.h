//=============================================================================================================
#ifndef _UDPCHANNEL_H_
#define _UDPCHANNEL_H_

#include <string>

namespace DummyFramework
{
	class udppacket;
	class ipv4address;

	class udpstation
	{
	public:
		virtual ~udpstation() {}
		virtual bool addpacket(unsigned long client_id, const udppacket& packet) = 0;
		virtual bool getpacket(unsigned long client_id, udppacket& packet) = 0;
		virtual void getaddress(unsigned long client_id, ipv4address& addr) = 0;
	};

	class udpchannel
	{
	private:
		udpstation* parent;
		unsigned short client_id;

	public:
		udpchannel();
		udpchannel(udpstation& station, unsigned short id);
		~udpchannel();

		udpchannel& operator <<(const std::string& str);
		udpchannel& operator >>(std::string& str);
		udpchannel& operator =(const udpchannel& other);

		ipv4address getaddress() const;
	};
}

#endif
//=============================================================================================================

