
#ifndef _HOE_RESOURCE_CODEC_H_
#define _HOE_RESOURCE_CODEC_H_

#include "hoe_res.h"

namespace HoeRes {

class PicCodec : public HoeRes::MediaStreamPic
{
	HoeCore::ReadStream& m_stream;
	bool m_closestream;
public:
	PicCodec(HoeCore::ReadStream& stream)
		: m_stream(stream.CanSeek() ? *stream.CreateReader():stream) 
	{
		m_closestream = stream.CanSeek();
	}
	virtual uint Close() 
	{
		if (m_closestream)
			m_stream.Close();
		delete this; 
		return 0; 
	}
};

PicCodec* CreatePicDecoder(HoeCore::ReadStream& stream, uint32 codec);

} // namespace HoeRes

#endif // _HOE_RESOURCE_CODEC_H
