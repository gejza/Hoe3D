
#include "input_app.h"
#include <HoeGame/hoe_inputkeys.h>

const char * g_TutorialName = "input";

int _hoemain(HOE_INSTANCE instance, HoeGame::Console * con)
{
	InputApp app(instance,con);

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

InputApp::InputApp(HOE_INSTANCE instance, HoeGame::Console * con) : HoeTutorial(instance, con)
{
	//f = NULL;
}


void InputApp::_Paint(IHoe2D *h)
{
	h->SetRect(640,480);
	//f->DrawText(100,100,0xffff0000,msg);
	printf("%s           \r", msg);
}

bool InputApp::LoadScene()
{
	//f = (IHoeFont *)GetEngine()->Create("font '../data/font.ttf' 24");
	//if (f == NULL)
	//	return false;

	strcpy(msg,"Hoe Tutorial Input!");
	GetEngine()->GetActiveScene()->Set2DCallback(this);

	GetEngine()->GetInput()->RegisterKeyboard(this);
	GetEngine()->GetInput()->RegisterMouse(IHoeInput::MT_Background, this);
	GetEngine()->GetInfo()->LoadFont("../data/font.ttf",12);

	return true;

}

void InputApp::_KeyDown(int key)
{
	//sprintf(msg,"Key %s",/*HoeGetInput(GetEngine())->GetKeyName(key)*/);
	
	sprintf(msg,"Key %s",GetKeyDesc(key)->text/*HoeGetInput(GetEngine())->GetKeyName(key)*/);
	if (key == HK_ESCAPE)
	{
		GetEngine()->exec("quit");
	}
	if (key == HK_F12)
	{
		GetEngine()->exec("quit!");
	}
}

void InputApp::_KeyUp(int key)
{
	strcpy(msg,"Hoe Tutorial Input!");
}

void InputApp::_Wheel(long l)
{
	sprintf(msg,"Wheel %d",l);
}

void InputApp::_MouseMove(float X, float Y)
{
	sprintf(msg,"Move %f %f",X,Y);
}

void InputApp::_ButtonDown(int b)
{
	sprintf(msg,"Button %d",b);
}



