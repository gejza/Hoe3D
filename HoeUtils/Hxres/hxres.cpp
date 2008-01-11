// Hxres.cpp : Defines the entry point for the console application.
//

#include "StdAfx.h"
#include "scan.h"
#define _NOXLIB
#include "linker.h"
#include "error.h"
//class scaner
//class parser
//  class compiler

int yyparse(Linker& linker, HoeCore::StringPool& pool, Scaner& lex);

class HoeFlexFileEx : public HoeCore::HoeFlexFile
{
	HoeCore::String_s<1024> name;
public:
	HoeFlexFileEx(const char * n) : HoeFlexFile(fopen(n,"rt")), name(n) {}
	virtual ~HoeFlexFileEx() { fclose(file); }
	virtual const char * GetIdentifier() { return name; }

};

// scan parameters
// pridat automatickou selekci sys float time
float SysFloatTime(void)
{
	return 0;
}

int main(int argc, char* argv[])
{
	// parse
	// output = binary files + map

	HoeFlexFileEx fs("test.txt");
	Scaner s;
	s.Switch(fs);

	Linker link;
	HoeCore::StringPool pool;
	
	int res = 0;
	try {
		res = yyparse(link,pool,s);
		if (!res)
		{
			printf("Linking...\n");
			res = link.Link("test.rc");
		}
	} catch (const Error& e)
	{
			fprintf(stderr, "%s(%d) : ", s.GetIdentifier(), s.GetLine());
			fprintf(stderr, e.GetStr());
			fprintf(stderr, "\n");
	}
	
	return res;
}

