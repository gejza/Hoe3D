
#ifndef _HOE_GAME_VARS_H_
#define _HOE_GAME_VARS_H_

#include "hoe_game.h"

BEGIN_HOEGAME

#ifndef BIT
#define BIT( num )				( 1 << ( num ) )
#endif 

enum EVarFlags
{
	TVAR_BOOL				= 1,	// variable is a boolean
	TVAR_INTEGER			= 2,	// variable is an integer
	TVAR_FLOAT				= 3,	// variable is a float
	TVAR_STR				= 4,	// variable is a string
	TVAR_SSTR				= 5,	// variable is a static string
	TVAR_TYPE				= 0xff, // all types
	TVAR_MODIFIED			= BIT(9)
};

struct TVar
{
	const char * name;
	union {
		bool b;
		int i;
		float f;
		char * str;
		const char * cstr;
	} value;
	int flags;
	TVar * next;
};

class CVar : protected TVar
{
protected:
	static TVar * staticVars;

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
	void Set(float f);
	void Set(const char * str);
	// statics
	static CVar * GetVar(const char * name);

	LUA_FUNCTION(l_setvar);
	LUA_FUNCTION(l_getvar);

	static int c_printvar(int argc, const char * argv[], void * param);
	static int c_setvar(int argc, const char * argv[], void * param);
};

END_HOEGAME

#endif // _HOE_GAME_VARS_H_

