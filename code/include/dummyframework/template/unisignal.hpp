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

		unisignal0(const unisignal0& other)
			: _conn(0), _func(0)
		{
			operator =(other);
		}

		~unisignal0() {
			disconnectall();
		}

		unisignal0& operator =(const unisignal0& other)
		{
			if( &other != this )
			{
				if( _conn )
				{
					_conn->getobj()->signaldisconnect(this);
					delete _conn;
				}

				_conn = NULL;

				if( other._conn )
				{
					other._conn->getobj()->signalconnect(this);
					other._conn->clone(&_conn);
				}

				_func = other._func;
			}

			return *this;
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

		unisignal1(const unisignal1& other)
			: _conn(0), _func(0)
		{
			operator =(other);
		}

		~unisignal1() {
			disconnectall();
		}

		unisignal1& operator =(const unisignal1& other)
		{
			if( &other != this )
			{
				if( _conn )
				{
					_conn->getobj()->signaldisconnect(this);
					delete _conn;
				}

				_conn = NULL;

				if( other._conn )
				{
					other._conn->getobj()->signalconnect(this);
					other._conn->clone(&_conn);
				}

				_func = other._func;
			}

			return *this;
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

	/**
	 * \brief 2 argument single-call signal
	 */
	template <typename t1, typename t2>
	class unisignal2 : public signal_base
	{
	private:
		connection2_base<t1, t2>* _conn;
		void (*_func)(t1, t2);

	public:
		unisignal2()
			: _conn(0), _func(0) {}

		unisignal2(const unisignal2& other)
			: _conn(0), _func(0)
		{
			operator =(other);
		}

		~unisignal2() {
			disconnectall();
		}

		unisignal2& operator =(const unisignal2& other)
		{
			if( &other != this )
			{
				if( _conn )
				{
					_conn->getobj()->signaldisconnect(this);
					delete _conn;
				}

				_conn = NULL;

				if( other._conn )
				{
					other._conn->getobj()->signalconnect(this);
					other._conn->clone(&_conn);
				}

				_func = other._func;
			}

			return *this;
		}

		void operator()(t1 p1, t2 p2)
		{
			if( _conn )
				_conn->emit(p1, p2);

			if( _func )
				_func(p1, p2);
		}

		template <typename object_type>
		void connect(object_type* obj, void (object_type::*memfunc)(t1, t2))
		{
			if( _conn )
				delete _conn;

			_conn = new connection2<object_type, t1, t2>(obj, memfunc);
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

		inline void connect(void (*func)(t1, t2)) {
			_func = func;
		}

		inline void disconnect(void (*func)(t1, t2)) {
			if( _func == func )
				_func = 0;
		}
	};
}

#endif
//=============================================================================================================
