
#ifndef _HOE_CONFIG_
#define _HOE_CONFIG_

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

	static const tchar * GetHoeDesc();
	static int GetHoeVersion()
	{
		return 92;
	}
	static const tchar * GetBuild()
	{
		return T("Build time: ") T(__TIME__) T(" ") T(__DATE__);
	}
	static void GetOSDesc(tchar *);
	bool CheckTexture(dword &width,dword &height,HOEFORMAT &format);

	bool Check(THoeInitSettings *);
	bool PostCheck();

	bool IsFullscreen();
	uint GetWidthView();
	uint GetHeightView();

	int GetMaxLights() { return max_lights; }
};


#endif // _HOE_CONFIG_
