
#include "StdAfx.h"
#include "comp_font.h"
#include "error.h"

using namespace HoeCore;
using namespace HoeRes::Res;

bool FontCompiler::AddProp(const CString name, const Value& value)
{
	/*if (name == "File")
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
	}*/
	return PInterface::AddProp(name, value);
}

bool FontCompiler::AddProp(const HoeCore::CString name, const Values& value)
{
	return PInterface::AddProp(name, value);
}

bool FontCompiler::Func(const HoeCore::CString name, 
                      const HoeCore::CString ret,
                      const Values& value)
{
	/*if (name == "FindColorKey")
	{
		return im.ComputeColorKey();
	}*/
	return PInterface::Func(name, ret, value);
}


void FontCompiler::Done()
{
	/*HoeRes::Res::PictureInfo head;
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

	im.Save(m_out);*/
}

/*bool FontCompiler::Func(const HoeCore::CString name, const VectorUniversal& value)
{
    printf("Running func %s width %d arguments.\n", (const tchar*)name, value.Count());
	return false;
}*/

/////






