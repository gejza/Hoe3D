
#include <stdio.h>
#include <io.h>
#include <iostream>
#include "../../Hoe3D/src/hoe_format.h"
#include "../../Hoe3D/include/hfmt/htex_file.h"
#include "../lib/hoeUtils.h"
#include "png_comp.h"
#include "texbase.h"

HTexCompilerPNG::HTexCompilerPNG(HTexBase * tex) : HTexCompiler(tex)
{
}

bool HTexCompilerPNG::SaveToStream(HoeUtils::Stream * stream)
{
    HoeUtils::File fin;
    if (!fin.Open(texture->GetFullPath(),HoeUtils::File::mRead))
    {
		return false;
    }

	// kontrola png

	hft_texture head;
	memset(&head,0,sizeof(head));
	head.format = HOE_PNG;
	head.size = fin.GetSize();
	stream->Write(&head,sizeof(hft_texture));
	
    if (!fin.CopyToStream(stream, head.size))
		return false;
	/*DWORD dwMagic;
	if (!fin.Read(&dwMagic,4) || dwMagic != DDS_MAGIC)
	{
		//print("Error: no dds file.\n");
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
	*/
	return true;
}
