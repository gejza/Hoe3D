#include "StdAfx.h"
#include "linker.h"
#include "error.h"
#include "scan.h"
#include "parse.tab.hpp"

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

bool operator < (const HoeRes::Res::Symbol& a, const HoeRes::Res::Symbol& b)
{
	return HoeCore::string::cmp(a.name,b.name) < 0;
}

bool operator > (const HoeRes::Res::Symbol& a, const HoeRes::Res::Symbol& b)
{
	return HoeCore::string::cmp(a.name,b.name) > 0;
}

void Export(Namespace& ns, HoeCore::WriteStream& str, 
			const HoeCore::Endianness& end, bool expheader = true)
{
	// vypocitat poctet symbolu
	int numsymbols = ns.GetObj().Count() + ns.GetNumNS();
	if (expheader)
	{
		Res::Namespace head;
		memset(&head, 0, sizeof(head));
		head.id = HoeRes::Res::IDNamespace;
		head.num_symbols = end.num<uint32>(numsymbols);
		head.size_struct = end.num<uint16>(sizeof(head));
		head.version_struct = end.num<uint16>(1);
		str.Write(&head, sizeof(head));
	}
	if (numsymbols < 1)
		return;
	Res::Symbol * syms = (Res::Symbol *) str.CreateBuffer(sizeof(Res::Symbol)*numsymbols);
	memset(syms, 0, sizeof(Res::Symbol)*numsymbols);
	int nsym = 0;
	for (HoeCore::LList<Namespace>::Iterator it = ns.GetNS();it;it++)
	{
		Res::Symbol& sym = syms[nsym];
		memset(&sym,0, sizeof(sym));
		HoeCore::string::copy(sym.name, it->GetName(), sizeof(sym.name));
		sym.type = end.num<uint32>(HoeRes::Res::IDNamespace);
		sym.position = end.num<uint64>(str.Tell());
		Export(*it, str, end);
		nsym++;
	}
	// Export objs
	for (HoeCore::List<Namespace::Obj>::Iterator it(ns.GetObj());it;it++)
	{
		Res::Symbol& sym = syms[nsym];
		memset(&sym,0, sizeof(sym));
		HoeCore::string::copy(sym.name, it->name, sizeof(sym.name));
		sym.type = it->type;
		sym.position = str.Tell();
		// export obj
		it->file.Flush();
		it->file.Seek(0);
		str.Write(it->file);
		nsym++;
	}
	hoe_assert(nsym == numsymbols);
	HoeCore::qsort(syms, numsymbols);
}

int Linker::Link(const char * output)
{
	HoeCore::File f;
	HoeCore::Stream& s = f;
	HoeCore::Endianness end(HoeCore::Endianness::Low);
	f.Open(output, HoeCore::File::hftRewrite);

	//m_obj.QSort();
	Res::MainNamespace head;
	memset(&head, 0, sizeof(head));
	head.id = le_uint32(Res::IDHRESHEADER);
	head.size_struct = be_uint16(sizeof(head));
	head.version_struct = be_uint16(Res::IDHRESVER);
	head.flags = be_uint32((uint32)end.Get());
	head.num_symbols = be_uint32(m_main.GetObj().Count() + m_main.GetNumNS());
	s.Write(&head, sizeof(head));
	Export(m_main, f, end, false);
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

		Compiler * c = link->AddObject(f.Get().name, HoeRes::Res::IDPicture, loc);
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

