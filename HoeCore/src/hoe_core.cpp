
#include "StdAfx.h"
#include "../include/hoe_core.h"

#define INDEX(x,y) ((y)*m_width+(x))

HoeCore::ByteTileMap::ByteTileMap()
{
	m_map = NULL;
	m_width = 0;
	m_height = 0;
}

HoeCore::ByteTileMap::ByteTileMap(uint width, uint height)
{
	m_map = NULL;
	m_width = 0;
	m_height = 0;
	Create(width, height);
}

HoeCore::ByteTileMap::~ByteTileMap()
{
	if (m_map)
		delete [] m_map;
}

void HoeCore::ByteTileMap::Create(uint width, uint height)
{
	if (width == m_width && height == m_height)
		return;
	if (m_map)
		delete [] m_map;
	m_map = new byte[width*height];
	m_width = width;
	m_height = height;
}

void HoeCore::ByteTileMap::Clear(byte b)
{
	hoe_assert(m_map);
	memset(m_map,b, m_width * m_height);
}

bool HoeCore::ByteTileMap::Find(byte b, uint * x, uint * y)
{
	hoe_assert(m_map);
	for (;*y < m_height;(*y)++)
	{
		for (;*x < m_width;(*x)++)
			if (m_map[INDEX(*x,*y)] == b)
				return true;
		*x = 0;
	}
	return false;
}

void HoeCore::ByteTileMap::Pisek(uint x, uint y, byte obl, byte vypln)
{
	hoe_assert(m_map);
	// jestli je v okoli nejaka 
	if (m_map[INDEX(x,y)] != vypln)
		return;
	m_map[INDEX(x,y)] = obl;
	if (x > 0 && m_map[INDEX(x-1,y)] == vypln)
		Pisek(x-1,y,obl, vypln);
	if (y > 0 && m_map[INDEX(x,y-1)] == vypln)
		Pisek(x,y-1,obl, vypln);
	if ((x+1) < m_width && m_map[INDEX(x+1,y)] == vypln)
		Pisek(x+1,y,obl,vypln);
	if ((y+1) < m_height && m_map[INDEX(x,y+1)] == vypln)
		Pisek(x,y+1,obl,vypln);
}

#undef INDEX



