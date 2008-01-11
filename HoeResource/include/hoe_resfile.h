
#ifndef _HOE_RESOURCE_DEF_FILE_
#define _HOE_RESOURCE_DEF_FILE_

#define IDHRESHEADER	('H'+('o'<<8)+('e'<<16)+('R'<<24))
#define IDHRESVER 107

namespace HoeRes {
namespace Res { 

#pragma pack(push,1)

struct HeadResource
{
	uint32 id;
	uint16 size_struct;
	uint16 version_struct;
};

#pragma pack(pop)

} // namespace Res
} // namespace HoeRes

#endif // _HOE_RESOURCE_DEF_FILE_
