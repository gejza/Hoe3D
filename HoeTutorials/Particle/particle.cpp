
#include "particle.h"

const char * g_TutorialName = "particle";

int _hoemain(HOE_INSTANCE instance, HoeGame::Console * con)
{
	Particle app(instance,con);

	if (!app.InitTutorial(HOETS_NORMAL))
	{
		app.HandleError();
		return 0;
	}
	if (app.LoadScene())
		app.Run();

	app.Destroy();

	return 0;
}

Particle::Particle(HOE_INSTANCE instance, HoeGame::Console * con) : HoeTutorial(instance, con)
{
}
bool Particle::LoadScene()
{
	GetFS()->AddResourceFile("../data/particle.hx");

	return true;

}


