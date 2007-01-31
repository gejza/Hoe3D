
#include "console_app.h"

const char * g_TutorialName = "console";

int _hoemain(HOE_INSTANCE instance, HoeGame::Console * con)
{
	ConApp app(instance,con);

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

ConApp::ConApp(HOE_INSTANCE instance, HoeGame::Console * con)
 : HoeTutorial(instance, con)/*, gcon(con)*/ , scene(&gcon)
{

}

bool ConApp::LoadScene()
{
	m_fs.AddResourceFile("../data/1.hx");
	HoeGetInfo(GetEngine())->LoadFont("../data/font.ttf",12);

	if (!gcon.Load(GetEngine()))
		return false;

	if (!scene.Load(GetEngine()))
		return false;

	//scene.Show(GetEngine()->GetActiveScene());
	
	HoeGetInput(GetEngine())->RegisterKeyboard(&scene);

	return true;

}

void ConApp::OnUpdate(float dtime)
{
	if (gcon.IsActive())
		gcon.Update(dtime);
}


Scene::Scene(HoeGame::GuiConsole * c)
{
	con = c;
	font = NULL;
}

void Scene::OnPaint()
{
	font->DrawText(100,100,0xff00ff00,"Press ~ to eject console.");
	//if (con && con->IsActive())
	//	con->Draw(Get2D());
}

bool Scene::Load(IHoe3DEngine * eng)
{
	font = (IHoeFont *)eng->Create("font '../data/font.ttf' 24");
	if (!font)
		return false;

	return true;
}

void Scene::_KeyDown(int k)
{

	switch (k)
	{
	case HK_GRAVE:
        con->Open();
		break;
	case HK_ESCAPE:
		//this->Get3D()->exec("quit");
		break;
	}
}



