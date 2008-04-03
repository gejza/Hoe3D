
#include "StdAfx.h"
#include "compiler.h"
#include "comp_font.h"

using namespace HoeCore;
using namespace HoeRes::Res;

bool PInterface::AddProp(const HoeCore::CString name, const Value& value)
{
    throw HoeUtils::UnknownError(name, "attribute");
    return false;
}

bool PInterface::AddProp(const HoeCore::CString name, const Values& value)
{
    throw HoeUtils::UnknownError(name, "attribute");
    return false;
}

bool PInterface::Func(const HoeCore::CString name, 
                  const HoeCore::CString ret,
                  const Values& value)
{
    throw HoeUtils::UnknownError(name, "function");
    return false;
}

bool PInterface::AddObject(const Compiler* cmp)
{
    throw HoeUtils::Error("Error add %s object.", 
		HoeRes::Res::GetTypeName(cmp->GetType()));
    return false;
}


void no_run(const HoeCore::CString str)
{
	throw HoeUtils::InternalError(str);
}

Compiler * Compiler::Create(HoeCore::String&, int type, HoeCore::WriteStream& s)
{
	Compiler * ret;
    switch (type)
    {
	case IDPicture:
		ret = new PictureCompiler(s); break;
	case IDStream:
		ret = new StreamCompiler(s); break;
	case IDFont:
		ret = new FontCompiler(s); break;
	case IDFile:
		ret = new FileCompiler(s); break;
    default:
		hoe_assert(!"Unknown compiler type.");
        return NULL;
    };
	ret->m_type = type;
	return ret;
}

//////////////////////////////////////////////////////////
// Compilers
bool Compiler::CheckArg(const CString name, const Universal& value, Universal::Type type, bool th)
{
	if (value.GetType() != type)
	{
		if (th)
		{
			throw HoeUtils::ConvertError(name, 
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
	if (name == "Source")
	{
		CheckArg(name, value, Universal::TypeString);
        im.SetSource(value.GetStringValue());
		return true;
	}
	if (name == "AlphaRef")
	{
		CheckArg(name, value, Universal::TypeDecimal);
        im.SetAlphaRef(value.GetUnsigned());
		return true;
	}
	return PInterface::AddProp(name, value);
}

bool PictureCompiler::AddProp(const HoeCore::CString name, const Values& value)
{
	return PInterface::AddProp(name, value);
}

bool PictureCompiler::Func(const HoeCore::CString name, 
                      const HoeCore::CString ret,
                      const Values& value)
{
	if (name == "FindColorKey")
	{
		return im.ComputeColorKey();
	}
	return PInterface::Func(name, ret, value);
}


void PictureCompiler::Done()
{
	HoeRes::Res::PictureInfo head;
	head.id = HoeRes::Res::IDPicture;
	head.size_struct = sizeof(head);
	head.version_struct = 1;

	head.codec = im.GetFormat();
	head.numchunk = 0;
	if (im.HasAlphaRef(0))
		head.numchunk++;
	if (im.HasColorKey(0))
		head.numchunk++;
	m_out.Write(&head, sizeof(head));
	
	HOECOLOR ck;
	if (im.HasColorKey(&ck))
	{
		// write color key
		HoeRes::Res::ChunkInfo chunk;
		memcpy(chunk.cid, "CKEY", 4);
		chunk.size = sizeof(HOECOLOR);
		m_out.WriteStruct(chunk);
		m_out.Write(&ck, sizeof(HOECOLOR));
	}

	byte ar;
	if (im.HasAlphaRef(&ar))
	{
		// write color key
		HoeRes::Res::ChunkInfo chunk;
		memcpy(chunk.cid, "AREF", 4);
		chunk.size = 1;
		m_out.WriteStruct(chunk);
		m_out.Write(&ar,1);
	}

	im.Save(m_out);

	// save constans
	HoeRes::PicCodec * c = im.GetDecoder();
	THoeSizeu res;
	c->GetSize(&res);
	AddConst(T("width"), (HoeCore::Universal::TDecimal)res.width);
	AddConst(T("height"), (HoeCore::Universal::TDecimal)res.height);
}

/*bool PictureCompiler::Func(const HoeCore::CString name, const VectorUniversal& value)
{
    printf("Running func %s width %d arguments.\n", (const tchar*)name, value.Count());
	return false;
}*/

/////

// picture
bool FileCompiler::AddProp(const CString name, const Value& value)
{
	if (name == "Source")
	{
		CheckArg(name, value, Universal::TypeString);
		if (!m_f.Open(value.GetStringValue()))
		{
			throw HoeUtils::Error(T("Failed open file %s"),value.GetStringValue());
		}
		return true;
	}
	return PInterface::AddProp(name, value);
}




void FileCompiler::Done()
{
	if (!m_f.Open())
		throw HoeUtils::Error(T("Property 'Source' must exist."));

	HoeRes::Res::FileInfo* head = (HoeRes::Res::FileInfo*)m_out.CreateBuffer(sizeof(HoeRes::Res::FileInfo));
	head->id = HoeRes::Res::IDFile;
	head->size_struct = sizeof(head);
	head->version_struct = 1;
	if (1)
	{
		HoeRes::StreamDeflate def(m_f, Z_DEFAULT_COMPRESSION);
		head->comp_size = m_out.Write(def);
		head->compress = HoeRes::Res::FileInfo::CSZlib;
	}
	else
	{
		head->comp_size = 0;
		head->compress = HoeRes::Res::FileInfo::CSPlain;
	}
	head->size = m_f.Tell();
}






