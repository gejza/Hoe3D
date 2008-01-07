
#ifndef _HXRES_COMPILER_H_
#define _HXRES_COMPILER_H_

typedef HoeCore::List<HoeCore::Universal> VectorUniversal;

#define EMPTY_FUNC { }

class PInterface
{
public:
	virtual void AddProp(const HoeCore::CString name, const HoeCore::CString value) EMPTY_FUNC
	virtual void AddProp(const HoeCore::CString name, double value) EMPTY_FUNC
	virtual void AddProp(const HoeCore::CString name, long long value) EMPTY_FUNC
	virtual void AddProp(const HoeCore::CString name, const VectorUniversal& value) EMPTY_FUNC
};

class Stream
{
public:
    virtual void Save(void* ptr, size_t size) = 0;
};

class File : public Stream
{
};

class Compiler : public PInterface
{
public:
    static Compiler * Create(HoeCore::String& str, int type);

	virtual void Save(Stream& s) {}
};

class PictureCompiler : public Compiler
{
public:
	virtual void AddProp(const HoeCore::CString name, const HoeCore::CString value);
	virtual void AddProp(const HoeCore::CString name, double value);
	virtual void AddProp(const HoeCore::CString name, long long value);
	virtual void AddProp(const HoeCore::CString name, const VectorUniversal& value);
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


