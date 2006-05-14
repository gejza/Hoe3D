
#include "first_app.h"


FirstApp::FirstApp(HOE_INSTANCE instance, HoeGame::Console * con) : HoeTutorial(instance, con)
{
}

const char * g_TutorialName = "first";

int _hoemain(HOE_INSTANCE instance, HoeGame::Console * con)
{
	FirstApp app(instance,con);

	if (!app.InitTutorial(HOETS_NORMAL))
	{
		app.HandleError();
		return 0;
	}

	app.Run();

	app.Destroy();

	return 0;
}


