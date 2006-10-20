
/*
* Parser
*/

#ifndef _HOE_SL_PARSER_H_
#define _HOE_SL_PARSER_H_

class BaseParser
{
public:
	virtual void BeginNamespace(const char * name) { };
	virtual void EndNamespace() { };
	virtual void BeginIndex(const char * name) { };
	virtual void BeginMaterial(const char * name) { };
	virtual void BeginStream(const char * name) { };
	virtual void BeginModel(const char * name) { };
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
	virtual void ParseNumber(int num) { };
	virtual void Point(const char * name, float x, float y, float z) { };
	virtual void FuncAutotracking(const char * name) { };
};

class PrintParser : public BaseParser
{
public:
	virtual void BeginNamespace(const char * name) { printf("Namespace %s\n", name); };
	virtual void EndNamespace() { };
	virtual void BeginIndex(const char * name) { printf("Index %s\n",name); };
	virtual void BeginMaterial(const char * name) { printf("Material %s\n",name); };
	virtual void BeginStream(const char * name) { printf("Stream %s\n",name); };
	virtual void BeginModel(const char * name) { printf("Model %s\n",name); };
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
	virtual void ParseNumber(int num) { };
	virtual void Point(const char * name, float x, float y, float z) { };
	virtual void FuncAutotracking(const char * name) { };
};

#endif // _HOE_SL_PARSER_H_

