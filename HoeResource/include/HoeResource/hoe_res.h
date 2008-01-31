
#ifndef _HOE_RESOURCE_RES_H_
#define _HOE_RESOURCE_RES_H_

#include "hoe_resfile.h"

namespace HoeRes {

class ResourceLoader
{
protected:
	HoeCore::ReadStream* m_stream;
public:
	ResourceLoader(HoeCore::ReadStream* stream);
	~ResourceLoader();

	size_t ReadHeader(uint32 id, Res::HeadResource* head, size_t size);
};

class PictureLoader : public ResourceLoader
{
public:
	PictureLoader(HoeCore::ReadStream* stream);
};

} // namespace HoeRes

#endif // _HOE_RESOURCE_RES_H_
