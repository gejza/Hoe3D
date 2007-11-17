
#ifdef _WIN32
#include <windows.h>
#endif

#include "../lib/hoeUtils.h"
#include "texture.h"
#include "jpeg.h"

HTexTexture::HTexTexture()
{
}

bool HTexTexture::Load(const char * p)
{
	path = p;
	name = HoeUtils::GetFileTitle(path);
	return HoeUtils::is_file(p);
}

const char * HTexTexture::GetName()
{
	return name.c_str();
}

const char * HTexTexture::GetFullPath()
{
	return path.c_str();
}

const char * HTexTexture::GetInfo()
{
	return GetName();
}

