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
	HoeCore::File m_file;
public:
	HoeFlexFileEx(const char * n) : HoeFlexFile(m_file) 
	{
		m_file.Open(n);
	}
	virtual const HoeCore::String& GetIdentifier() const { return m_file.GetName(); }

};

// scan parameters
// pridat automatickou selekci sys float time
float SysFloatTime(void)
{
	return 0;
}

void scanns(int l, HoeCore::ReadStream* s, const HoeCore::Endianness& end)
{
	HoeRes::Res::Namespace main;
	s->Read(&main, sizeof(main));	
	uint32 nums = end.num<uint32>(main.num_symbols);
	for (uint32 i=0;i < nums;i++)
	{
		HoeRes::Res::Symbol sym;
		s->Read(&sym, sizeof(sym));
		uint32 id = end.num<uint32>(sym.type);
		printf("%d:%s: %x\n", l, sym.name, id);
		if (id == HoeRes::Res::IDNamespace)
			scanns(l+1,s->CreateReader((size_t)end.num<uint64>(sym.position)),end);
	}
	s->Close();
}

void testfile()
{
	HoeCore::File f;
	if (!f.Open("test.rc"))
		return;
	
	HoeCore::ReadStream * s = f.CreateReader(0);
	HoeRes::Res::MainNamespace main;
	s->Read(&main, sizeof(main));	
	uint32 nums = be_uint32(main.num_symbols);
	HoeCore::Endianness end(be_uint32(main.flags));
	printf("endianness: %s\n", end.GetPlatformString());
	for (uint32 i=0;i < nums;i++)
	{
		HoeRes::Res::Symbol sym;
		s->Read(&sym, sizeof(sym));
		uint32 id = end.num<uint32>(sym.type);
		printf("%s: %x\n", sym.name, id);
		if (id == HoeRes::Res::IDNamespace)
			scanns(1,s->CreateReader((size_t)end.num<uint64>(sym.position)),end);
	}
	s->Close();
}

int main(int argc, char* argv[])
{
	// parse
	// output = binary files + map

	HoeFlexFileEx fs("test.txt");
	Scaner s;
	s.Switch(fs);

	HoeCore::StringPool pool;
	Linker link(pool);
	
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
		fprintf(stderr, "%s(%d) : ", (const tchar*)s.GetIdentifier(), s.GetLine());
		fprintf(stderr, e.GetStr());
		fprintf(stderr, "\n");
	}

	// open 
	testfile();
	
	return res;
}

