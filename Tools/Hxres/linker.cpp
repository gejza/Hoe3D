#include "StdAfx.h"
#include "linker.h"
#include "error.h"
#include "scan.h"
#include "parse.tab.hpp"
#include "linkout.h"

using namespace HoeRes;

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
	HoeCore::String fn = o.name;
	fn.Replace(':','_');
	fn += ".ors";
	o.file.Open(fn, HoeCore::File::hftTemp); // otevrit tmp soubor
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
		throw Error("No namespace to pop.");
	m_nss.Pop();
	m_act = m_nss.GetTop();
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
		if (f.Get().attrib & _A_SUBDIR)
			continue;

		const HoeCore::String n = HoeUtils::GetFileName(f.Get().name, false);
		Compiler * c = link->AddObject(n, HoeRes::Res::IDPicture, loc);
		c->AddProp("File", Value(f.Get().name, TK_string));
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
	throw UnknownError(name, "function");
	return false;
}

