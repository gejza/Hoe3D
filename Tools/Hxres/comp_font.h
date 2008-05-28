
#ifndef _HXRES_COMPILER_FONT_H_
#define _HXRES_COMPILER_FONT_H_

#include "compiler.h"

class CaseFoldingMap
{
	struct CharFolding
	{
		wchar_t c1;
		wchar_t c2;
		CharFolding(wchar_t cc1)
		{
			c1 = cc1; c2 = 0;
		}
		const CharFolding& operator = (wchar_t cc2)
		{
			c2 = cc2;
			return *this;
		}
		bool operator == (wchar_t cc1) const
		{
			return cc1 == c1;
		}
	};
	// todo predelat na mapovani vice aliasu
	HoeCore::Map<CharFolding, wchar_t> m_charmap;
public:
	CaseFoldingMap(const tchar* file);
	wchar_t Get(wchar_t c)
	{
		if (!m_charmap.IsExist(c)) return 0;
		return m_charmap[c].c2;
	}
};

class FontCompiler : public Compiler
{
	class FontDef
	{
		HoeCore::List<HoeRes::Res::FontInfo::FD> m_chd;
		HoeCore::List<HoeRes::Res::FontInfo::FDA> m_chad;
	public:
		bool Load(const char* path);
		bool LoadAliases(CaseFoldingMap& map);
		void Write(HoeCore::WriteStream& out);
	} m_fontdef;
public:
	FontCompiler(HoeCore::WriteStream& out) : Compiler(out) {}
	virtual bool AddProp(const HoeCore::CString name, const Value& value);
	virtual bool AddProp(const HoeCore::CString name, const Values& value);
	virtual bool Func(const HoeCore::CString name, 
                      const HoeCore::CString ret,
                      const Values& value);
	virtual bool AddObject(const Compiler* cmp);

	virtual void Done();
};

#endif //  _HXRES_COMPILER_FONT_H_


