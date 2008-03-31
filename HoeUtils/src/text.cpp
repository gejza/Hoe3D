
#include "StdAfx.h"
#include "../include/HoeUtils/text.h"

HoeUtils::TextProcessor::TextProcessor(HoeCore::WriteStream& out) 
	: m_out(out), m_nl(true) 
{
}

uint HoeUtils::TextProcessor::Close() 
{ 
	return m_out.Close(); 
}

size_t HoeUtils::TextProcessor::PreLine()
{
	return 0;
}

size_t HoeUtils::TextProcessor::Write(const void* ptr, size_t size)
{
	size_t w = 0;
	const tchar* p = (const tchar*)ptr;
	size /= sizeof(tchar);
nl:
	if (!size) return w;
	if (m_nl) w += PreLine();
	m_nl = false;
	for (size_t i=0;i < size;i++)
	{
		if (p[i] == T('\n'))
		{
			i++;
			w += m_out.Write(p, i*sizeof(tchar));
			p += i; size -= i;
			m_nl = true;
			goto nl;
		}
	}
	return m_out.Write(p, size*sizeof(tchar)) + w;
}

//// indent stream

HoeUtils::IndentStream::IndentStream(HoeCore::WriteStream& s) 
	: TextProcessor(s), m_level(0) 
{
}

size_t HoeUtils::IndentStream::PreLine()
{
	if (m_level)
		return m_out.Write(T("\t\t\t\t\t\t\t\t\t\t"), sizeof(tchar)*m_level);
	return 0;
}

void HoeUtils::IndentStream::Indent(int l)
{
	m_level += l;
	if (m_level < 0) m_level = 0;
}
