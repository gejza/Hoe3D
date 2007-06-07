
#include "StdAfx.h"
#include "../include/hoe_macosx.h"

BEGIN_HOEGAME

HoeMacOsX::HoeMacOsX()
{
}

HoeMacOsX::~HoeMacOsX()
{
}

int HoeMacOsX::GetMsg(IHoe3DEngine * engine)
{
	EventRef theEvent;
	EventTargetRef theTarget;
	theTarget = GetEventDispatcherTarget();
    while  (ReceiveNextEvent(0, NULL,kEventDurationForever,true,
            &theEvent)== noErr)
        {
            SendEventToEventTarget (theEvent, theTarget);
            ReleaseEvent(theEvent);
        }	
	return 1;
}

void HoeMacOsX::Destroy()
{
}

void HoeMacOsX::SetTitle(const char * title)
{
	//XStoreName(dpy,win, title);
}

//void HoeMacOsX::Update()
//{
//}
void HoeMacOsX::PaintConsole(Console *)
{
}
 
void HoeMacOsX::UpdateConsole()
{
}

void HoeMacOsX::HandleError()
{

}

//void HoeMacOsX::DestroyEngine()
//{ 
//	m_engine.Destroy(); 
//}

END_HOEGAME


