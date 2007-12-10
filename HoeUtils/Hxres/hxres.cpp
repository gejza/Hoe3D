// Hxres.cpp : Defines the entry point for the console application.
//

#include "StdAfx.h"
#include "scan.h"
//class scaner
//class parser
//  class compiler

int yyparse(Scaner * lex);


int main(int argc, char* argv[])
{
	// parse
	FILE * f = fopen("test.txt","r");
	if (!f)
		return 1;
	HoeCore::HoeFlexFile fs(f);
	Scaner s;
	s.Switch(fs);
	yyparse(&s);

	fclose(f);
	return 0;
}

