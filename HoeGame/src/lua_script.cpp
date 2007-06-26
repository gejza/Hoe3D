
#include "StdAfx.h"

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

// kompatibilita se starsi versi luy
#ifndef LUA_VERSION_NUM
#define LUA_VERSION_NUM 500
#endif

#include "../include/lua_script.h"
#include "../include/hoe_resource_mgr.h"
#include "../include/hoe_lang.h"
#include "../include/hoe_fs.h"
#include "../include/hoe_app.h"

BEGIN_HOEGAME

LuaScript * LuaScript::_this = NULL;

LuaThread::LuaThread()
{
	m_L = NULL;
}

bool LuaThread::Next()
{
	return lua_resume(m_L,0) == 0;
}

void LuaThread::run(const char *fn)
{
	lua_getglobal(m_L, fn);
	lua_pcall(m_L,0,0,0);
}

//////////////////////////////////////////////////////

LuaParam::LuaParam(lua_State * L)
{
	m_L = L;
	nump = 0;
}

bool LuaParam::CheckPar(int num, const char * par) const
{
	int i = 0;
	if (lua_gettop(m_L) != num)
		return false;

	while (par && *par)
	{
		i--;
		switch (*par)
		{
		case 'p':
			if (!lua_islightuserdata(m_L,i))
				return false;
			break;
		case 'n':
			if (!lua_isnumber(m_L,i))
				return false;			
			break;
		case 's':
			if (!lua_isstring(m_L,i))
				return false;
			break;
		case 'b':
		case '*':
			break;
		default:
			return false;
		}
		par++;
	}
	return true;
}

bool LuaParam::CheckPar(int num, const char * par, const char * func)
{
	if (!CheckPar(num, par))
	{
		Error("incorrect argument to function `%s'", func);
		return false;
	}
	return true;
}

int LuaParam::GetNum(int pos) const
{
	return (int)lua_tonumber(m_L, pos);
}

float LuaParam::GetFloat(int pos) const
{
	return (float)lua_tonumber(m_L, pos);
}

double LuaParam::GetDouble(int pos) const
{
	return lua_tonumber(m_L, pos);
}

const char * LuaParam::GetString(int pos) const
{
	return lua_tostring(m_L, pos);
}

bool LuaParam::GetBool(int pos) const
{
	return (lua_toboolean(m_L,pos) != 0);
}

void * LuaParam::GetPointer(int pos) const
{
	return lua_touserdata(m_L,pos);
}

bool LuaParam::Get(int pos, HoeCore::Universal& data) const
{
	// retrieve
	switch (lua_type(m_L,pos))
	{
	//case LUA_TNIL:LUA_TTABLE, LUA_TFUNCTION,LUA_TTHREAD
	case LUA_TNUMBER:
		data.Set(GetNum(pos)); break;
	case LUA_TBOOLEAN:
		data.Set(GetBool(pos)); break;
	case LUA_TSTRING:
		data.Set(GetString(pos)); break;
	case LUA_TUSERDATA:
	case LUA_TLIGHTUSERDATA:
		data.Set(GetPointer(pos)); break;
	default:
		return false;
	};
	return true;
}

void LuaParam::SaveString(const char *str)
{
	lua_pushstring(m_L,str);
	nump++;
}

void LuaParam::PushNum(int num)
{
	lua_pushnumber(m_L, num);
	nump++;
}

void LuaParam::PushPointer(void *p)
{
	lua_pushlightuserdata(m_L, p);
	nump++;
}

void LuaParam::PushFromStack(int idx)
{
	lua_pushvalue(m_L, idx);
	nump++;
}

int LuaParam::GetNumParam() const
{
	return lua_gettop(m_L);
}

bool LuaParam::IsString(int pos) const
{
	return lua_isstring(m_L,pos) != 0;
}

//bruca - neexistuje lua_ispointer()
bool LuaParam::IsPointer(int pos) const{
    return lua_islightuserdata(m_L, pos) != 0;
}

bool LuaParam::IsNum(int pos) const
{
	return lua_type(m_L,pos) == LUA_TNUMBER;
}

bool LuaParam::IsNil(int pos) const
{
	return lua_isnil(m_L, pos) != 0;
}

bool LuaParam::IsTable(int pos) const
{
	return lua_istable(m_L, pos) != 0;
}

const char * LuaParam::GetTypeString(int pos) const
{
	return lua_typename(m_L, lua_type(m_L,pos)); 
}

int LuaParam::GetType(int pos) const
{
	return lua_type(m_L,pos);
}

void LuaParam::PushTable()
{
	lua_newtable(m_L);
	nump++;
}

void LuaParam::SetTableInteger(const char *par, int data, int tab)
{
#if LUA_VERSION_NUM >= 501
	lua_pushinteger(m_L, data);
	lua_setfield(m_L,tab,par);
#else
	assert(!"tato funkce neni dostupna ve verzi Lua 5.0");
#endif
}

int LuaParam::GetTableInteger(const char *par, int tab) const
{
#if LUA_VERSION_NUM >= 501
	lua_getfield(m_L,tab,par);
#else
	assert(!"tato funkce neni dostupna ve verzi Lua 5.0");
#endif
	int i = (int)lua_tonumber(m_L,-1);
	lua_pop(m_L, 1);
	return i;
}

void LuaParam::SetTableFloat(const char *par, float data, int tab)
{
#if LUA_VERSION_NUM >= 501
	lua_pushnumber(m_L, data);
	lua_setfield(m_L,tab,par);
#else
	assert(!"tato funkce neni dostupna ve verzi Lua 5.0");
#endif
}


float LuaParam::GetTableFloat(const char *par, int tab) const
{
#if LUA_VERSION_NUM >= 501
	lua_getfield(m_L,tab,par);
#else
	assert(!"tato funkce neni dostupna ve verzi Lua 5.0");
#endif
	float i = (float)lua_tonumber(m_L,-1);
	lua_pop(m_L, 1);
	return i;
}

void LuaParam::SetTablePointer(const char *par, void * data, int tab)
{
#if LUA_VERSION_NUM >= 501
	lua_pushlightuserdata(m_L, data);
	lua_setfield(m_L,tab,par);
#else
	assert(!"tato funkce neni dostupna ve verzi Lua 5.0");
#endif
}

void * LuaParam::GetTablePointer(const char *par, int tab) const
{
#if LUA_VERSION_NUM >= 501
	lua_getfield(m_L,tab,par);
#else
	assert(!"tato funkce neni dostupna ve verzi Lua 5.0");
#endif
	void * p = lua_touserdata(m_L,-1);
	lua_pop(m_L, 1);
	return p;
}

void LuaParam::SetTable(const char * par, const HoeCore::Universal& data, int tab)
{
#if LUA_VERSION_NUM >= 501
	switch (data.GetType())
	{
	case HoeCore::Universal::TypeNone:
		lua_pushnil(m_L); break;
	case HoeCore::Universal::TypeString:
		lua_pushstring(m_L, data.GetStringValue()); break;
	case HoeCore::Universal::TypeDecimal:
		lua_pushnumber(m_L, data.GetDecimal()); break;
	case HoeCore::Universal::TypeUnsigned:
		lua_pushnumber(m_L, data.GetUnsigned()); break;
	case HoeCore::Universal::TypeFloat:
		lua_pushnumber(m_L, data.GetFloat()); break;
	case HoeCore::Universal::TypeBool:
		lua_pushboolean(m_L, data.GetBool()); break;
	case HoeCore::Universal::TypePointer:
	case HoeCore::Universal::TypeData:
		lua_pushlightuserdata(m_L, data.GetPointer()); break;
	default:
		hoe_assert(!"Unknown datetype of HoeCore::Universal");
		return;
	};
	lua_setfield(m_L,tab,par);
#else
	assert(!"tato funkce neni dostupna ve verzi Lua 5.0");
#endif
}

void LuaParam::SetTable(const HoeCore::Table & data, int tab)
{
	for (uint i=0; i < data.Count();i++)
		SetTable(data.GetKey(i), data.GetValue(i), tab);
}

void LuaParam::Pop(int num)
{
	lua_pop(m_L, num);
}

bool LuaParam::ToString(char * buff, size_t lb, int par)
{
    if (lua_isstring(m_L,par))
		snprintf(buff, lb, "%s",lua_tostring(m_L,par));
	else if (lua_isnil(m_L,par))
		snprintf(buff, lb, "%s","nil");
	else if (lua_isboolean(m_L,par))
		snprintf(buff, lb, "%s",lua_toboolean(m_L,par) ? "true" : "false");
	else
		snprintf(buff, lb, "%s:%p",lua_typename(m_L,lua_type(m_L,par)),lua_topointer(m_L,par));
	return true;
}

bool LuaParam::ToString(char * buff, size_t lb, int from, int to)
{
	buff[0] = '\0';
    int step = (from > to) ? -1: 1;
    to += step; // posunuti na akci vcetne
	for (int i=from; i!=to && lb; i+=step)
	{
        ToString(buff, lb, i);
        while (*buff) 
        {
            lb--;
            buff++;
        }
	}
	return true;
}

void LuaParam::Error(const char * szFormat, ...)
{
	/*lua_Debug ar;
	int l=0;
	while (lua_getstack(m_L, l++, &ar))
		lua_getinfo (m_L, ">SnlufL", &ar);*/

	va_list args;
	va_start(args, szFormat);
	lua_pushvfstring(m_L, szFormat, args);
	va_end(args);
	lua_error(m_L);
}

//////////////////////////////////////////////////////
LuaFunc::LuaFunc(LuaScript * scr, const char * funcname) : LuaParam(scr->GetLua())
{
	lua_getglobal(m_L, funcname);
}

bool LuaFunc::Run(int nres)
{
	//lua_Debug ar;
	lua_call(m_L,nump,nres);

	/*switch (err)
	{
	case LUA_ERRRUN:
	case LUA_ERRMEM:
	case LUA_ERRERR:
		//lua_getstack(m_L,1,&ar);
		Console::Printf("%s", lua_tostring(m_L, -1));
		exit(1);
		break;
	};*/
	return false;
}

//////////////////////////////////////////////////////


LuaScript::LuaScript()
{
	assert(_this == NULL);
	_this = this;
	engine = NULL;
	resources = NULL;
	lang = NULL;
	m_L = NULL;
}

LuaScript::~LuaScript()
{
	if (m_L)
		lua_close(m_L);
	assert(_this == this);
	_this = NULL;
}

bool LuaScript::Init()
{
	BaseConsole::Printf("Lua init.."); 
	/* initialize Lua */
	m_L = lua_open();
	//lua_atpanic(L, print);
	/* load Lua base libraries */
	//luaL_opentests(m_L); 

	//lua_baselibopen(m_L);
	//lua_tablibopen(m_L);
	//lua_iolibopen(m_L);
	luaopen_string(m_L);
	//lua_mathlibopen(m_L);
	//lua_dblibopen(m_L); 
	//luaL_openlibs(m_L);

	lua_register(m_L,"print",LuaScript::print);
	lua_register(m_L,"sleep",LuaScript::sleep);
	lua_atpanic(m_L,LuaScript::error);

	BaseConsole::Printf("$Lua: " LUA_VERSION " " LUA_COPYRIGHT " $");
	BaseConsole::Printf("$Authors: " LUA_AUTHORS " $");
    BaseConsole::Printf("$URL: www.lua.org $");
    BaseConsole::Printf("Stack: %dKb", lua_gc (m_L,LUA_GCCOUNT,0));

	return true;
}

void LuaScript::Close()
{
	if (m_L)
		lua_close(m_L);
	m_L = NULL;
}

void LuaScript::func(const char * name)
{
	LuaFunc f(this, name);
	f.Run(0);
}

///////////////////////////////////////////////

bool LuaFile::Open(const char * filename)
{
	m_act = m_bufflength = 0;
	m_utf = false;
	m_f = fopen(filename,"r");
	if (!m_f)
		return false;
	// jde o UTF-8 ?
	m_bufflength = fread(m_buff, 1, 3, m_f);
	if (m_bufflength == 3 && m_buff[0] == 0xef && m_buff[1] == 0xbb && m_buff[2] == 0xbf)
	{
		m_utf = true;
		m_act = m_bufflength = 0;
	}

	return true;
}

void LuaFile::Close()
{
	fclose(m_f);
}

int LuaFile::GetNextChar()
{
	if (m_act < m_bufflength)
	{
		return m_buff[m_act++];
	}
	else
	{
		if (feof(m_f))
			return -1;

		m_bufflength = fread(m_buff, 1, sizeof(m_buff), m_f);
		if (m_bufflength > 0)
		{
			m_act=1;
			return m_buff[0];
		}
		else
			return -1;
	}
}

/////////////////////////////////////////////////////////

LuaPreprocess::LuaPreprocess(lua_State*L)
{
	m_L = L;
	m_constans = NULL;
	m_lerror = "success";
}

const char * LuaPreprocess::StdCopy(size_t *size)
{
	m_buff.Reset();
	while (!m_buff.IsFull())
	{
		char ic = m_f.GetNextChar();
		if (ic == -1)
			break;
		else
            m_buff.Add(ic);
	}
	*size = m_buff.GetSize();
	if (*size > 0)
        return m_buff;
	else
		return NULL;
}

#define IS_WORDCHAR(c) ((c>='a' && c<='z') || (c>='A' && c<='Z') || (c>='0' && c<='9') || c=='_')

const char * LuaPreprocess::IDfilter(size_t *size){

	char ic;

	m_buff.Reset();
	LuaTextBuffer w;

	while(1){
        
		w.Reset();

		// preskoceni neprazdnych znaku az do pismene nebo do konce radky
		do
		{
			if (m_buff.IsFull())
				goto returnbuff;

			ic=m_f.GetNextChar();
			if (ic == -1)
				goto returnbuff;

			if (IS_WORDCHAR(ic))
				w.Add(ic);
			else
                m_buff.Add(ic);

		} while (!IS_WORDCHAR(ic));

		if (ic == -1)
			goto returnbuff;

		// vyjmout zbytek slova (max 200 znaku)
		while (w.GetSize() < 200)
		{
            ic=m_f.GetNextChar();
			if (IS_WORDCHAR(ic))
				w.Add(ic);
			else
				break;
		}

		int i=0;

		for(i=0;m_constans[i].String!=NULL;i++){
			if(strlen(m_constans[i].String) == w.GetSize() && strncmp(m_constans[i].String,w,w.GetSize()) == 0) break;
		}

		if (m_constans[i].String!=NULL)
		{
			m_buff.WriteID(m_constans[i].ID);
		}
		else
		{
			m_buff.Add(w);
		}

		if (ic!=-1)
            m_buff.Add(ic);
		else
			break;
	}

returnbuff:
	*size = m_buff.GetSize();
	if (*size > 0)
        return m_buff;
	else
		return NULL;
}


const char *LuaPreprocess::getF(lua_State *L, void *ud, size_t *size)
{
	const char * buff;
	LuaPreprocess *lp = reinterpret_cast<LuaPreprocess *>(ud);
	if (lp->m_constans)
        buff = lp->IDfilter(size);
	else
		buff = lp->StdCopy(size);

	if (lp->m_foutdbg)
		fwrite(buff,1,*size,lp->m_foutdbg);
	return buff;

}

bool LuaPreprocess::LoadFile(const char *filename)
{
	char filestr[1024];
	m_foutdbg = NULL;
#ifdef _DEBUG
	sprintf(filestr, "%s.dbg", filename);
	m_foutdbg = fopen(filestr, "w");
#endif

	sprintf(filestr, "@%s", filename);
	//if (luaL_loadfile(m_L, fn) == LUA_ERRFILE)
	//	return false;

 // LoadF lf;
	int status;
 // int c;
 // int fnameindex = lua_gettop(L) + 1;  /* index of filename on the stack */
 //   lua_pushfstring(L, "@%s", filename);
	if (!m_f.Open(filename))
	{
		lua_pushfstring(m_L, "Failed open script '%s'", filename);
		m_lerror = lua_tostring(m_L, -1);
		return false;
	}

	// zjisteni zda jde o UTF-8
	
 // if (lf.f == NULL) return errfile(L, fnameindex);  /* unable to open file */
 // c = ungetc(getc(lf.f), lf.f);
 // if (!(isspace(c) || isprint(c)) && lf.f != stdin) {  /* binary file? */
 //   fclose(lf.f);
 //   lf.f = fopen(filename, "rb");  /* reopen in binary mode */
 //   if (lf.f == NULL) return errfile(L, fnameindex); /* unable to reopen file */
 // }
	status = lua_load(m_L, getF, this, filestr);
	m_f.Close();

	if (status)
	{
		m_lerror = lua_tostring(m_L, -1);
		return false;
	}
	else
	{
		return true; 
	}

	if (m_foutdbg)
		fclose(m_foutdbg);
	m_foutdbg = NULL;
 
}

bool LuaScript::Load(const char * fn, LuaPreprocess::IDConst * csts, bool run)
{
	LuaPreprocess lp(m_L);
	lp.SetConstans(csts);
	
	if (!lp.LoadFile(fn))
	{
		BaseConsole::Printf("Lua error: %s", lp.GetLastError());
		return false;
	}

	if (run)
		lua_call(m_L,0,0);

	return true;
}

void LuaScript::AddFunc(const char * funcname, int (*func)(lua_State*))
{
	lua_register(m_L,funcname,func);
};

bool LuaScript::Connect(FileSystem * fs)
{
	filesystem = fs;
	lua_register(m_L,"AddFile",LuaScript::loadfile);
	return true;
}

bool LuaScript::Connect(IHoe3DEngine *eng)
{
	engine = eng;
	lua_register(m_L,"hoe",LuaScript::hoe);
	return true;
}

bool LuaScript::Connect(ResourceMgr *res)
{
	resources = res;
	lua_register(m_L,"LoadResource",LuaScript::loadresource);
	return true;
}

bool LuaScript::Connect(Lang * l)
{
	lang = l;
	lua_register(m_L,"LoadLang",LuaScript::loadlang);
	lua_register(m_L,"AddLang",LuaScript::addlang);
	lua_register(m_L,"GetLang",LuaScript::getlang);
	return true;
}

// user functions
int LuaScript::error(lua_State * L)
{
	int n=lua_gettop(L);
	int i;
	static char buff[512];
	static char mbuff[256];
	buff[0] = '\0';
	// debug
	/*lua_Debug ar;
	int l=0;
	while (lua_getstack(L, l++, &ar))
	lua_getinfo (L, ">SnlufL", &ar);*/
	for (i=1; i<=n; i++)
	{
		if (lua_isstring(L,i))
			sprintf(mbuff,"%s",lua_tostring(L,i));
		else if (lua_isnil(L,i))
			sprintf(mbuff,"%s","nil");
		else if (lua_isboolean(L,i))
			sprintf(mbuff,"%s",lua_toboolean(L,i) ? "true" : "false");
		else
			sprintf(mbuff,"%s:%p",lua_typename(L,lua_type(L,i)),lua_topointer(L,i));
		strcat(buff,mbuff);
	}
	BaseConsole::Printf(buff);
	HoeApp::GetApp<HoeApp>()->ShowMsg("Script error", buff);
  /*fprintf(stderr, "PANIC: unprotected error in call to Lua API (%s)\n",
                   lua_tostring(L, -1));*/

	return 0;
}

int LuaScript::print(lua_State * L)
{
	int n=lua_gettop(L);
	int i;
	static char buff[512];
	static char mbuff[256];
	buff[0] = '\0';
	for (i=1; i<=n; i++)
	{
		if (lua_isstring(L,i))
			sprintf(mbuff,"%s",lua_tostring(L,i));
		else if (lua_isnil(L,i))
			sprintf(mbuff,"%s","nil");
		else if (lua_isboolean(L,i))
			sprintf(mbuff,"%s",lua_toboolean(L,i) ? "true" : "false");
		else
			sprintf(mbuff,"%s:%p",lua_typename(L,lua_type(L,i)),lua_topointer(L,i));
		strcat(buff,mbuff);
	}
	BaseConsole::Printf(buff);
	return 0;
}

int LuaScript::sleep(lua_State * L)
{
	return lua_yield(L,0);
}

int LuaScript::loadfile(lua_State * L)
{
	LuaParam par(L);

	const char * fname;

	if (!par.CheckPar(1,"s"))
	{
		/*
		lua_pushstring(L, "incorrect argument to function `average'");
        lua_error(L);
		*/

		BaseConsole::Printf("Lua error: Bad param in function LoadFile");
		return 0;
	}

	fname = lua_tostring(L,-1);

	if (!GetInstance()->filesystem->AddResourceFile(fname))
		BaseConsole::Printf("WARNING: file '%s' not exist.",fname);

	return 0;
}

int LuaScript::hoe(lua_State * L)
{
	LuaParam par(L);

	const char * cmd;

	if (!par.CheckPar(1,"s"))
	{
		/*
		lua_pushstring(L, "incorrect argument to function `average'");
        lua_error(L);
		*/

		BaseConsole::Printf("Lua error: Bad param in function Exec");
		return 0;
	}

	cmd = lua_tostring(L,-1);

	GetInstance()->engine->exec(cmd);

	return 0;
}

int LuaScript::loadresource(lua_State * L)
{
	LuaParam par(L);

	if (!par.CheckPar(2,"sn"))
	{
		BaseConsole::Printf("Lua error: Bad param in function AddResource");
		return 0;
	}

	if (!GetInstance()->resources->AddResource(par.GetNum(-2), par.GetString(-1)))
	{
		BaseConsole::Printf("WARNING: cmd '%s' failed.",lua_tostring(L,-1));
	}

	return 0;
} 

int LuaScript::loadlang(lua_State * L)
{
	LuaParam par(L);

	if (!par.CheckPar(1,"s"))
	{
		BaseConsole::Printf("Lua error: Bad param in function LoadLang");
		return 0;
	}

	if (!GetInstance()->lang->Load(par.GetString(-1)))
	{
		BaseConsole::Printf("WARNING: Load lang file '%s' failed.",par.GetString(-1));
	}

	return 0;
} 

int LuaScript::addlang(lua_State * L)
{
	LuaParam par(L);

	if (!par.CheckPar(2,"sn"))
	{
		BaseConsole::Printf("Lua error: Bad param in function AddLang");
		return 0;
	}

	GetInstance()->lang->Add(par.GetNum(-2), par.GetString(-1));

	return 0;
} 

int LuaScript::getlang(lua_State * L)
{
	LuaParam par(L);

	if (!par.CheckPar(1,"n"))
	{
		BaseConsole::Printf("Lua error: Bad param in function GetLang");
		return 0;
	}

	par.SaveString(GetInstance()->lang->GetString(par.GetNum(-1)));

	return 1;
} 


END_HOEGAME











