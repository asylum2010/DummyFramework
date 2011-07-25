//=============================================================================================================
#ifndef _SYNCEDANIMATOR_HPP_
#define _SYNCEDANIMATOR_HPP_

namespace DummyFramework
{
	/**
	 * \brief Framerate independent value
	 */
    template <typename value_type>
    class syncedlinear
    {
    protected:
        value_type prev;
        value_type curr;

    public:
        value_type value;

        syncedlinear() {}

        syncedlinear(const value_type& t) {
            prev = curr = t;
        }

        syncedlinear& operator =(const value_type& t) {
            prev = curr = t;
            return *this;
        }

        void extend(const value_type& t)
		{
            prev = curr;
            curr += t;
        }
        
        inline void clamp(const value_type& low, const value_type& high) {
            curr = (curr < low ? low : (curr > high ? high : curr));
        }

        inline void smooth(float alpha) {
            value = prev + (curr - prev) * alpha;
        }

        inline value_type& current() {
            return curr;
        }

        inline const value_type& current() const {
            return curr;
        }

        inline value_type& previous() {
            return prev;
        }

        inline const value_type& previous() const {
            return prev;
        }
    };
    
	/**
	 * \brief Basic linear function
	 */
    template <typename value_type>
    class linear
    {
    public:
        inline static value_type y(const value_type& x) {
            return x;
        }
    };

	/**
	 * \brief Framerate independent animator
	 */
    template <typename value_type, int n, typename poly = linear<value_type> >
    class syncedanimator
    {
    protected:
        value_type subdivision[n];
        int prev, curr;

    public:
        value_type value;
        char direction;
        bool continous;

        syncedanimator()
		{
            prev = curr = 0;
            direction = 1;
            continous = false;
        }

        syncedanimator& operator =(int i)
		{
            prev = curr = i;
            value = subdivision[i];
            return *this;
        }

        void set(const value_type& low, const value_type& high)
		{
            subdivision[0] = low;
            subdivision[n - 1] = high;

            for( int i = 1; i < n - 1; ++i )
            {
                subdivision[i] =
                    poly::y(((float)i / (float)(n - 1)));
            }
        }

        void step()
		{
            prev = curr;

            if( continous )
            {
                curr = (curr + direction) % n;

                if( prev > curr )
                {
                    prev = curr;
                    ++curr;
                }
            }
            else
                curr = min(max(0, curr + direction), n - 1);

            value = subdivision[curr];
        }

        inline void smooth(float alpha) {
            value = subdivision[prev] + (subdivision[curr] - subdivision[prev]) * alpha;
        }

        inline int count() const {
            return n;
        }

        inline int current() const {
            return curr;
        }

        inline int previous() const {
            return prev;
        }

		inline int& current() {
            return curr;
        }

        inline int& previous() {
            return prev;
        }

		inline const value_type& low() const {
			return subdivision[0];
		}

		inline const value_type& high() const {
			return subdivision[n - 1];
		}
    };
}

#endif
//=============================================================================================================
