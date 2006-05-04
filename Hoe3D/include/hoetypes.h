
#ifndef _HOE_TYPES_H_
#define _HOE_TYPES_H_

#ifndef HOEAPI
#ifdef _WIN32
#define HOEAPI  __stdcall
#define CONEXEC __stdcall
#else
#define HOEAPI
#define CONEXEC
#endif
#endif

#ifdef _WIN32
typedef HWND HOE_WINDOW;
typedef HINSTANCE HOE_INSTANCE;
typedef void * HOE_DISPLAY;
#endif // _WIN32

#ifdef _LINUX

#ifdef _NOXLIB
typedef int HOE_WINDOW;
typedef int HOE_DISPLAY;
struct XEvent;
#else
typedef Display HOE_DISPLAY;
typedef Window HOE_WINDOW;
#endif

typedef void * HOE_INSTANCE;
#endif // _LINUX

#if !defined(_WIN32) && !defined(_LINUX)
#error Platform not defined
#endif

typedef unsigned long dword;
typedef unsigned short word;
typedef unsigned int  uint;
typedef unsigned char byte;

struct THoeRect
{
	float left; 
	float top; 
	float right; 
	float bottom; 
};

struct THoeBox
{
	float left;
	float right;
	float top;
	float bottom;
	float front;
	float back;
};

struct THoeParameter
{
	union {
		THoeBox box;
		THoeRect rect;
	};
};

typedef void * HOE_HANDLE;

#endif // _HOE_TYPES_H_
