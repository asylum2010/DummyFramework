//*************************************************************************************************************
#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <windows.h>

#define PROTOTYPE_UP     "EmptyProject10"
#define PROTOTYPE_LOW    "emptyproject10"
#define PROTOTYPE_DEF    "_EMPTYPROJECT10_H_"
#define CONC(a, b)       a##b

struct filedesc
{
    std::string name;
    std::string suffix;
    bool replace;
    bool lower;
};

int main()
{
    filedesc files[] =
    {
        { PROTOTYPE_UP,  "_VC80.sln",          true, false },
        { PROTOTYPE_UP,  "_VC80.vcproj",       true, false },
        { PROTOTYPE_UP,  "_VC90.sln",          true, false },
        { PROTOTYPE_UP,  "_VC90.vcproj",       true, false },
        { PROTOTYPE_LOW, ".h",                 true, true  },
        { PROTOTYPE_LOW, "_main.cpp",          true, true  },
        { PROTOTYPE_LOW, "_eventhandlers.cpp", true, true  },
        { "",            "main.cpp",           true, true  }
    };
    
	// create directory
    std::string projectname, projectdir, dest;

	size_t proto_up = strlen(PROTOTYPE_UP);
	size_t proto_low = strlen(PROTOTYPE_LOW);
	size_t proto_def = strlen(PROTOTYPE_DEF);

    std::cout << "Project name: ";
    std::cin >> projectname;

    projectdir = projectname;
    std::transform(projectname.begin(), projectname.end(), projectdir.begin(), tolower);

    CreateDirectoryA(projectdir.c_str(), NULL);
    size_t count = sizeof(files) / sizeof(files[0]);

	// copy files
    for( size_t i = 0; i < count; ++i )
    {
        dest = projectdir + "\\";

        if( files[i].lower )
        {
            dest +=
                (files[i].name == "" ? files[i].suffix : projectdir + files[i].suffix);

            CopyFileA(
                (CONC(PROTOTYPE_LOW, "\\") + files[i].name + files[i].suffix).c_str(),
                dest.c_str(), true);
        }
        else
        {
            dest +=
                (files[i].name == "" ? files[i].suffix : projectname + files[i].suffix);
            
            CopyFileA(
                (CONC(PROTOTYPE_LOW, "\\") + files[i].name + files[i].suffix).c_str(),
                dest.c_str(), true);
        }

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
                str.replace(pos, proto_low, projectdir);
                pos++;
                ++cnt;
            }

            length -= cnt * (proto_low - (signed)projectdir.length());

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

