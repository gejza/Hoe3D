
#include "StdAfx.h"
#include "../include/hoe_stream.h"
#include "../include/hoe_string.h"

HoeCore::BaseStream::BaseStream()
{
}

HoeCore::BaseStream::~BaseStream()
{
}

size_t HoeCore::WriteStream::WriteString(const HoeCore::CString str)
{
	return Write(str.GetPtr(), str.Length());
}

