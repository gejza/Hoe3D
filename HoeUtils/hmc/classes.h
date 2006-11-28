
#ifndef _CLASSES_H_
#define _CLASSES_H_

#include "fvf.h"

class CObject
{
	std::string name;
	HoeUtils::IDGEN id;
public:
	unsigned long GetID();
	virtual void PrintInfo(void) {}
	
};

class CStream : public CObject, public HoeSL::Parser::StreamParser
{
	HoeUtils::DataBuffer m_data;
	VERTEX m_vert;
public:
	CStream(_FVF & fvf_);
	void AddVertex(VERTEX & vert);
	virtual void AddNumber(int num);
	virtual void AddUnsigned(unsigned long num);
	virtual void AddFloat(float num);
	// vypise informace o objektu na stdout
	virtual unsigned long ExportHeader(HoeUtils::Stream * stream);
	virtual unsigned long ExportData(HoeUtils::Stream * stream);
	void Autotracking();
	void Move(float x, float y, float z);
};

class CIndex : public CObject, public HoeSL::Parser::IndexParser
{
	HoeUtils::DataBuffer m_data;
public:
	CIndex();
	virtual void AddIndex(unsigned short ind);
	virtual unsigned long ExportHeader(HoeUtils::Stream * stream);
	virtual unsigned long ExportData(HoeUtils::Stream * stream);
};

class CMaterial : public CObject, public HoeSL::Parser::MaterialParser
{
	std::string texture;
	CColor ambient;
	CColor diffuse;
	CColor specular;
	bool hastexture:1;
	bool hasamb:1;
	bool hasdiff:1;
	bool hasspec:1;
	int alphatest;
public:
	CMaterial();
	virtual void SetColor(Par p, CColor * c);
	virtual void SetTexture(const char * texture);
	virtual void SetPar(Par p, int val);
	virtual unsigned long ExportHeader(HoeUtils::Stream * stream);
	virtual unsigned long ExportData(HoeUtils::Stream * stream);
};

class CPoint : public CObject
{
	float x,y,z;
public:
	CPoint();

	virtual void SetPoint(float _x, float _y, float _z) { x=_x;y=_y;z=_z; }

	virtual unsigned long ExportHeader(HoeUtils::Stream * stream);
	virtual unsigned long ExportData(HoeUtils::Stream * stream);
};

class CModel : public CObject, public HoeSL::Parser::ModelParser
{
private:
	/*std::string name;
	std::vector<std::string> lnStreams;
	std::vector<CStream*> lStreams;
	std::vector<std::string> lnIndices;
	std::vector<CIndex*> lIndices;
	std::vector<std::string> lnMaterials;
	std::vector<CMaterial*> lMaterials;
	std::vector<std::string> lnHelpers;
	std::vector<CBaseHelper*> lHelpers;*/
public:
	virtual int Export(HoeUtils::Stream * stream);

	CModel();
	virtual void AddDefStream(const char * name);
	virtual void AddDefIndex(const char * name);
	virtual void AddDefMaterial(const char * name);
	virtual void AddDefHelper(const char * name);
};

#if 0

/*

// nejdriv se to nacte do objektu, a pak hodi na disk


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
	std::vector<CBaseHelper*> helpers;
public:
	std::vector<CStream *> FindStream(std::string name);
	CIndex * FindIndex(std::string name);
	CMaterial * FindMaterial(std::string name);
	CBaseHelper* FindHelper(std::string name);
};

#endif

#endif // _CLASSES_H_

