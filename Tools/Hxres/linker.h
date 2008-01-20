
#ifndef _HXRES_LINKER_H_
#define _HXRES_LINKER_H_

#include "compiler.h"
#include "scan.h"

class Namespace
{
public:
	struct Obj
	{
		HoeCore::String name;
		int type;
		Compiler * c;
		HoeCore::File file;
		Scaner::Location location;
		bool operator < (const Obj& o) const
		{
			return name < o.name;
		}
		bool operator > (const Obj& o) const
		{
			return name > o.name;
		}
	};
protected:
	HoeCore::MemoryPool& m_pool;
	HoeCore::List<Obj> m_obj;
	HoeCore::LList<Namespace> m_ns;
	HoeCore::String m_name;
public:
	Namespace(const HoeCore::String name, HoeCore::MemoryPool& pool)
		: m_pool(pool), m_ns(pool), m_name(name) {}
	Namespace::Obj* Find(const char * name);
	Namespace::Obj& CreateObj();
	Namespace* CreateNamespace(const char * name);
	HoeCore::LList<Namespace>::Iterator GetNS()
	{
		return HoeCore::LList<Namespace>::Iterator(m_ns);
	}
	uint GetNumNS() { return m_ns.Count(); }
	HoeCore::List<Obj>& GetObj() { return m_obj; }
	const HoeCore::String& GetName() const { return m_name; }
};

class Linker : public PInterface
{
protected:
	HoeCore::MemoryPool& m_pool;
	Namespace m_main;
	HoeCore::Stack<Namespace*> m_nss;
	Namespace * m_act;
public:
	Linker(HoeCore::MemoryPool& pool);
	virtual ~Linker(void);

	void PushNamespace(const char * name);
	void PopNamespace();

	virtual Compiler * AddObject(const char * name, int type, const Scaner::Location& l);
	virtual int Link(const char * output);

	// functions
	virtual bool Func(const HoeCore::CString name, const Values& value);

};

#endif // _HXRES_LINKER_H_


