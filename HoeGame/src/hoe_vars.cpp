
#include "StdAfx.h"
#include "../include/hoe_vars.h"
#include "../include/lua_script.h"

BEGIN_HOEGAME

THoeVar * CVar::staticVars;
char CVar::lastError[1024];

CVar::CVar(const char *_name, bool _value, int _flags)
{
	name = _name;
	value.b = _value;
	flags = (_flags & ~0xff) | TVAR_BOOL;
	Register();
}

CVar::CVar(const char *_name, int _value, int _flags)
{
	name = _name;
	value.i = _value;
	flags = (_flags & ~0xff) | TVAR_INTEGER;
	Register();
}

CVar::CVar(const char *_name, float _value, int _flags)
{
	name = _name;
	value.f = _value;
	flags = (_flags & ~0xff) | TVAR_FLOAT;
	Register();
}


CVar::CVar(const char *_name, const char *_value, int _flags)
{
	name = _name;
	if ((_flags & 0xff) == TVAR_SSTR)
	{
		value.cstr = _value;
		flags = _flags;
	}
	else
	{
		value.str = NULL;
		flags = (_flags & ~0xff) | TVAR_STR;
		Set(_value);
	}
		
	Register();
}

CVar::CVar(const char * _name, const THoeVarIndex * _s, THoeVarValue * _val, size_t _size, int _flags)
{
	name = _name;
	vars = _val;
	size = _size;
	index = _s;
	flags = (_flags & ~0xff) | TVAR_STRUCT;
		
	Register();
}

CVar::~CVar()
{
	if ((flags & 0xff) == TVAR_STR && value.str)
		delete [] value.str;
}

void CVar::Register()
{
	static bool first = true;
	if (first)
	{
		first = false;
		staticVars = this;
		this->next = NULL;
	}
	else
	{
		this->next = staticVars;
		staticVars = this;
	}
}

CVar * CVar::GetVar(const char * name)
{
	for (THoeVar * v = staticVars;v;v=v->next)
		if (strcmp(name,v->name)==0)
			return static_cast<CVar*>(v);
	return NULL;
}

const char * CVar::GetStringValue()
{
	static char str[100];
	switch (flags & TVAR_TYPE)
	{
	case TVAR_BOOL:
		return value.b ? "true":"false";
	case TVAR_INTEGER:
		sprintf(str, "%d", value.i);
		return str;
	case TVAR_FLOAT:
		sprintf(str, "%f", value.f);
		return str;
	case TVAR_STR:
	case TVAR_SSTR:
		return value.str;
	default:
		return "unknown";
	};
}

bool CVar::Set(const char * str)
{
	switch (flags & TVAR_TYPE)
	{
	case TVAR_BOOL:
		switch (str[0])
		{
		case 'f':
		case 'n':
		case '0':
			value.b = false;
			break;
		default:
			value.b = true;
			break;
		}
		break;
	case TVAR_INTEGER:
		value.i = atoi(str);
		break;
	case TVAR_FLOAT:
		value.f = (float)atof(str);
		break;
	case TVAR_STR:
	case TVAR_SSTR:
		SetString(str);
		break;
	default:
		assert(!"Unknown var type");
	};
	flags |= TVAR_MODIFIED;
	return true;
}

bool CVar::Set(float f)
{
	switch (flags & TVAR_TYPE)
	{
	case TVAR_BOOL:
		if (f==0.f)
			value.b = false;
		else
			value.b = true;
		break;
	case TVAR_INTEGER:
		value.i = (int)f;
		break;
	case TVAR_FLOAT:
		value.f = f;
		break;
	case TVAR_STR:
	case TVAR_SSTR:
		{
			char str[50];
			sprintf(str, "%f", f);
			SetString(str);
		}
		break;
	default:
		assert(!"Unknown var type");
	};
	flags |= TVAR_MODIFIED;
	return true;
}

bool CVar::Set(int i)
{
	switch (flags & TVAR_TYPE)
	{
	case TVAR_BOOL:
		if (i==0)
			value.b = false;
		else
			value.b = true;
		break;
	case TVAR_INTEGER:
		value.i = i;
		break;
	case TVAR_FLOAT:
		value.f = (float)i;
		break;
	case TVAR_STR:
	case TVAR_SSTR:
		{
			char str[50];
			sprintf(str, "%d", i);
			SetString(str);
		}
		break;
	default:
		assert(!"Unknown var type");
	};
	flags |= TVAR_MODIFIED;
	return true;
}

bool CVar::Set(bool b)
{
	switch (flags & TVAR_TYPE)
	{
	case TVAR_BOOL:
		value.b = b;
		break;
	case TVAR_INTEGER:
		value.i = b ? 1:0;
		break;
	case TVAR_FLOAT:
		value.f = b ? 1.f:0.f;
		break;
	case TVAR_STR:
	case TVAR_SSTR:
		{
			if (b)
				SetString("true");
			else
				SetString("false");
		}
		break;
	default:
		assert(!"Unknown var type");
	};
	flags |= TVAR_MODIFIED;
	return true;
}

bool CVar::SetStructItem(const char * str, int pos, int flags)
{
	switch (flags & TVAR_TYPE)
	{
	case TVAR_BOOL:
		switch (str[0])
		{
		case 'f':
		case 'n':
		case '0':
			vars[pos].b = false;
			break;
		default:
			vars[pos].b = true;
			break;
		}
		break;
	case TVAR_INTEGER:
		vars[pos].i = atoi(str);
		break;
	case TVAR_FLOAT:
		vars[pos].f = (float)atof(str);
		break;
	/*case TVAR_STR:
	case TVAR_SSTR:
		SetString(str);
		break;*/
	default:
		assert(!"Unknown var type");
	};
	this->flags |= TVAR_MODIFIED;
	return true;
}

bool CVar::SetStructItem(float f, int pos, int flags)
{
	switch (flags & TVAR_TYPE)
	{
	case TVAR_BOOL:
		if (f==0.f)
			vars[pos].b = false;
		else
			vars[pos].b = true;
		break;
	case TVAR_INTEGER:
		vars[pos].i = (int)f;
		break;
	case TVAR_FLOAT:
		vars[pos].f = f;
		break;
	/*case TVAR_STR:
	case TVAR_SSTR:
		{
			char str[50];
			sprintf(str, "%f", f);
			SetString(str);
		}
		break;*/
	default:
		assert(!"Unknown var type");
	};
	this->flags |= TVAR_MODIFIED;
	return true;
}

int GetBasePath(const char * path, char * name)
{
	int ret=0;
	while (*path && *path!='[' && *path != '.')
	{
		*name = *path;
		name++; path++; ret++;
	}
	*name = 0;
	return ret;
}

static int Find(const char * p, const THoeVarIndex * ix)
{
	for (int i=0;ix[i].name;i++)
	{
		if (ix[i].name[0]=='*' || strcmp(p, ix[i].name)==0)
			return i;
	}
	return -1;
}

bool CVar::ParseIndex(const char * idf, int &pos, int &flags, const THoeVarIndex *& ix) const
{
	pos = 0;
	// spocitat data z indexu nebo vnoreneho indexu
	ix = index;
	char i_name[256];
	while (ix)
	{
		if (*idf == '.') idf++;
		idf += GetBasePath(idf, i_name);
		// najit dalsi index
		int i = Find(i_name, ix);
		if (i==-1) 
		{
			sprintf(lastError, "Item %s not found.", i_name);
			return false;
		}
		pos += ix[i].position;
		if (*idf == ']') idf++;
		// je konec?
		if (*idf == 0)
		{
			flags = ix[i].flags;
			ix = ix[i].index;
			return true;
		}
		if (!ix[i].index)
		{
			sprintf(lastError, "Item %s is not struct.", i_name);
			return false;
		}
		ix = ix[i].index;
	}
	return false;
}

void CVar::SetString(const char * str)
{
	if ((flags & 0xff) == TVAR_STR && value.str)
		delete [] value.str;
	if (str == NULL || str[0] == 0)
	{
		str = "";
		flags = (flags & ~0xff) | TVAR_SSTR;
	}
	else
	{
		size_t l = strlen(str);
		l++;
		value.str = new char[l];
		strcpy(value.str, str);
		flags = (flags & ~0xff) | TVAR_STR;
	}
}

CVar * CVar::GetFullVar(const char * path, int &pos, int &flags, const THoeVarIndex *& index)
{
	char var_name[256];
	path += GetBasePath(path, var_name);
	CVar * v = GetVar(var_name);
	if (!v)
	{
		sprintf(lastError, "Var `%s' not exist.", var_name);
		return NULL;
	}
	// base bath
	if (*path)
	{
		if (!v->ParseIndex(path, pos, flags, index))
			return NULL;
		return v;
	}
	pos = -1;
	flags = v->flags;
	return v;
}

bool CVar::SetVarValue(const char * path, const char * vs)
{
	int pos, flags;
	const THoeVarIndex * index;
	CVar * v = GetFullVar(path, pos, flags, index);
	if (!v)
		return false;
	// set
	// rozdilny set
	// jestli je struktura, tak projit a nastavit vsechny
	if ((flags & TVAR_TYPE) == TVAR_STRUCT)
	{
		if (vs == NULL)
		{
			sprintf(lastError, "Only string can set to structure var.");
			return false;
		}
		// parse string
		while (*vs)
		{
			char name[256];
			char val[256];
			// parse
			while (*vs == ' ' || *vs == ';' || *vs == '\t') vs++;
			if (!*vs)
				break;
			// parse name
			int pi = 0;
			while (vs[pi] && vs[pi] != '=') { name[pi]=vs[pi]; pi++; }
			name[pi] = 0;
			vs += pi;
			if (*vs++ != '=')
			{
				sprintf(lastError, "Format error.");
				return false;
			}
			pi = 0;
			while (vs[pi] && vs[pi] != ';') { val[pi]=vs[pi]; pi++; }
			val[pi] = 0;
			vs += pi;
			int i = Find(name, index);
			if (i==-1)
			{
				sprintf(lastError, "Item %s not found.", name);
				return false;
			}
			if (!v->SetStructItem(val, pos + index[i].position, index[i].flags))
				return false;
		}
		return true;
	}
	if (pos >= 0)
		return v->SetStructItem(vs, pos, flags);
	else
		return v->Set(vs);
}

bool CVar::SetVarValue(const char * path, float vf)
{
	int pos, flags;
	const THoeVarIndex * index;
	CVar * v = GetFullVar(path, pos, flags, index);
	if (!v)
		return false;
	// set
	// rozdilny set
	// jestli je struktura, tak projit a nastavit vsechny
	if ((flags & TVAR_TYPE) == TVAR_STRUCT)
	{
		sprintf(lastError, "Only string can set to structure var.");
		return false;
	}
	if (pos >= 0)
		return v->SetStructItem(vf, pos, flags);
	else
		return v->Set(vf);
}

int CVar::c_printvar(int argc, const char * argv[], void * param)
{
	if (argc == 2)
	{
		CVar * v = CVar::GetVar(argv[1]);
		if (v)
		{
			BaseConsole::Printf(v->GetStringValue());
		}
		else
		{
			BaseConsole::Printf("unknown variable %s", argv[1]);
		}
	}
	return 0;
}

int CVar::c_printallvars(int argc, const char * argv[], void * param)
{
	FILE * f = NULL;
	if (argc>=2)
	{
		f = fopen(argv[1], "wt");
	}
	for (THoeVar * v = staticVars;v;v=v->next)
	{
		CVar *var = reinterpret_cast<CVar*>(v);
		if (f)
		{
			fprintf(f, "%s=%s\n", var->GetName(), var->GetStringValue());
		}
		else
			BaseConsole::Printf("%s=%s", var->GetName(), var->GetStringValue());
	}
	if (f)
	{
		BaseConsole::Printf("Print vars to file %s.", argv[1]);
		fclose(f);
	}
	return 0;
}

int CVar::c_setvar(int argc, const char * argv[], void * param)
{
	if (argc == 3)
	{
		if (!CVar::SetVarValue(argv[1], argv[2]))
			BaseConsole::Printf(CVar::GetLastError());
	}
	return 0;
}

int CVar::l_setvar(lua_State * L)
{
	HoeGame::LuaParam lp(L);
	if (lp.CheckPar(2,"s*", "SetVar"))
	{
		bool ok;
		if (!lp.IsNum(-2))
			ok = CVar::SetVarValue(lp.GetString(-2) , lp.GetString(-1));
		else
			ok = CVar::SetVarValue(lp.GetString(-2), (float)lp.GetNum(-1));	
		if (!ok)
			lp.Error(CVar::GetLastError());
	}
	return 0;
}

int CVar::l_getvar(lua_State * L)
{
	HoeGame::LuaParam lp(L);
	if (lp.CheckPar(1,"s", "GetVar"))
	{
		CVar * v = CVar::GetVar(lp.GetString(-1));
		if (v)
		{
			if (v->IsInt())
			{
				lp.PushNum(v->GetInt());
				return 1;
			}
			lp.SaveString(v->GetStringValue());
            return 1;
			/** @todo Dodelat vars */
			//assert(!"neni implementovano pro jine hodnoty nez int a string.");/*!!!*/
			return 1;
		}
		else
		{
			lp.Error("Var `%s' not exist.", lp.GetString(-1));
		}
	}
	return 0;
}

END_HOEGAME
