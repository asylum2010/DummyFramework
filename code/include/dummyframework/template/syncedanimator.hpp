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
	public:
		value_type	previous;
		value_type	current;
		value_type	value;

		syncedlinear() {
		}

		syncedlinear(const value_type& t) {
			previous = current = t;
		}

		syncedlinear& operator =(const value_type& t) {
			previous = current = t;
			return *this;
		}

		inline void extend(const value_type& t) {
			previous = current;
			current += t;
		}
		
		inline void clamp(const value_type& low, const value_type& high) {
			current = (current < low ? low : (current > high ? high : current));
		}

		inline void smooth(float alpha) {
			value = previous + (current - previous) * alpha;
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
	template <typename value_type, typename poly = linear<value_type> >
	class syncedanimator
	{
	protected:
		value_type*		subdivision;
		int				cnt;

	public:
		value_type		value;
		int				previous;
		int				current;
		char			direction;
		bool			continous;

		syncedanimator() {
			previous		= 0;
			current			= 0;
			direction		= 1;
			continous		= false;
			subdivision		= 0;
			cnt				= 0;
		}

		syncedanimator(const syncedanimator& other) {
			previous		= 0;
			current			= 0;
			direction		= 1;
			continous		= false;
			subdivision		= 0;
			cnt				= 0;

			operator =(other);
		}

		~syncedanimator() {
			safe_delete_array(subdivision);
		}
		
		syncedanimator& operator =(int i);
		syncedanimator& operator =(const syncedanimator& other);

		void set(int n, const value_type& low, const value_type& high);
		void step();

		inline void smooth(float alpha) {
			value = subdivision[previous] + (subdivision[current] - subdivision[previous]) * alpha;
		}

		inline int count() const {
			return cnt;
		}
		
		inline const value_type& low() const {
			return subdivision[0];
		}

		inline const value_type& high() const {
			return subdivision[cnt - 1];
		}
	};

	template <typename value_type, typename poly>
	syncedanimator<value_type, poly>& syncedanimator<value_type, poly>::operator =(int i)
	{
		previous = current = i;

		if( subdivision )
			value = subdivision[i];

		return *this;
	}

	template <typename value_type, typename poly>
	syncedanimator<value_type, poly>& syncedanimator<value_type, poly>::operator =(const syncedanimator& other)
	{
		if( &other != this )
		{
			cnt = 0;

			previous	= other.previous;
			current		= other.current;
			direction	= other.direction;
			continous	= other.continous;
			value		= other.value;

			set(other.cnt, other.subdivision[0], other.subdivision[other.cnt - 1]);
		}

		return *this;
	}

	template <typename value_type, typename poly>
	void syncedanimator<value_type, poly>::set(int n, const value_type& low, const value_type& high)
	{
		safe_delete_array(subdivision);
		subdivision = new value_type[n];

		subdivision[0] = low;
		subdivision[n - 1] = high;

		cnt = n;

		for( int i = 1; i < n - 1; ++i )
		{
			subdivision[i] =
				poly::y(((float)i / (float)(n - 1)));

			// DO NOT USE '* (high - low) + low'
		}
	}

	template <typename value_type, typename poly>
	void syncedanimator<value_type, poly>::step()
	{
		previous = current;

		if( continous )
		{
			current = (current + direction) % cnt;

			if( previous > current )
			{
				previous = current;
				++current;
			}
		}
		else
			current = min(max(0, current + direction), cnt - 1);

		value = subdivision[current];
	}
}

#endif
//=============================================================================================================
