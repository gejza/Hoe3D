
#include "StdAfx.h"
#include "../include/he/properties.h"

// -----------------------------------------------------------------------
// wxSizeProperty
// -----------------------------------------------------------------------
WX_PG_DECLARE_VALUE_TYPE_VOIDP(wxSize)
WX_PG_DECLARE_PROPERTY(wxSizeProperty,const wxSize&,wxSize(0,0))
WX_PG_IMPLEMENT_VALUE_TYPE_VOIDP(wxSize,wxSizeProperty,wxSize(0,0))

class wxSizePropertyClass : public wxPGPropertyWithChildren
{
    WX_PG_DECLARE_PROPERTY_CLASS()
public:

    wxSizePropertyClass ( const wxString& label, const wxString& name,
        const wxSize& value );
    virtual ~wxSizePropertyClass ();

    WX_PG_DECLARE_PARENTAL_TYPE_METHODS()
    WX_PG_DECLARE_PARENTAL_METHODS()

protected:
    wxSize                  m_value;
};

WX_PG_IMPLEMENT_PROPERTY_CLASS(wxSizeProperty,wxBaseParentProperty,wxSize,const wxSize&,TextCtrl)

wxSizePropertyClass::wxSizePropertyClass ( const wxString& label, const wxString& name,
    const wxSize& value) : wxPGPropertyWithChildren(label,name)
{
    wxPG_INIT_REQUIRED_TYPE(wxSize)
    DoSetValue((void*)&value);
    AddChild( wxIntProperty(_("Width"),wxPG_LABEL,value.x) );
    AddChild( wxIntProperty(_("Height"),wxPG_LABEL,value.y) );
}

wxSizePropertyClass::~wxSizePropertyClass () { }

void wxSizePropertyClass::DoSetValue ( wxPGVariant value )
{
    wxSize* pObj = (wxSize*)wxPGVariantToVoidPtr(value);
    m_value = *pObj;
    RefreshChildren();
}

wxPGVariant wxSizePropertyClass::DoGetValue () const
{
    return wxPGVariant((void*)&m_value);
}

void wxSizePropertyClass::RefreshChildren()
{
    if ( !GetCount() ) return;
    Item(0)->DoSetValue( (long)m_value.x );
    Item(1)->DoSetValue( (long)m_value.y );
}

void wxSizePropertyClass::ChildChanged ( wxPGProperty* p )
{
    switch ( p->GetIndexInParent() )
    {
        case 0: m_value.x = p->DoGetValue().GetLong(); break;
        case 1: m_value.y = p->DoGetValue().GetLong(); break;
    }
}

// -----------------------------------------------------------------------
// wxRectProperty
// -----------------------------------------------------------------------
WX_PG_DECLARE_VALUE_TYPE_VOIDP(wxRect)
WX_PG_DECLARE_PROPERTY(wxRectProperty,const wxRect&,wxRect(0,0,0,0))
WX_PG_IMPLEMENT_VALUE_TYPE_VOIDP(wxRect,wxRectProperty,wxRect(0,0,0,0))

class wxRectPropertyClass : public wxPGPropertyWithChildren
{
    WX_PG_DECLARE_PROPERTY_CLASS()
public:

    wxRectPropertyClass ( const wxString& label, const wxString& name,
        const wxRect& value );
    virtual ~wxRectPropertyClass ();

    WX_PG_DECLARE_PARENTAL_TYPE_METHODS()
    WX_PG_DECLARE_PARENTAL_METHODS()

protected:
    wxRect                  m_value;
};

WX_PG_IMPLEMENT_PROPERTY_CLASS(wxRectProperty,wxBaseParentProperty,wxRect,const wxRect&,TextCtrl)

wxRectPropertyClass::wxRectPropertyClass ( const wxString& label, const wxString& name,
    const wxRect& value) : wxPGPropertyWithChildren(label,name)
{
    wxPG_INIT_REQUIRED_TYPE(wxRect)
    DoSetValue((void*)&value);
    AddChild( wxIntProperty(_("Left"),wxPG_LABEL,value.GetLeft()) );
    AddChild( wxIntProperty(_("Top"),wxPG_LABEL,value.GetTop()) );
     AddChild( wxIntProperty(_("Right"),wxPG_LABEL,value.GetRight()) );
   AddChild( wxIntProperty(_("Bottom"),wxPG_LABEL,value.GetBottom()) );
}

wxRectPropertyClass::~wxRectPropertyClass () { }

void wxRectPropertyClass::DoSetValue ( wxPGVariant value )
{
    wxRect* pObj = (wxRect*)wxPGVariantToVoidPtr(value);
    m_value = *pObj;
    RefreshChildren();
}

wxPGVariant wxRectPropertyClass::DoGetValue () const
{
    return wxPGVariant((void*)&m_value);
}

void wxRectPropertyClass::RefreshChildren()
{
    if ( !GetCount() ) return;
    Item(0)->DoSetValue( (long)m_value.GetLeft() );
    Item(1)->DoSetValue( (long)m_value.GetTop() );
    Item(2)->DoSetValue( (long)m_value.GetRight() );
    Item(3)->DoSetValue( (long)m_value.GetBottom() );
}

void wxRectPropertyClass::ChildChanged ( wxPGProperty* p )
{
    switch ( p->GetIndexInParent() )
    {
        case 0: m_value.SetLeft(p->DoGetValue().GetLong()); break;
        case 1: m_value.SetTop(p->DoGetValue().GetLong()); break;
        case 2: m_value.SetRight(p->DoGetValue().GetLong()); break;
        case 3: m_value.SetBottom(p->DoGetValue().GetLong()); break;
    }
}

// -----------------------------------------------------------------------
// wxPointProperty
// -----------------------------------------------------------------------
WX_PG_DECLARE_VALUE_TYPE_VOIDP(wxRealPoint)
WX_PG_DECLARE_PROPERTY(wxPointProperty,const wxRealPoint&,wxRealPoint(0,0))
WX_PG_IMPLEMENT_VALUE_TYPE_VOIDP(wxRealPoint,wxPointProperty,wxRealPoint(0,0))

class wxPointPropertyClass : public wxPGPropertyWithChildren
{
    WX_PG_DECLARE_PROPERTY_CLASS()
public:

    wxPointPropertyClass( const wxString& label, const wxString& name,
        const wxRealPoint& value );
    virtual ~wxPointPropertyClass ();

    WX_PG_DECLARE_PARENTAL_TYPE_METHODS()
    WX_PG_DECLARE_PARENTAL_METHODS()

protected:
    wxRealPoint                  m_value;
};

WX_PG_IMPLEMENT_PROPERTY_CLASS(wxPointProperty,wxBaseParentProperty,
                               wxRealPoint,const wxRealPoint&,TextCtrl)

wxPointPropertyClass::wxPointPropertyClass ( const wxString& label, const wxString& name,
    const wxRealPoint& value) : wxPGPropertyWithChildren(label,name)
{
    wxPG_INIT_REQUIRED_TYPE(wxRealPoint)
    DoSetValue((void*)&value);
    AddChild( wxFloatProperty(wxT("X"),wxPG_LABEL,value.x) );
    AddChild( wxFloatProperty(wxT("Y"),wxPG_LABEL,value.y) );
}

wxPointPropertyClass::~wxPointPropertyClass () { }

void wxPointPropertyClass::DoSetValue ( wxPGVariant value )
{
    wxRealPoint* pObj = (wxRealPoint*)wxPGVariantToVoidPtr(value);
    m_value = *pObj;
    RefreshChildren();
}

wxPGVariant wxPointPropertyClass::DoGetValue () const
{
    return wxPGVariant((void*)&m_value);
}

void wxPointPropertyClass::RefreshChildren()
{
    if ( !GetCount() ) return;
    Item(0)->DoSetValue( m_value.x );
    Item(1)->DoSetValue( m_value.y );
}

void wxPointPropertyClass::ChildChanged ( wxPGProperty* p )
{
    switch ( p->GetIndexInParent() )
    {
        case 0: m_value.x = p->DoGetValue().GetDouble(); break;
        case 1: m_value.y = p->DoGetValue().GetDouble(); break;
    }
}

// -----------------------------------------------------------------------
// wxPoint3DProperty
// -----------------------------------------------------------------------

class Point3D
{
public:
    double x;
    double y;
	double z;

    Point3D() : x(0.0), y(0.0), z(0.0) { }
    Point3D(double xx, double yy, double zz) : x(xx), y(yy), z(zz) { }

    Point3D operator+(const Point3D& pt) const { return Point3D(x + pt.x, y + pt.y, z + pt.z); }
    Point3D operator-(const Point3D& pt) const { return Point3D(x - pt.x, y - pt.y, z - pt.z); }

    bool operator==(const Point3D& pt) const { return x == pt.x && y == pt.y && z == pt.z; }
    bool operator!=(const Point3D& pt) const { return x != pt.x || y != pt.y || z != pt.z; }
};

WX_PG_DECLARE_VALUE_TYPE_VOIDP(Point3D)
WX_PG_DECLARE_PROPERTY(wxPoint3DProperty,const Point3D&,Point3D(0,0,0))
WX_PG_IMPLEMENT_VALUE_TYPE_VOIDP(Point3D,wxPoint3DProperty,Point3D(0,0,0))

class wxPoint3DPropertyClass : public wxPGPropertyWithChildren
{
    WX_PG_DECLARE_PROPERTY_CLASS()
public:

    wxPoint3DPropertyClass( const wxString& label, const wxString& name,
        const Point3D& value );
    virtual ~wxPoint3DPropertyClass ();

    WX_PG_DECLARE_PARENTAL_TYPE_METHODS()
    WX_PG_DECLARE_PARENTAL_METHODS()

protected:
    Point3D                  m_value;
};

WX_PG_IMPLEMENT_PROPERTY_CLASS(wxPoint3DProperty,wxBaseParentProperty,
                               Point3D,const Point3D&,TextCtrl)

wxPoint3DPropertyClass::wxPoint3DPropertyClass ( const wxString& label, const wxString& name,
    const Point3D& value) : wxPGPropertyWithChildren(label,name)
{
    wxPG_INIT_REQUIRED_TYPE(Point3D)
    DoSetValue((void*)&value);
    AddChild( wxFloatProperty(wxT("X"),wxPG_LABEL,value.x) );
    AddChild( wxFloatProperty(wxT("Y"),wxPG_LABEL,value.y) );
    AddChild( wxFloatProperty(wxT("Z"),wxPG_LABEL,value.z) );
}

wxPoint3DPropertyClass::~wxPoint3DPropertyClass () { }

void wxPoint3DPropertyClass::DoSetValue ( wxPGVariant value )
{
    Point3D* pObj = (Point3D*)wxPGVariantToVoidPtr(value);
    m_value = *pObj;
    RefreshChildren();
}

wxPGVariant wxPoint3DPropertyClass::DoGetValue () const
{
    return wxPGVariant((void*)&m_value);
}

void wxPoint3DPropertyClass::RefreshChildren()
{
    if ( !GetCount() ) return;
    Item(0)->DoSetValue( m_value.x );
    Item(1)->DoSetValue( m_value.y );
    Item(2)->DoSetValue( m_value.z );
}

void wxPoint3DPropertyClass::ChildChanged ( wxPGProperty* p )
{
    switch ( p->GetIndexInParent() )
    {
        case 0: m_value.x = p->DoGetValue().GetDouble(); break;
        case 1: m_value.y = p->DoGetValue().GetDouble(); break;
        case 2: m_value.z = p->DoGetValue().GetDouble(); break;
    }
} 

///------------------------------------------------------------------------
HoeEditor::PropItem::PropItem(wxPGProperty * _p)
{
	prop = _p;
}

void HoeEditor::PropItem::GetSize(int * width, int * height) const
{
	wxSize *s = reinterpret_cast<wxSize *>(prop->DoGetValue().GetVoidPtr());
	*width = s->GetWidth();
	*height = s->GetHeight();
}

const wxRect HoeEditor::PropItem::GetRect() const
{
	wxRect *s = reinterpret_cast<wxRect *>(prop->DoGetValue().GetVoidPtr());
	return *s;
}

float HoeEditor::PropItem::GetFloat() const
{
	return (float)prop->DoGetValue().GetDouble();
}

const wxString & HoeEditor::PropItem::GetString() const
{
	return prop->DoGetValue().GetString();
}

unsigned long HoeEditor::PropItem::GetColor() const
{
	wxColour * c = (wxColour *)prop->DoGetValue().GetVoidPtr();
	return (0xff000000 | ((c->Red()) << 16) | ((c->Green()) << 8) | (c->Blue()));
}

void HoeEditor::PropItem::GetPoint(float *x, float *y) const
{
	wxRealPoint * p = (wxRealPoint *)prop->DoGetValue().GetVoidPtr();
	if (x) *x = p->x;
	if (y) *y = p->y;
}

void HoeEditor::PropItem::GetPoint3D(float *x, float *y, float *z) const
{
	Point3D * p = (Point3D *)prop->DoGetValue().GetVoidPtr();
	if (x) *x = p->x;
	if (y) *y = p->y;
	if (z) *z = p->z;
}

long HoeEditor::PropItem::GetLong() const 
{
	return prop->DoGetValue().GetLong();
}

bool HoeEditor::PropItem::GetBool() const 
{
	return prop->DoGetValue().GetBool();
}

float HoeEditor::PropItem::GetAngle() const
{
	return prop->DoGetValue().GetLong() * (3.1415926535/180.f);
}

const char * HoeEditor::PropItem::GetFilePath() const
{
	static wxString path;
	path = prop->GetValueAsString(wxPG_FULL_VALUE);
	return path.c_str();
}



////////////////////////////////////////////////////////////////////////////
BEGIN_EVENT_TABLE(HoeEditor::PropertyGrid, wxPropertyGridManager)
    // This occurs when a property is selected
    //EVT_PG_SELECTED( PGID, myPropertyGrid::OnSelect )
    // This occurs when a property value changes
    EVT_PG_CHANGED( PG_ID, HoeEditor::PropertyGrid::OnChange )
    // This occurs when a mouse moves over another property
    //EVT_PG_HIGHLIGHTED( PGID, myPropertyGrid::OnPropertyGridHighlight )
    // This occurs when mouse is right-clicked.
    //EVT_PG_RIGHT_CLICK( PGID, myPropertyGrid::OnPropertyGridItemRightClick )
    // Thus occurs when propgridmanager's page changes.
    //EVT_PG_PAGE_CHANGED( PGID, myPropertyGrid::OnPropertyGridPageChange )
END_EVENT_TABLE()
///////////////////////////////////////////////////////////////////////////

HoeEditor::PropertyGrid::PropertyGrid()
{
	m_propo = NULL;
}

HoeEditor::PropertyGrid::~PropertyGrid()
{
	wxConfigBase *pConfig = wxConfigBase::Get();
	if ( pConfig == NULL )
		return;

	// save the frame position
	int w, h;
	GetSize(&w, &h);
	pConfig->Write(_T("/MainFrame/prop_width"), (long) w);
	pConfig->Write(_T("/MainFrame/prop_height"), (long) h);
}

bool HoeEditor::PropertyGrid::Create(wxWindow * parent)
{
	wxConfigBase *pConfig = wxConfigBase::Get();
	int w = pConfig->Read(_T("/MainFrame/prop_width"), 100),
		h = pConfig->Read(_T("/MainFrame/prop_height"), 300);
	wxPropertyGridManager::Create(parent, PG_ID,
        wxDefaultPosition, wxSize(w,h),
        wxPG_BOLD_MODIFIED|
        wxPG_SPLITTER_AUTO_CENTER|
        wxPG_AUTO_SORT|
        //wxPG_HIDE_MARGIN|wxPG_STATIC_SPLITTER|
        //wxPG_HIDE_CATEGORIES|
        //wxPG_LIMITED_EDITING |
        wxTAB_TRAVERSAL |
        //wxPG_TOOLBAR |
        wxPG_DESCRIPTION |
        //wxPG_COMPACTOR |
        wxPGMAN_DEFAULT_STYLE
       );
	return true;
}

void HoeEditor::PropertyGrid::OnChange( wxPropertyGridEvent& event )
{
	if (m_propo)
	{
		PropItem pi( event.GetPropertyPtr());
		m_propo->OnChangeProp( (int)event.GetPropertyClientData(), pi);
	}
}

bool HoeEditor::PropertyGrid::Begin(PropObject * po)
{
	m_propo = po;
	GetGrid()->Clear();
	return true;
}

void HoeEditor::PropertyGrid::End()
{
	Refresh();
}

void HoeEditor::PropertyGrid::SetProp(wxPGId i, int id, unsigned long flags, const char * help)
{
	if (help)
		i.GetProperty().SetHelpString(help);

	if (flags & PG_PROP_DISABLE)
		DisableProperty(i);

	if (flags & PG_PROP_MODIFIED)
		i.GetProperty().SetFlag ( wxPG_PROP_MODIFIED );

	i.GetProperty().SetClientData((void*)id);
}

void HoeEditor::PropertyGrid::AppendLong(int id, const char * label, long def, unsigned long flags, const char * help)
{
	wxPGId pgid = Append(label, label, (int)def);
	SetProp( pgid, id, flags, help);
}

void HoeEditor::PropertyGrid::AppendBool(int id, const char * label, bool def, unsigned long flags, const char * help)
{
	wxPGId pgid = Append(label, label, (bool)def);
	SetProp( pgid, id, flags, help);
}

void HoeEditor::PropertyGrid::AppendFloat(int id, const char * label, float def, unsigned long flags, const char * help)
{
	wxPGId pgid = Append(label, label, (double)def);
	SetProp( pgid, id, flags, help);
}

void HoeEditor::PropertyGrid::AppendColor(int id, const char * label, unsigned long color, unsigned long flags, const char *help, void * data)
{
	wxPGId pgid = Append(
		wxColourProperty(label, label, wxColour((color>>16)&0xff,(color>>8)&0xff,0xff&color)));
	SetProp( pgid, id, flags, help);
}

void HoeEditor::PropertyGrid::AppendCategory(const char * label)
{
	wxPropertyGridManager::AppendCategory(label);
}

void HoeEditor::PropertyGrid::AppendString(int id, const char * label, const char * def, unsigned long flags, const char * help, void * data)
{
	wxPGId pgid = Append( wxStringProperty(label, label, def));
	SetProp( pgid, id, flags, help);
}

void HoeEditor::PropertyGrid::AppendSize(int id, const char *label, int width, int height, unsigned long flags, const char *help, void * data)
{
	wxPGId pgid = Append(wxSizeProperty(label, label, wxSize(width,height)));
	SetProp( pgid, id, flags, help);
}

void HoeEditor::PropertyGrid::AppendRect(int id, const char *label, const wxRect & Rect, unsigned long flags, const char *help, void * data)
{
	wxPGId pgid = Append(wxRectProperty(label, label, Rect));
	SetProp( pgid, id, flags, help);
}

void HoeEditor::PropertyGrid::AppendPoint(int id, const char * label, const float x, const float y, unsigned long flags, const char * help, void * data)
{
	wxPGId pgid = Append(wxPointProperty(label, label, wxRealPoint(x,y)));
	SetProp( pgid, id, flags, help);
}

void HoeEditor::PropertyGrid::AppendPoint3D(int id, const char * label, const float x, const float y, const float z, unsigned long flags, const char * help, void * data)
{
	wxPGId pgid = Append(wxPoint3DProperty(label, label, Point3D(x,y,z)));
	SetProp( pgid, id, flags, help);
}

void HoeEditor::PropertyGrid::AppendList(int id, const char * label, const char *list[], long values[], long value, unsigned long flags, const char * help, void * data)
{
	wxPGId pgid = Append(wxEnumProperty(label, label, list, values, value));
	SetProp( pgid, id, flags, help);
}

void HoeEditor::PropertyGrid::AppendAngle(int id, const char * label, float def, unsigned long flags, const char * help, void * data)
{
	wxPGId pgid = Append(label, label, int(def * (180.f / 3.141592654)));
	SetProp( pgid, id, flags, help);
}

void HoeEditor::PropertyGrid::AppendImageFile(int id, const char * label, const char * str, unsigned long flags, const char * help, void * data)
{
	wxPGId pgid = Append(wxImageFileProperty(label, label, str));
	SetProp( pgid, id, flags, help);
}

void HoeEditor::PropertyGrid::AppendFont(int id, const char * label, const wxFont & font, unsigned long flags, const char * help, void * data)
{
	wxPGId pgid = Append(wxFontProperty(label, label, font));
	SetProp( pgid, id, flags, help);
}

///////////////////////////////
/*
PropertyCtrl::PropertyCtrl()
{
	m_property = NULL;
	m_plgprop = NULL;
}

PropertyCtrl::~PropertyCtrl()
{
	if (m_property)
		delete m_property;
}

void PropertyCtrl::Create(wxWindow *parent)
{
	m_property = new myPropertyGrid(parent);
}

void PropertyCtrl::SetData(wxPGId id, int i, void *data)
{
	if (!m_plgprop)
		return;

	PropertyData * pd = new PropertyData;
	pd->plugin = m_plgprop;
	pd->id = i;
	pd->data = data;
	id.GetProperty().SetClientData(pd);

}

/*/

///////////////////////////////


