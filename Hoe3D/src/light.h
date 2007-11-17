
#ifndef _LIGHT_H_
#define _LIGHT_H_

class DebugObject
{
public:
	void Render();
};

class HoeLight : public IHoeLight
{
#ifdef _HOE_OPENGL_
	HoeMath::Vector4f pos;
	HoeMath::Vector4f color;
#else
	D3DLight light;
#endif
	DebugObject obj;
	bool m_diRECT;
public:
	HoeLight(bool diRECT);
	void Set(int slot);
	virtual void HOEAPI SetPosition(const HoeMath::Vector3v &pos);
	virtual void HOEAPI SetColor(const vfloat r, const vfloat g, const vfloat b);
	const HoeMath::Vector4f GetPosition() const;
	const HoeMath::Vector4f GetColor() const;
	// 
	HoeLight * next;
};

#endif // _LIGHT_H_


