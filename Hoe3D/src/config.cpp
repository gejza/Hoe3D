
/*
 * Configure Class
 */
#include "system.h"
#include "ref.h"
#include "shared.h"
#include "utils.h"
#include "hoe_format.h"
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

const char * Config::GetHoeDesc()
{
#if defined (_HOE_OPENGL_)
	return "Hoe3D OpenGL";
#elif defined (_HOE_D3D9_)
	return "Hoe3D Direct3D9";
#elif defined (_HOE_D3D8_)
	return "Hoe3D Direct3D8";
#else
#error No ref defined.
#endif
}

void Config::GetOSDesc(char * buff)
{
#if defined (_WIN32)
	OSVERSIONINFO	vinfo;
	vinfo.dwOSVersionInfoSize = sizeof(vinfo);

	GetVersionEx ( &vinfo );
	
	sprintf(buff,"Windows version %d.%d.%d",vinfo.dwMajorVersion,vinfo.dwMinorVersion,vinfo.dwBuildNumber);

#elif defined (_LINUX)

	sprintf(buff,"LINUX");

#else
#error "platform not defined..."
#endif
}

bool Config::CheckTexture(dword &width,dword &height,HOEFORMAT &format)
{
#ifdef _HOE_OPENGL_
	if (format == HOE_X8R8G8B8) format = HOE_R8G8B8;
#endif
#ifdef _HOE_D3D9_
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


