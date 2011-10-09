//=============================================================================================================
#include "networkserviceprovider.h"

/**
 * \brief Csak egy stub, hogy lehessen használni
 *
 * A tényleges implementáció el lesz rejtve
 */
class NetworkServiceProviderSTUB : public NetworkServiceProvider
{
public:
	bool Connect(const std::string& ip, unsigned short port) {
		return true;
	}

	bool ReceiveState(entityDTO& out, size_t from) {
		return false;
	}

	void Disconnect() {
	}

	void SendState(const entityDTO& state, size_t to = 0) {
	}

	void UpdateClient(bool send) {
	}

	void UpdateServer(bool send) {
	}

	inline size_t GetNumConnections() const {
		return 0;
	}

	inline bool IsConnected() const {
		return false;
	}
};

NetworkServiceProvider* NetworkServiceProvider::Create()
{
	return new NetworkServiceProviderSTUB();
}
//=============================================================================================================
