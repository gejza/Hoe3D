
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
	virtual void Func(const HoeCore::CString name, const VectorUniversal& value) EMPTY_FUNC

	virtual void Done() {}
};

class Stream
{
public:
    virtual void Save(void* ptr, size_t size) = 0;
};

class File : public Stream
{
public:
	virtual void Save(void* ptr, size_t size) {};
};

class Compiler : public PInterface
{
protected:
	Stream& m_out;
public:
	Compiler(Stream& out) : m_out(out) {}
    static Compiler * Create(HoeCore::String& str, int type);

};

class PictureCompiler : public Compiler
{
public:
	PictureCompiler(Stream& out) : Compiler(out) {}
	virtual void AddProp(const HoeCore::CString name, const HoeCore::CString value);
	virtual void AddProp(const HoeCore::CString name, double value);
	virtual void AddProp(const HoeCore::CString name, long long value);
	virtual void AddProp(const HoeCore::CString name, const VectorUniversal& value);
	virtual void Func(const HoeCore::CString name, const VectorUniversal& value);
};

class StreamCompiler : public Compiler
{
public:
	StreamCompiler(Stream& out) : Compiler(out) {}
};

enum EResourceType
{
	ERT_Picture,
	ERT_Stream,
	ERT_Index,
	ERT_Model,
};

#endif //  _HXRES_COMPILER_H_


