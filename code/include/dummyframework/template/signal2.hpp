//=============================================================================================================
#ifndef _SIGNAL2_HPP_
#define _SIGNAL2_HPP_

#include "connection.hpp"
#include <list>

namespace DummyFramework
{
    /**
	 * \brief 2 argument signal
	 */
	template <typename t1, typename t2>
	class signal2 : public signal_base
	{
	private:
		typedef std::list<connection2_base<t1, t2>*> slotlist;
		typedef std::list<void (*)(t1, t2)> funclist;

		slotlist _slots;
		funclist _funcs;

	public:
		signal2() {
		}

		signal2(const signal2& other) {
			operator =(other);
		}

		~signal2() {
			disconnectall();
		}

		signal2& operator =(const signal2& other);

		void operator ()(t1 p1, t2 p2);
		void disconnect(has_slots* obj);
		void disconnect(void (*func)(t1, t2));
		void disconnectall();

		template <typename object_type>
		void connect(object_type* obj, void (object_type::*memfunc)(t1, t2));

		inline void connect(void (*func)(t1, t2)) {
			_funcs.push_back(func);
		}
	};

	template <typename t1, typename t2>
	signal2<t1, t2>& signal2<t1, t2>::operator =(const signal2& other)
	{
		if( &other != this )
		{
			connection2_base<t1, t2>* conn;

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

	template <typename t1, typename t2>
	void signal2<t1, t2>::operator ()(t1 p1, t2 p2)
	{
		typename slotlist::iterator it = _slots.begin();
		typename slotlist::iterator end = _slots.end();
		typename slotlist::iterator next;

		while( it != end )
		{
			next = it;
			++next;

			(*it)->emit(p1, p2);
			it = next;
		}

		typename funclist::iterator fit = _funcs.begin();
		typename funclist::iterator fend = _funcs.end();
		typename funclist::iterator fnext = _funcs.end();

		while( fit != fend )
		{
			fnext = fit;
			++fnext;

			(*fit)(p1, p2);
			fit = fnext;
		}
	}

	template <typename t1, typename t2>
	template <typename object_type>
	void signal2<t1, t2>::connect(object_type* obj, void (object_type::*memfunc)(t1, t2))
	{
		connection2<object_type, t1, t2>* conn = new connection2<object_type, t1, t2>(obj, memfunc);
		_slots.push_back(conn);

		obj->signalconnect(this);
	}

	template <typename t1, typename t2>
	void signal2<t1, t2>::disconnect(has_slots* obj)
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

	template <typename t1, typename t2>
	void signal2<t1, t2>::disconnect(void (*func)(t1, t2))
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

	template <typename t1, typename t2>
	void signal2<t1, t2>::disconnectall()
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
