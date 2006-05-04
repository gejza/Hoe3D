
#ifndef _HOE_RES_FILE_
#define _HOE_RES_FILE_

#define IDRESHEADER	('H'+('o'<<8)+('e'<<16)+(' '<<24))

struct hfres_header
{
	unsigned int fileid;
	unsigned int resid;
	unsigned int ver;
	unsigned int numres;
	unsigned int numchunks;
};

struct hfres_name
{
	char name[32];
	unsigned int size;
	int	filepos;
};

struct hfres_chunk
{
	unsigned int chunkID;
	unsigned int size;
	int filepos;
};

#endif // _HOE_RES_FILE_

