
#ifndef _HOE_CORE_CONST_FILE_H_
#define _HOE_CORE_CONST_FILE_H_

#include "hoe_flex.h"
#include "hoe_structures.h"
#include "hoe_core.h"

namespace HoeCore {

class Universal;

class ConstParserSAX
{
public:
	typedef List<const tchar*> ValueName;
	typedef List<Universal> Values;

	virtual void SetConst(const ValueName& name,
		const Universal& value); // run from sax parser
	virtual void SetConst(const tchar* name,
		const Universal& value);
	virtual void SetConst(const ValueName& name, const tchar* type, const Values& params);
	// gets
	virtual bool GetConst(const ValueName& name, Universal& value); // run from sax parser
	virtual bool GetConst(const tchar* name, Universal& value);
	virtual void ParseError(const tchar* err) = 0; // run from sax parser
};

class ConstParser : public HoeFlex
{
	DECLARE_FLEX_FUNCTIONS(void)
protected:
	struct Token
	{
		int token;
		union {
			const tchar* str;
			double real;
			long number;
		};
	};
	List<const tchar*> m_name;
	List<Token> m_value;
	StringPool m_pool;

	// fnc
	bool ParseValue(ConstParserSAX&);
	bool GetValue(ConstParserSAX& parser, int& st, HoeCore::Universal& value);
public:
	bool Parse(ConstParserSAX&);
	virtual int Echo(const tchar * buff, size_t size)
	{
		hoe_assert(!"Never run");
		return 0;
	}
};
 
} // namespace HoeCore

#endif // _HOE_CORE_CONST_FILE_H_
