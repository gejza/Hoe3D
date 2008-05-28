
#include "StdAfx.h"
#include "../include/HoeCore/hoe_stream.h"
#include "../include/HoeCore/hoe_string.h"
#include "../include/HoeCore/hoe_platform.h"

HoeCore::BaseStream::BaseStream()
{
}

HoeCore::BaseStream::~BaseStream()
{
}

size_t HoeCore::BaseStream::Tell() const
{
	return 0;
}

size_t HoeCore::WriteStream::WriteString(const HoeCore::CString str)
{
	return Write(str.GetPtr(), str.Length()*sizeof(tchar));
}

void * HoeCore::WriteStream::CreateBuffer(size_t)
{
	return 0;
}

void HoeCore::WriteStream::CreateSpace(size_t s)
{
	char empty[1024] = {0};
	while (s > 0)
	{
		size_t ntw = s;
		if (ntw > 1024) ntw = 1024;
		Write(empty, ntw);
		s -= ntw;
	}
}

size_t HoeCore::WriteStream::Write(ReadStream& str, size_t size)
{
	size_t writed = 0;
	char buff[1024];
	while (1)
	{
		size_t ntr = 1024;
		if (size && (size-writed) < 1024)
			ntr = size - writed;
		size_t rntr = str.Read(buff, ntr);
		if (!rntr)
			break;
		size_t rntw = Write(buff, rntr);
		writed += rntw;
		if (rntw != rntr)
			break;
	}
	return writed;
}

int HoeCore::WriteStream::Print(const tchar* fmt, ...)
{
	int ret;
	HoeCore::String str;
	va_list args;
	va_start(args, fmt);
	ret = str.vprintf(fmt, args);
	va_end(args);
	WriteString(str);
	return ret;

}

const HoeCore::Endianness& HoeCore::BaseStream::GetDataFormat() const
{
	static const HoeCore::Endianness end(HoeCore::Endianness::GetPlatform());
	return end;
}

////////////////////////////////////
HoeCore::BufferStream::BufferStream(ReadStream& stream) 
	: m_stream(stream), m_usedynamic(false), 
	m_first(0), m_end(0), m_countdynamic(0), m_eos(false)
{
}

void HoeCore::BufferStream::Shift(size_t size)
{
	size_t s = m_end - m_first;
	if (size <= s)
		m_first += size;
	else
	{
		m_first = m_end = 0;
		m_stream.Skip(size-s);
	}
	if (m_first == m_end)
	{
		m_first = m_end = 0;
		m_usedynamic = false;
	}
}

byte* HoeCore::BufferStream::ReadNext(size_t size)
{
	if (m_eos) return NULL;
	// nacist dalsi buffer, v bufferu musi byt navic dalsi size
	// pokud je prazdny
	if (m_first && m_first == m_end)
	{
		m_first = m_end = 0;
		m_usedynamic = false;
		m_dynbuff.Free();
	}
	// pokud vysledek se nevejde do statickeho bufferu
	// presun smerem do dynamickeho
	if (!m_usedynamic && (m_end-m_first+size) > StaticSize)
	{
		m_dynbuff.GetPtr(m_end-m_first+size);
		// zkopirovat data
		if (m_end > m_first)
		{
			memcpy(m_dynbuff.GetPtr(), m_buff.Get() + m_first, m_end-m_first);
			m_end -= m_first;
			m_first = 0;
		}
		m_usedynamic = true;
	}
	// pokud je vysledek mensi nez staticky, presun do statickeho
	if (m_usedynamic && (m_end-m_first+size) <= StaticSize)
	{
		if (m_end > m_first)
		{
			memcpy(m_buff.Get(), m_dynbuff.GetPtr() + m_first, m_end-m_first);
			m_end -= m_first;
			m_first = 0;
		}
		m_usedynamic = false;
		m_countdynamic++;
		if ((m_end+size) < StaticSize >> 1 && m_countdynamic < 3) // optimalizovat
			m_dynbuff.Free();
	}
	// pokud je nejake misto na zacatku, presunout na zacatek (pokud se tim uvolni misto)
	if (m_first && (StaticSize-m_end) < size)
	{
		// posunout buffer
		m_end-=m_first;
		if (m_usedynamic)
			m_dynbuff.Shift(m_first, m_end);
		else
			m_buff.Shift(m_first, m_end);
		m_first = 0;
	}
	// nacist zbytek bufferu
	if (m_usedynamic)
	{
		size_t abssize = m_end+size;
		m_dynbuff.Realloc(abssize);
		// nacist cely zbytek bufferu
		if (abssize < m_dynbuff.GetSize()) size = m_dynbuff.GetSize()-m_end;
		size_t r = m_stream.Read(m_dynbuff.GetPtr()+m_end, size);
		if (!r) m_eos = true;
		m_end += r;
	}
	else // staticky buffer
	{
		hoe_assert((m_end+size) <= StaticSize);
		size = StaticSize-m_end;
		size_t r = m_stream.Read(m_buff.Get()+m_end, size);
		if (!r) m_eos = true;
		m_end += r;
	}

	return GetSize() ? GetPtr():NULL;
}


////////////////////////////////////
HoeCore::TextReadStream::TextReadStream(ReadStream& stream) 
	: m_end(0), m_stream(stream), m_eos(false)
{
	int s = m_end.SetTextType(m_stream.GetPtr());
	if (s) m_stream.Shift(s);
}

inline bool IsEnd(const char* str)
{
	return !*str || *str == '\n' || *str == '\r';
}

const char* HoeCore::TextReadStream::ReadLine()
{
	if (m_eos) return NULL;
	// find end
	int off=0;
	char* p = (char*)m_stream.GetPtr();
	do {
		size_t s = m_stream.GetSize();
		for (;off < s;off++)
		{
			if (IsEnd(p+off))
			{
				if (p[off] == '\0') m_eos = true;
				if (p[off] == '\r' && p[off+1] == '\n')
					m_stream.Shift(off+2);
				else
					m_stream.Shift(off+1);
				p[off] = '\0'; // ukoncit radku
				return p;
			}
		}
		// konec radky nenalezen, vyzadat nove data
		p = (char*)m_stream.ReadNext(off+1/* jen optimalizace */);
	} while (p);
	m_eos = true;
	if (off)
	{
		p = (char*)m_stream.GetPtr();
		p[off] = '\0';
		return p;
	}
	return NULL;
}









