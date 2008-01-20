
#include "StdAfx.h"
#include "../include/hoe_platform.h"

HoeCore::Endianness::Endianness(uint32 e)
{
	m_end = e;
}

HoeCore::Endianness::Endianness(const Endianness& e)
{
	m_end = e.m_end;
}

uint32 HoeCore::Endianness::GetPlatform()
{
	uint32 en = 0;
#if LOW_ENDIAN
	en |= LowNum;
#else
	en |= BigNum;
#endif
#if LOW_IEEE_ENDIAN
	en |= LowIEEE;
#else
	en |= BigIEEE;
#endif
	return en;
}

uint32 HoeCore::Endianness::Get() const
{ 
	return m_end; 
}

const char * HoeCore::Endianness::GetPlatformString(uint32 plat)
{
	switch (plat)
	{
	case Low:
		return "low";
	case Big:
		return "big";
	default:
		return "mixed";
	};
}

const char * HoeCore::Endianness::GetPlatformString()
{
	return GetPlatformString(Get());
}



