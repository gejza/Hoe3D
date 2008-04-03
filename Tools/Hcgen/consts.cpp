
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








