
#ifndef _CLASS_PARSER_H_
#define _CLASS_PARSER_H_

#include "parser.h"
#include "fvf.h"
#include "mshader.h"

class CBaseObject
{
};

class CBaseStream : public CBaseObject
{
public:
	virtual void EndData();
	virtual void AddVertex(VERTEX & vert);
	
};

class CBaseIndex : public CBaseObject
{
public:
	virtual void AddIndex(unsigned short ind);
	virtual void EndData();
};

struct CColor;

class CBaseMaterial : public CBaseObject
{
public:
	enum Par {
		MAmbient = 1,
		MDiffuse,
		MSpecular,
		MAlphaTest,
	};
	virtual void SetColor(Par p, CColor * c);
	virtual void SetTexture(const char * texture);
	virtual void SetPar(Par p, int val);
};

struct CPError
{
	int line;
	int code;
	char par1[256];
	char par2[256];

};

struct CColor
{
	float red;
	float green;
	float blue;
	float alpha;
};

class ScriptParser
{
public:

	
};

class CParser
{
public:
	virtual void Error(CPError * err);
	virtual bool Parse();
	virtual const char * GetNextLine() = 0; 
protected:
	ScriptParser m_parser;
	p_value m_val;

	virtual bool ScanMaterial();
	bool ScanInputList(ModelShader * shader);
	bool ScanIndexList(ModelShader * shader);
	bool ScanStreamList(ModelShader * shader);
	bool ScanMaterialList(ModelShader * shader);
	virtual ModelShader * CreateModelShader(const std::string &name) = 0;
	bool ScanParameters(InsParam * par);
	virtual CBaseIndex * CreateIndex(const std::string &name) = 0;
	virtual bool ScanIndex();
	virtual CBaseStream * CreateStream(const std::string &name, _FVF & fvf) = 0;
	virtual CBaseMaterial * CreateMaterial(const std::string &name) = 0;
	const char * ScanLine();
	virtual bool ScanModel(void);
	bool ScanStream();
	bool ScanName(std::string & name, int *index);
	// 1 = ok
	// 0 = nic
	// -1 = chyba
	// 2 = end
	int ScanVertex(VERTEX &vert);
	bool ScanFVF(_FVF & fvf);
	bool ScanColor(CColor * c);

	// functions
	virtual void Autotracking(const char * name) {};
};

#endif // _CLASS_PARSER_H_

