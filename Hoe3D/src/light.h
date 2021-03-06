
#ifndef _LIGHT_H_
#define _LIGHT_H_

class DebugObject
{
public:
	void Render();
};

class HoeLight : public IHoeLight
{
#ifdef _HOE_D3D_
	D3DLight light;
#endif
#ifdef _HOE_OPENGL_
	HoeMath::Vector4 pos;
	HoeMath::Vector4 color;
#endif
	DebugObject obj;
	bool m_diRECT;
public:
	HoeLight(bool diRECT);
	void Set(int slot);
	virtual void HOEAPI SetPosition(const HoeMath::Vector3 &pos);
	virtual void HOEAPI SetColor(const float r, const float g, const float b);
	const HoeMath::Vector4 GetPosition() const;
	const HoeMath::Vector4 GetColor() const;
	// 
	HoeLight * next;
};

#endif // _LIGHT_H_


