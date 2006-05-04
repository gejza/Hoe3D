
#include "StdAfx.h"
#include "hoe_vars.h"
#include "lua_script.h"

BEGIN_HOEGAME

TVar * CVar::staticVars;

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
	for (TVar * v = staticVars;v;v=v->next)
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

void CVar::Set(const char * str)
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
}

void CVar::Set(float f)
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

int CVar::c_setvar(int argc, const char * argv[], void * param)
{
	if (argc == 3)
	{
		CVar * v = CVar::GetVar(argv[1]);
		if (v)
		{
			v->Set(argv[2]);
		}
		else
		{
			BaseConsole::Printf("unknown variable %s", argv[1]);
		}
	}
	return 0;
}

int CVar::l_setvar(lua_State * L)
{
	HoeGame::LuaParam lp(L);
	if (lp.CheckPar(2,"s*", "SetVar"))
	{
		CVar * v = CVar::GetVar(lp.GetString(-2));
		if (v)
		{
			if (!lp.IsNum(-2))
				v->Set(lp.GetString(-1));
			else
				v->Set((float)lp.GetNum(-1));		
		}
		else
		{
			lp.Error("Var %s not exist.", lp.GetString(-2));
		}
	}
	return 0;
}

int CVar::l_getvar(lua_State * L)
{
	HoeGame::LuaParam lp(L);
	if (lp.CheckPar(1,"s", "GetVar"))
	{
		CVar * v = CVar::GetVar(lp.GetString(-2));
		if (v)
		{
			if (v->IsInt())
			{
				lp.PushNum(v->GetInt());
				return 1;
			}
			/** @todo Dodelat vars */
			assert(!"neni implementovano pro jine hodnoty nez int.");/*!!!*/
		}
		else
		{
			lp.Error("Var %s not exist.", lp.GetString(-2));
		}
	}
	return 0;
}

END_HOEGAME
