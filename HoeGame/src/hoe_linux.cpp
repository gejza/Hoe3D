
#include "StdAfx.h"
#include "../include/hoe_linux.h"

BEGIN_HOEGAME

HoeLinux::HoeLinux()
{
}

HoeLinux::~HoeLinux()
{
}

int HoeLinux::GetMsg(IHoeEngine * engine)
{
	// parse std input
    fd_set rfds;
    struct timeval tv;
    int retval;
    char str[1024];

    /* Watch stdin (fd 0) to see when it has input. */
    FD_ZERO(&rfds);
    FD_SET(0, &rfds);
    /* Wait up to five seconds. */
    tv.tv_sec = 0;
    tv.tv_usec = 0;

    retval = select(1, &rfds, NULL, NULL, &tv);
    if (retval)
    {
        /* FD_ISSET(0, &rfds) will be true. */
        gets(str);
        engine->exec(str);
    }
    
	XEvent event;
	bool repeated;
        /* handle the events in the queue */
        while (XPending(dpy) > 0)
        {
            XNextEvent(dpy, &event);
		repeated = false;
		// chytani opakovani klaves
		while (event.type == KeyRelease && XPending(dpy) ) 
		{
			XEvent peekevent;
			XPeekEvent(dpy, &peekevent);
			if ( (peekevent.type == KeyPress) &&
		     	(peekevent.xkey.keycode == event.xkey.keycode) &&
		     	((peekevent.xkey.time-event.xkey.time) < 2) )
			{
		  		XNextEvent(dpy, &event);
				repeated = true; 
			} 
			else
				break;
		}
		if (repeated)
			continue;

		if (engine == NULL || engine->XProc(&event) != true)            
		switch (event.type)
            {
                case Expose:
	                if (event.xexpose.count != 0)
	                    break;
			//hoe->Frame(0.1);
         	        break;
 
               case ClientMessage:
                    if (*XGetAtomName(dpy, event.xclient.message_type) == 
                        *"WM_PROTOCOLS")
                    {
                        printf("Exiting sanely...\n");
                        return 0;
                    }
                    break;
                default:
                    break;
            }
        }
	return 1;
}

void HoeLinux::Destroy()
{
}

void HoeLinux::SetTitle(const char * title)
{
	XStoreName(dpy,win, title);
}

//void HoeLinux::Update()
//{
//}
void HoeLinux::PaintConsole(Console *)
{
}
 
void HoeLinux::UpdateConsole()
{
}

void HoeLinux::HandleError()
{

}

//void HoeLinux::DestroyEngine()
//{ 
//	m_engine.Destroy(); 
//}

END_HOEGAME


