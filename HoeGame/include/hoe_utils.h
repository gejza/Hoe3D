
#ifndef _HOE_GAME_UTILS_H_
#define _HOE_GAME_UTILS_H_

#include "hoe_game.h"

BEGIN_HOEGAME

bool SetRootDir(const char * dir);
const char * GetBaseDir(const char * path);

bool SetRootFromExe(const char * path);
#ifdef _WIN32
bool SetRootFromInstance(HINSTANCE hInstance);
#endif

END_HOEGAME

#endif // _HOE_GAME_UTILS_H_

