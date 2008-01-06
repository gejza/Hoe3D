
#ifndef _HXRES_COMPILER_H_
#define _HXRES_COMPILER_H_

typedef HoeCore::List<HoeCore::Universal> VectorUniversal;

#define EMPTY_FUNC { }

class Compiler
{
public:
    static Compiler * Create(HoeCore::String& str, int type);

	//
	virtual void AddProp(const char* name, const char* value) EMPTY_FUNC
	virtual void AddProp(const char* name, double value) EMPTY_FUNC
	virtual void AddProp(const char* name, long long value) EMPTY_FUNC
	virtual void AddProp(const char* name, const VectorUniversal& value) EMPTY_FUNC

	virtual void Save(class Stream& s);
};

class PictureCompiler : public Compiler
{
public:
	virtual void AddProp(const char* name, const char* value);
	virtual void AddProp(const char* name, double value);
	virtual void AddProp(const char* name, long long value);
	virtual void AddProp(const char* name, const VectorUniversal& value);
};

class StreamCompiler : public Compiler
{
};

enum EResourceType
{
	ERT_Picture,
	ERT_Stream,
	ERT_Index,
	ERT_Model,
};

#endif //  _HXRES_COMPILER_H_


