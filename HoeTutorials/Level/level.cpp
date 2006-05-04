
#include "../Common/tutorial.h"
#include <hoe_view.h>

class LevelApp : public HoeTutorial, public XHoeKeyboard
{
	HoeGame::FreeCameraViewCtrl m_view;

public:
	LevelApp(HOE_INSTANCE instance, HoeGame::Console * con);
	bool LoadScene();
	virtual void HOEAPI _KeyDown(int);
	//virtual void HOEAPI _Paint(IHoe2D *);
};

const char * g_TutorialName = "level";

int _hoemain(HOE_INSTANCE instance, HoeGame::Console * con)
{
	LevelApp app(instance,con);

	if (!app.InitTutorial(HOETS_GRAPH))
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

LevelApp::LevelApp(HOE_INSTANCE instance, HoeGame::Console * con) : HoeTutorial(instance, con)
{

}

bool LoadBsp(IHoeSceneEnv * env, const char * fname);

bool LevelApp::LoadScene()
{
	if (!m_view.Init(GetEngine()->GetActiveScene()))
		return false;


	//p->Load("../data/test1.bsp");
	GetFS()->AddResourceFile("../data/bsp.hx");
	GetFS()->AddResourceFile("../data/test3.hx");

	IHoeSceneEnv * env = GetEngine()->GetActiveScene()->GetSceneEnv();

	// fan polygon
	/*{
	IHoeEnv::Poly * p = env->CreatePolygon(in);
	int idPol = p->GetID();
	p->SetPos(vectors);
	p->SetTex(tex);
	p->SetMaterial(0);
	p->Release();
	}*/
	LoadBsp(env, "../data/test2.bsp");

	m_view.SetPosition(0,20,0);
	//m_view.SetDistance(50);
	
	HoeGetInput(GetEngine())->RegisterKeyboard(this);
	HoeGetInput(GetEngine())->RegisterMouse(IHoeInput::MT_Foreground, &m_view);
	HoeGetRef(GetEngine())->SetBackgroundColor(0xff0000ff);

	return true;

}

void HOEAPI LevelApp::_KeyDown(int key)
{
	static float p = 10.f;

	if (!m_view.KeyDown(key))
	switch (key)
	{
	case HK_F:
		GetEngine()->exec("wireframe 0");
		break;
	case HK_G:
		GetEngine()->exec("wireframe 1");
		break;
	};
}


