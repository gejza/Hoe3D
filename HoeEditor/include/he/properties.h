
#ifndef _HOE_EDITOR_PROPERTIES_H_
#define _HOE_EDITOR_PROPERTIES_H_

// #include <wx/propgrid/propgrid.h>

/*
struct PropertyData
{
	HoePlugin::PluginObject * plugin;
	int id;
	void * data;
};

class PropertyCtrl : public HoeExtern::IProperty
{
public:
	void SetData(wxPGId id, int i, void * data);
	PropertyCtrl();
	~PropertyCtrl();
	void Create(wxWindow * parent);
	myPropertyGrid * GetPropertyGrid() const { return m_property; }
	virtual void Clear();
	virtual void AppendCategory(const char * label);
	virtual void AppendString(int id, const char * label, const char * str, unsigned long flags = 0, const char * help = NULL, void * data = NULL);
	virtual void AppendFloat(int id, const char * label, float def, unsigned long flags = 0,  const char * help = NULL, void * data = NULL);
	virtual void AppendBool(int id, const char * label, bool def, unsigned long flags = 0,  const char * help = NULL, void * data = NULL);
	virtual void AppendColor(int id, const char * label, unsigned long color, unsigned long flags = 0, const char * help = NULL, void * data = NULL);
	virtual void AppendSize(int id, const char * label, int width, int height, unsigned long flags = 0, const char * help = NULL, void * data = NULL);
	virtual void AppendPoint(int id, const char * label, const float x, const float y, unsigned long flags = 0, const char * help = NULL, void * data = NULL);
	virtual void AppendPoint3D(int id, const char * label, const float x, const float y, const float z, unsigned long flags = 0, const char * help = NULL, void * data = NULL);
	virtual void AppendList(int id, const char * label, const char *list[], long values[] = NULL, long value = 0, unsigned long flags = 0, const char * help = NULL, void * data = NULL); 
	virtual void AppendAngle(int id, const char * label, float def, unsigned long flags = 0, const char * help = NULL, void * data = NULL);
	virtual void AppendImageFile(int id, const char * label, const char * str, unsigned long flags = 0, const char * help = NULL, void * data = NULL);

	friend class LeftPanel;

};*/

namespace HoeEditor {

enum 
{
	PG_PROP_DISABLE = 1,
	PG_PROP_MODIFIED = 2,
};

class PropItem
{
	wxPGProperty * prop;
public:
	PropItem(wxPGProperty * _p);
	float GetFloat() const ;
	bool GetString(char * buff, size_t bufflen) const;
	void GetSize(int * width, int * height)const ;
	unsigned long GetColor() const ;
	void GetPoint(float *x, float *y) const;
	void GetPoint3D(float *x, float *y, float *z) const;
	long GetLong() const;
	float GetAngle() const;
	const char * GetFilePath() const;

};

class PropObject
{
public:
	virtual void OnChangeProp(int id, const PropItem & pi) {};
};

class PropertyGrid : public wxPropertyGridManager
{
protected:
	PropObject * m_propo;
public:
	PropertyGrid(wxWindow * parent);
	~PropertyGrid();
    void OnChange ( wxPropertyGridEvent& event );
    void OnSelect ( wxPropertyGridEvent& event );
    void OnHighlight ( wxPropertyGridEvent& event );
    void OnItemRightClick ( wxPropertyGridEvent& event );
    void OnPageChange ( wxPropertyGridEvent& event );

	bool Begin(PropObject * po);
	void End();
	void SetProp(wxPGId i, int id, unsigned long flags, const char * help);

	void AppendLong(int id, const char * label, long def, unsigned long flags = 0, const char * help = NULL);
	void AppendBool(int id, const char * label, bool def, unsigned long flags = 0, const char * help = NULL);

	DECLARE_EVENT_TABLE()
};

}; // namespace HoeEditor

#endif // _HOE_EDITOR_PROPERTIES_H_

