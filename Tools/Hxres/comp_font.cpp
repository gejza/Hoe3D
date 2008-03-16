
#include "StdAfx.h"
#include "comp_font.h"
#include "error.h"

using namespace HoeCore;
using namespace HoeRes::Res;

bool FontCompiler::AddProp(const CString name, const Value& value)
{
	if (name == "FontDef")
	{
		CheckArg(name, value, Universal::TypeString);
		return m_fontdef.Load(value);
	}
	/*if (name == "AlphaRef")
	{
		CheckArg(name, value, Universal::TypeDecimal);
        im.SetAlphaRef(value.GetUnsigned());
		return true;
	}*/
	return PInterface::AddProp(name, value);
}

bool FontCompiler::AddProp(const HoeCore::CString name, const Values& value)
{
	return PInterface::AddProp(name, value);
}

bool FontCompiler::Func(const HoeCore::CString name, 
                      const HoeCore::CString ret,
                      const Values& value)
{
	/*if (name == "FindColorKey")
	{
		return im.ComputeColorKey();
	}*/
	return PInterface::Func(name, ret, value);
}

bool FontCompiler::AddObject(const Compiler* cmp)
{
	switch (cmp->GetType())
	{
	case HoeRes::Res::IDPicture:

		return true;
	};
	return PInterface::AddObject(cmp);
}

void FontCompiler::Done()
{
	HoeRes::Res::FontInfo head;
	head.id = HoeRes::Res::IDFont;
	head.size_struct = sizeof(head);
	head.version_struct = 1;

	head.numchunk = 1;
	m_out.Write(&head, sizeof(head));
	
	m_fontdef.Write(m_out);
}

/*bool FontCompiler::Func(const HoeCore::CString name, const VectorUniversal& value)
{
    printf("Running func %s width %d arguments.\n", (const tchar*)name, value.Count());
	return false;
}*/

class TextReader
{
	HoeCore::ReadStream& m_stream;
	char m_buff[1000];
	int m_from, m_max;
public:
	TextReader(HoeCore::ReadStream& stream) : m_stream(stream), m_from(0), m_max(0)
	{
		m_from = 0;
		m_max = m_stream.Read(m_buff, sizeof(m_buff)-1);
		m_buff[m_max] = 0;
		if (m_max >= 3 && m_buff[0] == 0xef && m_buff[1] == 0xbb && m_buff[2] == 0xbf)
			m_from = 3;
	}
	int FindEnd()
	{
		for (int i=m_from;i < m_max;i++)
		{
			switch (m_buff[i])
			{
			case '\n':
			case '\r':
				return i;
			};
		}
		return -1;
	}
	template<typename TYPE> bool ReadLine(TYPE& str)
	{
		str = "";
		while (1)
		{
			int i=FindEnd();
			if (i >= 0) // todo
			{
				bool crlf = m_buff[i] == '\r' && m_buff[i+1] == '\n';
				m_buff[i] = 0;
				str.concat(m_buff+m_from);
				m_from = i+1;
				if (crlf) m_from++;
				return true;
			}
			if (m_from < m_max)
				str.concat(m_buff[m_from]);
			m_from = 0;
			m_max = m_stream.Read(m_buff, sizeof(m_buff)-1);
			m_buff[m_max] = 0;
			if (!m_max)
				return !str.IsEmpty();
		}
	}
};
/////
bool FontCompiler::FontDef::Load(const char *path)
{
	HoeCore::File f;
	if (!f.Open(path))
	{
		throw Error("Failed open file '%s'.", path);
	}
	// reader
	TextReader t(f);
	HoeCore::String_s<100> line;
	int l=0;
	while (t.ReadLine(line))
	{
		l++;
		const char* p = line.GetPtr();
		HoeRes::Res::FontInfo::FD& ch = m_chd.Add();
		ch.ch = HoeCore::string::utf2w(p);
		if (ch.ch == 0xfeff)
			ch.ch = HoeCore::string::utf2w(p);
		if (*p++ != ':')
			throw Error("Missing ':' on line %d.", l);
		ch.size=HoeCore::string::GetNumber(p);
	}

	return true;
}

void FontCompiler::FontDef::Write(HoeCore::WriteStream& out)
{
	HoeRes::Res::ChunkInfo chunk; // todo dodelat endianes
	memcpy(chunk.cid, "DEF ", 4);
	chunk.size = m_chd.Count() * sizeof(HoeRes::Res::FontInfo::FD);
	out.WriteStruct(chunk);
	out.Write(m_chd.GetBasePointer(), chunk.size);
}







