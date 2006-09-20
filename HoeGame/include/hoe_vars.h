
#ifndef _HOE_GAME_VARS_H_
#define _HOE_GAME_VARS_H_

#include "hoe_game.h"

BEGIN_HOEGAME

class CVar : protected THoeVar
{
protected:
	static THoeVar * staticVars;

	void Register();
	void Init(const char * name, int flags);
	void SetString(const char * str);
public:
	CVar(const char * name, bool value, int flags);
	CVar(const char * name, int value, int flags);
	CVar(const char * name, float value, int flags);
	CVar(const char * name, const char * value, int flags);
	~CVar();
	const char * GetName() const { return name; }
	const char * GetStringValue();
	bool GetBool() const { return value.b; }
	int GetInt() const { return value.i; }
	float GetFloat() const { return value.f; }
	const char * GetString() const { return value.str; }

	bool IsInt() { return (flags & TVAR_INTEGER) != 0; }
	bool IsFloat() { return (flags & TVAR_FLOAT) != 0; } 
	bool IsModified() { return (flags & TVAR_MODIFIED) != 0; }
	void Set(int i);
	void Set(float f);
	void Set(bool b);
	void Set(const char * str);
	// statics
	static CVar * GetVar(const char * name);

	LUA_FUNCTION(l_setvar);
	LUA_FUNCTION(l_getvar);

	static int c_printvar(int argc, const char * argv[], void * param);
	static int c_printallvars(int argc, const char * argv[], void * param);
	static int c_setvar(int argc, const char * argv[], void * param);
};

END_HOEGAME

#endif // _HOE_GAME_VARS_H_

