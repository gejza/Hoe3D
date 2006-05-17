
#include "../Common/tutorial.h"
#include <hoe_view.h>

class TerrainApp : public HoeTutorial, public XHoeKeyboard
{
	HoeGame::FreeCameraViewCtrl m_view;
	IHoeEnv::HeightMapSurface * terrain;
public:
	TerrainApp(HOE_INSTANCE instance, HoeGame::Console * con);
	bool LoadScene();
	virtual void HOEAPI _KeyDown(int);
	//virtual void HOEAPI _Paint(IHoe2D *);
};

const char * g_TutorialName = "terrain";

int _hoemain(HOE_INSTANCE instance, HoeGame::Console * con)
{
	TerrainApp app(instance,con);

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

TerrainApp::TerrainApp(HOE_INSTANCE instance, HoeGame::Console * con) : HoeTutorial(instance, con)
{

}

bool TerrainApp::LoadScene()
{
	if (!m_view.Init(GetEngine()->GetActiveScene()))
		return false;

	m_view.SetPosition(0,60,0);

	// vytvoreni terenu
	terrain = GetEngine()->GetActiveScene()->GetSceneEnv()->CreateHeightMapSurface();
	terrain->LoadHeight(100,100,4,NULL);

	HoeGetInput(GetEngine())->RegisterKeyboard(this);
	HoeGetInput(GetEngine())->RegisterMouse(IHoeInput::MT_Foreground, &m_view);

	HoeGetRef(GetEngine())->SetBackgroundColor(0xff0000ff);

	IHoeLight * l = GetEngine()->GetActiveScene()->CreateLight(false);
	l->SetColor(1,1,1);
	
	// 
	IHoeModel * m = (IHoeModel*)GetEngine()->Create("generate model box 5");
	XHoeObject * obj = new XHoeObject();
	GetEngine()->GetActiveScene()->RegisterObject(obj);
	obj->SetModel(m);
	obj->SetPosition(300,300,300);
	obj->Show(true);

	l->SetPosition(300,300,300);

	return true;

}

void HOEAPI TerrainApp::_KeyDown(int key)
{
	static float p = 10.f;

	if (!m_view.KeyDown(key))
	switch (key)
	{
	case HK_A:
		m_view.Rotate(-0.2f);
		break;
	case HK_D:
		m_view.Rotate(0.2f);
		break;
	case HK_Q:
		m_view.SetPosition(0,0,0);
		break;
	case HK_E:	
		m_view.SetPosition(0,120,0);
		break;
	case HK_R:
		m_view.SetPosition(0,60,0);
		break;
	case HK_F:
		GetEngine()->exec("wireframe 0");
		break;
	case HK_G:
		GetEngine()->exec("wireframe 1");
		break;
	case HK_X:		
		p -= 0.5;
		//terrain->SetLOD(p);
		break;
	case HK_Z:
		p += 0.5;
		//terrain->SetLOD(p);
		break;


	};
}


