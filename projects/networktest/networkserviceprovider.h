//=============================================================================================================
#ifndef _NETWORKSERVICEPROVIDER_H_
#define _NETWORKSERVICEPROVIDER_H_

#include <d3dx9.h>
#include <string>

// TODO: lecserélni az OGRE-s megfelelöre
typedef unsigned long inputstate;
typedef D3DXVECTOR3 vector3;
typedef D3DXQUATERNION quaternion;

/**
 * \brief Minimális adat amit egy entitás küld
 */
struct entityDTO
{
	inputstate input;
	vector3 position;
	quaternion orientation;
};

/**
 * \brief Interfész állapotok küldéséhez
 */
class NetworkServiceProvider
{
public:
	virtual ~NetworkServiceProvider() {}

	virtual bool Connect(const std::string& ip, unsigned short port) = 0;
	virtual bool ReceiveState(entityDTO& out, size_t from) = 0;

	virtual void Disconnect() = 0;
	virtual void SendState(const entityDTO& state, size_t to = 0) = 0;
	virtual void UpdateClient(bool send) = 0;
	virtual void UpdateServer(bool send) = 0;

	virtual size_t GetNumConnections() const = 0;
	virtual bool IsConnected() const = 0;

	static NetworkServiceProvider* Create();
};

/*
 A müködésröl:
 -------------

 c/i/o <=> channel/incoming/outgoing index

 +----------------------------+------------------------------+------------------------------+
 |           SERVER           |           CLIENT1            |           CLIENT2            |
 +----------------------------+------------------------------+------------------------------+
 | myid == 0                  |                              |                              |
 |                            | c/i/o 0: server  (id == 0)   | c/i/o 0: server  (id == 0)   |
 | c/i/o 0: client1 (id == 1) | c/i/o 1: self    (myid == 1) | c/i/o 1: client2 (id == 1)   |
 | c/i/o 1: client2 (id == 2) | c/i/o 2: client2 (id == 2)   | c/i/o 2: self    (myid == 2) |
 +----------------------------+------------------------------+------------------------------+

 - a felhasználás szemszögéböl mindig 0-tol indul a fogadás és küldés
 - pl. ha szerver vagy és az 1-es kliensnek akarsz küldeni, akkor SendPacket(..., 0)
 - ha kliens vagy és egy másik kliens állapota érdekel, akkor is 0-tol indul a dolog
 - de kliens csak a szervernek tud küldeni, aki a 0
 - amig nem 100%-os az implementácio addig a XXX_stub.cpp fájlt (is) lehet használni (nem csinál semmit)


 A repcsis progirol:
 -------------------

 - a 0. repcsi mindig az amivel játszol
 - tehát a többi kliens repcsije 1-töl indul
 - egyelöre nem számol külön a szerver meg a kliens
 - néha lefagyhat... (multithreading)

*/

#endif
//=============================================================================================================
