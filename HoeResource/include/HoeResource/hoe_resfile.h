
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
	IDIndex=0x30,
	IDFont=0x40,
	IDFile=0x10000,
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
	uint32 dataformat;
	uint32 flags;
};

struct Symbol
{
	char name[32];
	uint32 type;
	uint64 position;
};

struct ChunkInfo
{
	union {
		char cid[4];
		uint32 iud;
	};
	uint32 size;
};

struct PictureInfo : public HeadResource
{
	uint32 codec;
	uint32 numchunk;	
};

struct FontInfo : public HeadResource
{
	uint32 numchunk;	

	struct FD
	{
		wchar_t ch;
		int32 size;
	};
};

struct FileInfo : public HeadResource
{
	uint64 size;
	uint32 compress;
	uint64 comp_size;

	enum
	{
		CSPlain,
		CSZlib,
	};
};

#pragma pack(pop)

const tchar * GetTypeName(uint id);


} // namespace Res

struct SymbolLink
{
  int type;
  int fn;
  size_t pos;
  const tchar* name;
};

struct SymbolFile
{
	const tchar* name;
	size_t size;
	unsigned long adler; // todo, overit 64bit
};

} // namespace HoeRes

#endif // _HOE_RESOURCE_DEF_FILE_
