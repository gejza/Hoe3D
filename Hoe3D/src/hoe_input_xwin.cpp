
#include "StdAfx.h"
#include "utils.h"
#include "../include/hoe3d.h"
#include "hoe_input.h"
#include "hoe_input_xwin.h"

#ifdef _LINUX

#include <X11/Xutil.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/vt.h>

#include <X11/keysym.h>
#include <X11/cursorfont.h>

#include <X11/extensions/xf86dga.h>
#include <X11/extensions/xf86vmode.h>


#define KEY_MASK (KeyPressMask | KeyReleaseMask)
#define MOUSE_MASK (ButtonPressMask | ButtonReleaseMask | \
		    PointerMotionMask | ButtonMotionMask )
#define X_MASK (KEY_MASK | MOUSE_MASK | VisibilityChangeMask | StructureNotifyMask )

//#define NO_DGA_MOUSE

// tahle funkce by mela jit nekam do ksa
static Cursor CreateNullCursor(Display *display, Window root)
{
  Pixmap cursormask; 
  XGCValues xgc;
  GC gc;
  XColor dummycolour;
  Cursor cursor;

  cursormask = XCreatePixmap(display, root, 1, 1, 1/*depth*/);
  xgc.function = GXclear;
  gc =  XCreateGC(display, cursormask, GCFunction, &xgc);
  XFillRectangle(display, cursormask, gc, 0, 0, 1, 1);
  dummycolour.pixel = 0;
  dummycolour.red = 0;
  dummycolour.flags = 04;
  cursor = XCreatePixmapCursor(display, cursormask, cursormask,
                               &dummycolour,&dummycolour, 0,0);
  XFreePixmap(display,cursormask);
  XFreeGC(display,gc);
  return cursor;
}


///////////////////////

HoeInputXWin::HoeInputXWin()
{
  	/*m_keyb = NULL;
  	m_mouse = NULL;*/
	m_diRECTMouse = false;
	m_xwinMouse = false;
  	Con_Print("XWindow Input system Created");

}

bool HoeInputXWin::Init(THoeInitSettings * his)
{
	m_disp = his->dpy;
  	m_win = his->win;
	//ShowCursor(FALSE);
	return true;
}


bool HoeInputXWin::InstallMouse(MouseType mt)
{
	if (mt == MT_Foreground)
	{
		int MajorVersion, MinorVersion;
    		XDefineCursor(m_disp, m_win, CreateNullCursor(m_disp, m_win));
 		XGrabPointer(m_disp, m_win, // bk010108 - do this earlier?
					   False,
					   MOUSE_MASK,
					   GrabModeAsync, GrabModeAsync,
					   m_win,
					   None,
					   CurrentTime);
#ifndef NO_DGA_MOUSE
		if (!XF86DGAQueryVersion(m_disp, &MajorVersion, &MinorVersion)) { 
			// unable to query, probalby not supported
			Con_Print( "Failed to detect XF86DGA Mouse" );

      
		} else {
			Con_Print("Init XF86DGA %d.%d", MajorVersion, MinorVersion);
			XF86DGADirectVideo(m_disp, DefaultScreen(m_disp), XF86DGADirectMouse);
			XWarpPointer(m_disp, None, m_win, 0, 0, 0, 0, 0, 0);
			m_diRECTMouse = true;
		}
#endif

		if (!m_diRECTMouse)
		{
		  // inviso cursor
			  XWarpPointer(m_disp, None, m_win,
					   0, 0, 0, 0,
					   600, 400);
			  XSync(m_disp, False);

			  XGetPointerControl(m_disp, &m_mouse_accel_numerator, &m_mouse_accel_denominator, &m_mouse_threshold);

			  XChangePointerControl(m_disp, True, True, 1, 1, 0);

			  XSync(m_disp, False);

			m_xwinMouse = true;
		}
		 XChangePointerControl(m_disp, True, True, 1, 1, 0);

	}
	return true;
}

void HoeInputXWin::UninstallMouse()
{
	if (m_diRECTMouse)
	{
		XF86DGADirectVideo(m_disp, DefaultScreen(m_disp), 0);
		m_diRECTMouse = false;
	}
	if (m_xwinMouse || m_diRECTMouse)
	{

	  XChangePointerControl(m_disp, true, true, m_mouse_accel_numerator,m_mouse_accel_denominator, m_mouse_threshold);
	  XUngrabPointer(m_disp, CurrentTime);
	  //XUngrabKeyboard(dpy, CurrentTime);

	  //XWarpPointer(m_disp, None, win,
		//		   0, 0, 0, 0,
		//		   glConfig.vidWidth / 2, glConfig.vidHeight / 2);

	  // inviso cursor
	  XUndefineCursor(m_disp, m_win);
		m_xwinMouse = false;
	}
}

int HoeInputXWin::XKeyEventToHKey(XKeyEvent *ev)
{
	char buf[64];
	KeySym keysym;
  	int XLookupRet;
	XLookupRet = XLookupString(ev, buf, sizeof buf, &keysym, 0);

	Con_Print("XLookupString ret: %d buf: %s keysym: %x", XLookupRet, buf, keysym);

	switch (keysym)
	{
		case XK_KP_Page_Up: 
  		case XK_KP_9:  return HK_NUMPAD9;
  		case XK_Page_Up: return HK_PRIOR;
		case XK_KP_Page_Down: 
  		case XK_KP_3: return HK_NUMPAD3;
  		case XK_Page_Down: return HK_NEXT;
		case XK_KP_Home:
		case XK_KP_7: return HK_NUMPAD7;
		case XK_Home:  return HK_HOME;
		case XK_KP_End:
		case XK_KP_1: return HK_NUMPAD1;
		case XK_End: return HK_END;
		case XK_KP_Left:
		case XK_KP_4: return HK_NUMPAD4;
		case XK_Left: return HK_LEFT;
		case XK_KP_Right:
		case XK_KP_6: return HK_NUMPAD6;
		case XK_Right: return HK_RIGHT;
		case XK_KP_Down:
		case XK_KP_2: return HK_NUMPAD2;
		case XK_Down: return HK_DOWN;
		case XK_KP_Up:   
		case XK_KP_8: return HK_NUMPAD8;
		case XK_Up: return HK_UP;
		case XK_Escape: return HK_ESCAPE;
		/*case XK_KP_Enter: *key = K_KP_ENTER;  break;*/
		case XK_Return: return HK_RETURN;
		case XK_Tab: return HK_TAB;
		case XK_F1: return HK_F1;
		case XK_F2: return HK_F2;
		case XK_F3: return HK_F3;
		case XK_F4: return HK_F4;
		case XK_F5: return HK_F5;
		case XK_F6: return HK_F6;
		case XK_F7: return HK_F7;
		case XK_F8: return HK_F8;
		case XK_F9: return HK_F9;
		case XK_F10: return HK_F10;
		case XK_F11: return HK_F11;
		case XK_F12: return HK_F12;
		case XK_BackSpace: return HK_BACK;
		case XK_KP_Delete:
		case XK_KP_Decimal: return HK_DECIMAL;
		case XK_Delete: return HK_DELETE;
		case XK_Pause:  return HK_PAUSE;
		case XK_Shift_L: return HK_LSHIFT;
		case XK_Shift_R: return HK_RSHIFT;
		case XK_Execute:
		case XK_Control_L: return HK_LCONTROL;
		case XK_Control_R: return HK_RCONTROL;
		case XK_Alt_L: return HK_LMENU;
		//case XK_Meta_L: 
		case XK_Alt_R: return HK_RMENU;  
  		//case XK_Meta_R: *key = K_ALT;     break;
/*
  case XK_KP_Begin: *key = K_KP_5;  break;

  case XK_Insert:   *key = K_INS; break;
  case XK_KP_Insert:
  case XK_KP_0: *key = K_KP_INS; break;

  case XK_KP_Multiply: *key = '*'; break;
  case XK_KP_Add:  *key = K_KP_PLUS; break;
  case XK_KP_Subtract: *key = K_KP_MINUS; break;
  case XK_KP_Divide: *key = K_KP_SLASH; break;

    // bk001130 - from cvs1.17 (mkv)
  case XK_exclam: *key = '1'; break;
  case XK_at: *key = '2'; break;
  case XK_numbersign: *key = '3'; break;
  case XK_dollar: *key = '4'; break;
  case XK_percent: *key = '5'; break;
  case XK_asciicircum: *key = '6'; break;
  case XK_ampersand: *key = '7'; break;
  case XK_asterisk: *key = '8'; break;
  case XK_parenleft: *key = '9'; break;
  case XK_parenright: *key = '0'; break;
  */
  // weird french keyboards ..
  // NOTE: console toggle is hardcoded in cl_keys.c, can't be unbound
  //   cleaner would be .. using hardware key codes instead of the key syms
  //   could also add a new K_KP_CONSOLE
  		case XK_twosuperior: return HK_GRAVE;
		case XK_space: return HK_SPACE;
		//case XK_KP_Space: *key = K_SPACE; break;
		default:
	 		if (XLookupRet == 0)
    			{
     	 			Con_Print( "Warning: XLookupString failed on KeySym %d", keysym);
      				return 0;
    			}
   	 		else
    			{
      			// XK_* tests failed, but XLookupString got a buffer, so let's try it
    		 	 unsigned char key = *(unsigned char *)buf;
    	 		 if (key >= 'A' && key <= 'Z')
   	  	   		key = key - 'A' + 'a';
   	 	  // if ctrl is pressed, the keys are not between 'A' and 'Z', for instance ctrl-z == 26 ^Z ^C etc.
   	 	  // see https://zerowing.idsoftware.com/bugzilla/show_bug.cgi?id=19
  			   	 else if (key >= 1 && key <= 26)
   	  	  		key = key + 'a' - 1;
    			switch (key)
			{
			case 'a': return HK_A;
			case 'b': return HK_B;
			case 'c': return HK_C;
			case 'd': return HK_D;
			case 's': return HK_S;
			case 'w': return HK_W;
			default: 
				Con_Print("Warning: input char %c failed.", key);return 0;
			}
			}
	}
}

bool HoeInputXWin::XProc(XEvent * event)
{
	assert(event);
	int key;
	bool dowarp = false;
	switch (event->type)
	{
	case KeyPress:
		key = XKeyEventToHKey(&event->xkey);
		if (key && m_keyb)
			m_keyb->_KeyDown(key);
		break;
	case KeyRelease:
		key = XKeyEventToHKey(&event->xkey);
		if (key && m_keyb)
			m_keyb->_KeyUp(key);
		break;
	case ButtonPress:
        if (m_mouse)
        {
            switch (event->xkey.keycode)
            {
                case 1:
                    m_mouse->_ButtonDown(HMB_LEFT);
                    break;
                case 2:
                    m_mouse->_ButtonDown(HMB_MIDDLE);
                    break;
                case 3:
                    m_mouse->_ButtonDown(HMB_RIGHT);
                    break;
      
            };
        }
        break;
	case ButtonRelease:
		//Con_Print("tlacitko nahore %d", event->xkey.keycode);
		if (m_mouse)
        {
            switch (event->xkey.keycode)
            {
                case 1:
                    m_mouse->_ButtonUp(HMB_LEFT);
                    break;
                case 2:
                    m_mouse->_ButtonUp(HMB_MIDDLE);
                    break;
                case 3:
                    m_mouse->_ButtonUp(HMB_RIGHT);
                    break;
             };
        }
        break;
	case MotionNotify:
		if (m_mouse)
		{
			int mx=0,my=0;
			if (m_absolutMouse)
			{
				mx = event->xmotion.x; my = event->xmotion.y;
				MoveAbsolut(mx,my);
				break;
			}
			else if (m_diRECTMouse)
			{
				mx = event->xmotion.x_root; my = event->xmotion.y_root;
			}
			else if (m_xwinMouse)
			{
				mx = event->xmotion.x-600; my = event->xmotion.y - 400;
				if (mx != 0 || my != 0)
					dowarp = true;
			}
			if (mx != 0 || my != 0)
				m_mouse->_MouseMove(mx,my);
			//Con_Print("%d %d %d %d", event->xmotion.x,event->xmotion.y,event->xmotion.x_root,event->xmotion.y_root);
		}
		break;
	default:
		Con_Print("stisknuto %d", event->type);
		break;
	}
	
	if (dowarp)
	{
		XWarpPointer(m_disp,None,m_win,0,0,0,0,600,400);
	}

	return false;
}

void HoeInputXWin::Process(float time)
{
}

#endif

