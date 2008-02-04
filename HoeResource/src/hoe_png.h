
#ifndef _HOE_RESOURCE_PNG_H_
#define _HOE_RESOURCE_PNG_H_

#include "hoe_codec.h"

typedef struct png_struct_def png_struct;
typedef struct png_info_struct png_info;

namespace HoeRes {

class PNGDecoder : public PicCodec
{
	uint m_width;
	uint m_height;
	uint m_pitch;
	HOEFORMAT m_format;
	png_struct* m_png;
	png_info* m_info;
	uint m_readline;
public:
	PNGDecoder(HoeCore::ReadStream& stream);
	virtual ~PNGDecoder();
	virtual HOEFORMAT GetFormat() { return m_format; }
	virtual uint GetPitch() { return m_pitch; }
	virtual void GetSize(THoeSizeu* size);
	virtual uint GetRow(byte* ptr);
	HOEFORMAT GetSourceFormat();
	virtual uint GetPalette(HOECOLOR * palette);
};

} // namespace HoeRes

#endif // _HOE_RESOURCE_PNG_H_
