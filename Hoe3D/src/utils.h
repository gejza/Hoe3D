

#ifndef _HOE_UTIL_H
#define _HOE_UTIL_H


#define HOE_INLINE inline

//-----------------------------------------------------------------------------
// Miscellaneous helper functions
//-----------------------------------------------------------------------------
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

//-----------------------------------------------------------------------------
// Debug printing support & ERRORS HTHROW
//-----------------------------------------------------------------------------

#define _tp Con_Print("Tracing point " __FILE__ " l: %d",__LINE__ );

/*void HThrowDbg(char*, DWORD, HRESULT);
void HThrow(HRESULT hr);

#if defined(DEBUG) | defined(_DEBUG)
	#define HTHROW(hr) HThrowDbg(__FILE__, (DWORD)__LINE__, hr)
#else
	#define HTHROW(hr) HThrow(hr)
#endif*/

///// Console /////////////////////
class HoeLog;

void Con_Print(const char *,...);
void Con_Print(HoeLog * log, const char * szFormat,...);
namespace HoeMath {
	class MATRIX;
};
void Con_Print(HoeMath::MATRIX &m);

#ifdef _WIN32
void d3derr(const char * file, dword line, const char * fnc, const char *ffnc, HRESULT hRes);
#define checkres(hRes,fnc) if (FAILED(hRes)) d3derr(__FILE__, __LINE__, fnc,__FUNCTION__, hRes);
#endif
void glerr(const char * file, dword line, const char * fnc, const char *ffnc,int code);
#define checkgl(fnc) { register int code;if ((code=glGetError()) != GL_NO_ERROR) glerr(__FILE__, __LINE__, fnc,__FUNCTION__, code);}


/*#define TEST_OPENGL glEnableClientState(GL_VERTEX_ARRAY);\
		checkgl("glEnableClientState");\
		glDisableClientState(GL_VERTEX_ARRAY);\
		checkgl("glDisableClientState");*/
//#define TEST_OPENGL checkgl("check error")

static const char tovelk(const char c)
{
	return ((c >= 'a' && c <= 'z') ? c - 'a' + 'A':c);
}

static int cmp(const char *s1,const char *s2)
{
	while (*s1)
	{
		if (tovelk(*s1) != tovelk(*s2))
			return 0;
		s1++;s2++;
	}
	return (*s2 == '\0');
}

// porovna string str podle vzoru ex
// pri shode vraci 0
// pokud je za vraci 1, pokud pred -1
int string_ex(const char * ex,const char * str);

/**
 * Ukonci hru
 * @param hard Neposilat ve Win quit message
 * @param code Navratovy kod programu
 */
void QuitGame(bool hard, int code);

////////////////////////////////
// Log class

class HoeLog
{
public:
	virtual void Log(const char *,...);
	virtual void Delete() = 0;
};

class HoeLogConsole : public HoeLog
{
	char m_name[256];
public:
	HoeLogConsole(const char * name);
	virtual void Log(const char *,...);
	virtual void Delete() { delete this; }
};

// use LOG(log)->Log("text");
#define LOG(x) if (x) (x)

///////////////////////////////
// Ptr list
template<class PTR> class PtrList
{
	PTR ** _first;
	uint _num;
public:
	PtrList() 
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
};

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
}; 

#include "cmdexec.h"

#endif //_HOE_UTIL_H



