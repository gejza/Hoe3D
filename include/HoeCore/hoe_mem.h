
#ifndef _HOE_CORE_MEM_H_
#define _HOE_CORE_MEM_H_

#include <new>
#include <HoeCore/hoe_types.h>

namespace HoeCore {

/**
* Bezpecne presunuti pameti, 
* vyhodne pokud se zdrojove a cilove casti prekryvaji
*/
void CrossMemMove(void * dest, void * src, size_t size);

template<typename TYPE> void MemSwitch(TYPE* a, TYPE* b,  const TYPE ** ctrl = NULL)
{
	unsigned char buff[sizeof(TYPE)];
	memcpy(buff, a, sizeof(TYPE));
	memcpy(a, b, sizeof(TYPE));
	memcpy(b, buff, sizeof(TYPE));
	if (ctrl)
	{
		if (a == *ctrl)
			*ctrl = b;
		else if (b == *ctrl)
			*ctrl = a;
	}
}

template<size_t size> class Buffer_s
{
	byte m_buff[size];
public:
	inline byte * Get() { return m_buff; }
	inline size_t GetSize() { return size; }
	inline void Shift(size_t offset, size_t s)
	{
		hoe_assert((offset+s) <= size);
		CrossMemMove(m_buff, m_buff + offset, s);
	}
};

class Buffer
{
	void* m_buff;
	size_t m_alloc;
public:
	Buffer();
	~Buffer();
	byte* GetPtr() { return reinterpret_cast<byte*>(m_buff); }
	byte* GetPtr(size_t num);
	byte* Realloc(size_t num);
	template<typename TYPE> TYPE Get(size_t num) 
		{ return reinterpret_cast<TYPE>(this->GetPtr(num)); }
	size_t GetSize() { return m_alloc; }
	void Free();
	inline void Shift(size_t offset, size_t size)
	{
		hoe_assert(m_buff && (offset+size) <= m_alloc);
		CrossMemMove(m_buff, reinterpret_cast<byte*>(m_buff) + offset, size);
	}
};

template<typename TYPE> class Dynamic : private Buffer
{
public:
	TYPE* operator ->()
	{
		return this->Get<TYPE*>(sizeof(TYPE));
	}
	TYPE& operator *()
	{
		return *this->Get<TYPE*>(sizeof(TYPE));
	}
	TYPE* operator &()
	{
		return this->Get<TYPE*>(sizeof(TYPE));
	}
};

/** Optimalizator pro alokovani malych objektu */
class MemoryPool
{
	struct PoolItem
	{
		PoolItem * next;
		char * base;
		size_t size;
		size_t max;
		inline size_t GetAvail() { return max - size; }
		void * GetMem(size_t s, size_t aligment);
		void * Clone(const void * p, size_t s, size_t aligment);
	};
	PoolItem * m_pool;
	PoolItem * CreateNew(size_t size);
	PoolItem * FindFree(size_t size);
public:
	struct Stats
	{
		size_t numpools;
		size_t avail;
		size_t used;
	};

	MemoryPool();
	~MemoryPool();
	/** Prirazeni pameti z poolu */
	void * GetMem(size_t s, size_t aligment=1);
	/** Zkopirovani pameti do poolu */
	void * Clone(const void * p, size_t s, size_t aligment=1);
	/** Uvolneni cele pameti */
	void Free();
	/** Statistiky */
	void GetStats(Stats* stat);

};

} // namespace HoeCore

void * operator new(size_t, HoeCore::MemoryPool& pool);
void * operator new[](size_t, HoeCore::MemoryPool& pool);
// empty delete operators
void operator delete( void* , HoeCore::MemoryPool& pool );

#if 0 
inline void * operator new(size_t s, void* ptr)
{
	return ptr;
}
inline void * operator new[](size_t s, void* ptr)
{
	return ptr;
}
inline void operator delete( void* , void* )
{
}
#endif

#endif // _HOE_CORE_MEM_H_
