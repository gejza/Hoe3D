
#ifndef _HOE_CORE_STRUCT_FILE_H_
#define _HOE_CORE_STRUCT_FILE_H_

#include "hoe_flex.h"

namespace HoeCore {

class StructParserSAX
{
public:
	void Begin(const tchar* name)
	{
		name = name;
	}
	void End()
	{

	}
	void SetProp(const tchar* name, const tchar* value)
	{
	}
};

class StructParser : public HoeCore::HoeFlex
{
public:
	DECLARE_FLEX_FUNCTIONS(StructParserSAX&)
};
 
class XMLParser : public HoeCore::HoeFlex
{
	void SetProp(StructParserSAX& parser, tchar* text);
	const tchar* m_textprop;
	HoeCore::String m_str;
public:
	XMLParser();

	DECLARE_FLEX_FUNCTIONS(StructParserSAX&)
};
 

} // namespace HoeCore

#endif // _HOE_CORE_STRUCT_FILE_H_
