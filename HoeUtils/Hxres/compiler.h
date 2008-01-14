
#ifndef _HXRES_COMPILER_H_
#define _HXRES_COMPILER_H_

#include "image.h"

typedef HoeCore::List<HoeCore::Universal> VectorUniversal;

void no_run(const HoeCore::CString str);
#define EMPTY_FUNC { no_run(__FUNCTION__); return false; }

class PInterface
{
public:
	virtual bool AddProp(const HoeCore::CString name, const HoeCore::Universal& value) EMPTY_FUNC
	virtual bool AddProp(const HoeCore::CString name, const VectorUniversal& value) EMPTY_FUNC
	virtual bool Func(const HoeCore::CString name, const VectorUniversal& value) EMPTY_FUNC

	virtual void Done() {}
};

class Compiler : public PInterface
{
protected:
	HoeCore::WriteStream& m_out;
public:
	Compiler(HoeCore::WriteStream& out) : m_out(out) {}
    static Compiler * Create(HoeCore::String& str, int type, HoeCore::WriteStream& s);

};

class PictureCompiler : public Compiler
{
	Image im;
public:
	PictureCompiler(HoeCore::WriteStream& out) : Compiler(out) {}
	virtual bool AddProp(const HoeCore::CString name, const HoeCore::Universal& value);
	virtual bool AddProp(const HoeCore::CString name, const VectorUniversal& value);
	virtual bool Func(const HoeCore::CString name, const VectorUniversal& value);

	virtual void Done();
};

class StreamCompiler : public Compiler
{
public:
	StreamCompiler(HoeCore::WriteStream& out) : Compiler(out) {}
};

enum EResourceType
{
	ERT_Picture,
	ERT_Stream,
	ERT_Index,
	ERT_Model,
};

#endif //  _HXRES_COMPILER_H_


