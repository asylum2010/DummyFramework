//=============================================================================================================
#ifndef _CLABEL_H_
#define _CLABEL_H_

#include "../base/d3d9/CGame9.h"
#include "../base/d3d9/CSpriteFont9.h"
#include "CControl.h"

namespace DummyFramework
{
	class CLabel : public CControl
	{
	protected:
		size_t textid;

	public:
		D3DXVECTOR2		Position;
		D3DXCOLOR		Color;
		std::string		Text;

		CLabel();
		~CLabel() {}

		bool Initialize(CGame9& mygame, CSpriteFont9& font);
		bool LoadContent();
		void Draw();
		void SetState(unsigned int newstate);

		inline void GetSize(D3DXVECTOR2& out) const {
			spritefont->MeasureText(out, GroupID, textid);
		}
	};
}

#endif
//=============================================================================================================
