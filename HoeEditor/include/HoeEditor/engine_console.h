
#ifndef _HOE_EDITOR_ENGINE_CONSOLE_H_
#define _HOE_EDITOR_ENGINE_CONSOLE_H_

#include <hoe3d.h>
#include "base_console.h"

class EngineConsole : public EditBaseConsole
{
public:
	virtual void HOEAPI Con_Print(const char *);
};

#endif // _HOE_EDITOR_ENGINE_CONSOLE_H_

