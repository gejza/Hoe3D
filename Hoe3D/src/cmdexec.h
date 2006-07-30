
#ifndef _COMMAND_EXEC_H_
#define _COMMAND_EXEC_H_

#include "../include/hoe3d.h"

#ifndef HOE_CMDFUNC_DEF
#define HOE_CMDFUNC_DEF
typedef int (*HOE_CMDFUNC)(int argc, const char * argv[], void * param);
#endif

struct TCmd;
struct THoeVar;

class CVar : public THoeVar
{
public:
	CVar(const char * name, bool value, int flags) {}
	bool GetBool() const { return value.b; }
};

class CmdExec
{
protected:
	TCmd * FindCmd(const char * name);
	int RunCmd(TCmd * tcmd,const char *cmd);
public:
	CmdExec();
	~CmdExec();
	bool Register(const char * cmd, HOE_CMDFUNC func, void * par, const char * help = NULL);
	bool Register(THoeVar * var);
	int exec(const char *cmd);
};

#endif // _COMMAND_EXEC_H_

