
#ifndef _HXRES_LINKER_H_
#define _HXRES_LINKER_H_

#include "compiler.h"

class Scaner;
class Linker : public PInterface
{
public:
	struct Obj
	{
		HoeCore::String name;
		int type;
		Compiler * c;
		HoeCore::File file;
		HoeCore::String define_file;
		int define_line;
	};
protected:
	HoeCore::List<Obj> m_obj;
	HoeCore::String_s<2048> m_ns;
public:
	Linker(void);
	virtual ~Linker(void);

	void PushNamespace(const char * name);
	void PopNamespace();

	virtual Compiler * AddObject(const char * name, int type, Scaner* scan);
	virtual int Link(const char * output);

	Obj* Find(const char * name);
};

#endif // _HXRES_LINKER_H_


