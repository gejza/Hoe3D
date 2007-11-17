
#ifndef _HOE_TEX_FILE_
#define _HOE_TEX_FILE_

#define IDTEXHEADER	('T'+('e'<<8)+('x'<<16)+(' '<<24))
#define IDTEXVER	104

enum HOEFORMAT;

struct hft_texture
{
	int width;
	int height;
	HOEFORMAT format;
	int pitch;
	int size;
	int compsize;
	int mipmap;
};

#endif // _HOE_TEX_FILE_

