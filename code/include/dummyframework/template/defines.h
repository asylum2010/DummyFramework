//=============================================================================================================
#ifndef _DEFINES_H_
#define _DEFINES_H_

#define _DUMMY_WIN32

#ifndef _DUMMY_DX
#   define _DUMMY_DX   9
#endif

#include <iostream>
#include <set>

#ifdef _DEBUG
#   define derror(r, e, x) { if( !(x) ) { std::cout << ":( " << e << "!\n"; return r; } }
#   define dnerror(r, e, x) { if( (x) ) { std::cout << ":( " << e << "!\n"; return r; } }
#else
#   define derror(r, e, x)
#   define dnerror(r, e, x)
#endif

#define dassert(r, e, x) { if( !(x) ) { std::cout << ":( " << e << "!\n"; return r; } }
#define dnassert(r, e, x) { if( (x) ) { std::cout << ":( " << e << "!\n"; return r; } }
#define dhappy(x) { std::cout << ":) " << (x) << "!\n"; }
#define dsad(x) { std::cout << ":( " << (x) << "!\n"; }

#define safe_release(x) { if( (x) ) { (x)->Release(); (x) = NULL; } }
#define safe_delete(x) { if( (x) ) { delete (x); (x) = NULL; } }
#define safe_delete_array(x) { if( (x) ) { delete[] (x); (x) = NULL; } }

#define safe_free(x) { if( (x) ) { free(x); (x) = NULL; } }
#define functor struct

#ifndef events
#   define events public
#endif

#ifndef eventhandlers
#   define eventhandlers public
#endif

#ifndef min
#   define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef max
#   define max(a, b) (((a) > (b)) ? (a) : (b))
#endif

#include "syncedanimator.hpp"

namespace DummyFramework
{
	/**
	 * \brief 2D point
	 */
	template <typename T>
    struct point2
    {
        T x, y;
        
        point2() : x(0), y(0) {}
        point2(T _x, T _y) : x(_x), y(_y) {}
    };

	/**
	 * \brief 3D point
	 */
	template <typename T>
    struct point3
    {
        T x, y, z;
        
        point3() : x(0), y(0), z(0) {}
        point3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}
    };

	/**
	 * \brief 4D point
	 */
	template <typename T>
	struct point4
    {
        T x, y, z, w;
        
        point4() : x(0), y(0), z(0), w(0) {}
        point4(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) {}
    };

	/**
	 * \brief Resolution ordering functor
	 */
    struct resolution_less
    {
        bool operator ()(const std::string& a, const std::string& b) const {
            if( a.length() < b.length() )
                return true;
            else if( a.length() > b.length() )
                return false;
            else
                return a.compare(b) < 0;
        }
    };

    typedef std::set<std::string, resolution_less> resolutionset;
}

#endif
//=============================================================================================================

