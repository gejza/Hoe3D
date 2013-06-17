
#include "StdAfx.h"
#include "../include/HoeGame/hoe_vars.h"
#include "../include/HoeGame/lua_script.h"

namespace HoeGame {

THoeVar * CVar::staticVars;
HoeCore::String_s<1024> CVar::lastError;

CVar::CVar(const tchar *_name, bool _value, int _flags)
{
	name = _name;
	value.b = _value;
	flags = (_flags & ~0xff) | TVAR_BOOL;
	Register();
}

CVar::CVar(const tchar *_name, int _value, int _flags)
{
	name = _name;
	value.i = _value;
	flags = (_flags & ~0xff) | TVAR_INTEGER;
	Register();
}

CVar::CVar(const tchar *_name, float _value, int _flags)
{
	name = _name;
	value.f = _value;
	flags = (_flags & ~0xff) | TVAR_FLOAT;
	Register();
}


CVar::CVar(const tchar *_name, const tchar *_value, int _flags)
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

CVar::CVar(const tchar * _name, const THoeVarIndex * _s, THoeVarValue * _val, size_t _size, int _flags)
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

CVar * CVar::GetVar(const tchar * name)
{
	for (THoeVar * v = staticVars;v;v=v->next)
		if (HoeCore::string::cmp(name,v->name)==0)
			return static_cast<CVar*>(v);
	return NULL;
}

const tchar * CVar::GetStringValue()
{
	static HoeCore::String_s<100> str;
	switch (flags & TVAR_TYPE)
	{
	case TVAR_BOOL:
		return value.b ? T("true"):T("false");
	case TVAR_INTEGER:
		str.printf(T("%d"), value.i);
		return str;
	case TVAR_FLOAT:
		str.printf(T("%f"), value.f);
		return str;
	case TVAR_STR:
	case TVAR_SSTR:
		return value.str;
	default:
		return T("unknown");
	};
}

bool CVar::Set(const tchar * str)
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
		value.i = HoeCore::string::GetNumber(str);
		break;
	case TVAR_FLOAT:
		value.f = HoeCore::string::GetReal(str);
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
			HoeCore::String_s<50> str;
			str.printf(str, T("%f"), f);
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
			HoeCore::String_s<50> str;
			str.printf(str, T("%d"), i);
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
				SetString(T("true"));
			else
				SetString(T("false"));
		}
		break;
	default:
		assert(!"Unknown var type");
	};
	flags |= TVAR_MODIFIED;
	return true;
}

bool CVar::SetStructItem(const tchar * str, int pos, int flags)
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
		vars[pos].i = HoeCore::string::GetNumber(str);
		break;
	case TVAR_FLOAT:
		vars[pos].f = HoeCore::string::GetReal(str);
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

int GetBasePath(const tchar * path, tchar * name)
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

static int Find(const tchar * p, const THoeVarIndex * ix)
{
	for (int i=0;ix[i].name;i++)
	{
		if (ix[i].name[0]=='*' || HoeCore::string::cmp(p, ix[i].name)==0)
			return i;
	}
	return -1;
}

bool CVar::ParseIndex(const tchar * idf, int &pos, int &flags, const THoeVarIndex *& ix) const
{
	pos = 0;
	// spocitat data z indexu nebo vnoreneho indexu
	ix = index;
	tchar i_name[256];
	while (ix)
	{
		if (*idf == '.') idf++;
		idf += GetBasePath(idf, i_name);
		// najit dalsi index
		int i = Find(i_name, ix);
		if (i==-1) 
		{
			lastError.printf( T("Item %s not found."), i_name);
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
			lastError.printf(T("Item %s is not struct."), i_name);
			return false;
		}
		ix = ix[i].index;
	}
	return false;
}

void CVar::SetString(const tchar * str)
{
	if ((flags & 0xff) == TVAR_STR && value.str)
		delete [] value.str;
	if (str == NULL || str[0] == 0)
	{
		str = T("");
		flags = (flags & ~0xff) | TVAR_SSTR;
	}
	else
	{
		size_t l = HoeCore::string::len(str);
		l++;
		value.str = new tchar[l];
		HoeCore::string::copy(value.str, str, l);
		flags = (flags & ~0xff) | TVAR_STR;
	}
}

CVar * CVar::GetFullVar(const tchar * path, int &pos, int &flags, const THoeVarIndex *& index)
{
	tchar var_name[256];
	path += GetBasePath(path, var_name);
	CVar * v = GetVar(var_name);
	if (!v)
	{
		lastError.printf(T("Var `%s' not exist."), var_name);
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

bool CVar::SetVarValue(const tchar * path, const tchar * vs)
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
			lastError.printf( T("Only string can set to structure var."));
			return false;
		}
		// parse string
		while (*vs)
		{
			tchar name[256];
			tchar val[256];
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
				lastError = T("Format error.");
				return false;
			}
			pi = 0;
			while (vs[pi] && vs[pi] != ';') { val[pi]=vs[pi]; pi++; }
			val[pi] = 0;
			vs += pi;
			int i = Find(name, index);
			if (i==-1)
			{
				lastError.printf( T("Item %s not found."), name);
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

bool CVar::SetVarValue(const tchar * path, float vf)
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
		lastError.printf(T("Only string can set to structure var."));
		return false;
	}
	if (pos >= 0)
		return v->SetStructItem(vf, pos, flags);
	else
		return v->Set(vf);
}

#ifndef _UNICODE

int CVar::c_printvar(int argc, const tchar * argv[], void * param)
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
			BaseConsole::Printf(T("unknown variable %s"), argv[1]);
		}
	}
	return 0;
}

int CVar::c_printallvars(int argc, const tchar * argv[], void * param)
{
	HoeCore::File f;
	if (argc>=2)
	{
		f.Open(argv[1], HoeCore::File::hftRewrite);
	}
	for (THoeVar * v = staticVars;v;v=v->next)
	{
		CVar *var = reinterpret_cast<CVar*>(v);
		if (f.IsOpen())
		{
			f.Print(T("%s=%s\n"), var->GetName(), var->GetStringValue());
		}
		else
			BaseConsole::Printf(T("%s=%s"), var->GetName(), var->GetStringValue());
	}
	if (f.IsOpen())
	{
		BaseConsole::Printf(T("Print vars to file %s."), argv[1]);
	}
	return 0;
}

int CVar::c_setvar(int argc, const tchar * argv[], void * param)
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
			lp.Error(T("Var `%s' not exist."), lp.GetString(-1));
		}
	}
	return 0;
}

#endif

} // namespace HoeGame
