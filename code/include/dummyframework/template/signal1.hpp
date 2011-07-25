//=============================================================================================================
#ifndef _SIGNAL1_HPP_
#define _SIGNAL1_HPP_

#include "connection.hpp"
#include <list>

namespace DummyFramework
{
    /**
     * \brief 1 argument signal
     */
    template <typename t1>
    class signal1 : public signal_base
    {
    private:
        typedef std::list<connection1_base<t1>*> slotlist;
        typedef std::list<void (*)(t1)> funclist;

        slotlist _slotlist;
        funclist _funclist;

    public:
        ~signal1() {
            disconnectall();
        }

        void operator ()(t1 p1);
        void disconnect(has_slots* obj);
        void disconnect(void (*func)(t1));
        void disconnectall();

        template <typename object_type>
        void connect(object_type* obj, void (object_type::*memfunc)(t1));

        inline void connect(void (*func)(t1)) {
            _funclist.push_back(func);
        }
    };
    //=============================================================================================================
    template <typename t1>
    void signal1<t1>::operator ()(t1 p1)
    {
        slotlist::iterator it = _slotlist.begin();
        slotlist::iterator end = _slotlist.end();
        slotlist::iterator next;

        while( it != end )
        {
            next = it;
            ++next;

            (*it)->emit(p1);
            it = next;
        }

        funclist::iterator fit = _funclist.begin();
        funclist::iterator fend = _funclist.end();
        funclist::iterator fnext = _funclist.end();

        while( fit != fend )
        {
            fnext = fit;
            ++fnext;

            (*fit)(p1);
            fit = fnext;
        }
    }
    //=============================================================================================================
    template <typename t1>
    template <typename object_type>
    void signal1<t1>::connect(object_type* obj, void (object_type::*memfunc)(t1))
    {
        connection1<object_type, t1>* conn = new connection1<object_type, t1>(obj, memfunc);
        _slotlist.push_back(conn);

        obj->signalconnect(this);
    }
    //=============================================================================================================
    template <typename t1>
    void signal1<t1>::disconnect(has_slots* obj)
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
    //=============================================================================================================
    template <typename t1>
    void signal1<t1>::disconnect(void (*func)(t1))
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
    //=============================================================================================================
    template <typename t1>
    void signal1<t1>::disconnectall()
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
}

#endif
//=============================================================================================================
