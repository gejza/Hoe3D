
#include "StdAfx.h"
#include "../../src/hoe3d_math.h"
#include "../../src/hmap_file.h"
#include "hmap.h"
#include "map.h"

int main(int argc,char *argv[])
{
	HoeUtils::XMLParser par;
	HoeUtils::File fout;
	Map map;

	HoeUtils::XMLNode * node = par.ParseFile("example.xml");
	if (!node)
		return 1;

	try {
		if (!map.Process(node))
			throw "Process failed";
	}
	catch (const char * str)
	{
		printf("error: %s\n",str);
		return 1;
	}

	fout.Open("map.out",HoeUtils::File::mWrite);
	map.SaveToFile(&fout);

	return 0;
}
