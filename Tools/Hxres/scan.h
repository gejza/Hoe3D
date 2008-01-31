
#ifndef _HOE_UTIL_RESOURCE_COMPILER_SCAN_H_
#define _HOE_UTIL_RESOURCE_COMPILER_SCAN_H_

#include <HoeCore/hoe_flex.h>
#include <HoeCore/hoe_core.h>

class Scaner : public HoeCore::HoeFlex
{
public:
#define FLEX_PARAM HoeCore::StringPool& pool, union YYSTYPE * l
	DECLARE_FLEX_FUNCTIONS(FLEX_PARAM)
#undef FLEX_PARAM
public:
	struct Location
	{
		HoeCore::String ident;
		int line;
	};
	Location GetLocation()
	{
		Location l = { this->GetIdentifier(), this->GetLine() };
		return l;
	}
};

#endif // _HOE_UTIL_RESOURCE_COMPILER_SCAN_H_

