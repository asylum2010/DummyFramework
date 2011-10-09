//*************************************************************************************************************
#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <windows.h>

#define _QUANTUM       1
#define _DUMMY_DX9     9
#define _DUMMY_DX10    10

#define _TARGET_TYPE   _DUMMY_DX10
#define CONC(a, b)     a##b

#if _TARGET_TYPE == _DUMMY_DX10
#   define PROTOTYPE_UP     "EmptyProject10"
#   define PROTOTYPE_LOW    "emptyproject10"
#   define PROTOTYPE_DEF    "_EMPTYPROJECT10_H_"
#elif _TARGET_TYPE == _DUMMY_DX9
#   define PROTOTYPE_UP     "EmptyProject9"
#   define PROTOTYPE_LOW    "emptyproject9"
#   define PROTOTYPE_DEF    "_EMPTYPROJECT9_H_"
#elif _TARGET_TYPE == _QUANTUM
#   define PROTOTYPE_UP     "EmptyProject"
#   define PROTOTYPE_LOW    "emptyproject"
#   define PROTOTYPE_DEF    "_EMPTYPROJECT_H_"
#endif

struct filedesc
{
	std::string folder;
    std::string name;
    std::string suffix;
    bool replace;
    bool lower;
	bool source;
};

int main()
{
    filedesc files[] =
    {
        { "vs\\vc90\\", PROTOTYPE_UP,  ".vcproj",    true, false, false },
        { "vs\\vc100\\", PROTOTYPE_UP,  ".vcxproj",  true, false, false },

        { "", PROTOTYPE_LOW, ".h",                   true, true, true  },
        { "", PROTOTYPE_LOW, "_main.cpp",            true, true, true  },
        { "", PROTOTYPE_LOW, "_eventhandlers.cpp",   true, true, true  },
        { "", "",            "main.cpp",             true, true, true  }
    };
    
	std::string projectname, projectname_low, projectdir, dest, src;
	std::string protosrcdir, protoprojdir, sourcedir;

	size_t proto_up = strlen(PROTOTYPE_UP);
	size_t proto_low = strlen(PROTOTYPE_LOW);
	size_t proto_def = strlen(PROTOTYPE_DEF);

    std::cout << "Project name: ";
    std::cin >> projectname;

	projectname_low = projectname;
    std::transform(projectname.begin(), projectname.end(), projectname_low.begin(), tolower);

	sourcedir = "source\\" + projectname_low;
	protosrcdir = std::string("source\\") + CONC(PROTOTYPE_LOW, "\\");

    CreateDirectoryA(sourcedir.c_str(), NULL);

    CreateDirectoryA(projectdir.c_str(), NULL);
    size_t count = sizeof(files) / sizeof(files[0]);

	// copy files
    for( size_t i = 0; i < count; ++i )
    {
		projectdir = protoprojdir = files[i].folder;

		if( files[i].source )
		{
			dest = sourcedir + "\\";
			src = protosrcdir + files[i].name + files[i].suffix;
		}
		else
		{
			dest = projectdir + "\\";
			src = protoprojdir + files[i].name + files[i].suffix;
		}

        if( files[i].lower )
        {
            dest +=
                (files[i].name == "" ? files[i].suffix : projectname_low + files[i].suffix);

			std::cout << "copying\n    " << src << " to\n    " << dest << "\n\n";

            CopyFileA(
                src.c_str(),
                dest.c_str(), true);
        }
        else
        {
            dest +=
                (files[i].name == "" ? files[i].suffix : projectname + files[i].suffix);
            
			std::cout << "copying\n    " << src << " to\n    " << dest << "\n\n";

            CopyFileA(
                src.c_str(),
                dest.c_str(), true);
        }

		// replace strings
        if( files[i].replace )
        {
            std::ifstream f(dest.c_str(), std::ifstream::binary);

            f.seekg(0, std::ios::end);
            int length = f.tellg();
            f.seekg(0, std::ios::beg);

            std::string str;
            str.resize(length + 1, '\0');

            f.read(&str[0], length);

            std::string::size_type pos = 0;
            size_t cnt = 0;

            while( (pos = str.find(PROTOTYPE_LOW, pos)) != std::string::npos )
            {
                str.replace(pos, proto_low, projectname_low);

                pos++;
                ++cnt;
            }

            length -= cnt * (proto_low - (signed)projectname_low.length());

            cnt = 0;
            pos = 0;

            while( (pos = str.find(PROTOTYPE_UP, pos)) != std::string::npos )
            {
                str.replace(pos, proto_up, projectname);

                pos++;
                ++cnt;
            }

            length -= cnt * (proto_up - (signed)projectname.length());

            cnt = 0;
            pos = 0;

            std::string tmp = projectname;
            std::transform(tmp.begin(), tmp.end(), tmp.begin(), toupper);

            while( (pos = str.find(PROTOTYPE_DEF, pos)) != std::string::npos )
            {
                str.replace(pos, proto_def, "_" + tmp + "_H_");

                pos++;
                ++cnt;
            }

            length -= cnt * (proto_def - (signed)projectname.length() + 4);
            f.close();

            std::ofstream of(dest.c_str(), std::ofstream::binary);
            of.write(str.c_str(), length);
            of.close();
        }
    }

    system("pause");
    return 0;
}
//*************************************************************************************************************

