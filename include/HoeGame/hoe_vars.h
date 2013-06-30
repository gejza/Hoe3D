
#ifndef _HOE_GAME_VARS_H_
#define _HOE_GAME_VARS_H_

#include <HoeCore/hoe_string.h>
#include "hoe_game.h"

namespace HoeGame {

class CVarValue : public THoeVarValue
{
public:
	CVarValue() { i = 0; }
	CVarValue(float _f) { f = _f; }
};

class CVar : protected THoeVar
{
protected:
	static THoeVar * staticVars;
	static HoeCore::String_s<1024> lastError;

	void Register();
	void Init(const char * name, int flags);
	void SetString(const tchar * str);
	bool ParseIndex(const tchar * idf, int &pos, int &flags, const THoeVarIndex *& ix) const;
public:
	CVar(const tchar * name, bool value, int flags=0);
	CVar(const tchar * name, int value, int flags=0);
	CVar(const tchar * name, float value, int flags=0);
	CVar(const tchar * name, const tchar * value, int flags=0);
	CVar(const tchar * name, const THoeVarIndex * s, THoeVarValue * val, size_t size, int flags=0);
	~CVar();
	const tchar * GetName() const { return name; }
	const tchar * GetStringValue();
	bool GetBool() const { return value.b; }
	int GetInt() const { return value.i; }
	float GetFloat() const { return value.f; }
	const tchar * GetString() const { return value.str; }

	bool GetBool(int i) const { return vars[i].b; }
	int GetInt(int i) const { return vars[i].i; }
	float GetFloat(int i) const { return vars[i].f; }
	const tchar * GetString(int i) const { return vars[i].str; }

	// structured types

	bool IsInt() const { return (flags & TVAR_TYPE) == TVAR_INTEGER; }
	bool IsFloat() const { return (flags & TVAR_TYPE) == TVAR_FLOAT; } 
	bool IsModified() const { return (flags & TVAR_MODIFIED) != 0; }
	bool IsArray() const { return (flags & TVAR_TYPE) == TVAR_ARRAY; } 
	bool IsStruct() const { return (flags & TVAR_TYPE) == TVAR_STRUCT; } 

	bool Set(int i);
	bool Set(float f);
	bool Set(bool b);
	bool Set(const tchar * str);

	bool SetStructItem(int i, int idf, int flags=TVAR_INTEGER);
	bool SetStructItem(float f, int idf, int flags=TVAR_FLOAT);
	bool SetStructItem(bool b, int idf, int flags=TVAR_BOOL);
	bool SetStructItem(const tchar * str, int idf, int flags=TVAR_STR);

	// statics
	static CVar * GetVar(const tchar * name);
	static CVar * GetFullVar(const tchar * path, int &pos, int &flags, const THoeVarIndex *& ix);
	static bool SetVarValue(const tchar * path, const tchar * vs);
	static bool SetVarValue(const tchar * path, float vf);

	static const tchar * GetLastError() { return lastError; }

	LUA_FUNCTION(l_setvar);
	LUA_FUNCTION(l_getvar);

	static int c_printvar(int argc, const tchar * argv[], void * param);
	static int c_printallvars(int argc, const tchar * argv[], void * param);
	static int c_setvar(int argc, const tchar * argv[], void * param);
};

} // namespace HoeGame

#endif // _HOE_GAME_VARS_H_

