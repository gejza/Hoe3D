
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
	DECLARE_FLEX_FUNCTIONS(void)
public:
	bool Parse(ConstParserSAX&);
	virtual int Echo(const tchar * buff, size_t size)
	{
		/*size_t t = fwrite(buff, 1, size, stdout);
		putchar('\n');
		return t; */
		return 0;
	}
};
 
} // namespace HoeCore

#endif // _HOE_CORE_CONST_FILE_H_
