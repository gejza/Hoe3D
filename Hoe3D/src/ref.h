
/**
   @file    ref.h
   @date    Mar 2006
   @version 1.0
   @brief   Deklarace tridy Ref.
*/

#ifndef _HOE_3D_REF_
#define _HOE_3D_REF_

#if (defined (_MACOSX) || defined (_LINUX)) && !defined (_USE_OPENGL_)
#define _USE_OPENGL_
#endif

#ifdef _USE_OPENGL_
#include "ref_opengl.h"
typedef RefOpenGL RefImpl;
#endif

#ifdef _USE_D3D9_
#include "ref_d3d9.h"
typedef RefD3D9 RefImpl;
#endif 

#ifdef _USE_D3D8_
#include "ref_d3d8.h"
typedef RefD3D8 RefImpl;
#endif 

#ifdef _USE_D3DM_
#include "ref_d3dmobile.h"
typedef RefD3DM RefImpl;
#endif 

/**
* @brief Nejvyssi Trida ref. 
* Zajistuje spravne nahravani a odebrani shared pointru.
* @see RefOpenGL
* @see RefD3D9
* @see RefD3D8
*/
class Ref : public RefImpl
{
public:
	/**
	* Konstruktor
	*/
	Ref();
	/**
	* Destruktor
	*/
	virtual ~Ref();
};



#endif // _HOE_3D_REF_




