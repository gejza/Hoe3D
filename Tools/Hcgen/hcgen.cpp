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

int main(int argc, char* argv[])
{
	// parse
	// output = binary files + map
	if (argc < 2)
	{
		fprintf(stderr, "hcgen <file>\n");
		return 1;
	}

	//yydebug = 1;
	HoeFlexFileEx fs(argv[1]);
	HoeCore::SetRootDir(HoeCore::GetBaseDir(argv[1]));
	HoeCore::ConstParserSAX parser;
	HoeCore::ConstParser p;
	p.Switch(fs);

	// nacist
	p.Parse(parser);


	return 0;
}

