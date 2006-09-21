
#include "model_app.h"

const char * g_TutorialName = "models";

const int n_light = 2;

int _hoemain(HOE_INSTANCE instance, HoeGame::Console * con)
{
	ModelApp app(instance,con);

	if (!app.InitTutorial(HOETS_NORMAL))
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

ModelApp::ModelApp(HOE_INSTANCE instance, HoeGame::Console * con) : HoeTutorial(instance, con)
{
}

bool ModelApp::LoadScene()
{

	GetFS()->AddResourceFile("../data/jackolan.hm");
	GetFS()->AddResourceFile("../data/angel_anim.hm");
	GetFS()->AddResourceFile("../data/bump.hx");
	//GetEngine()->AddResource("../data/konvice.hm");
	//HoeGetRef(GetEngine())->SetBackgroundColor(0xfff00a79);
	HoeGetRef(GetEngine())->SetBackgroundColor(0xff000000);

	if (!m_view.Init(GetEngine()->GetActiveScene()))
		return false;

	m_view.SetTargetPosition(0,0,0);
	m_view.SetAngle(0.1f);
	m_view.SetArcAngle(-0.16f);
	m_view.SetDistance(150);
//	HoeGetInput(GetEngine())->RegisterKeyboard(&m_view);
	HoeGetInput(GetEngine())->RegisterMouse(IHoeInput::MT_Foreground, &m_view);

	IHoeModel * lght = (IHoeModel*)GetEngine()->Create("generate model box 5");
	if (!lght)
		return false;

	for (int i=0;i < n_light;i++)
	{
		m_l[i].light = GetEngine()->GetActiveScene()->CreateLight(false);
		GetEngine()->GetActiveScene()->RegisterObject(&m_l[i]);
		m_l[i].SetModel(lght);
		m_l[i].SetPosition(0.f,0.f,0.f);
		m_l[i].Show(true);
	}
	if (0 < n_light) m_l[ 0].light->SetColor( 1.0f, 1.0f, 1.0f);
	//if (0 < n_light) m_l[ 0].light->SetColor( 1.0f, 0.0f, 0.0f);
	if (1 < n_light) m_l[ 1].light->SetColor( 1.0f, 1.0f, 1.0f);
	//if (1 < n_light) m_l[ 1].light->SetColor( 0.0f, 1.0f, 0.0f);
	if (2 < n_light) m_l[ 2].light->SetColor( 0.0f, 0.0f, 1.0f);
	if (3 < n_light) m_l[ 3].light->SetColor( 0.5f, 0.5f, 0.0f);
	if (4 < n_light) m_l[ 4].light->SetColor( 0.5f, 0.0f, 0.5f);
	if (5 < n_light) m_l[ 5].light->SetColor( 0.0f, 0.5f, 0.5f);
	if (6 < n_light) m_l[ 6].light->SetColor( 0.3f, 0.3f, 0.3f);
	if (7 < n_light) m_l[ 7].light->SetColor( 0.6f, 0.4f, 0.0f);
	if (8 < n_light) m_l[ 8].light->SetColor( 0.6f, 0.0f, 0.4f);
	if (9 < n_light) m_l[ 9].light->SetColor( 0.4f, 0.6f, 0.0f);
	if (10 < n_light) m_l[10].light->SetColor( 0.0f, 0.6f, 0.4f);
	if (11 < n_light) m_l[11].light->SetColor( 0.4f, 0.0f, 0.6f);
	if (12 < n_light) m_l[12].light->SetColor( 0.0f, 0.4f, 0.6f);
	if (13 < n_light) m_l[13].light->SetColor( 0.6f, 0.2f, 0.2f);
	if (14 < n_light) m_l[14].light->SetColor( 0.2f, 0.6f, 0.2f);
	if (15 < n_light) m_l[15].light->SetColor( 0.2f, 0.2f, 0.6f);
	if (16 < n_light) m_l[16].light->SetColor( 0.8f, 0.1f, 0.1f);

	//IHoeModel * mod1 = (IHoeModel*)GetEngine()->Create("model jackolan");
	/*IHoeModel * mod1 = (IHoeModel*)GetEngine()->Create("generate model box file:'box.txt' -dump");
	if (!mod1)
		return false;*/
	//IHoeModel * mod2 = (IHoeModel*)GetEngine()->Create("model angel file:'angel.txt' -dump");
	IHoeModel * mod2 = (IHoeModel*)GetEngine()->Create("model angel");
	//IHoeModel * mod2 = (IHoeModel*)GetEngine()->Create("generate model box 40");
	if (!mod2)
		return false;

	const float leng = 10.f;
	const float st = 1.f;

	/*for (float a=0.f;a < (6.28f - st);a += st)
	{
		CreateObj(mod, sinf(a) * leng, cosf(a) * leng);
	}*/
	m_model = CreateObj(mod2, 0, 0);
	/*CreateObj(mod1, 30, -10);
	CreateObj(mod1, -30, -10);
	CreateObj(mod1, 0, -10);
	CreateObj(mod1, -15, -10);
	CreateObj(mod1, 15, -10);
	CreateObj(mod1, -45, -10);
	CreateObj(mod1, 45, -10);*/

	//GetEngine()->exec("hidefps");

	return true;

}
#ifndef PI180
#define PI180	0.0174532925199432957692369076848861f	// pi / 180
#endif
struct Vec
{
	float x,y,z;
	void RotY( const float angle)
	{
		float s = (float) sin( angle * PI180);
		float c = (float) cos( angle * PI180);
		float X=x;
		x =  x*c + z*s;
		z = -X*s + z*c;
	}
	void RotZ( const float angle)
	{
		float s = (float) sin( angle * PI180);
		float c = (float) cos( angle * PI180);
		float X=x;
		x =  x*c - y*s;
		y =  X*s + y*c;
	} 
};
void ModelApp::OnUpdate(float timeframe)
{
	static float rotz = 0;
	static float roty = 0;
	const float speed = .3f;
	rotz+=90.f*timeframe*speed;
	roty+=10.f*timeframe*speed;
	const float l = 50.f;
	Vec a;
	for(int i=0; i<n_light; i++)
	{
		a.x = 2.0f; a.y = 0; a.z = 0;
		if(i<8)
		{
			a.RotZ(rotz+(float)i*360.f/(float)8.f);
			a.y *= 0.5f;
			a.RotZ(45.f);
			a.RotY(roty);
		}
		else
		{
			a.RotZ(rotz+(float)i*360.f/(float)9.f+45.f);
			a.y *= 0.5f;
			a.RotZ(-45.f);
			a.RotY(roty+90.f);
		}
		m_l[i].light->SetPosition(a.x*l, a.y*l, a.z*l);
		m_l[i].SetPosition(a.x*l, a.y*l, a.z*l);
	}

	/*for(int i=0; i<n_light; i++)
	{
		Vec a;
		a.x = 2.0f; a.y = 0; a.z = 0;
		if(i<8)
		{
			a.RotZ(rotz+(float)i*360.f/(float)4.f);
			a.y *= 0.5;
			a.RotZ(45.f);
			a.RotY(roty);
		}
		else
		{
			a.RotZ(rotz+(float)i*360.f/(float)4.f+45.f);
			a.y *= 0.5f;
			a.RotZ(-45.f);
			a.RotY(roty+90.f);
		}
		m_l[i].light->SetPosition(a.x*l, a.y*l, a.z*l);
		m_l[i].SetPosition(a.x*l, a.y*l, a.z*l);
	}*/

	static float at = 0.f;
	if (at > 1.f)
		at -= 1.f;
	m_model->SetAnimationTime(at);
	at += timeframe * 2.f;
}

Model * ModelApp::CreateObj(IHoeModel *m, float x, float y)
{
	Model * obj = new Model;
	GetEngine()->GetActiveScene()->RegisterObject(obj);
	obj->SetModel(m);
	//obj->SetOrientation(0,1,0,0.3f);
	obj->SetPosition(x,0.f,y);
	obj->Show(true);
	return obj;
}
