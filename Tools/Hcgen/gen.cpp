
#include "StdAfx.h"
#include "gen.h"
#include "consts.h"

CGen::CGen(HoeCore::WriteStream& s) 
	: IndentStream(s) 
{
	
}

HGen::HGen(HoeCore::WriteStream& s, Consts& c) 
: CGen(s), m_c(c)
{
	Print(T("// generated const file\n"));
	// structures
	// values
	if (0)
	{
		Print(T("namespace %s {\n"), "franta");
		Indent(1);
	}
	// seradit podle zavislosti

	for (uint i=0;i< c.GetTypes().Count();i++)
	{
		const Type& t=c.GetTypes().Get(i);
		if (!t.isroot)
			continue;
		
		Print("extern ");
		TypeOut(t,t.name);
	}
	if (0) 
	{
		Indent(-1);
		Print(T("} // namespace %s\n"), "franta");
	}

	// generovat struktury ?

}

void HGen::StructOut(const Type& t)
{
	Print("struct %s_t\n", t.name.GetPtr());
	Print("{\n");
	Indent(1);
	for (uint j=0;j < t.str_child.Count();j++)
	{
		const HoeCore::String& name = t.str_child.Get(j);
		const Type& ti = m_c.GetTypes()[name];
		TypeOut(ti, name);
	}

	Indent(-1);
	Print("} ");
}

void HGen::TypeOut(const Type& ti, const HoeCore::String& name)
{
	if (ti.type == ti.TStruct)
	{
		StructOut(ti);
		//Print("%s_t ", name.GetPtr());
	}
	else if (ti.type == ti.TUser)
		Print("%s ", ti.usr_type.GetPtr());
	else if (ti.type == ti.TConst)
	{
		switch (ti.cst_type)
		{
		case HoeCore::Universal::TypeBool:
			Print("bool "); break;
		case HoeCore::Universal::TypeDecimal:
			Print("int "); break;
		case HoeCore::Universal::TypeFloat:
			Print("float "); break;
		case HoeCore::Universal::TypeString:
			Print("HoeCore::String "); break;
		case HoeCore::Universal::TypeUnsigned:
			Print("unsigned long "); break;
		default:
			Print("HoeCore::Universal ");
		};
	}

	Print("%s;\n", name.GetPtr());
}


CppGen::CppGen(HoeCore::WriteStream& s, Consts& c, const tchar* fn) 
: CGen(s), m_c(c)
{
	Print(T("// generated const file\n"));
	Print(T("#include \"StdAfx.h\"\n"));
	Print(T("#include \"%s.h\"\n\n"), fn);
	// structures
	// values
	if (0)
	{
		Print(T("namespace %s {\n"), "franta");
		Indent(1);
	}
	// seradit podle zavislosti

	for (uint i=0;i< c.GetTypes().Count();i++)
	{
		m_nl = 0;
		const Type& t=c.GetTypes().Get(i);
		if (!t.isroot)
			continue;
		
		if (t.type == t.TStruct)
		{
			m_nl += Print("%s_t %s = { ", t.name.GetPtr(), t.name.GetPtr());
			Indent(1);
		}
		if (t.type == t.TUser)
			m_nl += Print("%s %s = ", t.usr_type.GetPtr(), t.name.GetPtr());
		
		HoeCore::Stack<const tchar*> n;
		n.Push(t.name);
		TypeOut(t, n);
		// out 
		if (t.type == t.TStruct)
		{
			Indent(-1);
			m_nl += Print("}");
		}
		m_nl += Print(";\n");
	}
	if (0) 
	{
		Indent(-1);
		Print(T("} // namespace %s\n"), "franta");
	}

	// generovat struktury ?

}

void CppGen::TypeOut(const Type& t, HoeCore::Stack<const tchar*>& name)
{
	HoeCore::String_s<2048> n;
	n.Join(name, T("."));

	if (t.type == t.TStruct)
	{
		// out
		if (!t.isroot)
		{
			m_nl += Print("/* %s */ {", n.GetPtr());
		}
		for (uint j=0;j < t.str_child.Count();j++)
		{
			const HoeCore::String& np = t.str_child.Get(j);
			const Type& ti = m_c.GetTypes()[np];
			name.Push(np);
			TypeOut(ti, name);
			name.Pop();
		}
		if (!t.isroot)
		{
			m_nl += Print("},");
		}
		return;
	}
	const Value* v = m_c.GetValues().Find(n);
	if (t.type == t.TUser)
	{
		// predelat po tomto
			//Print("%s(", t.usr_type.GetPtr());
		m_nl += Print("{ ");
			if (v) for (uint i=0;i<v->param.Count();i++)
			{
				char buff[1024];
				v->param[i].Dump(buff, 1024);
				m_nl += Print(i ? ", %s":"%s", buff);
			}
			//Print("),"); 
			m_nl += Print("}, ");
	} else if (t.type == t.TConst)
	{
		if (v) 
		{
			char buff[1024];
			v->value.Dump(buff, 1024);
			m_nl += Print(v->value.GetType()==v->value.TypeString 
				? "\"%s\", ": "%s, ", buff);
		}
		else
			m_nl += Print("/*d*/0, ");
	}
	if (m_nl >= 55)
	{
		Print("\n\t");
		m_nl = 0;
	}
}
