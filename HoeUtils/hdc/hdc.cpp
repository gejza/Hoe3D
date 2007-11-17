
#include <stdio.h>
#include "../lib/hoeUtils.h"
#include "../../Hoe3D/include/hfmt/hres_file.h"
#include "../../Hoe3D/include/hfmt/hmodel_file.h"
#include "../../Hoe3D/include/hfmt/htex_file.h"

union header {
struct 
{
	unsigned int id;
	unsigned int ver;
	unsigned int numres;
	unsigned int numchunks;
} mdl106;

struct 
{
	unsigned int id;
	unsigned int ver;
	unsigned int numtex;
} tex103;

};

struct hfres_name_v106
{
	char name[32];
	unsigned int size;
	int	filepos;
};

struct hfres_chunk_v106
{
	unsigned int chunkID;
	unsigned int size;
	int filepos;
};

int main(int argc, char * argv[])
{
	HoeUtils::File file;
	if (argc == 1)
	{
		printf("Usage: hdc <file>\n");
		return 1;
	}

	if (!file.Open(argv[1],HoeUtils::File::mRead))
	{
		fprintf(stderr, "Failed open file %s for read.\n", argv[1]);
		return 1;
	}

	size_t s = file.GetSize();
	if (s == 0)
		return 0;

	char * b = new char [s];
	char * p = b;
	if (!file.Read(b,s))
	{
		fprintf(stderr, "Failed read file %s.\n", argv[1]);
		return 1;
	}

	file.Close();
	if (!file.Open(argv[1],HoeUtils::File::mWrite))
	{
		fprintf(stderr, "Failed open file %s for write.\n", argv[1]);
		return 1;
	}

	header * head = reinterpret_cast<header*>(p);
	if (head->mdl106.id == ('H'+('M'<<8)+('d'<<16)+('l'<<24)) && head->mdl106.ver == 106)
	{
		hfres_header header = { IDRESHEADER, IDMDLHEADER, IDMDLVER, 0, 0 };
		header.numres = head->mdl106.numres;
		header.numchunks = head->mdl106.numchunks;
		file.Write(&header,sizeof(header));
		p += sizeof(head->mdl106);

		for (unsigned int nr = 0; nr < header.numres; nr++)
		{
			hfres_name_v106 * n = reinterpret_cast<hfres_name_v106*>(p);
			p += sizeof(hfres_name_v106);
			n->filepos += sizeof(int);
			file.Write(n,sizeof(hfres_name_v106));
		}

		for (unsigned int nc = 0; nc < header.numchunks; nc++)
		{
			hfres_chunk_v106 * c = reinterpret_cast<hfres_chunk_v106*>(p);
			p += sizeof(hfres_chunk_v106);
			c->filepos += sizeof(int);
			file.Write(c,sizeof(hfres_chunk_v106));
		}

		file.Write(p, s - (p - b));

		printf("Model file %s convert succesfull.\n", argv[1]);
	}
	else if (head->tex103.id == ('H'+('T'<<8)+('e'<<16)+('x'<<24)) && head->tex103.ver == 103)
	{
		hfres_header header = { IDRESHEADER, IDTEXHEADER, IDTEXVER, 0, 0 };
		header.numres = head->tex103.numtex;
		header.numchunks = 0;
		file.Write(&header,sizeof(header));
		p += sizeof(head->tex103);

		for (unsigned int nr = 0; nr < header.numres; nr++)
		{
			hfres_name_v106 * n = reinterpret_cast<hfres_name_v106*>(p);
			p += sizeof(hfres_name_v106);
			n->filepos += sizeof(int) * 2;
			file.Write(n,sizeof(hfres_name_v106));
		}

		file.Write(p, s - (p - b));
		printf("Texture file %s convert succesfull.\n", argv[1]);
	
	}

	delete [] b;
	return 0;
}

