#include "StdAfx.h"
#include "linker.h"

Linker::Linker(void)
{
}

Linker::~Linker(void)
{
}

Compiler * Linker::AddObject(const char * name, int type)
{
	Obj& o = m_obj.Add();
	o.name = m_ns;
	if (!o.name.IsEmpty())
		o.name += ':';
	o.name += name;
	o.type = type;
	HoeCore::String fn = o.name;
	fn.Replace(':','_');
	o.file.Open(fn); // otevrit tmp soubor
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
}

int Linker::Link(const char * output)
{
	FILE * f = fopen(output, "wt");
	fprintf(f, "// Autogenerate file by Hoe Resource Compiler\n\n");
	for (int i=0;i < m_obj.Count();i++)
	{
		Obj& o = m_obj.Get(i);
		fprintf(f, "%s\tHOERES\tsoubor.txt\n", (const char*)o.name); 
	}
	fclose(f);
	return 0;
}
