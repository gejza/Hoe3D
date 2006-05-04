
#include "StdAfx.h"
#include "sound_app.h"

const char * g_TutorialName = "sound";

int _hoemain(HOE_INSTANCE instance, HoeGame::Console * con)
{
	SoundApp app(instance,con);

	if (!app.InitTutorial(HOETS_2D))
	{
		app.HandleError();
		return 0;
	}

	app.LoadScene();

	app.Run();

	app.Destroy();

	return 0;
}

/////////////////////////////////////////////////////////////////

SoundApp::SoundApp(HOE_INSTANCE instance, HoeGame::Console * con) : HoeTutorial(instance, con)
{

}

bool SoundApp::LoadScene()
{
	IHoeSound * s = (IHoeSound *)GetEngine()->Create("sound '../data/kuk.WAV'");
	/*m_fs.AddResourceFile("../data/1.hx");

	pic[1] = (IHoePicture *)GetEngine()->Create("picture Sample");
	//pic[2] = (IHoePicture *)GetEngine()->Create("picture OLDMETAL");
	//pic[3] = (IHoePicture *)GetEngine()->Create("picture MetalGrate");
	f = (IHoeFont *)GetEngine()->Create("font '../data/font.ttf' 24");
	if (f == NULL)
		return false;

	GetEngine()->GetActiveScene()->Set2DCallback(this);
*/
	s->Play();
	return true;

}





