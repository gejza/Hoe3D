
#include "tutorial.h"

using namespace HoeGame;

#pragma comment (lib,"libfl.a")
#pragma comment (lib,"Lua5.1.lib")

HoeTutorial::HoeTutorial(HOE_INSTANCE instance, HoeGame::Console * con) : HoeApp(instance, con)
{
}

bool HoeTutorial::InitTutorial(HOE_TYPE_SCENE ts)
{
	if (!Init())
		return false;

	GetEngine()->SetActiveScene(GetEngine()->CreateScene(ts));
	HoeGetInfo(GetEngine())->LoadFont("../data/font.ttf",12);

	return true;
}

int _hoemain(HOE_INSTANCE instance, HoeGame::Console * con);

static const char * GetLogname()
{
	static char logname[512];
	sprintf(logname,"%s.log",g_TutorialName);
	return logname;
}

#ifdef _WIN32
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE,LPSTR lpStr,int iCmdShow)
{
	SetRootFromInstance(hInstance);

	Console con;
	con.SetFileLogging(GetLogname());
	ConfigVars config;
	config.Load("tutorials.conf");

	return _hoemain(hInstance,&con);
}

#endif
#ifdef _LINUX

int main(int argc,char * argv[])
{
	SetRootFromExe(argv[0]);

	Console con;
        con.SetFileLogging(GetLogname());
	ConfigVars config;
	config.Load("tutorials.conf");

	return _hoemain(NULL,&con);
}

#endif


