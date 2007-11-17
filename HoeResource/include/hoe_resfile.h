
#ifndef _HOE_RESOURCE_DEF_FILE_
#define _HOE_RESOURCE_DEF_FILE_

#define IDHRESHEADER	('H'+('o'<<8)+('e'<<16)+('R'<<24))
#define IDHRESVER 107

namespace HoeRes {

#pragma pack(push,1)

struct Head
{
	unsigned long id;
	unsigned long ver;
	unsigned long num_sumbols;
	unsigned char byteorder;
};

struct SymbolPointer 
{
	unsigned long hash;
	unsigned long str_pos;
	unsigned long long pos;
};

#pragma pack(pop)

} // namespace HoeRes

#endif // _HOE_RESOURCE_DEF_FILE_
