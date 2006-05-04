
#ifndef _MODEL_VIEW_H_
#define _MODEL_VIEW_H_

#include <hoe_view.h>

class ModelView : public HoeEditor::FileEditor
{
public:
};

class ModelViewApp : public HoeEditor::App
{
public:
	virtual HoeEditor::BaseEditor * CreateEditor();
};

/*
struct FileItem
{
	std::string fname;
	int item;
};

class ModelObject : public XHoeObject
{
	std::string m_name;
	FileItem * m_file;
	bool m_valid;
public:
	ModelObject(const char * name, FileItem * file);
	void Unvalidate() { m_valid = false; }
	void Reload();
	std::string & GetName() { return m_name; }
	void Select();
	void Unselect();

};

class ModelView
{
	std::vector<FileItem*> m_files;
	std::vector<ModelObject*> m_models;
	ModelObject * m_act;
	class IHoeScene * m_scene;
	HoeGame::ModelView m_view;
	int m_fileimage;
	int m_modelimage;
	unsigned long m_backcolor;
public:
	ModelView();
	virtual const char * GetIS();
	virtual bool Init();
	virtual int GetNumExt() { return 1; }
	virtual const char * GetExt(int i);
	virtual const char * GetLongDesc(int i);
	virtual const char * GetShortDesc(int i);
	virtual bool OpenFile(const char * fpath);
	virtual void OnSelectItem(int item, void * data);
	virtual void OnUnselectItem(int item, void * data);
	virtual void OnChangeProperty(HoeExtern::IPropertyItem * item);
	virtual void UnloadEngine();
	virtual void ReloadEngine();
	virtual bool IsModified() { return true; }
	virtual bool Save(HoeExtern::IProjectSave *);
	virtual bool Load(HoeExtern::IProjectLoad *);
	void ActivateModel(ModelObject * m);
	IHoeScene * GetScene();
	void UpdateBackground();


};*/

#endif // _MODEL_VIEW_H_

