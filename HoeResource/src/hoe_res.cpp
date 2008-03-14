
#include "StdAfx.h"
#include "../include/HoeResource/hoe_res.h"
#include "../include/HoeResource/color.h"
#include "hoe_jpg.h"
#include "hoe_png.h"

HoeRes::ResourceLoader::ResourceLoader(HoeCore::ReadStream* stream)
: m_stream(stream), m_chunks(m_pool)
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

HoeRes::PictureLoader::PictureLoader(HoeCore::ReadStream *stream)
: ResourceLoader(stream)
{
	Res::PictureInfo head;
	ReadHeader(Res::IDPicture, &head, sizeof(head));
	const HoeCore::Endianness& end = m_stream->GetDataFormat();
	// prekopirovat
	m_codec = end.num(head.codec);
	uint nch = end.num(head.numchunk);
	// read chunks
	m_chunks.Read(stream, nch);
	// chunk, cache, pokud nejde na streamu skipovat -> nacitat data dovnitr
	// pokud ano, ukladat jen pointry na data (pokud velikost vetsi nez konstanta)

}

HoeRes::FontLoader::FontLoader(HoeCore::ReadStream *stream)
: ResourceLoader(stream)
{
	Res::FontInfo head;
	ReadHeader(Res::IDFont, &head, sizeof(head));
	const HoeCore::Endianness& end = m_stream->GetDataFormat();
	// prekopirovat
	uint nch = end.num(head.numchunk);
	// read chunks
	m_chunks.Read(stream, nch);
	// chunk, cache, pokud nejde na streamu skipovat -> nacitat data dovnitr
	// pokud ano, ukladat jen pointry na data (pokud velikost vetsi nez konstanta)

}

bool HoeRes::ChunkCache::Read(HoeCore::ReadStream* stream, uint num)
{
	const int lim = 32;
	const HoeCore::Endianness& end = stream->GetDataFormat();
	const bool canseek = stream->CanSeek();
	bool reqstream = false;
	size_t ft = stream->Tell();
	while (num--)
	{
		Chunk& ch = m_chunks.Add();
		stream->Read(&ch, sizeof(Res::ChunkInfo));
		ch.size = end.num(ch.size);
		if (canseek && ch.size > 32)
		{
			ch.data = NULL;
			ch.pos = stream->Tell();
			stream->Skip(ch.size);
			reqstream = true;
		}
		else
		{
			ch.pos = canseek ? stream->Tell():(size_t)-1;
			ch.data = (byte*)m_pool.GetMem(ch.size,1);
			stream->Read(ch.data, ch.size);
		}
	}
	if (reqstream)
	{
		m_stream = stream->CreateReader(ft);
		m_ownstream = m_stream != 0;
		if (!m_stream)
			m_stream = stream;
	}
	return true;
}

HoeRes::ChunkCache::Chunk* HoeRes::ChunkCache::FindChunk(uint32 id)
{
	for (ChunkList::Iterator it(m_chunks);it;it++)
	{
		if (it->iud == id)
		{
			return it;
		}
	}
	return NULL;
}

bool HoeRes::ChunkCache::GetChunk(uint32 id, byte** data, uint32* size)
{
	Chunk* chunk = FindChunk(id);
	if (!chunk) return false;
	*size = it->size;
	if (it->data == NULL)
	{
		hoe_assert(m_stream);
		it->data = (byte*)m_pool.GetMem(it->size,4);
		m_stream->Read(it->data, it->size);
	}
	*data = it->data;
	return true;
}

size_t HoeRes::ChunkCache::GetChunkSize(uint32 id)
{
	Chunk* chunk = FindChunk(id);
	if (!chunk) return 0;
	return chunk->size;
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

uint HoeRes::FormatConv::GetRow(byte* ptr)
{
	uint w = m_stream->GetRow(m_buff);
	byte * f = m_buff;
	byte * t = ptr;
	HoeRes::ColorConv conv(m_inputformat, HOE_R5G6B5);
	if (m_inputformat == HOE_P8)
		conv.SetSrcPalette(m_colors, 256);

	for (uint x=0;x < w;x++)
	{
		conv.Conv(t,f);
		t += conv.NumDestByte();
		f += conv.NumSrcByte();
	}
	return w;
}

void HoeRes::FormatConv::SetColorKey(HOECOLOR& c, byte alpharef)
{
	if (m_inputformat == HOE_P8)
	{
		for (int i=0;i < 256;i++)
			if (m_colors[i].a < alpharef)
				m_colors[i] = c;
	}
	else
	{
		hoe_assert(!"not implemented");
	}
}

//////////////////////////////////////////
// zoom konvertor
HoeRes::DoubleZoom::DoubleZoom(HoeRes::MediaStreamPic* stream)
	: m_stream(stream)
{
	m_num = 2;
	m_buff = new byte[stream->GetPitch()];
	m_readline = 0;
	stream->GetSize(&m_size);
}

HoeRes::DoubleZoom::~DoubleZoom()
{
	delete [] m_buff;
}

void HoeRes::DoubleZoom::GetSize(THoeSizeu* size)
{
	size->width = m_size.width * 2;
	size->height = m_size.height * 2;
}

uint HoeRes::DoubleZoom::GetRow(byte* ptr)
{
	if (m_readline >= (m_size.height * 2))
		return 0;
	if (m_readline % 2 == 0)
	{
		m_stream->GetRow(m_buff);
	}

	byte * pb = m_buff;
	for (int i=m_size.width;i > 0;i--)
	{
		ptr[0] = ptr[2] = pb[0];
		ptr[1] = ptr[3] = pb[1];
		ptr += 4;
		pb += 2;
	}

	m_readline++;
	return m_size.width * 2;
}
