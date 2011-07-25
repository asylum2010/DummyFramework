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

        slotlist _slotlist;
        funclist _funclist;

    public:
        ~signal0() {
            disconnectall();
        }

        void operator ()()
        {
            slotlist::iterator it = _slotlist.begin();
            slotlist::iterator end = _slotlist.end();
            slotlist::iterator next;

            while( it != end )
            {
                next = it;
                ++next;

                (*it)->emit();
                it = next;
            }

            funclist::iterator fit = _funclist.begin();
            funclist::iterator fend = _funclist.end();
            funclist::iterator fnext = _funclist.end();

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
            slotlist::iterator it = _slotlist.begin();
            slotlist::iterator end = _slotlist.end();
            slotlist::iterator next;

            while( it != end )
            {
                next = it;
                ++next;

                if( (*it)->getobj() == obj )
                {
                    (*it)->getobj()->signaldisconnect(this);
                    delete (*it);

                    _slotlist.erase(it);
                }

                it = next;
            }
        }

        void disconnect(void (*func)())
        {
            // TODO: és ha többször van benne?

            funclist::iterator it = _funclist.begin();
            funclist::iterator end = _funclist.end();

            for( ; it != end; ++it )
            {
                if( (*it) == func )
                {
                    _funclist.erase(it);
                    return;
                }
            }
        }

        void disconnectall()
        {
            slotlist::iterator it = _slotlist.begin();
            slotlist::iterator end = _slotlist.end();

            for( ; it != end; ++it )
            {
                (*it)->getobj()->signaldisconnect(this);
                delete (*it);
            }
                        
            _slotlist.clear();
            _funclist.clear();
        }
        
        template <typename object_type>
        void connect(object_type* obj, void (object_type::*memfunc)());

        inline void connect(void (*func)()) {
            _funclist.push_back(func);
        }
    };
    //=============================================================================================================
    template <typename object_type>
    void signal0::connect(object_type* obj, void (object_type::*memfunc)())
    {
        connection0<object_type>* conn = new connection0<object_type>(obj, memfunc);
        _slotlist.push_back(conn);

        obj->signalconnect(this);
    }
}

#endif
//=============================================================================================================
