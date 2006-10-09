
#ifndef _HOE_STRUCTURES_H_
#define _HOE_STRUCTURES_H_

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

/**
* Trida udrzujici mnozinu objektu
*/
template<class C> class Set
{
protected:
	uint m_count;
	uint m_size;
	C * m_ptr;
	void Resize(uint num)
	{
		if (!num) return;
		m_ptr = (C*)realloc(m_ptr, num * sizeof(C));
		assert(m_ptr); /*!!!*/
		m_size = num;
	}
public:
	Set()
	{
		m_count = 0;m_size = 0; m_ptr = NULL;
	}
	
	Set(uint initnum)
	{
		m_count = 0;m_size = 0; m_ptr = NULL;
		Resize(initnum);
	}
	void Add(C c)
	{
		if (m_size == m_count)
			Resize(m_size + (m_size/5>=1 ? m_size/5:1));
		m_ptr[m_count] = c;m_count++;
	}
	C & Add()
	{
		if (m_size == m_count)
			Resize(m_size + (m_size/5>=1 ? m_size/5:1));
		m_count++;
		return m_ptr[m_count-1];
	}
	C & Get(uint n)
	{
		assert(n < m_count);
		return m_ptr[n];
	}
	C & operator [] (const int index)
	{
		return Get(index);
	}
	/** Odebrani vsech stejnych objektu */
	void Remove(C c)
	{
		assert(m_ptr);
		for (uint i=0;i < m_count;)
		{
			if (m_ptr[i] == c)
			{
				if (i < (m_count-1))
					m_ptr[i] = m_ptr[m_count-1];
				m_count--;
			}
			else
				i++;
		}
	}
	/** Odebrani vsech stejnych objektu */
	template <class D> C Remove(D d)
	{
		assert(m_ptr);
		for (uint i=0;i < m_count;)
		{
			if (*m_ptr[i] == d)
			{
				C ret = m_ptr[i];
				if (i < (m_count-1))
					m_ptr[i] = m_ptr[m_count-1];
				m_count--;
				return ret;
			}
			else
				i++;
		}
		return NULL;
	}
	uint Count() { return m_count; }
	void Delete() { m_count = 0; }
	void RemoveIndex(uint index)
	{
		if (index < (m_count-1))
			m_ptr[index] = m_ptr[m_count-1];
		m_count--;
	}
	void SetCount(int count) { m_count=count; } 
}; 

template<class C> class List : public Set<C>
{
};

} // namespace HoeCore

#endif // _HOE_STRUCTURES_H_
