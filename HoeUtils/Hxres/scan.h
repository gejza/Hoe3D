
#ifndef _HOE_UTIL_RESOURCE_COMPILER_SCAN_H_
#define _HOE_UTIL_RESOURCE_COMPILER_SCAN_H_

#include <hoe_flex.h>
#include <hoe_core.h>

class Scaner : public HoeCore::HoeFlex
{
public:
#define FLEX_PARAM HoeCore::StringPool& pool, union YYSTYPE * l
	DECLARE_FLEX_FUNCTIONS(FLEX_PARAM)
#undef FLEX_PARAM
public:
};

#endif // _HOE_UTIL_RESOURCE_COMPILER_SCAN_H_
