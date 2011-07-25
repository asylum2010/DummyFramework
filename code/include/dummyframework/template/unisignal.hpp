//=============================================================================================================
#ifndef _UNISIGNAL_HPP_
#define _UNISIGNAL_HPP_

#include "connection.hpp"

namespace DummyFramework
{
    /**
     * \brief 0 argument single-call signal
     */
    class unisignal0 : public signal_base
    {
    private:
        connection0_base* _conn;
        void (*_func)();

    public:
        unisignal0()
            : _conn(0), _func(0) {}

		~unisignal0() {
            disconnectall();
        }

        void operator()()
        {
            if( _conn )
                _conn->emit();

            if( _func )
                _func();
        }

        template <typename object_type>
        void connect(object_type* obj, void (object_type::*memfunc)())
        {
            if( _conn )
                delete _conn;

            _conn = new connection0<object_type>(obj, memfunc);
            obj->signalconnect(this);
        }
        
        void disconnect(has_slots* obj)
        {
            if( _conn && _conn->getobj() == obj )
            {
                _conn->getobj()->signaldisconnect(this);
                delete _conn;

                _conn = 0;
            }
        }

        void disconnectall()
        {
            if( _conn )
            {
                _conn->getobj()->signaldisconnect(this);
                delete _conn;

                _conn = 0;
            }

            _func = 0;
        }

        inline void connect(void (*func)()) {
            _func = func;
        }

        inline void disconnect(void (*func)()) {
            if( _func == func )
                _func = 0;
        }
    };
    
    /**
     * \brief 1 argument single-call signal
     */
    template <typename t1>
    class unisignal1 : public signal_base
    {
    private:
        connection1_base<t1>* _conn;
        void (*_func)(t1);

    public:
        unisignal1()
            : _conn(0), _func(0) {}

		~unisignal1() {
            disconnectall();
        }

        void operator()(t1 p1)
        {
            if( _conn )
                _conn->emit(p1);

            if( _func )
                _func(p1);
        }

        template <typename object_type>
        void connect(object_type* obj, void (object_type::*memfunc)(t1))
        {
            if( _conn )
                delete _conn;

            _conn = new connection1<object_type, t1>(obj, memfunc);
            obj->signalconnect(this);
        }
        
        void disconnect(has_slots* obj)
        {
            if( _conn && _conn->getobj() == obj )
            {
                _conn->getobj()->signaldisconnect(this);
                delete _conn;

                _conn = 0;
            }
        }

        void disconnectall()
        {
            if( _conn )
            {
                _conn->getobj()->signaldisconnect(this);
                delete _conn;

                _conn = 0;
            }

            _func = 0;
        }

        inline void connect(void (*func)(t1)) {
            _func = func;
        }

        inline void disconnect(void (*func)(t1)) {
            if( _func == func )
                _func = 0;
        }
    };
}

#endif
//=============================================================================================================

