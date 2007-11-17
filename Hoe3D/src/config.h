
#ifndef _HOE_CONFIG_
#define _HOE_CONFIG_

#include "hoe_format.h"

struct THoeInitSettings;

class Config
{
protected:
	bool fullscreen;
	uint fs_width;
	uint fs_height;

	// stats
	int max_lights;
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
	bool PostCheck();

	bool IsFullscreen();
	uint GetWidthView();
	uint GetHeightView();

	int GetMaxLights() { return max_lights; }
};


#endif // _HOE_CONFIG_
