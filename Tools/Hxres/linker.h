
#ifndef _HXRES_LINKER_H_
#define _HXRES_LINKER_H_

#include "compiler.h"
#include "scan.h"

class Linker : public PInterface
{
public:
	struct Obj
	{
		HoeCore::String name;
		int type;
		Compiler * c;
		HoeCore::File file;
		Scaner::Location location;
	};
protected:
	HoeCore::List<Obj> m_obj;
	HoeCore::String_s<2048> m_ns;
public:
	Linker(void);
	virtual ~Linker(void);

	void PushNamespace(const char * name);
	void PopNamespace();

	virtual Compiler * AddObject(const char * name, int type, const Scaner::Location& l);
	virtual int Link(const char * output);

	Obj* Find(const char * name);

	// functions
	virtual bool Func(const HoeCore::CString name, const Values& value);

};

#endif // _HXRES_LINKER_H_


