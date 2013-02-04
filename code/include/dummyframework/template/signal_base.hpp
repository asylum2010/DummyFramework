//=============================================================================================================
#ifndef _SIGNALBASE_HPP_
#define _SIGNALBASE_HPP_

#include <set>

namespace DummyFramework
{
    class has_slots;

	/**
	 * \brief Base class for signals
	 */
	class signal_base
	{
	public:
		virtual ~signal_base() {}
		virtual void disconnect(has_slots* obj) = 0;
	};

	/**
	 * \brief Base of slot owner classes
	 */
	class has_slots
	{
	private:
		std::set<signal_base*> senders;

	public:
		virtual ~has_slots()
		{
			disconnectall();
		}

		void disconnectall()
		{
			std::set<signal_base*>::iterator it = senders.begin();
			std::set<signal_base*>::iterator end = senders.end();
			std::set<signal_base*>::iterator next;

			while( it != end )
			{
				next = it;
				++next;

				(*it)->disconnect(this);
				it = next;
			}

			senders.clear();
		}

		inline void signalconnect(signal_base* signal) {
			senders.insert(signal);
		}

		inline void signaldisconnect(signal_base* signal) {
			senders.erase(signal);
		}
	};
}

#endif
//=============================================================================================================
