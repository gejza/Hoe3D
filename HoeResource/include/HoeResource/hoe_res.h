
#ifndef _HOE_RESOURCE_RES_H_
#define _HOE_RESOURCE_RES_H_

#include "hoe_resfile.h"

namespace HoeRes {

// picture, sound, video
class MediaStream : public HoeCore::BaseStream
{
public:
};

class ResourceLoader
{
protected:
	HoeCore::ReadStream* m_stream;
public:
	ResourceLoader(HoeCore::ReadStream* stream);
	~ResourceLoader();

	size_t ReadHeader(uint32 id, Res::HeadResource* head, size_t size);
};

class MediaStreamPic : public MediaStream
{
public:
	virtual HOEFORMAT GetFormat() = 0;
	virtual uint GetPitch() = 0;
	virtual void GetSize(THoeSizeu* size) = 0;
	virtual uint GetRow(byte* ptr) = 0;
	virtual uint GetPalette(HOECOLOR * palette) { return 0; }
};

class PictureLoader : public ResourceLoader
{
	uint m_codec;
public:
	PictureLoader(HoeCore::ReadStream* stream);
	MediaStreamPic * GetData();
};


class FormatConv : public HoeRes::MediaStreamPic
{
	HoeRes::MediaStreamPic * m_stream;
	byte * m_buff;
	HOECOLOR m_colors[256];
	HOEFORMAT m_inputformat;
public:
	HOECOLOR key;
	dword dwkey;

	FormatConv(HoeRes::MediaStreamPic* stream);
	virtual ~FormatConv();
	virtual HOEFORMAT GetFormat();
	virtual uint GetPitch();
	virtual void GetSize(THoeSizeu* size);
	virtual uint Close();
	virtual uint GetRow(byte* ptr);
};

} // namespace HoeRes

#endif // _HOE_RESOURCE_RES_H_
