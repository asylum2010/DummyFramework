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

        virtual void emit(t1 p1) = 0;
        virtual has_slots* getobj() const = 0;
    };
    
    /**
     * \brief 0 argument connection
     */
    template <typename object_type>
    class connection0 : public connection0_base
    {
    private:
        object_type* m_obj;
        void (object_type::*m_memfunc)();

    public:
        connection0(object_type* obj, void (object_type::*memfunc)())
        {
            m_obj = obj;
            m_memfunc = memfunc;
        }
        
        inline void emit() {
            (m_obj->*m_memfunc)();
        }

        inline has_slots* getobj() const {
            return m_obj;
        }
    };

    /**
     * \brief 1 argument connection
     */
    template <typename object_type, typename t1>
    class connection1 : public connection1_base<t1>
    {
    private:
        object_type* m_obj;
        void (object_type::*m_memfunc)(t1);

    public:
        connection1(object_type* obj, void (object_type::*memfunc)(t1))
        {
            m_obj = obj;
            m_memfunc = memfunc;
        }

        inline void emit(t1 p1) {
            (m_obj->*m_memfunc)(p1);
        }

        inline has_slots* getobj() const {
            return m_obj;
        }
    };
}

#endif
//=============================================================================================================
