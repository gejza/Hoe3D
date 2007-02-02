
#ifndef _STDAFX_H_
#define _STDAFX_H_

//#define _CRT_SECURE_NO_DEPRECATE
#ifdef _WIN32
//#define _WIN32_WINNT 0x0500
#include <windows.h>
#define snprintf _snprintf
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <memory.h>

#ifdef _LINUX
#include <X11/Xlib.h>
#include <X11/Xutil.h>
//#include <X11/extensions/xf86vmode.h>
#include <X11/keysym.h>
#include <stdarg.h>
#include <string.h>
#endif


#include "wx/wxprec.h"

#include "wx/app.h"
#include "wx/grid.h"
#include "wx/treectrl.h"
#include "wx/spinctrl.h"
#include "wx/artprov.h"
#include "wx/clipbrd.h"
#include "wx/image.h"
#include "wx/colordlg.h"
#include "wx/wxhtml.h"
#include "wx/imaglist.h"
#include "wx/dataobj.h"
#include "wx/dcclient.h"
#include "wx/bmpbuttn.h"
#include "wx/menu.h"
#include "wx/toolbar.h"
#include "wx/statusbr.h"
#include "wx/msgdlg.h"
#include "wx/textdlg.h"
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/manager.h>

#include "wx/aui/aui.h"

#endif // _STDAFX_H_




