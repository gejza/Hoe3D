#include "StdAfx.h"
#include "linker.h"
#include "error.h"
#include "scan.h"

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
	o.file.Open(fn, HoeCore::File::hftRewrite); // otevrit tmp soubor
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
	f.Open(output, HoeCore::File::hftRewrite);
	f.WriteString("// Autogenerate file by Hoe Resource Compiler\n\n");
	for (uint i=0;i < m_obj.Count();i++)
	{
		Obj& o = m_obj.Get(i);
		f.WriteString(o.name);
		f.WriteString("\tHOERES\t");
		f.WriteString(o.file.GetName());
		f.WriteString("\n"); 
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
bool AddPictures(Linker* link, const VectorUniversal& value)
{
	Scaner::Location loc = { "AddPictures", 0 };
	// search folder
	Compiler * c = link->AddObject("a", ERT_Picture, loc);
	c->AddProp("File", "eee.jpg");
	c->Done();
	c = link->AddObject("b", ERT_Picture, loc);
	c->AddProp("File", "eee.jpg");
	c->Done();
	c = link->AddObject("c", ERT_Picture, loc);
	c->AddProp("File", "eee.jpg");
	c->Done();

	return true;
}

bool Linker::Func(const HoeCore::CString name, const VectorUniversal& value)
{
	if (name == T("AddPictures"))
	{
		return AddPictures(this, value);
	}
	throw UnknownError(name, "function");
	return false;
}

