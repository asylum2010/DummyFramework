//=============================================================================================================
#ifndef _ANIMATEDBUTTON_H_
#define _ANIMATEDBUTTON_H_

#include <dummyframework/menu/CButton.h>

/**
 * \brief An animatable button
 */
class AnimatedButton : public DummyFramework::CButton
{
	functor triangle
	{
		inline static float y(float x) {
			return (0.5f - fabs(x - 0.5f)) * 2;
		}
	};

protected:
	DummyFramework::syncedanimator<float, triangle> alpha;
	D3DXVECTOR4 bounds;
	float scale;

	virtual void UpdateBounds();

public:
	AnimatedButton();
	~AnimatedButton() {}

	//! Initialize content 
	bool LoadContent();

	//! Returns true if the point is over the button
	bool MouseOver(short mx, short my);

	//! Set the button's state
	void SetState(unsigned int newstate);

	//! Update the button
	void Update();

	//! Render the button
	void Draw();

	inline void SetText(const std::string& newtext) {
		CButton::SetText(newtext);
		UpdateBounds();
	}

_DUMMY_EVENTHANDLERS:
	void onresetdevice();
};

#endif
//=============================================================================================================
