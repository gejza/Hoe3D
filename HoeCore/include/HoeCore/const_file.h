
#ifndef _HOE_CORE_CONST_FILE_H_
#define _HOE_CORE_CONST_FILE_H_

#include "hoe_flex.h"

namespace HoeCore {

class ConstParserSAX
{
public:
	virtual void SetProp(const tchar* name, const tchar* value) {};
};

class ConstParser : public HoeCore::HoeFlex
{
public:
	DECLARE_FLEX_FUNCTIONS(ConstParserSAX&)
};
 
} // namespace HoeCore

#endif // _HOE_CORE_CONST_FILE_H_
