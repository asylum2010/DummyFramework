//=============================================================================================================
#include <dummyframework/gui/COpenFileDialog.h>
#include <dummyframework/base/CHelper.h>

namespace DummyFramework
{
	COpenFileDialog::COpenFileDialog()
	{
		memset(&ofn, 0, sizeof(OPENFILENAMEA));

		Filter = "All Files (*.*)|*.*";
		InitialDirectory = "./";

		FileName.resize(512);

		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.lpstrCustomFilter = 0;
		ofn.nMaxCustFilter = 0;
		ofn.nFilterIndex = 0;
		ofn.nMaxFile = 512;
		ofn.lpstrFileTitle = 0;
		ofn.nMaxFileTitle = 0;
		ofn.Flags = OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST;
		ofn.nFileOffset = 0;
		ofn.nFileExtension = 0;
		ofn.lpstrDefExt = 0;
		ofn.lCustData = 0;
		ofn.lpfnHook = 0;
		ofn.lpTemplateName = 0;
	}
	//=============================================================================================================
	COpenFileDialog::~COpenFileDialog()
	{
	}
	//=============================================================================================================
	bool COpenFileDialog::Show(HWND parent)
	{
		FileName.resize(512, '\0');
		FileName[0] = '\0';

		std::string tmp;
		CHelper::Replace(tmp, '|', '\0', Filter + "||");
		
		ofn.hwndOwner = parent;
		ofn.lpstrFilter = tmp.c_str();
		ofn.lpstrFile = &FileName[0];
		ofn.lpstrInitialDir = InitialDirectory.c_str();

		BOOL result = GetOpenFileNameA(&ofn);

		if( result > 0 )
		{
			//std::string tmp;

			size_t pos = FileName.find_first_of('\0', 0);
			FileName = FileName.substr(0, pos);

			//CHelper::Crop(tmp, FileName, '\0');
			//FileName = tmp;
		}

		return (result > 0);
	}
	//=============================================================================================================
}

