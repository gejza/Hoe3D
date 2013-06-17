
#ifndef _HOE_TYPES_H_
#define _HOE_TYPES_H_

#define hoe_assert assert
#define not_implemented(str) assert(0)

#ifndef HOEAPI
#ifdef _WIN32
#define HOEAPI  __stdcall
#define CONEXEC __stdcall
#define FCALL   __fastcall
#else
#define HOEAPI
#define CONEXEC
#define FCALL
#endif
#endif

#ifdef _WIN32
typedef HWND HOE_WINDOW;
typedef HINSTANCE HOE_INSTANCE;
typedef void * HOE_DISPLAY;
#endif // _WIN32

#ifdef _LINUX

#if defined(_NOXLIB)
typedef int HOE_WINDOW;
typedef int HOE_DISPLAY;
//typedef union _XEvent XEvent
#else
typedef Display HOE_DISPLAY;
typedef Window HOE_WINDOW;
#endif

typedef void * HOE_INSTANCE;
#endif // _LINUX

#if defined(_MACOSX)
typedef WindowRef HOE_WINDOW;
typedef void * HOE_DISPLAY;
//typedef union _XEvent XEvent
typedef void * HOE_INSTANCE;
#endif

#if !defined(_WIN32) && !defined(_LINUX) && !defined(_MACOSX)
#error Platform not defined
#endif

typedef unsigned int dword;
typedef unsigned short word;
typedef unsigned int  uint;
typedef unsigned char byte;

#ifdef _WIN32
typedef __int32 int32;
typedef __int16 int16;
typedef __int64 int64;
typedef unsigned __int32 uint32;
typedef unsigned __int16 uint16;
typedef unsigned __int64 uint64;
#else
typedef int int32;
typedef short int16;
typedef long long int64;
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned long long uint64;
#endif

#undef T

#ifdef _UNICODE
	typedef wchar_t tchar;
#error Unicode not supported
#define TWCHAR(text) L ## text
#define T(a) TWCHAR(a)
#else
	typedef char tchar;
#define T(text) text
#endif

#define makeword(a,b) (((a)<<8)|(b))
#define makedword(a,b) (((a)<<16)|(b))
#define hiword(a) (((a)&0xffff0000)>>16)
#define loword(a) ((a)&0xffff)

#ifdef __cplusplus

#ifdef _WIN32_WCE
#include "hoe_fixed.h"

typedef HoeMath::fixed vfloat;
typedef HoeMath::fixed vdouble;
#else
typedef float vfloat;
typedef double vdouble;
#endif

struct THoeRect
{
	vfloat left; 
	vfloat top; 
	vfloat right; 
	vfloat bottom; 
};

template<typename TYPE> struct THoeSize
{
	TYPE height; 
	TYPE width; 
};

typedef THoeSize<int> THoeSizei;
typedef THoeSize<uint> THoeSizeu;
typedef THoeSize<float> THoeSizef;
typedef THoeSize<vfloat> THoeSizev;
typedef THoeSize<vdouble> THoeSizevd;
typedef THoeSize<double> THoeSized;

struct THoeBox
{
	vfloat left;
	vfloat right;
	vfloat top;
	vfloat bottom;
	vfloat front;
	vfloat back;
};

#endif

#ifndef HOE_INLINE
#define HOE_INLINE inline
#endif // HOE_INLINE

typedef void * HOE_HANDLE;

#ifdef __cplusplus

template<class TYPE> HOE_INLINE void ConstPointerAdd(TYPE &p, uint stride)
{
	p = reinterpret_cast<TYPE>(reinterpret_cast<const byte*>(p)+stride);
}

#endif // __cplusplus

#endif // _HOE_TYPES_H_
