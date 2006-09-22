
#ifndef _CLASSES_H_
#define _CLASSES_H_

#include "cparser.h"

class CStream : public CBaseStream
{
	_FVF fvf;
	HoeUtils::DataBuffer data;
	std::string name;

	HoeUtils::IDGEN id;
public:
	unsigned long GetID();
	CStream(const std::string &name_, _FVF & fvf_);
	std::string & GetName() { return name; }
	virtual void AddVertex(VERTEX & vert);

	typedef std::vector<CStream*> List; 
	// vypise informace o objektu na stdout
	virtual void PrintInfo(void);
	virtual unsigned long ExportHeader(HoeUtils::Stream * stream);
	virtual unsigned long ExportData(HoeUtils::Stream * stream);

	void Autotracking();
	void Move(float x, float y, float z);
};

class CIndex : public CBaseIndex
{
	std::string name;
	HoeUtils::DataBuffer data;

	HoeUtils::IDGEN id;
public:
	unsigned long GetID();
	CIndex(const std::string &name_);

	std::string & GetName() { return name; }
	virtual void AddIndex(unsigned short ind);

	typedef std::vector<CIndex*> List;
	// vypise info na stdout
	virtual void PrintInfo(void);
	virtual unsigned long ExportHeader(HoeUtils::Stream * stream);
	virtual unsigned long ExportData(HoeUtils::Stream * stream);
};

class CMaterial : public CBaseMaterial
{
	std::string name;
	std::string texture;
	CColor ambient;
	CColor diffuse;
	CColor specular;
	bool hastexture:1;
	bool hasamb:1;
	bool hasdiff:1;
	bool hasspec:1;
	int alphatest;
	HoeUtils::IDGEN id;

public:
	CMaterial(const std::string &name_);
	unsigned long GetID();
	typedef std::vector<CMaterial*> List;
	std::string & GetName() { return name; }
	virtual void SetColor(Par p, CColor * c);
	virtual void SetTexture(const char * texture);
	virtual void SetPar(Par p, int val);
	virtual void PrintInfo(void);
	virtual unsigned long ExportHeader(HoeUtils::Stream * stream);
	virtual unsigned long ExportData(HoeUtils::Stream * stream);


};

struct TNamespace;

class CModel : public ModelShader
{
private:
	std::string name;
	std::vector<std::string> lnStreams;
	std::vector<CStream*> lStreams;
	std::vector<std::string> lnIndices;
	std::vector<CIndex*> lIndices;
	std::vector<std::string> lnMaterials;
	std::vector<CMaterial*> lMaterials;
public:
	virtual int Export(HoeUtils::Stream * stream);

	CModel(const std::string &name_);
	std::string & GetName() { return name; }
	virtual void AddDefStream(const char * name);
	virtual void AddDefIndex(const char * name);
	virtual void AddDefMaterial(const char * name);

	bool Compile(TNamespace * names);

	typedef std::vector<CModel*> List;
};

/*

#include "../../src/hmodel_file.h"

class StreamBuffer : public Object
{
	_FVF m_fvf;
	HoeUtils::DataBuffer *m_data;
	int m_fpos;
public:
	StreamBuffer(Object * parent);
	void ReadFVF();
	virtual bool Parse();
	TypeObj GetType() { return Stream;}

	//bool Property(int type,const void *);
	//bool BeginData();
	//bool AddData(int type,const void *);
	//bool EndData();

	// export func
	int Export(HoeUtils::File *);

	static StreamBuffer * CreateStream(std::string & name,Object * parent);
};

class IndexBuffer : public Object
{
	HoeUtils::List<word> m_data;
	int m_fpos;
public:
	IndexBuffer(Object * parent);
	virtual bool Parse();

	TypeObj GetType() { return Index;}
	// export func
	int Export(HoeUtils::File *);

	void AddUnsignedData(unsigned int d);

	static IndexBuffer * CreateIndex(std::string & name,Object * parent);
};

class Material : public Object
{
	int m_fpos;
	hfm_color ambient;
	hfm_color diffuse;
	std::string texname;
	unsigned long flags;
public:
	Material(Object * parent);
	virtual bool Parse();
	TypeObj GetType() { return Mat;}
	int Export(HoeUtils::File *);

	static void hex2color(unsigned long hex, hfm_color & color);
	static Material * CreateMaterial(std::string & name,Object * parent);
};

class ModelBuffer : public Object
{
	std::vector<Object*> m_objs;
	//HoeUtils::DataBuffer m_render;
	int m_fpos;
public:
	ModelBuffer(Object * parent);
	virtual bool Parse();
	Object * Find(std::string name,bool rek);
	int GetIndex(TypeObj,std::string name);
	TypeObj GetType() { return Model;}

	int Export(HoeUtils::File *);

	static ModelBuffer * CreateModel(std::string & name,Object * parent);
};

class MainBuffer : public Object
{
	std::vector<Object*> m_objs;
public:
	MainBuffer();
	//virtual Object * CreateObject(TypeObj);
	Object * Find(std::string name,bool rek);
    TypeObj GetType() { return Main;}

	virtual bool Parse();
	bool Export(const char*);
};
*/

struct TNamespace
{
	CStream::List streams; 
	CIndex::List indices;
	CModel::List models;
	CMaterial::List materials;

public:
	std::vector<CStream *> FindStream(std::string name);
	CIndex * FindIndex(std::string name);
	CMaterial * FindMaterial(std::string name);
};

#endif // _CLASSES_H_

