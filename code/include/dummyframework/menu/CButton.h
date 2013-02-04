//=============================================================================================================
#ifndef _CBUTTON_H_
#define _CBUTTON_H_

#include "CLabel.h"

namespace DummyFramework
{
	class CButton : public CLabel
	{
	protected:
		D3DXCOLOR currentcolor;

	public:
		D3DXCOLOR ActiveColor;

		CButton();
		~CButton() {}

		void Draw();
		void SetState(unsigned int newstate);
	};
}

#endif
//=============================================================================================================
 
 
 
