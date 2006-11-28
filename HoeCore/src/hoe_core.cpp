
#include "StdAfx.h"
#include "../include/hoe_core.h"
#include "../include/hoe_structures.h"

#define INDEX(x,y) ((y)*m_width+(x))

HoeCore::WordTileMap::WordTileMap()
{
	m_map = NULL;
	m_width = 0;
	m_height = 0;
}

HoeCore::WordTileMap::WordTileMap(uint width, uint height)
{
	m_map = NULL;
	m_width = 0;
	m_height = 0;
	Create(width, height);
}

HoeCore::WordTileMap::~WordTileMap()
{
	if (m_map)
		delete [] m_map;
}

void HoeCore::WordTileMap::Create(uint width, uint height)
{
	if (m_map && width == m_width && height == m_height)
		return;
	if (m_map)
		delete [] m_map;
	m_map = new word[width*height];
	m_width = width;
	m_height = height;
}

void HoeCore::WordTileMap::Clear(word w)
{
	hoe_assert(m_map);
	for (uint y=0;y < m_height;y++)
	{
		for (uint x=0;x < m_width;x++)
			m_map[INDEX(x,y)] = w;
	}
}

bool HoeCore::WordTileMap::Find(word b, uint * x, uint * y)
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

// hack
struct Pnt
{
	uint x,y;
};

void HoeCore::WordTileMap::FloodFill(uint x, uint y, word w)
{
	hoe_assert(m_map);
	// s rekurzi jde rychle do prdele zasobnik

	// jestli je v okoli nejaka 
	dword find = m_map[INDEX(x,y)];

	HoeCore::Stack<Pnt> stk;
	Pnt p;
	p.x = x;p.y = y;
	stk.Push(p);
	m_map[INDEX(x,y)] = w;

	while (!stk.IsEmpty())
	{
		p = stk.Pop();
		x = p.x;
		y = p.y;
		if (x > 0 && m_map[INDEX(x-1,y)] == find)
		{
			m_map[INDEX(x-1,y)] = w;
			p.x = x-1;p.y = y;
			stk.Push(p);
		}
		if (y > 0 && m_map[INDEX(x,y-1)] == find)
		{
			m_map[INDEX(x,y-1)] = w;
			p.x = x;p.y = y-1;
			stk.Push(p);
		}
		if ((x+1) < m_width && m_map[INDEX(x+1,y)] == find)
		{
			m_map[INDEX(x+1,y)] = w;
			p.x = x+1;p.y = y;
			stk.Push(p);
		}
		if ((y+1) < m_height && m_map[INDEX(x,y+1)] == find)
		{
			m_map[INDEX(x,y+1)] = w;
			p.x = x;p.y = y+1;
			stk.Push(p);
		}
	}
}

void HoeCore::WordTileMap::FloodFillPotencial()
{
	bool needact;
	do {
		needact = false;
		for (uint y=0;y < m_height;y++)
			for (uint x=0;x < m_width;x++)
			{
				if (m_map[INDEX(x,y)] == 0)
					continue;

				// najit nejmensi okolni prvek
				word min = m_map[INDEX(x,y)];
				if (x > 0)
				{
					if (m_map[INDEX(x-1,y)] < min) min = m_map[INDEX(x-1,y)];
				}
				else
					min = 0;
				if (y > 0)
				{ 
					if (m_map[INDEX(x,y-1)] < min)
					min = m_map[INDEX(x,y-1)];
				}
				else
					min = 0;
				if ((x+1) < m_width)
				{
					if (m_map[INDEX(x+1,y)] < min)
					min = m_map[INDEX(x+1,y)];
				}
				else
					min = 0;
				if ((y+1) < m_height)
				{
					if (m_map[INDEX(x,y+1)] < min)
					min = m_map[INDEX(x,y+1)];
				}
				else
					min = 0;
				// mam minimum
				min++;
				if (min != m_map[INDEX(x,y)])
				{
					m_map[INDEX(x,y)] = min;
					needact = true;
				}
			}
	}
	while (needact);
}

void HoeCore::WordTileMap::Copy(const HoeCore::WordTileMap * map)
{
	Create(map->m_width, map->m_height);
	memcpy(m_map, map->m_map, m_width * m_height);
}

int HoeCore::WordTileMap::GetLines(word tile, HoeCore::Set<HoeMath::VLine2Int> &lines)
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
/////////////////

HoeCore::TimeMeter::TimeMeter()
{
	start = max = total = avarage = percent = pubmax = last = 0.f;
	num = 0;
}

void HoeCore::TimeMeter::Begin()
{
	start = SysFloatTime();
}

void HoeCore::TimeMeter::End()
{
	last = SysFloatTime() - start;
	total += last;
	if (last > max)
		max = last;
	num++;
}

////////////////////////////////////
dword HoeCore::HashString(const char * str)
{
	register dword hash = 0;
	while (*str)
	{
		register dword g = hash & 0xf0000000;
		hash = hash ^ (g >> 24);
		hash = hash ^ g;
		hash = hash << 4;
		hash += *str;
		str++;
	}
	return hash;
}




