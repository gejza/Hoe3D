
#include "system.h"
#include "utils.h"
#include "ref_opengl.h"
#include "glext.h"


#ifdef _HOE_OPENGL_

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



#endif // _HOE_OPENGL_

