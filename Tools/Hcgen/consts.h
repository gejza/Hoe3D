
#ifndef _HCGEN_CONSTS_H_
#define _HCGEN_CONSTS_H_

struct Prop
{
	HoeCore::String name;
	HoeCore::Universal::Type type;
	HoeCore::Map<Prop*, const HoeCore::CString> prop;
};

struct Value
{
	HoeCore::String longname;
	HoeCore::Universal value;
};

struct Item
{
	HoeCore::String name;
	Prop* prop; // typ promene
	HoeCore::Map<Value, const HoeCore::CString> values; // hodnoty
	Item(const HoeCore::String& n) : name(n), prop(0) {}
	bool operator == (const HoeCore::String& key)
	{
		return name == key;
	}
};
///////////////////////////////////////////

class Consts : public HoeCore::ConstParserSAX
{
	HoeCore::Map<Prop, const HoeCore::CString> m_types;
	typedef HoeCore::Map<Item, const HoeCore::String> ItemMap;
	ItemMap m_items;
public:
	virtual void SetConst(const HoeCore::List<const tchar*>& name,
		const HoeCore::Universal& value);
	virtual void ParseError(const tchar* err);
};

#endif // _HCGEN_CONSTS_H_
