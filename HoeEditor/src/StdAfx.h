
#ifndef _STDAFX_H_
#define _STDAFX_H_

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx/wx.h".
#ifdef _LINUX
#define stricmp strcasecmp
#endif

#ifdef __WXGTK__
extern "C" {
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk/gdkx.h>
}
#endif

#include <wx/wxprec.h>
#include <wx/ptr_scpd.h>
#include <wx/log.h>
#include <wx/config.h>
#include <wx/treectrl.h>
#include <wx/propgrid/propgrid.h>
#include "wx/calctrl.h"
// Main propertygrid header.
#include <wx/propgrid/propgrid.h>
// Needed for implementing custom properties.
#include <wx/propgrid/propdev.h>
// Extra property classes.
#include <wx/propgrid/advprops.h>
// This defines wxPropertyGridManager.
#include <wx/propgrid/manager.h>
#include <wx/splitter.h>
#include <wx/filedlg.h>
#include <wx/dialog.h>
#include <wx/listctrl.h>
#include <wx/statline.h>
#include <wx/xml/xml.h>
#include <wx/stc/stc.h>
#include <wx/busyinfo.h>
#include <wx/dirdlg.h>
#include <wx/tipdlg.h>
#include <wx/dynlib.h>
#include <wx/propdlg.h>
#include <wx/sysopt.h>
#include <wx/artprov.h>
#include <wx/statusbr.h>

#ifdef __WXGTK__
#include <wx/gtk/win_gtk.h>
#endif

#include <vector>
#include <string.h>
#include <string>

#include <hoe_types.h>
#include <hoe_math.h>

#include <hoe3d.h>

#include <hoe_resource_mgr.h>

#include "../../HoeUtils/lib/hoeUtils.h"

enum {
	PG_ID,
    TREE_ID,
	ENGINE_ID,
	ND_LIST_ID,
};
// -----------------------------------------------------------------------

#endif // _STDAFX_H_

