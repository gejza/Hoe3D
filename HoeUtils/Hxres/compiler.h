
#ifndef _HXRES_COMPILER_H_
#define _HXRES_COMPILER_H_

#include "image.h"

typedef HoeCore::List<HoeCore::Universal> VectorUniversal;

#define EMPTY_FUNC { return false; }

class PInterface
{
public:
	virtual bool AddProp(const HoeCore::CString name, const HoeCore::Universal& value) EMPTY_FUNC
	virtual bool AddProp(const HoeCore::CString name, const VectorUniversal& value) EMPTY_FUNC
	virtual bool Func(const HoeCore::CString name, const VectorUniversal& value) EMPTY_FUNC

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
	Image im;
public:
	PictureCompiler(Stream& out) : Compiler(out) {}
	virtual bool AddProp(const HoeCore::CString name, const HoeCore::Universal& value);
	virtual bool AddProp(const HoeCore::CString name, const VectorUniversal& value);
	virtual bool Func(const HoeCore::CString name, const VectorUniversal& value);
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


