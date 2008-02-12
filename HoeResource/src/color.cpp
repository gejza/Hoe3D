
#include "StdAfx.h"
#include "../include/HoeResource/color.h"

template<int a,int r, int g, int b> struct ColorARGB
{
	enum {
		// ~
		BlueMask = dword((1 << b) - 1) /*& ~dword((1 << b) - 1)*/,
		GreenMask = dword((1 << (b+g)) - 1) & ~dword((1 << b) - 1),
		RedMask = dword((1 << (b+g+r)) - 1) & ~dword((1 << (b+g)) - 1),
		AlphaMask = dword((1 << (b+g+r+a)) - 1) & ~dword((1 << (b+g+r)) - 1),

		BlueShift = b - 8,
		GreenShift = (g+b) - 8,
		RedShift = (g+b+r) - 8,
		AlphaShift = (g+b+r+a) - 8,

		NumBits = (g+b+r+a),
	};
	static inline dword _fastcall conv2dw(const HOECOLOR& c)
	{
		dword dw = 0;
		if (a > 0)
			dw  |= safe_lshift(c.a & ~((1 << (8-a))-1),AlphaShift);
		return dw | safe_lshift(c.r & ~((1 << (8-r))-1),RedShift)
			| safe_lshift(c.g & ~((1 << (8-g))-1),GreenShift)
			| safe_lshift(c.b & ~((1 << (8-b))-1),BlueShift); 
	}
	static inline void _fastcall conv2c(HOECOLOR& c, const dword dw)
	{
		if (a > 0)
			c.a = safe_rshift(dw & AlphaMask, AlphaShift);
		else
			c.a = 0xff;
		c.r = safe_rshift(dw & RedMask, RedShift);
		c.g = safe_rshift(dw & GreenMask, GreenShift);
		c.b = safe_rshift(dw & BlueMask, BlueShift);
	}
};

HoeRes::ColorConv::ColorConv(HOEFORMAT from, HOEFORMAT to)
	: m_fromformat(from), m_toformat(to), m_colors(0), m_numcolors(0) 
{
	switch (from)
	{
	case HOE_R8G8B8:
		m_from = ColorARGB<0,8,8,8>::conv2c;
		m_fromnum = ColorARGB<0,8,8,8>::NumBits / 8;
		break;
	case HOE_P8:
		m_from = NULL;
		m_fromnum = 1;
		break;
	default:
		hoe_assert(!"Unknown format");
	};

	switch (to)
	{
	case HOE_R5G6B5:
		m_to = ColorARGB<0,5,6,5>::conv2dw;
		m_tonum = ColorARGB<0,5,6,5>::NumBits / 8;
		break;
	default:
		hoe_assert(!"Unknown format");
	};
}

bool HoeRes::ColorConv::Conv(byte* dest, byte* src)
{
	hoe_assert(m_to);
	HOECOLOR c;
	if (m_fromformat == HOE_P8)
	{
		hoe_assert(*src < m_numcolors);
		c = m_colors[*src];
	}
	else
	{
		hoe_assert(m_from);
		switch (m_fromnum)
		{
		case 1:
			m_from(c, src[0]); break;
		case 2:
			m_from(c, src[0] << 8 | src[1]); break;
		case 3:
			m_from(c, src[0] << 16 | src[1] << 8 | src[2]); break;
		case 4:
			m_from(c, src[0] << 24 | src[1] << 16 | src[2] << 8 | src[3]); break;
		}
	}

	dword dw = m_to(c);
	switch (m_tonum)
	{
	case 4:
		*dest++ = dw & 0xff; dw >>= 8;
	case 3:
		*dest++ = dw & 0xff; dw >>= 8;
	case 2:
		*dest++ = dw & 0xff; dw >>= 8;
	case 1:
		*dest++ = dw & 0xff; dw >>= 8;
	};
	return true;
}

dword HoeRes::CompileColor(HOEFORMAT fmt, const HOECOLOR& c)
{
	switch (fmt)
	{
	case HOE_R5G6B5:
		return ColorARGB<0,5,6,5>::conv2dw(c);
	};
	return 0;
}




