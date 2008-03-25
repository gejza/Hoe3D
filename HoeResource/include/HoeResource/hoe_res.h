
#ifndef _HOE_RESOURCE_RES_H_
#define _HOE_RESOURCE_RES_H_

#include "hoe_resfile.h"

typedef struct z_stream_s z_stream;

namespace HoeRes {

// picture, sound, video
class MediaStream : public HoeCore::BaseStream
{
public:
};

// nadrazena struktura
class ChunkCache
{
	struct Chunk : public Res::ChunkInfo
	{
		byte * data;
		size_t pos;
	};
	HoeCore::MemoryPool& m_pool;
	typedef HoeCore::LList<Chunk> ChunkList;
	ChunkList m_chunks;
	HoeCore::ReadStream* m_stream;
	bool m_ownstream;
	Chunk* FindChunk(uint32 id);
public:
	ChunkCache(HoeCore::MemoryPool& pool) : m_pool(pool), m_chunks(pool), m_stream(0),
		m_ownstream(false)
	{
	}
	~ChunkCache()
	{
		if (m_ownstream) m_stream->Close();
	}
	bool Read(HoeCore::ReadStream* stream, uint num);
	bool GetChunk(uint32 id, byte** data, uint32* size);
};

class ResourceLoader
{
protected:
	HoeCore::ReadStream* m_stream;
	HoeCore::MemoryPool m_pool;
	ChunkCache m_chunks;
public:
	ResourceLoader(HoeCore::ReadStream* stream);
	~ResourceLoader();

	size_t ReadHeader(uint32 id, Res::HeadResource* head, size_t size);

	bool GetChunk(uint32 id, byte** data, uint32* size) 
	{ 
		return m_chunks.GetChunk(id, data, size); 
	}
	template<typename TYPE> bool GetChunk(uint32 id, TYPE** t)
	{
		uint32 s;
		if (!GetChunk(id, reinterpret_cast<byte**>(t), &s)) return false;
		if (s != sizeof(TYPE)) return false;
		return true;
	}
	template<typename TYPE> bool GetChunk(uint32 id, TYPE& t)
	{
		uint32 s; byte* b;
		if (!GetChunk(id, &b, &s)) return false;
		if (s != sizeof(TYPE)) return false;
		memcpy(&t, b, sizeof(TYPE));
		return true;
	}

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

class FontLoader : public ResourceLoader
{
public:
	FontLoader(HoeCore::ReadStream* stream);
};


class FormatConv : public HoeRes::MediaStreamPic
{
	HoeRes::MediaStreamPic * m_stream;
	byte * m_buff;
	HOECOLOR m_colors[256];
	HOEFORMAT m_inputformat;
public:
	FormatConv(HoeRes::MediaStreamPic* stream);
	virtual ~FormatConv();
	virtual HOEFORMAT GetFormat();
	virtual uint GetPitch();
	virtual void GetSize(THoeSizeu* size);
	virtual uint Close();
	virtual uint GetRow(byte* ptr);
	void SetColorKey(HOECOLOR& c, byte alpharef);
};

class DoubleZoom : public HoeRes::MediaStreamPic
{
	HoeRes::MediaStreamPic * m_stream;
	int m_num;
	byte* m_buff;
	int m_readline;
	THoeSizeu m_size;
public:
	DoubleZoom(HoeRes::MediaStreamPic* stream);
	virtual ~DoubleZoom();
	virtual HOEFORMAT GetFormat() { return m_stream->GetFormat(); }
	virtual uint GetPitch() { return m_stream->GetFormat() * 2; }
	virtual void GetSize(THoeSizeu* size);
	virtual uint Close() { return 0; }
	virtual uint GetRow(byte* ptr);
};

class StreamDeflate : public HoeCore::ReadStream
{
	HoeCore::ReadStream& m_stream;
	HoeCore::Buffer m_buff;
	HoeCore::Dynamic<z_stream> m_zs;
	bool m_open;
	// buffer
public:
	StreamDeflate(HoeCore::ReadStream& stream, int level);
	virtual ~StreamDeflate();
	virtual size_t Read(void* ptr, size_t size);
	virtual uint Close(void);
};

class StreamInflate : public HoeCore::ReadStream
{
	HoeCore::ReadStream& m_stream;
	HoeCore::Buffer m_buff;
	HoeCore::Dynamic<z_stream> m_zs;
	bool m_open;
	// buffer
public:
	StreamInflate(HoeCore::ReadStream& stream);
	virtual ~StreamInflate();
	virtual size_t Read(void* ptr, size_t size);
	virtual uint Close(void);
};

} // namespace HoeRes

#endif // _HOE_RESOURCE_RES_H_
