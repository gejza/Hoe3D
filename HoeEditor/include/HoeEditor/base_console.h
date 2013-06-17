
#ifndef _HOE_EDITOR_BASE_CONSOLE_H_
#define _HOE_EDITOR_BASE_CONSOLE_H_

#include <hoe3d.h>

extern FILE * g_filelog;

class EditBaseConsole : public XHoeConsole
{
public:
	void Printf(const char *, ...);
	void Printf(const char *, va_list argptr);
	virtual void HOEAPI Con_Print(const tchar *);
};

#endif // _HOE_EDITOR_BASE_CONSOLE_H_

