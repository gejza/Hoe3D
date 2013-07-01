
#include "paint2d_app.h"
#include <HoeGame/hoe_lang.h>

const char * g_TutorialName = "paint2d";

//HoeGame::Lang g_lang;

int _hoemain(HOE_INSTANCE instance, HoeGame::Console * con)
{
	Paint2DApp app(instance,con);

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

/////////////////////////////////////////////////////////

Paint2DApp::Paint2DApp(HOE_INSTANCE instance, HoeGame::Console * con) : HoeTutorial(instance, con)
{

}

void Paint2DApp::_Paint(IHoe2D *h)
{
	
	h->SetRect(200,150);
	/*h->Blt(pic[0],0,200,0,150);
	h->Blt(pic[1],50,85,50,70);
	//h->BltFast(0,200,150,300,pic[2]);
	//h->BltFast(200,400,150,300,pic[3]);
	h->SetRect(200,480);
	h->SetRect(0,0);
	//f->DrawText(100,100,0xffff0000,"Hoe Tutorial Paint 2D!");
	f->DrawText(100,100,0xffff0000,g_lang.Get(2));
	h->SetRect(400,300);
	*/
	THoeRect dest;
	dest.left = 10;
	dest.right = 180;
	dest.top = 10;
	dest.bottom = 130;
	h->PaintRect(&dest, 0xff56ff35, true);
	dest.left = 100;
	h->PaintRect(&dest, 0xffff0000, true);
	f2->DrawText(50,250,0xff00ff00,"Mixle v Pixle");

}

bool Paint2DApp::LoadScene()
{
	//g_lang.Load("bin/czech.lng");
	//GetEngine()->exec(g_lang.Get(1));
	//HoeGame::BaseConsole::Printf("test: %s",g_lang.Get(2));
	
	/*GetFS()->AddResourceFile("data/tex.hx");

	pic[0] = (IHoePicture *)GetEngine()->Create("picture samo");
	pic[1] = (IHoePicture *)GetEngine()->Create("picture earth");
	//pic[2] = (IHoePicture *)GetEngine()->Create("picture OLDMETAL");
	//pic[3] = (IHoePicture *)GetEngine()->Create("picture MetalGrate");
	*/
	f = (IHoeFont *)GetEngine()->Create("font 'data/font.ttf' 24");
	if (f == NULL)
		return false;
	f2 = (IHoeFont *)GetEngine()->Create("font 'data/Kidsn.ttf' 24");
	if (f2 == NULL)
		return false;

	GetEngine()->GetActiveScene()->Set2DCallback(this);

	return true;

}


