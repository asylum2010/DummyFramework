//=============================================================================================================
#ifndef _NETWORKSERVICEPROVIDER_H_
#define _NETWORKSERVICEPROVIDER_H_

#include <d3dx9.h>
#include <string>

// TODO: lecser�lni az OGRE-s megfelel�re
typedef unsigned long inputstate;
typedef D3DXVECTOR3 vector3;
typedef D3DXQUATERNION quaternion;

/**
 * \brief Minim�lis adat amit egy entit�s k�ld
 */
struct entityDTO
{
	inputstate input;
	vector3 position;
	quaternion orientation;
};

/**
 * \brief Interf�sz �llapotok k�ld�s�hez
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
 A m�k�d�sr�l:
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

 - a felhaszn�l�s szemsz�g�b�l mindig 0-tol indul a fogad�s �s k�ld�s
 - pl. ha szerver vagy �s az 1-es kliensnek akarsz k�ldeni, akkor SendPacket(..., 0)
 - ha kliens vagy �s egy m�sik kliens �llapota �rdekel, akkor is 0-tol indul a dolog
 - de kliens csak a szervernek tud k�ldeni, aki a 0
 - amig nem 100%-os az implement�cio addig a XXX_stub.cpp f�jlt (is) lehet haszn�lni (nem csin�l semmit)


 A repcsis progirol:
 -------------------

 - a 0. repcsi mindig az amivel j�tszol
 - teh�t a t�bbi kliens repcsije 1-t�l indul
 - egyel�re nem sz�mol k�l�n a szerver meg a kliens
 - n�ha lefagyhat... (multithreading)

*/

#endif
//=============================================================================================================
