
#include "StdAfx.h"
#include "image.h"
#include "error.h"

Image::Image(void)
{
}

Image::~Image(void)
{
}

void Image::SetSource(const HoeCore::CString& str)
{
	m_filename = str;
	if (!m_f.Open(m_filename))
	{
		throw Error(T("Failed open file %s"),m_filename.GetPtr());
	}
}

void Image::Save(HoeCore::WriteStream &stream)
{
	if (!m_f.IsOpen())
		throw Error(T("No file specified."));

	m_f.Reset();
	stream.Write(m_f);
}


