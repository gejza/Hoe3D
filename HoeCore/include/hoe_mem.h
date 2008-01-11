
#ifndef _HOE_CORE_MEM_H_
#define _HOE_CORE_MEM_H_

namespace HoeCore {

/**
* Bezpecne presunuti pameti, 
* vyhodne pokud se zdrojove a cilove casti prekryvaji
*/
void CrossMemMove(void * dest, void * src, size_t size);

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
		void * GetMem(size_t s);
		void * Clone(const void * p, size_t s);
	};
	PoolItem * m_pool;
	PoolItem * CreateNew(size_t size);
	PoolItem * FindFree(size_t size);
public:
	MemoryPool();
	~MemoryPool();
	/** Prirazeni pameti z poolu */
	void * GetMem(size_t s);
	/** Zkopirovani pameti do poolu */
	void * Clone(const void * p, size_t s);
	/** Uvolneni cele pameti */
	void Free();
};

} // namespace HoeCore

void * operator new(size_t, HoeCore::MemoryPool& pool);
void * operator new(size_t, void *);
void * operator new[](size_t, HoeCore::MemoryPool& pool);
void * operator new[](size_t, void *);
// empty delete operators
void operator delete( void* , void* );
void operator delete( void* , HoeCore::MemoryPool& pool );


#endif // _HOE_CORE_MEM_H_
