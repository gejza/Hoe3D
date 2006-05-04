

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
#define ASSERT(a) // pouzivat!!!

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

#ifdef _DXERR9_H_
#define Con_PrintHRes(text,hRes) Con_Print(text ": %s",DXGetErrorString9(hRes))
#pragma comment (lib,"dxerr9.lib")

#else
#define Con_PrintHRes(text,hRes) Con_Print(text ": 0x%x",hRes)
#endif


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

#include "cmdexec.h"

#endif //_HOE_UTIL_H



