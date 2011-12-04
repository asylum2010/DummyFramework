//=============================================================================================================
#ifndef _CONNECTION_HPP_
#define _CONNECTION_HPP_

#include "signal_base.hpp"

namespace DummyFramework
{
    /**
     * \brief Base class for 0 argument connections
     */
    class connection0_base
    {
    public:
        virtual ~connection0_base() {}

		virtual void clone(connection0_base** out) = 0;
        virtual void emit() = 0;

        virtual has_slots* getobj() const = 0;
    };

    /**
     * \brief Base class for 1 argument connections
     */
    template <typename t1>
    class connection1_base
    {
    public:
        virtual ~connection1_base() {}

		virtual void clone(connection1_base** out) = 0;
        virtual void emit(t1 p1) = 0;

        virtual has_slots* getobj() const = 0;
    };

	/**
     * \brief Base class for 2 argument connections
     */
    template <typename t1, typename t2>
    class connection2_base
    {
    public:
        virtual ~connection2_base() {}

		virtual void clone(connection2_base** out) = 0;
        virtual void emit(t1 p1, t2 p2) = 0;

        virtual has_slots* getobj() const = 0;
    };

    /**
     * \brief 0 argument connection
     */
    template <typename object_type>
    class connection0 : public connection0_base
    {
    private:
        object_type* _obj;
        void (object_type::*_memfunc)();

    public:
        connection0(object_type* obj, void (object_type::*memfunc)())
        {
            _obj = obj;
            _memfunc = memfunc;
        }

		connection0(const connection0& other)
		{
			_obj = other._obj;
			_memfunc = other._memfunc;
		}

		inline void clone(connection0_base** out) {
			(*out) = new connection0<object_type>(_obj, _memfunc);
		}

        inline void emit() {
            (_obj->*_memfunc)();
        }

        inline has_slots* getobj() const {
            return _obj;
        }
    };

    /**
     * \brief 1 argument connection
     */
    template <typename object_type, typename t1>
    class connection1 : public connection1_base<t1>
    {
    private:
        object_type* _obj;
        void (object_type::*_memfunc)(t1);

    public:
        connection1(object_type* obj, void (object_type::*memfunc)(t1))
        {
            _obj = obj;
            _memfunc = memfunc;
        }

		connection1(const connection1& other)
		{
			_obj = other._obj;
			_memfunc = other._memfunc;
		}

		inline void clone(connection1_base<t1>** out) {
			(*out) = new connection1<object_type, t1>(_obj, _memfunc);
		}

        inline void emit(t1 p1) {
            (_obj->*_memfunc)(p1);
        }

        inline has_slots* getobj() const {
            return _obj;
        }
    };

	/**
     * \brief 2 argument connection
     */
    template <typename object_type, typename t1, typename t2>
    class connection2 : public connection2_base<t1, t2>
    {
    private:
        object_type* _obj;
        void (object_type::*_memfunc)(t1, t2);

    public:
        connection2(object_type* obj, void (object_type::*memfunc)(t1, t2))
        {
            _obj = obj;
            _memfunc = memfunc;
        }

		connection2(const connection2& other)
		{
			_obj = other._obj;
			_memfunc = other._memfunc;
		}

		inline void clone(connection2_base<t1, t2>** out) {
			(*out) = new connection2<object_type, t1, t2>(_obj, _memfunc);
		}

        inline void emit(t1 p1, t2 p2) {
            (_obj->*_memfunc)(p1, p2);
        }

        inline has_slots* getobj() const {
            return _obj;
        }
    };
}

#endif
//=============================================================================================================
