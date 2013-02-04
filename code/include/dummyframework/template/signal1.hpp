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

		slotlist _slots;
		funclist _funcs;

	public:
		signal1() {
		}

		signal1(const signal1& other) {
			operator =(other);
		}

		~signal1() {
			disconnectall();
		}

		signal1& operator =(const signal1& other);

		void operator ()(t1 p1);
		void disconnect(has_slots* obj);
		void disconnect(void (*func)(t1));
		void disconnectall();

		template <typename object_type>
		void connect(object_type* obj, void (object_type::*memfunc)(t1));

		inline void connect(void (*func)(t1)) {
			_funcs.push_back(func);
		}
	};

	template <typename t1>
	signal1<t1>& signal1<t1>::operator =(const signal1& other)
	{
		if( &other != this )
		{
			connection1_base<t1>* conn;

			for( typename slotlist::iterator it = _slots.begin(); it != _slots.end(); ++it )
			{
				(*it)->getobj()->signaldisconnect(this);
				delete (*it);
			}

			_slots.clear();
			_funcs.clear();

			for( typename slotlist::const_iterator it = other._slots.begin(); it != other._slots.end(); ++it )
			{
				(*it)->getobj()->signalconnect(this);
				(*it)->clone(&conn);

				_slots.push_back(conn);
			}

			for( typename funclist::const_iterator it = other._funcs.begin(); it != other._funcs.end(); ++it )
				_funcs.push_back(*it);
		}

		return *this;
	}

	template <typename t1>
	void signal1<t1>::operator ()(t1 p1)
	{
		typename slotlist::iterator it = _slots.begin();
		typename slotlist::iterator end = _slots.end();
		typename slotlist::iterator next;

		while( it != end )
		{
			next = it;
			++next;

			(*it)->emit(p1);
			it = next;
		}

		typename funclist::iterator fit = _funcs.begin();
		typename funclist::iterator fend = _funcs.end();
		typename funclist::iterator fnext = _funcs.end();

		while( fit != fend )
		{
			fnext = fit;
			++fnext;

			(*fit)(p1);
			fit = fnext;
		}
	}

	template <typename t1>
	template <typename object_type>
	void signal1<t1>::connect(object_type* obj, void (object_type::*memfunc)(t1))
	{
		connection1<object_type, t1>* conn = new connection1<object_type, t1>(obj, memfunc);
		_slots.push_back(conn);

		obj->signalconnect(this);
	}

	template <typename t1>
	void signal1<t1>::disconnect(has_slots* obj)
	{
		typename slotlist::iterator it = _slots.begin();
		typename slotlist::iterator end = _slots.end();
		typename slotlist::iterator next;

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

	template <typename t1>
	void signal1<t1>::disconnect(void (*func)(t1))
	{
		typename funclist::iterator it = _funcs.begin();
		typename funclist::iterator end = _funcs.end();

		for( ; it != end; ++it )
		{
			if( (*it) == func )
			{
				_funcs.erase(it);
				return;
			}
		}
	}

	template <typename t1>
	void signal1<t1>::disconnectall()
	{
		typename slotlist::iterator it = _slots.begin();
		typename slotlist::iterator end = _slots.end();

		for( ; it != end; ++it )
		{
			(*it)->getobj()->signaldisconnect(this);
			delete (*it);
		}

		_slots.clear();
		_funcs.clear();
	}
}

#endif
//=============================================================================================================
