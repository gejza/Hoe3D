
#ifndef _HOE_CONFIG_
#define _HOE_CONFIG_

#include "hoe_format.h"

struct THoeInitSettings;

class Config
{
	bool fullscreen;
	uint fs_width;
	uint fs_height;
public:
	Config();
	~Config();

	static const char * GetHoeDesc();
	static int GetHoeVersion()
	{
		return 92;
	}
	static const char * GetBuild()
	{
		return "Build time: "__TIME__" "__DATE__;
	}
	static void GetOSDesc(char *);
	bool CheckTexture(dword &width,dword &height,HOEFORMAT &format);

	bool Check(THoeInitSettings *);

	bool IsFullscreen();
	uint GetWidthView();
	uint GetHeightView();

};


#endif // _HOE_CONFIG_
