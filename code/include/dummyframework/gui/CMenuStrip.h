//=============================================================================================================
#ifndef _CMENUSTRIP_H_
#define _CMENUSTRIP_H_

#include "CWindow.h"
#include <vector>

namespace DummyFramework
{
	class CMenuStrip : public CWindow
	{
		struct popupmenu
		{
			HMENU handle;
			std::string text;
			int items;
			int ids[32];
		};

	protected:
		typedef std::vector<popupmenu> menulist;

		HMENU hmenu;
		menulist submenus;

	public:
		CMenuStrip();
		~CMenuStrip();

		bool Initialize(HWND parent);
		LRESULT ProcessCommands(WPARAM wparam, LPARAM lparam);

		size_t AddMenu(const std::string& text);
		size_t AddItem(size_t menuid, const std::string& text);
		size_t AddSeparator(size_t menuid);

		void EnableItem(size_t menuid, size_t itemid, bool enable) const
		{
			const popupmenu& pm = submenus[menuid];
			EnableMenuItem(pm.handle, pm.ids[itemid], (enable ? MF_BYCOMMAND|MF_ENABLED : MF_BYCOMMAND|MF_GRAYED));
		}

		inline WORD ID(size_t menuid, size_t itemid) const {
			return submenus[menuid].ids[itemid];
		}
	};
}

#endif
//=============================================================================================================
