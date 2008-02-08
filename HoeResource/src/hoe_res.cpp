
#include "StdAfx.h"
#include "../include/HoeResource/hoe_res.h"
#include "hoe_jpg.h"
#include "hoe_png.h"

HoeRes::ResourceLoader::ResourceLoader(HoeCore::ReadStream* stream)
: m_stream(stream)
{
}

HoeRes::ResourceLoader::~ResourceLoader()
{
	if (m_stream)
		m_stream->Close();
}

size_t HoeRes::ResourceLoader::ReadHeader(uint32 id, Res::HeadResource* head, size_t size)
{
	hoe_assert(size >= sizeof(Res::HeadResource));
	memset(head, 0, size);
	const HoeCore::Endianness& end = m_stream->GetDataFormat();
	m_stream->Read(head, sizeof(Res::HeadResource));
	// id
	if (id != end.num<uint32>(head->id))
		return 0;
	// size
	size_t tor = end.num<uint16>(head->size_struct);
	if (tor > size)
		throw;// todo
	if (tor > sizeof(Res::HeadResource))
	{
		m_stream->Read(reinterpret_cast<byte*>(head)+sizeof(Res::HeadResource),
			tor-sizeof(Res::HeadResource));
	}
	return tor;
}

// nadrazena struktura


HoeRes::PictureLoader::PictureLoader(HoeCore::ReadStream *stream)
: ResourceLoader(stream)
{
	Res::PictureInfo head;
	ReadHeader(Res::IDPicture, &head, sizeof(head));
	const HoeCore::Endianness& end = m_stream->GetDataFormat();
	// prekopirovat
	m_codec = end.num(head.codec);
}

HoeRes::MediaStreamPic* HoeRes::PictureLoader::GetData()
{
	return CreatePicDecoder(*m_stream, m_codec);
}

HoeRes::PicCodec* HoeRes::CreatePicDecoder(HoeCore::ReadStream& stream, uint32 codec)
{
	if (codec == MAKE_FOURCC('J','P','E','G'))
		return new JPEGDecoder (stream);
	if (codec == MAKE_FOURCC('P','N','G',' '))
		return new PNGDecoder (stream);
	return NULL;

}

//////////////////////////////////////////
// pic stream konvertor
HoeRes::FormatConv::FormatConv(HoeRes::MediaStreamPic* stream)
	: m_stream(stream)
{
	m_buff = new byte[stream->GetPitch()];
	// scan input format
	m_inputformat = stream->GetFormat();
	if (m_inputformat == HOE_P8)
	{
		uint np = m_stream->GetPalette(m_colors);
		// najit referencni hodnotu pro alfu a volnou barvu
		dword nap = 0;
		for (uint n=0;n < np;n++)
		{
			nap += m_colors[n].a;
		}
		byte nref = nap / np;

		// get color key
	}
}

HoeRes::FormatConv::~FormatConv()
{
	delete [] m_buff;
}

HOEFORMAT HoeRes::FormatConv::GetFormat() 
{ 
	return HOE_A1R5G5B5; 
}

uint HoeRes::FormatConv::GetPitch() 
{
	THoeSizeu s;
	m_stream->GetSize(&s);
	return s.width * 2;
}

void HoeRes::FormatConv::GetSize(THoeSizeu* size)
{ 
	m_stream->GetSize(size); 
}

uint HoeRes::FormatConv::Close()
{ 
	return 0; 
}

template<int a,int r, int g, int b> struct MaskARGB
{
	enum {
		// ~
		BlueMask = dword((1 << b) - 1) /*& ~dword((1 << b) - 1)*/,
		GreenMask = dword((1 << (b+g)) - 1) & ~dword((1 << b) - 1),
		RedMask = dword((1 << (b+g+r)) - 1) & ~dword((1 << b+g) - 1),
		AlphaMask = dword((1 << (b+g+r+a)) - 1) & ~dword((1 << b+g+r) - 1),

		BlueShift = b - 8,
		GreenShift = (g+b) - 8,
		RedShift = (g+b+r) - 8,
		AlphaShift = (g+b+r+a) - 8,
		DW = 0x8fffffff,
	};
	static inline dword getdw(HOECOLOR& c)
	{
		dword dw = 0;
		if (a > 0)
			dw  |= (c.a & ~((1 << 8-a)-1)) << AlphaShift;
		dword dwb	= (c.b & ~((1 << 8-b)-1)) >> 3; 
		dword dwg	= (c.g & ~((1 << 8-g)-1)) << GreenShift; 
		dword dwr	= (c.r & ~((1 << 8-r)-1)) << RedShift;
		return dwb | dwg | dwr;
	}
};

typedef MaskARGB<1,5,5,5> MaskA1R5G5B5;
typedef MaskARGB<0,5,6,5> MaskR5G6B5;

uint HoeRes::FormatConv::GetRow(byte* ptr)
{
	uint w = m_stream->GetRow(m_buff);
	byte * f = m_buff;
	byte * t = ptr;
	for (uint x=0;x < w;x++)
	{
		HOECOLOR c;
		if (m_inputformat == HOE_P8)
		{
			c = m_colors[*f];  f++;
		}
		else if (m_inputformat == HOE_R8G8B8)
		{
			c.r = f[0];c.g = f[1];c.b = f[2];
			c.a = 0x80;
			f += 3;
		}
		else
			hoe_assert(!"Format not found");
		const dword cc = MaskR5G6B5::getdw(c);
		t[1] = (0xff00&cc) >> 8;
		t[0] = 0xFF & cc;
		t += 2;
	}
	dwkey = MaskR5G6B5::getdw(key);
	return w;
}




