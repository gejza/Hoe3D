
#ifndef _HOE_GAME_AI_PATH_H_
#define _HOE_GAME_AI_PATH_H_

#include "hoe_game.h"
#include <hoe_core.h>

BEGIN_HOEGAME
namespace AI {

class Path
{
public:
   // hodil by se stack, a ne fronta ze :)
   //HoeCore::Stack<int> points;
};

// find path
// nemela by to byt jen funkce, ktera dostane cestu a v te hleda??

class MapFindPath : public HoeCore::WordTileMap
{
public:
};

// trida ktera umi hledat v mape, staci ji podstrcit jekkoukoliv mapu
class PathFinder
{
public:
	
};

} // namespace AI
END_HOEGAME

#endif // _HOE_GAME_AI_PATH_H_
