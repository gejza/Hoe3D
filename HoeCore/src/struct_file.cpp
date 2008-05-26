
#include "StdAfx.h"
#include "../include/HoeCore/struct_file.h"

void HoeCore::StructParserSAX::Begin(const tchar* name)
{
	if (!m_fn.IsEmpty())
	{
		m_fns.Push(m_fn.Length());
		m_fn.concat(T("."));
	}
	m_fn.concat(name);
}

void HoeCore::StructParserSAX::End()
{
	if (m_fns.Count())
	{
		size_t r = m_fns.Pop();
		m_fn.Strip(r);
	}
	else
		m_fn = T("");
}


