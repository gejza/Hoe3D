

#include <stdio.h>
#include <io.h>
#include <iostream>

typedef unsigned char byte;

#include "../../Hoe3D/src/hoe_format.h"
#include "../../Hoe3D/include/hfmt/htex_file.h"
#include "../lib/hoeUtils.h"
#include "linker.h"
#include "texture.h"

using namespace HoeUtils;

bool addtexture(HTexLinker * linker, const char *file)
{
	HTexTexture * tex = new HTexTexture();
	if (!tex->Load(file))
	{
		fprintf(stderr,"File not found '%s'\n",file);
		return false;
	}

	linker->Add(tex);

	printf("addfile: %s (%s)\n",file,tex->GetInfo());
	return true;
}

int add(HTexLinker * linker, const char *name)
{
	struct _finddata_t c_file;
	long/*intptr_t*/ hFile;
	if( (hFile = _findfirst( name, &c_file )) == -1L )
	{
		printf("File not found..'%s'\n",name);
		return 0;
	}
	
	int nFiles = 0;

	do
	{
		if (c_file.attrib & _A_SUBDIR)
			continue;
			
		std::string fname = HoeUtils::GetFileDir(name) + c_file.name;

		if (!addtexture(linker,fname.c_str()))
			return -1;
		
		nFiles++;
	} while ( _findnext( hFile, &c_file ) == 0 );

	return nFiles;
}

int main(int argc,char *argv[])
{
	HoeUtils::print("HOE Texture Studio 2004\n");
	HoeUtils::print("-----------------------\n");
	
	if (argc < 3)
	{
		printf("usage: %s <output> <input1> [input2] ...\n",argv[0]);
		return 1;
	}

	HTexLinker linker;

	int numtextures = 0;

	for (int i=2;i < argc;i++)
	{
		int ret = add(&linker,argv[i]);
		if (ret == -1)
		{
			return 2;
		}
		numtextures += ret;
	}

	if (numtextures > 0)
	{
		printf("%d textures link.\n",numtextures);
	}
	else
	{
		printf("No textures to compile...\n");
		return 1;
	}

	HoeUtils::File out;
	if (!out.Open(argv[1],HoeUtils::File::mWrite))
	{
		fprintf(stderr,"Failed open output file '%s'\n",argv[1]);
		return 1;
	};

	if (!linker.Build(&out))
	{
		fprintf(stderr,"Failed link files.\n");
		return 1;
	}
	printf("Done.\n");

	out.Close();

	return 0;
}




