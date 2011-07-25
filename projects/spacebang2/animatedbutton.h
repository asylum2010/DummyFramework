//=============================================================================================================
#ifndef _ANIMATEDBUTTON_H_
#define _ANIMATEDBUTTON_H_

#include <dummyframework/menu/CButton.h>

/**
 * \brief An animable button
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
    DummyFramework::syncedanimator<float, 12, triangle> alpha;
    float scale;

public:
    AnimatedButton();
    ~AnimatedButton() {}

	//! Initialize content 
    bool LoadContent();

	//! Set the button's state
    void SetState(unsigned long newstate);

	//! Update the button
    void Update();

	//! Render the button
    void Draw();

eventhandlers:
	void onresetdevice();
};

#endif
//=============================================================================================================