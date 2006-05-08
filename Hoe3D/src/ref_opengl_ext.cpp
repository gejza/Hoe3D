
// ciste opengl zdrojak

#include "system.h"
#include "utils.h"
#include "ref_opengl.h"
#include "glext.h"

namespace GLExt { 
//////////////////////////////////

Extension::Extension()
{
	supported = false;
}

bool Extension::TestExt(const char * ext_name,const char * extensions)
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

void Extension::PrintGlExt()
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

GLPROCEDURE Extension::GetProc(const char * name)
{
#ifdef _WIN32
	return (GLPROCEDURE)wglGetProcAddress(name);
#endif
#ifdef _LINUX
	return glXGetProcAddressARB((GLubyte*)name);
#endif
}

////////////////////////////////////////////////
// Compressed textures
void CompressTextures::Check()
{
	supported = TestExt("GL_ARB_texture_compression");
	if (supported)
		Con_Print("Use extension: GL_ARB_texture_compression");
	/*"GL_3DFX_texture_compression_FXT1";
	"GL_EXT_texture_compression_s3tc";
	"GL_S3_s3tc";


	GLint NumFormat = 0;
	GLint *Formats = NULL;
	glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS_ARB, &NumFormat);
	NumFormat;*/
}

///////////////////////////////////////////////
// Vertex buffer
void VertexBuffer::Check()
{
	supported = false;//TestExt("GL_ARB_vertex_buffer_object");
	if (supported)
	{
		Con_Print("Use extension: GL_ARB_vertex_buffer_object");
		glGenBuffersARB = (PFNGLGENBUFFERSARBPROC) GetProc("glGenBuffersARB");
		glBindBufferARB = (PFNGLBINDBUFFERARBPROC) GetProc("glBindBufferARB");
		glBufferDataARB = (PFNGLBUFFERDATAARBPROC) GetProc("glBufferDataARB");
		glDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC) GetProc("glDeleteBuffersARB");
	}
	else
	{
		glGenBuffersARB = NULL;// Generování VBO jména
		glBindBufferARB = NULL;// Zvolení VBO bufferu
		glBufferDataARB = NULL;// Nahrávání dat VBO
		glDeleteBuffersARB = NULL;// Mazání VBO
	}
}

};

