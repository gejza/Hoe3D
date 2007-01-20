
#ifndef _HOE_GAME_MEM_H_
#define _HOE_GAME_MEM_H_

#include "hoe_game.h"

BEGIN_HOEGAME



class Mem
{
public:
	enum EItemType
	{
		ENotSet,
		EInt,
		EString,
	};
	struct Item
	{
		EItemType type;
		int id;
		const char * key;
		union {
			int ivalue;
			const char * svalue;
		};
		Item * next;
		void Set(int i);
		void Set(const char * s);
	};
private:
	Item ** m_mem;
	dword m_hash;
public:
	Mem(dword hashsize);
	virtual ~Mem();
	Item * Get(int id, const char * key, bool createnew = false);
	static bool MemTest();
};

END_HOEGAME

#endif // _HOE_GAME_MEM_H_

