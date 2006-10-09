
#include "StdAfx.h"
#include "shared.h"
#include "utils.h"
#include "cmdexec.h"

struct TCmd
{
	const char * cmd;
	HOE_CMDFUNC func;
	void * par;
	const char * help;
};

static TCmd cmds[100];
static int numc = 0;

int c_quit(int argc, const char * argv[], void * param);
int c_fquit(int argc, const char * argv[], void * param);
int c_help(int argc, const char * argv[], void * param);
int c_list(int argc, const char * argv[], void * param);

CmdExec::CmdExec()
{
	SET_SHARED_PTR(exec);
	Register("quit",c_quit,NULL,"quit program");
	Register("quit!",c_fquit,NULL,"force quit");
	Register("help",c_help,NULL);
	Register("list",c_list,NULL,"List all commands.");
}

CmdExec::~CmdExec()
{
	UNSET_SHARED_PTR(exec);
}

bool CmdExec::Register(const char * cmd, HOE_CMDFUNC func, void * par, const char * help)
{
	if (numc == 100)
		return false;
	cmds[numc].cmd = cmd;
	cmds[numc].func = func;
	cmds[numc].par = par;
	cmds[numc].help = help;
	numc++;
	return true;
}

bool CmdExec::Register(THoeVar * var)
{
	return false;
}

int CmdExec::exec(const char *cmd)
{
	const char * p = cmd;
	while (*p == ' ' || *p == '\t') p++;

	TCmd * scmd = FindCmd(p);
	if (!scmd)
	{
		Con_Print("unknown command '%s'",cmd);
		return -1;
	}

	return RunCmd(scmd,p);;

}

TCmd * CmdExec::FindCmd(const char * name)
{
	for (int i=0;i < numc;i++)
	{
		if (string_ex(cmds[i].cmd,name) == 0)
			return &cmds[i];

	}
	return NULL;
}

int CmdExec::RunCmd(TCmd * tcmd,const char *cmd)
{
	char cmdline[1024];
	strncpy(cmdline,cmd,1023);
	const char * argv[20];
	int nump = 0;
	char * p = cmdline;

	while (1)
	{
		// 
		while (*p == ' ' || *p == '\t') p++;		
		if (*p == '\0')
		{
			return tcmd->func(nump,argv,tcmd->par);
		}

		if (nump == 20)
		{
			Con_Print("Error: more parameters.");
			return -2;
		}
		argv[nump] = p;
		nump++;

		while (*p != ' ' && *p != '\t' && *p != '\0') p++;
		if (*p == '\0')
		{
			return tcmd->func(nump,argv,tcmd->par);
		}
        *p++ = '\0';		
	}
}

// std commands

int c_quit(int argc, const char * argv[], void * param)
{
	QuitGame(false,0);
	return 0;
}

int c_fquit(int argc, const char * argv[], void * param)
{
	QuitGame(true,0);
	return 0;
}

int c_help(int argc, const char * argv[], void * param)
{
	for (int i=1; i < argc;i++)
		Con_Print("%d : %s",i,argv[i]);
	return 0;
}

int c_list(int argc, const char * argv[], void * param)
{
	for (int i=0;i < numc;i++)
	{
		if (cmds[i].help)
			Con_Print("%d. %s (%s)",i,cmds[i].cmd,cmds[i].help);
		else
			Con_Print("%d. %s",i,cmds[i].cmd);

	}
	return 0;
}
