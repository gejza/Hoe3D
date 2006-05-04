
#include "texbase.h"
#include "jpeg.h"
#include "dds_comp.h"

HTexCompiler * HTexBase::GetCompiler()
{
	if (HoeUtils::is_ext(GetFullPath(),".dds"))
		return new HTexCompilerDDS(this);
	else if (HoeUtils::is_ext(GetFullPath(),".jpg") || HoeUtils::is_ext(GetFullPath(),".jpeg"))
		return new HTexCompilerJPG(this);
	else
		return NULL;
}

