
#include "StdAfx.h"
#include "gen.h"

CGen::CGen(HoeCore::WriteStream& s) 
	: IndentStream(s) 
{
}

HGen::HGen(HoeCore::WriteStream& s, Consts& c) 
: CGen(s)
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

