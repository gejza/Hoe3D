
#ifndef _OBJECT_CONTROLLER_H_
#define _OBJECT_CONTROLLER_H_

#include "../include/hoeobject.h"
#include "map_utils.h"

class HoeModel;
class HoeScene;

/**
* Trida udrzujici mnozinu objektu
*/
template<class C> class PtrSet
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
	PtrSet()
	{
		m_count = 0;m_size = 0; m_ptr = NULL;
	}
	PtrSet(uint initnum)
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
	C Get(uint n)
	{
		assert(n < m_count);
		return m_ptr[n];
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
	uint Count() { return m_count; }
}; 

class ObjectController : public IHoeObjectController
{
	MapOrientation pos;

	dword flags;
public:
	XHoeObject * object;
	HoeModel * model;
	HoeMath::VECTOR3 m_scale;
	struct TSubObjectPtr
	{
		THoeSubObject::Type type;
		THoeSubObject * ptr;
	};
	PtrSet<TSubObjectPtr> m_adv; // seznam dalsich podobjektu a efektu

	ObjectController();
	bool IsValid() { return (object != NULL); }
	bool Create(XHoeObject * obj);
	void Render(const HoeScene * scene);
	//inline bool IsShow() { return flags & 0x1; }
	inline dword GetFlags() { return flags; }
	inline XHoeObject * GetObject() { return object; }
	inline const HoeMath::VECTOR3 & GetVectorPosition() { return pos.xyz; }

	virtual void HOEAPI SetModel(IHoeModel * model);
	virtual IHoeModel * HOEAPI GetModel();
	virtual bool HOEAPI LoadModel(const char * cmd);
	virtual void HOEAPI Unregister();

	virtual void HOEAPI SetPosition(const float x, const float y, const float z);
	virtual void HOEAPI SetOrientation(const float x, const float y, const float z, const float angle);
	virtual void HOEAPI GetPosition(float *x, float *y, float *z);
	virtual void HOEAPI GetOrientation(float *x, float *y, float *z, float *angle);
	virtual void HOEAPI SetScale(const float x, const float y, const float z);
	virtual void HOEAPI GetScale(float *x, float *y, float *z);
	/** @see IHoeObjectController::LinkSubObject */
	virtual void HOEAPI Link(THoeSubObject::Type type, THoeSubObject * obj);
	/** @see IHoeObjectController::UnlinkSubObject */
	virtual void HOEAPI Unlink(THoeSubObject * obj);

	virtual void HOEAPI SetFlags(unsigned long flags);
	virtual void HOEAPI UnsetFlags(unsigned long flags);
};

#endif // _OBJECT_CONTROLLER_H_

