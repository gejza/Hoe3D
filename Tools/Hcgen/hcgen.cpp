// Hcgen.cpp : Defines the entry point for the console application.
//

#include "StdAfx.h"

class HoeFlexFileEx : public HoeCore::HoeFlexFile
{
	HoeCore::File m_file;
public:
	HoeFlexFileEx(const tchar * n) : HoeFlexFile(m_file) 
	{
		if (!m_file.Open(n))
			throw HoeUtils::Error(T("Failed open file %s"), n);
	}
	virtual const HoeCore::String& GetIdentifier() const { return m_file.GetName(); }

};

// pridat automatickou selekci sys float time
float SysFloatTime(void)
{
	return 0;
}

//////////////////////////////////////////
struct Prop
{
	HoeCore::String name;
	HoeCore::Universal::Type type;
	HoeCore::Set<Prop*> prop;
};

struct Value
{
	HoeCore::String longname;
	HoeCore::Universal value;
};

struct Item
{
	HoeCore::String name;
	Prop* prop; // typ promene
	HoeCore::Set<Value> values; // hodnoty
	Item() : prop(0) {}
};
///////////////////////////////////////////

class Consts : public HoeCore::ConstParserSAX
{
	HoeCore::LList<Prop> m_types;
	HoeCore::List<Item> m_items;
public:
	virtual void SetConst(const HoeCore::List<const tchar*>& name,
		const HoeCore::Universal& value) 
	{
		hoe_assert(name.Count());
		HoeCore::String_s<2048> n;
		n.Join(name, T("."));
		
		printf("%s: %s\n", n.GetPtr(), value.GetStringValue());
		// tady uz muze vyhazovat vyjimky
		
		Item * i = FindItem(name[0]);
		Prop * p = FindProp(name[0]);
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
		}
		
	}
	virtual void ParseError(const tchar* err)
	{
		throw HoeUtils::Error(T("Parse error: %s"), err);
	}
	Item * FindItem(const tchar* name)
	{
		for (HoeCore::List<Item>::Iterator i=m_items;i;i++)
		{
			if (i->name == name)
				return &i;
		}
		return NULL;
	}
	Prop * FindProp(const tchar* name)
	{
		for (HoeCore::LList<Prop>::Iterator i=m_types;i;i++)
		{
			if (i->name == name)
				return &i;
		}
		return NULL;
	}
};

class TextProcessor : public HoeCore::WriteStream
{
protected:
	HoeCore::WriteStream& m_out;
	bool m_nl;
public:
	TextProcessor(HoeCore::WriteStream& out) : m_out(out), m_nl(true) {}
	virtual uint Close() { return m_out.Close(); }
	virtual size_t Write(const void* ptr, size_t size)
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
	virtual size_t PreLine()
	{
		return 0;
	}
};

class IndentStream : public TextProcessor
{
	int m_level;
public:
	IndentStream(HoeCore::WriteStream& s) : TextProcessor(s), m_level(0) {}
	virtual size_t PreLine()
	{
		if (m_level)
			return m_out.Write(T("\t\t\t\t\t\t\t\t\t\t"), sizeof(tchar)*m_level);
		return 0;
	}
	void Indent(int l)
	{
		m_level += l;
		if (m_level < 0) m_level = 0;
	}
};

class CGen : public IndentStream
{
public:
	CGen(HoeCore::WriteStream& s) : IndentStream(s) {}
};

class HGen : public CGen
{
public:
	HGen(HoeCore::WriteStream& s, Consts& c) : CGen(s)
	{
		Print(T("// generated const file\n"));
		// structures
		// values
		if (1)
		{
			Print(T("namespace %s {\n"), "franta");
			Indent(1);
		}
		Print("struct\n");
		Print("{\n// struktura\n// dalsi cement");
		Indent(1);
		Print("int a;\n");
		Indent(-1);
		Print("}\n");
		if (1) 
		{
			Indent(-1);
			Print(T("} // namespace %s\n"), "franta");
		}
	}
};

/*
const - bez parseru
universal - universalni hodnoty
traditional
strict
*/

// hodnoty: univerzal
// parser: y n
// rozsirit struktury
// sdileni typu struktur
// misto struktur namespace
// main namespace
// use stdafx

int main(int argc, char* argv[])
{
	// parse
	// output = binary files + map
	if (argc < 2)
	{
		fprintf(stderr, "hcgen <file>\n");
		return 1;
	}

	try {
		//yydebug = 1;
		HoeFlexFileEx fs(argv[1]);
		HoeCore::SetRootDir(HoeCore::GetBaseDir(argv[1]));
		Consts parser;
		HoeCore::ConstParser p;
		p.Switch(fs);

		// nacist
		p.Parse(parser);

		// generate
		HoeCore::String_s<256> o;
		o.printf(T("%s.h"), HoeUtils::GetFileName(argv[1],false).GetPtr());
		HoeCore::File fo;
		if (!fo.Open(o, HoeCore::File::hftRewrite))
			throw HoeUtils::Error(T("Failed open file %s for write."), o.GetPtr());
		HGen(fo, parser);

	} catch (const HoeUtils::Error& err)
	{
		fprintf(stderr, "%s\n", err.GetStr().GetPtr());
		return 1;
	}

	return 0;
}

