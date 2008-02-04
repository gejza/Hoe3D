
#include "StdAfx.h"
#include "compiler.h"
#include "error.h"

using namespace HoeCore;
using namespace HoeRes::Res;

void no_run(const HoeCore::CString str)
{
	throw InternalError(str);
}

Compiler * Compiler::Create(HoeCore::String&, int type, HoeCore::WriteStream& s)
{
    switch (type)
    {
	case IDPicture:
		return new PictureCompiler(s);
	case IDStream:
		return new StreamCompiler(s);
    default:
		hoe_assert(!"Unknown compiler type.");
        return NULL;
    };
}

//////////////////////////////////////////////////////////
// Compilers
bool CheckArg(const CString name, const Universal& value, Universal::Type type, bool th=true)
{
	if (value.GetType() != type)
	{
		if (th)
		{
			throw ConvertError(name, 
				value.GetTypeName(),
				Universal::GetTypeName(type));
		}
		return false;
	}
	return true;
}

// picture
bool PictureCompiler::AddProp(const CString name, const Value& value)
{
	if (name == "File")
	{
		CheckArg(name, value, Universal::TypeString);
        im.SetSource(value.GetStringValue());
		return true;
	}
	else
		throw UnknownError(name, "attribute");
	return false;
}

bool PictureCompiler::AddProp(const HoeCore::CString name, const Values& value)
{
	return false;
}

void PictureCompiler::Done()
{
	HoeRes::Res::PictureInfo head;
	head.id = HoeRes::Res::IDPicture;
	head.size_struct = sizeof(head);
	head.version_struct = 1;

	head.format = im.GetFormat();
	head.codec = im.GetFormat();
	m_out.Write(&head, sizeof(head));
	
	im.Save(m_out);
}

/*bool PictureCompiler::Func(const HoeCore::CString name, const VectorUniversal& value)
{
    printf("Running func %s width %d arguments.\n", (const tchar*)name, value.Count());
	return false;
}*/

/////






