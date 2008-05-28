
#include "StdAfx.h"
#include "../include/HoeCore/hoe_platform.h"

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
#if _UNICODE
	en |= ByteChar;
#else
	en |= WChar;
#endif
	return en;
}

int HoeCore::Endianness::SetTextType(const byte* str)
{
	// scan bom
	switch (str[0])
	{
	case 0xEF:
		if (str[1] == 0xBB && str[2] == 0xBF)
		{
			m_end = (m_end & MaskChar);
			return 3;
		}
		break;
	case 0xFE:
		if (str[1] == 0xFF)
		{
			m_end = (m_end & (MaskChar|MaskEnd)) | WChar | Big;
			return 2;
		}
		break;
	case 0xFF:
		if (str[1] == 0xFE)
		{
			m_end = (m_end & (MaskChar|MaskEnd)) | WChar | Low;
			return 2;
		}
		break;
	};
	return 0; // nenalezeno
}

uint32 HoeCore::Endianness::Get() const
{ 
	return m_end; 
}

const tchar * HoeCore::Endianness::GetPlatformString(uint32 plat)
{
	switch (plat)
	{
	case Low:
		return T("low");
	case Big:
		return T("big");
	default:
		return T("mixed");
	};
}

const tchar * HoeCore::Endianness::GetPlatformString()
{
	return GetPlatformString(Get());
}



