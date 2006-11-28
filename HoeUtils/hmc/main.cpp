
#include "StdAfx.h"
//#include "hmc.h"
#include "hmc.h"
#include "fvf.h"

float SysFloatTime() { return 0;}

// zpracovani parametru prikazove radky
int main(int argc, char * argv[])
{
	char fout[1024];

	if (!VertexTest())
		return 2;

	if (!HoeUtilsTest())
		return 2;	

	if (argc < 2)
	{
		printf("Usage: %s <filein>\n",argv[0]);
		return 1;
	}	

	// parser parameters
	HMC hmc;
	if (hmc.Compile(argv[1]))
		return 1;

	if (argc > 2)
		strcpy(fout,argv[2]);
	else if (argc > 1)
		strcpy(fout,argv[1]);

	HoeUtils::change_ext(fout,"hm");

	hmc.Link(fout);

	return 0;
}


