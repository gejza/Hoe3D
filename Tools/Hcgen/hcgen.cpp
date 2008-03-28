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

class Consts : public HoeCore::ConstParserSAX
{
public:
	virtual void SetConst(const HoeCore::List<const tchar*>& name,
		const HoeCore::Universal& value) 
	{
		HoeCore::String_s<2048> n;
		n.Join(name, T("."));
		
		printf("%s: %s\n", n.GetPtr(), value.GetStringValue());
		// tady uz muze vyhazovat vyjimky
	}
	virtual void ParseError(const tchar* err)
	{
		throw HoeUtils::Error(T("Parse error: %s"), err);
	}
};

class Gen
{
protected:
	HoeCore::WriteStream& m_out;
	int m_level;
public:
	Gen(HoeCore::WriteStream& s) : m_out(s), m_level(0) {}
	void Tab()
	{
		if (m_level)
			m_out.Write(T("\t\t\t\t\t\t\t\t\t\t"), sizeof(tchar)*m_level);
	}
	void Indent(int l)
	{
		m_level += l;
		if (m_level < 0) m_level = 0;
	}
};

class HGen : public Gen
{
public:
	HGen(HoeCore::WriteStream& s, Consts& c) : Gen(s)
	{
		s.Print(T("// generated const file\n"));
		// structures
		// values
		
	}
};

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

