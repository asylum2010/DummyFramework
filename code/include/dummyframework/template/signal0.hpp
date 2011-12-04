//=============================================================================================================
#ifndef _SIGNAL0_HPP_
#define _SIGNAL0_HPP_

#include "connection.hpp"
#include <list>

namespace DummyFramework
{
    /**
     * \brief 0 argument signal
     */
    class signal0 : public signal_base
    {
    private:
        typedef std::list<connection0_base*> slotlist;
        typedef std::list<void (*)()> funclist;

        slotlist _slots;
        funclist _funcs;

    public:
		signal0() {
		}

		signal0(const signal0& other) {
			operator =(other);
		}

        ~signal0() {
            disconnectall();
        }

		signal0& operator =(const signal0& other)
		{
			if( &other != this )
			{
				connection0_base* conn;

				for( slotlist::iterator it = _slots.begin(); it != _slots.end(); ++it )
				{
					(*it)->getobj()->signaldisconnect(this);
					delete (*it);
				}

				_slots.clear();
				_funcs.clear();

				for( slotlist::const_iterator it = other._slots.begin(); it != other._slots.end(); ++it )
				{
					(*it)->getobj()->signalconnect(this);
					(*it)->clone(&conn);

					_slots.push_back(conn);
				}
				
				for( funclist::const_iterator it = other._funcs.begin(); it != other._funcs.end(); ++it )
					_funcs.push_back(*it);
			}

			return *this;
		}

        void operator ()()
        {
            slotlist::iterator it = _slots.begin();
            slotlist::iterator end = _slots.end();
            slotlist::iterator next;

            while( it != end )
            {
                next = it;
                ++next;

                (*it)->emit();
                it = next;
            }

            funclist::iterator fit = _funcs.begin();
            funclist::iterator fend = _funcs.end();
            funclist::iterator fnext = _funcs.end();

            while( fit != fend )
            {
                fnext = fit;
                ++fnext;

                (*fit)();
                fit = fnext;
            }
        }

        void disconnect(has_slots* obj)
        {
            slotlist::iterator it = _slots.begin();
            slotlist::iterator end = _slots.end();
            slotlist::iterator next;

            while( it != end )
            {
                next = it;
                ++next;

                if( (*it)->getobj() == obj )
                {
                    (*it)->getobj()->signaldisconnect(this);
                    delete (*it);

                    _slots.erase(it);
                }

                it = next;
            }
        }

        void disconnect(void (*func)())
        {
            // TODO: és ha többször van benne?

            funclist::iterator it = _funcs.begin();
            funclist::iterator end = _funcs.end();

            for( ; it != end; ++it )
            {
                if( (*it) == func )
                {
                    _funcs.erase(it);
                    return;
                }
            }
        }

        void disconnectall()
        {
            slotlist::iterator it = _slots.begin();
            slotlist::iterator end = _slots.end();

            for( ; it != end; ++it )
            {
                (*it)->getobj()->signaldisconnect(this);
                delete (*it);
            }
                        
            _slots.clear();
            _funcs.clear();
        }
        
        template <typename object_type>
        void connect(object_type* obj, void (object_type::*memfunc)());

        inline void connect(void (*func)()) {
            _funcs.push_back(func);
        }
    };

    template <typename object_type>
    void signal0::connect(object_type* obj, void (object_type::*memfunc)())
    {
        connection0<object_type>* conn = new connection0<object_type>(obj, memfunc);
        _slots.push_back(conn);

        obj->signalconnect(this);
    }
}

#endif
//=============================================================================================================
