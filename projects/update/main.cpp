//*************************************************************************************************************
#ifdef _DEBUG
#	pragma comment(lib, "../../code/lib/Debug/DummyFramework.lib")
#else
#   pragma comment(lib, "../../code/lib/Release/DummyFramework.lib")
#endif

#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <stack>
#include <windows.h>

#include <dummyframework/base/CHelper.h>

int main()
{
	WIN32_FIND_DATA data;

	std::stack<HANDLE> handles;
	std::stack<std::string> path;
	std::string file, ext, name, type;
	
	HANDLE h = FindFirstFileA("*.*", &data);
	handles.push(h);
	path.push(std::string("."));

	if( h != INVALID_HANDLE_VALUE )
	{
		while( !handles.empty() )
		{
			h = handles.top();

			do
			{
				file = data.cFileName;
				DummyFramework::CHelper::GetExtension(ext, file);

				if( data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
				{
					if( file != "." && file != ".." )
					{
						std::string p = path.top();
						p += "/" + file;

						path.push(p);

						std::cout << "Changed to " << p << "\n";
						memset(data.cFileName, 0, sizeof(data.cFileName));

						h = FindFirstFileA((p + "/*.*").c_str(), &data);

						if( h != INVALID_HANDLE_VALUE )
						{
							handles.push(h);
							continue;
						}
						else
						{
							h = handles.top();
							std::cout << "No files found\n";
						}
					}
				}
				else if( ext == "vcproj" )
				{
					DummyFramework::CHelper::GetName(name, file);
					size_t pos = name.find_last_of("_");

					if( pos != std::string::npos )
					{
						type = name.substr(pos);

						if( type == "_VC90" )
						{
							file = path.top() + "/" + file;
							std::ifstream infile(file.c_str());

							if( infile.fail() )
							{
								std::cout << "Could not open \'" << file << "\'\n";
							}
							else
							{
								// megkeresni az egyikben
								pos = infile.tellg();
								infile.seekg(0, std::ios::end);

								pos = (size_t)infile.tellg() - pos;
								infile.seekg(0, std::ios::beg);

								std::string buff, part;

								buff.resize(pos);
								infile.read(&buff[0], pos);
								infile.close();

								size_t start = buff.find("<Files>");
								size_t end = buff.find("</Files>");

								if( start == std::string::npos || end == std::string::npos )
									part = "\n";
								else
									part = buff.substr(start, end - start + 8);

								infile.clear();

								// replace a másikban
								std::string repl;
								DummyFramework::CHelper::Replace(repl, "_VC90", "_VC80", file);

								infile.open(repl.c_str());

								pos = infile.tellg();
								infile.seekg(0, std::ios::end);

								pos = (size_t)infile.tellg() - pos;
								infile.seekg(0, std::ios::beg);

								memset(&buff[0], ' ', buff.length());
								buff.resize(pos, ' ');
								infile.read(&buff[0], pos);
								infile.close();

								start = buff.find("<Files>");
								end = buff.find("</Files>");

								if( start != std::string::npos && end != std::string::npos )
								{
									buff.replace(start, end - start + 8, part);

									std::ofstream of(repl.c_str());
									of << buff;
									of.close();

									std::cout << file << " -> " << repl << "\n";
								}
								else
									std::cout << "Target file has no <Files></Files> tags\n";
							}
						}
					}
				}
				//else
				//	std::cout << "Skipping " << file << "\n";
			}
			while( FindNextFileA(h, &data) != 0 );

			FindClose(h);

			handles.pop();
			path.pop();

			if( !path.empty() )
			{
				std::cout << "Changed to " << path.top() << "\n";
				memset(data.cFileName, 0, sizeof(data.cFileName));
			}
		}
	}

	system("pause");
	return 0;
}
//*************************************************************************************************************

