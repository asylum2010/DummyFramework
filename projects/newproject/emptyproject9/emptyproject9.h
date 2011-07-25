//=============================================================================================================
#ifndef _EMPTYPROJECT9_H_
#define _EMPTYPROJECT9_H_

#include <dummyframework/base/d3d9/CGame9.h>
#include <dummyframework/base/d3d9/CObject9.h>
#include <vector>

class EmptyProject9 : public DummyFramework::CGame9
{
public:
	EmptyProject9();
	~EmptyProject9();

protected:
	bool Initialize();
	bool LoadContent();
	bool LoadSettings();

	void ResetRenderStates();
	void Update();
	void Draw();

private:
	typedef std::vector<DummyFramework::CObject9> objectlist;

eventhandlers:
	void application_keyup(const DummyFramework::skeyboardstate& kstate);
};

#endif
//===============================================================================================