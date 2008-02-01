
#ifndef _HOE_RESOURCE_CODEC_H_
#define _HOE_RESOURCE_CODEC_H_

#include "../include/HoeResource/hoe_res.h"

namespace HoeRes {

class PicCodec : public HoeRes::MediaStreamPic
{
public:
	PicCodec(HoeCore::ReadStream& stream) {}
	virtual uint Close() { delete this; return 0; }
};

} // namespace HoeRes

#endif // _HOE_RESOURCE_CODEC_H
