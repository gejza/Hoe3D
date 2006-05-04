
#include "StdAfx.h"
#include "hmc.h"


int main(int argc, char * argv[])
{
	char fout[128];

	if (!VertexTest())
		return 1;

		
	if (!HoeUtilsTest())
		return 1;	

	if (argc < 2)
	{
		printf("Usage: %s <filein>\n",argv[0]);
		return 1;
	}

	HMC hmc;

	if (!hmc.Open(argv[1]))
	{
		fprintf(stderr,"failed open file %s\n",argv[1]);
		return 1;
	}

	try
	{
		if (!hmc.Parse())
		{
			printf("chyba na radku %d\n",hmc.GetActLine());
			return 1;
		}
            hmc.Finish();
        //main_buff.Parse();
	}
	catch (const char * p)
	{
		fprintf(stderr,"Error on line %d: %s\n",hmc.GetActLine(),p);
		return 2;
	}

	/*fclose(fin);*/

	if (argc > 2)
		strcpy(fout,argv[2]);
	else if (argc > 1)
		strcpy(fout,argv[1]);
	else
		strcpy(fout,"out.hm");

	HoeUtils::change_ext(fout,"hm");

	hmc.Link(fout);

	return 0;
}


