
/**
   @file    ref_opengl.h
   @date    Mar 2006
   @version 1.0
   @brief   Hlavni soubor definice OpenGL
*/

#ifndef _HOE_OPENGL_
#define _HOE_OPENGL_

#ifdef _MACOSX
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
//#include <GL/glu.h>

#ifdef _LINUX
#include <GL/glx.h>
#include <X11/extensions/xf86vmode.h>
#include <X11/extensions/xf86dga.h>
#endif 
#endif

#include "ref_base.h"
#include "ref_opengl_ext.h"

typedef GLuint SysTexture;
typedef GLuint SysIndexBuffer;
typedef GLuint SysVertexBuffer;

struct THoeInitSettings;

#define FVF_XYZ			(1 << 0)
#define FVF_XYZRHW		(1 << 1) 
#define FVF_NORMAL		(1 << 2)
#define FVF_DIFFUSE		(1 << 3)
#define FVF_SPECULAR	(1 << 4)
#define FVF_TEX1		(1 << 5)
#define FVF_TEX2		(1 << 6)

typedef void (*GLPROCEDURE)(void);

/**
* @brief Trida s implementaci zakladnich vlastnosti <b>OpenGL</b>
*/
class RefOpenGL : public RefBase
{
protected:
#ifdef _LINUX
	Display *m_Dpy; ///< Display
    Window m_Win; ///< Handle okna
	GLXContext m_Ctx; ///< Rendering Context
    int m_Screen; ///< Handle na screen
	static XF86VidModeModeInfo * GetMode(Display *dpy, int screen, int width, int height);
#endif

#ifdef _WIN32
	HDC m_hDC; ///< Privatni GDI Device Context
	HGLRC m_hRC; ///< Trvaly Rendering Context
	HWND m_hWnd; ///< Handle okna
#endif
	
public:
	GLExt ext;
    struct Param
    {
        GLint max_indices;
        GLint max_vertices;
    } param;
	/**
	* Konstruktor
	*/
	RefOpenGL();
	/**
	* Destruktor
	*/
	virtual ~RefOpenGL() {}
	/**
	* Inicializace DIRECTD3
	* @param his vyplnena struktura THoeInitSettings
	* @return Pokud se zdari inicializace, vraci true, jinak false.
	*/
	bool Init(THoeInitSettings *);
	/**
	* Zacatek sceny. viz OpenGL help.
	*/
	bool Begin();
	/**
	* Konec vykreslovani sceny. viz OpenGL help.
	*/
	void End();
	/**
	* Vymazani bufferu
	* @param target Smaze target buffer
	* @param depth Smaze depth buffer
	*/
	void ClearBuffers(bool target, bool depth);
	/**
	* Nastaveni barvy pozadi
	*/
	virtual void HOEAPI SetBackgroundColor(unsigned long color);
	/**
	* Vykresleni objektu. Pocet trojuhelniku se urcuje podle velikosti bufferu.
	* @param stream Vrcholy
	* @param index Indexy
	*/
	static void DrawStdObject(class HoeStream * stream, class HoeIndex * index);
	/**
	* Vykresleni objektu s nastavenim velikosti bufferu.
	* @param stream Vrcholy
	* @param index Indexy
	* @param vert Pocet platnych vrcholu
	* @param ind Pocet indexu = pocet trojuhelniku * 3
	*/
	static void DrawStdObject(HoeStream * stream, HoeIndex * index, dword vert, dword ind);
	/**
	* Vykresleni objektu s urcenim intervalu.
	* @param stream Vrcholy
	* @param index Indexy
	* @param from Zacatek kresleni
	* @param num Pocet indexu = pocet trojuhelniku * 3
	*/
	static void DrawStdObjectFT(HoeStream * stream, HoeIndex * index, dword from, dword num);

	/**
	* Vykresleni indexu s prednastavenym streamem.
	* @param index Indexy
	* @param offset Od ktereho index uzacit
	* @param ind Pocet indexu = pocet trojuhelniku * 3
	*/
	static void DrawIndex(HoeIndex * index, dword offset, dword count);
	/**
	* Vykresleni fanu.
	* @param offset Od ktereho index uzacit
	* @param count Pocet indexu = pocet trojuhelniku + 2
	*/
	static void DrawFanObject(dword offset, dword count);
	/**
	* Vykresleni car podle vrcholu.
	* @param stream Vrcholy
	* @param lineCount Pocet car
	*/
	static void DrawLineObject(class HoeStream * stream, int lineCount);
	/**
	* Vykresleni objektu tvoreneho body.
	* @param stream Body
	* @param vertCount Pocet bodu
	*/
	static void DrawPointObject(class HoeStream * stream, int vertCount);
	/** 
	* Nastaveni aktualni modelove matice.
	* @param m Matice
	*/
	static HOE_INLINE void SetMatrix(const HoeMath::Matrix & m);
	/**
	* Test na pritomnost rozsireni
	* @param ext_name Jmeno extension
	* @param extensions String extensions. Pokud je NULL funkce si zjisti rozsireni z ovladece OpenGL
	*/
	static bool TestExt(const char * ext_name,const char * extensions = NULL);
	/**
	* Vypise opengl rozisreni do konzole
	*/
	static void PrintGlExt();
	/**
	* Vrati ukazatel na proceduru opengl
	*/
	static GLPROCEDURE GetProc(const char * name);
	/**
	* Load extensions and desc
	*/
	void LoadExtensions();
	/**
	* Podporovana verze vertex shaderu
	* @return Verze shaderu jako hiword a loword
	*/
	dword HOE_INLINE GetVertexShaderVersion()
	{	
		if (ext.ARB_vertex_program)
			return makeword(1,1);
		else
			return makeword(0,0);
	}

	/**
	* Zruseni OpenGL
	*/
	void Destroy();


};

#include "ref_opengl.inl"

#endif // _HOE_OPENGL_
