
#ifndef _HOE_STRUCTURES_H_
#define _HOE_STRUCTURES_H_

#include "hoe_mem.h"

namespace HoeCore {

///////////////////////////////
// Ptr list
/*template<class C> class List
{
	C ** _first;
	uint _num;
public:
	List() 
	{ 
		_num = 0;
		Con_Print("create");
	}
	void Add(PTR *);
	void Set(PTR **f,uint n) { _first = f;_num = n;}
	PTR * operator [](const int i) { return _first[i];}
	uint GetNum() { return _num;} 
	bool IsEmpty() { return _num == 0;}
	PTR * GetLast() { return _first[_num-1]; }
};*/


template<class C> class SetBase
{
protected:
	uint m_count;
	uint m_size;
	C * m_ptr;
	void Resize(uint num)
	{
		if (!num) return;
		this->m_ptr = (C*)realloc(this->m_ptr, num * sizeof(C));
		hoe_assert(this->m_ptr); // todo
		this->m_size = num;
	}
public:
	SetBase()
	{
		this->m_count = 0;m_size = 0; this->m_ptr = NULL;
	}
	SetBase(uint initnum)
	{
		this->m_count = 0;m_size = 0; this->m_ptr = NULL;
		Resize(initnum);
	}
	SetBase(const SetBase & base)
	{
		this->m_count = 0;m_size = 0; this->m_ptr = NULL;
		Copy(base);
	}
	~SetBase()
	{
		this->Delete();
		if (this->m_ptr) free(this->m_ptr);
	}
	uint Count() const { return this->m_count; }
	void Delete() 
	{
		while (this->m_count)
		{
			this->m_ptr[--this->m_count].~C();
		}
	}
	void SetCount(int count) { this->m_count=count; } 
	C & Get(uint n) const
	{
		assert(n < this->m_count);
		return this->m_ptr[n];
	}
	C & operator [] (const int index)
	{
		return Get(index);
	}
	const C & operator [] (const int index) const
	{
		return Get(index);
	}
	bool IsEmpty() { return this->m_count == 0; }
	void Copy(const SetBase & base)
	{
		if (base.m_count > this->m_count)
			Resize(base.m_count);
		memcpy(this->m_ptr, base.m_ptr, base.m_count * sizeof(C));
	}
	const SetBase & operator = (const SetBase & base)
	{
		Copy(base);
		return *this;
	}
	C * GetBasePointer(uint i=0)
	{
		return m_ptr+i;
	}
};

/**
* Trida udrzujici mnozinu objektu
*/
template<class CC> class Set : public SetBase<CC>
{
public:
	void Add(CC c)
	{
		if (this->m_size == this->m_count)
			Resize(this->m_size + (this->m_size/5>=1 ? this->m_size/5:1));
		this->m_ptr[this->m_count] = c;this->m_count++;
	}
	CC & Add()
	{
		if (this->m_size == this->m_count)
			Resize(this->m_size + (this->m_size/5>=1 ? this->m_size/5:1));
		this->m_count++;
		return this->m_ptr[this->m_count-1];
	}
	/** Odebrani vsech stejnych objektu */
	void Remove(CC c)
	{
		assert(this->m_ptr);
		for (uint i=0;i < this->m_count;)
		{
			if (this->m_ptr[i] == c)
			{
				if (i < (this->m_count-1))
					this->m_ptr[i] = this->m_ptr[this->m_count-1];
				this->m_count--;
			}
			else
				i++;
		}
	}
	/** Odebrani vsech stejnych objektu */
	template <class D> CC Remove(D d)
	{
		assert(this->m_ptr);
		for (uint i=0;i < this->m_count;)
		{
			if (*this->m_ptr[i] == d)
			{
				CC ret = this->m_ptr[i];
				if (i < (this->m_count-1))
					this->m_ptr[i] = this->m_ptr[this->m_count-1];
				this->m_count--;
				return ret;
			}
			else
				i++;
		}
		return NULL;
	}
	void RemoveIndex(uint index)
	{
		if (index < (this->m_count-1))
			this->m_ptr[index] = this->m_ptr[this->m_count-1];
		this->m_count--;
	}
}; 

template<int NUM, class C> class ListSet
{
	SetBase<C> m_set[NUM];
public:

};

template<typename TYPE> int qsort_cmp(const void * a, const void * b)
{
	const TYPE* A = reinterpret_cast<const TYPE*>(a);
	const TYPE* B = reinterpret_cast<const TYPE*>(b);
	if (*A < *B)
		return -1;
	else if (*A > *B)
		return 1;
	return 0;
}

template<typename TYPE> void qsort(TYPE * base, size_t nums)
{
	::qsort(base, nums, sizeof(TYPE), qsort_cmp<TYPE>); 
}

template<class C> class List : public SetBase<C>
{
public:
	void Add(C c)
	{
		if (this->m_size == this->m_count)
			Resize(this->m_size + (this->m_size/5>=1 ? this->m_size/5:1));
		new (&this->m_ptr[this->m_count]) C(c);
		this->m_count++;
	}
	C & Add()
	{
		if (this->m_size == this->m_count)
			Resize(this->m_size + (this->m_size/5>=1 ? this->m_size/5:1));
		this->m_count++;
		return * new (&this->m_ptr[this->m_count-1]) C;
	}
    void Set(C c)
    {
        this->Delete();
        Add(c);
    }
    C& Set()
    {
        this->Delete();
        return Add();
    }
	void QSort()
	{
		qsort<C>(this->m_ptr, this->m_count);
	}

	class Iterator
	{
		List<C>& m_list;
		uint m_it;
	public:
		Iterator(List & list) : m_list(list)
		{
			m_it = 0;
		}
		void next()
		{
			if (valid())
				m_it++;
		}
		void operator ++ (int)
		{
			next();
		}
		bool valid() const
		{
			return m_it < m_list.Count();
		}
		operator bool () const
		{
			return valid();
		}
		C* operator ->()
		{
			return &(m_list[m_it]);
		}
		C& operator *()
		{
			return m_list[m_it];
		}
	};
};

template<class C> class Stack : public SetBase<C>
{
public:
	void Push(const C &c)
	{
		if (this->m_size == this->m_count)
			Resize(this->m_size + (this->m_size/5>=1 ? this->m_size/5:1));
		this->m_ptr[this->m_count] = c;this->m_count++;
	}
	C & Push()
	{
		if (this->m_size == this->m_count)
			Resize(this->m_size + (this->m_size/5>=1 ? this->m_size/5:1));
		this->m_count++;
		return this->m_ptr[this->m_count-1];
	}
	const C & Pop()
	{
		this->m_count--;
		return this->m_ptr[this->m_count];
	}
	const C & GetTop()
	{
		return this->m_ptr[this->m_count-1];
	}
};

/** 
 * Fronta
 */
template<class C> class Queue : private SetBase<C>
{
protected:
	uint m_index;
	void Reindex()
	{
		hoe_assert(m_index > 0);
		CrossMemMove(this->m_ptr,this->m_ptr+this->m_index, sizeof(C)*(this->m_count-this->m_index));
		this->m_count -= this->m_index;
		this->m_index = 0;
	}
public:
	Queue()
	{
		this->m_index = 0;
	}
	uint Count() { return this->m_count-this->m_index; }
	bool IsEmpty() { return this->m_index==this->m_count; }
	void Delete() { this->m_count = this->m_index = 0; }
	C & Get(uint n) const
	{
		hoe_assert(n < (this->m_count-this->m_index));
		return this->m_ptr[n+this->m_index];
	}
	C & operator [] (const int index)
	{
		return Get(index);
	}
	void Push(const C &c)
	{
		if (this->m_size == this->m_count)
		{
			if (this->m_index)
				Reindex();
			else
				Resize(this->m_size + (this->m_size/5>=1 ? this->m_size/5:1));
		}
		new (&this->m_ptr[this->m_count]) C(c);
		this->m_count++;
	}
	C & Push()
	{
		if (this->m_size == this->m_count)
		{
			if (m_index)
				Reindex();
			else
				Resize(this->m_size + (this->m_size/5>=1 ? this->m_size/5:1));
		}
		this->m_count++;
		return *new (&this->m_ptr[this->m_count-1]) C;
	}
	const C & Front()
	{
		hoe_assert(this->m_index < this->m_count);
		return this->m_ptr[this->m_index++];
	}
	const C * Front(uint n)
	{
		hoe_assert((this->m_index+n) <= this->m_count);
		this->m_index += n;
		return this->m_ptr + this->m_index-n;
	}
	void Remove(uint n)
	{
		hoe_assert(n < (this->m_count-this->m_index));
		if (!n)
			this->m_index++;
		else if (n==(this->m_count-this->m_index-1))
			this->m_count--;
		// vyzaduje posunuti pameti
		else if (this->m_index)
		{
			// presun prvni casti
			if (n)
				CrossMemMove(this->m_ptr, this->m_ptr + m_index,sizeof(C)*n);
			// presun druhe
			register const int tail = this->m_count-this->m_index-n-1;
			if (tail)
				CrossMemMove(this->m_ptr+n, this->m_ptr+this->m_index+n+1,sizeof(C)*tail);
			this->m_count -= (this->m_index + 1);
			this->m_index = 0;
		}
		else
		{
			if (n < (this->m_count/3)) // pokud je n v prvni tretine, presouva se index
			{
				CrossMemMove(this->m_ptr+n,this->m_ptr,sizeof(C)*n);
				this->m_index++;
			}
			else
			{
				CrossMemMove(this->m_ptr+n,this->m_ptr+n+1,sizeof(C)*(this->m_count-this->m_index-n-1));
				this->m_count--;
			}
		}
	}
};

template<typename TYPE> class LList
{
protected:
	struct LListItem
	{
		TYPE data;
		LListItem * next;
	};

	HoeCore::MemoryPool* m_pool;
	LListItem * m_first;
public:
	class Iterator
	{
		LListItem * m_it;
	public:
		Iterator(LList & list)
		{
			m_it = list.m_first;
		}
		void next()
		{
			if (m_it)
				m_it = m_it->next;
		}
		void operator ++ (int)
		{
			next();
		}
		bool valid() const
		{
			return m_it != NULL;
		}
		operator bool () const
		{
			return valid();
		}
		TYPE* operator ->()
		{
			return &(m_it->data);
		}
		TYPE& operator *()
		{
			return m_it->data;
		}
	};
public:
	LList() : m_pool(NULL), m_first(NULL) {}
	LList(HoeCore::MemoryPool& pool) : m_pool(&pool), m_first(NULL) {}
	~LList()
	{
		Delete();
	}
	TYPE & Add()
	{
		return new (this->AddForNew()) TYPE();
	}
	void * AddForNew()
	{
		LListItem * li;
		if (m_pool)
			li = (LListItem*)m_pool->GetMem(sizeof(LListItem));
		else
			li = (LListItem*)malloc(sizeof(LListItem));
		memset(li, 0, sizeof(LListItem));
		//li->first 
		li->next = m_first;
		m_first = li;
		return &li->data;
	}
	void Delete()
	{
		while (m_first)
		{
			m_first->data.~TYPE();
			LListItem * tmp = m_first->next;
			if (!m_pool)
				free(m_first);
			m_first = tmp;
		}
	}
	uint Count()
	{
		uint cnt = 0;
		for (Iterator it(*this);it;it++)
			cnt++;
		return cnt;
	}
};

/** 
 * Implementace haldy
 * prvky musi mit zabudovany operator <>
 */
template<class C> class Heap : public SetBase<C>
{
public:
    void Insert(C c)
    {
        if (this->m_size == this->m_count)
            Resize(this->m_size + (this->m_size/5>=1 ? this->m_size/5:1));
        int i=this->m_count++;
        this->m_ptr[i] = c;
        while (i > 0 && (this->m_ptr[(i-1)/2] > this->m_ptr[i]))
        {
            this->m_ptr[i] = this->m_ptr[(i-1)/2];
            i = (i-1)/2;
            this->m_ptr[i] = c;
        }
    }
    const C & GetHeap() const
    {
        return this->m_ptr[0];
    }
    void RemoveHeap()
    {
        this->m_ptr[0] = this->m_ptr[this->m_count-1];
        uint j,i; this->m_count--;
        i=0;
        // while ((2*i+1) <= this->m_count-1)
        while ((2*i+2) <= this->m_count)
        {
            j=i;
            if (2*i+1<=this->m_count-1 && this->m_ptr[j]>this->m_ptr[2*i+1])
                j=2*i+1;
            if (2*i+2<=this->m_count-1 && this->m_ptr[j]>this->m_ptr[2*i+2])
                j=2*i+2;
            if (i==j)
                break;
            C x = this->m_ptr[i]; this->m_ptr[i]=this->m_ptr[j];this->m_ptr[j]=x;
            i=j;
        }
    }
};


/** Seznam klicu, umoznuje rychle hledani  
 * C musi obsahovat operatory <> a operator == s klicem */
template<class C, class INDEX> class KeyList : public SetBase<C>
{
public:
	void Add(C c)
	{
		if (this->m_size == this->m_count)
			Resize(this->m_size + (this->m_size/5>=1 ? this->m_size/5:1));
		// vlozit serazene
		this->m_ptr[this->m_count] = c;this->m_count++;
	}
	C & Add(INDEX i)
	{
		if (this->m_size == this->m_count)
			Resize(this->m_size + (this->m_size/5>=1 ? this->m_size/5:1));
		this->m_ptr[this->m_count] = C(i);this->m_count++;
        return this->m_ptr[this->m_count-1];
	}
    C * Find(const INDEX& index)
    {
		// hledat slozitosti log n
		// todo hledat log n
        for (uint i=0;i< this->m_count;i++)
            if (this->m_ptr[i] == index)
                return this->m_ptr + i;
        return NULL;
    }
};



///////////////////////////////////////////////////
// GRAF
struct TGraphEdge;

struct TGraphVertex
{
	TGraphEdge * e1;
	TGraphEdge * e2;
	TGraphEdge * e3;
	Set<TGraphEdge *> ee;
};


struct TGraphEdge
{
	TGraphVertex * v1;
	TGraphVertex * v2;
};

// kruhovy seznam, list optimalizovany pro pridavani na konec omezenemu seznamu
// vybirani po jedne
template<class T, int size> 
class CircleList
{
	T m_items[size];
	int m_count;
	int m_ptr;
public:
	CircleList();
	int Count() const { return m_count; }
	T & GetLine(int i);
	const T & GetLine(int i) const ;
	T & AddTail();
};

template<class T, int size> 
inline CircleList<T,size>::CircleList()
{
	m_count = m_ptr = 0;
}

template<class T, int size>
inline T & CircleList<T,size>::GetLine(int i)
{
	int nn = (m_ptr + size - i) % size; 
	return m_items[nn];
}

template<class T, int size>
inline const T & CircleList<T,size>::GetLine(int i) const
{
	int nn = (m_ptr + size - i) % size; 
	return m_items[nn];
}

template<class T, int size>
inline T & CircleList<T,size>::AddTail()
{
	m_ptr++;
	if (m_ptr >= size)
		m_ptr = 0;

	if (m_count < size)
		m_count++;

	return m_items[m_ptr];
}

} // namespace HoeCore


#endif // _HOE_STRUCTURES_H_
