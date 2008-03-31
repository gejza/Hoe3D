// Hcgen.cpp : Defines the entry point for the console application.
//

#include "StdAfx.h"
#include "consts.h"
#include "gen.h"

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

