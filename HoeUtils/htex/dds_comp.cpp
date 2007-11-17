
#include <windows.h>
#include <stdio.h>
#include <io.h>
#include <iostream>

//typedef unsigned char byte;

#include "../../Hoe3D/src/hoe_format.h"
#include "../../Hoe3D/include/hfmt/htex_file.h"
#include "../lib/hoeUtils.h"
#include "dds.h"
#include "texbase.h"
#include "dds_comp.h"

///////////////////
HTexCompilerDDS::HTexCompilerDDS(HTexBase * tex) : HTexCompiler(tex)
{
}

bool HTexCompilerDDS::SaveToStream(HoeUtils::Stream * stream)
{
    HoeUtils::File fin;
    if (!fin.Open(texture->GetFullPath(),HoeUtils::File::mRead))
    {
		return false;
    }

	DWORD dwMagic;
	if (!fin.Read(&dwMagic,4) || dwMagic != DDS_MAGIC)
	{
		//printf("Error: no dds file.\n");
		return false;
	}

	DDS_HEADER ddsh;
	fin.Read(&ddsh,sizeof(DDS_HEADER));

	if (ddsh.dwDepth > 0)
		return false;

	hft_texture head;
	memset(&head,0,sizeof(head));
	head.format = DDSGetFormat(&ddsh.ddspf);
	head.width = ddsh.dwWidth;
	head.height = ddsh.dwHeight;
	head.size = DDSGetSize(ddsh.dwWidth,ddsh.dwHeight,&ddsh.ddspf);
	head.pitch = DDSGetPitch(ddsh.dwWidth,ddsh.dwHeight,&ddsh.ddspf);
 
	fin.SetPos(4 + ddsh.dwSize);
    stream->Write(&head,sizeof(hft_texture));

    if (!fin.CopyToStream(stream, head.size))
		return false;

	return true;
}

HOEFORMAT HTexCompilerDDS::DDSGetFormat(DDS_PIXELFORMAT * ddpf)
{
	if (ddpf->dwFlags == DDS_FOURCC)
	{
		return (HOEFORMAT)ddpf->dwFourCC;
	}
	else  if (ddpf->dwFlags == DDS_RGBA)
	{
		switch (ddpf->dwRGBBitCount)
		{
		case 32:
			return HOE_B8G8R8A8;
		/*case 16:
			if (ddpf->dwABitMask == 0x8000)
				return HOE_A1R5G5B5; //16-bit pixel format where 5 bits are reserved for each color and 1 bit is reserved for alpha. 
			else if (ddpf->dwABitMask == 0xf000)
				return HOE_A4R4G4B4; //16-bit ARGB pixel format with 4 bits for each channel. 
			else if (ddpf->dwABitMask == 0xff00)
				return HOE_A8R3G3B2; //16-bit ARGB texture format using 8 bits for alpha, 3 bits each for red and green, and 2 bits for blue.
				*/
		}
	}
	else  if (ddpf->dwFlags == DDS_RGB)
	{
		switch (ddpf->dwRGBBitCount)
		{
		case 32:
			return HOE_B8G8R8X8;
		case 24:
			return HOE_B8G8R8;
		/*case 16:
			if (ddpf->dwGBitMask == 0x07B0)
				return HOE_R5G6B5; //16-bit RGB pixel format with 5 bits for red, 6 bits for green, and 5 bits for blue. 
			else if (ddpf->dwGBitMask == 0x03B0)
				return HOE_X1R5G5B5; //16-bit pixel format where 5 bits are reserved for each color. 
			else if (ddpf->dwGBitMask == 0x00F0)
				return HOE_X4R4G4B4; //16-bit RGB pixel format using 4 bits for each color. 
			break;
		case 8:
			return HOE_R3G3B2; //8-bit RGB texture format using 3 bits for red, 3 bits for green, and 2 bits for blue. */
		}
	}
	else if (ddpf->dwFlags == 0x80000)
	{
		switch (ddpf->dwRGBBitCount)
		{
		case 16:
			return HOE_U8V8;
		};
	}
	
	return HOE_UNKNOWN;
}

int HTexCompilerDDS::DDSGetSize(int w,int h,DDS_PIXELFORMAT * ddpf)
{
	if (ddpf->dwFlags == DDS_FOURCC)
	{
		return ((w > 4 ? w / 4:1) *
			(h > 4 ? h / 4:1) *
			(ddpf->dwFourCC == MAKEFOURCC('D','X','T','1') ? 8:16));
	}
	else
	{
		return w * h * (ddpf->dwRGBBitCount / 8);
	}
}

int HTexCompilerDDS::DDSGetPitch(int w,int h,DDS_PIXELFORMAT * ddpf)
{
	if (ddpf->dwFlags == DDS_FOURCC)
	{
		return ((w > 4 ? w / 4:1) *
			(h > 4 ? h / 4:1) *
			(ddpf->dwFourCC == MAKEFOURCC('D','X','T','1') ? 8:16));
	}
	else
	{
		return w * (ddpf->dwRGBBitCount / 8);
	}
}
