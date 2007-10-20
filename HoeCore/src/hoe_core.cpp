
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

int HoeCore::WordTileMap::GetLines(word tile, HoeCore::Set<HoeMath::SegmentLine2i> &lines)
{
	hoe_assert(m_map);
	HoeMath::SegmentLine2i line;
	uint scount = lines.Count();
	for (uint y=0;y < m_height;y++)
		for (uint x=0;x<m_width;x++)
		{
			// pokud atd
			if (m_map[INDEX(x,y)] != tile)
				continue;
			if ((x+1) < m_width && m_map[INDEX(x+1,y)] != tile)
			{
				line.a = HoeMath::Vector2i(x+1,y);
				line.b = HoeMath::Vector2i(x+1,y+1);
				lines.Add(line);
			}
			if ((y+1) < m_height && m_map[INDEX(x,y+1)] != tile)
			{
				line.a = HoeMath::Vector2i(x,y+1);
				line.b = HoeMath::Vector2i(x+1,y+1);
				lines.Add(line);
			}
			if (x > 0 && m_map[INDEX(x-1,y)] != tile)
			{
				line.a = HoeMath::Vector2i(x,y);
				line.b = HoeMath::Vector2i(x,y+1);
				lines.Add(line);
			}
			if (y > 0 && m_map[INDEX(x,y-1)] != tile)
			{
				line.a = HoeMath::Vector2i(x,y);
				line.b = HoeMath::Vector2i(x+1,y);
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

// algorithm
bool HoeCore::Algorythm::Dajkrs::Process(TGraphPoint * from, TGraphPoint * to)
{
	hoe_assert(from != NULL && to != NULL);
	m_open.SetCount(0);
	PGraphPoint p;
	p.p = from;
    p.p->inopen = true;
    m_open.Insert(p);
    while (m_open.Count() > 0)
    {
        const PGraphPoint n = m_open.GetHeap();
        m_open.RemoveHeap();
		n.p->inopen = false;
		// ono by se to dalo udelat tak, ze kazdy ten point, bude ukazovat na point
		// a dostane seznam sousedu
        if (n.p == to)
            return true;
		hoe_assert(n.p->souseds != NULL);
		for (TGraphPoint ** souseds = n.p->souseds; *souseds;souseds++)
		{
			register TGraphPoint * nn = *souseds;
			float g = n.p->g + w(n.p, nn);
            if ((nn->inclosed || nn->inopen) && nn->g <= g)
                    continue;
            nn->prev = n.p;
			nn->g = g;
			nn->f = g + Heuristic(nn,to);
			nn->inclosed = false;
			if (!nn->inopen)
			{
				nn->inopen = true;
				p.p = nn;
				m_open.Insert(p);
            }
		}
		n.p->inclosed = true;
    }
	return false;
}

void HoeCore::CrossMemMove(void * dest, void * src, size_t size)
{
	// rozhoduje smer odkud a kam
	byte* d = reinterpret_cast<byte*>(dest);
	byte* s = reinterpret_cast<byte*>(src);
	if (d < s)
	{
		// musi projet popredu, optimalizace by byla kdyby memcpy kopirovala vzdy odpredu
		size_t di = s - d;
		for (size_t i=0;i < size;i+=di)
		{
			memcpy(d+i, s+i, size < (di+i) ? size-i:di);
		}
	}
	else if (d > s)
	{
		size_t di = d - s;
		for (int i=size-di;i >= 0;i-=di)
		{
			memcpy(d+i, s+i, i > di ? di:di-i);
		}
	}
}

/////////////////////////////////////////////
HoeCore::Table::Table(HoeCore::StringPool & pool)
: m_pool(pool) 
{
}

bool HoeCore::Table::IfExist(const HoeCore::KeyString& key) const
{
	for (uint u=0;u < m_items.Count();u++)
		if (m_items[u].key == key)
			return true;
	return false;
}

HoeCore::Universal & HoeCore::Table::Get(const HoeCore::KeyString& key)
{
	for (uint u=0;u < m_items.Count();u++)
		if (m_items[u].key == key)
			return m_items[u].value;
	// vytvorit
	Item &i = m_items.Add();
	i.key = KeyString(m_pool.Insert(key.key));
	return i.value;
}

/////////////////////////////////////////////////
// Pools
void * HoeCore::MemoryPool::PoolItem::GetMem(size_t s)
{
	if (s > (max-size)) return NULL;
	void * ret = base + size;
	size += s;
	return ret;
}

void * HoeCore::MemoryPool::PoolItem::Clone(const void * p, size_t s)
{
	void * pp = GetMem(s);
	memcpy(pp, p, s);
	return pp;
}

HoeCore::MemoryPool::PoolItem * HoeCore::MemoryPool::CreateNew(size_t size)
{
	PoolItem pi = {0};
	pi.max = size + sizeof(PoolItem);
	pi.base = new char[pi.max];
	return reinterpret_cast<PoolItem*>(pi.Clone(&pi, sizeof(PoolItem)));
}

HoeCore::MemoryPool::PoolItem * HoeCore::MemoryPool::FindFree(size_t size)
{
	PoolItem * b = m_pool;
	while (b)
	{
		if (b->GetAvail() >= size)
			return b;
		b = b->next;
	}
	return NULL;
}
void HoeCore::MemoryPool::Free()
{
	while (m_pool)
	{
		PoolItem * next = m_pool->next;
		delete [] m_pool->base;
		m_pool = next;
	}
}

HoeCore::MemoryPool::MemoryPool() 
{ 
	m_pool = NULL; 
}

HoeCore::MemoryPool::~MemoryPool()
{
	Free();
}

void * HoeCore::MemoryPool::GetMem(size_t s)
{
	PoolItem * b = FindFree(s);
	if (b) { return b->GetMem(s); }
	// vytvorit
	b = CreateNew(s > 10000 ? s*2:10000);
	b->next = m_pool;
	m_pool = b;
	return b->GetMem(s);
}

void * HoeCore::MemoryPool::Clone(const void * p, size_t s)
{
	void * pp = GetMem(s);
	memcpy(pp, p, s);
	return pp;
}

HoeCore::StringPool::ConstString::ConstString(const char * s) 
{
	hash = HoeCore::HashString(s);
	str = s;
}

HoeCore::StringPool::PoolIndex::PoolIndex(const char * s) : ConstString(s)
{
	ref = 1;
}

HoeCore::StringPool::StringPool() 
{
}

const char * HoeCore::StringPool::Strdup(const char * str)
{
	size_t l = strlen(str)+1;
	return reinterpret_cast<const char*>(Clone(str, l));
}

void HoeCore::StringPool::Remove(const char * str)
{
	// zatim vypnuto
	/*todo PoolIndex * ppi = m_keys.Find(str);
	hoe_assert(!ppi);
	if (ppi)
	{
		if (ppi <= 1) 
		{
			// odebrat ze seznamu
			free(ppi->str);
		}
		else
			ppi->ref--;
	}*/
}

const char * HoeCore::StringPool::Insert(const char * str)
{
	PoolIndex * ppi = m_keys.Find(str);
	if (ppi)
	{
		ppi->ref++;
		return ppi->str;
	}
	// vytvorit
	PoolIndex pi(this->Strdup(str));
	m_keys.Add(pi);
	return pi.str;
}

