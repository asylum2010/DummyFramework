//=============================================================================================================
#ifndef _EMPTYPROJECT9_H_
#define _EMPTYPROJECT9_H_

#define _DUMMY_DX 9
#include <dummyframework/base/d3d9/CGame9.h>

class EmptyProject9 : public DummyFramework::CGame9
{
public:
	EmptyProject9();
	~EmptyProject9();

protected:
	bool Initialize();
	bool LoadContent();
	bool LoadSettings();

	void Update();
	void Draw();

_DUMMY_EVENTHANDLERS:
	void application_keyup(const DummyFramework::skeyboardstate& kstate);
};

#endif
//===============================================================================================