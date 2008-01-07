
#include "StdAfx.h"
#include "../include/hoe_mem.h"

void * operator new(size_t s, HoeCore::MemoryPool& pool)
{
	return pool.GetMem(s);
}

void * operator new[](size_t s, HoeCore::MemoryPool& pool)
{
	return pool.GetMem(s);
}

void * operator new(size_t s, void* ptr)
{
	return ptr;
}

void * operator new[](size_t s, void* ptr)
{
	return ptr;
}

////////////////////////////////////////////////////

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




