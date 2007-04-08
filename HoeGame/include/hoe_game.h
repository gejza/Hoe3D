
/**
   @file    hoe_game.h
   @date    Nov 2004
   @version $Revision: 1.3 $
   @brief   Hlavní definice pro HoeGame
*/

#ifndef _HOE_GAME_H_
#define _HOE_GAME_H_

#include <hoe3d.h>
#include <hoe_structures.h>

#ifdef _WIN32
typedef HMODULE HOE_LIBRARY;
#define ENGINE_DLL "Hoe3D_D3D9.dll"
typedef HINSTANCE HOE_INSTANCE;
#endif // _WIN32

#ifdef _LINUX

typedef void * HOE_LIBRARY;
#define ENGINE_DLL "./Hoe3D.so"
typedef void* HOE_INSTANCE;

#endif // _LINUX

#define BEGIN_HOEGAME namespace HoeGame { 
#define END_HOEGAME  } // namespace HoeGame

#define HOEGAMESDK "HoeGame v1.0"

#define TRACE(text) fprintf(stderr,"trace point: %s\n",text);

#define NUM_PI	3.1415926535897932
#define ToRadian( degree ) ((degree) * (NUM_PI / 180.0f))
#define ToDegree( radian ) ((radian) * (180.0f / NUM_PI))

typedef struct lua_State lua_State; 
#define LUA_FUNCTION(name) static int name(lua_State *L) 

#endif // _HOE_GAME_H_
