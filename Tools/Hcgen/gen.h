
#ifndef _HCGEN_GEN_H_
#define _HCGEN_GEN_H_

class Consts;

class CGen : public HoeUtils::IndentStream
{
public:
	CGen(HoeCore::WriteStream& s);
};

struct Type;
class HGen : public CGen
{
	Consts& m_c;
	HoeCore::KeyList<HoeCore::String, HoeCore::String> m_deftypes;
public:
	HGen(HoeCore::WriteStream& s, Consts& c);
	void StructOut(const Type& t);
	void TypeOut(const Type& ti, const HoeCore::String& name);

};

class CppGen : public CGen
{
	Consts& m_c;
	int m_nl;
public:
	CppGen(HoeCore::WriteStream& s, Consts& c, const tchar* fn);
	void TypeOut(const Type& ti, HoeCore::Stack<const tchar*>& name);

};

#endif // _HCGEN_GEN_H_

