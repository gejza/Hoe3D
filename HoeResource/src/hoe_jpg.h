
#ifndef _HOE_RESOURCE_JPEG_H_
#define _HOE_RESOURCE_JPEG_H_

#include "../include/HoeResource/codec.h"

namespace HoeRes {

class JPEGDecoder : public PicCodec
{
	void * jpginfo;

	uint m_width;
	uint m_height;
	uint m_pitch;
	HOEFORMAT m_format;
public:
	JPEGDecoder(HoeCore::ReadStream& stream);
	virtual ~JPEGDecoder();
	virtual HOEFORMAT GetFormat() { return m_format; }
	virtual uint GetPitch() { return m_pitch; }
	virtual void GetSize(THoeSizeu* size);
	virtual uint GetRow(byte* ptr);

};

} // namespace HoeRes

#endif // _HOE_RESOURCE_JPEG_H_
