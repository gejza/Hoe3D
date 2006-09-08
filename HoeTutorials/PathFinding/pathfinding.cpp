
#include "pathfinding.h"

const char * g_TutorialName = "pathfind";

int _hoemain(HOE_INSTANCE instance, HoeGame::Console * con)
{
	PathFindApp app(instance,con);

	if (!app.InitTutorial(HOETS_2D))
	{
		app.HandleError();
		return 0;
	}

	if (app.LoadScene())
		app.Run();

	app.Destroy();

	return 0;
}

/////////////////////////////////////////////////////////

PathFindApp::PathFindApp(HOE_INSTANCE instance, HoeGame::Console * con) : HoeTutorial(instance, con)
{

}

void PathFindApp::_Paint(IHoe2D *h)
{
	h->SetRect(200,150);
	h->BltFast(0,200,0,150,pic[0]);
	h->BltFast(50,85,50,70,pic[1]);
	//h->BltFast(0,200,150,300,pic[2]);
	//h->BltFast(200,400,150,300,pic[3]);
	h->SetRect(200,480);
	h->SetRect(0,0);
	f->DrawText(100,100,0xffff0000,"Hoe Tutorial Path Find!");
	h->SetRect(400,300);
	f2->DrawText(50,250,0xff00ff00,"Mixle v Pixle");

}

bool PathFindApp::LoadScene()
{
	GetFS()->AddResourceFile("../data/tex.hx");

	pic[0] = (IHoePicture *)GetEngine()->Create("picture samo");
	pic[1] = (IHoePicture *)GetEngine()->Create("picture earth");
	//pic[2] = (IHoePicture *)GetEngine()->Create("picture OLDMETAL");
	//pic[3] = (IHoePicture *)GetEngine()->Create("picture MetalGrate");
	f = (IHoeFont *)GetEngine()->Create("font '../data/font.ttf' 24");
	if (f == NULL)
		return false;
	f2 = (IHoeFont *)GetEngine()->Create("font '../data/Kidsn.ttf' 24");
	if (f2 == NULL)
		return false;

	GetEngine()->GetActiveScene()->Set2DCallback(this);

	return true;

}


