
#include "StdAfx.h"
#include "../include/he/base_console.h"

FILE * g_filelog = NULL;

void EditBaseConsole::Printf(const char * szFormat, va_list args)
{
	static char szBuff[2048];
#ifdef _WIN32
	_vsnprintf( szBuff, 1024, szFormat, args );
#else
	vsnprintf( szBuff, 1024, szFormat, args );
#endif
	//fprintf(stderr,"%s\n",szBuff);
	Con_Print(szBuff);
}

void EditBaseConsole::Printf(const char * szFormat, ...)
{
	va_list args;

	va_start(args, szFormat);
	Printf(szFormat, args);
	va_end(args);
}

void EditBaseConsole::Con_Print(const char * str)
{
	fprintf(stderr,"%s\n",str);
	//this->AppendText(s);
	wxLogMessage(str);
}
