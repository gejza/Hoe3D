
#include "StdAfx.h"
#include "../include/HoeCore/hoe_stream.h"
#include "../include/HoeCore/hoe_string.h"
#include "../include/HoeCore/hoe_platform.h"

HoeCore::BaseStream::BaseStream()
{
}

HoeCore::BaseStream::~BaseStream()
{
}

size_t HoeCore::BaseStream::Tell() const
{
	return 0;
}

size_t HoeCore::WriteStream::WriteString(const HoeCore::CString str)
{
	return Write(str.GetPtr(), str.Length());
}

void * HoeCore::WriteStream::CreateBuffer(size_t)
{
	return 0;
}

void HoeCore::WriteStream::CreateSpace(size_t s)
{
	char empty[1024] = {0};
	while (s > 0)
	{
		size_t ntw = s;
		if (ntw > 1024) ntw = 1024;
		Write(empty, ntw);
		s -= ntw;
	}
}

size_t HoeCore::WriteStream::Write(ReadStream& str, size_t size)
{
	size_t writed = 0;
	char buff[1024];
	while (1)
	{
		size_t ntr = 1024;
		if (size && (size-writed) < 1024)
			ntr = size - writed;
		size_t rntr = str.Read(buff, ntr);
		if (!rntr)
			break;
		size_t rntw = Write(buff, rntr);
		writed += rntw;
		if (rntw != ntr || rntr != ntr)
			break;
	}
	return writed;
}

int HoeCore::WriteStream::Print(const char* fmt, ...)
{
	int ret;
	HoeCore::String str;
	va_list args;
	va_start(args, fmt);
	ret = str.vprintf(fmt, args);
	va_end(args);
	WriteString(str);
	return ret;

}

const HoeCore::Endianness& HoeCore::BaseStream::GetDataFormat() const
{
	static const HoeCore::Endianness end(HoeCore::Endianness::GetPlatform());
	return end;
}





