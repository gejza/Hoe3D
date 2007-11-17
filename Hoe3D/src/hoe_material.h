
#ifndef _HOE_MATERIAL_
#define _HOE_MATERIAL_

#include "../include/hfmt/hmodel_file.h"

struct HoeMaterialColor
{
	float r,g,b,a;

	HoeMaterialColor();
	HoeMaterialColor(float _r, float _g, float _b, float _a);
#ifdef _HOE_OPENGL_
	inline operator GLfloat * () const
	{
		return (GLfloat *)this;
	}
#endif
#ifdef _HOE_D3D_
	inline operator _D3DCOLORVALUE& () const
	{
		return *((_D3DCOLORVALUE *)this);
	}
#endif
	const HoeMaterialColor & operator = (dword c)
	{
		b = float(c & 0xff) * (1/255.f);c = c >> 8;
		g = float(c & 0xff) * (1/255.f);c = c >> 8;
		r = float(c & 0xff) * (1/255.f);c = c >> 8;
		a = float(c & 0xff) * (1/255.f);c = c >> 8;
		return *this;
	}
};

class HoeMaterial
{
	class HoeTexture * m_tex;
	class HoeTexture * m_bump;
	bool m_lightreag;
#ifdef _HOE_D3D_
	D3DMaterial m_mtrl;
#endif
#ifdef _HOE_OPENGL_
	HoeMaterialColor m_ambient;
	HoeMaterialColor m_diffuse;
	HoeMaterialColor m_specular;
#endif
public:
	//TODO:
	HoeMath::Matrix4v m_over;
	bool m_overlap;


	static const int Ambient = 0x01;
	static const int Diffuse = 0x02;
	static const int Specular = 0x04;

	HoeMaterial();
	void Setup(dword overcolor);
	const char * GetName();
	bool LoadFromFile(const char *path);
	void SetTexture(HoeTexture * t) { m_tex = t; }
	void SetColor(int type, const HoeMaterialColor & color);
	void SetLightReag(bool lr) { m_lightreag = lr; }
		
	friend class MaterialSystem;
};

#endif // _HOE_MATERIAL_

