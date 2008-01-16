
#include "StdAfx.h"
#include "image.h"

Image::Image(void)
{
}

Image::~Image(void)
{
}

void Image::SetSource(const HoeCore::CString& str)
{
	m_filename = str;
}

