
#include "StdAfx.h"
#include "../include/HoeResource/hoe_resfile.h"

const tchar * HoeRes::Res::GetTypeName(uint id)
{
	switch (id)
	{
	case HoeRes::Res::IDHRESHEADER:
		return T("MainNamespace");
	case HoeRes::Res::IDNamespace:
		return T("Namespace");
	case HoeRes::Res::IDPicture:
		return T("Picture");
	case HoeRes::Res::IDStream:
		return T("Stream");
	case HoeRes::Res::IDFont:
		return T("Font");
	case HoeRes::Res::IDFile:
		return T("File");
	default:
		hoe_assert(!"Not implemented");
		return T("unknown");
	};
}


