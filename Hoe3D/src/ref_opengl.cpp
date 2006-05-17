
#include "system.h"
#include "shared.h"
#include "utils.h"
#include "config.h"
#include "../include/hoe3d.h"
#include "ref_opengl.h"
#include "hoe_stream.h"
#include "hoe_index.h"
#include "hoe_info.h"


#pragma comment (lib,"opengl32.lib")
#pragma comment (lib,"glu32.lib")


RefOpenGL::RefOpenGL()
{
	Con_Print("------ OpenGL  ------");
}

bool RefOpenGL::Init(THoeInitSettings * his)
{
	m_Fullscreen = GetConfig()->IsFullscreen();
	m_Width = GetConfig()->GetWidthView();
	m_Height = GetConfig()->GetHeightView();
#ifdef _WIN32
	//
	m_hWnd = his->win;
	GLuint PixelFormat;
	dword dwExStyle;
	dword dwStyle;			
	RECT WindowRect;

	if (!his->forcewnd)
	{

		if (m_Fullscreen)
		{
			long res;
			DEVMODE dmScreenSettings;								
			memset(&dmScreenSettings,0,sizeof(dmScreenSettings));
			dmScreenSettings.dmSize=sizeof(dmScreenSettings);
			dmScreenSettings.dmPelsWidth	= m_Width;
			dmScreenSettings.dmPelsHeight	= m_Height;	
			dmScreenSettings.dmBitsPerPel	= 16;
			dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

			if ((res = ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN))!=DISP_CHANGE_SUCCESSFUL)
			{
				Con_Print("Error: The Requested Fullscreen Mode (%dx%dx%d) Is Not Supported by Your Video Card. (%d)",
					dmScreenSettings.dmPelsWidth,dmScreenSettings.dmPelsHeight,dmScreenSettings.dmBitsPerPel,res);
				return false;	
			}

			dwExStyle=WS_EX_APPWINDOW;	
			dwStyle=WS_POPUP;
			ShowCursor(FALSE);
		}
		else
		{
			dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;	
			dwStyle=WS_OVERLAPPEDWINDOW;
		}

		AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);	

		if (SetWindowLong(m_hWnd,GWL_EXSTYLE,dwExStyle) == 0)
		{
			Con_Print("error init: %d",GetLastError());
			return false;
		}
		if (SetWindowLong(his->win,GWL_STYLE,dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN) == 0)
		{
			Con_Print("error init: %d",GetLastError());
			return false;
		}
		if (m_Fullscreen)
			SetWindowPos(m_hWnd,HWND_TOP,0,0,m_Width,m_Height,0);
	}

	static PIXELFORMATDESCRIPTOR pfd=
{
		sizeof(PIXELFORMATDESCRIPTOR),// Velikost struktury
		1,// Èíslo verze
		PFD_DRAW_TO_WINDOW |// Podpora okna
		PFD_SUPPORT_OPENGL |// Podpora OpenGL
		PFD_DOUBLEBUFFER,// Podpora Double Bufferingu
		PFD_TYPE_RGBA,// RGBA Format
		16,// Zvolí barevnou hloubku
		0, 0, 0, 0, 0, 0,// Bity barev ignorovány
		0,// Žádný alpha buffer
		0,// Ignorován Shift bit
		0,// Žádný akumulaèní buffer
		0, 0, 0, 0,// Akumulaèní bity ignorovány
		24,// 24-bitový hloubkový buffer (Z-Buffer)
		0,// Žádný Stencil Buffer
		0,// Žádný Auxiliary Buffer
		PFD_MAIN_PLANE,// Hlavní vykreslovací vrstva
		0,// Rezervováno
		0, 0, 0// Maska vrstvy ignorována
	};

	if (!(m_hDC=::GetDC(m_hWnd)))
	{
		Con_Print("Error: Can't Create A GL Device Context.");
		return false;
	}

	if (!(PixelFormat=ChoosePixelFormat(m_hDC,&pfd)))
	{
		Con_Print("Error: Can't Find A Suitable PixelFormat.");
		return false;
	}

	if(!SetPixelFormat(m_hDC,PixelFormat,&pfd))
	{
		Con_Print("Error: Can't Set The PixelFormat.");
		return false;
	}

	if (!(m_hRC=wglCreateContext(m_hDC)))
	{
		Con_Print("Error: Can't Create A GL Rendering Context.");
		return false;
	}

	if(!wglMakeCurrent(m_hDC,m_hRC))
	{
		Con_Print("Error: Can't Activate The GL Rendering Context.");
		return false;
	}

	ShowWindow(m_hWnd,SW_SHOW);	
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);

#endif // _WIN32
#ifdef _LINUX

    XVisualInfo *vi;
    Colormap cmap;
    int dpyWidth, dpyHeight;
    int i;
    int glxMajorVersion, glxMinorVersion;
    int vidModeMajorVersion, vidModeMinorVersion;
    XF86VidModeModeInfo **modes;
    int modeNum;
    int bestMode;
    Atom wmDelete;
    Window winDummy;
    unsigned int borderDummy;
	XSetWindowAttributes attr;

    int attrListDbl[] = { GLX_RGBA,
              GLX_RED_SIZE, 1,
                GLX_GREEN_SIZE, 1,
                GLX_BLUE_SIZE, 1,
                GLX_DEPTH_SIZE, his->depth,
		GLX_DOUBLEBUFFER,
                None };
    


    /* set best mode to current */
    bestMode = 0;
    /* get a connection */
    XF86VidModeQueryVersion(his->dpy, &vidModeMajorVersion,
        &vidModeMinorVersion);
	Con_Print("XF86VidModeExtension-Version %d.%d", vidModeMajorVersion,
        vidModeMinorVersion);

	if (!his->forcewnd)
	{ 

		XF86VidModeGetAllModeLines(his->dpy, his->screen, &modeNum, &modes);
		/* save desktop-resolution before switching modes */
		//GLWin.deskMode = *modes[0];
		/* look for mode with requested resolution */
		for (i = 0; i < modeNum; i++)
		{
			if ((modes[i]->hdisplay == m_Width) && (modes[i]->vdisplay == m_Height))
			{
				bestMode = i;
			}
		}

	}
    /* get an appropriate visual */
    vi = glXChooseVisual(his->dpy, his->screen, attrListDbl);
	if (vi == NULL)
    {
        Con_Print("Error with get double buffered visual.");
		return false;
    }
    glXQueryVersion( his->dpy, &glxMajorVersion, &glxMinorVersion);
    Con_Print("glX-Version %d.%d", glxMajorVersion, glxMinorVersion);
    /* create a GLX context */
	
	m_Ctx = glXCreateContext(his->dpy, vi, None, GL_TRUE);
	if (m_Ctx == NULL)
	{
	        Con_Print("Error create context.");
                return false;
	}
		
	if (!his->forcewnd)
	{ 
		/* create a color map */
		cmap = XCreateColormap(his->dpy, RootWindow(his->dpy, vi->screen),
			vi->visual, AllocNone);
		attr.colormap = cmap;
		attr.border_pixel = 0;		

		if (m_Fullscreen)
		{
			XF86VidModeSwitchToMode(his->dpy, his->screen, modes[bestMode]);
			XF86VidModeSetViewPort(his->dpy, his->screen, 0, 0);
			dpyWidth = modes[bestMode]->hdisplay;
			dpyHeight = modes[bestMode]->vdisplay;
			Con_Print("Resolution %dx%d\n", dpyWidth, dpyHeight);
			XFree(modes);
	    
			/* create a fullscreen window */
			attr.override_redirect = True;
			attr.event_mask = ExposureMask | KeyPressMask | ButtonPressMask |
				StructureNotifyMask;
			his->win = XCreateWindow(his->dpy, RootWindow(his->dpy, vi->screen),
				0, 0, dpyWidth, dpyHeight, 0, vi->depth, InputOutput, vi->visual,
				CWBorderPixel | CWColormap | CWEventMask | CWOverrideRedirect,
				&attr);
			/*XWarpPointer(his->dpy, None, his->win, 0, 0, 0, 0, 0, 0);
			XMapRaised(his->dpy, his->win);
			XGrabKeyboard(his->dpy, his->win, True, GrabModeAsync,
				GrabModeAsync, CurrentTime);
			XGrabPointer(his->dpy, his->win, True, ButtonPressMask,
				GrabModeAsync, GrabModeAsync, his->win, None, CurrentTime);*/
		}
		else
		{
			/* create a window in window mode*/
			attr.event_mask = ExposureMask | KeyPressMask | ButtonPressMask | KeyReleaseMask | ButtonReleaseMask | PointerMotionMask | StructureNotifyMask;
			his->win = XCreateWindow(his->dpy, RootWindow(his->dpy, vi->screen),
				0, 0, m_Width, m_Height, 0, vi->depth, InputOutput, vi->visual,
				CWBorderPixel | CWColormap | CWEventMask, &attr);
			/* only set window title and handle wm_delete_events if in windowed mode */
			XMapRaised(his->dpy, his->win);
		}    
	} // konci force wnd
    /* connect the glx-context to the window */
    if (!glXMakeCurrent(his->dpy, his->win, m_Ctx))
	{
		Con_Print("Error connect context to window");
		return false;
	}
	
	int x,y;
    //XGetGeometry(m_Dpy, his->win, &winDummy, &x, &y,
    //    &his->width, &his->height, &borderDummy, &his->depth);
    
    m_Dpy = his->dpy;
    m_Win = his->win;

#endif // _LINUX

	Con_Print("%s",(char *)glGetString(GL_VENDOR));// Výpis výrobce
	Con_Print("%s",(char *)glGetString(GL_RENDERER));// Výpis typu grafické karty
	Con_Print("GL ver. %s",(char *)glGetString(GL_VERSION));// Výpis verze

	glViewport(0,0,m_Width,m_Height);// Resetuje aktuální nastavení
	glMatrixMode(GL_PROJECTION);// Zvolí projekèní matici
	glLoadIdentity();// Reset matice
	glMatrixMode(GL_MODELVIEW);// Zvolí matici Modelview
	glLoadIdentity();// Reset matice

	glDisable(GL_TEXTURE_2D);// Zapne mapování textur
	glShadeModel(GL_SMOOTH);// Jemné stínování
	SetBackgroundColor(m_BackColor);
	glClearDepth(1.0f);// Nastavení hloubkového bufferu
	glEnable(GL_DEPTH_TEST);// Zapne hloubkové testování
	glDepthFunc(GL_LESS);// Typ hloubkového testování
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);// Nejlepší perspektivní korekce

	// extensions 
	GLExt::Extension::PrintGlExt();
	ext.comp.Check();
	ext.vb.Check();
	ext.vs.Check();
	//glPixelStorei(GL_UNPACK_SWAP_BYTES,GL_FALSE);

	return true;
}


bool RefOpenGL::Begin()
{
	return true;
}

void RefOpenGL::End()
{
	// Prohození bufferù (Double Buffering)
#ifdef _LINUX
	glXSwapBuffers(m_Dpy, m_Win);

#elif defined (_WIN32)
	SwapBuffers(m_hDC);
#endif
}

void RefOpenGL::ClearBuffers(bool target, bool depth)
{
	glClear((target?GL_COLOR_BUFFER_BIT:0) | (depth?GL_DEPTH_BUFFER_BIT:0));
}

void RefOpenGL::SetBackgroundColor(unsigned long color)
{
	glClearColor((color >> 16 & 0xff) / 255.f,
		(color >> 8 & 0xff) / 255.f,
		(color >> 0 & 0xff) / 255.f,
		0.f);
}

void RefOpenGL::Destroy()
{
#ifdef _LINUX
	if (m_Ctx)
    {
        if (!glXMakeCurrent(m_Dpy, None, NULL))
        {
			Con_Print("Could not release drawing context.");   
        }
        glXDestroyContext(m_Dpy, m_Ctx);
        m_Ctx = NULL;
    }
#endif // _LINUX

#ifdef _WIN32
	if (m_hRC)
	{
        if (!wglMakeCurrent(m_hDC,NULL))
		{
			Con_Print("Error: Release Of DC And RC Failed.");
        }
		if (!wglDeleteContext(m_hRC))
		{
			Con_Print("Error: Release Rendering Context Failed.");
		}
		m_hRC=NULL;
	}
	if (m_hDC && !ReleaseDC(m_hWnd,m_hDC))
	{
		DWORD err = GetLastError();
		Con_Print("Error: Release Device Context Failed. error=%d",err);
	}
    m_hDC=NULL;
	
#endif // _WIN32
	Con_Print("Ref destroyed.");

}

void RefOpenGL::DrawStdObject(HoeStream * stream, HoeIndex * index)
{
	stream->Set(0);
	glDrawElements(GL_TRIANGLES,index->GetNumIndices(),GL_UNSIGNED_SHORT,(GLvoid*)index->GetIndexBuffer());	
	GetInfo()->AddStatTriangles(index->GetNumIndices()/3);
}

void RefOpenGL::DrawStdObject(HoeStream * stream, HoeIndex * index, dword vert, dword ind)
{
	stream->Set(0);
	glDrawElements(GL_TRIANGLES,ind,GL_UNSIGNED_SHORT,(GLvoid*)index->GetIndexBuffer());	
	GetInfo()->AddStatTriangles(ind/3);
}

void RefOpenGL::DrawIndex(HoeIndex * index, dword offset, dword count)
{
	byte * ui = reinterpret_cast<byte*>(index->GetIndexBuffer());
	ui += offset * 2;
	glDrawElements(GL_TRIANGLES,count,GL_UNSIGNED_SHORT,(GLvoid*)ui);	
	GetInfo()->AddStatTriangles(count/3);
}

void RefOpenGL::DrawFanObject(dword offset, dword count)
{
	glDrawArrays(GL_TRIANGLE_FAN, offset, count);
	GetInfo()->AddStatTriangles(count-2);
}

void RefOpenGL::DrawPointObject(class HoeStream * stream, int vertCount)
{
	stream->Set(0);
	glDrawArrays(GL_POINTS, 0, vertCount);
}

void RefOpenGL::DrawLineObject(class HoeStream * stream, int lineCount)
{
	stream->Set(0);
	glDrawArrays(GL_LINES, 0, lineCount);

}








