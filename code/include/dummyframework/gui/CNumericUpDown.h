//=============================================================================================================
#ifndef _CNUMERICUPDOWN_H_
#define _CNUMERICUPDOWN_H_

#include "CWindow.h"

namespace DummyFramework
{
	enum eupdownaction
	{
		uda_valuechanged = 2
	};

	class CNumericUpDown : public CWindow
	{
	protected:
		HWND buddy;
		short buddyid;
		double value;

	public:
		double MinValue;
		double MaxValue;
		double Increment;
		unsigned char Precision;

		CNumericUpDown();
		~CNumericUpDown();

		bool Initialize(HWND parent);
		LRESULT ProcessCommands(WPARAM wparam, LPARAM lparam);

		void SetValue(double v);

		inline short BuddyID() const {
			return buddyid;
		}

		inline HWND GetBuddyHandle() {
			return buddy;
		}

		inline double GetValue() const {
			return value;
		}

		inline void Enable(bool enable) const {
			EnableWindow(hwnd, enable);
			EnableWindow(buddy, enable);
		}
	};
}

#endif
//=============================================================================================================
