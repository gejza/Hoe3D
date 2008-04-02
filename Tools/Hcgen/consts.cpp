
#include "StdAfx.h"
#include "consts.h"

void Consts::SetConst(const HoeCore::List<const tchar*>& name,
	const HoeCore::Universal& value) 
{
	hoe_assert(name.Count());
	HoeCore::String_s<2048> n;
	n.Join(name, T("."));
	
	Item& i = m_items[name[0]];

	printf("-\n");
	for (ItemMap::Iterator ii(m_items);ii;ii++)
	{
		printf("n:%s\n", ii->name.GetPtr());
	}
	/*Prop * p = FindProp(name[0]);
	if (!i)
	{
		i = &m_items.Add();
		i->name = name[0];
		if (!p)
		{
			p = &m_types.Add();
			p->name = i->name;
		}
		i->prop = p;
	}
	if (!p)
		throw HoeUtils::InternalError("Property not exist");
	if (name.Count() == 1)
	{
		if (i->values.Count())
			throw HoeUtils::Error(T("`%s' redefined."), name[0]);
		Value& v = i->values.Add();
		v.longname = i->name;
		v.value = value;
		return;
	}*/
	
}

void Consts::ParseError(const tchar* err)
{
	throw HoeUtils::Error(T("Parse error: %s"), err);
}








