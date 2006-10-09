

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



#include "cmdexec.h"

#endif //_HOE_UTIL_H



