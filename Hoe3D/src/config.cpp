
/*
 * Configure Class
 */
#include "StdAfx.h"
#include "ref.h"
#include "shared.h"
#include "utils.h"
#include "config.h"
#include "../include/hoe3d.h"

extern "C" {
int testMMX(void);
}

Config::Config()
{
	SET_SHARED_PTR(config);
}

Config::~Config()
{
	UNSET_SHARED_PTR(config);
}

const tchar * Config::GetHoeDesc()
{
#if defined (_HOE_OPENGL_)
	return T("Hoe3D OpenGL");
#elif defined (_HOE_D3D9_)
	return T("Hoe3D Direct3D9");
#elif defined (_HOE_D3D8_)
	return T("Hoe3D Direct3D8");
#elif defined (_HOE_D3DM_)
	return T("Hoe3D Direct3DMobile");
#elif defined (_HOE_DD_)
	return T("Hoe2D DirectDraw");
#else
#error No ref defined.
#endif
}

void Config::GetOSDesc(tchar * b)
{
#if defined (_WIN32)
	HoeCore::String_s<50> buff;
	OSVERSIONINFO	vinfo;
	vinfo.dwOSVersionInfoSize = sizeof(vinfo);

	GetVersionEx ( &vinfo );
	
	buff.printf(T("Windows version %d.%d.%d"),vinfo.dwMajorVersion,vinfo.dwMinorVersion,vinfo.dwBuildNumber);
	HoeCore::string::copy(b, buff, 30);
#elif defined (_LINUX)

	HoeCore::string::copy(b, "LINUX", 6);
#elif defined (_MACOSX)
	HoeCore::string::copy(b, "Mac OS X", 9);
#else
#error "platform not defined..."
#endif
}

bool Config::CheckTexture(dword &width,dword &height,HOEFORMAT &format)
{
#ifdef _HOE_OPENGL_
	if (format == HOE_X8R8G8B8) format = HOE_R8G8B8;
	// resize to 2*n
	dword w1=1,h1=1;
	while (w1 < width) w1 <<= 1;
	while (h1 < height) h1 <<= 1;
	width = w1;
	height = h1;
#endif
#ifdef _HOE_D3D_
	switch (format)
	{
	case HOE_A8R8G8B8:
	case HOE_R8G8B8A8:
		format = HOE_B8G8R8A8;
		break;
	case HOE_X8R8G8B8:
	case HOE_R8G8B8:
		format = HOE_B8G8R8X8;
		break;
	case HOE_L8A8:
		if (GetRef()->IsTextureFormatOk(HOE_L8A8))
			format = HOE_L8A8;
		else
			format = HOE_B8G8R8X8;
		break;
	// povolene formaty
	case HOE_DXT1:
	case HOE_DXT2:
	case HOE_DXT3:
	case HOE_DXT4:
	case HOE_DXT5:
	case HOE_U8V8:
		break;
	default:
		assert(!"warning: check format");
	};
#endif
	return false;
}

bool Config::Check(THoeInitSettings * his)
{
	fullscreen = his->fullscreen;
	fs_width = his->width;
	fs_height = his->height;
	return true;
}

bool Config::PostCheck()
{
#ifdef _HOE_D3D_
	max_lights = 8;
#endif
#ifdef _HOE_OPENGL_
	glGetIntegerv(GL_MAX_LIGHTS, &max_lights);
#endif
	return true;
}

////////////////////////////

bool Config::IsFullscreen()
{
	return fullscreen;
}

uint Config::GetWidthView()
{
	return fs_width;
}

uint Config::GetHeightView()
{
	return fs_height;
}
////////////////////////////////////


