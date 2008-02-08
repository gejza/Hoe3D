
#include "StdAfx.h"
#include "image.h"
#include "error.h"

Image::Image(void) : m_hasalpha(false), m_hascolorkey(false), 
	m_codec(NULL), m_data(NULL)
{
	/*HOECOLOR m_colorkey;
	bool m_hasalpha;
	byte m_alpharef;*/
}

Image::~Image(void)
{
}

void Image::DeleteTmpData()
{
	if (m_codec)
		m_codec->Close();
	m_codec = NULL;
	if (m_data)
		delete [] m_data;
	m_data = NULL;
}

void Image::SetSource(const HoeCore::CString& str)
{
	m_filename = str;
	if (!m_f.Open(m_filename))
	{
		throw Error(T("Failed open file %s"),m_filename.GetPtr());
	}
}

void Image::Save(HoeCore::WriteStream &stream)
{
	if (!m_f.Open())
		throw Error(T("No file specified."));

	m_f.Reset();
	stream.Write(m_f);
}

uint32 Image::GetFormat()
{
	if (m_filename.wmatch(T("*.jpg")))
		return MAKE_FOURCC('J','P','E','G');
	if (m_filename.wmatch(T("*.png")))
		return MAKE_FOURCC('P','N','G',' ');
	throw Error(T("Codec for file %s not found."), m_filename.GetPtr());
}

HoeRes::PicCodec* Image::GetDecoder()
{
	if (m_codec) return m_codec;
	if (!m_f.Open())
		throw Error(T("No file specified."));

	m_f.Reset();
	m_codec = HoeRes::CreatePicDecoder(m_f, this->GetFormat());
	return m_codec;
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

bool Image::ComputeColorKey()
{
	HoeRes::PicCodec* pc = GetDecoder();
	if (!pc) return false;
	
	// compute color key
	HOECOLOR colors[256];
	uint np = m_codec->GetPalette(colors);
	// najit referencni hodnotu pro alfu a volnou barvu
	// upravit paletu
	FindDiffestColor(m_colorkey, colors, np, 0x01);
	m_hascolorkey = true;

	return true;
}



