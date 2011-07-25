//=============================================================================================================
#ifndef _BIJECTIVEMAP_HPP_
#define _BIJECTIVEMAP_HPP_

#include <map>

namespace DummyFramework
{
    template <typename key_type, typename value_type>
    class bijectivemap
    {
    protected:
        std::map<key_type, value_type> forwardmap;
        std::map<value_type, key_type> reversemap;

    public:
        typedef typename std::map<key_type, value_type>::iterator key_iterator;
        typedef typename std::map<key_type, value_type>::const_iterator const_key_iterator;

        void removebykey(const key_type& k);
        bool getvalue(value_type& out, const key_type& k);
        bool getkey(key_type& out, const value_type& v);

		inline void insert(const key_type& k, const value_type& v) {
            forwardmap[k] = v;
            reversemap[v] = k;
        }

        inline void clear() {
            forwardmap.clear();
            reversemap.clear();
        }

        inline size_t size() const {
			return forwardmap.size();
		}

        inline size_t keycount(const key_type& k) const {
			return forwardmap.count(k);
		}

        inline size_t valuecount(const value_type& v) const {
			return reversemap.count(v);
		}

        inline key_iterator kbegin() {
			return forwardmap.begin();
		}

        inline key_iterator kend() {
			return forwardmap.end();
		}
    };
	//=============================================================================================================
	template <typename key_type, typename value_type>
	void bijectivemap<key_type, value_type>::removebykey(const key_type& k)
    {
        std::map<key_type, value_type>::iterator it = forwardmap.find(k);

        if( it != forwardmap.end() )
        {
            value_type v = it->second;

            forwardmap.erase(k);
            reversemap.erase(v);
        }
    }
	//=============================================================================================================
	template <typename key_type, typename value_type>
    bool bijectivemap<key_type, value_type>::getvalue(value_type& out, const key_type& k)
    {
        std::map<key_type, value_type>::iterator it = forwardmap.find(k);

        if( it != forwardmap.end() )
        {
            out = it->second;
            return true;
        }

        return false;
    }
	//=============================================================================================================
	template <typename key_type, typename value_type>
    bool bijectivemap<key_type, value_type>::getkey(key_type& out, const value_type& v)
    {
        std::map<value_type, key_type>::iterator it = reversemap.find(v);

        if( it != reversemap.end() )
        {
            out = it->second;
            return true;
        }

        return false;
    }
}

#endif
//=============================================================================================================

