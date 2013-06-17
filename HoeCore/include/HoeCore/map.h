
#ifndef _HOE_CORE_MAP_H_
#define _HOE_CORE_MAP_H_

#include "hoe_structures.h"

namespace HoeCore {

template<typename TYPE, typename KEY> class Map : public SetBase<TYPE>
{
public:
	TYPE* Find(const KEY key)
	{
		for (size_t i=0;i < this->m_count;i++)
			if (this->m_ptr[i] == key)
				return this->m_ptr + i;
		return NULL;
	}
	const TYPE* Find(const KEY key) const
	{
		for (size_t i=0;i < this->m_count;i++)
			if (this->m_ptr[i] == key)
				return this->m_ptr + i;
		return NULL;
	}
	bool IsExist(const KEY key)
	{
		return this->Find(key) != NULL;
	}
	TYPE& operator [] (const KEY key)
	{
		// find
		TYPE* t = this->Find(key);
		if (t) return *t;
		// new
		if (this->m_size == this->m_count)
			this->Resize(this->m_size + (this->m_size/5>=1 ? this->m_size/5:1));
		this->m_count++;
		return *new (&this->m_ptr[this->m_count-1]) TYPE(key);
	}
	const TYPE& operator [] (const KEY key) const
	{
		// find
		const TYPE* t = this->Find(key);
		return *t;
		// vyjimku...
	}

	// iterator
	class Iterator
	{
		const Map& m_map;
		uint m_it;
	public:
		Iterator(const Map & map) : m_map(map)
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
			return m_it < m_map.Count();
		}
		operator bool () const
		{
			return valid();
		}
		TYPE* operator ->()
		{
			return m_map.m_ptr + m_it;
		}
		TYPE& operator *()
		{
			return m_map.m_ptr[m_it];
		}
		TYPE* operator &()
		{
			return m_map.m_ptr + m_it;
		}

	};

	friend class Iterator;
};

} // namespace HoeCore

#endif // _HOE_CORE_MAP_H_
