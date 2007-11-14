
#include "StdAfx.h"
#include "utils.h"
#include "hoe_time.h"
#include "../include/hoeinterfaces.h"
#include "hoe_info.h"
#include "shared.h"


#ifdef _LINUX
#include <sys/time.h>
#endif

////////// Timer /////////////////
#ifdef _WIN32
static LONGLONG g_time_QPFTicksPerSec = 0;
static bool g_useQPC = false; 

bool time_initQPC()
{
	LARGE_INTEGER qwTicksPerSec;
	BOOL ret = FALSE;
    g_useQPC = QueryPerformanceFrequency( &qwTicksPerSec ) != FALSE;
    if( g_useQPC )
	{
		g_time_QPFTicksPerSec = qwTicksPerSec.QuadPart;
		Con_Print("High-resolution timer init.");
	}
	return g_useQPC;
}
#endif // _WIN32

//#ifndef HOE_STATIC_ENGINE
vfloat SysFloatTime()
{
#ifdef _WIN32
#ifndef _WIN32_WCE
	static bool b = false;//time_initQPC();

	if (g_useQPC)
	{
		LARGE_INTEGER qwTime; 

		QueryPerformanceCounter( &qwTime );

		return (vfloat) qwTime.QuadPart / (vfloat) g_time_QPFTicksPerSec;
	}
	else
#endif
		return (vfloat)(/*GetTickCount()*/ timeGetTime() * 0.001f);
#endif // _WIN32
#ifdef _LINUX

	struct timeval ctime;
    //struct timezone dummy;

	gettimeofday(&ctime, NULL);

	return ((ctime.tv_sec % 100000) + ctime.tv_usec * 0.000001f);
#endif // _LINUX
}
//#endif


TimeMeter::TimeMeter(const char * _name, bool autoregister)
{
	name = _name;
	if (autoregister)
		::GetInfo()->RegisterTimeMeter(this);

}

void TimeMeter::GetInfo(char * buff)
{
	sprintf(buff,"%s - %.4f%%%% %.3fms %.3fms",this->name,this->percent,this->avarage,this->pubmax);
}

