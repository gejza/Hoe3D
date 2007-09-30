
#ifndef _HOE_TYPES_H_
#define _HOE_TYPES_H_

#define hoe_assert assert
#undef assert
#define assert(a) 

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

#undef T

#ifdef UNICODE
	typedef wchar_t tchar;
#define T(text) L##text
#else
	typedef char tchar;
#define T(text) text
#endif

/* pro big endian */
#if 1 // little endian
#define le_1b(i) (i)
#define be_1b(i) (i) /* prevraci */
#define le_4b(i) (i)
#define be_4b(i) (i) /* prevraci */
#endif
#define le_int(i) le_4b(i)
#define be_int(i) be_4b(i)
#define le_uint(i) le_4b(i)
#define be_uint(i) be_4b(i)
#define le_dword(i) le_4b(i)
#define be_dword(i) be_4b(i)
#define le_float(i) le_4b(i)
#define be_float(i) be_4b(i)
#define le_word(i) le_2b(i)
#define be_word(i) be_2b(i)
#define le_short(i) le_2b(i)
#define be_short(i) be_2b(i)
#define le_byte(i) le_1b(i)
#define be_byte(i) be_1b(i)

#define makeword(a,b) (((a)<<8)|(b))
#define makedword(a,b) (((a)<<16)|(b))
#define hiword(a) (((a)&0xffff0000)>>16)
#define loword(a) ((a)&0xffff)

#ifdef __cplusplus

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
		THoeRect Rect;
	};
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

// sdilena funkce pro psani do konzole
void Con_Print(const char *,...);


#endif // _HOE_TYPES_H_
