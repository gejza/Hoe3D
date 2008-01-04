#pragma once


class Linker
{
public:
	class ObjectData
	{
	};
	struct Obj
	{
		HoeCore::String name;
		int type;
		ObjectData * obj;
	};
protected:
	virtual ObjectData * CreateObject(const char * name, int type);
	HoeCore::List<Obj> m_obj;
	HoeCore::String_s<2048> m_ns;
public:
	Linker(void);
	virtual ~Linker(void);

	void PushNamespace(const char * name);
	void PopNamespace();

	virtual ObjectData * AddObject(const char * name, int type);
	virtual int Link(const char * output);

};
