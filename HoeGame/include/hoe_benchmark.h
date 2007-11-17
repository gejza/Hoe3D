
#ifndef _HOEGAME_BENCHMARK_H_
#define _HOEGAME_BENCHMARK_H_

#include <hoe3d.h>
#include "hoe_game.h"

BEGIN_HOEGAME

class Bench
{
	unsigned long start;
public:
	void Start(const tchar * name);
	void End(int num);
};

END_HOEGAME


#endif // _HOEGAME_BENCHMARK_H_



