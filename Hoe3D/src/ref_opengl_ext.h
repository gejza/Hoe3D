
/**
   @file    ref_opengl_ext.h
   @date    May 2006
   @version 1.0
   @brief   Rozsireni OpenGL
*/

#ifndef _HOE_OPENGL_EXT_H_
#define _HOE_OPENGL_EXT_H_

#include "glext.h"

struct GLExt
{
	bool vb;
	bool vs;
	bool comp;
};

extern PFNGLPROGRAMLOCALPARAMETER4FVARBPROC glProgramLocalParameter4fvARB;
extern PFNGLGENPROGRAMSARBPROC glGenProgramsARB;
extern PFNGLBINDPROGRAMARBPROC glBindProgramARB;
extern PFNGLPROGRAMSTRINGARBPROC glProgramStringARB;

extern PFNGLGENBUFFERSARBPROC glGenBuffersARB;// Generování VBO jména
extern PFNGLBINDBUFFERARBPROC glBindBufferARB;// Zvolení VBO bufferu
extern PFNGLBUFFERDATAARBPROC glBufferDataARB;// Nahrávání dat VBO
extern PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB;// Mazání VBO

#endif // _HOE_OPENGL_EXT_H_

