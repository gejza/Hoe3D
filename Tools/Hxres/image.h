
#ifndef _HXRES_IMAGE_H_
#define _HXRES_IMAGE_H_

class Image
{
	HoeCore::String m_filename;
	HoeCore::File m_f;
public:
	Image(void);
	virtual ~Image(void);
	void SetSource(const HoeCore::CString& str);
	void Save(HoeCore::WriteStream& stream);
};

#endif // _HXRES_IMAGE_H_

