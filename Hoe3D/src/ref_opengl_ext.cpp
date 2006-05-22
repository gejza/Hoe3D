
// ciste opengl zdrojak

#include "system.h"
#include "utils.h"
#include "ref_opengl.h"
#include "glext.h"

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
	proc = glXGetProcAddressARB((GLubyte*)name);
#endif
	assert(proc && "Procedure not found");
	return proc;
}


void RefOpenGL::LoadExtensions()
{
	memset(&ext, 0, sizeof(ext));
	// Compressed textures
	ext.ARB_texture_compression = TestExt("GL_ARB_texture_compression");
	ext.ARB_vertex_buffer_object = TestExt("GL_ARB_vertex_buffer_object");
	if (!ext.ARB_vertex_buffer_object)
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


