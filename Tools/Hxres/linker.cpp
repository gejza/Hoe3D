
#include "StdAfx.h"
#include "linker.h"
#include "scan.h"
#include "parse.tab.hpp"
#include "linkout.h"

using namespace HoeRes;

class DefineError : public HoeUtils::Error
{
public:
	DefineError(const HoeCore::CString name, Namespace::Obj* obj)
	{
		if (obj->location.ident == "")
			m_str.printf(T("Symbol %s is already defined."), 
				name.GetPtr());
		else if (!obj->location.line)
			m_str.printf(T("Symbol %s is already defined in %s"), 
				name.GetPtr(), 
				obj->location.ident.GetPtr());
		else
			m_str.printf(T("Symbol %s is already defined in %s(%d)"), 
				name.GetPtr(), 
				obj->location.ident.GetPtr(),
				obj->location.line);
	}
};


Linker::Linker(HoeCore::MemoryPool& pool) : m_main("", pool), m_pool(pool)
{
	m_nss.Push(&m_main);
	m_act = &m_main;
}

Linker::~Linker(void)
{
}

Compiler * Linker::AddObject(const char * name, int type, const Scaner::Location& l)
{
	Namespace::Obj* prev;
	if (prev = m_act->Find(name))
		throw DefineError(name,prev);

	Namespace::Obj& o = m_act->CreateObj();
	o.type = type;
	o.name = name;
	o.location = l;
	HoeCore::String fn;
	fn.printf(T("%s_%d.ors"), o.name.GetPtr(), HoeCore::RandInt(30000));
	fn.Replace(':','_');
	bool b = o.file.Open(fn, HoeCore::File::hftTemp); // otevrit tmp soubor
	hoe_assert(b);
	o.c = Compiler::Create(o.name, type,o.file);
	return o.c;
}

void Linker::PushNamespace(const char * name)
{
	// najit jestli uz neni a kdyztak aktivovat
	Namespace * n = m_act->CreateNamespace(name);
	m_nss.Push(n);
	m_act = n;
}

void Linker::PopNamespace()
{
	if (m_nss.Count() <= 1)
		throw HoeUtils::Error("No namespace to pop.");
	m_nss.Pop();
	m_act = m_nss.Top();
}

int Linker::Link(const char * output)
{
	LinkRes link(output);
	link.Export(m_main);
	return 0;
}

Namespace::Obj* Namespace::Find(const char * name)
{
	for (uint i=0;i < m_obj.Count();i++)
	{
		if (m_obj[i].name == name)
			return &(m_obj[i]);
	}
	return NULL;
}

Namespace::Obj& Namespace::CreateObj()
{
	return m_obj.Add();
}

Namespace* Namespace::CreateNamespace(const char * name)
{
	return new (m_ns.AddForNew()) Namespace(name, m_pool);
}


// functions //
bool AddPictures(Linker* link, const Values& value)
{
	Scaner::Location loc = { "AddPictures", 0 };
	if (value.Count() != 1)
		return false;
	const HoeCore::CString name = value[0].GetStringValue();
	for (HoeUtils::FindFile f(name);f;++f)
	{

		const HoeCore::String n = HoeUtils::GetFileName(f.GetName(), false);
		Compiler * c = link->AddObject(n, HoeRes::Res::IDPicture, loc);
		c->AddProp("File", Value(f.GetName(), TK_string));
		c->Done();
	}

	return true;
}

bool Linker::Func(const HoeCore::CString name, const Values& value)
{
	if (name == T("AddPictures"))
	{
		return AddPictures(this, value);
	}
	throw HoeUtils::UnknownError(name, "function");
	return false;
}

