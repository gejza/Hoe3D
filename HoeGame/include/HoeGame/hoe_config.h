
#ifndef _HOE_GAME_CONFIG_H_
#define _HOE_GAME_CONFIG_H_

#include "hoe_game.h"
#include "hoe_fs.h"

BEGIN_HOEGAME

class CVar;

class Config
{
private:
	bool Realloc(size_t max);
protected:
	char * str;
	size_t size;
	bool ParseValue(const char * var);
public:
	Config();
	/** Velmi strategicka funkce, jeste se musi rozhodnout jak prezentovat vysledek */
	int Load(const char * filename);

	bool LoadToModify(const char * filename);
	void Set(const char * varname, const char * value);
	bool Save(const char * filename);

	virtual void Var(const char * varname, const char * value) {}
	virtual void Var(const char * varname, bool value);
	virtual void Var(const char * varname, int value);
	virtual void Var(const char * varname, float value);

	virtual void ParseArgs(HOE_INSTANCE hInstance, const tchar * cmdline);
	virtual void ParseArgs(int argc,tchar * argv[]);
	/**
	* Callback funkce ktera se vola pro parsovani parametru.
	* @return Pocet parsovanych parametru, nebo -1 pri chybe
	*/
	virtual int Arg(int argc,tchar * argv[]);
};

class ConfigVars : public Config
{
public:
	virtual void Var(const tchar * varname, const tchar * value);
	void PushVar(CVar & var);
	void PushVar(const tchar * varname);
};

END_HOEGAME

#endif // _HOE_GAME_CONFIG_H_

