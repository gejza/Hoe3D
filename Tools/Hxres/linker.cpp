#include "StdAfx.h"
#include "linker.h"
#include "error.h"
#include "scan.h"
#include "parse.tab.hpp"

using namespace HoeRes;

Linker::Linker(void)
{
}

Linker::~Linker(void)
{
}

Compiler * Linker::AddObject(const char * name, int type, const Scaner::Location& l)
{
	HoeCore::String n = m_ns;
	if (!n.IsEmpty())
		n += ':';
	n += name;
	Obj* prev;
	if (prev = Find(n))
		throw DefineError(n,prev);

	Obj& o = m_obj.Add();
	o.type = type;
	o.name = n;
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
	if (!m_ns.IsEmpty())
		m_ns += ':';
	m_ns += name;
}

void Linker::PopNamespace()
{
	int f = m_ns.FindLast(':');
	if (f < 0)
		m_ns = "";
	else
		m_ns[f] = '\0';
}

int Linker::Link(const char * output)
{
	HoeCore::File f;
	HoeCore::Stream& s = f;
	f.Open(output, HoeCore::File::hftRewrite);

	Res::MainNamespace head;
	memset(&head, 0, sizeof(head));
	head.id = Res::IDHRESHEADER;
	head.size_struct = be_uint16(sizeof(head));
	head.version_struct = be_uint16(Res::IDHRESVER);
	head.num_symbols = le_uint32(m_obj.Count());
	s.Write(&head, sizeof(head));
	for (uint i=0;i < m_obj.Count();i++)
	{
		Obj& o = m_obj.Get(i);
		Res::Symbol sym;
		memset(&sym,0, sizeof(sym));
		HoeCore::string::copy(sym.name, o.name, sizeof(sym.name));
	}
	return 0;
}

Linker::Obj* Linker::Find(const char * name)
{
	for (uint i=0;i < m_obj.Count();i++)
	{
		if (m_obj[i].name == name)
			return &(m_obj[i]);
	}
	return NULL;
}

// functions //
void e(int, int);

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

		Compiler * c = link->AddObject(f.Get().name, ERT_Picture, loc);
		c->AddProp("File", Value("eee.jpg", TK_string));
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

