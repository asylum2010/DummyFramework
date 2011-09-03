//=============================================================================================================
#include <dummyframework/network/udpclient.h>
#include <dummyframework/network/udpserver.h>

#include "networkserviceprovider.h"

#include <dummyframework/base/CHelper.h>

#include <queue>
#include <string>
#include <sstream>

// kbps = ((packetsize * 8) / 1024) / UpdateInterval;

class NetworkServiceProviderImpl : public NetworkServiceProvider
{
private:
	typedef std::queue<std::string> packetqueue;
	typedef std::vector<packetqueue> packettable;
	typedef std::vector<DummyFramework::udpchannel> channellist;

	packettable outgoing;
	packettable incoming;
	channellist channels;

	size_t myid;
	bool attached;
	bool connected;

	DummyFramework::udpconnection connection;
	DummyFramework::udpclient client;
	DummyFramework::udpserver server;

public:
	NetworkServiceProviderImpl();

	bool Connect(const std::string& ip, unsigned short port);
	bool ReceiveState(entityDTO& out, size_t from);

	void SendState(const entityDTO& state, size_t to = 0);
	void UpdateClient(bool send);
	void UpdateServer(bool send);

	inline void Disconnect() {
		connection.disconnect();
	}

	inline size_t GetNumConnections() const {
		return incoming.size();
	}

	inline bool IsConnected() const {
		return (myid != 0xffffffff);
	}
};

NetworkServiceProvider* NetworkServiceProvider::Create()
{
	return new NetworkServiceProviderImpl();
}

NetworkServiceProviderImpl::NetworkServiceProviderImpl()
{
	myid = 0xffffffff;

	attached = false;
	connected = false;

	incoming.reserve(10);
	outgoing.reserve(10);
	channels.reserve(10);
}
//=============================================================================================================
bool NetworkServiceProviderImpl::Connect(const std::string& ip, unsigned short port)
{
	if( ip == "" )
		connection.connect(0, 0, port);
	else
		connection.connect(ip.c_str(), port);

	return true;
}
//=============================================================================================================
bool NetworkServiceProviderImpl::ReceiveState(entityDTO& out, size_t from)
{
	dnassert(false, "NetworkServiceProviderImpl::ReceiveState(): Invalid client id", incoming.size() <= from);

	std::vector<std::string> v;
	packetqueue& pq = incoming[from];

	if( pq.size() == 0 )
		return false;

	const std::string& str = pq.front();
	DummyFramework::CHelper::Split(v, "|;,", str, true);

	// nem jo mert kliensnél egyenlö, szervernél viszont + 1
	//size_t checkid = (size_t)atoi(v[0].c_str());
	//dnassert(false, "NetworkServiceProviderImpl::ReceiveState(): Packet ID doesn't match", checkid != from);

	out.input = (inputstate)atoi(v[1].c_str());

	out.position.x = (float)atof(v[2].c_str());
	out.position.y = (float)atof(v[3].c_str());
	out.position.z = (float)atof(v[4].c_str());

	out.orientation.w = (float)atof(v[5].c_str());
	out.orientation.x = (float)atof(v[6].c_str());
	out.orientation.y = (float)atof(v[7].c_str());
	out.orientation.z = (float)atof(v[8].c_str());

	pq.pop();
	return true;
}
//=============================================================================================================
void NetworkServiceProviderImpl::SendState(const entityDTO& state, size_t to)
{
	dnassert(, "NetworkServiceProviderImpl::SendState(): Invalid client id", outgoing.size() <= to);
	dnassert(, "NetworkServiceProviderImpl::SendState(): Not yet connected", myid == 0xffffffff);
	
	std::stringstream ss("");

	ss << myid << "|" << state.input << ";" <<
		state.position.x << "," << state.position.y << "," << state.position.z << ";" <<
		state.orientation.w << "," << state.orientation.x << "," << state.orientation.y << "," << state.orientation.z;

	outgoing[to].push(ss.str());
}
//=============================================================================================================
void NetworkServiceProviderImpl::UpdateClient(bool send)
{
	std::string str;
	std::vector<std::string> v;
	size_t id;

	if( !attached )
	{
		attached = true;
		client.attach(connection);

		client.SendRate = 20.0f; // kbps

		channels.resize(1);
		outgoing.resize(1);
		incoming.reserve(10);
	}

	if( connected )
	{
		channels[0] >> str;

		if( str.length() > 0 )
		{
			//std::cout << "* NETWORK: Message from server: " << str << "\n";

			switch( str[0] )
			{
			case 'a':
				// a szerver fogadott minket
				myid = (size_t)atoi(str.substr(2).c_str());

				// legalább ennyi player már van (elvileg)
				if( myid > incoming.size() )
					incoming.resize(myid);

				std::cout << "* NETWORK: <<accept>>: myid == " << myid << "\n";
				break;

			case 's':
				// állapot jött
				DummyFramework::CHelper::Split(v, '|', str, true);
				id = (size_t)atoi(v[1].c_str());
				
				// uj player kapcsolodott a szerverhez
				if( id >= incoming.size() )
					incoming.resize(id + 1);

				incoming[id].push(str.substr(2));
				break;

			default:
				break;
			}
		}
		
		// küldés a szervernek
		if( send )
		{
			size_t count = min(5, outgoing[0].size());

			for( size_t i = 0; i < count; ++i )
			{
				str = "s|" + outgoing[0].front();
				channels[0] << str;

				outgoing[0].pop();
			}
		}
	}
	else
	{
		if( connected = client.join(channels[0]) )
			std::cout << "* NETWORK: Connected...\n";
	}

	client.run();
}
//=============================================================================================================
void NetworkServiceProviderImpl::UpdateServer(bool send)
{
	if( !attached )
	{
		attached = true;
		server.attach(connection);

		server.SendRate = 50.0f; // kbps
		channels.resize(1);

		myid = 0;
	}

	std::string str;
	std::vector<std::string> v;

	size_t id;
	size_t cnt = channels.size() - 1;

	// valaki csatlakozni próbál
	if( server.accept(channels[cnt]) )
	{
		std::cout << "* NETWORK: " << channels[cnt].getaddress().tostring() << " connected...\n";
		
		// kiküldjük neki az id-jét
		str = "a|" + DummyFramework::CHelper::DiscreteToString<size_t>(cnt + 1);
		channels[cnt] << str;

		incoming.resize(cnt + 1);
		outgoing.resize(cnt + 1);
		channels.resize(cnt + 2);

		++cnt;
	}

	// üzenetek fogadása
	for( size_t i = 0; i < cnt; ++i )
	{
		channels[i] >> str;

		if( str.length() > 0 )
		{
			//std::cout << "* NETWORK: Message from " << channels[i].getaddress().tostring() << ": " << str << "\n";

			switch( str[0] )
			{
			case 's':
				// állapot jött az (i + 1) id-jü klienstöl
				DummyFramework::CHelper::Split(v, '|', str, true);
				id = (size_t)atoi(v[1].c_str());
				
				incoming[id - 1].push(str.substr(2));
				break;

			default:
				break;
			}
		}
	}

	// küldés mindenkinek
	if( send )
	{
		for( size_t i = 0; i < outgoing.size(); ++i )
		{
			size_t count = min(5, outgoing[i].size());

			for( size_t j = 0; j < count; ++j )
			{
				str = "s|" + outgoing[i].front();
				channels[i] << str;

				outgoing[i].pop();
			}
		}
	}

	server.run();
}
//=============================================================================================================
