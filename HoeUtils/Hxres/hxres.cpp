// Hxres.cpp : Defines the entry point for the console application.
//

#include "StdAfx.h"
#include "scan.h"
#define _NOXLIB
#include "linker.h"
//class scaner
//class parser
//  class compiler

int yyparse(Scaner * lex);

class HoeFlexFileEx : public HoeCore::HoeFlexFile
{
	HoeCore::String_s<1024> name;
public:
	HoeFlexFileEx(const char * n) : HoeFlexFile(fopen(n,"rt")), name(n) {}
	virtual ~HoeFlexFileEx() { fclose(file); }
	virtual const char * GetIdentifier() { return name; }

};

int main(int argc, char* argv[])
{
	// parse
	// output = binary files + map

	HoeFlexFileEx fs("test.txt");
	Scaner s;
	s.Switch(fs);

	Linker link;
	int res = yyparse(&s);
	if (!res)
	{
		printf("Linking...\n");
		res = link.Link("test.rc");
	}
	
	return res;
}

