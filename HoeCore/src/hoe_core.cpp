
#include "StdAfx.h"
#include "../include/hoe_core.h"
#include "../include/hoe_structures.h"

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
	if (m_map && width == m_width && height == m_height)
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
	struct Pnt
	{
		uint x,y;
	};
	hoe_assert(m_map);
	// s rekurzi jde rychle do prdele zasobnik

	// jestli je v okoli nejaka 
	if (m_map[INDEX(x,y)] != vypln)
		return;
	HoeCore::Stack<Pnt> stk;
	Pnt p;
	p.x = x;p.y = y;
	stk.Push(p);
	m_map[INDEX(x,y)] = obl;

	while (!stk.IsEmpty())
	{
		p = stk.Pop();
		x = p.x;
		y = p.y;
		if (x > 0 && m_map[INDEX(x-1,y)] == vypln)
		{
			m_map[INDEX(x-1,y)] = obl;
			p.x = x-1;p.y = y;
			stk.Push(p);
		}
		if (y > 0 && m_map[INDEX(x,y-1)] == vypln)
		{
			m_map[INDEX(x,y-1)] = obl;
			p.x = x;p.y = y-1;
			stk.Push(p);
		}
		if ((x+1) < m_width && m_map[INDEX(x+1,y)] == vypln)
		{
			m_map[INDEX(x+1,y)] = obl;
			p.x = x+1;p.y = y;
			stk.Push(p);
		}
		if ((y+1) < m_height && m_map[INDEX(x,y+1)] == vypln)
		{
			m_map[INDEX(x,y+1)] = obl;
			p.x = x;p.y = y+1;
			stk.Push(p);
		}
	}
}

void HoeCore::ByteTileMap::Copy(const HoeCore::ByteTileMap * map)
{
	Create(map->m_width, map->m_height);
	memcpy(m_map, map->m_map, m_width * m_height);
}

int HoeCore::ByteTileMap::GetLines(byte tile, HoeCore::Set<HoeMath::VLine2Int> &lines)
{
	hoe_assert(m_map);
	HoeMath::VLine2Int line;
	uint scount = lines.Count();
	for (uint y=0;y < m_height;y++)
		for (uint x=0;x<m_width;x++)
		{
			// pokud atd
			if (m_map[INDEX(x,y)] != tile)
				continue;
			if ((x+1) < m_width && m_map[INDEX(x+1,y)] != tile)
			{
				line.a = HoeMath::Vector2Int(x+1,y);
				line.b = HoeMath::Vector2Int(x+1,y+1);
				lines.Add(line);
			}
			if ((y+1) < m_height && m_map[INDEX(x,y+1)] != tile)
			{
				line.a = HoeMath::Vector2Int(x,y+1);
				line.b = HoeMath::Vector2Int(x+1,y+1);
				lines.Add(line);
			}
			if (x > 0 && m_map[INDEX(x-1,y)] != tile)
			{
				line.a = HoeMath::Vector2Int(x,y);
				line.b = HoeMath::Vector2Int(x,y+1);
				lines.Add(line);
			}
			if (y > 0 && m_map[INDEX(x,y-1)] != tile)
			{
				line.a = HoeMath::Vector2Int(x,y);
				line.b = HoeMath::Vector2Int(x+1,y);
				lines.Add(line);
			}
		}
	return lines.Count() - scount;
}

#undef INDEX



