
#include "StdAfx.h"
#include "../include/HoeResource/hoe_resfile.h"

const char * HoeRes::Res::GetTypeName(uint id)
{
	switch (id)
	{
	case HoeRes::Res::IDHRESHEADER:
		return "MainNamespace";
	case HoeRes::Res::IDNamespace:
		return "Namespace";
	case HoeRes::Res::IDPicture:
		return "Picture";
	case HoeRes::Res::IDStream:
		return "Stream";
	case HoeRes::Res::IDFont:
		return "Font";
	default:
		hoe_assert(!"Not implemented");
		return "unknown";
	};
}


