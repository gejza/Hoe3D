
#ifndef _HOE_CORE_STRUCT_FILE_H_
#define _HOE_CORE_STRUCT_FILE_H_

#include "hoe_flex.h"
#include "hoe_structures.h"

namespace HoeCore {

class StructParserSAX
{
	HoeCore::String m_fn;
	HoeCore::Stack<size_t> m_fns;
protected:
	const HoeCore::String& GetTagName() { return m_fn; }
public:
	virtual void Begin(const tchar* name);
	virtual void End();
	virtual void SetProp(const tchar* name, const tchar* value) = 0;
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
public:
	XMLParser();
	void SetTagText(const tchar* tag) { m_textprop = tag; }

	DECLARE_FLEX_FUNCTIONS(StructParserSAX&)
};
 

} // namespace HoeCore

#endif // _HOE_CORE_STRUCT_FILE_H_
