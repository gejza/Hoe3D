
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

template<class C> class SetBase
{
protected:
	uint m_count;
	uint m_size;
	C * m_ptr;
	void Resize(uint num)
	{
		if (!num) return;
		m_ptr = (C*)realloc(m_ptr, num * sizeof(C));
		hoe_assert(m_ptr); /*!!!*/
		m_size = num;
	}
public:
	SetBase()
	{
		m_count = 0;m_size = 0; m_ptr = NULL;
	}
	SetBase(uint initnum)
	{
		m_count = 0;m_size = 0; m_ptr = NULL;
		Resize(initnum);
	}
	SetBase(const SetBase & base)
	{
		m_count = 0;m_size = 0; m_ptr = NULL;
		Copy(base);
	}
	~SetBase()
	{
		if (m_ptr) free(m_ptr);
	}
	uint Count() { return m_count; }
	void Delete() { m_count = 0; }
	void SetCount(int count) { m_count=count; } 
	C & Get(uint n)
	{
		assert(n < m_count);
		return m_ptr[n];
	}
	C & operator [] (const int index)
	{
		return Get(index);
	}
	bool IsEmpty() { return m_count == 0; }
	void Copy(const SetBase & base)
	{
		if (base.m_count > m_count)
			Resize(base.m_count);
		memcpy(m_ptr, base.m_ptr, base.m_count * sizeof(C));
	}
	const SetBase & operator = (const SetBase & base)
	{
		Copy(base);
		return *this;
	}

};

/**
* Trida udrzujici mnozinu objektu
*/
template<class C> class Set : public SetBase<C>
{
public:
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
	void RemoveIndex(uint index)
	{
		if (index < (m_count-1))
			m_ptr[index] = m_ptr[m_count-1];
		m_count--;
	}
}; 

template<class C> class List : public SetBase<C>
{
public:
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
};

template<class C> class Stack : public SetBase<C>
{
public:
	void Push(const C &c)
	{
		if (m_size == m_count)
			Resize(m_size + (m_size/5>=1 ? m_size/5:1));
		m_ptr[m_count] = c;m_count++;
	}
	C & Push()
	{
		if (m_size == m_count)
			Resize(m_size + (m_size/5>=1 ? m_size/5:1));
		m_count++;
		return m_ptr[m_count-1];
	}
	const C & Pop()
	{
		m_count--;
		return m_ptr[m_count];
	}
};

} // namespace HoeCore

namespace HoeMath {

template<class TYPE> struct PolygonT
{
	HoeCore::List<TYPE> points;
};

typedef PolygonT<Vector2> Polygon2;

}

#endif // _HOE_STRUCTURES_H_
