
#ifndef _HOE_CORE_CONST_FILE_H_
#define _HOE_CORE_CONST_FILE_H_

#include "hoe_flex.h"
#include "hoe_structures.h"
#include "hoe_core.h"

namespace HoeCore {

class Universal;

class ConstParserI
{
public:
	typedef List<const tchar*> ValueName;
	typedef List<Universal> Values;

	virtual void SetConst(const ValueName& name,
		const Universal& value) = 0; // run from sax parser
	virtual void SetConst(const ValueName& name, const tchar* type, const Values& params) = 0;
	// gets
	virtual bool GetConst(const ValueName& name, Universal& value) = 0; // run from sax parser
	virtual void ParseError(const tchar* err) = 0; // run from sax parser
	virtual long PixelFunc(const ValueName& name, const tchar* type, int pos, long num) = 0;
};

class ConstParserSAX : public ConstParserI
{
public:
	virtual void SetConst(const ValueName& name,
		const Universal& value); // run from sax parser
	virtual void SetConst(const tchar* name,
		const Universal& value);
	virtual void SetConst(const ValueName& name, const tchar* type, const Values& params);
	// gets
	virtual bool GetConst(const ValueName& name, Universal& value); // run from sax parser
	virtual bool GetConst(const tchar* name, Universal& value);
	virtual void ParseError(const tchar* err) = 0; // run from sax parser
	virtual long PixelFunc(const ValueName& name, const tchar* type, int pos, long num) { return num; }
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
	bool ParseValue(ConstParserI&);
	bool GetValue(ConstParserI& parser, const tchar* type, int& st, 
		HoeCore::Universal& value, int numv);
public:
	bool Parse(ConstParserI&);
	virtual int Echo(const tchar * buff, size_t size)
	{
		hoe_assert(!"Never run");
		return 0;
	}
};
 
} // namespace HoeCore

#endif // _HOE_CORE_CONST_FILE_H_
