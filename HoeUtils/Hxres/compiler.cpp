
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
void PictureCompiler::AddProp(const char* name, const char* value)
{
}

void PictureCompiler::AddProp(const char* name, double value)
{
}

void PictureCompiler::AddProp(const char* name, long long value)
{
}

void PictureCompiler::AddProp(const char* name, const VectorUniversal& value)
{
	for (uint i=0;i < value.Count();i++)
	{
		printf("%s = %d\n", name, value.Get(i).GetDecimal());
	}
}




