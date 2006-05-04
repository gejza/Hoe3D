
#include "system.h"
#include "utils.h"
#include "ref.h"
#include "hoe_format.h"

const char * HoeFormatString(HOEFORMAT format)
{
    switch (format)
	{
	case HOE_A8R8G8B8: return "A8R8G8B8";
	case HOE_X8R8G8B8: return "X8R8G8B8";
	case HOE_DXT1: return "DXT1";
	case HOE_DXT2: return "DXT2";
	case HOE_DXT3: return "DXT3";
	case HOE_DXT4: return "DXT4";
	case HOE_DXT5: return "DXT5";
	case HOE_R8G8B8: return "R8G8B8";
	case HOE_R5G6B5: return "R5G6B5";
	case HOE_X1R5G5B5: return "X1R5G5B5";
	case HOE_A1R5G5B5: return "A1R5G5B5";
	case HOE_A8R3G3B2: return "A8R3G3B2";
	case HOE_X4R4G4B4: return "X4R4G4B4";
	case HOE_L16: return "L16";
	case HOE_A4R4G4B4: return "A4R4G4B4";
	case HOE_R3G3B2: return "R3G3B2";
	case HOE_A8: return "A8";
	case HOE_A8L8: return "A8L8";
	default:
		return "UNKNOWN";
	}
}

int HoeFormatSize(HOEFORMAT format)
{
    switch (format)
	{
	case HOE_A8R8G8B8:
	case HOE_X8R8G8B8:
	case HOE_DXT2:
	case HOE_DXT3:
	case HOE_DXT4:
	case HOE_DXT5:
		return 32;
	case HOE_R8G8B8:
		return 24;
	case HOE_DXT1:
	case HOE_R5G6B5:
	case HOE_X1R5G5B5:
	case HOE_A1R5G5B5:
	case HOE_A8R3G3B2:
	case HOE_X4R4G4B4:
	case HOE_L16:
	case HOE_A8L8:
	case HOE_A4R4G4B4:
		return 16;
	case HOE_R3G3B2:
	case HOE_A8:
		return 8;
	default:
		return 0;
	}
}

int HoeFormatSizeAlpha(HOEFORMAT format)
{
    switch (format)
	{
	case HOE_A8R8G8B8:
	case HOE_A8R3G3B2:
	case HOE_A8:
	case HOE_A8L8:
		return 8;
	case HOE_A4R4G4B4:
		return 4;
	case HOE_A1R5G5B5:
		return 1;
	default:
		return 0;
	}
}

#ifdef _HOE_D3D_

HOEFORMAT HoeFormatX(D3DFORMAT format)
{
	/*switch (format)
	{
	case D3DFMT_R8G8B8: return HOE_R8G8B8;
    case D3DFMT_A8R8G8B8: return HOE_A8R8G8B8;
    case D3DFMT_X8R8G8B8: return HOE_X8R8G8B8;
    case D3DFMT_R5G6B5: return HOE_R5G6B5;
    case D3DFMT_X1R5G5B5: return HOE_X1R5G5B5;
    case D3DFMT_A1R5G5B5: return HOE_A1R5G5B5;
    case D3DFMT_A4R4G4B4: return HOE_A4R4G4B4;
    case D3DFMT_R3G3B2: return HOE_R3G3B2;
    case D3DFMT_A8: return HOE_A8;
    case D3DFMT_A8R3G3B2: return HOE_A8R3G3B2;
    case D3DFMT_X4R4G4B4: return HOE_X4R4G4B4;
    //case D3DFMT_G16R16: return HOE_G16R16;
    //case D3DFMT_A2R10G10B10: return HOE_A2R10G10B10;
    //case D3DFMT_A16B16G16R16: return HOE_A16B16G16R16;

    //case D3DFMT_A8P8: return HOE_A8P8;
    //case D3DFMT_P8: return HOE_P8;

    //case D3DFMT_L8: return HOE_L8;
    case D3DFMT_A8L8: return HOE_A8L8;
    //case D3DFMT_A4L4: return HOE_A4L4;

    //case D3DFMT_V8U8: return HOE_V8U8;
    //case D3DFMT_L6V5U5: return HOE_L6V5U5;
    //case D3DFMT_X8L8V8U8: return HOE_X8L8V8U8;
    //case D3DFMT_Q8W8V8U8: return HOE_Q8W8V8U8;
    //case D3DFMT_V16U16: return HOE_V16U16;
    //case D3DFMT_A2W10V10U10: return HOE_A2W10V10U10;

    //case D3DFMT_UYVY: return HOE_UYVY;
    //case D3DFMT_R8G8_B8G8: return HOE_R8G8_B8G8;
    //case D3DFMT_YUY2: return HOE_YUY2;
    //case D3DFMT_G8R8_G8B8: return HOE_G8R8_G8B8;
    case D3DFMT_DXT1: return HOE_DXT1;
    case D3DFMT_DXT2: return HOE_DXT2;
    case D3DFMT_DXT3: return HOE_DXT3;
    case D3DFMT_DXT4: return HOE_DXT4;
    case D3DFMT_DXT5: return HOE_DXT5;

    case D3DFMT_D16_LOCKABLE: return HOE_D16_LOCKABLE;
    case D3DFMT_D32: return HOE_D32;
    case D3DFMT_D15S1: return HOE_D15S1;
    case D3DFMT_D24S8: return HOE_D24S8;
    case D3DFMT_D24X8: return HOE_D24X8;
    case D3DFMT_D24X4S4: return HOE_D24X4S4;
    case D3DFMT_D16: return HOE_D16;

    //case D3DFMT_D24FS8: assert(0 && "format error");return HOE_D24FS8;

    case D3DFMT_VERTEXDATA: return HOE_VERTEXDATA;
    case D3DFMT_INDEX16: return HOE_INDEX16;
    case D3DFMT_INDEX32: return HOE_INDEX32;

    //case D3DFMT_Q16W16V16U16: return HOE_Q16W16V16U16;

    //case D3DFMT_MULTI2_ARGB8: return HOE_MULTI2_ARGB8;

    // Floating point surface formats

    // s10e5 formats (16-bits per channel)
    //case D3DFMT_R16F: return HOE_R16F;
    //case D3DFMT_G16R16F: return HOE_G16R16F;
    //case D3DFMT_A16B16G16R16F: return HOE_A16B16G16R16F;

    // IEEE s23e8 formats (32-bits per channel)
    //case D3DFMT_R32F: return HOE_R32F;
    //case D3DFMT_G32R32F: return HOE_G32R32F;
    //case D3DFMT_A32B32G32R32F: return HOE_A32B32G32R32F;

    //case D3DFMT_CxV8U8: return HOE_CxV8U8;

	default:
		return HOE_UNKNOWN;
	}*/
	return (HOEFORMAT) format;
}

D3DFORMAT HoeFormatX(HOEFORMAT format)
{
	/*switch (format)
	{
	case HOE_X8R8G8B8: return D3DFMT_X8R8G8B8;
    case HOE_A8R8G8B8: return D3DFMT_A8R8G8B8;
	case HOE_R8G8B8: return D3DFMT_R8G8B8;
	case HOE_A8L8: return D3DFMT_
	default:
		Con_Print("warning: %s format not convert to D3D",HoeFormatString(format));
		return D3DFMT_UNKNOWN;
	}*/
	return (D3DFORMAT)format;
	
}
#endif // _HOE_D3D9_
#ifdef _HOE_OPENGL_

GLint HoeFormatX(HOEFORMAT format)
{
	switch (format)
	{
    case HOE_A8R8G8B8: return GL_RGBA8;
	case HOE_R8G8B8: return GL_RGB8;
	default:
		Con_Print("warning: %s format not convert to Glformat",HoeFormatString(format));
		return 0;
	}
	
}
#endif // _HOE_OPENGL_


namespace HoeFormatFunctions
{

void set_X8R8G8B8(byte * p,HOECOLOR * c)
{
	*p++ = c->b;
	*p++ = c->g;
	*p++ = c->r;
	*p++ = 0xff;
}

void get_X8R8G8B8(byte * p,HOECOLOR * c)
{
	p++;
	c->a = 0xff;
	c->r = *p++;
	c->g = *p++;
	c->b = *p++;
}

void set_A8R8G8B8(byte * p,HOECOLOR * c)
{
	*p++ = c->a;
	*p++ = c->r;
	*p++ = c->g;
	*p++ = c->b;
}

void get_A8R8G8B8(byte * p,HOECOLOR * c)
{
	c->a = *p++;
	c->r = *p++;
	c->g = *p++;
	c->b = *p++;
}

void set_R8G8B8(byte * p,HOECOLOR * c)
{
	*p++ = c->r;
	*p++ = c->g;
	*p++ = c->b;
}

void get_R8G8B8(byte * p,HOECOLOR * c)
{
	c->a = 0xff;
	c->r = *p++;
	c->g = *p++;
	c->b = *p++;
}

void set_B8G8R8(byte * p,HOECOLOR * c)
{
	*p++ = c->b;
	*p++ = c->g;
	*p++ = c->r;
}

void get_B8G8R8(byte * p,HOECOLOR * c)
{
	c->a = 0xff;
	c->b = *p++;
	c->g = *p++;
	c->r = *p++;
}

void unknown(byte * p,HOECOLOR * c)
{
	
}

};

parse_format_func HoeFormatGetFunc(HOEFORMAT format,bool set)
{
	switch (format)
	{
	case HOE_X8R8G8B8:
		if (set)
			return HoeFormatFunctions::set_X8R8G8B8;
		else
			return HoeFormatFunctions::get_X8R8G8B8;	
	case HOE_A8R8G8B8:
		if (set)
			return HoeFormatFunctions::set_A8R8G8B8;
		else
			return HoeFormatFunctions::get_A8R8G8B8;
	case HOE_R8G8B8:
		if (set)
			return HoeFormatFunctions::set_R8G8B8;
		else
			return HoeFormatFunctions::get_R8G8B8;
	default:
		return HoeFormatFunctions::unknown;
	}
}


