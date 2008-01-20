
#ifndef _HOE_RESOURCE_DEF_FILE_
#define _HOE_RESOURCE_DEF_FILE_

#define MAKE_FOURCC(a,b,c,d) (a+(b<<8)+(c<<16)+(d<<24))

namespace HoeRes {
namespace Res { 

enum {
	IDHRESHEADER=MAKE_FOURCC('H','o','e','R'),
	IDHRESVER=108,

	IDNamespace=0x01,
	IDPicture=0x10,
	IDStream=0x20,
};

#pragma pack(push,1)

struct HeadResource
{
	uint32 id; 
	uint16 size_struct;
	uint16 version_struct;
};

struct Namespace : public HeadResource
{
	uint32 num_symbols;
};

struct MainNamespace : public Namespace
{
	uint32 flags;
};

struct Symbol
{
	char name[32];
	uint32 type;
	uint64 position;
};

// -> prvni namespace
// jine id, verze plati pro cely soubor

#pragma pack(pop)

} // namespace Res
} // namespace HoeRes

#endif // _HOE_RESOURCE_DEF_FILE_
