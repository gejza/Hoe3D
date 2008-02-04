
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
	m_format = (HOEFORMAT)end.num(head.format);

}

HoeRes::MediaStreamPic* HoeRes::PictureLoader::GetData()
{
	if (m_codec == MAKE_FOURCC('J','P','E','G'))
		return new JPEGDecoder (*m_stream);
	if (m_codec == MAKE_FOURCC('P','N','G',' '))
		return new PNGDecoder (*m_stream);
	return NULL;
}

// prohledani do sirky?
struct Reli
{
	HOECOLOR min;
	HOECOLOR max;
	float destmin;
	float destmax;
};

float GetDist(HOECOLOR& a, HOECOLOR& b)
{
	float r = float(a.r) - float(b.r);
	float g = float(a.g) - float(b.g);
	float bb = float(a.b) - float(b.b);
	return sqrtf(r*r+g*g+bb*bb);
}


float GetMinDist(HOECOLOR& min, HOECOLOR& max, HOECOLOR& c)
{
#define GET_INTE(sl) if (min.sl > c.sl) m.sl = min.sl; else if (max.sl < c.sl) m.sl = max.sl; else m.sl = c.sl;
	// vytvorit nejvzdalenejsi vektor
	HOECOLOR m;
	GET_INTE(r);
	GET_INTE(g);
	GET_INTE(b);
#undef GET_INTE
	return GetDist(m,c);
}

float GetMaxDist(HOECOLOR& min, HOECOLOR& max, HOECOLOR& c)
{
	// vytvorit nejvzdalenejsi vektor
	HOECOLOR m;
	m.r = (min.r + max.r) / 2 > c.r ? max.r:min.r;
	m.g = (min.g + max.g) / 2 > c.g ? max.g:min.g;
	m.b = (min.b + max.b) / 2 > c.b ? max.b:min.b;
	return GetDist(m,c);
}

float FindMax(HOECOLOR& min, HOECOLOR&max, HOECOLOR* cls, int num_cls, byte refalpha)
{
	float m = 100000.f;
	for (int i=0;i < num_cls;i++)
	{
		if (cls[i].a < refalpha) continue;
		float d = GetMaxDist(min, max, cls[i]);
		if (d < m) m = d;
	}
	return m;
}

float FindMin(HOECOLOR& min, HOECOLOR&max, HOECOLOR* cls, int num_cls, byte refalpha)
{
	float m = 100000.f;
	for (int i=0;i < num_cls;i++)
	{
		if (cls[i].a < refalpha) continue;
		float d = GetMinDist(min, max, cls[i]);
		if (d < m) m = d;
	}
	return m;
}

bool FindDiff(Reli& r, Reli& bst, HOECOLOR* cls, int num_cls, byte refalpha)
{
	if (r.destmin > bst.destmin) // best
		bst = r;
	if (r.destmax < bst.destmin) // optimize
		return false;
	if (r.min.r >= r.max.r || r.min.g >= r.max.g || r.min.b >= r.max.b) // end
		return false;

	// zkusit pro kazdou krychlu
	Reli t;
#define RECUR(red, rb, green, gb, blue, bb) \
				t = r; \
				t.red.r = (r.min.r + r.max.r) / 2 + rb; \
				t.green.g = (r.min.g + r.max.g) / 2 + gb; \
				t.blue.b = (r.min.b + r.max.b) / 2 + bb; \
				t.destmin = FindMin(t.min, t.max, cls, num_cls, refalpha); \
				t.destmax = FindMax(t.min, t.max, cls, num_cls, refalpha); \
				FindDiff(t, bst, cls, num_cls, refalpha);
	RECUR(min,1,min,1,min,1);
	RECUR(min,1,min,1,max,0);
	RECUR(min,1,max,0,min,1);
	RECUR(min,1,max,0,max,0);
	RECUR(max,0,min,1,min,1);
	RECUR(max,0,min,1,max,0);
	RECUR(max,0,max,0,min,1);
	RECUR(max,0,max,0,max,0);
#undef RECUR
	return true;
}

float FindDiffestColor(HOECOLOR& c, HOECOLOR* cls, int num_cls, byte refalpha)
{
	Reli r;
	r.max.r = 0xff;
	r.max.g = 0xff;
	r.max.b = 0xff;
	r.min.r = 0;
	r.min.g = 0;
	r.min.b = 0;
	r.destmin = 0; // vysplhat co nejvice nahoru
	r.destmax = FindMax(r.min, r.max, cls, num_cls, refalpha);
	Reli bst = r;
	FindDiff(r, bst, cls, num_cls, refalpha);
	c = bst.max;
	return 0.f;
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
		// find
		// jak najit nejvzdaleneji hodnotu?
		// pro kazdou barvu zvlast....

		// upravit paletu
		FindDiffestColor(key, m_colors, np, nref);
		for (uint n=0;n < np;n++)
		{
			if (m_colors[n].a < nref)
				m_colors[n] = key;
		}

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
	for (uint x=0;x < w;x++)
	{
		register byte r,g,b,a;
		if (m_inputformat == HOE_P8)
		{
			r = m_colors[*f].r & 0xF8;
			g = m_colors[*f].g & 0xF8;
			b = m_colors[*f].b & 0xF8;
			a = m_colors[*f].a & 0x80;  f++;
		}
		else if (m_inputformat == HOE_R8G8B8)
		{
			r = f[0] & 0xF8;g = f[1] & 0xF8;b = f[2] & 0xF8;
			a = 0x80;
			f += 3;
		}
		else
			hoe_assert(!"Format not found");
		const short c = a << 8 | b >> 3 | g << 2 | r << 7;
		t[1] = c >> 8;
		t[0] = 0xFF & c;
		t += 2;
	}
	return w;
}




