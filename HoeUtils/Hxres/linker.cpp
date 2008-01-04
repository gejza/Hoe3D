#include "StdAfx.h"
#include "linker.h"

Linker::Linker(void)
{
}

Linker::~Linker(void)
{
}

Linker::ObjectData * Linker::CreateObject(const char * name, int type)
{
	return new Linker::ObjectData;
}

Linker::ObjectData * Linker::AddObject(const char * name, int type)
{
	Obj& o = m_obj.Add();
	memset(&o, 0, sizeof(o));
	o.name = m_ns;
	if (!o.name.IsEmpty())
		o.name += ':';
	o.name += name;
	o.type = type;
	o.obj = CreateObject(name, type);
	return o.obj;
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
