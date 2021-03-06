
#ifndef _STDAFX_H_
#define _STDAFX_H_

#define _CRT_SECURE_NO_DEPRECATE
#ifdef _WIN32
#pragma warning (disable: 4996 4267)
#define _WIN32_WINNT 0x0500
#include <windows.h>
#endif

#ifdef _LINUX
#define stricmp strcasecmp
#include <X11/Xlib.h>
#endif

#include <vector>

// wxwindows
#include <wx/wxprec.h>
#include <wx/bitmap.h>
#include <wx/choicebk.h>
#include <wx/propdlg.h>
#include <wx/spinctrl.h>
#include <wx/listctrl.h>
#include <wx/statline.h>
#include <wx/clipbrd.h>
#include <wx/rawbmp.h>
#include <wx/dcclient.h>
#include <wx/sizer.h>
#include <wx/cshelp.h>
#include <wx/dynlib.h>
#include <wx/menu.h>
#include <wx/bmpbuttn.h>
#include <wx/file.h>
#include <wx/config.h>
#include <wx/splash.h>
#include <wx/splitter.h>
#include <wx/filesys.h>
#include <wx/fs_zip.h>
#include <wx/spinctrl.h>
#include <wx/msgdlg.h>
#include <wx/bmpbuttn.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/statusbr.h>
#include <wx/filedlg.h>
#include <wx/log.h>
#include <wx/treectrl.h>
#include <wx/toolbar.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/manager.h>
#include <wx/html/helpctrl.h>
#include <wx/aui/aui.h>

#include <hoe_utils.h>
#include "hoe_console.h"
#include "hoe_engine.h"
#include "hoe_app.h"
#include "hoe_lang.h"
#include "hoe_resource_mgr.h"
#include "hoe_view.h"
#include "hoe_scene.h"
#include "hoe_object.h"
#include "hoe_strategy.h"
#include "hoe_stratmap.h"
#include "hoe_input.h"
#include "hoe_music.h"
#include "lua_script.h"
#include "hoe_timer.h"
#include "hoe_vars.h" 

#include <he/app.h>
#include <he/editor.h>
#include <he/engview.h>
#include <he/panels.h>
#include <he/properties.h>
#include <he/resources.h>
#include <he/utils.h>
#include <he/undolist.h> 

#include <hoe3d.h>

#endif // _STDAFX_H_

