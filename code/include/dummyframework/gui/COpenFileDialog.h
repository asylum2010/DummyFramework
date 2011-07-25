//=============================================================================================================
#ifndef _COPENFILEDIALOG_H_
#define _COPENFILEDIALOG_H_

#include <windows.h>
#include <string>

namespace DummyFramework
{
	class COpenFileDialog
	{
	protected:
		OPENFILENAMEA ofn;

	public:
		std::string Filter;
		std::string FileName;
		std::string InitialDirectory;

		COpenFileDialog();
		~COpenFileDialog();

		bool Show(HWND parent);
	};
}

#endif
//=============================================================================================================
