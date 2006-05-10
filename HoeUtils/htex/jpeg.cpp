
#include <stdio.h>
#include <io.h>
#include <iostream>
#define HOEFORMAT_ONLY_DEF
#include "../../Hoe3D/src/hoe_format.h"
#include "../../Hoe3D/include/hfmt/htex_file.h"
#include "../lib/hoeUtils.h"
#include "jpeg.h"
#include "texbase.h"

HTexCompilerJPG::HTexCompilerJPG(HTexBase * tex) : HTexCompiler(tex)
{
}

bool HTexCompilerJPG::SaveToStream(HoeUtils::Stream * stream)
{
    HoeUtils::File fin;
    if (!fin.Open(texture->GetFullPath(),HoeUtils::File::mRead))
    {
		return false;
    }

	hft_texture head;
	memset(&head,0,sizeof(head));
	head.format = HOE_JPEG;
	head.size = fin.GetSize();

    stream->Write(&head,sizeof(hft_texture));
    if (!fin.CopyToStream(stream, head.size))
		return false;
        
	return true;
}

