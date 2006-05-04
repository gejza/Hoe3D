
#include "StdAfx.h"
#include "hoe_utils.h"

BEGIN_HOEGAME

#ifdef _LINUX
#define MAX_PATH 512
#endif

bool SetRootDir(const char * dir)
{
#ifdef _WIN32
	return (SetCurrentDirectory(dir) != FALSE);
#endif
#ifdef _LINUX
	return (chdir(dir) == 0);
#endif
}

const char * GetBaseDir(const char * path)
{
	static char basedir[ MAX_PATH ];
	int j;

	strcpy( basedir, path );

	for (j=strlen(basedir)-1;j > 0;j--)
	{
 		if (basedir[j] == '/' || basedir[j] == '\\')
		{
			basedir[j] = '\0';
			return basedir;
		}
	}

	return ".";
}

bool SetRootFromExe(const char * path)
{
	return SetRootDir(GetBaseDir(path));
}

#ifdef _WIN32
bool SetRootFromInstance(HINSTANCE hInstance)
{
	char moduleName[ MAX_PATH ];
	if ( !GetModuleFileName( hInstance, moduleName, MAX_PATH ) )
	{
		return false;
	}

	return SetRootFromExe(moduleName);
}
#endif

END_HOEGAME
