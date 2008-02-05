
#ifndef _HXRES_COMPILER_H_
#define _HXRES_COMPILER_H_

#include "image.h"

class Value : public HoeCore::Universal
{
	int m_vt;
public:
	template <class TYPE>
	Value(TYPE t, int vt) : HoeCore::Universal(t), m_vt(vt)
	{
	}
};

typedef HoeCore::List<Value> Values;

class PInterface
{
public:
	virtual bool AddProp(const HoeCore::CString name, const Value& value);
	virtual bool AddProp(const HoeCore::CString name, const Values& value);
	virtual bool Func(const HoeCore::CString name, 
                      const HoeCore::CString ret,
                      const Values& value);

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
	virtual bool AddProp(const HoeCore::CString name, const Value& value);
	virtual bool AddProp(const HoeCore::CString name, const Values& value);
	//virtual bool Func(const HoeCore::CString name, const VectorUniversal& value);

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


