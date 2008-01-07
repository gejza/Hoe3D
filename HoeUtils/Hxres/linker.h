
#ifndef _HXRES_LINKER_H_
#define _HXRES_LINKER_H_

#include "compiler.h"

class Linker : public PInterface
{
public:
	struct Obj
	{
		HoeCore::String name;
		int type;
		Compiler * c;
        // file for save
	};
protected:
	HoeCore::List<Obj> m_obj;
	HoeCore::String_s<2048> m_ns;
public:
	Linker(void);
	virtual ~Linker(void);

	void PushNamespace(const char * name);
	void PopNamespace();

	virtual Compiler * AddObject(const char * name, int type);
	virtual int Link(const char * output);

};

#endif // _HXRES_LINKER_H_


