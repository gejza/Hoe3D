
#include "system.h"
#include "shared.h"
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
	case HOE_JPEG: return "JPEG";
	case HOE_PNG: return "PNG";
	default:
		return "UNKNOWN";
	}
}

int HoeFormatSize(HOEFORMAT format)
{
    switch (format)
	{
	case HOE_A8R8G8B8:
	case HOE_R8G8B8A8:
	case HOE_X8R8G8B8:
	case HOE_B8G8R8A8:
	case HOE_B8G8R8X8:
	case HOE_DXT2:
	case HOE_DXT3:
	case HOE_DXT4:
	case HOE_DXT5:
		return 32;
	case HOE_R8G8B8:
	case HOE_B8G8R8:
		return 24;
	case HOE_DXT1:
	case HOE_R5G6B5:
	case HOE_X1R5G5B5:
	case HOE_A1R5G5B5:
	case HOE_A8R3G3B2:
	case HOE_X4R4G4B4:
	case HOE_L16:
	case HOE_A8L8:
	case HOE_L8A8:
	case HOE_A4R4G4B4:
		return 16;
	case HOE_R3G3B2:
	case HOE_A8:
		return 8;
	default:
		assert(!"unsupported format");
		return 0;
	}
}

int HoeFormatSizeAlpha(HOEFORMAT format)
{
    switch (format)
	{
	case HOE_A8R8G8B8:
	case HOE_R8G8B8A8:
	case HOE_A8R3G3B2:
	case HOE_A8:
	case HOE_A8L8:
	case HOE_L8A8:
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
	switch (format)
	{
	case D3DFMT_X8R8G8B8: return HOE_B8G8R8X8;
    case D3DFMT_A8R8G8B8: return HOE_B8G8R8A8;
	case D3DFMT_R8G8B8: return HOE_B8G8R8;
	default:
		//Con_Print("warning: %s format not convert to D3D",HoeFormatString(format));
		return HOE_UNKNOWN;
	}
}


/** Funkce ktera prevadi HOEFORMAT (tak jak je to na disku), 
 * na D3DFORMAT (tak jak to chape direct x) */
D3DFORMAT HoeFormatX(HOEFORMAT format)
{
	switch (format)
	{
	case HOE_B8G8R8X8: return D3DFMT_X8R8G8B8;
    case HOE_B8G8R8A8: return D3DFMT_A8R8G8B8;
	case HOE_B8G8R8: return D3DFMT_R8G8B8;
	case HOE_L8A8: return D3DFMT_A8L8;
	default:
		Con_Print("warning: %s format not convert to D3D",HoeFormatString(format));
		return D3DFMT_UNKNOWN;
	}

}
#endif // _HOE_D3D9_
#ifdef _HOE_OPENGL_
#include "glext.h"

GLint HoeFormatX(HOEFORMAT format)
{
	switch (format)
	{
	case HOE_R8G8B8A8: return GetRef()->ext.comp.IsSupported() ? GL_COMPRESSED_RGBA:GL_RGBA8;
	case HOE_R8G8B8: return GetRef()->ext.comp.IsSupported() ? GL_COMPRESSED_RGB:GL_RGB8;
	default:
		Con_Print("warning: %s format not convert to Glformat",HoeFormatString(format));
		return 0;
	}
	
}
#endif // _HOE_OPENGL_

HFConvert::HFConvert(size_t num, HOEFORMAT from, HOEFORMAT to)
{
	m_num = num;
	m_from = from;
	m_to = to;
}

HFConvert::~HFConvert()
{
}

byte * HFConvert::GetPointer(byte *origin)
{
	return m_origin = origin;
}

void HFConvert::Make()
{
	if (m_from == HOE_R8G8B8 && m_to == HOE_B8G8R8X8)
	{
		for (int n=m_num-1;n >=0;n--)
		{
			// nacist 
			byte r = *(m_origin+(n*3)+0);
			byte g = *(m_origin+(n*3)+1);
			byte b = *(m_origin+(n*3)+2);
			*(m_origin+(n*4)+0) = b;
			*(m_origin+(n*4)+1) = g;
			*(m_origin+(n*4)+2) = r;
			*(m_origin+(n*4)+3) = 0xff;
		}
	}
	if (m_from == HOE_R8G8B8A8 && m_to == HOE_B8G8R8A8)
	{
		for (int n=m_num-1;n >=0;n--)
		{
			// nacist 
			byte r = *(m_origin+(n*4)+0);
			byte b = *(m_origin+(n*4)+2);
			*(m_origin+(n*4)+0) = b;
			*(m_origin+(n*4)+2) = r;
		}
	}
}



