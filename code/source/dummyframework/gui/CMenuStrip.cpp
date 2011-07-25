//=============================================================================================================
#include <dummyframework/gui/CMenuStrip.h>

namespace DummyFramework
{
	CMenuStrip::CMenuStrip()
		: CWindow()
	{
		hmenu = 0;
		submenus.reserve(10);
	}
	//=============================================================================================================
	CMenuStrip::~CMenuStrip()
	{
	}
	//=============================================================================================================
	bool CMenuStrip::Initialize(HWND parent)
	{
		if( hmenu )
			return true;

		CWindow::Initialize(parent);
		hmenu = CreateMenu();

		for( size_t i = 0; i < submenus.size(); ++i )
		{
			popupmenu& pm = submenus[i];
			AppendMenuA(hmenu, MF_STRING|MF_POPUP, (UINT)pm.handle, pm.text.c_str());
		}

		SetMenu(parent, hmenu);
		return (hmenu != NULL);
	}
	//=============================================================================================================
	LRESULT CMenuStrip::ProcessCommands(WPARAM wparam, LPARAM lparam)
	{
		return 0;
	}
	//=============================================================================================================
	size_t CMenuStrip::AddMenu(const std::string& text)
	{
		popupmenu pm;

		pm.handle = CreatePopupMenu();
		pm.text = text;
		pm.items = 0;

		memset(pm.ids, -1, 32);

		submenus.push_back(pm);
		return submenus.size() - 1;
	}
	//=============================================================================================================
	size_t CMenuStrip::AddItem(size_t menuid, const std::string& text)
	{
		// TODO: errorhandling
		popupmenu& pm = submenus[menuid];
		pm.ids[pm.items] = GenerateID();
		
		AppendMenuA(pm.handle, MF_STRING, (UINT_PTR)pm.ids[pm.items], text.c_str());
		++pm.items;

		return pm.items;
	}
	//=============================================================================================================
	size_t CMenuStrip::AddSeparator(size_t menuid)
	{
		// TODO: errorhandling
		popupmenu& pm = submenus[menuid];
		pm.ids[pm.items] = GenerateID();

		AppendMenuA(pm.handle, MF_SEPARATOR, (UINT_PTR)pm.ids[pm.items], 0);
		++pm.items;

		return pm.items;
	}
	//=============================================================================================================
}
