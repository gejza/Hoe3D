
#include "StdAfx.h"
#include "compiler.h"

Compiler * Compiler::Create(HoeCore::String&, int type)
{
    switch (type)
    {
	case ERT_Picture:
		return new PictureCompiler();
	case ERT_Stream:
		return new StreamCompiler();
    default:
		hoe_assert(!"Unknown compiler type.");
        return NULL;
    };
}

//////////////////////////////////////////////////////////
// Compilers

// picture
void PictureCompiler::AddProp(const HoeCore::CString name, const HoeCore::CString value)
{
    if (name == "File")
        printf("Soubor ");
    printf("%s = %s\n", (const char*)name, (const char*)value);
}

void PictureCompiler::AddProp(const HoeCore::CString name, double value)
{
}

void PictureCompiler::AddProp(const HoeCore::CString name, long long value)
{
}

void PictureCompiler::AddProp(const HoeCore::CString name, const VectorUniversal& value)
{
}




