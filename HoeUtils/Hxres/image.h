
#ifndef _HXRES_IMAGE_H_
#define _HXRES_IMAGE_H_

class Image
{
	HoeCore::String m_filename;
public:
	Image(void);
	virtual ~Image(void);
	void SetSource(const HoeCore::CString& str);
};

#endif // _HXRES_IMAGE_H_

