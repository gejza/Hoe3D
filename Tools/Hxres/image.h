
#ifndef _HXRES_IMAGE_H_
#define _HXRES_IMAGE_H_

#define GET_FUNC(b, gv, v) { if (b) { if (gv) *gv = v;return true; } else return false; }
#define SET_FUNC(b, gv, v) { b = true; v = gv; }

class Image
{
protected:
	HoeCore::String m_filename;
	HoeCore::File m_f;
	bool m_hascolorkey;
	HOECOLOR m_colorkey;
	bool m_hasalpha;
	byte m_alpharef;
	byte * m_data;
	dword m_pitch;
	HoeRes::PicCodec* m_codec; 
	HoeRes::PicCodec* GetDecoder();
public:
	Image(void);
	virtual ~Image(void);
	void DeleteTmpData();
	void SetSource(const HoeCore::CString& str);
	void Save(HoeCore::WriteStream& stream);
	uint32 GetFormat();
	byte * GetData(dword* pitch);

	bool HasColorKey(HOECOLOR* ck) GET_FUNC(m_hascolorkey, ck, m_colorkey)
	bool HasAlphaRef(byte* ar) GET_FUNC(m_hasalpha, ar, m_alpharef)
	void SetColorKey(HOECOLOR& ck) SET_FUNC(m_hascolorkey, ck, m_colorkey)
	void SetAlphaRef(byte ar) SET_FUNC(m_hasalpha, ar, m_alpharef)

	// functions
	bool ComputeColorKey();

};

#

#endif // _HXRES_IMAGE_H_

