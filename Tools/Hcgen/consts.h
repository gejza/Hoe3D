
#ifndef _HCGEN_CONSTS_H_
#define _HCGEN_CONSTS_H_

struct Value
{
	HoeCore::String longname;
	HoeCore::Universal value;
	HoeCore::List<HoeCore::Universal> param;

	Value(const HoeCore::String& name) : longname(name) {}
	bool operator == (const tchar* key)
	{
		return longname == key;
	}
};

struct Type
{
	HoeCore::String name;
	enum EType
	{
		TNone,
		TStruct,
		TUser,
		TConst,
	} type;
	bool isroot;
	HoeCore::KeyList<HoeCore::String, const HoeCore::String> str_child;
	HoeCore::String usr_type;
	HoeCore::Universal::Type cst_type;
	Type(const HoeCore::String& n) 
		: name(n), type(TNone), cst_type(HoeCore::Universal::TypeNone),
		isroot(false) {}
	bool operator == (const HoeCore::String& key)
	{
		return name == key;
	}

};
///////////////////////////////////////////

class Consts : public HoeCore::ConstParserSAX
{
	typedef HoeCore::Map<Type, const HoeCore::String> TypeMap;
	typedef HoeCore::Map<Value, const tchar*> ValueMap;
	ValueMap m_values;
	TypeMap m_types;
public:
	virtual void SetConst(const ValueName& name,
		const HoeCore::Universal& value);
	virtual void SetConst(const ValueName& name, const tchar* type, const Values& params);
	virtual bool GetConst(const tchar* name, HoeCore::Universal& value);
	Type& GetProperty(const ValueName& name);
	virtual void ParseError(const tchar* err);
	virtual long PixelFunc(const ValueName& name, const tchar* type, int pos, long num);

	const TypeMap& GetTypes() { return m_types; }
	const ValueMap& GetValues() { return m_values; }
};

#endif // _HCGEN_CONSTS_H_
