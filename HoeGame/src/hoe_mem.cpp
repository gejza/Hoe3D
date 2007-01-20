
#include "StdAfx.h"
#include "../include/hoe_mem.h"


BEGIN_HOEGAME

Mem::Mem(dword hashsize)
{
	hoe_assert(hashsize > 0);
	m_mem = new Item*[hashsize];
	memset(m_mem, 0, sizeof(Item*) * hashsize);
	m_hash = hashsize;
}

Mem::~Mem()
{
	// TODO projit vsechny a promazat
	//SAFE_DELETE_ARRAY(m_mem);
}

Mem::Item * Mem::Get(int id, const char * key, bool createnew)
{
	// spocitat hash:
	dword hash = HoeCore::HashString(key) + id;
	hash = hash % m_hash;
	Item ** p = &m_mem[hash];
	while (*p)
	{
		if ((*p)->id == id && (*p)->key[0] == *key 
			&& strcmp((*p)->key,key) == 0)
			return *p;
		p = &((*p)->next);
	}
	// polozka nexistuje, vytvorit?
	if (createnew)
	{
		*p = new Item;
		(*p)->id = id;
		(*p)->key = strdup(key); // TODO lepsi vyuziti pameti
		(*p)->type = ENotSet;
		(*p)->next = NULL;
		return *p;
	}
	return NULL;
}

void Mem::Item::Set(int i)
{
	type = Mem::EInt;
	ivalue = i;
}

void Mem::Item::Set(const char * s)
{
	type = Mem::EString;
	hoe_assert(!"Nedodelano");
}


bool Mem::MemTest()
{
	Mem m(1);
	m.Get(1, "a", true);
	m.Get(2, "a", true);
	m.Get(1, "b", true);
	m.Get(2, "b", true);

	return true;
}

END_HOEGAME


