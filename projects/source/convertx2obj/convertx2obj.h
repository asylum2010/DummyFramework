//=============================================================================================================
#ifndef _CONVERTX2OBJ_H_
#define _CONVERTX2OBJ_H_

#define _DUMMY_DX 9
#include <dummyframework/base/d3d9/CGame9.h>

class ConvertX2OBJ : public DummyFramework::CGame9
{
public:
	ConvertX2OBJ();
	~ConvertX2OBJ();

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