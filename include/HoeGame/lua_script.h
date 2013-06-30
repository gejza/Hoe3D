
#ifndef _HOE_LUA_SCRIPT_H_
#define _HOE_LUA_SCRIPT_H_

#include "hoe_game.h"
#include "hoe_console.h"

namespace HoeGame {

class ResourceMgr;
class Lang;
class FileSystem;

class LuaScript;

class LuaParam
{
protected:
	lua_State * m_L;
	int nump;
public:
	LuaParam(lua_State * L);
	void SaveString(const tchar * str);
	bool GetBool(int pos) const;
	const char * GetString(int pos) const;
	int GetNum(int pos) const;
	double GetDouble(int pos) const;
	float GetFloat(int pos) const;
	void * GetPointer(int pos) const;
	bool Get(int pos, HoeCore::Universal& data) const;
	bool CheckPar(int num, const char * par) const;
	bool CheckPar(int num, const char * par, const char * func);
	int GetNumParam() const; 
	bool IsString(int pos) const;
	bool IsNum(int pos) const;
	bool IsNil(int pos) const;
	bool IsTable(int pos) const;
	const char * GetTypeString(int pos) const;
	int GetType(int pos) const;
    //bruca - neexistuje lua_ispointer()
    bool IsPointer(int pos) const;
	void Error(const tchar * szFormat, ...);
	void PushNum(int num);
	void PushPointer(void * p);
	void PushTable();
	void PushFromStack(int idx);
	void SetTableInteger(const char * par, int data, int tab = -2);
	int GetTableInteger(const char * par, int table) const;
	void SetTableFloat(const char * par, float data, int tab = -2);
	float GetTableFloat(const char * par, int table) const;
	void SetTablePointer(const char * par, void * data, int tab = -2);
	void * GetTablePointer(const char * par, int table) const;
	void SetTable(const char * par, const HoeCore::Universal& data, int tab = -2);
	void SetTable(const HoeCore::Table & data, int tab = -2);

    bool ToString(char * buff, size_t lb, int par);
    bool ToString(char * buff, size_t lb, int from, int to);
    
	void Pop(int num);

	lua_State * GetLua() { return m_L; }
};

class LuaFunc : public LuaParam
{
public:
	LuaFunc(LuaScript * scr, const char * funcname);
	bool Run(int nres);
};

class LuaThread
{
protected:
	lua_State* m_L;
public:
	void run(const char * fn);
	LuaThread();
	void func(const char * name, int);
	bool Next();

};

class LuaFile
{
	FILE * m_f;
	byte m_buff[1000];
	size_t m_bufflength;
	size_t m_act;
	bool m_utf;
public:
	bool Open(const char * filename);
	void Close();
	int GetNextChar();
};

class LuaTextBuffer
{
	char buff[2048];
	size_t size;
public:
	bool IsFull() { return size >= (sizeof(buff)-256);}
	void Add(char c) { if (size < sizeof(buff)) buff[size++] = c; }
	void Add(const LuaTextBuffer &b)
	{
		assert(size+b.size < sizeof(buff));
		memcpy(&buff[size],b.buff,b.size);
		size += b.size;
	}
	void Reset() { size = 0; }
	operator const char * () { return buff; }
	size_t GetSize() { return size; }
	const char * GetBuffer() { return buff; }
	void WriteID(int id)
	{
		size += sprintf(&buff[size],"%d", id);
	}
};

class LuaPreprocess
{
public:
	struct IDConst{
		int ID;
		const char *String;
	};
private:
	lua_State* m_L;
	IDConst * m_constans;
	const char * m_lerror;
	LuaFile m_f;
	LuaTextBuffer m_buff;
	LuaTextBuffer m_autoid;
	FILE * m_foutdbg;
public:
	
	LuaPreprocess(lua_State*L);
	static const char *getF(lua_State *L, void *ud, size_t *size);
	bool LoadFile(const char *filename);
	const char * IDfilter(size_t *size);
	const char * StdCopy(size_t *size);
	void SetConstans(IDConst * csts) { m_constans = csts; }
	const char * GetLastError() { return m_lerror; }
	int GetAutoID(const char * id);
};

class LuaScript
{
	static LuaScript * _this;
protected:
	lua_State* m_L;
	IHoeEngine * engine;
	ResourceMgr * resources;
	FileSystem * filesystem;
	//Lang * lang;
public:
	bool Connect(ResourceMgr * res);
	bool Connect(IHoeEngine * eng);
	bool Connect(Lang * l);
	bool Connect(FileSystem * fs);
	LuaScript();
	~LuaScript();
	static LuaScript * GetInstance() { return _this; }
	lua_State * GetLua() { return m_L; }
	bool Init();
	bool Load(const char * fn, LuaPreprocess::IDConst * csts = NULL, bool run = true);
	void Close();
	void AddFunc(const char * funcname, int (*)(lua_State*));
	void func(const char * name);
private:
	static int error(lua_State * L);
	static int print(lua_State * L);
	static int sleep(lua_State * L);
	// engine func
	static int loadfile(lua_State * L);
	static int hoe(lua_State * L);
	static int loadresource(lua_State * L);
	/*static int loadlang(lua_State * L);
	static int addlang(lua_State * L);
	static int getlang(lua_State * L);*/

	friend class LuaThread;
};


} // namespace HoeGame

#endif // _HOE_LUA_SCRIPT_H_

