//=============================================================================================================
#ifndef _WINAPITEST_H_
#define _WINAPITEST_H_

#define _DUMMY_DX 9
#include <dummyframework/base/d3d9/CGame9.h>
#include <dummyframework/base/CHelper.h>
#include <dummyframework/renderers/d3d9/CBloomRenderer9.h>

#include <dummyframework/gui/CMenuStrip.h>
#include <dummyframework/gui/CPictureBox.h>
#include <dummyframework/gui/CStatusStrip.h>
#include <dummyframework/gui/COpenFileDialog.h>
#include <dummyframework/gui/CGroupBox.h>
#include <dummyframework/gui/CTrackBar.h>
#include <dummyframework/gui/CStatic.h>
#include <dummyframework/gui/CNumericUpDown.h>
#include <dummyframework/gui/CPushButton.h>

class WinapiTest : public DummyFramework::CGame9
{
public:
	WinapiTest();
	~WinapiTest();

protected:
	bool Initialize();
	bool LoadContent();
	bool LoadSettings();

	void InitializeGUI();
	void Update();
	void Draw();

private:
	DummyFramework::CMenuStrip       menustrip1;
	DummyFramework::CPictureBox      picturebox1;
	DummyFramework::CStatusStrip     statusstrip1;
	DummyFramework::COpenFileDialog  openfile1;
	DummyFramework::CGroupBox        groupbox1;
	DummyFramework::CPushButton      button1;
	DummyFramework::CNumericUpDown   updown1;
	DummyFramework::CTrackBar        trackbar1;
	DummyFramework::CStatic          label1;

	DummyFramework::CBloomRenderer9* bloomrenderer;

	LPDIRECT3DTEXTURE9 current;
	D3DXMATRIX world;

eventhandlers:
	void application_keyup(const DummyFramework::skeyboardstate& kstate);
	void application_command(const DummyFramework::scommand& cmd);
	void application_notify(const DummyFramework::scommand& cmd);
};

#endif
//===============================================================================