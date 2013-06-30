
/*****************************************
 * High perfOrmace enginE                *
 *                 by:                   *
 *                     Gejza Motyka      *
 *****************************************/

/**
   @file    hoe3d.h
   @version 1.0
   @brief   Hlavní soubor s definici Hoe3D
*/

#ifndef _HOE_3D_
#define _HOE_3D_

#include <HoeCore/hoe_types.h>
#include <HoeCore/hoe_math.h>
#include "hoeinterfaces.h"

#define HOESDK_VERSION  (136 + HOE_INTERFACE_VER)

namespace HoeRes {
class XResourceMgr;
};

class IHoeInterface;

struct THoeInitSettings
{
	HOE_INSTANCE hInst;
	HOE_DISPLAY * dpy;
	int screen;
	HOE_WINDOW win;
	bool forcewnd;
	bool fullscreen;
	int width;
	int height;
	int depth;
};

enum HOE_TYPE_SCENE
{
	HOETS_2D = 1,
	HOETS_NORMAL = 2,
	HOETS_GRAPH = 3,
	HOETS_FORCEDW = 0x7fffffff
};

#ifndef HOE_CMDFUNC_DEF
#define HOE_CMDFUNC_DEF
typedef int (*HOE_CMDFUNC)(int argc, const tchar * argv[], void * param);
#endif

#ifndef HOE_VAR_DEF
#define HOE_VAR_DEF
#ifndef BIT
#define BIT( num )				( 1 << ( num ) )
#endif 

enum EVarFlags
{
	TVAR_BOOL				= 1,	// variable is a boolean
	TVAR_INTEGER			= 2,	// variable is an integer
	TVAR_FLOAT				= 3,	// variable is a float
	TVAR_STR				= 4,	// variable is a string
	TVAR_SSTR				= 5,	// variable is a static string
	TVAR_STRUCT				= 6,	// variable is struct
	TVAR_ARRAY				= 7,	// variable is array
	TVAR_TYPE				= 0xff, // all types
	TVAR_MODIFIED			= BIT(9),
	TVAR_UNIVERSAL			= BIT(10),


    TVAR_SAVE               = BIT(11),
};

struct THoeVarIndex
{
	const tchar * name;
	int position;
	int flags;
	THoeVarIndex * index;
};

struct THoeVarValue {
	union {
		bool b;
		int i;
		float f;
		tchar * str;
		const tchar * cstr;
	};
};

struct THoeVar
{
	const tchar * name;
	union {
		THoeVarValue value;
		struct {
			THoeVarValue * vars;
			int size;
			const THoeVarIndex * index;
		};
	};
	int flags;
	THoeVar * next;
};
#endif

/**
 * Hlavni trida enginu
 */
class IHoeEngine : public IHoeInterface
{
public:
	/** Ukazatel na funkci k ziskani casu */
	vfloat (*SysFloatTime)(); 

	/** Init funkce */
	virtual bool HOEAPI Init(THoeInitSettings *) = 0;

	/** Funkce co vytvari interface tridy */
	virtual IHoeInterface * HOEAPI Create(const tchar *) = 0;

	/** Funkce pro pristup k systemum hoe */
	virtual IHoeSystem * HOEAPI GetSystem(HOESYSTEMS sys) = 0;

	/** Slouzi ke spusteni prikazu */
	virtual int HOEAPI exec(const tchar *) = 0;

	/** Zaregistrovani noveho prikazu */
	virtual bool HOEAPI RegisterCmd(const tchar * cmd, HOE_CMDFUNC func, void * par) = 0;
	/** Zaregistrovani var */
	virtual bool HOEAPI RegisterVar(THoeVar * var) = 0;
	
	/** Vypocita kolize a input */
	virtual void HOEAPI Process(const double) = 0;
	/** Vykresli snimek */
	virtual bool HOEAPI Frame() = 0;
	/** Pro aktualizaci okna */
	virtual bool HOEAPI Resize(unsigned int width,unsigned int height) = 0;

	virtual IHoeScene * HOEAPI CreateScene(HOE_TYPE_SCENE type) = 0;

	virtual void HOEAPI SetActiveScene(IHoeScene * scene) = 0;
	virtual IHoeScene * HOEAPI GetActiveScene() = 0;

	/** Zrusi engine */
	virtual void HOEAPI Destroy() = 0; // vymazani

	#ifdef _WIN32
	/** Metoda pro vstup přes okenní zprávy. */
	virtual LRESULT CALLBACK HOEAPI WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) = 0;
	#endif // _WIN32
	#ifdef _LINUX
	/** Matoda pro zpravy Xwindows */
	virtual bool HOEAPI XProc(XEvent * event) = 0;	
	#endif // _LINUX

	// support functions
	inline IHoeMaterialSystem * GetMaterialSystem()
	{
		IHoeMaterialSystem * ms = reinterpret_cast<IHoeMaterialSystem *>(this->GetSystem(HOE_SYS_MATERIAL));
		return ms;
	}

	inline IHoeInfo * GetInfo()
	{
		IHoeInfo * info = reinterpret_cast<IHoeInfo *>(this->GetSystem(HOE_SYS_INFO));
		return info;
	}

	inline IHoeInput * GetInput()
	{
		IHoeInput * input = reinterpret_cast<IHoeInput *>(this->GetSystem(HOE_SYS_INPUT));
		return input;
	}

	inline IHoeRef * GetRef()
	{
		IHoeRef * ref = reinterpret_cast<IHoeRef *>(this->GetSystem(HOE_SYS_REF));
		return ref;
	}
};

#define HOEF_NOINPUT	0x01
#define HOEF_NOSOUND	0x02

class IHoeEngineInfo : public IHoeInterface
{
public:
	struct InfoDevice
	{
		const char * name;
		const char * description;
	};
	struct InfoFormat
	{
	};
	struct InfoDisplay
	{
	};

	/** Vypise popisek enginu */
	virtual const char * GetEngineName() = 0;
	virtual const char * GetEngineDescription() = 0;
	virtual const char * GetEngineVersion() = 0;
	/** Ziska pocet moznych zarizeni */
	virtual const InfoDevice * GetDevice(uint i) = 0;
	virtual const InfoFormat * GetFormat(const InfoDevice * dev, uint i) = 0;
	virtual const InfoDisplay * GetDisplay(const InfoDevice * dev, const InfoFormat * fmt, uint i) = 0;
};

typedef IHoeEngine * (HOEAPI *HOE_FUNCCREATE)(int sdk_ver, XHoeConsole * con, HoeRes::XResourceMgr * fs, int flags);
typedef IHoeEngineInfo * (HOEAPI *HOE_FUNCINFO)(int sdk_ver);
typedef int (HOEAPI *HOE_FUNCGETSDKVER)();


#ifndef LIB_EXTERN
#define LIB_EXTERN
#endif
LIB_EXTERN IHoeEngine * HOEAPI CreateHoeEngine(int sdk_ver, ::XHoeConsole * con, HoeRes::XResourceMgr * fs, int flags);
LIB_EXTERN IHoeEngineInfo * HOEAPI GetEngineInfo(int sdk_ver);
LIB_EXTERN int HOEAPI GetSDKVersion();

#endif // _HOE_3D_

