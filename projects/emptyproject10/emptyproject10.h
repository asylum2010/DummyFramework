//=============================================================================================================
#ifndef _EMPTYPROJECT10_H_
#define _EMPTYPROJECT10_H_

#define _DUMMY_DX 10
#include <dummyframework/base/d3d10/CGame10.h>

class EmptyProject10 : public DummyFramework::CGame10
{
public:
	EmptyProject10();
	~EmptyProject10();

protected:
	bool Initialize();
	bool LoadContent();
	bool LoadSettings();

	void Update();
	void Draw();

eventhandlers:
	void application_keyup(const DummyFramework::skeyboardstate& kstate);
};

#endif
//===============================================================================