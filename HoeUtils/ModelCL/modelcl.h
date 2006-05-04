

#ifndef _MODEL_COMPILER_H_
#define _MODEL_COMPILER_H_

#include "../sdk/hoePlugin.h"

class ModelScript : public HoePlugin::PluginObject
{
	std::string m_fname;
	int item;
	HoeExtern::ICodeBuffer * m_buffer;
public:
	ModelScript();
	bool OpenFileName(std::string fname);
	virtual int Msg(HoePlugin::EPluginMsg msg, int par1, void * par2);
	std::string GetFileName() { return m_fname; }
};

class ModelCL : public HoePlugin::PluginFile
{
	std::vector<ModelScript*> m_files;
	/*int m_teximage;
	class IHoeScene * m_scene;
	std::vector<TextureItem*> m_textures;
	TextureItem * m_active;
	int width,height;
	unsigned long back;
	bool aa;*/
public:
	ModelCL();
	virtual const char * GetIS();
	virtual bool Init();
	virtual int GetNumExt() { return 1; }
	virtual const char * GetExt(int i);
	virtual const char * GetLongDesc(int i);
	virtual const char * GetShortDesc(int i);
	virtual bool OpenFile(const char * fpath);
	void NewPack(const char * fpath);
	/*void ShowTexture(TextureItem *);
	virtual void HOEAPI _Paint(IHoe2D * );
	virtual void OnSelectItem(int item, void * data);
	virtual void OnChangeProperty(HoeExtern::IPropertyItem * item);
	virtual void UnloadEngine();
	virtual void ReloadEngine();
	virtual bool IsModified() { return true; }*/
	virtual bool Save(HoeExtern::IProjectSave *);
	virtual bool Load(HoeExtern::IProjectLoad *);
	virtual int Msg(HoePlugin::EPluginMsg msg, int par1, void * par2);
};

#endif // _MODEL_COMPILER_H_

