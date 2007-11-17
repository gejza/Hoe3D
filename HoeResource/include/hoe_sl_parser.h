
/*
* Parser
*/

#ifndef _HOE_SL_PARSER_H_
#define _HOE_SL_PARSER_H_

namespace HoeSL {
	namespace Parser {

class ObjectParser
{
public:
	virtual void End() {};
};

class StreamParser : public ObjectParser
{
public:
	virtual void AddNumber(int num) { };
	virtual void AddUnsigned(unsigned long num) { };
	virtual void AddFloat(float num) { };
};

class IndexParser : public ObjectParser
{
public:
	virtual void ParseNumber(int num) { };
};

class MaterialParser : public ObjectParser
{
public:
	struct CColor
	{
		float red;
		float green;
		float blue;
		float alpha;
	};
	enum Par {
		MAmbient = 1,
		MDiffuse,
		MSpecular,
		MAlphaTest,
	};
	virtual void SetTexture(const char * name) { };
};

class ModelParser : public ObjectParser
{
public:

};

class BaseParser
{
public:
	virtual void BeginNamespace(const char * name) { };
	virtual void EndNamespace() { };
	virtual IndexParser * BeginIndex(const char * name, bool define) { return NULL; };
	virtual MaterialParser * BeginMaterial(const char * name, bool define) { return NULL; };
	virtual StreamParser * BeginStream(const char * name, const char * fvf, bool define) { return NULL; };
	virtual ModelParser * BeginModel(const char * name, bool define) { return NULL; };
	//virtual void Begin(const char * name) { };
	//virtual void Begin(const char * name) { };
	//virtual void Begin(const char * name) { };
	//virtual void Begin(const char * name) { };
	//virtual void Begin(const char * name) { };
	//virtual void Begin(const char * name) { };
	virtual void EndIndex() { };
	virtual void EndMaterial() { };
	virtual void EndStream() { };
	virtual void EndModel() { };
	//virtual void End() { };
	virtual void Point(const char * name, float x, float y, float z) { };
	virtual void FuncAutotracking(const char * name) { };

	virtual void Error(const char * str) {}
};

////////////////////
void parser_scan(FILE * f, BaseParser * parser);
int get_line();

	} // namespace Parser
} // namespace SL


class PrintParser : public HoeSL::Parser::BaseParser
{
public:
	virtual void BeginNamespace(const char * name) { printf("Namespace %s\n", name); };
	virtual void EndNamespace() { };
	virtual HoeSL::Parser::IndexParser * BeginIndex(const char * name, bool define) { 
		printf("Index %s\n",name); return NULL; };
	virtual HoeSL::Parser::MaterialParser * BeginMaterial(const char * name, bool define) { 
		printf("Material %s\n",name); return NULL; };
	virtual HoeSL::Parser::StreamParser * BeginStream(const char * name, const char * fvf, bool define) { 
		printf("Stream %s[%s]\n",name,fvf); return NULL; };
	virtual HoeSL::Parser::ModelParser * BeginModel(const char * name, bool define) { 
		printf("Model %s\n",name);return NULL; };
	//virtual void Begin(const char * name) { };
	//virtual void Begin(const char * name) { };
	//virtual void Begin(const char * name) { };
	//virtual void Begin(const char * name) { };
	//virtual void Begin(const char * name) { };
	//virtual void Begin(const char * name) { };
	virtual void EndIndex() { };
	virtual void EndMaterial() { };
	virtual void EndStream() { };
	virtual void EndModel() { };
	//virtual void End() { };
	virtual void SetTexture(const char * name) { };
	virtual void Point(const char * name, float x, float y, float z) { };
	virtual void FuncAutotracking(const char * name) { };

	virtual void Error(const char * str) { fprintf(stderr, "Error %s\n",str); }
};

#endif // _HOE_SL_PARSER_H_

