
#ifndef _HOE_GAME_UTILS_H_
#define _HOE_GAME_UTILS_H_

#include "hoe_game.h"

BEGIN_HOEGAME

bool SetRootDir(const char * dir);
const char * GetBaseDir(const char * path);

void RemoveEndLine(char * line);

bool SetRootFromExe(const char * path);
#ifdef _WIN32
bool SetRootFromInstance(HINSTANCE hInstance);
#endif

#ifdef _WIN32
#define BEGIN_TRY __try {
#define END_TRY(code)  } __except (HoeGame::ExpFilter(GetExceptionInformation(), GetExceptionCode())) \
{ \
	code ; \
} 
LONG WINAPI ExpFilter(EXCEPTION_POINTERS* pExp, DWORD dwExpCode);
#else
#define BEGIN_TRY
#define END_TRY(code)
#endif

END_HOEGAME

#endif // _HOE_GAME_UTILS_H_

