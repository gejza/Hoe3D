
#include "StdAfx.h"
#include "consts.h"

Type& Consts::GetProperty(const ValueName& name)
{
	// najit property
	int i;
	for (i=0;i < name.Count()-1;i++)
	{
		Type& t = m_types[name[i]];
		if (t.type == Type::TNone)
			t.type = Type::TStruct;
		if (t.type != Type::TStruct)
		{
			HoeCore::String_s<2048> n;
			n.Join(name, T("."), i+1);
			throw HoeUtils::Error("Type %s is not struct.", n.GetPtr());
		}
		if (!t.str_child.Find(name[i+1]))
			t.str_child.Create(name[i+1]);
	}
	return m_types[name[i]];
}

void Consts::SetConst(const HoeCore::List<const tchar*>& name,
	const HoeCore::Universal& value) 
{
	hoe_assert(name.Count());
	HoeCore::String_s<2048> n;
	n.Join(name, T("."));
	m_types[name[0]].isroot = true;
	
	if (m_values.IsExist(n))
		throw HoeUtils::Error("redefinition");
	m_values[n].value = value; // hodnota

	Type& t = GetProperty(name);
	if (t.type == Type::TNone)
		t.type = Type::TConst;
	if (t.type != Type::TConst)
	{
		throw HoeUtils::Error("Property '%s' has incompatible type.", n.GetPtr());
	}
	t.cst_type = value.GetType();
}

void Consts::SetConst(const ValueName& name, const tchar* type, const Values& params)
{
	hoe_assert(name.Count());
	HoeCore::String_s<2048> n;
	n.Join(name, T("."));
	m_types[name[0]].isroot = true;

	if (m_values.IsExist(n))
		throw HoeUtils::Error("redefinition");
	m_values[n].param.Copy(params);

	Type& t = GetProperty(name);
	if (t.type == Type::TNone)
		t.type = Type::TUser;
	if (t.type != Type::TUser)
	{
		throw HoeUtils::Error("Property '%s' has incompatible type.", n.GetPtr());
	}
	t.usr_type = type;

}

bool Consts::GetConst(const tchar* name, HoeCore::Universal& value)
{
	if (!m_values.IsExist(name))
		return false;
	value = m_values[name].value;
	return true;
}


void Consts::ParseError(const tchar* err)
{
	throw HoeUtils::Error(T("Syntax error: %s"), err);
}

struct CT
{
	const tchar* p;
	int n;
} c_cts[] = {
	T("x"), 0,
	T("width"), 0,
	T("left"), 0,
	T("right"), 0,
	T("y"), 1,
	T("height"), 1,
	T("top"), 1,
	T("bottom"), 1,
	0, 0,
};

long Consts::PixelFunc(const ValueName& name, const tchar* type, int pos, long num)
{
	int p[2] = {0};
	if (type && HoeCore::string::cmp(T("THoeRect"),type)==0)
	{
		p[pos%2]++;
	}
	else
	{
		for (ValueName::CIterator i(name);i;i++)
		{
			for (CT* ct=c_cts;ct->p;ct++)
			{
				if (HoeCore::string::cmp(*i, ct->p) == 0)
				{
					p[ct->n]++;
				}
			}
		}
	}
	if ((p[0] && p[1]) || (!p[0] && !p[1]))
		throw HoeUtils::Error("Symbol px is ambigous");

	if (p[0])
		return num * 2;
	if (p[1])
		return num * 2;
	return num;
}









