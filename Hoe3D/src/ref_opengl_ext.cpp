
// ciste opengl zdrojak

#include "system.h"
#include "utils.h"
#include "ref_opengl.h"
#include "glext.h"

// multitexture
/*PFNGLACTIVETEXTUREARBPROC glActiveTextureARB = NULL;
PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB = NULL;
PFNGLMULTITEXCOORD1DARBPROC glMultiTexCoord1dARB = NULL;
PFNGLMULTITEXCOORD1DVARBPROC glMultiTexCoord1dvARB = NULL;
PFNGLMULTITEXCOORD1FARBPROC glMultiTexCoord1fARB = NULL;
PFNGLMULTITEXCOORD1FVARBPROC glMultiTexCoord1fvARB = NULL;
PFNGLMULTITEXCOORD1IARBPROC glMultiTexCoord1iARB = NULL;
PFNGLMULTITEXCOORD1IVARBPROC glMultiTexCoord1ivARB = NULL;
PFNGLMULTITEXCOORD1SARBPROC glMultiTexCoord1sARB = NULL;
PFNGLMULTITEXCOORD1SVARBPROC glMultiTexCoord1svARB = NULL;
PFNGLMULTITEXCOORD2DARBPROC glMultiTexCoord2dARB = NULL;
PFNGLMULTITEXCOORD2DVARBPROC glMultiTexCoord2dvARB = NULL;
PFNGLMULTITEXCOORD2FARBPROC glMultiTexCoord2fARB = NULL;
PFNGLMULTITEXCOORD2FVARBPROC glMultiTexCoord2fvARB = NULL;
PFNGLMULTITEXCOORD2IARBPROC glMultiTexCoord2iARB = NULL;
PFNGLMULTITEXCOORD2IVARBPROC glMultiTexCoord2ivARB = NULL;
PFNGLMULTITEXCOORD2SARBPROC glMultiTexCoord2sARB = NULL;
PFNGLMULTITEXCOORD2SVARBPROC glMultiTexCoord2svARB = NULL;
PFNGLMULTITEXCOORD3DARBPROC glMultiTexCoord3dARB = NULL;
PFNGLMULTITEXCOORD3DVARBPROC glMultiTexCoord3dvARB = NULL;
PFNGLMULTITEXCOORD3FARBPROC glMultiTexCoord3fARB = NULL;
PFNGLMULTITEXCOORD3FVARBPROC glMultiTexCoord3fvARB = NULL;
PFNGLMULTITEXCOORD3IARBPROC glMultiTexCoord3iARB = NULL;
PFNGLMULTITEXCOORD3IVARBPROC glMultiTexCoord3ivARB = NULL;
PFNGLMULTITEXCOORD3SARBPROC glMultiTexCoord3sARB = NULL;
PFNGLMULTITEXCOORD3SVARBPROC glMultiTexCoord3svARB = NULL;
PFNGLMULTITEXCOORD4DARBPROC glMultiTexCoord4dARB = NULL;
PFNGLMULTITEXCOORD4DVARBPROC glMultiTexCoord4dvARB = NULL;
PFNGLMULTITEXCOORD4FARBPROC glMultiTexCoord4fARB = NULL;
PFNGLMULTITEXCOORD4FVARBPROC glMultiTexCoord4fvARB = NULL;
PFNGLMULTITEXCOORD4IARBPROC glMultiTexCoord4iARB = NULL;
PFNGLMULTITEXCOORD4IVARBPROC glMultiTexCoord4ivARB = NULL;
PFNGLMULTITEXCOORD4SARBPROC glMultiTexCoord4sARB = NULL;
PFNGLMULTITEXCOORD4SVARBPROC glMultiTexCoord4svARB = NULL;
*/
PFNGLPROGRAMLOCALPARAMETER4FVARBPROC glProgramLocalParameter4fvARB = NULL;
PFNGLGENPROGRAMSARBPROC glGenProgramsARB = NULL;
PFNGLBINDPROGRAMARBPROC glBindProgramARB = NULL;
PFNGLPROGRAMSTRINGARBPROC glProgramStringARB = NULL;

PFNGLGENBUFFERSARBPROC glGenBuffersARB = NULL;// Generování VBO jména
PFNGLBINDBUFFERARBPROC glBindBufferARB = NULL;// Zvolení VBO bufferu
PFNGLBUFFERDATAARBPROC glBufferDataARB = NULL;// Nahrávání dat VBO
PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB = NULL;// Mazání VBO

// compiled arrays
PFNGLLOCKARRAYSEXTPROC glLockArraysEXT = NULL;
PFNGLUNLOCKARRAYSEXTPROC glUnlockArraysEXT = NULL;

#define check(p) assert((p)!=NULL)

//////////////////////////////////

bool RefOpenGL::TestExt(const char * ext_name,const char * extensions)
{
	const char * pe;
	if (extensions)
		pe = extensions;
	else
		pe = (const char *) glGetString(GL_EXTENSIONS);

	const char * en = ext_name;
	while (*pe)
	{
		while (*pe && *pe != ' ' && *en)
		{
			if (*pe != *en)
				break;
			pe++;
			en++;
		}
		if ((*pe == ' ' || *pe == '\0') && *en == '\0')
			return true;
		en = ext_name; // reset ukazatele..
		// odskoci si na zacatek dalsi
        while (*pe && *pe != ' ') pe++;
		if (*pe == '\0')
			break;
		else
			*pe++;
	}
	return false;
}

void RefOpenGL::PrintGlExt()
{
	//
	char buff[256];
	char * pe = (char *) glGetString(GL_EXTENSIONS);
	char * pb = buff;
    while (*pe)
	{
		if (*pe == ' ')
		{
			pe++;
			*pb = '\0';
			Con_Print(buff);
			pb = buff;
		}
		else
		{
			*pb = *pe;
			pb++;
			pe++;
		}
	}

}

GLPROCEDURE RefOpenGL::GetProc(const char * name)
{
	GLPROCEDURE proc = NULL;
#ifdef _WIN32
	proc = (GLPROCEDURE)wglGetProcAddress(name);
#endif
#ifdef _LINUX
	proc = glXGetProcAddress((GLubyte*)name);
#endif
	assert(proc && "Procedure not found");
	return proc;
}


void RefOpenGL::LoadExtensions()
{
	memset(&ext, 0, sizeof(ext));
	// Compressed textures
	ext.ARB_multitexture = TestExt("GL_ARB_multitexture");
	ext.EXT_texture_env_combine = TestExt("GL_EXT_texture_env_combine");
	ext.ARB_texture_compression = TestExt("GL_ARB_texture_compression");
	ext.ARB_vertex_buffer_object = TestExt("GL_ARB_vertex_buffer_object");
	ext.EXT_compiled_vertex_array = TestExt("GL_EXT_compiled_vertex_array");
	ext.ARB_vertex_program = TestExt("GL_ARB_vertex_program");

	if (ext.ARB_texture_compression)
		Con_Print("Use extension: GL_ARB_texture_compression");
	/*"GL_3DFX_texture_compression_FXT1";
	"GL_EXT_texture_compression_s3tc";
	"GL_S3_s3tc";
	GLint NumFormat = 0;
	GLint *Formats = NULL;
	glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS_ARB, &NumFormat);
	NumFormat;*/
	
	if (ext.ARB_multitexture)
	{
		glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB, &ext.MAX_TEXTURE_UNITS_ARB);
		Con_Print("Use extension: GL_ARB_multitexture, %d texture units", ext.MAX_TEXTURE_UNITS_ARB);
		/*glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC) GetProc("glActiveTextureARB");
		glClientActiveTextureARB = (PFNGLCLIENTACTIVETEXTUREARBPROC) GetProc("glClientActiveTextureARB");
		glMultiTexCoord1dARB = (PFNGLMULTITEXCOORD1DARBPROC) GetProc("glMultiTexCoord1dARB");
		glMultiTexCoord1dvARB = (PFNGLMULTITEXCOORD1DVARBPROC) GetProc("glMultiTexCoord1dvARB");
		glMultiTexCoord1fARB = (PFNGLMULTITEXCOORD1FARBPROC) GetProc("glMultiTexCoord1fARB");
		glMultiTexCoord1fvARB = (PFNGLMULTITEXCOORD1FVARBPROC) GetProc("glMultiTexCoord1fvARB");
		glMultiTexCoord1iARB = (PFNGLMULTITEXCOORD1IARBPROC) GetProc("glMultiTexCoord1iARB");
		glMultiTexCoord1ivARB = (PFNGLMULTITEXCOORD1IVARBPROC) GetProc("glMultiTexCoord1ivARB");
		glMultiTexCoord1sARB = (PFNGLMULTITEXCOORD1SARBPROC) GetProc("glMultiTexCoord1sARB");
		glMultiTexCoord1svARB = (PFNGLMULTITEXCOORD1SVARBPROC) GetProc("glMultiTexCoord1svARB");
		glMultiTexCoord2dARB = (PFNGLMULTITEXCOORD2DARBPROC) GetProc("glMultiTexCoord2dARB");
		glMultiTexCoord2dvARB = (PFNGLMULTITEXCOORD2DVARBPROC) GetProc("glMultiTexCoord2dvARB");
		glMultiTexCoord2fARB = (PFNGLMULTITEXCOORD2FARBPROC) GetProc("glMultiTexCoord2fARB");
		glMultiTexCoord2fvARB = (PFNGLMULTITEXCOORD2FVARBPROC) GetProc("glMultiTexCoord2fvARB");
		glMultiTexCoord2iARB = (PFNGLMULTITEXCOORD2IARBPROC) GetProc("glMultiTexCoord2iARB");
		glMultiTexCoord2ivARB = (PFNGLMULTITEXCOORD2IVARBPROC) GetProc("glMultiTexCoord2ivARB");
		glMultiTexCoord2sARB = (PFNGLMULTITEXCOORD2SARBPROC) GetProc("glMultiTexCoord2sARB");
		glMultiTexCoord2svARB = (PFNGLMULTITEXCOORD2SVARBPROC) GetProc("glMultiTexCoord2svARB");
		glMultiTexCoord3dARB = (PFNGLMULTITEXCOORD3DARBPROC) GetProc("glMultiTexCoord3dARB");
		glMultiTexCoord3dvARB = (PFNGLMULTITEXCOORD3DVARBPROC) GetProc("glMultiTexCoord3dvARB");
		glMultiTexCoord3fARB = (PFNGLMULTITEXCOORD3FARBPROC) GetProc("glMultiTexCoord3fARB");
		glMultiTexCoord3fvARB = (PFNGLMULTITEXCOORD3FVARBPROC) GetProc("glMultiTexCoord3fvARB");
		glMultiTexCoord3iARB = (PFNGLMULTITEXCOORD3IARBPROC) GetProc("glMultiTexCoord3iARB");
		glMultiTexCoord3ivARB = (PFNGLMULTITEXCOORD3IVARBPROC) GetProc("glMultiTexCoord3ivARB");
		glMultiTexCoord3sARB = (PFNGLMULTITEXCOORD3SARBPROC) GetProc("glMultiTexCoord3sARB");
		glMultiTexCoord3svARB = (PFNGLMULTITEXCOORD3SVARBPROC) GetProc("glMultiTexCoord3svARB");
		glMultiTexCoord4dARB = (PFNGLMULTITEXCOORD4DARBPROC) GetProc("glMultiTexCoord4dARB");
		glMultiTexCoord4dvARB = (PFNGLMULTITEXCOORD4DVARBPROC) GetProc("glMultiTexCoord4dvARB");
		glMultiTexCoord4fARB = (PFNGLMULTITEXCOORD4FARBPROC) GetProc("glMultiTexCoord4fARB");
		glMultiTexCoord4fvARB = (PFNGLMULTITEXCOORD4FVARBPROC) GetProc("glMultiTexCoord4fvARB");
		glMultiTexCoord4iARB = (PFNGLMULTITEXCOORD4IARBPROC) GetProc("glMultiTexCoord4iARB");
		glMultiTexCoord4ivARB = (PFNGLMULTITEXCOORD4IVARBPROC) GetProc("glMultiTexCoord4ivARB");
		glMultiTexCoord4sARB = (PFNGLMULTITEXCOORD4SARBPROC) GetProc("glMultiTexCoord4sARB");
		glMultiTexCoord4svARB = (PFNGLMULTITEXCOORD4SVARBPROC) GetProc("glMultiTexCoord4svARB");*/
	}
	else
		ext.MAX_TEXTURE_UNITS_ARB = 1;

	if (ext.EXT_texture_env_combine)
	{
		Con_Print("Use extension: GL_EXT_texture_env_combine");
	}

	// Vertex buffer
	if (ext.ARB_vertex_buffer_object)
	{
		Con_Print("Use extension: GL_ARB_vertex_buffer_object");
		glGenBuffersARB = (PFNGLGENBUFFERSARBPROC) GetProc("glGenBuffersARB");
		glBindBufferARB = (PFNGLBINDBUFFERARBPROC) GetProc("glBindBufferARB");
		glBufferDataARB = (PFNGLBUFFERDATAARBPROC) GetProc("glBufferDataARB");
		glDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC) GetProc("glDeleteBuffersARB");
	}
	// Vertex shader
	if (ext.ARB_vertex_program)
	{
		Con_Print("Use extension: GL_ARB_vertex_program");
		glProgramLocalParameter4fvARB = (PFNGLPROGRAMLOCALPARAMETER4FVARBPROC) GetProc("glProgramLocalParameter4fvARB");
		glGenProgramsARB = (PFNGLGENPROGRAMSARBPROC) GetProc("glGenProgramsARB");
		glBindProgramARB = (PFNGLBINDPROGRAMARBPROC) GetProc("glBindProgramARB");
		glProgramStringARB = (PFNGLPROGRAMSTRINGARBPROC) GetProc("glProgramStringARB");
	}

	// locked arrays
	if (ext.EXT_compiled_vertex_array)
	{
		glLockArraysEXT = (PFNGLLOCKARRAYSEXTPROC) GetProc("glLockArraysEXT");
		glUnlockArraysEXT = (PFNGLUNLOCKARRAYSEXTPROC) GetProc("glUnlockArraysEXT");	
		Con_Print("Use extension: GL_EXT_compiled_vertex_array");
	}

}


