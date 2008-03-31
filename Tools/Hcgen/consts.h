
#ifndef _HCGEN_CONSTS_H_
#define _HCGEN_CONSTS_H_

struct Prop
{
	HoeCore::String name;
	HoeCore::Universal::Type type;
	HoeCore::Set<Prop*> prop;
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
	HoeCore::Set<Value> values; // hodnoty
	Item() : prop(0) {}
};
///////////////////////////////////////////

class Consts : public HoeCore::ConstParserSAX
{
	HoeCore::LList<Prop> m_types;
	HoeCore::List<Item> m_items;
public:
	virtual void SetConst(const HoeCore::List<const tchar*>& name,
		const HoeCore::Universal& value);
	virtual void ParseError(const tchar* err);
	Item * FindItem(const tchar* name);
	Prop * FindProp(const tchar* name);
};

#endif // _HCGEN_CONSTS_H_
