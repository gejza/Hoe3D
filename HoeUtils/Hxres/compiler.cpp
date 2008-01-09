
#include "StdAfx.h"
#include "compiler.h"
#include "error.h"

using namespace HoeCore;

Compiler * Compiler::Create(HoeCore::String&, int type)
{
    switch (type)
    {
	case ERT_Picture:
		return new PictureCompiler(*new File);
	case ERT_Stream:
		return new StreamCompiler(*new File);
    default:
		hoe_assert(!"Unknown compiler type.");
        return NULL;
    };
}

//////////////////////////////////////////////////////////
// Compilers
bool CheckArg(const Universal& value, Universal::Type p_num[])
{
	return false;
}

// picture
bool PictureCompiler::AddProp(const CString name, const Universal& value)
{
	throw Error("chybicka 2");

	printf("cannot convert from %s to %s\n", 
		HoeCore::Universal::GetTypeName(value.GetType()),
		HoeCore::Universal::GetTypeName(HoeCore::Universal::TypeDecimal));

    if (name == "File")
	{
        im.SetSource(value.GetStringValue());
		return true;
	}
	else
		printf("%s = %s\n", (const char*)name, (const char*)value);
	return false;
}

bool PictureCompiler::AddProp(const HoeCore::CString name, const VectorUniversal& value)
{
	return false;
}

bool PictureCompiler::Func(const HoeCore::CString name, const VectorUniversal& value)
{
    printf("Running func %s width %d arguments.\n", (const tchar*)name, value.Count());
	return false;
}




