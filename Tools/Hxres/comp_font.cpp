
#include "StdAfx.h"
#include "comp_font.h"

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
	if (name == "ComplementFont")
	{
		// doplneni aliasu
		// no return value
		if (ret != "")
			throw HoeUtils::Error("Function ComplementFont not return value.");
		if (value.Count() != 1)
			throw HoeUtils::Error("Function ComplementFont requie one argument.");
		CaseFoldingMap cfm(value[0].GetStringValue());
		return m_fontdef.LoadAliases(cfm);
	}
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

	head.numchunk = 2;
	m_out.Write(&head, sizeof(head));
	
	m_fontdef.Write(m_out);
}

/*bool FontCompiler::Func(const HoeCore::CString name, const VectorUniversal& value)
{
    printf("Running func %s width %d arguments.\n", (const tchar*)name, value.Count());
	return false;
}*/

/////
bool FontCompiler::FontDef::Load(const char *path)
{
	HoeCore::File f;
	if (!f.Open(path))
	{
		throw HoeUtils::Error("Failed open file '%s'.", path);
	}
	// reader
	HoeCore::TextReadStream t(f);
	const tchar* p;
	int l=0;
	while (p = t.ReadLine())
	{
		l++;
		HoeRes::Res::FontInfo::FD& ch = m_chd.Add();
		ch.ch = HoeCore::string::utf2w(p);
		if (ch.ch == 0xfeff)
			ch.ch = HoeCore::string::utf2w(p);
		if (*p++ != ':')
			throw HoeUtils::Error("Missing ':' on line %d.", l);
		ch.size=HoeCore::string::GetNumber(p);
	}

	return true;
}

bool FontCompiler::FontDef::LoadAliases(CaseFoldingMap& map)
{
	// aliases
	for (uint i=0;i < m_chd.Count();i++)
	{
		wchar_t a = map.Get(m_chd[i].ch);
		if (!a)
			continue;
		// zjistit zda neni alias na pismeno ktere existuje
		for (uint j=0;j < m_chd.Count();j++)
			if (m_chd[j].ch == a)
				goto next_char;
		// zapsat alias
		{
			HoeRes::Res::FontInfo::FDA& fda = m_chad.Add();
			fda.index = i;
			fda.alias = a;
		}
next_char:
		1; // pokracovani
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

	// write aliases
	memcpy(chunk.cid, "DEFA", 4);
	chunk.size = m_chad.Count() * sizeof(HoeRes::Res::FontInfo::FDA);
	out.WriteStruct(chunk);
	out.Write(m_chad.GetBasePointer(), chunk.size);
}

const tchar* GotoBehind(const tchar* p, tchar c)
{
	while (*p && *p != c) p++;
	if (!*p) return p;
	return p+1;
}
////////////////////////////////////////////////
// font map
CaseFoldingMap::CaseFoldingMap(const tchar* path)
{
	HoeCore::File f;
	if (!f.Open(path))
	{
		throw HoeUtils::Error("Failed open file '%s'.", path);
	}
	// reader
	HoeCore::TextReadStream t(f);
	const tchar* p;
	while (p = t.ReadLine())
	{
		// load font def
		if (*p == T('#'))
			continue;
		int code = HoeCore::string::GetHex(p);
		p = GotoBehind(p, ';');
		// type
		while (*p == ' ') p++;
		if (*p != 'C' && *p != 'S')
			continue;
		
		p = GotoBehind(p, ';');
		p = GotoBehind(p, ' ');
		int alias = HoeCore::string::GetHex(p);
		if (code > 0xffff || alias > 0xffff)
			continue; // nepodporovane znaky mimo utf-16

		// pridat alias
		m_charmap[code] = alias;
		m_charmap[alias] = code;
	}

}








