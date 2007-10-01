
#include "StdAfx.h"
#include "shared.h"
#include "utils.h"
#include "cmdexec.h"

struct TCmd
{
	const tchar * cmd;
	HOE_CMDFUNC func;
	void * par;
	const tchar * help;
};

static TCmd cmds[100];
static int numc = 0;

int c_quit(int argc, const tchar * argv[], void * param);
int c_fquit(int argc, const tchar * argv[], void * param);
int c_help(int argc, const tchar * argv[], void * param);
int c_list(int argc, const tchar * argv[], void * param);

CmdExec::CmdExec()
{
	SET_SHARED_PTR(exec);
	Register(T("quit"),c_quit,NULL,T("quit program"));
	Register(T("quit!"),c_fquit,NULL,T("force quit"));
	Register(T("help"),c_help,NULL);
	Register(T("list"),c_list,NULL,T("List all commands."));
}

CmdExec::~CmdExec()
{
	UNSET_SHARED_PTR(exec);
}

bool CmdExec::Register(const tchar * cmd, HOE_CMDFUNC func, void * par, const tchar * help)
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

int CmdExec::exec(const tchar *cmd)
{
	const tchar * p = cmd;
	while (*p == ' ' || *p == '\t') p++;

	TCmd * scmd = FindCmd(p);
	if (!scmd)
	{
		Con_Print("unknown command '%s'",cmd);
		return -1;
	}

	return RunCmd(scmd,p);;

}

TCmd * CmdExec::FindCmd(const tchar * name)
{
	for (int i=0;i < numc;i++)
	{
		if (HoeCore::string::ifbegin(cmds[i].cmd,name) == 0)
			return &cmds[i];

	}
	return NULL;
}

int CmdExec::RunCmd(TCmd * tcmd,const tchar *cmd)
{
	tchar cmdline[1024];
	HoeCore::string::copy(cmdline,cmd,1023);
	const tchar * argv[20];
	int nump = 0;
	tchar * p = cmdline;

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
