
#ifndef _LANG_H_
#define _LANG_H_

#include "hoe_game.h"

BEGIN_HOEGAME

class BaseConsole;

class Lang
{
	struct iter
	{
		int id;
		int string;
	};
	iter iters[1000];
	int num_i;
	char * strings;
	int num_chars;
	int dim;

	int GetPosByID(int id);
public:
	Lang();
	~Lang();
	void Reset();
	bool Load(const char * filename);
	const char * Get(int id);
	const char * GetString(int id);
	void Add(int id, const char * str);
	void Add(int id, const char * str, int len);
};

END_HOEGAME

#endif // _LANG_H_

